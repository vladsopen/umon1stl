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
#include <QPainter>
#include <QWidget>
#include <QTextStream>
#include <QCoreApplication>
#include <QMessageBox>
#include "qtui.h"

static void initQtDerived(
        UiBasic* ui,
        QWidget* widget)
{
    QtPanel* qParent = ui->driverParentOrNull();

    // register it in the layout
    if (qParent)
    {
        qParent->addToRowLayout(widget);
    }

    // init the rest depending on UI type
    if (ui->createUi().frameFlags() & CreateUi::mainWindow)
    {
        widget->show();

        // a hack for tests: move each new main window on
        //   the screen so they dont overlap perfectly
        static int shift = 0;
        widget->move(QPoint(widget->pos().x() + shift, widget->pos().y() + shift));

        // slightly random offset for each new window
        shift = 30 + (shift + static_cast<int>(time(nullptr))) % 100;

        // make sure it is visible and stresstest our ability to handle
        //   unexpected message pumping before and during init stages
        if (ui->io()->interactivity() == Io::Interactivity::gui)
            Io::Os::yeldToEventLoopForTestingIfSupported();
    }
    else
    {
        rASSERT(ui->createUi().frameFlags() == CreateUi::notFrame);

        widget->show();
    }
}

UiBasic::~UiBasic()
{
    if (createUi().frameFlags() & CreateUi::mainWindow)
        if (m_driverOrNull)
            delete m_driverOrNull;

    m_driverOrNull = nullptr;
}

void UiBasic::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiBasic::impl_afterUiDefined()
{
    /*
    // force final layout to be ready and painted predictably
    if (auto layout = driver()->layout())
    {
        layout->invalidate();
        layout->update();
        layout->activate();
        driver()->repaint();
    }
    */
}

void UiBasic::impl_width(int px)
{
    driver()->setMinimumWidth(px);
}

void UiBasic::impl_height(int px)
{
    driver()->setMinimumHeight(px);
}

void UiBasic::impl_niceX()
{
    driver()->
        setSizePolicy(
            QSizePolicy::Fixed,
            driver()->sizePolicy().verticalPolicy());
}

void UiBasic::impl_niceY()
{
    driver()->
        setSizePolicy(
            driver()->sizePolicy().horizontalPolicy(),
            QSizePolicy::Fixed);
}

void UiBasic::impl_stretchX()
{
    driver()->
        setSizePolicy(
            QSizePolicy::Preferred,
            driver()->sizePolicy().verticalPolicy());
}

void UiBasic::impl_stretchY()
{
    driver()->
        setSizePolicy(
            driver()->sizePolicy().horizontalPolicy(),
            QSizePolicy::Preferred);
}

void UiBasic::impl_inflateX()
{
    driver()->
        setSizePolicy(
            QSizePolicy::MinimumExpanding,
            driver()->sizePolicy().verticalPolicy());
}

void UiBasic::impl_inflateY()
{
    driver()->
        setSizePolicy(
            driver()->sizePolicy().horizontalPolicy(),
            QSizePolicy::MinimumExpanding);
}

void UiBasic::impl_wrap(bool yes)
{
    UNUSED(yes);

    //TODO: need to switch to Flow QT layout
}

void UiBasic::impl_disable(const std::string& why)
{
    UNUSED(why);

    driver()->setEnabled(!isDisabled());
}

void UiBasic::impl_timer(int milliseconds)
{
    rASSERT(milliseconds > 0 && milliseconds < 1 * 1000 * 1000); // just for sanity

    m_timerId = Maybe<int>(driver()->startTimer(milliseconds));
}

void UiBasic::impl_timerOff()
{
    if (m_timerId.valid())
    {
        driver()->killTimer(m_timerId.useOr(-1));

        m_timerId = Nothing();
    }
}

void UiBasic::impl_repaint()
{
    driver()->repaint();
}

DriveBasic* UiPanel::newDriver()
{
    return new QtPanel(this);
}

void UiPanel::impl_destroy()
{
    driver()->close();
}

QtPanel::QtPanel(UiPanel* ui) :
    QWidget(ui->driverParentOrNull()),
    ui(ui),
    m_verticalLayout(new QVBoxLayout)
{
    initQtDerived(ui, this);

    setLayout(m_verticalLayout);

    setWindowTitle(ui->io()->sessionName().c_str());

    //setMinimumWidth(300);
    //setMinimumHeight(300);
}

void UiPanel::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiPanel::impl_afterUiDefined()
{
}

void UiPanel::impl_spacingX(int px)
{
    driver()->m_verticalLayout->setSpacing(px);
}

void UiPanel::impl_spacingY(int px)
{
    driver()->
        setContentsMargins(0, 0, 0, px);
    driver()->m_verticalLayout->
        setContentsMargins(0, 0, 0, px);
}

void UiPanel::impl_br()
{
    driver()->br();
}

void QtPanel::br()
{
    // do we have any rows in the layout?
    if (m_rowLayouts.size() == 0 ||
        columnsInLastRow() > 0)
    {
        auto horizontalLayout = new QHBoxLayout;

        //QPushButton* stretchMe = new QPushButton("stretch me", this);
        //stretchMe->setSizePolicy(QSizePolicy::MinimumExpanding, stretchMe->sizePolicy().verticalPolicy());
        //horizontalLayout->addWidget(stretchMe);

        horizontalLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        //horizontalLayout->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

        m_rowLayouts.push_back(horizontalLayout);
        m_verticalLayout->addLayout(m_rowLayouts.back());
        columnsInLastRow(0);
    }
}

