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
#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextStream>
#include "common.h"
#include "qttesthelpers.h"

TestHelpers::TestHelpers()
{
}

void testQtLayout()
{
    QWidget* floater = new QWidget(NULL);
    floater->show();

    QVBoxLayout* vLayout = new QVBoxLayout;

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPushButton* stretchMe = new QPushButton("stretch me", floater);
    stretchMe->setSizePolicy(QSizePolicy::MinimumExpanding, stretchMe->sizePolicy().verticalPolicy());

    hLayout->addWidget(stretchMe);

    QPushButton* more = new QPushButton("more", floater);
    //more->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    hLayout->addWidget(more);

    vLayout->addLayout(hLayout);
    floater->setLayout(vLayout);
}


