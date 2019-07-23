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
#ifndef TESTALL_H
#define TESTALL_H

#include "application.h"

class App;

// Test internal interfaces in the whole app

class TestAll : public Object
{
public:
    ~TestAll();

    static auto runAll(
            const std::function<void ()>& appCleanup)
            -> std::shared_ptr<TestAll>;

    bool running() const;

    GetSet<std::shared_ptr<Io>> testIoOrNull;

    // called from timer
    void nextAsyncStep();

    static void runAtStartup();

    void spawn();

    static void testButton(
            ParentUi& parent);

private:
    std::function<bool ()> m_step;
    std::function<void ()> m_finish;
    std::function<void ()> m_checkLeaks;
    std::function<void ()> m_appCleanup;

    std::string m_testsRun;

    int m_fakeTick = 12345;
    int m_fakeDelta = 54321;

    std::shared_ptr<TestAll> m_runningThisTestAllOrNull;

    bool m_needToClose = false;

    void startAsync(
            std::shared_ptr<TestAll> thisTest);

    std::shared_ptr<App> initTestApp(
            std::shared_ptr<Io> ioTestMode);

    //void runSequentially();
};

#endif
