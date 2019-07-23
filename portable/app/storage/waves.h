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
#ifndef WAVES_H
#define WAVES_H

#include "application.h"

// Multichannel wave container

class Waves : public Context
{
public:
    explicit Waves(
            std::shared_ptr<Io> io);

    int size() const;

    void forEachWave(
            std::function<void (const Wave* wave)> f)
            const;

    bool isLast(
            const Wave* wave)
            const;

    // read wave portions from a device
    void acquire(
            Feeder* feeder,
            View* view);

    // for view/incrementer: where our data is
    int pointsRecorded() const;
    int pointsLastBlock() const;
    int pointsMaxDeleted() const;
    //int pointsPerSecond() const;

    // testing
    void dump(
            const std::string& comment)
            const;

private:
    //const GetParent<Storage*> storage;

    std::vector<std::shared_ptr<Wave>> m_waves;
    int m_pointsRecorded = 0;
    int m_pointsLastBlock = 0;

    friend class TestAll;
    friend class TestFindMax;

    // testable version
    using Reader =
        std::function<void (
            int channel,
            Out, std::vector<int>& wave,
            Out, int& maxAmplitude)>;
    void acquire(
            Reader reader,
            int maxStorePoints,
            int flushOldPoints);

    friend class TestWaveAcquire;
};

#endif
