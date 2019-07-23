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
#include "emjs.h"
#include "useful.h"
#include "empaint.h"

EM_JS(void, js_addToHtml, (
        const char* cc_tag,
        uiid idNew,
        uiid idParent,
        const char* cc_text)
, {
    let text = Pointer_stringify(cc_text);
    let tag = Pointer_stringify(cc_tag);

    let parent = document.getElementById(idParent);

    let tagEmit = tag;
    if (tag == "br")
        tagEmit = "span";

    add = "";

    //
    // outer box controls layout for each element
    //

    add += "<div id='box" + idNew + "' ";

        add += " style=' ";

            //HACK:_spacing_
            //add += "padding: 2px; ";

            if (tag == "br")
                add += 
                    "flex-basis: 100%; " +
                    // flex-wrap: wrap;
                    "";
            else
                add += "position: relative; "; 
                    //"flex-grow: 1; "; -- can be set with inflateX()

        add += "touch-action: pan-y; ";

        //if (tag == "div")
        //    add += " width: 300px; height: 300px; ";

        add += "'"; // style=

        add += ">"; // style=

        //
        // the main element tag is inside the layout box
        //

        add += "<" + tagEmit;

            if (idNew != 0)
                add += " id=" + idNew;

            add += " style='";

            add += "width: 100%; height: 100%; "; 

            if (tag == "div")
                add += 
                    "display: flex; " +
                    "flex-wrap: wrap; " +
                    "align-items: center; " + // height-centering
                    //"justify-content: center; " + // causes X-center
                    //"text-align: center; " + // no effect
                    ""; 

            add += "'"; // style=

            add +=
                " onclick=\"" +
                    "Module.proxyClick_" + tag + "(" + idNew + ");" +
                    "Module.repaintDebugCanvas(" + idNew + "); " +
                "\"";

            add += ">";

            add += text;

        add += "</" + tagEmit + ">";

        //
        // and most controls support onPaint canvas 
        //   which we overlay on top of them
        //

        if (tag != "span")
        {
            add += " \
                <canvas id='myCanvas" + idNew + "' \
                    style=' \
                        top: 0; \
                        left: 0; \
                        position: absolute; \
                        width: 100%; \
                        height: 100%; \
                        /* border: 4px solid #239cab; */ \
                        pointer-events: none; \
                        ' \
                    onclick='' \
                    ></canvas> \
                ";
        }

    add += "</div>"; // the layout box

    parent.innerHTML += add;

    // now innerHTML may not create all internal tags 
    //   so we have to delay further init until the message
    //   loop is run:

    setTimeout(function ()
        {
            let touch = document.getElementById(idNew);
            if (touch) // && idParent != 1)
            {
                function onPointer(handler, ev)
                {
                    Module.onPointerEvent(
                        idNew,
                        handler.charCodeAt(0),
                        ev.pointerType.charCodeAt(0),
                        ev.pointerId,
                        ev.button,
                        ev.clientX,
                        ev.clientY,
                        ev.shiftKey,
                        ev.ctrlKey,
                        ev.altKey);
                }

                const handlers = 
                    new Map([
                        ["d", "pointerdown"], 
                        ["u", "pointerup"], 
                        ["o", "pointerout"],
                        ["m", "pointermove"],
                        ]);
                for (let [h, handler] of handlers)
                {
                    touch.addEventListener(handler, 
                        ev => { onPointer(h, ev); });
                }
            }
        },
        1);
})

void EmJs::createControl(
        const char* cc_tag,
        uiid idNew,
        uiid idParent,
        const char* cc_text)
{
    rASSERT(idNew >= 0); // can be 0 for <br>
    rASSERT(idParent > 0);
    rASSERT(cc_tag[0] != 0);

    js_addToHtml(
        cc_tag,
        idNew,
        idParent,
        cc_text);
}


/*
EM_JS(void, js_setTagAttribute, (
      uiid id, 
      const char* subelement, 
      const char* attribute, 
      const char* value)
, {
    document.
        getElementById(UTF8ToString(subelement) + id).
        setAttribute(UTF8ToString(attribute), UTF8ToString(value));
});
*/

void EmJs::setTagAttributeToInt(
      uiid id, 
      const char* subelement, 
      const char* attribute, 
      const char* value)
{
    rASSERT(id > 0);
    //rASSERT(subelement[0] != 0);
    rASSERT(attribute[0] != 0);
    rASSERT(value[0] != 0);

    emscripten_run_script(
        (std::string() + 
            "document." 
                "getElementById(\"" + subelement + "\" + " + std::to_string(id) + ")" +
                "[\"" + attribute + "\"] = " + value + ""
        ).c_str());
}

void EmJs::setStyleAttributeToString(
      uiid id, 
      const char* subelement, 
      const char* attribute, 
      const char* value)
{
    rASSERT(id > 0);
    //rASSERT(subelement[0] != 0);
    rASSERT(attribute[0] != 0);
    rASSERT(value[0] != 0);

    emscripten_run_script(
        (std::string() + 
            "document." 
                "getElementById(\"" + subelement + "\" + " + std::to_string(id) + ")" +
                ".style[\"" + attribute + "\"] = \"" + value + "\""
        ).c_str());
}

EM_JS(void, js_replaceInnerHtml, (
        uiid id,
        const char* cc_text)
, {
    document.getElementById(id).innerHTML = Pointer_stringify(cc_text);
})

void EmJs::replaceInnerHtml(
        uiid id,
        const char* cc_text)
{
    rASSERT(id > 0);
    //rASSERT(cc_text[0] != 0);

    js_replaceInnerHtml(id, cc_text);
}

void EmJs::onPointerEvent(
        int id,
        char eventName,
        char deviceType,
        int pointerId,
        int buttonIndex,
        int clientX,
        int clientY,
        bool withShift,
        bool withCtrl,
        bool withAlt)
{
    if (eventName != 'm') // "pointermove"
    {
        std::cout << "Pointer event " << eventName << " for " << id << " "
            << "from " << deviceType << "[" << pointerId << "]." 
            << buttonIndex << " (" << clientX << ", " << clientY << ") shift=" << withShift
            << "\n";
    }

    UiBasic* ui = reinterpret_cast<UiBasic*>(id);

    auto ev = std::make_shared<PointerEvent>();

    PointerEvent::What what = ev->what();
    if (eventName == 'd') // "pointerdown"
        what = PointerEvent::pointerDown;
    else
    if (eventName == 'u') // "pointerup"
        what = PointerEvent::pointerUp;
    else
    if (eventName == 'o') // "pointerout"
        what = PointerEvent::pointerUp;
    else
    if (eventName == 'm') // "pointermove"
        what = PointerEvent::pointerMove;
    else
        rFAIL("unknown pointer event name");
    ev->what(what);

    PointerEvent::Device device = ev->device();
    if (deviceType == 'm') // "mouse"
        device = PointerEvent::mouse;
    else
    if (deviceType == 't') // "touch"
        device = PointerEvent::touch;
    else
    if (deviceType == 'p') // "pen"
        device = PointerEvent::pen;
    else
        rFAIL("unknown pointer event device");
    ev->device(device);

    ev->pointerId(pointerId);

    ev->buttonIndex(buttonIndex);

    ev->clientX(clientX);
    ev->clientY(clientY);

    ev->withShift(withShift);
    ev->withAlt(withAlt);
    ev->withCtrl(withCtrl);

    ui->invokePointer(ev);

    //std::cout << "\n";
}

