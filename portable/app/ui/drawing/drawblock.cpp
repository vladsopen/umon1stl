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
#include "drawwave.h"
#include "drawblock.h"
#include "waves.h"
#include "view.h"

DrawBlock::DrawBlock(DrawAll* drawAll) :
    Context(drawAll->io()),
    drawAll(drawAll)
{
}

const View* DrawBlock::view() const
{
    return drawAll()->view();
}

void DrawBlock::paintLastBlock(
        Paint& paint,
        int x,
        int y,
        int height,
        int pointsRecorded,
        int pointsLastBlock,
        As<bool, class UnstableState> unstable)
{
    UNUSED(pointsRecorded);

    Size size = paint.size();

    //
    // |======================####| m_pointsRecorded
    //       | m_scroll       LAST block
    //       \_________________/
    //         pointsPerScreen

    int pointsPerScreen = view()->pointsPerScreen();

    int blockWidth = useful::mulDiv(pointsLastBlock, size.w(), pointsPerScreen);
    int pointStartBlock = x;

    int margin = 2;

    Rgb color = Design::colors::normalCurrentBox(); // normal yellow
    if (unstable)
    {
        if (time(nullptr) & 1)
            color = Design::colors::unstableCurrentBox(); // orange warning
    }

    paint.pen(1, Design::colors::normalCurrentBox());
    paint.brush(color);

    Point from(pointStartBlock, margin + y);
    Size extent(blockWidth, height - margin * 2);
    paint.drawBar(from, extent);

    LOG(drawDetails, "DrawBlock",
            "(" << from.x() << ", " << from.y() << ") "
                << "[" << extent.w() << " x " << extent.h() << "]"
                << (unstable? " (unstable)" : ""));
}

void DrawBlock::paintLastBlockBefore(
        Paint& paint,
        int pointsRecorded,
        int pointsLastBlock)
{
    if (!view()->showScroll())
        return;

    Size size = paint.size();

    //
    // |======================####| pointsRecorded
    //       | m_scroll       LAST block
    //       \_________________/
    //         pointsPerScreen

    int pointsPerScreen = view()->pointsPerScreen();

    int pointStartBlock = pointsRecorded - pointsLastBlock - view()->pointsScrollInt();
    int xStartBlock = useful::mulDiv(pointStartBlock, size.w(), pointsPerScreen);

    paintLastBlock(
        paint,
        xStartBlock,
        0,
        div0(size.h(), /, (view()->showScroll() + view()->showSweep()), or 0),
        pointsRecorded,
        pointsLastBlock,
        As<bool, class UnstableState>(false));
}

void DrawBlock::paintLastBlockAfter(
        Paint& paint,
        DrawWave* drawWave,
        int pointsRecorded,
        int pointsLastBlock)
{
    if (!view()->showSweep())
        return;

    Size size = paint.size();

    int pointsPerScreen = view()->pointsPerScreen();

    int pointsDelay = pointsRecorded - drawWave->pointNewestDrawn();
    int pointsStartBlock = pointsDelay - pointsLastBlock;
    int xShiftStartBlock = useful::mulDiv(pointsStartBlock, size.w(), pointsPerScreen);

    paintLastBlock(
        paint,
        drawWave->xSweep() + xShiftStartBlock,
        size.h() - 15,
        size.h(),
        pointsRecorded,
        pointsLastBlock,
        As<bool, class UnstableState>(drawWave->adjustingSweep()));
}
