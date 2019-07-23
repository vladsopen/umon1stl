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
#include "common.h"
#include "io.h"

using namespace std::chrono;

Io::Io(
        Interactivity interactivity,
        const std::string& sessionName) :
    interactivity(interactivity),
    sessionName(sessionName)
{
    rASSERT(sessionName.length() != 0);
}

std::shared_ptr<Io> Io::construct(
        Interactivity interactivity,
        const std::string& sessionName)
{
    struct force_make_shared_accept_private_constructor : public Io
    {
        force_make_shared_accept_private_constructor(
                Interactivity interactivity,
                const std::string& sessionName) :
            Io::Io(interactivity, sessionName) {}
    };

    auto io = std::make_shared<force_make_shared_accept_private_constructor>(
                interactivity, sessionName);

    // save the shared_ptr to itself
    io->m_io = io;

    return io;
}

Io::Debug::Debug(std::shared_ptr<Io> io) :
    Context(io),
    logs(std::make_shared<Logs>(io))
{
}

std::shared_ptr<Io::Debug> Io::debug()
{
    if (!m_debug)
        m_debug = std::make_shared<Debug>(m_io.lock());

    return m_debug;
}

Io::Mock::Mock(std::shared_ptr<Io> io) :
    Context(io),
    nextTick(
        []
        {
            return duration_cast<milliseconds>(
                system_clock::now().time_since_epoch());
        })
{
}

std::shared_ptr<Io::Mock> Io::mock()
{
    if (!m_mock)
        m_mock = std::make_shared<Mock>(m_io.lock());

    return m_mock;
}

/*
void Io::Debug::currentStatus(
        const std::string& s)
{
    if (m_currentStatus == s)
        return;

    m_currentStatus = s;

    m_uiStatus.apply([] (auto ui) { ui->update(); });
}
*/

void Io::Debug::statusBar(
        ParentUi& parent)
{
    // status bar will only be created if it initially has some text
    if (currentStatus() == "")
        return;

    Ui<UiStatic> ui(parent);

    //m_uiStatus += ui;
    currentStatus[ui] = [=] () mutable
        { ui->update(); };

    ui->onUpdate = [=] () mutable
    {
        ui->text(currentStatus());
    };

}
