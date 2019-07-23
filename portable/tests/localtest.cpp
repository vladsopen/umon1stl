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

LocalTest::LocalTest()
{
    registry().push_back(this);
}

std::vector<LocalTest*>& LocalTest::registry()
{
    static std::vector<LocalTest*> s_singletonOnDemand;
    return s_singletonOnDemand;
}

std::string LocalTest::runAll()
{
    std::string testNames;

    for (auto test : registry())
    {
        Io::Os::say("=== Running test " + test->name() + "...\n");

        auto ioTestMode =
            Io::construct(
                Io::Interactivity::batch,
                "Local-" + test->name());

        testNames += test->name() + ", ";

        // turn io variable into a function for LOG()
        auto io = [ioTestMode] { return ioTestMode; };

        io()->debug()->logging(true); //Io::Os::canYeldToEventLoop());
        io()->debug()->diffLogs(true);
        io()->mock()->disableTimerHandler(true);

        auto logs = io()->debug()->logs();
        //std::shared_ptr<Log> drawDetails = logs->drawDetails();

        test->runTest([=] { return io(); });

        Io::Os::say("=== done test " + test->name() + " ===\n");

        ioTestMode->debug()->logs()->finish();
    }

    return testNames;
}
