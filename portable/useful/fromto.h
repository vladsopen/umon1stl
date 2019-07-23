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
#ifndef FROMTO_H
#define FROMTO_H

/*
  This is optimized to be identical to the classic for loop:
    (still I prefer the repeat(i, n) macro)

    1.                                      2.
        void f1(int n, void (*g)(int i))        void f2(int n, void (*g)(int i))
        {                                       {
            for (int i : FromTo(2, n))              for (int i = 2; i < n; ++i)
            {                                       {
                g(i);                                   g(i);
            }                                       }
        }                                       }

    THE SAME (at -O3):
                        fX(int, void (*)(int)):
                                cmp     edi, 2
                                jle     .L6
                                push    r12
                                mov     r12, rsi
                                push    rbp
                                mov     ebp, edi
                                push    rbx
                                mov     ebx, 2
                        .L3:
                                mov     edi, ebx
                                add     ebx, 1
                                call    r12
                                cmp     ebp, ebx
                                jne     .L3
                                pop     rbx
                                pop     rbp
                                pop     r12
                                ret
                        .L6:
                                ret
*/

class FromTo
{
public:
    explicit constexpr FromTo(int start, int n) : m_start(start), m_i(n) {}
    FromTo(const FromTo&) = default;
    void operator = (const FromTo&) = delete;

    constexpr FromTo begin() const
        { return FromTo(0, m_start); }
    constexpr FromTo end() const
        { return *this; }

    constexpr bool operator != (const FromTo& other) const
        { return m_i < other.m_i; } // we dont want to overshoot
    constexpr int operator * () const
        { return m_i; }
    void operator ++ ()
        { ++m_i; }

private:
    const int m_start;
    int m_i;
};

#endif
