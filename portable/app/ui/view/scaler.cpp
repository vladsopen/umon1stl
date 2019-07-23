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
#include "view.h"
#include "scaler.h"

Scaler::Scaler(std::shared_ptr<Io> io) :
    Context(io)
    //view(view)
{
}

int Scaler::defaultPointsPerScreen()
{
    return defaults::pointsPerScreen;
}

int Scaler::deltaPointsPerScreen()
{
    return defaults::pointsPerScreenDelta;
}

int Scaler::limitPointsPerScreen(
        int fromPointsPerScreen,
        int paintWidth)
{
    if (paintWidth <= 1 ||
        fromPointsPerScreen <= 1)
    {
        return fromPointsPerScreen;
    }

    int xPerPoint = div0(paintWidth, /, fromPointsPerScreen, or 0);
    int pointsPerX = div0(fromPointsPerScreen, /, paintWidth, or 0);

    // select only odd multiplys of points because some
    //   even values will cause flicker in sweep mode
    int multiplyPointsPerX =
        //pointsPerX;
        pointsPerX > 5? pointsPerX | 1 : pointsPerX;

    // choose well aligned nicely divisible values for PPS vs pixels
    int pointsPerScreenBest =
        xPerPoint > 0?
            div0(paintWidth, /, xPerPoint, or 0)
            :
        pointsPerX > 0?
            paintWidth * multiplyPointsPerX
            :
            paintWidth;

    return pointsPerScreenBest;
}

int Scaler::nextPointsPerScreen(
        int fromPointsPerScreen,
        View::Delta delta,
        int paintWidth,
        const std::string& why,
        As<int, class RecursionDepth> recursionDepth,
        int startedFromPointsPerScreen)
{
    if (recursionDepth == 0)
        m_deepestRecursion = 0;

    int next =
        nextPointsPerScreenNoLog(
            fromPointsPerScreen,
            delta,
            paintWidth,
            why,
            recursionDepth,
            startedFromPointsPerScreen);

    LOG(scalerDetails, "PointsPerScreenProgress",
        std::string(recursionDepth, delta < 0? '-' : '+') << " "
            << why << " " << fromPointsPerScreen << " -> " << next);

    if (recursionDepth == 0)
    {
        LOG(all, "PointsPerScreenFinal",
            why << " "
                << "from " << fromPointsPerScreen << "pt "
                << "for " << paintWidth << "px "
                << (delta < 0? "-" : "+") << " "
                << "*" << m_deepestRecursion << " loops "
                << " --> " << next << "pt");
    }

    return next;
}

int Scaler::nextPointsPerScreenNoLog(
        int fromPointsPerScreen,
        View::Delta delta,
        int paintWidth,
        const std::string& why,
        int recursionDepth,
        int startedFromPointsPerScreen)
{
    rASSERT(delta == 1 || delta == -1);

    // we need to find the next points-per-pixel ratio which would match
    //   all conditions in limitPointsPerScreen() which depends on
    //   the current resolution (window width in pixels)

    // NOTE: We will discard most of this when we implement proper portable
    //   incremental paint.
    //   An alternative maybe simpler implementation would be traveling
    //   through a list of precalculated matching PPX ratios.

    m_deepestRecursion = std::max(m_deepestRecursion, recursionDepth);

    // on 1st non recursive call remember the starting point
    if (startedFromPointsPerScreen == -1)
        startedFromPointsPerScreen = fromPointsPerScreen;

    // have we tried all possible values?
    if (recursionDepth >
            div0(defaults::pointsPerScreenMax - defaults::pointsPerScreenMin,
                 /,
                 defaults::pointsPerScreenDelta,
                 or 0))
    {
        return fromPointsPerScreen;
            // this is an extra safety precaution which never happens,
            //   recursion normally stops much earlier hitting a limit checked below
    }

    int newPointsPerScreen =
        fromPointsPerScreen +
            delta * defaults::pointsPerScreenDelta;

    int clampedPointsPerScreen =
        useful::clamp(
            newPointsPerScreen,
            defaults::pointsPerScreenMin,
            defaults::pointsPerScreenMax);

    if (clampedPointsPerScreen == fromPointsPerScreen)
        return fromPointsPerScreen;

    newPointsPerScreen = clampedPointsPerScreen;

    // change more if pixel adjustment would kill our increment
    int limitedNew = limitPointsPerScreen(newPointsPerScreen, paintWidth);
    int limitedStart = limitPointsPerScreen(startedFromPointsPerScreen, paintWidth);
    if (std::abs(limitedNew - limitedStart)
            < defaults::pointsPerScreenDelta
        ||
        std::abs(limitedNew - startedFromPointsPerScreen)
            < defaults::pointsPerScreenDelta)
    {
        return
            nextPointsPerScreen(
                newPointsPerScreen,
                delta,
                paintWidth,
                why,
                As<int, class RecursionDepth>(recursionDepth + 1),
                startedFromPointsPerScreen);
    }

    newPointsPerScreen =
        limitPointsPerScreen(newPointsPerScreen, paintWidth);

    // is the change at least minimum delta?
    if (std::abs(newPointsPerScreen - startedFromPointsPerScreen)
            < defaults::pointsPerScreenDelta)
    {
        return fromPointsPerScreen;
    }

    return newPointsPerScreen;
}