void QtPanel::addToRowLayout(
        QWidget* add)
{
    // do we have any rows in the layout?
    if (m_rowLayouts.size() == 0)
    {
        // create the first row
        br();
    }

    // always append to the last row
    if (QHBoxLayout* layout = m_rowLayouts.back())
    {
        layout->addWidget(add);
        //layout->setStretchFactor(add, 1);
    }
    else
    {
        rFAIL("missing last layout after br()!");
    }

    columnsInLastRow(columnsInLastRow() + 1);
}

void QtPanel::paintEvent(QPaintEvent *event)
{
    if (ui()->onPaint.valid())
    {
        Paint paint(ui());

        ui()->invokePaint(paint);
    }

    return
        QWidget::paintEvent(event);
}

void QtPanel::timerEvent(QTimerEvent *event)
{
    if (!ui()->destroyed())
    {
        ui()->invokeTimer();
    }

    return
        QWidget::timerEvent(event);
}

void QtPanel::closeEvent(QCloseEvent *event)
{
    if (!ui()->destroyed())
    {
        ui()->invokeClose();
    }

    return
        QWidget::closeEvent(event);
}
void UiTextBased::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);

    std::string style =
        " {"
           "font-size: 20px; "
           "font-family: Verdana, sans-serif; "
           //"color: rgb(0, 50, 100); " (remember disabled state!)
           "padding: 10px; "
           //"background-color: rgb(38,56,76);"
        "}";

    driver()->
        setStyleSheet(
            ("QPushButton" + style + " QLabel" + style).c_str());
}

void UiTextBased::impl_afterUiDefined()
{
}

void UiTextBased::impl_afterText(const std::string& value)
{
    UNUSED(value);
}

DriveBasic* UiButton::newDriver()
{
    return new QtButton(this);
}

QtButton::QtButton(UiButton* ui) :
    QPushButton(Io::Os::debug()? "no text button" : "", ui->driverParentOrNull()),
    ui(ui)
{
    initQtDerived(ui, this);

    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QObject::connect(this, &QPushButton::clicked, this, &QtButton::onClicked);
}

void UiButton::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiButton::impl_afterUiDefined()
{
}

void UiButton::impl_afterText(const std::string& value)
{
    UiTextBased::impl_afterText(value);

    auto set =
        Io::Os::debug()?
            value + " [" + std::to_string(reinterpret_cast<int64_t>(this)) + "]"
            :
            value;

    driver()->setText(set.c_str());
}

void QtButton::onClicked()
{
    ui()->invokeClick();
}

void QtButton::paintEvent(QPaintEvent *event)
{
    return
        QPushButton::paintEvent(event);
}

void QtButton::timerEvent(QTimerEvent *event)
{
    if (!ui()->destroyed())
    {
        ui()->invokeTimer();
    }

    return
        QWidget::timerEvent(event);
}

DriveBasic* UiStatic::newDriver()
{
    return new QtLabel(this);
}

QtLabel::QtLabel(UiStatic* ui) :
    QLabel(Io::Os::debug()? "no text button" : "", ui->driverParentOrNull()),
    ui(ui)
{
    initQtDerived(ui, this);

    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    //QObject::connect(this, &QLabel::clicked, this, &QtLabel::onClicked);
}

void UiStatic::impl_afterConstructor(const std::shared_ptr<UiBasic>& thisUi)
{
    UNUSED(thisUi);
}

void UiStatic::impl_afterUiDefined()
{
}

void UiStatic::impl_afterText(const std::string& value)
{
    UiTextBased::impl_afterText(value);

    auto set =
        Io::Os::debug()?
            value + " [" + std::to_string(reinterpret_cast<int64_t>(this)) + "]"
            :
            value;

    driver()->setText(set.c_str());
}

//void QtLabel::onClicked()
void QtLabel::mousePressEvent(QMouseEvent *event)
{
    UNUSED(event);

    ui()->invokeClick();
}

void QtLabel::paintEvent(QPaintEvent *event)
{
    return
        QLabel::paintEvent(event);
}

void QtLabel::timerEvent(QTimerEvent *event)
{
    if (!ui()->destroyed())
    {
        ui()->invokeTimer();
    }

    return
        QWidget::timerEvent(event);
}

std::string Io::Os::unicode(int code)
{
    rASSERT(code > 0 && code < 0xffff);

    return QString::fromUtf16(reinterpret_cast<const char16_t*>(&code), 1).toUtf8().constData();
}

bool Io::Os::debug()
{
    return false;
}

void Io::Os::say(const std::string& output)
{
    QTextStream out(stdout);

    out << output.c_str();
}

void Io::Os::alert(const std::string& output)
{
    QMessageBox::information(nullptr, "Alert", output.c_str());
}

void Io::Os::yeldToEventLoopForTestingIfSupported()
{
    QCoreApplication::processEvents();
}

bool Io::Os::canYeldToEventLoop()
{
    return true;
}

void Io::Os::diffTwoFiles(
        const std::string& originalFile,
        const std::string& changedFile)
{
    system(
        (std::string() +
            //
            //******** WINDOWS ONLY ********************************************
            //
            //TODO: support non-Windows debugging (test log changes)
            //
#if defined(__linux__)
           // experimental... default 'kompare' install on debian seems OK
           "kompare "
#else
           "difftool\\WinMerge.exe "
#endif
            //
            // insert you favorite GUI diff here!
            //   it must be able to update the log template (the second arg)
            //   from all or selected changes in the temp new log file (the 1st arg)
            //
            //******** WINDOWS ONLY ********************************************
            //
            //
            "\"" + originalFile + "\""
            " "
            "\"" + changedFile + "\""
        ).c_str());
}

bool Io::Os::testLogsAvailable()
{
    return true;
}
