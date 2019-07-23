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
#include "useful.h"

int useful::mulDiv(int mul1, int mul2, int div)
{
    //TODO: check 32 bit version asm

    if (div == 0)
    {
        rFAIL("mulDiv divide by 0");
        return 0; // safe for all typical uses
    }

    long long result = mul1;
    result *= mul2;
    result /= div;
    return static_cast<int>(result);
}

int useful::iround(double x)
{
    if (x < std::numeric_limits<int>::min())
    {
        rFAIL("iround underflow");
        return std::numeric_limits<int>::min();
    }
    else
    if (x > std::numeric_limits<int>::max())
    {
        rFAIL("iround overflow");
        return std::numeric_limits<int>::max();
    }
    else
    {
        return static_cast<int>(std::lround(x));
    }
}

std::string useful::replaceAll(
        const std::string& subject,
        const std::string& from,
        const std::string& to)
{
    if (from.empty())
        return subject;

    std::string s = subject;
    size_t start = 0;
    while ((start = s.find(from, start)) != std::string::npos)
    {
        s.replace(start, from.length(), to);
        start += to.length();
    }

    return s;
}

LOCAL_TEST(TestStringReplaceAll)(PassIo io)
{
    auto test = [=] (
            const std::string& subject,
            const std::string& from,
            const std::string& to,
            const std::string& expect)
    {
        std::string result = useful::replaceAll(subject, from, to);
        rASSERT(result == expect);

        LOG(all, "StringReplace",
            "\"" << subject << "\": \"" << from << "\" -> \"" << to << "\" "
                "==> \"" << result << "\"");
    };

    test("", "", "", "");
    test("1", "", "", "1");
    test("", "1", "", "");
    test("", "", "1", "");
    test("", "1", "1", "");
    test("1", "1", "1", "1");
    test("1", "1", "11", "11");
    test("", "x", "1", "");
    test("1", "1", "", "");
    test("1", "", "1", "1");
    test("x", "1", "", "x");
    test("x", "", "1", "x");
    test("xxx", "x", "x1", "x1x1x1");
    test("xxx", "x", "1x", "1x1x1x");
    test("x1xx", "1", "x", "xxxx");
    test("x1x1x", "1", "x", "xxxxx");
    test("x1x1x", "1", "2", "x2x2x");
    test("1x1", "1", "2", "2x2");
    test("11", "1", "2", "22");
    test("xx1xx", "1", "2", "xx2xx");
    test("1xx", "1", "2", "2xx");
    test("xx1", "1", "2", "xx2");
}

#undef div0

int useful::div0(int numerator, int divisor, int fallback)
{
    return divisor == 0? fallback : numerator / divisor;
}

double useful::div0(double numerator, double divisor, double fallback)
{
    static constexpr double plusZero = std::numeric_limits<double>::min() * 10;
    static constexpr double minusZero = -plusZero;

    return divisor < plusZero && divisor > minusZero? fallback : numerator / divisor;
}

// all other overloads convert to double implicitly
double useful::div0(double numerator, int divisor, double fallback)
{ return div0(double(numerator), double(divisor), double(fallback)); }
double useful::div0(int numerator, double divisor, double fallback)
{ return div0(double(numerator), double(divisor), double(fallback)); }
double useful::div0(double numerator, int divisor, int fallback)
{ return div0(double(numerator), double(divisor), double(fallback)); }
double useful::div0(int numerator, double divisor, int fallback)
{ return div0(double(numerator), double(divisor), double(fallback)); }
double useful::div0(double numerator, double divisor, int fallback)
{ return div0(double(numerator), double(divisor), double(fallback)); }

LOCAL_TEST(TestDiv0)(PassIo io)
{
    auto test = [=] (auto numerator, auto divisor, auto fallback)
    {
        LOG(all, "Div0",
            "" << numerator << " / " << divisor << " (" << fallback << ") = " <<
                useful::div0(numerator, divisor, fallback));
    };

    test(10, 5, 0);
    test(5, 2, 0);
    test(-13, 8, 0);
    test(1000, -1, 0);
    test(-20, -13, -111);
    test(1, 0, 100);
    test(-1000, 0, -222);
    test(0, 0, 42);

    test(10., 5, 0);
    test(5, 2., 0);
    test(-13., 8., 0);
    test(-2.5, -8.55, 777);
    //test(1, 0, 100.); error!
    test(-1000., 0., -222.);
    test(0., 0, 42.);

    test(3.14, 0.0000000001, 1);
    test(3.14, 1e-300, 2);
    test(-3.14, -1e-301, 3);
    test(314, -1e-302, 4);

    test(111, std::numeric_limits<double>::min(), 999);
    test(-1., std::numeric_limits<double>::min() * 2, 999999);
    test(222, -std::numeric_limits<double>::min(), 111);
    test(0.5, -std::numeric_limits<double>::min() * 3, 333);
}
