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
#include "wave.h"
#include "drawpolyline.h"
#include "waves.h"
#include "view.h"

DrawPolyline::DrawPolyline(
        const Wave* wave,
        const View* view) :
    Context(wave->io()),
    wave(wave),
    view(view)
{
}

void DrawPolyline::reset()
{
}

void DrawPolyline::paintNext(
        Paint& paint,
        int iRead,
        int x,
        int yZero,
        int hPerChannel,
        bool half)
{
    double amplitudeScale = view()->amplitudeScale();

    rASSERT(iRead >= 0);
    rASSERT(iRead < wave()->pointsStored());
    auto amplitude =  wave()->pointAmplitude(iRead, 0);

    amplitude = static_cast<int>(amplitudeScale * amplitude);

    int y =
        yZero
            -
            useful::mulDiv(
                amplitude,
                hPerChannel,
                wave()->maxAmplitude());

    pointsDrawn(pointsDrawn() + 1);

    m_iLastDrawn = Maybe<int>(iRead);
    m_debugAverageAmplitude =
        (m_debugAverageAmplitude + y) / 2.0;

    if (pointsDrawn() > 1)
    {
        const bool extraLog = false;

        if (!half) // && ranges.scroll() > 0)
        {
            paint.drawLine(m_from, Point(x, y));

            if (pointsDrawn() < 3 || extraLog)
                LOG(drawDetails, "DrawSample",
                    x << "x" << y);
        }
        else
        if (m_fromPaintedX != x)
        {
            // we skip drawing intermediate points which map to the same single pixel

            if (m_fromPaintedX != -1)
            {
                /* we could do this to optimize the intermediate case
                paint.drawLine(
                    Point(m_fromPaintedX, m_fromPaintedMinY),
                    Point(m_fromPaintedX, m_fromPaintedMaxY));
                    */

                // we optimize the slowest highest density case at expence
                //   of faster modes with less points per pixel
                paint.drawLine(
                    Point(m_fromPaintedX, m_fromPaintedY),
                    Point(m_fromPaintedX, m_fromPaintedMaxY));
                paint.drawLine(
                    Point(m_fromPaintedX, m_fromPaintedY),
                    Point(m_fromPaintedX, m_fromPaintedMinY));
                paint.drawLine(
                    Point(m_fromPaintedX, m_fromPaintedMaxY),
                    Point(x, y));
                paint.drawLine(
                    Point(m_fromPaintedX, m_fromPaintedMinY),
                    Point(x, y));

                if (pointsDrawn() < 10 || extraLog)
                    LOG(drawDetails, "DrawSample",
                        m_fromPaintedX << "x" <<
                            "(" << m_fromPaintedMinY << "..." << m_fromPaintedMaxY << ")"
                            " -> " << x << "x" << y);
            }
            else
            {
                if (pointsDrawn() < 15 || extraLog)
                    LOG(drawDetails, "DrawSample",
                        "skipping x = " << x);
            }

            m_fromPaintedX = x;
            m_fromPaintedY = y;
            m_fromPaintedMinY = y;
            m_fromPaintedMaxY = y;
        }
        else
        {
            m_fromPaintedMinY = std::min(m_fromPaintedMinY, y);
            m_fromPaintedMaxY = std::max(m_fromPaintedMaxY, y);
        }
    }
    else
    {
        LOG(drawDetails, "DrawFirst",
            "[" << iRead << "] "
                << x << "x" << y);
    }

    m_from = Point(x, y);
}

std::string DrawPolyline::logFinalState() const
{
    std::string log =
        std::string() +
            "[" + m_iLastDrawn.stringOr("Never drawn") + " last], " +
            "average Y = " + std::to_string(useful::iround(m_debugAverageAmplitude));

    return log;
}
