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
#include "emui.h"
#include "emjs.h"
#include "emglobals.h"
#include "useful.h"
#include "empaint.h"

UiBasic::~UiBasic()
{
}

void UiBasic::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiBasic::impl_afterUiDefined()
{
}

void UiBasic::impl_width(int px)
{
    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "box",
        "width",
        (std::to_string(px) + "px").c_str());

    //driver()->setMinimumWidth(px);
}

void UiBasic::impl_height(int px)
{
    //driver()->setMinimumHeight(px);

    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "box",
        "height",
        (std::to_string(px) + "px").c_str());
}

void UiBasic::impl_niceX()
{
    //TODO:
}

void UiBasic::impl_niceY()
{
    //TODO:
}

void UiBasic::impl_stretchX()
{
    //TODO:
}

void UiBasic::impl_stretchY()
{
    //TODO:
}

void UiBasic::impl_inflateX()
{
    //TODO:

    // add += "position: relative; flex-grow: 1; ";
    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "box",
        "flex-grow",
        "1");
}

void UiBasic::impl_inflateY()
{
    //TODO:
}

void UiBasic::impl_wrap(bool yes)
{
    UNUSED(yes);

    //TODO: need to switch to Flow QT layout
}

void UiBasic::impl_disable(const std::string& why)
{
    UNUSED(why);

    EmJs::setTagAttributeToInt(
        reinterpret_cast<size_t>(this),
        "",
        "disabled",
        isDisabled()? "1" : "0");
}

void UiBasic::impl_timer(int milliseconds)
{
    rASSERT(milliseconds > 0 && milliseconds < 1 * 1000 * 1000); // just for sanity

    EM_ASM_({
        if (typeof Module.timerHandles === 'undefined')
            Module.timerHandles = {};

        Module.timerHandles[$0] = setInterval(function() { Module.proxyTimer($0); }, $1); 

        //function step(time) { Module.proxyTimer($0); window.requestAnimationFrame(step); }
        //window.requestAnimationFrame(step); 
    },
    reinterpret_cast<size_t>(this),
    milliseconds);
}

void UiBasic::impl_timerOff()
{
    EM_ASM_({
        if (typeof Module.timerHandles !== 'undefined' &&
            Module.timerHandles[$0])
        {
            clearInterval(Module.timerHandles[$0]); 
            Module.timerHandles[$0] = 0;
        }
    },
    reinterpret_cast<size_t>(this));
}

void UiBasic::impl_repaint()
{
    //repaintDebugCanvas(reinterpret_cast<size_t>(this));

    if (onPaint.valid())
    {
        Paint paint(this);

        invokePaint(paint);
    }
}

DriveBasic* UiPanel::newDriver()
{
    return new EmPanel(this);
}

void UiPanel::impl_destroy()
{
    EmJs::replaceInnerHtml(
        reinterpret_cast<size_t>(this),
        "[Panel Closed]");
}

EmPanel::EmPanel(UiPanel* ui) :
    ui(ui)
{
    EmGlobals::get().initElement(ui, "div");

    //repaintDebugCanvas(reinterpret_cast<size_t>(ui));
    EmGlobals::get().invalidate(reinterpret_cast<size_t>(ui));
}

void UiPanel::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiPanel::impl_afterUiDefined()
{
    impl_spacingX(m_spacing.w());
    impl_spacingY(m_spacing.h());
}

void UiPanel::applySpacingToChildren(
        const std::string& side1,
        const std::string& side2,
        int px)
{
    auto pxString = std::to_string(px) + "px";

    for (auto child : m_children)
    {
        EmJs::setStyleAttributeToString(
            reinterpret_cast<size_t>(child.get()),
            "box",
            side1.c_str(),
            pxString.c_str());

        EmJs::setStyleAttributeToString(
            reinterpret_cast<size_t>(child.get()),
            "box",
            side2.c_str(),
            pxString.c_str());
    }
}

void UiPanel::impl_spacingX(int px)
{
    //UNUSED(px);

    applySpacingToChildren(
        "padding-left",
        "padding-right",
        px);
}

void UiPanel::impl_spacingY(int px)
{
    //UNUSED(px);

    applySpacingToChildren(
        "padding-top",
        "padding-bottom",
        px);

    /* debug
    if (px == 2)
    for (auto child : m_children)
    {
        EmJs::setStyleAttributeToString(
            reinterpret_cast<size_t>(child.get()),
            "",
            "background-color",
            "#ff0000");
    } */
}

void UiPanel::impl_br()
{
    EmJs::createControl(
        "br",
        0, // idNew
        reinterpret_cast<size_t>(this),
        Io::Os::debug()?
            "(br)"
            :
            "");
}

void UiTextBased::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);

    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "",
        "font-size",
        "18px");
    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "",
        "font-family",
        "Verdana, sans-serif");
    EmJs::setStyleAttributeToString(
        reinterpret_cast<size_t>(this),
        "",
        "padding",
        "5px");
}

void UiTextBased::impl_afterText(const std::string& value)
{
    auto clean = useful::replaceAll(value, "\n", "<br />");

    if (Io::Os::debug())
        EmJs::createControl(
            "span",
            0, // idNew
            reinterpret_cast<size_t>(this),
            clean.c_str());
    else
        EmJs::replaceInnerHtml(
            reinterpret_cast<size_t>(this),
            clean.c_str());
}

void UiTextBased::impl_afterUiDefined()
{
}

DriveBasic* UiButton::newDriver()
{
    return new EmButton(this);
}

EmButton::EmButton(UiButton* ui) :
    ui(ui)
{
    EmGlobals::get().initElement(ui, "button");
}

void UiButton::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiButton::impl_afterUiDefined()
{
}

void UiButton::impl_afterText(const std::string& value)
{
    UiTextBased::impl_afterText(value);
}

DriveBasic* UiStatic::newDriver()
{
    return new EmStatic(this);
}

EmStatic::EmStatic(UiStatic* ui) :
    ui(ui)
{
    EmGlobals::get().initElement(ui, "span");
}

void UiStatic::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiStatic::impl_afterUiDefined()
{
}

void UiStatic::impl_afterText(const std::string& value)
{
    UiTextBased::impl_afterText(value);
}
