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
#ifndef UIRGB_H
#define UIRGB_H

#include "object.h"

// Portable RGB object

class Rgb : public Object
{
public:
    Rgb();
    Rgb(const Rgb& rgb);
    Rgb(int r, int g, int b);

    void operator = (const Rgb& rgb); // 'void' by design

    int red() const;
    int green() const;
    int blue() const;

    Rgb& red(int r);
    Rgb& green(int g);
    Rgb& blue(int b);

    Rgb apply(const std::function<int (int)>& f) const;

    // JS compatibility "color: #11aaff;"
    //   optimized to avoid memory allocation
    class Hex
    {
    public:
        Hex(char r, char g, char b);
        Hex(const Hex&) = default;
        Hex& operator = (const Hex&) = default;

        const char* asZeroTerminated() const;

    private:
        char m_string[3 * 2 + 1]; // "11aaff\0"

        Hex(int r, int g, int b);
        friend class Rgb;
    };
    Hex asHex() const;

#if defined(__EMSCRIPTEN__)
#elif defined(WITH_QT)

    operator QColor() const;

#elif defined(WITH_UV)
#else
    #error unknown compilation mode
#endif

    std::string asDecimal(const std::string& separator) const;

private:
    unsigned char m_r = 0;
    unsigned char m_g = 0;
    unsigned char m_b = 0;

    static char assertAndSaturate(
            int rgb);
};

#endif
