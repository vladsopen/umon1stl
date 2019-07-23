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
#ifndef WAVE_H
#define WAVE_H

class Waves;

// Single channel signal array

class Wave : public Context
{
public:
    explicit Wave(
            int channel,
            std::shared_ptr<Io> io);

    //const GetParent<Waves*> waves;

    const GetPredefined<int> channel;

    // curve array cache length, not equal to total recording! see pointsDeleted()
    int pointsStored() const;

    // they need to know which parts they cannot paint anymore
    int pointsDeleted() const;

    // lookup a point value
    int pointAmplitude(int at, int orReturn) const;
    int maxAmplitude() const;

    // read signal portion from a data source
    void append(
            const std::vector<int>& add,
            As<int, class MaxAmplitude> maxAmplitude,
            As<int, class MaxStorePoints> maxStorePoints,
            As<int, class FlushOldPoints> flushOldPoints);

    // debug
    std::string report()
            const;

private:

    std::vector<int> m_curve;
    int m_maxAmplitude = 0;
    int m_pointsDeleted = 0;

    void flushToPermanentStorage(
            As<int, class MaxStorePoints> maxStorePoints,
            As<int, class FlushOldPoints> flushOldPoints);

    friend class TestFindMax;
};

#endif
