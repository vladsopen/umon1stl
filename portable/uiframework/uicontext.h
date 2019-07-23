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
#ifndef UICONTEXT_H
#define UICONTEXT_H

// Creation context (a struct really)

class CreateUi : public Context
{
public:

    // for main window init with Ui<> ui(?)
    enum FrameFlags { mainWindow = 1, notFrame = 0 };

    explicit CreateUi(
            const FrameFlags& frameFlags,
            UiPanel* parentOrNull,
            std::shared_ptr<Io> io);
    CreateUi(
            const CreateUi& from);

    // initial constant options
    const GetPredefined<FrameFlags> frameFlags;
    const GetPredefined<UiPanel*> parentOrNull;
};

// Internal initialization context

class InitUi : public CreateUi
{
public:
    explicit InitUi(
            const CreateUi& createUi,
            UiBasic* api);

    // the window or control being created
    const GetPredefined<UiBasic*> api;
};

// Passed to child window creation routine

class ParentUi : public Object
{
public:
    explicit ParentUi(
            UiPanel* uiParent);
    ParentUi(
            std::nullptr_t) = delete;

    const GetPredefined<UiPanel*> uiParent;
};

// Event handler data

class PointerEvent : public Object
{
public:
    explicit PointerEvent() {}

    enum What { nothing, pointerDown, pointerUp, pointerOut, pointerMove, };
    GetSet<What> what = nothing;

    enum Device { unknown, mouse, touch, pen };
    GetSet<Device> device = unknown;

    GetSet<int> pointerId = 0;

    GetSet<int> buttonIndex = 0;

    GetSet<int> clientX = 0;
    GetSet<int> clientY = 0;

    GetSet<bool> withShift = false;
    GetSet<bool> withAlt = false;
    GetSet<bool> withCtrl = false;
};

// Common internal state for the whole window hierarchy

class RelatedUi : public Object
{
private:
    std::weak_ptr<UiPanel> m_main;

    friend class UiBasic;
    friend class UiPanel;
};

#endif
