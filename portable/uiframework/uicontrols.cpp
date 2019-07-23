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

UiBasic::UiBasic(
        const CreateUi& createUi) :
    Context(createUi.io()),
    createUi(createUi),
    m_globalUi(
        createUi.parentOrNull()?
            createUi.parentOrNull()->relatedUi()
            :
            std::make_shared<RelatedUi>())
{
}

void UiBasic::afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    if (UiPanel* parent = createUi().parentOrNull())
    {
        parent->attachChild(thisUi);
    }

    // low level init
    driver();

    // the rest of framework-specific init
    impl_afterConstructor(thisUi);
}

void UiBasic::afterUiDefined()
{
    // this is called when Ui<> block ends (Ui destructor)
    //   and here we have all initial client attributes and handlers defined

    // initial updates can be done now
    update();

    // the rest of framework-specific init
    impl_afterUiDefined();
}

void UiBasic::destroy()
{
    rASSERT(!m_destroyed);

    m_destroyed = true;

    timerOff();

    //impl_destroy();
}

bool UiBasic::destroyed() const
{
    return m_destroyed;
}

DriveBasic* UiBasic::driver()
{
    return m_driverOrNull? m_driverOrNull : (m_driverOrNull = newDriver());
}

DrivePanel* UiBasic::driverParentOrNull()
{
    return parentOrNull()? reinterpret_cast<DrivePanel*>(parentOrNull()->driver()) : nullptr;
}

std::shared_ptr<RelatedUi> UiBasic::relatedUi()
{
    return m_globalUi;
}

void UiBasic::updateAllExistingWindows()
{
    if (auto main = relatedUi()->m_main.lock())
    {
        main->updateAllChildren();
    }
    else
    {
        rFAIL("missing main window!");
    }
}

void UiBasic::width(int px)
{
    impl_width(px);
}

void UiBasic::height(int px)
{
    impl_height(px);
}

void UiBasic::niceX()
{
    impl_niceX();
}

void UiBasic::niceY()
{
    impl_niceY();
}

void UiBasic::stretchX()
{
    impl_stretchX();
}

void UiBasic::stretchY()
{
    impl_stretchY();
}

void UiBasic::inflateX()
{
    impl_inflateX();
}

void UiBasic::inflateY()
{
    impl_inflateY();
}

void UiBasic::wrap(bool yes)
{
    impl_wrap(yes);
}

void UiBasic::disable(const std::string& why)
{
    m_whyDisabled = why;

    impl_disable(why);
}

bool UiBasic::isDisabled() const
{
    return m_whyDisabled.length() != 0;
}

void UiBasic::timer(int milliseconds)
{
    timerOff();
    impl_timer(milliseconds);
}

void UiBasic::timerOff()
{
    impl_timerOff();
}

void UiBasic::repaint()
{
    impl_repaint();
}

void UiBasic::updateAllChildren()
{
    update();
}

void UiBasic::update()
{
    if (m_destroyed)
        return;

    if (onUpdate.valid())
        onUpdate.get()();
}

void UiBasic::invokeTimer()
{
    if (onTimer.valid()) // && !io()->mock()->disableTimerHandler())
                    // we need some timers...
        onTimer.get()();
}

void UiBasic::invokeClose()
{
    if (onClose.valid())
        onClose.get()();
}

void UiBasic::invokePointer(std::shared_ptr<PointerEvent> event)
{
    if (onPointer.valid())
        onPointer.get()(event);
}

void UiBasic::invokePaint(Paint& paint)
{
    if (onPaint.valid())
        onPaint.get()(paint);
}

UiPanel::UiPanel(
        const CreateUi& createUi) :
    // (this looks complicated but allows us to keep most members const
    //   and helps to init everything in the constructor)
    UiBasic(
        createUi)
{
}

void UiPanel::afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    // remember the top window
    if (createUi().frameFlags() & CreateUi::mainWindow)
    {
        rASSERT(!relatedUi()->m_main.lock());

        relatedUi()->m_main = std::static_pointer_cast<UiPanel>(thisUi);
    }

    UiBasic::afterConstructor(thisUi);

    // defaults:
    niceX();
    niceY();
    wrap(false);
    spacingX(0);
    spacingY(0);

    impl_afterConstructor(thisUi);
}

void UiPanel::afterUiDefined()
{
    createChildren();

    // must be called AFTER createChildren() to update them 1st time
    UiBasic::afterUiDefined();

    impl_afterUiDefined();
}

void UiPanel::destroy()
{
    rASSERT(!m_destroyed);

    UiBasic::destroy();

    rASSERT(m_destroyed);

    for (auto child : m_children)
    {
        child->destroy();
    }

    m_children.clear();

    impl_destroy();
}

ParentUi::ParentUi(
        UiPanel* uiParent) :
    uiParent(uiParent)
{
    rASSERT(uiParent);
}

void UiPanel::operator << (ChildMaker add)
{
    m_eggs.push_back(add);
}

void UiPanel::createChildren()
{
    for (const auto& maker : m_eggs)
    {
        ParentUi parentUi(this);

        maker(parentUi);
    }
}

void UiPanel::attachChild(
        std::shared_ptr<UiBasic> child)
{
    m_children.push_back(child);
}

void UiPanel::updateAllChildren()
{
    UiBasic::updateAllChildren();

    for (auto child : m_children)
    {
        child->updateAllChildren();
    }
}

void UiPanel::br()
{
    *this << [this] (ParentUi&)
    {
        impl_br();
    };
}

void UiPanel::spacingX(int px)
{
    m_spacing.w(px);

    impl_spacingX(px);
}

void UiPanel::spacingY(int px)
{
    m_spacing.h(px);

    impl_spacingY(px);
}

UiTextBased::UiTextBased(
        const CreateUi& createUi) :
    UiBasic(createUi)
{
}

void UiTextBased::afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UiBasic::afterConstructor(thisUi);

    // defaults:
    niceX();
    stretchY();

    impl_afterConstructor(thisUi);
}

void UiTextBased::afterUiDefined()
{
    UiBasic::afterUiDefined();

    impl_afterUiDefined();
}

void UiTextBased::text(const std::string& value)
{
    m_text = value;

    impl_afterText(value);
}

std::string UiTextBased::text() const
{
    return m_text;
}

void UiTextBased::invokeClick()
{
    if (onClick.valid())
        onClick.get()();

    updateAllExistingWindows();
}

UiButton::UiButton(
        const CreateUi& createUi) :
    UiTextBased(createUi)
{
}

void UiButton::afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UiTextBased::afterConstructor(thisUi);

    // defaults:
    niceX();
    stretchY();

    impl_afterConstructor(thisUi);
}

void UiButton::afterUiDefined()
{
    UiTextBased::afterUiDefined();

    impl_afterUiDefined();
}

void UiButton::invokeClick()
{
    UiTextBased::invokeClick();
}

UiStatic::UiStatic(
        const CreateUi& createUi) :
    UiTextBased(createUi)
{
}

void UiStatic::afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UiTextBased::afterConstructor(thisUi);

    // defaults:
    niceX();
    stretchY();

    impl_afterConstructor(thisUi);
}

void UiStatic::afterUiDefined()
{
    UiTextBased::afterUiDefined();

    impl_afterUiDefined();
}

void UiStatic::invokeClick()
{
    UiTextBased::invokeClick();
}
