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

// declare UiPanel ui block
template<>
Ui<UiPanel>::Ui(
        ParentUi& parent)
{
    constructAsChild(parent);

    m_isBlock = true;

    establishBlockScope();
}

// for main window
template<>
Ui<UiPanel>::Ui(CreateUi::FrameFlags frameFlags, std::shared_ptr<Io> io)
{
    m_isBlock = true;
    m_isCreating = true;

    // construct the main window
    m_thisUi =
        std::make_shared<UiPanel>(
            CreateUi(
                frameFlags,
                static_cast<UiPanel*>(nullptr), // no parent for MainWindow
                io));

    m_thisUi->afterConstructor(m_thisUi);

    establishBlockScope();
}

