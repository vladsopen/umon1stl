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
#include "drawwave.h"
#include "drawzones.h"
#include "waves.h"
#include "view.h"

DrawZones::DrawZones(const Wave* wave) :
    Context(wave->io()),
    wave(wave)
{
}

/*
View* DrawZones::view() const
{
    return wave()->view();
}
*/

void DrawZones::reset()
{
    m_xDrawNextCross = Nothing();
}

void DrawZones::paintDeleted(
        Paint& paint,
        int iAt,
        int xCenter,
        int yZero)
{
    int y = yZero;

    int xAligned = ((xCenter - cross::pixels) &~ cross::bitSnap) + cross::bitSnap;

    if (m_xCrossAlreadyDrawn.valueOr(-1) != xAligned)
    {
        if (m_xCrossAlreadyDrawn.valid())
        {
            if (m_xDrawNextCross.valid())
            {
                int x = m_xDrawNextCross.useOr(-1);

                paint.drawLine(
                    Point(x - cross::pixels, y - cross::pixels),
                    Point(x + cross::pixels, y + cross::pixels));
                paint.drawLine(
                    Point(x + cross::pixels, y - cross::pixels),
                    Point(x - cross::pixels, y + cross::pixels));

                ++m_debugCrossCount;

                if (m_debugCrossCount % 10 == 0)
                    LOG(drawDetails, "DrawCross",
                        "[" << iAt << "] "
                            << m_debugCrossCount << "th cross");
            }

            m_xDrawNextCross = Maybe<int>(xAligned);
        }

        m_xCrossAlreadyDrawn = Maybe<int>(xAligned);
    }
}

std::string DrawZones::logFinalState() const
{
    if (m_debugCrossCount > 0 && wave()->channel()  == 0)
        LOG(drawEssential, "DrawZones",
            std::to_string(m_debugCrossCount) + " crosses");

    return std::to_string(m_debugCrossCount) + " crosses";
}
