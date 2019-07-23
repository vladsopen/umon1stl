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
#ifndef LOG_H
#define LOG_H

// A logging stream contained in Logs

class Log : public Context
{
public:
    explicit Log(
            std::shared_ptr<Io> io,
            const std::string name);

    // sends all text to another log
    GetSet<std::weak_ptr<Log>> redirectTo;

    void silence();

    //
    // *** Use LOG() macro instead of calling this directly! ***
    //

    // start next log entry
    void intro(std::string key);

    // access the stream to simplify << value
    std::stringstream& stream();

    // called after stream() portion is done
    void next();

    // an option to read what was jsut added for log duplication
    std::string added() const;

    //
    // *** end of LOG() macro support ***
    //

    // save new log when done
    void save();

    // for init routines
    static const char* getLogDirToEnsureMounted();

private:
    std::stringstream m_stream;
    std::string m_currentKey;
    int m_posInStream = 0;
    int m_posInOriginal = 0;
    std::string m_originalLog;
    std::string m_actualLog;
    std::string m_firstMismatch;
    bool m_missingErrorReported = false;

    // log internal key = file name
    const GetPredefined<std::string> name;
    std::string fullName() const;

    struct files // (a const namespace)
    {
        static constexpr const char* extension = ".testlog";
        static constexpr const char* logDir = "testlogs";
    };

    void matchAdded(
            const std::string& added);

    std::string makeFileName(
            const std::string& optional);
};

#endif
