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
#ifndef DEFINEUI_H
#define DEFINEUI_H

// UI declaration block scope

template<class UiX>
class Ui final // no vtable
{
private:
    void constructAsChild(
            ParentUi& parent)
    {
        m_isBlock = false;
        m_isCreating = true;

        m_thisUi =
            std::make_shared<UiX>(
                CreateUi(
                    CreateUi::notFrame,
                    parent.uiParent(),
                    parent.uiParent()->io()));

        m_thisUi->afterConstructor(m_thisUi);
    }

public:

    // declare normal non-UiPanel ui leaf control
    explicit Ui(
            ParentUi& parent)
    {
        constructAsChild(parent);
    }

    // for main window
    explicit Ui(CreateUi::FrameFlags, std::shared_ptr<Io>)
    {
        this->___use_only_with_UiPanel_which_has_the_specialized_MainWindow_constructor___;
    }

    // we need to capture Ui values in handlers
    Ui(const Ui& from)
    {
        constructAsReference(from.m_thisUi);
    }

    // support for implicit conversion to super class
    template<class BASE>
    Ui(const Ui<BASE>& from)
    {
        constructAsReference(from.shared_ptr());
    }

    // non-virtual - we have no vtable here
    ~Ui()
    {
        // call specialized version by type T
        destruct(m_thisUi.get());
    }

    // implicitly convert to normal smart pointer
    operator const std::shared_ptr<UiX>& () const
    {
        return m_thisUi;
    }

    const std::shared_ptr<UiX>& shared_ptr() const
    {
        return m_thisUi;
    }

    // use operator std::shared_ptr<UiX> () instead of assigning Ui<>
    void operator = (const Ui&) = delete;

    UiX* operator -> ()
    {
        return m_thisUi.get();
    }

    UiX& operator * ()
    {
        return *m_thisUi.get();
    }

private:
    std::shared_ptr<UiX> m_thisUi;
    bool m_isBlock = false;
    bool m_isCreating = false;

    void constructAsReference(std::shared_ptr<UiX> ui)
    {
        m_isBlock = false;
        m_isCreating = false;

        m_thisUi = ui;
    }

    void establishBlockScope()
    {
    }

    void destruct(void*)
    {
    }

    void destruct(UiBasic*)
    {
        // notify ui when it has all on-handlers defined
        if (m_thisUi && m_isCreating)
            m_thisUi->afterUiDefined();
    }

};

// declare UiPanel ui block
template<>
Ui<UiPanel>::Ui(
        ParentUi& parent);

// for main window
template<>
Ui<UiPanel>::Ui(CreateUi::FrameFlags, std::shared_ptr<Io> io);

#endif
