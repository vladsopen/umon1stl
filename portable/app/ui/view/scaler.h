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
#ifndef SCALER_H
#define SCALER_H

#include "application.h"
#include "view.h" // Delta

// Selects valid Points-per-Pixel horizontal time scale
//   (extracted from View class)

class Scaler : public Context
{
public:
    explicit Scaler(
        //View* view, // no need anymore
        std::shared_ptr<Io> io);

    // parent object
    //const GetParent<View*> view;

    // this is called for changing PPS
    //   and for testing if it is at the limit with all the restrictions
    int nextPointsPerScreen(
            int fromPointsPerScreen,
            View::Delta delta,
            int paintWidth,
            const std::string& why,
            As<int, class RecursionDepth> recursionDepth =
                As<int, class RecursionDepth>(0),
            int startedFromPointsPerScreen = -1);

    // snap to a nicely scaled value
    static int limitPointsPerScreen(
            int fromPointsPerScreen,
            int paintWidth);

    static int defaultPointsPerScreen();
    static int deltaPointsPerScreen();

private:

    // debug info
    int m_deepestRecursion = 0;

    struct defaults // (a const namespace)
    {
        static constexpr int pointsPerScreen = 2000;
        static constexpr int pointsPerScreenMin = 500;
        static constexpr int pointsPerScreenMax = 6000;
        static constexpr int pointsPerScreenDelta = 300;
    };

    int nextPointsPerScreenNoLog(
            int fromPointsPerScreen,
            View::Delta delta,
            int paintWidth,
            const std::string& why,
            int recursionDepth,
            int startedFromPointsPerScreen);

    friend class TestScaler;
};

#endif
