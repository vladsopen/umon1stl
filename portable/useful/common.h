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
#ifndef COMMON_H
#define COMMON_H

// Common non application includes and definitions included everywhere

#if defined(_MSC_VER)
    #pragma warning(push, 0)
#endif

#include <cstdint>
#include <functional>
#include <tuple>
#include <vector>
#include <list>
#include <memory>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <numeric>

#if defined(_MSC_VER)
    #pragma warning(pop)
    #pragma warning(push, 4)
#endif

#if defined(QT_CORE_LIB) //|| defined(WIN32)
    #define WITH_QT
#endif

#if defined(WIN32) || defined(_MSC_VER)
    #define WITH_UV
#endif

#if !defined(_MSC_VER)
    #pragma GCC diagnostic ignored "-Wsign-conversion" // for -Wconversion with myvector[int]
#endif

#if defined(__EMSCRIPTEN__)

    #pragma GCC diagnostic ignored "-Wdollar-in-identifier-extension" // for EM_ASM_($1...)
    #pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments" // for EM_ASM_(zero args)

    #include "emcommon.h"

    // we append integers to element ids in HTML
    typedef int uiid;
    const int nulluiid = 0;

    #define DriveBasic EmBasic
    #define DrivePanel EmPanel
    #define DriveButton EmButton
    #define DriveStatic EmStatic
    class EmBasic;
    class EmPanel;
    class EmButton;
    class EmStatic;

#elif defined(WITH_QT)

    class QWidget;
    class QPainter;
    class QSize;
    class QColor;
    class UiBasic;

    typedef UiBasic* uiid;
    UiBasic* const nulluiid = nullptr;

    #define DriveBasic QWidget
    #define DrivePanel QtPanel
    #define DriveButton QtButton
    #define DriveStatic QtLabel
    class QWidget;
    class QtPanel;
    class QtButton;
    class QtLabel;

#elif defined(WITH_UV)

    class UiBasic;
    class DriveBasic;
    class DrivePanel;
    class DriveButton;
    class DriveStatic;

    typedef UiBasic* uiid;
    UiBasic* const nulluiid = nullptr;

#else
    #error unknown compilation mode
#endif

class Io;
class Log;
class Logs;
class RelatedUi;
class Paint;
class UiBasic;

#include "defines.h"
#include "useful.h"
#include "fromto.h"
#include "weakmap.h"
#include "property.h"
#include "object.h"
#include "uipaint.h"
#include "uirgb.h"
#include "uixy.h"
#include "uicontext.h"
#include "uicontrols.h"
#include "defineui.h"
#include "io.h"
#include "logs.h"
#include "localtest.h"

#endif
