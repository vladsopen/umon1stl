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
#ifndef DRAWRANGES_H
#define DRAWRANGES_H

class DrawWave;

// Calculates currently visible horizontal polyline sector metrics for DrawWave

class DrawRanges : public Context
{
public:
    explicit DrawRanges(
            Size sizePaint,
            const Wave* wave,
            const View* view);

    typedef double xpoints;
        // (this might need a sync with DrawWave::xpoints,
        //   but too early to make it global)

    const GetParent<const Wave*> wave;

    // screen metrics
    //   (we calculate most of the values here for use in DrawWave;
    //    could be a fun idea to make them all const by limiting DrawRanges
    //    scope to a single subrange)
    xpoints widthLimited() const;
    xpoints pointsPerRange() const;
    xpoints scroll() const;
    xpoints sweep() const;
    xpoints leftOffset() const;

    // debugging
    std::string logKey;
    std::string logDetails;

    //
    //      sweep
    //       pos
    //        |
    //  ------v--------
    // |\/\/\/ /\/\/\/\|  ---> time [ms]
    //  ---------------        view [pixels]
    //  left   right           sampled amplitudes [points]
    //  side   side
    //  new    previous
    //  data   points
    //

    void calcLeftSide(DrawWave* drawWave);
    void calcRightSide(DrawWave* drawWave);

    int pointsToDraw() const;

private:
    const xpoints m_widthFull;
    const xpoints m_pointsEver;
    const xpoints m_pointsPerFullScreen;

    xpoints m_widthLimited;
    xpoints m_pointsPerRange;
    xpoints m_scroll;
    xpoints m_sweep;
    xpoints m_leftOffset;

};

#endif
