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
#ifndef DESIGN_H
#define DESIGN_H

#include "application.h"

// Common UI constants

class Design : public Context
{
    explicit Design(App* app); // unused yet
public:

    static constexpr int xSpaceBetweenButtons = 2;
    static constexpr int ySpaceDefault = 2;
    static constexpr int xIconButton = 60;

    static constexpr int xBestViewWidth = 300;
    static constexpr int yBestViewHeight = 300;

    struct colors
    {
        // (C++14 missing inline const)

        static const Rgb background() { return Rgb(255, 255, 255); }

        static const Rgb weakGrid() { return Rgb(236, 227, 217); }
        static const Rgb strongGrid() { return Rgb(225, 209, 191); }

        static const Rgb normalCurrentBox() { return Rgb(255, 255, 200); }
        static const Rgb unstableCurrentBox() { return Rgb(255, 235, 175); }

        static const Rgb cross() { return Rgb(192, 192, 192); }

        static const Rgb channelPolyline(int channel);

    };

};

#endif
