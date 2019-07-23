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
#include "incrementer.h"
#include "view.h"
#include "storage.h"
#include "waves.h"
#include "wave.h"
#include "feeder.h"
#include "makeappmainwindow.h"
#include "testscaler.h"

TestAll::~TestAll()
{
}

void TestAll::spawn()
{
    auto io = Io::construct(Io::Interactivity::batch, "Spawned");
    io->debug()->logging(true);
    io->debug()->diffLogs(false);

    auto close = [] (App* app)
    {
        app->terminate();
    };

    std::shared_ptr<App> app = std::make_shared<App>(io, close);

    // make sure the main singleton never freed
    static std::vector<std::shared_ptr<App>> s_apps;
    s_apps.push_back(app); // never free!

    app->open();
}

void TestAll::runAtStartup()
{
    TestScaler().run();
    //TestAll().runAll();
}

auto TestAll::runAll(
        const std::function<void ()>& appCleanup)
        -> std::shared_ptr<TestAll>
{
    auto test = std::make_shared<TestAll>();

    test->m_appCleanup = appCleanup;

    test->m_testsRun += LocalTest::runAll();

    TestScaler().run();
    test->m_testsRun += "TestScaler, ";

    test->startAsync(test);

    return test;
}

void TestAll::startAsync(
        std::shared_ptr<TestAll> thisTest)
{
    using namespace std::chrono;

    // leak checking
    std::weak_ptr<App> pApp;
    std::weak_ptr<MakeAppMainWindow> pMakeAppMainWindow;
    std::weak_ptr<Feeder> pFeeder;
    std::weak_ptr<View> pView;
    std::weak_ptr<Incrementer> pIncrementer;
    std::weak_ptr<Storage> pStorage;
    std::weak_ptr<Waves> pWaves;
    std::weak_ptr<Wave> pWave0;

    // all app objects lifecycle scope
    {
        auto ioTestMode = Io::construct(Io::Interactivity::batch, "TestAll");
        testIoOrNull(ioTestMode);

        // turn io variable into a function for LOG()
        auto io = [ioTestMode] { return ioTestMode; };

        io()->debug()->logging(true);
        io()->debug()->diffLogs(true);
        io()->mock()->disableTimerHandler(true);

        //io()->debug()->runStressTesting(true);

        io()->debug()->currentStatus(
            "Testing...");

        std::shared_ptr<App> app =
            initTestApp(ioTestMode);

        auto logs = io()->debug()->logs();
        std::shared_ptr<Log> drawDetails = logs->drawDetails();

        app->asyncTestAllOrNull(thisTest);

        if (!Io::Os::canYeldToEventLoop())
            io()->debug()->logging(false);

        // save weak pointers
        pApp = app;
        pMakeAppMainWindow = app->makeAppMainWindow();
        pFeeder = app->feeder();
        pView = app->view();
        pIncrementer = app->incrementer();
        pStorage = app->storage();
        pWaves = app->storage()->waves();
        rASSERT(app->storage()->waves()->m_waves.size() > 0);
        pWave0 = app->storage()->waves()->m_waves[0];

        int loops = 300;
        int i = 0;

        // async loop emulation
        m_step = [=] () mutable
        {
            if (i >= loops)
                return false;

            io()->debug()->currentStatus(
                "Testing " + std::to_string(div0(i * 100, /, loops, or 0)) + "%...");

            // change view mode
            if (i % 10 == 0)
                app->view()->forTests.uiMode.apply([] (auto ui) { ui->invokeClick(); });

            // change time scale
            if (i % 11 == 0)
                app->view()->changePointsPerScreen(
                    i < loops / 2? View::decrease : View::increase);

            // quickly go to max points-per-screen to show dead areas
            if (i == loops / 2 - loops / 10)
                for (int j NOTUSED : FromTo(0, 100))
                    app->view()->changePointsPerScreen(
                        View::increase);

            // change amplitude scale
            if (i % (i < loops / 2? 15 : 11) == 0)
                app->view()->changeAmplitudeScale(View::increase);
            if (i % (i < loops / 2? 12 : 14) == 0)
                app->view()->changeAmplitudeScale(View::decrease);

            // imitate [Force] button
            if (i == loops / 2)
            {
                for (int j NOTUSED : FromTo(0, 30))
                {
                    app->storage()->waves()->
                        acquire(
                            app->feeder().get(),
                            app->view().get());
                }
            }

            app->incrementer()->step();

            // temporarily enable verbose paint logs
            if (i == 120)
                drawDetails->redirectTo(std::weak_ptr<Log>());
            if (i == 124)
                drawDetails->silence();
            LOG(drawDetails, "\n" "AtStep", i << "\n--------------------");

            app->view()->forTests.uiGraph.apply([] (auto ui) { ui->repaint(); });

            //Io::Os::yeldToEventLoopForTestingIfSupported();

            ++i;
            return true; // more loops;
        };

        // when loop ends
        m_finish = [=] () mutable
        {
            app->terminate();

            LOG(all, "TestAll", "Ended.");

            // enable separate drawing log
            drawDetails->redirectTo(std::weak_ptr<Log>());

            if (m_needToClose)
            {
                Io::Os::alert("Premature test termination, logs discarded.");
            }
            else
            {
                if (io()->debug()->diffLogs())
                    ioTestMode->debug()->logs()->finish();
            }

            app->asyncTestAllOrNull(nullptr);

            if (!io()->debug()->runStressTesting())
            {
                Io::Os::alert(
                    std::string() +
                        "Testing complete. " +
                        (io()->debug()->logging()? "(have log)" : "(logging was off)") +
                        "\n\n" +
                        std::to_string(
                            std::count_if(
                                m_testsRun.begin(),
                                m_testsRun.end(),
                                [] (char c) { return c == ','; })
                            + 1) +
                        " tests: " + m_testsRun + "and TestAll.");
            }
        };
    }

    // after all ends and lambda captures are killed
    m_checkLeaks = [=] ()
    {
        // now all objects must be dead,
        //   if you hit here you have either a circular dependency
        //   or a surviving strong pointer to something
        rASSERT(!pApp.lock());
        rASSERT(!pMakeAppMainWindow.lock());
        rASSERT(!pFeeder.lock());
        rASSERT(!pView.lock());
        rASSERT(!pIncrementer.lock());
        rASSERT(!pStorage.lock());
        rASSERT(!pWaves.lock());
        rASSERT(!pWave0.lock());
    };
}

