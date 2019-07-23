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
#ifndef UIXY_H
#define UIXY_H

#include "object.h"

// A 2D point position

class Point : public Object
{
public:
    Point();
    Point(const Point& point);
    Point(int x, int y);

    void operator = (const Point& point); // 'void' by design

    bool operator == (const Point& point) const;
    bool operator != (const Point& point) const;

    int x() const;
    int y() const;

    Point& x(int x);
    Point& y(int y);

private:
    int m_x = 0;
    int m_y = 0;
};

// A 2D distance

class Size : public Object
{
public:
    Size();
    Size(const Size& size);
    Size(int w, int h);

    void operator = (const Size& size); // 'void' by design

    bool operator == (const Size& size) const;
    bool operator != (const Size& size) const;

    int w() const;
    int h() const;

    Size& w(int w);
    Size& h(int h);

#if defined(__EMSCRIPTEN__)
#elif defined(WITH_QT)
    Size(const QSize& qsize);
#elif defined(WITH_UV)
#else
    #error unknown compilation mode
#endif

private:
    int m_w = 0;
    int m_h = 0;
};

#endif
