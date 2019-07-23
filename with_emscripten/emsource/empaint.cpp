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
#include "empaint.h"

EM_JS(void, js_prepareCanvasContext, (
      uiid id)
, {
    var canvas = document.getElementById("myCanvas" + id);
    var ctx = canvas.getContext("2d");

    // clear canvas
    canvas.width = canvas.offsetWidth;
    canvas.height = canvas.offsetHeight;

    return ctx;
});

EM_JS(int, js_canvas_getWidth, (
      uiid id)
, {
    var canvas = document.getElementById("myCanvas" + id);
    return canvas.offsetWidth;
});

EM_JS(int, js_canvas_getHeight, (
      uiid id)
, {
    var canvas = document.getElementById("myCanvas" + id);
    return canvas.offsetHeight;
});

EM_JS(void, js_canvas_drawLine, (
      uiid id, 
      int xFrom,
      int yFrom,
      int xTo,
      int yTo,
      int thickness, 
      const char* color)
, {
    var canvas = document.getElementById("myCanvas" + id);
    var ctx = canvas.getContext("2d");

    ctx.beginPath();

    ctx.lineWidth = thickness;
    ctx.strokeStyle = "#" + UTF8ToString(color);

    ctx.moveTo(xFrom, yFrom);
    ctx.lineTo(xTo, yTo);

    ctx.stroke();
});

EM_JS(void, js_canvas_drawBar, (
      uiid id, 
      int x, 
      int y, 
      int w, 
      int h, 
      const char* color)
, {
    var canvas = document.getElementById("myCanvas" + id);
    var ctx = canvas.getContext("2d");

    ctx.beginPath();

    ctx.fillStyle = "#" + UTF8ToString(color);
    ctx.fillRect(x, y, w, h);

    ctx.stroke();
});

Paint::Paint(UiBasic* ui) :
    m_id(reinterpret_cast<size_t>(ui))
{
    js_prepareCanvasContext(m_id);
}

Paint::~Paint()
{
}

Size Paint::size() const
{
    return 
        Size(
            js_canvas_getWidth(m_id),
            js_canvas_getHeight(m_id));
}

void Paint::drawLine(Point from, Point to)
{
    js_canvas_drawLine(
        m_id, 
        from.x(), from.y(), to.x(), to.y(), 
        penThickness(), penColor().asHex().asZeroTerminated());
}

void Paint::drawBar(Point origin, Size size)
{
    js_canvas_drawBar(
        m_id, 
        origin.x(), origin.y(), size.w(), size.h(), 
        brushColor().asHex().asZeroTerminated());
}

void repaintDebugCanvas(uiid id)
{
    if (!Io::Os::debug())
        return;

    Paint paint((UiBasic*) id);

    Size size = paint.size();

    paint.pen(3, Rgb(0, 128, 255));
    paint.drawLine(Point(10, 10), Point(100, 100));

    paint.pen(1, Rgb(0, 255, 0));
    paint.drawLine(Point(0, 0), Point(size.w(), size.h()));

    paint.brush(Rgb(255, 100, 0));
    paint.drawBar(Point(100, 100), Size(20, 20));

    paint.pen(1, Rgb(0, 128, 0));
    for (int i = 0; i < 30; ++i)
    {
        paint.drawLine(
            Point(i, i), 
            Point(
                (time(nullptr) * i * i * i) % 100, 
                ((i + i + time(nullptr)) * i) % 100));
    }
}
