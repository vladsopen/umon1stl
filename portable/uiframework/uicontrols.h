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
#ifndef UICONTROLS_H
#define UICONTROLS_H

// The base class for all UI-controls

class UiBasic : public Context
{
public:
    explicit UiBasic(const CreateUi& createUi);
    virtual ~UiBasic();

    // Called internally after shared_ptr is constructed
    virtual void afterConstructor(const std::shared_ptr<UiBasic>& thisUi);

    // Complete initialization when Ui<> block ends
    virtual void afterUiDefined();

    // for closing the main window
    virtual void destroy();
    bool destroyed() const;

    // Constant type
    const GetPredefined<CreateUi> createUi;
    UiPanel* parentOrNull()
        { return createUi().parentOrNull(); }

    // Misc debug info
    GetSet<std::string> debugStatus;

    // Custom size
    void width(int px);
    void height(int px);

    // Auto-size control
    void niceX();
    void niceY();
    void stretchX();
    void stretchY();
    void inflateX();
    void inflateY();
    void wrap(bool yes);

    // Disable, always providing an explanation for the user!
    //   (empty string to enable back)
    void disable(const std::string& why);
    bool isDisabled() const;

    // Timer
    void timer(int milliseconds);
    void timerOff();

    // Just call paint with cached attrubutes
    void repaint();

    // Call onUpdate() to update control attributes
    void update();

    // Useful for derived multichildren controls (UiPanel)
    virtual void updateAllChildren();
    void updateAllExistingWindows();

    template<typename T>
    class AssignOnly : public ::AssignOnly<T>
    {
    public:
        using ::AssignOnly<T>::AssignOnly;
        friend class UiBasic;
        void operator = (const T& assign) { ::AssignOnly<T>::operator = (assign); }
    };

    // Event handlers
    AssignOnly<std::function<void ()>> onUpdate;
    AssignOnly<std::function<void ()>> onTimer;
    AssignOnly<std::function<void ()>> onClose;
    AssignOnly<std::function<void (Paint&)>> onPaint;
    AssignOnly<std::function<void (std::shared_ptr<PointerEvent>)>> onPointer;

    //void invokeUpdate(); see update()
    void invokeTimer();
    void invokeClose();
    void invokePaint(Paint& paint);
    void invokePointer(std::shared_ptr<PointerEvent> event);

    // platform-dependent parts
    DriveBasic* driver();
    DrivePanel* driverParentOrNull();
protected:
    DriveBasic* m_driverOrNull = nullptr;
    virtual DriveBasic* newDriver() = 0;

    void impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi);
    void impl_afterUiDefined();
    void impl_width(int px);
    void impl_height(int px);
    void impl_niceX();
    void impl_niceY();
    void impl_stretchX();
    void impl_stretchY();
    void impl_inflateX();
    void impl_inflateY();
    void impl_wrap(bool yes);
    void impl_disable(const std::string& why);
    void impl_timer(int milliseconds);
    void impl_timerOff();
    void impl_repaint();

protected:
    std::shared_ptr<RelatedUi> relatedUi();

    bool m_destroyed = false;

private:
    const std::shared_ptr<RelatedUi> m_globalUi;

    std::string m_whyDisabled;
    Maybe<int> m_timerId = Nothing();
};

// Layout container child control or a top level frame

class UiPanel : public UiBasic
{
public:
    explicit UiPanel(const CreateUi& createUi);

    // Layout control - "<BR />"-like
    void br();

    // Content intervals (//TODO: work in progress, see //HACK:_spacing_)
    void spacingX(int px);
    void spacingY(int px);

    // Very inefficient total update via onUpdate
    virtual void updateAllChildren() override;

    // add child windows
    using ChildMaker = std::function<void (ParentUi&)>;
    void operator << (ChildMaker);

protected:

    std::list< ChildMaker > m_eggs;
    std::list< std::shared_ptr<UiBasic> > m_children;
        // we may need to replace the lists
        //   with a single vector for better performance
        //   but only if the pattern stays the same:
        //   egg count matches children count
        //   (depends on whether we are going to support incremental
        //   child append or only total content rebuild)

    void attachChild(
            std::shared_ptr<UiBasic> child);
    friend void UiBasic::afterConstructor(const std::shared_ptr<UiBasic>& thisUi);

    // platform-dependent parts
public:
    DrivePanel* driver()
        { return reinterpret_cast<DrivePanel*>(UiBasic::driver()); }
    virtual void afterUiDefined() override;
    virtual void afterConstructor(const std::shared_ptr<UiBasic>& thisUi) override;
    virtual void destroy() override;
protected:
    virtual DriveBasic* newDriver() override;

    void impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi);
    void impl_afterUiDefined();
    void impl_destroy();
    void impl_br();
    void impl_spacingX(int px);
    void impl_spacingY(int px);

private:
    Size m_spacing;

    void applySpacingToChildren(
            const std::string& side1,
            const std::string& side2,
            int px);

    void createChildren();
};

// A control with a text as the central property (Button, Static, CheckBox, etc)
//   (only used as the base class for the specific controls)

class UiTextBased : public UiBasic
{
public:
    UiTextBased(const CreateUi& createUi);

    // Main text (like innerHTML)
    void text(const std::string& value);
    std::string text() const;

    template<typename T>
    class AssignOnly : public ::AssignOnly<T>
    {
    public:
        using ::AssignOnly<T>::AssignOnly;
        friend class UiTextBased;
        void operator = (const T& assign) { ::AssignOnly<T>::operator = (assign); }
    };

    // Event handlers
    AssignOnly<std::function<void ()>> onClick;

    virtual void invokeClick();

private:
    std::string m_text;

    // platform-dependent parts
public:
    // this control type does not need a driver
    //driver()
    //    { return reinterpret_cast<*>(UiBasic::driver()); }
    virtual void afterConstructor(const std::shared_ptr<UiBasic>& thisUi) override;
    virtual void afterUiDefined() override;
protected:
    //virtual DriveBasic* newDriver() override;

    void impl_afterUiDefined();
    void impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi);
    virtual void impl_afterText(const std::string& value);
};

// A clickable control

class UiButton : public UiTextBased
{
public:
    explicit UiButton(const CreateUi& createUi);

    // Event handlers
    virtual void invokeClick() override;

private:

    // platform-dependent parts
public:
    DriveButton* driver()
        { return reinterpret_cast<DriveButton*>(UiBasic::driver()); }
    virtual void afterConstructor(const std::shared_ptr<UiBasic>& thisUi) override;
    virtual void afterUiDefined() override;
protected:
    virtual DriveBasic* newDriver() override;

    void impl_afterUiDefined();
    void impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi);
    virtual void impl_afterText(const std::string& value) override;
};

// Just a text label or indicator or spacer
//   (it secretly handles clicks but no proper UX for that)

class UiStatic : public UiTextBased
{
public:
    explicit UiStatic(const CreateUi& createUi);

    // Event handlers
    virtual void invokeClick() override;

private:

    // platform-dependent parts
public:
    DriveStatic* driver()
        { return reinterpret_cast<DriveStatic*>(UiBasic::driver()); }
    virtual void afterConstructor(const std::shared_ptr<UiBasic>& thisUi) override;
    virtual void afterUiDefined() override;
protected:
    virtual DriveBasic* newDriver() override;

    void impl_afterUiDefined();
    void impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi);
    virtual void impl_afterText(const std::string& value) override;
};

#endif
