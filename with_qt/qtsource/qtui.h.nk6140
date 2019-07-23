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
#ifndef QTUI_H
#define QTUI_H

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include "common.h"

class QtPanel;
class QtButton;
class QtLabel;

class QtPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QtPanel(UiPanel* ui);

    const GetPredefined<UiPanel*> ui;

    // 'Lines' + 'characters'
    QVBoxLayout* const m_verticalLayout;
    std::vector<QHBoxLayout*> m_rowLayouts;
    GetSet<int> columnsInLastRow;

    // Add a new control creating the first row if necessary
    void addToRowLayout(
            QWidget* add);

    // <BR />
    void br();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};

class QtButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QtButton(UiButton* ui);

    const GetPredefined<UiButton*> ui;

signals:

public slots:
    void onClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
};

class QtLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QtLabel(UiStatic* ui);

    const GetPredefined<UiStatic*> ui;

signals:

public slots:
    //void onClicked(); (QLabel is missing the event, see mousePressEvent)

protected:
    void mousePressEvent(QMouseEvent *event) override; // for click event
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
};

#endif
