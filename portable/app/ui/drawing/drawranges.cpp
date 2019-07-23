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
#include "drawranges.h"
#include "wave.h"
#include "drawwave.h"
#include "drawzones.h"
#include "waves.h"
#include "view.h"

DrawRanges::DrawRanges(
        Size sizePaint,
        const Wave* wave,
        const View* view) :
    Context(view->io()),
    wave(wave),
    m_widthFull(sizePaint.w()),
    m_pointsEver(wave->pointsStored() + wave->pointsDeleted()),
    m_pointsPerFullScreen(view->pointsPerScreen()),
    m_widthLimited(m_widthFull),
    m_pointsPerRange(view->pointsPerScreen()),
    m_scroll(view->pointsScrollInt()),
    m_sweep(0),
    m_leftOffset(0)
{
}

DrawRanges::xpoints DrawRanges::widthLimited() const { return m_widthLimited; }
DrawRanges::xpoints DrawRanges::pointsPerRange() const { return m_pointsPerRange; }
DrawRanges::xpoints DrawRanges::scroll() const { return m_scroll; }
DrawRanges::xpoints DrawRanges::sweep() const { return m_sweep; }
DrawRanges::xpoints DrawRanges::leftOffset() const { return m_leftOffset; }

void DrawRanges::calcLeftSide(
        DrawWave* drawWave)
{
    //
    // |============================| pointsRecorded
    //       | scroll
    //       \_________________/<---> invisible
    //         pointsPerScreen

    const double invisible = m_pointsEver - (m_scroll + m_pointsPerRange);
    const double empty = -invisible;

    // detect and report if we are completely in sync with
    //   the incoming data and the screen buffer is full
    if (empty > 0)
    {
        m_scroll -= empty;

        if (empty > 1)
            drawWave->adjustingSweep(true);

        LOG(drawDetails, "Draw", "| chasing |"; logKey += logTo->added());
    }
    else
    if (drawWave->adjustingSweep())
    {
        drawWave->adjustingSweep(false);

        LOG(drawDetails, "Draw", "| synched |"; logKey += logTo->added());
    }

    // we fill screen up left to right in sweep mode
    m_sweep =
        m_pointsPerRange > 1?
            fmod(m_scroll, m_pointsPerRange)
            :
            0;

    // now we dont want to debug the edge case when we are
    //   at the screen edges (pun intended),
    //   so we move 1 px away from the edges (the resulting
    //   bug is visually undetectable)

    if (m_sweep < 1)
    {
        m_scroll += 1 - m_sweep;
        m_sweep = 1;

        LOG(drawDetails, "Draw", "| skipping 1st sweep step |"; logKey += logTo->added());
    }

    if (m_pointsPerRange - m_sweep < 1)
    {
        m_scroll -= m_pointsPerRange - m_sweep;
        m_sweep = m_pointsPerRange;

        LOG(drawDetails, "Draw", "| skipping last sweep step |"; logKey += logTo->added());
    }

    // new half-screen range width
    m_widthLimited =
        div0(m_widthLimited * m_sweep, /, m_pointsPerRange, or 0);

    // scroll pos within the range
    m_scroll =
        m_scroll - m_sweep
        + (m_pointsPerFullScreen - m_pointsPerRange)
        + m_pointsPerFullScreen;

    if (m_scroll < 0)
    {
        LOG(drawDetails, "Draw", "| no negative scroll |"; logKey += logTo->added());

        m_scroll = 0;
    }

    m_pointsPerRange = m_sweep;
}

void DrawRanges::calcRightSide(
        DrawWave* drawWave)
{
    UNUSED(drawWave);

    //
    // rightSide = screen - leftSide
    //

    m_leftOffset = div0(m_sweep * m_widthLimited, /, m_pointsPerRange, or 0);
    m_widthLimited = m_widthFull - m_widthLimited;
    m_pointsPerRange = m_pointsPerFullScreen - m_pointsPerRange;
    m_scroll -= m_pointsPerRange;
}

int DrawRanges::pointsToDraw() const
{
    xpoints pointsToDraw = m_pointsPerRange;
    xpoints end = m_scroll + pointsToDraw;

    //if (view()->sweep())
    {
        // haven't filled the whole screen yet?
        if (end > m_pointsEver)
        {
            pointsToDraw -= end - m_pointsEver;
            end = m_pointsEver;

            LOG(drawDetails, "Draw", "Adjusting for incomplete screen: " << pointsToDraw);
        }
    }

    return static_cast<int>(pointsToDraw);
}

