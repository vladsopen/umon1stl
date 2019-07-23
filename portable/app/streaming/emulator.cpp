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
#include "emulator.h"
#include "storage.h"

Emulator::Emulator(std::shared_ptr<Io> io) :
    Feeder(io)
{
    // we just send the same predefined blocks for the demo app
    //   (transformed just to match each other nicer)

    // channel[0]
    {
        std::vector<int> block =
            { -82, -79, -75, -78, -78, -80, -83, -81, -77, -79, -80, -85, -81, -78, -85, -84, -84,
              -79, -81, -76, -83, -99, -90, -90, -56, 35, 206, 332, 314, 303, 148, 46, -4, -34, -40,
              -35, -40, -50, -59, -59, -61, -59, -63, -62, -62, -60, -58, -56, -54, -58, -66, -62,
              -49, -49, -46, -43, -39, -37, -40, -35, -28, -23, -21, -17, -14, -8, -2, 8, 20, 30,
              39, 46, 53, 64, 74, 86, 98, 105, 112, 124, 133, 137, 134, 132, 127, 120, 111, 94, 86,
              68, 48, 33, 17, 4, -9, -16, -19, -23, -28, -29, -29, -29, -29, -30, -29, -27, -29,
              -29, -25, -24, -27, -31, -28, -25, -25, -26, -28, -27, -28, -27, -28, -28, -30, -30,
              -33, -34, -39, -37, -35, -35, -39, -40, -42, -43, -44, -43, -39, -41, -46, -47, -46,
              -45, -48, -48, -46, -45, -48, -48, -45, -46, -48, -49, -49, -46, -48, -48, -49, -50,
              -51, -53, -45, -41, -37, -33, -27, -24, -22, -7, 8, 12, 15, 17, 13, 11, 5, -6, -13,
              -11, -22, -35, -38, -47, -56, -66, -65, -68, -69, -71, -68, -72, -75, -74, -74, -71,
              -74, -75, -75, -72, -74, -73, -69, -66, -73, -90, -86, -74, -53, 22, 190, 356, 351,
              324, 203, 72, 10, -19, -31, -27, -32, -42, -45, -48, -52, -55, -55, -54, -54, -54,
              -53, -50, -51, -47, -47, -43, -44, -43, -41, -37, -35, -34, -31, -29, -25, -22, -17,
              -14, -10, -3, 7, 14, 19, 29, 42, 51, 60, 73, 82, 96, 104, 107, 116, 121, 125, 128,
              133, 132, 126, 117, 106, 91, 72, 52, 39, 23, 7, -8, -16, -24, -32, -37, -38, -39,
              -44, -45, -42, -42, -46, -46, -43, -44, -46, -46, -44, -44, -47, -47, -47, -45, -46,
              -47, -45, -43, -46, -50, -47, -47, -52, -54, -52, -53, -56, -60, -58, -58, -59, -63,
              -62, -61, -65, -65, -64, -66, -66, -67, -64, -64, -67, -69, -69, -67, -70, -70, -67,
              -70, -71, -74, -71, -66, -69, -72, -69, -67, -69, -67, -59, -51, -49, -44, -41, -35,
              -15, -15, -7, 0, -4, -10, -15, -19, -32, -38, -36, -43, -58, -65, -70, -76, -84, -87,
              -85, -84, -90, -91, -90, -91, -93, -93, -90, -90, -93, -94, -93, -92, -92, -90, -87,
              -86, -85, -83, -85, -82, };

        std::transform(
            block.begin(), block.end(), block.begin(),
            [] (int a) { return static_cast<int>(1.1 * (a - 80)); });

        m_templates.push_back(block);
    }

    // channel[1]
    {
        std::vector<int> block =
            { -38, -38, -29, -26, -27, -26, -24, -26, -41, -45, -40, -38, -35, -36, -34, -33, -33,
              -24, -23, -25, -41, -41, -21, -53, -73, -103, -84, -95, -124, -18, -33, -33, -21, -20,
              -22, -13, -23, -20, -14, -15, -28, -35, -29, -27, -31, -27, -22, -16, -26, -22, -17,
              -15, -22, -22, -22, -17, -17, -24, -21, -22, -23, -19, -21, -17, -13, -14, -16, -10,
              -4, -7, -12, -3, -6, -13, -19, -4, -1, -14, -12, 1, 1, 2, -4, 4, 14, 14, -4, -10, -8,
              0, 12, 0, -11, -7, -10, -24, -28, -21, -10, -10, -20, -22, -11, -19, -27, -29, -19,
              -7, -10, -23, -30, -22, -19, -20, -25, -25, -16, -18, -23, -27, -27, -22, -27, -25,
              -22, -19, -22, -26, -31, -29, -22, -29, -29, -18, -21, -36, -36, -28, -34, -39, -26,
              -18, -26, -36, -33, -30, -30, -25, -18, -24, -34, -32, -27, -26, -30, -34, -25, -27,
              -26, -27, -31, -36, -24, -13, -11, -9, -10, -10, 3, 17, 11, -6, -7, -7, -16, -26,
              -31, -25, -20, -22, -28, -23, -29, -41, -41, -34, -30, -33, -46, -51, -52, -45, -36,
              -44, -53, -55, -50, -42, -45, -41, -40, -40, -45, -55, -45, -48, -72, -113, -92, -64,
              -108, -43, -10, -33, -32, -33, -32, -34, -29, -22, -19, -29, -37, -40, -34, -27, -22,
              -25, -35, -38, -35, -34, -32, -32, -32, -34, -32, -34, -32, -33, -31, -26, -30, -27,
              -24, -16, -10, -19, -32, -31, -24, -27, -30, -22, -12, -13, -21, -19, -19, -19, -14,
              0, 4, -2, -6, -18, -17, -10, 3, 8, -2, -20, -20, -20, -21, -23, -23, -22, -21, -20,
              -28, -34, -37, -32, -29, -28, -22, -22, -30, -30, -24, -26, -34, -37, -30, -31, -32,
              -24, -18, -28, -35, -32, -31, -31, -34, -32, -24, -20, -30, -33, -29, -27, -26, -34,
              -39, -38, -30, -28, -37, -34, -28, -22, -27, -39, -41, -31, -23, -26, -36, -32, -21,
              -26, -28, -25, -24, -42, -35, -36, -39, -35, -32, -20, -27, -43, -36, -28, -23, -19,
              -10, -3, 1, -4, -4, -13, -6, -5, -9, -13, -27, -44, -35, -29, -33, -31, -34, -37,
              -40, -43, -44, -49, -43, -34, -30, -34, -46, -53, -53, -51, -46, -35, -34, -36, -42,
              -48, -46, -46, -39, -26, -57, -82, };

        std::transform(
            block.begin(), block.end(), block.begin(),
            [] (int a) { return static_cast<int>(3.0 * (a + 60)); });

        m_templates.push_back(block);
    }

    // channel[2]
    {
        std::vector<int> block =
            { 1, 0, 1, 1, 3, 1, 4, 1, -1, -5, -3, -2, 0, -1, 0, -1, -2, 4, 2, 1, -11, -9, 12, 1, 9,
              37, 117, 121, 80, 140, 95, 65, 46, 24, 11, 12, 2, 1, 2, 1, -7, -11, -10, -10, -13,
              -10, -7, -6, -9, -13, -21, -14, -10, -10, -11, -8, -9, -12, -12, -13, -13, -10, -14,
              -14, -16, -16, -18, -18, -14, -15, -20, -19, -31, -34, -37, -32, -32, -38, -35, -24,
              -19, -21, -23, -20, -11, -2, 1, 2, 9, 12, 16, 13, 9, 9, 6, 3, 2, 3, 8, 8, 4, 2, 8, 3,
              -1, -3, 1, 5, 5, 0, -3, -3, -3, 0, 0, 0, 3, 2, -2, -2, 1, 2, 0, -2, -1, 0, 0, -4, -3,
              -3, 0, -2, -5, -1, -2, -6, -7, -4, -5, -6, -3, -2, -3, -7, -6, -3, -5, -3, 0, -2, -5,
              -4, -2, -2, -4, -5, -4, -3, -4, -3, -3, -5, 4, 8, 9, 9, 8, 14, 24, 34, 32, 27, 24,
              23, 17, 8, 4, 7, 6, 2, 0, 1, -2, -9, -9, -9, -7, -8, -10, -12, -12, -11, -9, -10,
              -14, -14, -12, -6, -10, -10, -9, -8, -14, -20, -7, 5, 0, 14, 98, 143, 92, 120, 117,
              68, 43, 18, 4, 0, -3, -5, -4, -8, -12, -15, -14, -13, -11, -12, -15, -17, -18, -18,
              -15, -14, -18, -18, -18, -18, -19, -19, -20, -20, -20, -21, -21, -20, -19, -22, -27,
              -28, -28, -29, -32, -30, -29, -28, -29, -27, -29, -29, -25, -20, -14, -16, -14, -14,
              -13, -3, 4, 8, 6, 0, 1, 3, 4, 0, 3, 3, 2, 3, 1, -3, -5, -4, 0, 1, 0, 1, -2, -1, 0,
              0, -3, -5, -2, -5, -4, 1, 2, -3, -6, -5, -6, -5, -7, -6, -4, -1, -4, -5, -4, -1, 0,
              -3, -5, -4, -2, -3, -6, -6, -2, -2, -3, -7, -8, -4, -3, -2, -6, -4, -1, -2, -4, -5,
              -3, -8, -5, -7, -9, -9, -7, -2, -4, -10, -7, -3, -3, -1, 0, 6, 16, 19, 21, 18, 19,
              17, 16, 9, 1, -5, 0, 0, -6, -5, -5, -9, -13, -14, -14, -17, -17, -14, -12, -12, -16,
              -20, -18, -17, -16, -12, -11, -11, -14, -17, -17, -20, -18, 1, -8, -5, };

        std::transform(
            block.begin(), block.end(), block.begin(),
            [] (int a) { return static_cast<int>(3.0 * (a - 30)); });

        m_templates.push_back(block);
    }

    // some maintenance:
    // check sizes - all must match, the demo client has no recovery and resync features
    size_t size = 0;
    for (const auto& block : m_templates)
    {
        // check sizes
        rASSERT(block.size() == size || size == 0);
        size = block.size();

        // init debug noise state
        //m_noise.push_back(iBlock);
        m_noise.push_back(0);
    }

    rASSERT(Storage::memory::storedChannels == m_noise.size());
}

