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
#ifndef INCREMENTER_H
#define INCREMENTER_H

#include "application.h"

// Receives data blocks, syncronizes timing, scrolls.
//   (for network reception online mode)
//   In this simplified version it mainly synchronizes the emulator and
//   makes waves sweep nice and smooth for the emulated data reception.

class Incrementer : public Context
{
public:
    explicit Incrementer(
            std::shared_ptr<Io> io,
            View* view,
            Storage* storage,
            Feeder* feeder);

    // temporarily stop signal reception
    GetSet<bool> pause = false;

    // current delay indicator
    GetSetTell<std::string> timeReport;

    // the timer handler
    void step();

private:

    const GetRelated<View*> view;
    const GetRelated<Storage*> storage;
    const GetRelated<Feeder*> feeder;
    Waves* waves() const;

    using ms = std::chrono::milliseconds;

    // sync control
    ms m_msStart = ms::zero();
    ms m_msLast = ms::zero();
    ms m_msSpeedForward = ms::zero();

    // UI optimization
    ms m_msLastUiTimeUpdated = ms::zero();

    int m_pointsAccelerate = 0;

    struct defaults // (a const namespace)
    {
        // (not using static int or milliseconds - enum makes unaddresable const (C++14))

        enum { msInitialAcceleration = 0 };
        enum { msTimeDeltaDisplayTimeout = 300 };
    };

    ms stepToNextRecord(
            ms msNow);

    void scrollForward(
            ms msNow,
            ms msStep);

    void displayTimeDelta(
            ms msNow,
            ms msOverRecorded);

    void handleEndOfScreen();

};

#endif
