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
#ifndef LOCALTEST_H
#define LOCALTEST_H

#include "application.h"

// Smaller unit tests globally registered

#define LOCAL_TEST(Name) \
    class Name : public LocalTest \
    { \
        virtual void runTest(PassIo io) override; \
        virtual std::string name() const override \
            { return #Name; } \
    }; \
    Name s_##Name; \
    void Name::runTest

class LocalTest : public Object
{
public:
    explicit LocalTest();

    using PassIo = std::function<std::shared_ptr<Io> ()>;

    virtual void runTest(
            PassIo io)
            = 0;

    static std::string runAll();

    virtual std::string name()
            const
            = 0;

private:
    static std::vector<LocalTest*>& registry();
        // tests are global objects, we dont need smart pointers
};

#endif
