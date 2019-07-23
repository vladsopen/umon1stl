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
#include "design.h"
#include "drawall.h"
#include "drawback.h"
#include "waves.h"

DrawBack::DrawBack(DrawAll* drawAll) :
    Context(drawAll->io()),
    drawAll(drawAll)
{
}

void DrawBack::paintBack(
        Paint& paint)
{
    Size size = paint.size();

    paint.brush(Design::colors::background());

    paint.drawBar(Point(0, 0), size);
}

void DrawBack::paintGrid(
        Paint& paint)
{
    Size size = paint.size();

    for (int iHalfCm : FromTo(0, 2))
    {
        int offset = iHalfCm * grid::pxPerCmY / 2;

        paint.pen(1, iHalfCm == 0? Design::colors::strongGrid() : Design::colors::weakGrid());

        // horizontal
        for (int i : FromTo(0, div0(size.h(), /, grid::pxPerCmY, or 0) + 1))
        {
            int y = i * grid::pxPerCmY + offset;
            paint.drawLine(
                Point(0, y),
                Point(size.w(), y));
        }

        // vertical
        for (int i : FromTo(0, div0(size.w(), /, grid::pxPerCmY, or 0) + 1))
        {
            int x = i == 0? offset : i * grid::pxPerCmX - 1 + offset;
            paint.drawLine(
                Point(x, 0),
                Point(x, size.h()));
        }
    }
}

