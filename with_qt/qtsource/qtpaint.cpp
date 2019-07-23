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
#include <QPainter>
#include <QWidget>
#include "common.h"
#include "qtui.h"

Size::Size(const QSize& qsize)
{
    m_w = qsize.width();
    m_h = qsize.height();
}

Paint::Paint(UiBasic* ui) :
    m_id(ui),
    m_qpainter(std::make_unique<QPainter>(ui->driver()))
{
    m_qpainter->setRenderHint(QPainter::Antialiasing, true);
}

Paint::~Paint()
{
}

Size Paint::size() const
{
    return
        m_id->driver()->size();
        //Size(1, 1);
}

void Paint::drawLine(Point from, Point to)
{
    QPen qpen(penColor());
    qpen.setWidth(penThickness());

    m_qpainter->setPen(qpen);

    m_qpainter->
        drawLine(
            QPoint(from.x(), from.y()),
            QPoint(to.x(), to.y()));
}

void Paint::drawBar(Point origin, Size extent)
{
    QBrush qbrush(brushColor());

    m_qpainter->setBrush(qbrush);

    m_qpainter->setPen(Qt::NoPen);

    m_qpainter->
        drawRect(
            origin.x(), origin.y(),
            extent.w(), extent.h());
}

Rgb::operator QColor() const
{
    return QColor(red(), green(), blue());
}
