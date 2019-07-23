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

LOCAL_TEST(TestFromTo)(PassIo io)
{
    auto test = [=] (auto from, auto to)
    {
        std::string log;
        for (int i : FromTo(from, to))
            log += std::to_string(i) + " ";

        LOG(all, "FromTo",
            std::to_string(from) << "... <" << std::to_string(to) << ": " << log);
    };

    test(3, 5);
    test(0, 4);
    test(-3, 1);
    test(-5, -3);
    test(0, 0);
    test(2, 2);
    test(-100, -100);
    test(5, 2);
    test(100, 0);
    test(100, 99);
    test(100, 100);
    test(100, 101);
    test(-101, -100);
    test(10, -2);
    test(-10, -20);
    test(-10, 0);
}
