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
#ifndef FEEDER_H
#define FEEDER_H

#include "application.h"

// Abstract data source (device/network/emulator/file...)

class Feeder : public Context
{
public:
    explicit Feeder(std::shared_ptr<Io> io);

    // metrics
    virtual int pointsPerSecond() const = 0;

    // read next data portion
    //   (the demo app expects all sizes to be synchronized)
    //   (must recover/report from any transient errors)
    virtual void acquire(
            int channel,
            Out, std::vector<int>& wave,
            Out, int& maxAmplitude)
            = 0;

    // debug buttons
    virtual void toolbar(
            ParentUi& parent)
            = 0;

private:
};

#endif
