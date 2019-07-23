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
#ifndef VIEW_H
#define VIEW_H

#include "application.h"

// Wave view settings

class View : public Context
{
public:
    explicit View(
            std::shared_ptr<Io> io);

    enum Delta
    {
        decrease = -1,
        increase = 1,
    };

    // how much curve we fit into the window
    int pointsPerScreen() const;
    //void pointsPerScreen(int points);

    // align the point count to window pixels
    void adjustToPaintSize(Size size);

    // time [-]/[+] buttons
    void changePointsPerScreen(Delta delta);
    std::string whyCannotChangePointsPerScreen(Delta delta) const;

    // vertical multiplier for each Wave
    double amplitudeScale() const;

    // [-]/[+] buttons
    void changeAmplitudeScale(Delta delta);
    std::string whyCannotChangeAmplitudeScale(Delta delta) const;

    // image shift to control online animation
    //   (not suitable for offline scrolling)
    GetSet<double> pointsScroll = 0;
    int pointsScrollInt() const;
        // (note: integer value stored would lose precision over time)

    // circular paint position
    //GetSet<double> pointsSweep = 0;

    // max points we could have by time elapsed
    GetSet<double> pointsLimitByTime = 0;

    // render mode
    GetSet<bool> showSweep = false;
    GetSet<bool> showScroll = true;

    // auto switch to sweep mode
    GetSet<bool> pendingShowSweep = true;

    // debug
    void reportStatus() const;
    struct ForTests
    {
        WeakMap<UiPanel> uiGraph;
        WeakMap<UiButton> uiMode;
    } forTests;

private:

    int m_pointsPerScreen; // initializer depends on Scaler static const
    double m_amplitudeScale = 1.0;
    Size m_paintSize;

    std::shared_ptr<Scaler> scalerTemp() const;

    struct defaults // (a const namespace)
    {
        static constexpr double amplitudeScaleMin = 0.05;
        static constexpr double amplitudeScaleMax = 3;
        static constexpr double amplitudeScaleDelta = 0.5;
    };
};

#endif
