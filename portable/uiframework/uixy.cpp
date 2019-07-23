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
#include "uixy.h"

Point::Point()
{
}

Point::Point(const Point& point)
    : Object()
{
    *this = point;
}

Point::Point(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Point::operator = (const Point& point)
{
    m_x = point.m_x;
    m_y = point.m_y;
}

bool Point::operator == (const Point& point) const
{
    return m_x == point.m_x && m_y == point.m_y;
}

bool Point::operator != (const Point& point) const
{
    return m_x != point.m_x || m_y != point.m_y;
}

int Point::x() const
{
    return m_x;
}

int Point::y() const
{
    return m_y;
}

Point& Point::x(int x)
{
    m_x = x;
    return *this;
}

Point& Point::y(int y)
{
    m_y = y;
    return *this;
}

Size::Size()
{
}

Size::Size(const Size& size)
    : Object()
{
    *this = size;
}

Size::Size(int w, int h)
{
    m_w = w;
    m_h = h;
}

void Size::operator = (const Size& size)
{
    m_w = size.m_w;
    m_h = size.m_h;
}

bool Size::operator == (const Size& size) const
{
    return m_w == size.m_w && m_h == size.m_h;
}

bool Size::operator != (const Size& size) const
{
    return m_w != size.m_w || m_h != size.m_h;
}

int Size::w() const
{
    return m_w;
}

int Size::h() const
{
    return m_h;
}

Size& Size::w(int w)
{
    m_w = w;
    return *this;
}

Size& Size::h(int h)
{
    m_h = h;
    return *this;
}
