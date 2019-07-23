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
#include "uirgb.h"

Rgb::Rgb()
{
}

Rgb::Rgb(const Rgb& rgb)
    : Object()
{
    *this = rgb;
}

Rgb::Rgb(int r, int g, int b)
{
    m_r = assertAndSaturate(r);
    m_g = assertAndSaturate(g);
    m_b = assertAndSaturate(b);
}

void Rgb::operator = (const Rgb& rgb)
{
    m_r = rgb.m_r;
    m_g = rgb.m_g;
    m_b = rgb.m_b;
}

int Rgb::red() const
{
    return m_r;
}

int Rgb::green() const
{
    return m_g;
}

int Rgb::blue() const
{
    return m_b;
}

Rgb& Rgb::red(int r)
{
    m_r = assertAndSaturate(r);
    return *this;
}

Rgb& Rgb::green(int g)
{
    m_g = assertAndSaturate(g);
    return *this;
}

Rgb& Rgb::blue(int b)
{
    m_b = assertAndSaturate(b);
    return *this;
}

Rgb Rgb::apply(const std::function<int (int)>& f) const
{
    return Rgb(f(red()), f(green()), f(blue()));
}

Rgb::Hex Rgb::asHex() const
{
    return Hex(m_r, m_g, m_b);
}

const char* Rgb::Hex::asZeroTerminated() const
{
    return m_string;
}

Rgb::Hex::Hex(int r, int g, int b)
{
    // compose HTML style RGB "#01A3FF" as fast as possible
    //   not using memory alloc or slow general purpose functions

    static const char* digits = "0123456789abcdef";

    m_string[0] = static_cast<char>(digits[r >> 4]);
    m_string[1] = static_cast<char>(digits[r & 0xf]);
    m_string[2] = static_cast<char>(digits[g >> 4]);
    m_string[3] = static_cast<char>(digits[g & 0xf]);
    m_string[4] = static_cast<char>(digits[b >> 4]);
    m_string[5] = static_cast<char>(digits[b & 0xf]);
    m_string[6] = 0;
    static_assert(sizeof(m_string) == 7, "unsafe index");
}

LOCAL_TEST(TestRgbHex)(PassIo io)
{
    std::vector<Rgb> tests =
        { Rgb(1, 222, 16), Rgb(15, 128, 254), Rgb(0, 0, 0), Rgb(255, 255, 255), };
    for (auto rgb : tests)
    {
        LOG(all, "RgbHex",
            "RGB(" << rgb.asDecimal(", ") <<
                ") == " << rgb.asHex().asZeroTerminated());
    }
}

char Rgb::assertAndSaturate(
        int rgb)
{
    if (rgb < 0)
    {
        rFAIL("negative Rgb component!");
        rgb = 0;
    }

    if (rgb > 255)
    {
        rFAIL("overflow in Rgb component!");
        rgb = 255;
    }

    return static_cast<char>(rgb);
}

std::string Rgb::asDecimal(const std::string& separator) const
{
    return
        std::to_string(red()) + separator +
        std::to_string(green()) + separator +
        std::to_string(blue());
}

