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
#ifndef DRAWWAVE_H
#define DRAWWAVE_H

class DrawRanges;

// Draw signal array

class DrawWave : public Context
{
public:
    explicit DrawWave(
            const Wave* wave,
            const Waves* waves,
            const View* view);

    void paint(
            Paint& paint);

    // where paint ends now
    GetSet<int> pointNewestDrawn = 0;
    GetSet<int> xSweep = 0;
    GetSet<bool> adjustingSweep = false;

private:

    const GetRelated<const Wave*> wave;
    const GetRelated<const Waves*> waves;
    const GetRelated<const View*> view;

    typedef double xpoints;
        // we switch to floating point for x-scaling to avoid polyline flicker
        //   on rounding errors

    // deleted wave marker icon
    struct visuals // (a const namespace)
    {
        static constexpr int penThickness = 1;
    };

    void paintAll(
            Paint& paint);

    int paintRange(
            const DrawRanges& ranges,
            bool half,
            Paint& paint,
            int sizeH,
            int yTop,
            As<bool, class FadeColors> fadeColors);

    Maybe<int> calcReadIndex(
            int iVisible,
            bool half,
            int toX,
            const DrawRanges& ranges);

    Rgb channelColor() const;

    Rgb fadeColor(
            const Rgb& colorFrom,
            xpoints pointsPerScreen)
            const;
};

#endif