void TestAll::nextAsyncStep()
{
    if (m_step)
    {
        // continue the loop?
        if (!m_step() ||
            m_needToClose)
        {
            m_finish();

            // release refs now for leak checks!
            m_step = nullptr;
            m_finish = nullptr;

            m_checkLeaks();

            if (m_appCleanup)
            {
                m_appCleanup();
                m_appCleanup = nullptr;
            }
        }
    }
}

bool TestAll::running() const
{
    return !!m_step;
}

std::shared_ptr<App> TestAll::initTestApp(
        std::shared_ptr<Io> ioTestMode)
{
    using namespace std::chrono;

    auto io = [ioTestMode] { return ioTestMode; };

    auto logs = io()->debug()->logs();
    std::shared_ptr<Log> drawDetails = logs->drawDetails();

    // mute detailed logs tested elsewhere
    logs->scalerDetails()->silence();

    // run random fake RTC timer
    io()->mock()->nextTick(
        [=] () mutable -> milliseconds
        {
            m_fakeDelta += 13;
            m_fakeTick += m_fakeDelta % 1000;
            return milliseconds(m_fakeTick);
        });

    auto close = [this] (App*)
    {
        m_needToClose = true;
    };

    std::shared_ptr<App> app = std::make_shared<App>(io(), close);

    //static std::vector<std::shared_ptr<App>> s_apps;
    //s_apps.push_back(app); // never free!

    app->open();

    LOG(all, "TestAll", "Ready...");

    // call initial volatile updates without logging
    //  (log result will depend on random repaint/message processing sequence)
    io()->debug()->logging(false);
    Io::Os::yeldToEventLoopForTestingIfSupported();
    app->makeAppMainWindow()->updateAllStates();
    io()->debug()->logging(true);

    LOG(all, "TestAll", "Started!");

    return app;
}

void TestAll::testButton(
        ParentUi& parent)
{
    //
    // Run test button
    //
    Ui<UiButton> ui(parent);

    ui->text("Test");

    static std::shared_ptr<TestAll> s_nullOrRunningTestAll;
    ui->onClick = [=] () mutable
    {
        //insideTest = true;
        //auto&& clear = cleaner([=] () mutable { insideTest = false; });

        s_nullOrRunningTestAll = TestAll().runAll([] {});
    };

    ui->timer(1000);
    ui->onTimer = [=] () mutable
    {
        // this uglines is needed for strict memory test
        //   and also tests local Io usage

        if (s_nullOrRunningTestAll)
        {
            auto ioTest = s_nullOrRunningTestAll->testIoOrNull();
            if (ioTest)
            {
                if (ioTest->debug()->runStressTesting())
                {
                    if (!s_nullOrRunningTestAll->running())
                    {
                        if (ioTest->debug()->runStressTesting())
                            ui->invokeClick();
                    }
                }
            }
        }
    };
}

