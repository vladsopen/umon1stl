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
#include "application.h"
#include "wave.h"
#include "drawwave.h"
#include "waves.h"
#include "view.h"

Wave::Wave(
        int channel,
        std::shared_ptr<Io> io) :
    Context(io),
    channel(channel)
{
}

void Wave::append(
        const std::vector<int>& add,
        As<int, class MaxAmplitude> maxAmplitude,
        As<int, class MaxStorePoints> maxStorePoints,
        As<int, class FlushOldPoints> flushOldPoints)
{
    rASSERT(maxAmplitude >= 0);

    m_maxAmplitude = std::max<int>(m_maxAmplitude, maxAmplitude); // 850

    m_curve.insert(m_curve.end(), add.begin(), add.end());

    flushToPermanentStorage(
        maxStorePoints,
        flushOldPoints);
}

void Wave::flushToPermanentStorage(
        As<int, class MaxStorePoints> maxStorePoints,
        As<int, class FlushOldPoints> flushOldPoints)
{
    //
    // STUB CODE.
    //
    // This demo just tests our ability to draw partly deleted data
    //

    // we just delete old data instead of saving it
    int pointsDelete = flushOldPoints;

    if (pointsDelete > maxStorePoints ||
        pointsDelete <= 0 ||
        maxStorePoints <= 0)
    {
        rFAIL("invalid flush setup");
        return;
    }

    // safe limited loop
    int maxRepeatDelete = div0(useful::size(m_curve), /, pointsDelete, or 0);
    for (int iRepeatDelete NOTUSED : FromTo(0, maxRepeatDelete))
    {
        // deletion condition:
        if (useful::size(m_curve) <= maxStorePoints)
            break;

        rASSERT(pointsDelete <= useful::size(m_curve));

        m_curve.erase(m_curve.begin(), m_curve.begin() + pointsDelete);

        m_pointsDeleted += pointsDelete;

        if (channel() == 0)
        {
            LOG(all, "DeleteOld",
                pointsDelete << " points deleted, total: "
                    << m_pointsDeleted);
        }
    }
}

LOCAL_TEST(TestWaveAppend)(PassIo io)
{
    auto wave = std::make_shared<Wave>(12345, io());

    for (int iFlushOldPoints : FromTo(1, 10))
    {
        for (int iMaxStorePoints : FromTo(iFlushOldPoints + 1, iFlushOldPoints + 10))
        {
            auto add = std::vector<int>(iFlushOldPoints / 2 + 2);
            wave->
                append(
                    add,
                    As<int, class MaxAmplitude>(iFlushOldPoints * iMaxStorePoints),
                    As<int, class MaxStorePoints>(iMaxStorePoints),
                    As<int, class FlushOldPoints>(iFlushOldPoints));

            LOG(all, "WaveAppend",
                ""
                    "stored: " << iMaxStorePoints << ", "
                    "flush: " << iFlushOldPoints << ", "
                    "add: " << useful::size(add) << ", "
                    "result: " << wave->pointsStored() << ", "
                    "maxA: " << wave->maxAmplitude() << ", "
                    "deleted: " << wave->pointsDeleted() << ".");
        }
    }
}

int Wave::pointsStored() const
{
    return useful::size(m_curve);
}

int Wave::pointsDeleted() const
{
    return m_pointsDeleted;
}

int Wave::pointAmplitude(int at, int orReturn) const
{
    if (at < 0 || at >= useful::size(m_curve))
    {
        LOG_FAIL(critical, "Wave", "out of curve bounds!");
        return orReturn;
    }

    return m_curve[at];
}

int Wave::maxAmplitude() const
{
    return m_maxAmplitude;
}

std::string Wave::report()
        const
{
    std::string result;
    result.reserve(1000);

    result += "[";
    for (auto amplitude : m_curve)
    {
        result += std::to_string(amplitude) + ", ";
    }
    result += "] ";

    result += "max=" + std::to_string(m_maxAmplitude) + ", ";
    result += "deleted=" + std::to_string(m_pointsDeleted) + ".";

    return result;
}
