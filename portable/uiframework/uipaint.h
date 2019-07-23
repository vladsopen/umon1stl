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
#ifndef UIPAINT_H
#define UIPAINT_H

#include "object.h"
#include "uirgb.h"
#include "uixy.h"

class UiBasic;
class UiPanel;

// Portable painter context

class Paint : public Object
{
public:

    explicit Paint(UiBasic* ui);
    virtual ~Paint();

    Size size() const;

    void pen(int thickness, Rgb color);
    Rgb penColor() const;
    int penThickness() const;

    void brush(Rgb color);
    Rgb brushColor() const;

    void drawLine(Point from, Point to);
    void drawBar(Point origin, Size extent);

private:

    // element/widget selection
    const uiid m_id;

#if defined(__EMSCRIPTEN__)

#elif defined(WITH_QT)

    std::unique_ptr<QPainter> m_qpainter;

#elif defined(WITH_UV)

#else

    #error unknown compilation mode

#endif

    // current line pen
    int m_penThickness = 1;

    // current fill brush
    Rgb m_penRgb;
    Rgb m_brushRgb;

};

#endif
