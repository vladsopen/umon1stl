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
#ifndef EMULATOR_H
#define EMULATOR_H

#include "application.h"
#include "feeder.h"

// Test wave generator

class Emulator : public Feeder
{
public:
    explicit Emulator(std::shared_ptr<Io> io);

protected:

    virtual int pointsPerSecond() const override;
    virtual void acquire(
            int channel,
            Out, std::vector<int>& wave,
            Out, int& maxAmplitude)
            override;
    virtual void toolbar(
            ParentUi& parent)
            override;

private:
    std::vector<std::vector<int>> m_templates;
    std::vector<int> m_noise;

    WeakMap<UiButton> m_observeNoise;

    struct defaults // (a const namespace)
    {
        static constexpr int pointsPerSecond = 390 / 2; // for the demo generator
        static constexpr int maxAmplitude = 850;
    };

};

#endif
