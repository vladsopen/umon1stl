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
#ifndef IO_H
#define IO_H

class ParentUi;

//
// The environment in which app is running: GUI, batch, testing, logging,...
//
//   This is basically everything-outside-of-application.
//   All application-related inputs, outputs and system communication must
//   be virtualized for testing and other batch jobs.
//   This also allows us to have multiple independent application sessions running
//   with their own Ui-framework contexts, even own main windows, which cooperate properly.

class Io
{
public:
    enum class Interactivity { gui, batch };

private:
    explicit Io(
            Interactivity interactivity,
            const std::string& sessionName);
    friend struct force_make_shared_accept_private_constructor;
public:
    static std::shared_ptr<Io> construct(
            Interactivity interactivity,
            const std::string& sessionName);

    // batch mode is intended for tests and will ensure testlog consistency
    const GetPredefined<Interactivity> interactivity;

    // mostly for debug context
    const GetPredefined<std::string> sessionName;
        // (this is also the name for a separate test log file)

    // (we have to make all dependent objects init-on-demand to
    // avoid circular dependency with shared_ptr<Io> initialization)

    // global logs
    class Debug : public Context
    {
    public:
        explicit Debug(std::shared_ptr<Io> io);

        const GetUnique<Logs> logs;
        GetSet<bool> logging = false;
        GetSet<bool> diffLogs = false;

        GetSet<bool> runStressTesting = false;

        GetSetTell<std::string> currentStatus;

        void statusBar(
                ParentUi& parent);

    private:
    };
    std::shared_ptr<Debug> debug();

    // system virtualization
    class Mock : public Context
    {
    public:
        explicit Mock(std::shared_ptr<Io> io);

        GetSet<std::function<std::chrono::milliseconds ()>> nextTick;

        GetSet<bool> disableTimerHandler = false;
    };
    std::shared_ptr<Mock> mock();

    // global OS fuctions
    class Os
    {
        Os() = delete;
    public:

        // format a valid unicode character
        static std::string unicode(int code);

        // internal UI debugging mode
        static bool debug();

        // console output
        static void say(const std::string& output);

        // message box
        static void alert(const std::string& output);

        // unrecoverable abnornal termination
        static void fatal(const std::string& message);

        // this is unsafe and not supported on some platforms
        static void yeldToEventLoopForTestingIfSupported();
        static bool canYeldToEventLoop();

        // open GUI diff tool for mismatched test logs
        static void diffTwoFiles(
                const std::string& originalFile,
                const std::string& changedFile);

        // chekc if log files bundled with exe
        static bool testLogsAvailable();
    };

private:
    std::weak_ptr<Io> m_io;
    std::shared_ptr<Debug> m_debug;
    std::shared_ptr<Mock> m_mock;
};

#endif
