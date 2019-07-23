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
#ifndef DEFINES_H
#define DEFINES_H

// debug-only version
#define ASSERT(...) assert(__VA_ARGS__)

// remains and logs errors in the release build too
#define rASSERT(...) assert(__VA_ARGS__)
    //TODO: log in release

// unconditional faulure
#define rFAIL(...) assert(false && __VA_ARGS__)
    //TODO: log in release

// suppress 'unused' warning
#if defined(__GNUC__)
    #define UNUSED(x) ((void) x); struct x // incompatible with -Wshadow
    #define NOTUSED __attribute__ ((unused))
#elif defined(_MSC_VER)
    #define UNUSED(x) ((void) x); struct x
    #define NOTUSED __pragma(warning(suppress:4100)) __pragma(warning(suppress:4189))
#else
    #define NOTUSED
#endif

// shorter loop (see FromTo)
//#define repeat(i, n) for (int n_##i = (n), i = 0; i < n_##i; ++i)
//#define repeatback(i, n) for (int n_##i = (n) - 1; n_##i >= 0; --n_##i)

// prefixes output (nonconst&) function parameters
class Out {};

#endif
