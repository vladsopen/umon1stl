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
#include "incrementer.h"
#include "view.h"
#include "storage.h"
#include "waves.h"
#include "feeder.h"

Incrementer::Incrementer(
        std::shared_ptr<Io> io,
        View* view,
        Storage* storage,
        Feeder* feeder) :
    Context(io),
    view(view),
    storage(storage),
    feeder(feeder)
{
}

Waves* Incrementer::waves() const
{
    return storage()->waves().get();
}

void Incrementer::step()
{
    ms msNow =
        //duration_cast<ms>(
        //    system_clock::now().time_since_epoch());
        io()->mock()->nextTick()();

    ms msStep =
        stepToNextRecord(
            msNow);

    scrollForward(
        msNow,
        msStep);

    m_msLast = msNow;
}

Incrementer::ms Incrementer::stepToNextRecord(
        ms msNow)
{
    // This function emulates real time data reception by
    //   requesting new blocks after a calculated timeout expires.
    //   It supports a pause function and restores synchronization.

    int pointsPerSecond = feeder()->pointsPerSecond();

    auto convertPointsToMs = [=] (int points)
    {
        return ms(useful::mulDiv(points, 1000, pointsPerSecond));
    };

    ms msTotal = ms::zero();
    ms msStep = ms::zero();

    // first call?
    if (m_msStart == ms::zero())
    {
        m_msStart = msNow - ms(defaults::msInitialAcceleration);

        LOG(all, "IncrementerStart", "first time step() call");
    }
    else
    {
        msTotal = msNow - m_msStart;
        msStep = msNow - m_msLast;
    }

    int pointsRecorded = waves()->pointsRecorded();
    int pointsPerScreen = view()->pointsPerScreen();

    ms msRecorded = convertPointsToMs(pointsRecorded);

    // time to grab next data block?
    ms msOverRecorded = msRecorded - (msTotal + m_msSpeedForward);
    if (msOverRecorded <= ms(0))
    {
        if (!pause())
        {
            waves()->
                acquire(
                    feeder(),
                    view());

            LOG(all, "IncrementerAcquire",
                msOverRecorded.count() << "ms behind");
        }
    }
    else
    if (msOverRecorded > convertPointsToMs(pointsPerScreen))
    {
        m_msSpeedForward += (msRecorded - msTotal) / 10;

        LOG(incrementerDetails, "IncrementerSkip",
            m_msSpeedForward.count() << "ms current skip");
    }

    displayTimeDelta(msNow, msOverRecorded);

    return msStep;
}

void Incrementer::scrollForward(
        ms msNow,
        ms msStep)
{
    UNUSED(msNow);

    // No deep math here.
    //   This function just ensures the demo application automatically
    //   scrolls data forward smoothly or quickly chases the most recent block
    //   when for some (stress testing) reason we are way behind live signal.
    //   (Production view balacer is way more complicated and heavily dependent
    //   on the nature of the signal and on network reception modes)

    int pointsPerScreen = view()->pointsPerScreen();
    int pointsPerSecond = feeder()->pointsPerSecond();
    int pointsRecorded = waves()->pointsRecorded();
    int pointsLastBlock = waves()->pointsLastBlock();
    double pointsScroll = view()->pointsScroll();

    //
    // |============================| pointsRecorded
    //       | scroll
    //       \_________________/<---> invisible
    //         pointsPerScreen

    int invisible = pointsRecorded - (static_cast<int>(pointsScroll) + pointsPerScreen);
    int empty = -invisible;

    double pointsAdd = 0;

    double pointsDelta = double(msStep.count()) * pointsPerSecond / 1000.0;

    // calculate ideal point count according to the time elapsed so that we could
    //   draw a smooth sweep instead of jumping at avery new block
    view()->pointsLimitByTime(
        std::min<double>(
            std::max(
                view()->pointsLimitByTime() + pointsDelta,
                double(pointsRecorded - pointsLastBlock)),
            pointsRecorded));

    // time to scroll?
    if (pointsRecorded > pointsPerScreen + pointsLastBlock &&
        empty <= 0)
    {
        pointsAdd = pointsDelta;

        handleEndOfScreen();
    }
    /*
    else
    if (view()->showSweep() && !pause() &&
        pointsRecorded > pointsLastBlock * 2)
    {
        msAdd = std::max(msAdd, msDelta);
    }
    */

    // need to more quickly scroll to the newer data?
    if (invisible > pointsLastBlock + pointsLastBlock / 10)
    {
        m_pointsAccelerate = invisible / 10;

        LOG(incrementerDetails, "IncrementerAccelerating", m_pointsAccelerate << "pt current speed up");
    }

    // smooth out scrolling acceleration over time (avoid hard jumps ahead if possible)
    if (m_pointsAccelerate > 0)
    {
        int faster = m_pointsAccelerate / 10;
        pointsAdd += faster;
        m_pointsAccelerate -= faster;

        LOG(incrementerDetails, "IncrementerShift", faster << "pt faster");
    }

    pointsScroll += pointsAdd;

    // instant fast forward on VERY long delay
    // (here we cause the scroll speed to fluctuate on long delays,
    //  switching between larger acceleration and scroll resets,
    //  this is actually great: we decrease visual effect of data moving backward)
    if (invisible > pointsPerScreen * 5)
    {
        pointsScroll = std::max(pointsRecorded - pointsPerScreen * 2, 0);

        LOG(incrementerDetails, "IncrementerFastForward", "after long delay");
    }

    // quickly reset the scroll if the whole curve is out of the screen
    int deleted = waves()->pointsMaxDeleted();
    if (pointsScroll < deleted)
    {
        pointsScroll = std::max(pointsRecorded - pointsPerScreen, 0);

        LOG(incrementerDetails, "IncrementerEscapesZombies", "dont stall in deleted zone");
    }

    // commit new scroll pos
    //double pointsMoved = pointsScroll - view()->pointsScroll();
    view()->pointsScroll(pointsScroll);

    /* -- no separate sweep pos, we use fmod() for now
    // move sweep pos
    double pointsSweep = view()->pointsSweep() + pointsMoved;
    while (pointsSweep > pointsPerScreen)
        pointsSweep -= pointsPerScreen;
    view()->pointsSweep(pointsSweep);
    */
}

void Incrementer::displayTimeDelta(
        ms msNow,
        ms msOverRecorded)
{
    using namespace std::chrono;

    if (msNow - m_msLastUiTimeUpdated
            >
            ms(defaults::msTimeDeltaDisplayTimeout))
    {
        m_msLastUiTimeUpdated = msNow;

        auto secondsSigned = duration_cast<seconds>(msOverRecorded).count();
        auto secondsAbs = std::abs(secondsSigned);

        timeReport(
            secondsAbs < 2?
               ""
               :
               std::to_string(secondsAbs) +
                   " s. " +
                   (secondsSigned < 0? "behind" : "ahead"));

        // NOTE: timeReport is smart now and GetSetTell<> causes UI observers to update
        //   only as the data changes

        //uiTimeReport.apply([] (auto ui) { ui->update(); });
        //timeReport.fire();
    }
}

void Incrementer::handleEndOfScreen()
{
    // auto switch from scroll mode to sweep at the end of the first screen
    //   only if the user haven't changed the display mode
    // when the sweep mode is able to draw from incomplete first buffers this
    //   won't be needed

    if (view()->pendingShowSweep())
    {
        LOG(all, "IncrementerForcingSweep", "(happens once)");

        view()->pendingShowSweep(false);
        if (view()->showScroll() && !view()->showSweep())
        {
            view()->showScroll(false);
            view()->showSweep(true);
        }
    }
}