int Emulator::pointsPerSecond() const
{
    // here we fix it for simplicity, in reality it is fixed for a subsession
    return defaults::pointsPerSecond;
}

void Emulator::acquire(
        int channel,
        Out, std::vector<int>& wave,
        Out, int& maxAmplitude)
{
    if (channel < 0 || channel > useful::size(m_templates))
    {
        LOG_FAIL(critical, "Emulator", "unsupported channel requested from the Emulator!");

        return;
    }

    // just send them more clones
    wave = m_templates[channel];

    maxAmplitude = defaults::maxAmplitude;

    int noise = m_noise[channel];
    if (noise > 0)
    {
        m_noise[channel]--;

        //m_uiNoise.apply([] (auto ui) { ui->update(); });
        m_observeNoise.fire();

        int rnd = 0;
        int range = 50 * noise + 10;
        for (auto& amplitude : wave)
        {
            rnd += noise + rand() % (range | 1) - range / 2;
            amplitude += rnd;
        }
    }

    LOG(all, "EmulatorStep", channel << "th channel");
}

void Emulator::toolbar(
        ParentUi& parent)
{
    Ui<UiButton> ui(parent);

    // update button text when we eat up some noise
    m_observeNoise[ui] = [=] () mutable
        { ui->update(); };

    ui->onUpdate = [=] () mutable
    {
        ui->text(
            "Noise" + std::string() +
                (m_noise[0] > 0? "+" + std::to_string(m_noise[0]) : ""));
    };

    ui->onClick = [=] () mutable
    {
        for (auto& noise : m_noise)
            ++noise;
    };
}
