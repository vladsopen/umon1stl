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

Logs::Logs(
        std::shared_ptr<Io> io) :
    Context(io),
    all(std::make_shared<Log>(io, "all")),
    silence(std::make_shared<Log>(io, "silence")),
    critical(std::make_shared<Log>(io, "critical")),
    scalerDetails(std::make_shared<Log>(io, "scalerDetails")),
    incrementerDetails(std::make_shared<Log>(io, "incrementerDetails")),
    drawEssential(std::make_shared<Log>(io, "drawEssential")),
    drawDetails(std::make_shared<Log>(io, "drawDetails")),
    m_logs
    {
        all(),
        silence(),
        critical(),
        scalerDetails(),
        incrementerDetails(),
        drawEssential(),
        drawDetails(),
    }
{
    // default redirects
    critical()->redirectTo(all());
    scalerDetails()->redirectTo(all());
    incrementerDetails()->redirectTo(all());
    drawEssential()->redirectTo(all());
    drawDetails()->redirectTo(silence());
}

void Logs::finish()
{
    for (auto log : m_logs)
    {
        log->save();
    }
}
