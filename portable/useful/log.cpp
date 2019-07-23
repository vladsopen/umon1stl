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

Log::Log(
        std::shared_ptr<Io> io,
        const std::string name) :
    Context(io),
    name(name)
{
    // read the original log from file
    std::ifstream file(makeFileName(""), std::ios::binary);
    if (file.is_open())
    {
        std::ostringstream wholeFile;
        wholeFile << file.rdbuf();
        m_originalLog = wholeFile.str();
    }
}

void Log::silence()
{
    redirectTo(io()->debug()->logs()->silence());
}

void Log::save()
{
    if (auto redirect = redirectTo().lock())
    {
        UNUSED(redirect);
        return;
    }

    if (this == io()->debug()->logs()->silence().get())
        return;

    // save the original log from file
    auto newName = makeFileName("-new");
    //if (m_firstMismatch.length() ||
    //    m_originalLog.length() == 0)
        // lets resave everytime for debugging if we dont merge
        //   all the changes every time
    {
        std::ofstream file(newName, std::ios::binary);
        rASSERT(file.is_open());
        file << m_actualLog;
        file.close();
        if (file.bad())
        {
            LOG_FAIL(critical, "Logs", "cannot write log file");
        }
    }

    // show diff if we have a mismatch
    if (m_firstMismatch.length() &&
        !io()->debug()->runStressTesting())
    {
        Io::Os::alert(
            std::string() +
                "Test log " + fullName() + " has changed!\n\n"
                "You need to review the changes and either fix the code "
                "or update the log using the diff tool which will "
                "start at the end of the test if your platform supports it.\n\n" +
                "Details:\n" + m_firstMismatch);

        Io::Os::diffTwoFiles(newName, makeFileName(""));
    }
}

void Log::intro(std::string key)
{
    if (auto redirect = redirectTo().lock())
    {
        redirect->intro(key);
        return;
    }

    m_currentKey = key;
    Io::Os::say("TEST LOG - " + key + ": ");
}

std::stringstream& Log::stream()
{
    if (auto redirect = redirectTo().lock())
    {
        return redirect->stream();
    }

    return m_stream;
}

std::string Log::added() const
{
    if (auto redirect = redirectTo().lock())
    {
        return redirect->added();
    }

    // cut all added chars as the last chunk
    std::string fullStream = m_stream.str();
    std::string addedToStream =
        fullStream.substr(m_posInStream, fullStream.length() - m_posInStream);

    return addedToStream;
}

void Log::next()
{
    if (auto redirect = redirectTo().lock())
    {
        return redirect->next();
    }

    // cut all added chars as the last chunk
    std::string fullStream = m_stream.str();
    std::string addedToStream =
        added();
        //fullStream.substr(m_posInStream, fullStream.length() - m_posInStream);

    Io::Os::say("<" + addedToStream + ">\n");

    if (!io()->debug()->diffLogs())
    {
        // release memory
        m_stream.str("");
        return;
    }

    // make the statement we will mathch to the original
    std::string addedStatement = m_currentKey + ": " + addedToStream + "\n";
    m_actualLog += addedStatement;

    matchAdded(addedStatement);

    // prepare to the next call to LOG()
    m_posInOriginal += useful::length(addedStatement);
    m_posInStream = useful::length(fullStream);
}

void Log::matchAdded(
        const std::string& added)
{
    if (this == io()->debug()->logs()->silence().get())
        return;

    if (!io()->debug()->diffLogs())
        return;

    // if we have the original log lets compare with it
    if (m_originalLog.length() == 0)
    {
        if (Io::Os::testLogsAvailable())
        {
            if (!m_missingErrorReported)
            {
                m_missingErrorReported = true;

                Io::Os::say("\n" "**** ERROR: missing " + fullName() + " log ****\n");
                rFAIL("missing a testlog file");
            }
        }
    }
    else
    {
        std::string mismatch;
        if (m_originalLog.length() >= m_posInOriginal + added.length())
        {
            // the respective fragment from the original log
            std::string expected =
                m_originalLog.substr(m_posInOriginal, added.length());

            if (expected != added)
            {
                mismatch =
                    "expected: <" + expected + ">\n"
                    "produced: <" + added + ">\n";
            }
        }
        else
        {
            mismatch =
                "original log is shorter than the current stream, "
                    "dangling text starts at <" + added + ">";
        }

        if (mismatch.length())
        {
            if (m_firstMismatch.length() == 0)
            {
                m_firstMismatch = mismatch;
                //rFAIL(mismatch.c_str());

                /* we wanted to alert earlier but we need to avoid
                 *   entering message loop DURING batch test for desktop mode
                Io::Os::alert(
                    std::string() + "Test log " + fullName() + " has changed!\n\n"
                        "You need to review the changes and either fix the code "
                        "or update the log using the diff tool which will "
                        "start at the end of the test if your platform supports it.\n\n" +
                        "Details: " + m_firstMismatch);
                */
            }
        }
    }
}

std::string Log::fullName() const
{
    return io()->sessionName() + "-" + name();
}

const char* Log::getLogDirToEnsureMounted()
{
    return files::logDir;
}

std::string Log::makeFileName(
        const std::string& optional)
{
    return
        std::string() +
            files::logDir + "/" +
            fullName() + optional + files::extension;
}

