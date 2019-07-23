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
#include "application.h"
#include "design.h"
#include "view.h"
#include "storage.h"
#include "incrementer.h"
#include "waves.h"
#include "makegraphics.h"
#include "drawall.h"

MakeGraphics::MakeGraphics(App* app) :
    Context(app->io()),
    app(app)
{
}

void MakeGraphics::create(
        ParentUi& parent)
{
    auto view = [=] { return app()->view().get(); };

    Ui<UiPanel> ui(parent);

    view()->forTests.uiGraph += ui;

    ui->width(Design::xBestViewWidth);
    ui->height(Design::yBestViewHeight);

    ui->inflateX();
    ui->inflateY();

    ui->timer(20);
        // around 50 fps seems to be fine for modern devices
        //   (until we support incremental repaint)

    ui->onTimer = [=] () mutable
    {
        // debug mode?
        if (app()->asyncTestAllOrNull())
        {
            app()->asyncTestAllOrNull()->nextAsyncStep();
            return;
        }

        rASSERT(app()->isOpen());

        // a HACK! this should go to the system level,
        //   but we still need to enable some timers for the async test
        //   mode to operate!
        //   So for now lets leave it for app to decide which timer it needs.
        if (io()->mock()->disableTimerHandler())
            return;

        app()->incrementer()->step();

        ui->repaint();
    };

    ui->onPaint = [=] (Paint& paint) mutable
    {
        //Paint paint(ui.p().get());

        view()->adjustToPaintSize(paint.size());

        DrawAll drawAll(
            app()->storage()->waves().get(),
            view());

        drawAll.paintAll(paint);
    };

    ui->onPointer = [=] (std::shared_ptr<PointerEvent> ev) mutable
    {
        handlePointer(ev);
    };
}

void MakeGraphics::handlePointer(
        std::shared_ptr<PointerEvent> ev)
{
    // this is a test for pointer handling -
    //   it duplicates arrow button functions
    //   by detecting pointer drag in 4 directions
    // (not very useful for real app where we have horizontal scrolling,
    //   and also vertial direction is problematic in the mobile browser)

    auto view = [=] { return app()->view().get(); };

    if (ev->what() == PointerEvent::pointerDown)
    {
        m_start = { ev->clientX(), ev->clientY() };
        m_pointerId = ev->pointerId();
    }

    if (ev->what() == PointerEvent::pointerUp ||
        ev->what() == PointerEvent::pointerOut)
    {
        m_start = { 0, 0 };
    }

    if (ev->what() == PointerEvent::pointerMove &&
        m_pointerId == ev->pointerId())
    {
        auto moverXorY = [&] (int startXY, int nowXY, auto changer)
        {
            int delta = startXY != 0? startXY - nowXY : 0;
            if (std::abs(delta) > 50)
            {
                changer(delta > 0? View::increase : View::decrease);

                m_start = { ev->clientX(), ev->clientY() };
            }
        };

        moverXorY(m_start.x(), ev->clientX(),
            [=] (auto delta) { view()->changePointsPerScreen(delta); });

        moverXorY(m_start.y(), ev->clientY(),
            [=] (auto delta) { view()->changeAmplitudeScale(delta); });
    }

    //Io::Os::say("onPointer! " + std::to_string((int) ev->what()) + "\n");
}
