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
#ifndef DRAWBACK_H
#define DRAWBACK_H

// Draw the common background graphics

class DrawBack : public Context
{
public:
    explicit DrawBack(DrawAll* drawAll);

    void paintBack(
            Paint& paint);

    void paintGrid(
            Paint& paint);

private:

    const GetParent<const DrawAll*> drawAll;

    struct grid // (a const namespace)
    {
        static constexpr int pxPerMmX = 4; // 40 pix/cm
        static constexpr int pxPerMmY = 4; // 40 pix/cm
        static constexpr int pxPerCmX = pxPerMmX * 10;
        static constexpr int pxPerCmY = pxPerMmY * 10;
    };

};

#endif
