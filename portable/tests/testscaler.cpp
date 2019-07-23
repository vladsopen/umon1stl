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
#include "view.h"
#include "scaler.h"
#include "testscaler.h"

void TestScaler::run()
{
    auto ioTestMode = Io::construct(Io::Interactivity::batch, "TestScaler");

    // turn io variable into a function for LOG()
    auto io = [ioTestMode] { return ioTestMode; };

    io()->debug()->logging(true);
    auto logs = io()->debug()->logs();
    logs->scalerDetails()->redirectTo(logs->all());

    LOG(all, "TestScaler", "Started!");

    auto min = Scaler::defaults::pointsPerScreenMin;
    auto max = Scaler::defaults::pointsPerScreenMax;
    auto delta = Scaler::defaults::pointsPerScreenDelta;

    As<int, class RecursionDepth> n;

    Scaler(io()).nextPointsPerScreen(1000, View::decrease, 1000, "Nice match case");
    Scaler(io()).nextPointsPerScreen(100, View::decrease, 1000, "Quick end- case");
    Scaler(io()).nextPointsPerScreen(10000, View::increase, 1000, "Quick end+ case");
    Scaler(io()).nextPointsPerScreen(111, View::increase, 112, "Unmatched+1 case");
    Scaler(io()).nextPointsPerScreen(111, View::increase, 110, "Unmatched-1 case");
    Scaler(io()).nextPointsPerScreen(1234, View::increase, 11, "Small screen");
    Scaler(io()).nextPointsPerScreen(12, View::increase, 100000, "Huge screen");
    Scaler(io()).nextPointsPerScreen(max, View::increase, 123, "Clamped max");
    Scaler(io()).nextPointsPerScreen(min, View::decrease, 321, "Clamped min");

    // lets try to increase to nearly max with both previous and new scales perfectly
    //   matching screen size (divisible by prev and next pppx) to force
    //   check limiting only by insufficient delta
    Scaler(io()).nextPointsPerScreen(max - 1, View::increase,
        (max - 1) * (max - 1 - delta), "Nearly max");
    Scaler(io()).nextPointsPerScreen(min + 1, View::decrease,
        (min + 1) * (min + 1 + delta), "Nearly min");

    Scaler(io()).nextPointsPerScreen(0, View::decrease, 0, "Zero case");
    Scaler(io()).nextPointsPerScreen(0, View::increase, 0, "Zero case");
    Scaler(io()).nextPointsPerScreen(1, View::decrease, 0, "1-Zero case");
    Scaler(io()).nextPointsPerScreen(0, View::increase, 1, "Zero-1 case");

    LOG(all, "TestScaler", "Ended.");

    // see TestScaler-all.testlog for asserted results
    ioTestMode->debug()->logs()->finish();

    //Io::Os::alert("Test is complete.");
}
