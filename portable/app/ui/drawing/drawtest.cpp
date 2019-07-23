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
#include "application.h"
#include "drawall.h"
#include "drawtest.h"
#include "waves.h"

DrawTest::DrawTest(DrawAll* drawAll) :
    Context(drawAll->io()),
    drawAll(drawAll)
{
}

void DrawTest::paintTest(
        Paint& paint)
{
    if (!Io::Os::debug())
        return;

    Size size = paint.size();
    if (size.w() <= 0 || size.h() <= 0)
        return;

    paint.pen(1, Rgb(255, 255, 0));

    // TEST: draw some garbage
    static int animate = 0;
    ++animate;
    for (int i = 0; i < 100; ++i)
        paint.
            drawLine(
                Point(size.w() / 2, size.h() / 2),
                Point(
                    (static_cast<int>(time(nullptr)) * i * animate) % (size.w() | 1),
                    (animate + i * i) % (size.h() | 1)));
}

