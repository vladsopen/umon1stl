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
#include "storage.h"
#include "incrementer.h"
#include "emulator.h"
#include "makeappmainwindow.h"
#include "version.h"

App::App(
        std::shared_ptr<Io> io,
        CloseCallback closeCallback) :
    Context(io),
    storage(std::make_shared<Storage>(io)),
    feeder(std::make_shared<Emulator>(io)),
    view(std::make_shared<View>(io)),
    incrementer(std::make_shared<Incrementer>(
        io, view().get(), storage().get(), feeder().get())),
    makeAppMainWindow(std::make_shared<MakeAppMainWindow>(this)),
    m_closeCallback(closeCallback),
    m_io(io)
{
}

void App::runFromMain()
{
    const std::string appAndVersion = "Umon " APP_VERSION;
    const std::string sayAppAndVersion = "\n\n" "  " + appAndVersion + "\n\n";
    Io::Os::say(sayAppAndVersion);
        // print at the top: easy to find

    TestAll::runAtStartup();

    // call this from main() only once!
    {
        static bool doneOnce = false;
        rASSERT(!doneOnce);
        doneOnce = true;
    }

    // this is not a drill! GUI mode in real system
    auto io = Io::construct(Io::Interactivity::gui, appAndVersion);

    //io->debug()->logging(true);

    // make sure the main singleton never freed
    static auto app = std::make_shared<App>(io, [] (App*) {});
    app->open();

    Io::Os::say(sayAppAndVersion);
        // print again after all the test junk
}

void App::open()
{
    rASSERT(!m_initialized);
    rASSERT(!m_terminated);

    m_initialized = true;

    LOG(all, "App", "Creating...");

    makeAppMainWindow()->create();

    LOG(all, "App", "Created.");
}

void App::terminate()
{
    rASSERT(m_initialized);
    rASSERT(!m_terminated);

    m_terminated = true;

    LOG(all, "App", "Terminating...");

    makeAppMainWindow()->close();

    LOG(all, "App", "Terminated.");
}

bool App::isOpen() const
{
    return m_initialized && !m_terminated;
}

void App::requestClose()
{
    rASSERT(isOpen());

    if (m_closeCallback)
        m_closeCallback(this);
    else
        rFAIL("app does not known how to handle close!");
}

