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
#include "wave.h"
#include "drawwave.h"
#include "drawranges.h"
#include "drawzones.h"
#include "drawpolyline.h"
#include "waves.h"
#include "view.h"
#include "storage.h"

DrawWave::DrawWave(
        const Wave* wave,
        const Waves* waves,
        const View* view) :
    Context(wave->io()),
    wave(wave),
    waves(waves),
    view(view)
{
}

void DrawWave::paint(
        Paint& paint)
{
    paintAll(paint);
}

void DrawWave::paintAll(
        Paint& paint)
{
    const Size sizePaint = paint.size();

    // initial horizontal metrics
    DrawRanges ranges(sizePaint, wave(), view());

    // initial vertical metrics
    const int height = sizePaint.h();
    const int subviews = view()->showSweep() + view()->showScroll();

    // scroll mode?
    int yTop = 0;
    if (view()->showScroll())
    {
        paintRange(
            ranges,
            false, // half
            paint,
            div0(height, /, subviews, or 0),
            yTop,
            As<bool, class FadeColors>(false));

        LOG(drawDetails, "Draw", "| scroll mode |"; ranges.logKey += logTo->added());

        yTop = div0(height, /, subviews, or 0);
    }

    if (view()->showSweep())
    {
        // left side - most recent data
        ranges.calcLeftSide(this);
        int drawnLeft =
            paintRange(
                ranges,
                true, // half
                paint,
                div0(height, /, subviews, or 0),
                yTop,
                As<bool, class FadeColors>(false));

        // right size - older data
        ranges.calcRightSide(this);
        int drawnRight =
            paintRange(
                ranges,
                true, // half
                paint,
                div0(height, /, subviews, or 0),
                yTop,
                As<bool, class FadeColors>(true));

        LOG(drawDetails, "Draw", "| sweep mode |"; ranges.logKey += logTo->added());

        UNUSED(drawnLeft);
        UNUSED(drawnRight);
    }

    if (wave()->channel() == 0)
        LOG(drawEssential, "Draw", ranges.logKey << " " << ranges.logDetails);
}

int DrawWave::paintRange(
        const DrawRanges& ranges,
        bool half,
        Paint& paint,
        int height,
        int yTop,
        As<bool, class FadeColors> fadeColors)
{
    // ---- Horizontal ----
    //   we have visible polyline and deleted zone
    bool inVisible = false;
    bool inDeleted = false;
    DrawPolyline drawPolyline(wave(), view());
    DrawZones drawZones(wave());

    // |||| Vertical ||||
    //   we split the drawing area proportionally for each channel
    int hPerChannel = div0(height, /, waves()->size(), or 0);
    int yZero = yTop + hPerChannel * wave()->channel() + hPerChannel / 2;

    //
    // color
    //

    Rgb color = Design::colors::channelPolyline(wave()->channel());
    if (fadeColors)
        color = fadeColor(color, ranges.pointsPerRange());

    // loop for all points in the screen (or half the screen)
    for (int iVisible : FromTo(0, static_cast<int>(ranges.pointsToDraw())))
    {
        // x on screen:
        int x =
            useful::iround(
                ranges.leftOffset()
                    +
                    div0(xpoints(iVisible) * ranges.widthLimited(),
                        /,
                        ranges.pointsPerRange(),
                        or 0));

        Maybe<int> iReadMaybe = calcReadIndex(iVisible, half, x, ranges);
        if (!iReadMaybe.valid())
            continue;
        int iRead = iReadMaybe.useOr(-1);

        if (iRead >= 0)
        {
            // visible signal zone:

            if (!inVisible)
            {
                drawZones.reset();

                inVisible = true;
                inDeleted = false;

                paint.pen(visuals::penThickness, color);
            }

            drawPolyline.
                paintNext(
                    paint,
                    iRead,
                    x,
                    yZero,
                    hPerChannel,
                    half);
        }
        else
        {
            // deleted zone:

            if (!inDeleted)
            {
                drawPolyline.reset();

                inVisible = false;
                inDeleted = true;

                paint.pen(5, Design::colors::cross());
            }

            drawZones.
                paintDeleted(
                    paint,
                    iRead,
                    x,
                    yZero);
        }
    }

    //m_debugCrossCount = drawZones.debugCrossCount();

    auto zonesLog = drawZones.logFinalState();
    auto polyLog = drawPolyline.logFinalState();

    LOG(drawDetails, "DrawDone",
            drawPolyline.pointsDrawn() << " segments, "
                << zonesLog << ", "
                << polyLog << ", "
                << "color: " << color.asDecimal(", "));

    return drawPolyline.pointsDrawn();
}

Maybe<int> DrawWave::calcReadIndex(
        int iVisible,
        bool half,
        int toX,
        const DrawRanges& ranges)
{
    // index in array:
    int iAt =
        useful::iround(
            ranges.scroll() + iVisible - wave()->pointsDeleted());

    // absolute recording offset:
    int iFromBeginning =
        iAt + wave()->pointsDeleted();

    // report the most advanced position drawn
    if (iFromBeginning > pointNewestDrawn() && half)
    {
        pointNewestDrawn(iFromBeginning);
        xSweep(toX);
    }

    // we draw slightly behind real time,
    //   this provides for smooth incremental sweep mode,
    //   hiding some points from the already received block,
    //   hoping we receive the next block right after we sweep
    //   the current one
    if (iFromBeginning > view()->pointsLimitByTime())
        return Nothing();

    // bounds check
    if (iAt >= wave()->pointsStored())
        return Nothing();

    return Maybe<int>(iAt);
}

Rgb DrawWave::fadeColor(
        const Rgb& colorFrom,
        xpoints pointsPerScreen)
        const
{
    // Calculate dimmer color for the right part (older side) of wave.
    // Make it fade slower using the current sweep position.

    // No deep math here.
    // Just an empirical non essential experimental visual effect
    //   only tested on white background.

    double screen = view()->pointsPerScreen();
    double sweep = screen - pointsPerScreen;
    double add = div0(100.0 * sweep, /, screen, or 0);
    add = std::min(add, 10.0);

    double fade = add / 5.0;

    Rgb color = colorFrom;

    int average = 30 + (color.red() + color.blue() + color.green()) / 3;
    color =
        color.apply(
            [=] (int c) {
                return static_cast<int>(div0((c + average * fade), /, (fade + 1), or 0)); });

    add *= 5.0;
    color =
        color.apply(
            [=] (int c) {
                return static_cast<int>(c + add); });

    return color;
}
