/**
 * MIT license:
 * 
 * Copyright (c) 2019 vlads
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "common.h"
#include "emglobals.h"
#include "emui.h"
#include "emjs.h"
#include "useful.h"
#include "empaint.h"

EmGlobals::EmGlobals()
{
    EM_ASM_({
        setInterval(Module.onGlobalTimer, 1000); // function() { alert("T!");
    });
}

EmGlobals& EmGlobals::get()
{
    static EmGlobals s_global;
    return s_global;
}

void EmGlobals::invalidate(uiid id)
{
    m_updateList.push_back(id);
}

void onGlobalTimer()
{
    //EmGlobals::emGlobals.updateAll();
}

/*
void EmGlobals::updateAll()
{
    // make iteration safe
    auto copyList = m_updateList;
    m_updateList.clear();

    // flush pending updates
    for (uiid id : copyList)
    {
        //repaintDebugCanvas(id);

        UiPanel* pPanel = reinterpret_cast<UiPanel*>(id);
        if (!pPanel->destroyed())
            pPanel->onPaint();
    }
}
*/

void EmGlobals::initElement(
        UiBasic* ui,
        const std::string& tag)
{
    bool isMain = ui->createUi().frameFlags() & CreateUi::mainWindow;

    uiid idParent =
        isMain?
            EmJs::predefined::mainWindowParentId
            :
            reinterpret_cast<size_t>(ui->createUi().parentOrNull());

    // clear the predefined HTML placeholder for mainwindow
    //   only on the very first call
    static bool s_first = true;
    if (isMain)
    {
        if (s_first)
        {
            s_first = false;

            EmJs::replaceInnerHtml(
                idParent,
                "");
        }
    }
    else
        rASSERT(!s_first);

    //std::cout << "--- Parent: " << idParent << "\n";

    EmJs::createControl(
        tag.c_str(),
        reinterpret_cast<size_t>(ui),
        idParent,
        Io::Os::debug()?
            ("(no text " + tag + (isMain? " [main]" : "") + ")").c_str()
            :
            "");
}

std::string Io::Os::unicode(int code)
{
    rASSERT(code > 0 && code < 0xffff);

    return "&#" + std::to_string(code) + ";";
}

bool Io::Os::debug()
{
    return false;
    //return true;
}

void Io::Os::say(const std::string& output)
{
    std::cout << output.c_str();
}

void Io::Os::alert(const std::string& output)
{
    EM_ASM_({
        alert(Pointer_stringify($0));
    },
    output.c_str());
}

void Io::Os::yeldToEventLoopForTestingIfSupported()
{
    // not supported with JS
}

bool Io::Os::canYeldToEventLoop()
{
    return false;
}

void Io::Os::diffTwoFiles(
        const std::string& originalFile,
        const std::string& changedFile)
{
    UNUSED(originalFile);
    UNUSED(changedFile);

    rFAIL("diff is not supported yet with JS");
}

bool Io::Os::testLogsAvailable()
{
    return false;
}


void proxyClick_div(int id)
{
    std::cout << "Click div " << id << "\n";
}

void proxyClick_span(int id)
{
    std::cout << "Click span " << id << "\n";
}

void proxyClick_button(int id)
{
    std::cout << "Click button " << id << " ";

    UiButton* pButton = reinterpret_cast<UiButton*>(id);

    //pButton->onClick();
    pButton->invokeClick();

    std::cout << "\n";
}

void proxyTimer(uiid id)
{
    UiBasic* ui = reinterpret_cast<UiBasic*>(id);

    if (!ui->destroyed())
    {
        ui->invokeTimer();
    }
}

EMSCRIPTEN_BINDINGS(my_module)
{
    emscripten::function("proxyClick_button", &proxyClick_button);
    emscripten::function("proxyClick_div", &proxyClick_div);
    emscripten::function("proxyClick_span", &proxyClick_span);
    emscripten::function("onPointerEvent", &EmJs::onPointerEvent);
    emscripten::function("proxyTimer", &proxyTimer);
    emscripten::function("repaintDebugCanvas", &repaintDebugCanvas);
    emscripten::function("onGlobalTimer", onGlobalTimer);
}
