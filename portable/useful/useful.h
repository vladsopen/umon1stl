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
#ifndef USEFUL_H
#define USEFUL_H

namespace useful
{
    // uses temporary 64 bit
    int mulDiv(int mul1, int mul2, int div);

    // safe/0
    //template<class T>
    //T div0(T numerator, T divisor, T fallback)
    //    { return divisor == 0? fallback : numerator / divisor; }
    int div0(int numerator, int divisor, int fallback);
    double div0(double numerator, double divisor, double fallback);
    double div0(double numerator, int divisor, double fallback);
    double div0(int numerator, double divisor, double fallback);
    double div0(double numerator, int divisor, int fallback);
    double div0(int numerator, double divisor, int fallback);
    double div0(double numerator, double divisor, int fallback);

    // makes it more readable: div0(height, /, width, or 0)
    #define div0(numerator, op, divisor, fallback) \
        useful::div0((numerator), (divisor), fallback_##fallback)
    #define fallback_or

    // force rounding to int
    int iround(double x);

    // replaces multiple substrings at once
    std::string replaceAll(
            const std::string& subject,
            const std::string& from,
            const std::string& to);

    // no std::clamp in C++14
    template<class T>
    T clamp(T value, T min, T max)
        { return std::max(std::min(value, max), min); }

    // a fix to the fucked up STL unsigned situation
    template<class STLCONTAINER>
    int size(const STLCONTAINER& c)
    {
        typename STLCONTAINER::size_type unsignedSize = c.size();
        return static_cast<int>(unsignedSize);
    }
    template<class STLCONTAINER>
    int length(const STLCONTAINER& c)
    {
        typename STLCONTAINER::size_type unsignedLength = c.length();
        return static_cast<int>(unsignedLength);
    }
}

//
// strict parameter cheching: tags a basic type with a unique label,
//   and demands explicit conversion
//
// void Foo(
//         As<int, class RecursionDepth> recursionDepth,
//         As<std::string, class UserName> userName,
//         As<bool, class BeastMode> beastMode)
//
template<class Value, class FakeTypeForStrictChecking>
class As
{
public:
    As() : m_value(Value()) {}
    explicit As(const Value& v) : m_value(v) {}
    As(const As& other) : m_value(other.m_value) {}

    operator Value() const { return m_value; }
    Value value() const { return m_value; }

private:
    Value m_value;
};

// Represent invalid value for error handling
class NoStrictTypeCheck {};
class Nothing {};
template<class Value, class FakeTypeForStrictChecking = NoStrictTypeCheck>
class Maybe
{
public:
    //Maybe() : m_value(Value()) {}
    Maybe(Nothing) : m_value(Value()), m_valid(false) {}
    explicit Maybe(const Value& v) : m_value(v), m_valid(true) {}
    Maybe(const Maybe& other) : m_value(other.m_value), m_valid(true) {}

    bool valid() const
    {
        return m_valid;
    }

    Value valueOr(const Value& whenNothing) const
    {
        return m_valid? m_value : whenNothing;
    }

    std::string stringOr(const std::string& whenNothing) const
    {
        return m_valid? std::to_string(m_value) : whenNothing;
    }

    Value useOr(const Value& whenNothing) const
    {
        if (m_valid)
            return m_value;
        else
        {
            rFAIL("expected valid Maybe!");
            return whenNothing;
        }
    }

private:
    Value m_value;
    bool m_valid;
};

// cleanup block: auto&& clearFoo = cleaner([=] () mutable { foo = 0; });
template<typename F>
class Cleaner
{
public:
    Cleaner(F f) : m_f(std::move(f)) {}

    ~Cleaner() { m_f(); }

private:
    F m_f;

    Cleaner(Cleaner&&) = delete;
    Cleaner(Cleaner const&) = delete;
};

template<typename F>
Cleaner<F> cleaner(F f) { return { std::move(f) }; }

#endif
