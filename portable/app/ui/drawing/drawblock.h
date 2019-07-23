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
#ifndef DRAWBLOCK_H
#define DRAWBLOCK_H

class DrawWave;

// Highligh the current received data block

class DrawBlock : public Context
{
public:
    DrawBlock(DrawAll* drawAll);

    void paintLastBlockBefore(
            Paint& paint,
            int pointsRecorded,
            int pointsLastBlock);

    void paintLastBlockAfter(
            Paint& paint,
            DrawWave* drawWave,
            int pointsRecorded,
            int pointsLastBlock);

private:

    const GetParent<const DrawAll*> drawAll;
    const View* view() const;

    void paintLastBlock(
            Paint& paint,
            int x,
            int y,
            int height,
            int pointsRecorded,
            int pointsLastBlock,
            As<bool, class UnstableState> unstable);
};

#endif
