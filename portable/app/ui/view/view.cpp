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

View::View(std::shared_ptr<Io> io) :
    Context(io),
    m_pointsPerScreen(Scaler::defaultPointsPerScreen())
{
}

int View::pointsPerScreen() const
{
    return
        Scaler::limitPointsPerScreen(
            m_pointsPerScreen,
            m_paintSize.w());
}

/*
void View::pointsPerScreen(int points)
{
    m_pointsPerScreen = points;
}
*/

void View::adjustToPaintSize(Size size)
{
    if (m_paintSize == size)
        return;

    m_paintSize = size;

    reportStatus();
}

int View::pointsScrollInt() const
{
    return static_cast<int>(pointsScroll());
}

std::shared_ptr<Scaler> View::scalerTemp() const
{
    return std::make_shared<Scaler>(io());
}

void View::changePointsPerScreen(Delta delta)
{
    m_pointsPerScreen =
        scalerTemp()->
            nextPointsPerScreen(
                m_pointsPerScreen,
                delta,
                m_paintSize.w(),
                "change");

    reportStatus();

    /*
    if (waves()->pointsRecorded() - waves()->maxDeletedPoints() < pointsPerScreen())
    {
        if (showSweep() && !showScroll())
        {
            showScroll(true);
            showSweep(false);
            pendingShowSweep(true);
        }
    }
    */
}

void View::reportStatus() const
{
    Io::Os::say(
        "Pt/Screen: " + std::to_string(pointsPerScreen()) +
            " (" + std::to_string(m_pointsPerScreen) + ")"
            " at " + std::to_string(m_paintSize.w()) + "px,"
            " pt/px: " + std::to_string(div0(1.0 * pointsPerScreen(), /, m_paintSize.w(), or 0)) + ","
            " px/pt: " + std::to_string(div0(1.0 * m_paintSize.w(), /, pointsPerScreen(), or 0)) + ""
            "\n");
}

std::string View::whyCannotChangePointsPerScreen(Delta delta) const
{
    int newPointsPerScreen =
        scalerTemp()->
            nextPointsPerScreen(
                m_pointsPerScreen,
                delta,
                m_paintSize.w(),
                "disabling");

    if (std::abs(newPointsPerScreen - m_pointsPerScreen)
            < Scaler::deltaPointsPerScreen())
    {
        return
            delta < 0? "At minimum" : "At maximum";
    }

    return ""; // ok to change
}

double View::amplitudeScale() const
{
    return m_amplitudeScale;
}

void View::changeAmplitudeScale(Delta delta)
{
    rASSERT(delta == 1 || delta == -1);

    auto amplitudeScaleBefore = m_amplitudeScale;

    m_amplitudeScale +=
        delta * defaults::amplitudeScaleDelta;

    m_amplitudeScale =
        useful::clamp(
            m_amplitudeScale,
            defaults::amplitudeScaleMin,
            defaults::amplitudeScaleMax);

    LOG(all, "ViewAmplitude", amplitudeScaleBefore << " --> " << m_amplitudeScale);
}

std::string View::whyCannotChangeAmplitudeScale(Delta delta) const
{
    return
        delta < 0? (m_amplitudeScale <= defaults::amplitudeScaleMin? "At minimum" : "")
                 : (m_amplitudeScale >= defaults::amplitudeScaleMax? "At maximum" : "");
}
