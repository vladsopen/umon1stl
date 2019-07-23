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
#ifndef DRAWZONES_H
#define DRAWZONES_H

// Draw special ranges like deleted wave pointsa

class DrawZones : public Context
{
public:
    explicit DrawZones(const Wave* wave);

    const GetParent<const Wave*> wave;
    //View* view() const;

    void paintDeleted(
            Paint& paint,
            int iAt,
            int xCenter,
            int yZero);

    // when crosses end and wave section starts
    void reset();

    // debug
    std::string logFinalState() const;

private:

    // snap crosses to some grid and only draw them once
    Maybe<int> m_xCrossAlreadyDrawn = Nothing();

    // delay first cross
    //   (we draw 1 cross less to keep it away from the polyline)
    Maybe<int> m_xDrawNextCross = Nothing();

    // debug only
    int m_debugCrossCount = 0;

    // deleted wave marker icon
    struct cross // (a const namespace)
    {
        static constexpr int pixels = 10;
        static constexpr int bitSnap = 31; // bit mask 2-power - 1
    };

};

#endif