/*
void TestAll::runSequentially()
{
    // *************************************
    // EXPERIMENTAL COPYPASTE TO BE REMOVED!
    // *************************************

    using namespace std::chrono;

    // leak checking
    std::weak_ptr<App> pApp;
    std::weak_ptr<MakeAppMainWindow> pMakeAppMainWindow;
    std::weak_ptr<Feeder> pFeeder;
    std::weak_ptr<View> pView;
    std::weak_ptr<Incrementer> pIncrementer;
    std::weak_ptr<Storage> pStorage;
    std::weak_ptr<Waves> pWaves;
    std::weak_ptr<Wave> pWave0;

    // all app objects lifecycle scope
    {
        auto ioTestMode = Io::construct(Io::batch, "TestAll");

        // turn io variable into a function for LOG()
        auto io = [ioTestMode] { return ioTestMode; };

        io()->debug()->logging(true);
        io()->debug()->disableTimerHandler(true);

        auto logs = io()->debug()->logs();
        std::shared_ptr<Log> drawDetails = logs->drawDetails();

        std::shared_ptr<App> app =
            initTestApp(ioTestMode);

        // *************************************
        // EXPERIMENTAL COPYPASTE TO BE REMOVED!
        // *************************************

        int loops = 300;
        for (int i : FromTo(0, loops))
        {
            // change view mode
            if (i % 10 == 0)
                app->view()->uiMode.apply([] (auto ui) { ui->invokeClick(); });

            // change time scale
            if (i % 11 == 0)
                app->view()->changePointsPerScreen(
                    i < loops / 2? View::decrease : View::increase);

            // quickly go to max points-per-screen to show dead areas
            if (i == loops / 2 - loops / 10)
                for (int iScale : FromTo(0, 100))
                    app->view()->changePointsPerScreen(
                        View::increase);

            // change amplitude scale
            if (i % (i < loops / 2? 15 : 11) == 0)
                app->view()->changeAmplitudeScale(View::increase);
            if (i % (i < loops / 2? 12 : 14) == 0)
                app->view()->changeAmplitudeScale(View::decrease);

            // imitate [Force] button
            if (i == loops / 2)
            {
                for (int iForce : FromTo(0, 30))
                    app->storage()->waves()->acquire();
            }

            app->view()->incrementer()->step();

            // temporarily enable verbose paint logs
            if (i == 120)
                drawDetails->redirectTo(std::weak_ptr<Log>());
            if (i == 124)
                drawDetails->silence();
            LOG(drawDetails, "\n" "AtStep", i << "\n--------------------");

            app->view()->uiGraph.apply([] (auto ui) { ui->repaint(); });

            //Io::Os::yeldToEventLoopForTestingIfSupported();
        }

        // *************************************
        // EXPERIMENTAL COPYPASTE TO BE REMOVED!
        // *************************************

        // save weak pointers
        pApp = app;
        pMakeAppMainWindow = app->makeAppMainWindow();
        pFeeder = app->feeder();
        pView = app->view();
        pIncrementer = app->view()->incrementer();
        pStorage = app->storage();
        pWaves = app->storage()->waves();
        rASSERT(app->storage()->waves()->m_waves.size() > 0);
        pWave0 = app->storage()->waves()->m_waves[0];

        app->terminate();

        LOG(all, "TestAll", "Ended.");

        // enable separate drawing log
        drawDetails->redirectTo(std::weak_ptr<Log>());

        ioTestMode->debug()->logs()->finish();
    }

    // now all objects must be dead,
    //   if you hit here you have either a circular dependency
    //   or a surviving strong pointer to something
    rASSERT(!pApp.lock());
    rASSERT(!pMakeAppMainWindow.lock());
    rASSERT(!pFeeder.lock());
    rASSERT(!pView.lock());
    rASSERT(!pIncrementer.lock());
    rASSERT(!pStorage.lock());
    rASSERT(!pWaves.lock());
    rASSERT(!pWave0.lock());

    Io::Os::alert("Test is complete.");

    if (m_appCleanup)
    {
        m_appCleanup();
        m_appCleanup = nullptr;
    }
}
*/
