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
#include "storage.h"
#include "feeder.h"
#include "view.h"
#include "wave.h"
#include "waves.h"
#include "drawwave.h"
#include "drawback.h"
#include "drawblock.h"
#include "drawtest.h"

Waves::Waves(
        std::shared_ptr<Io> io) :
    Context(io)
{
    // demo channels
    for (int i NOTUSED : FromTo(0, Storage::memory::storedChannels))
    {
        m_waves.push_back(std::make_shared<Wave>(size(), io));
    }
}

int Waves::size() const
{
    return useful::size(m_waves);
}

void Waves::forEachWave(
        std::function<void (const Wave* wave)> f)
        const
{
    for (const auto wave : m_waves)
        if (f)
            f(wave.get());
}

bool Waves::isLast(
        const Wave* wave)
        const
{
    return useful::size(m_waves) && m_waves.back().get() == wave;
}

int Waves::pointsRecorded() const
{
    return m_pointsRecorded;
}

int Waves::pointsLastBlock() const
{
    return m_pointsLastBlock;
}

int Waves::pointsMaxDeleted() const
{
    if (m_waves.begin() == m_waves.end())
    {
        rFAIL("must have waves!");
        return 0;
    }

    // 1. first convoluted way of finding max(wave[i].deleted)
    int max1 =
        std::accumulate(
            m_waves.begin(),
            m_waves.end(),
            std::numeric_limits<int>::min(),
            [] (int max, auto wave)
                { return std::max(max, wave->pointsDeleted()); });

    // 2. uglier, longer, unsafer but supposedly more idiomatic
    //      also the accessor is duplicated 3 times!
    int max2 =
        (*std::max_element(
            m_waves.begin(),
            m_waves.end(),
            [] (auto wave1, auto wave2)
                { return wave1->pointsDeleted() < wave2->pointsDeleted(); }))
            ->pointsDeleted();

    // 3. the sane way
    int max = std::numeric_limits<decltype(max)>::min();
    for (auto wave : m_waves)
        max = std::max(max, wave->pointsDeleted());

    rASSERT(max1 == max);
    rASSERT(max1 == max2); //TODO: performance!  profile both, leave one
                                // preliminary: (1) is a little faster, == (3)
    rASSERT(max >= 0);

    return max;
}

/* profiling:
 *
    auto now = []
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    };

    auto t0 = now();
    for (auto i NOTUSED : FromTo(1, 10000000))
    {
        int max =
            // debug: 38000 release 331
            std::accumulate(
                m_waves.begin(),
                m_waves.end(),
                0,
                [] (int max, auto wave)
                    { return std::max(max, wave->pointsDeleted()); });
        or
            // debug 37000 release 332
            std::accumulate(
                m_waves.begin(),
                m_waves.end(),
                0,
                [] (int max, auto wave)
                    { return std::max(max, wave->pointsDeleted()); });
        or
            // debug: 37000 release: 331
            for (auto wave : m_waves)
                max = std::max(max, wave->pointsDeleted());


        UNUSED(max);
    }
    auto dt = now() - t0;
    static long long s_average = dt.count();
    s_average = (s_average * 10 + dt.count()) / 11;
    Io::Os::say("Time: " + std::to_string(dt.count()) + " ~" + std::to_string(s_average) + "\n");
*/

LOCAL_TEST(TestFindMax)(PassIo io)
{
    // (this is a test for LOCAL_TEST and the corresponding logging facilities)

    Waves waves(io());

    int i = -1;
    int max = -1;
    for (auto wave : waves.m_waves)
    {
        ++i;

        wave->m_pointsDeleted = (i * 100) % 128;

        max = std::max(max, wave->m_pointsDeleted);

        LOG(all, "Max", "[" << i << "] = " << wave->m_pointsDeleted);
    }

    auto tested = waves.pointsMaxDeleted();

    rASSERT(tested == max);

    // (really uneeded log diff invocation for such a simple case)
    LOG(all, "Max",
        "Good result = " << max <<
            ", maxDeletedPoints() = " << tested);
}

void Waves::acquire(
        Feeder* feeder,
        View* view)
{
    int maxStorePoints =
        view->pointsPerScreen() * Storage::memory::maxCacheScreens;
    int flushOldPoints =
        view->pointsPerScreen();

    acquire(
        [feeder] (
                int channel,
                Out, std::vector<int>& wave,
                Out, int& maxAmplitude)
        {
            feeder->
                acquire(
                    channel,
                    Out(), wave,
                    Out(), maxAmplitude);
        },
        maxStorePoints,
        flushOldPoints);
}

void Waves::acquire(
        Reader reader,
        int maxStorePoints,
        int flushOldPoints)
{
    std::vector<int> portion;

    int i = -1;
    int count = -1;
    for (auto wave : m_waves)
    {
        ++i;

        int maxAmplitude = -1;

        reader(
            i,
            Out(), portion,
            Out(), maxAmplitude);

        rASSERT(maxAmplitude >= 0);

        wave->
            append(
                portion,
                As<int, class MaxAmplitude>(maxAmplitude),
                As<int, class MaxStorePoints>(maxStorePoints),
                As<int, class FlushOldPoints>(flushOldPoints));

        // all added sizes must be equal
        int countAdded = useful::size(portion);
        rASSERT(countAdded == count || count == -1);
        count = countAdded;
    }

    m_pointsRecorded += count;
    m_pointsLastBlock = count;
}

void Waves::dump(
        const std::string& comment)
        const
{
    int i = -1;
    for (auto wave : m_waves)
    {
        i++;

        LOG(all, "Wave",
            comment << " [" << std::to_string(i) << "]: " << wave->report());
    }

    LOG(all, "Waves",
        comment << " total " << std::to_string(m_pointsRecorded) << "pts "
        "(" << std::to_string(m_pointsLastBlock) << "pt last)"
        "\n");
}

LOCAL_TEST(TestWaveAcquire)(PassIo io)
{
    Waves waves(io());

    std::vector<int> sendBlock = { 1, 2, 3, 4, -55555 };
    int sendMaxAmplitude = 111;

    int maxStorePoints = 3;
    int flushOldPoints = 1;
    int limitTestPoints = 0;

    // fake Feeder
    auto reader = [&] (
            int channel,
            Out, std::vector<int>& wave,
            Out, int& maxAmplitude)
    {
        wave =
            // add only limited number of points
            std::vector<int>(
                sendBlock.begin(),
                sendBlock.begin() + std::min(useful::size(sendBlock), limitTestPoints));

        maxAmplitude = sendMaxAmplitude + channel * 100;
    };

    waves.dump("Empty:");

    limitTestPoints = 0; // add nothing at first

    for (int iRepeat : FromTo(0, 10))
    {
        waves.acquire(
            reader,
            maxStorePoints,
            flushOldPoints);

        waves.dump("Loop" + std::to_string(iRepeat) + ":");

        // modify added blocks
        sendMaxAmplitude = (sendMaxAmplitude + 13) % 41; // (random)
        sendBlock.push_back(1000 + useful::size(sendBlock));
        sendBlock.push_back(2000 + useful::size(sendBlock));

        // gradually increase storage
        maxStorePoints += iRepeat & 2;

        limitTestPoints = 10000; // remove limit
    }

    // (matched against Local-TestWaveAcquire-all.testlog)
}


