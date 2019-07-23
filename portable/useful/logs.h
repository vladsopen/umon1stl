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
#ifndef LOGS_H
#define LOGS_H

#include "log.h"

// Global logging for tests with multiple independent Log streams

class Logs : public Context
{
public:
    explicit Logs(std::shared_ptr<Io> io);

    // very special main log to which we redirect and mix in most other logs
    const GetUnique<Log> all;

    // redirect to this fake log to silence your log completely
    const GetUnique<Log> silence;

    // for serious errors
    const GetUnique<Log> critical;

    // specific logs
    //   they maybe useful in separated files or redirected to 'all'
    //   or disabled for system tests
    //TODO: Decouple universal parts of the test framework from app specifics.
    //      Here we'll need either app supplied log container
    //      or just reference them by string keys.
    const GetUnique<Log> scalerDetails;
    const GetUnique<Log> incrementerDetails;
    const GetUnique<Log> drawEssential;
    const GetUnique<Log> drawDetails;

    // complete all logging, report diffs
    void finish();

private:
    const std::vector<std::shared_ptr<Log>> m_logs;
};

// simple optional logging operator

#define LOG_ALWAYS(log, key, values) \
    do { \
        std::shared_ptr<Log> logTo = io()->debug()->logs()->log(); \
        logTo->intro(key); \
        logTo->stream() << values; \
        logTo->next(); \
    } while(false) // helps with braceles statements: if (...) LOG(...); else ...

#define LOG(log, key, values) \
    do { \
        if (io()->debug()->logging()) { \
            LOG_ALWAYS(log, key, values); \
        } \
    } while(false) // helps with braceles statements: if (...) LOG(...); else ...

#define LOG_FAIL(log, key, values) \
    do { \
        assert(#key ": " #values); \
        LOG_ALWAYS(log, key, values); \
    } while(false) // helps with braceles statements: if (...) LOG(...); else ...

#endif
