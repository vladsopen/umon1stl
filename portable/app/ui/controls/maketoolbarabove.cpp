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
#include "storage.h"
#include "view.h"
#include "incrementer.h"
#include "feeder.h"
#include "wave.h"
#include "wave.h"
#include "waves.h"
#include "maketoolbarabove.h"

MakeToolbarAbove::MakeToolbarAbove(App* app) :
    Context(app->io()),
    app(app)
{
}

void MakeToolbarAbove::create(
        ParentUi& parent)
{
    Ui<UiPanel> ui(parent);

    ui->spacingX(Design::xSpaceBetweenButtons);

    ui->inflateX(); // take over the whole top row

    //
    // Play/stop
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->onUpdate = [=] () mutable
        {
            ui->text(app()->incrementer()->pause()? "Resume" : "Pause");
        };

        ui->onClick = [=] () mutable
        {
            app()->incrementer()->pause(!app()->incrementer()->pause());
        };
    };

    //
    // Add more button
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text("Force");

        ui->onClick = [=] () mutable
        {
            app()->storage()->waves()->
                acquire(
                    app()->feeder().get(),
                    app()->view().get());
        };
    };

    //
    // Device toolbar additions
    //
    *ui << [=] (ParentUi& parent)
    {
        app()->feeder()->toolbar(parent);
    };

    //
    // Render mode button
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        auto view = [=] { return app()->view(); };

        view()->forTests.uiMode += ui;

        ui->text("Mode");

        ui->onClick = [=] () mutable
        {
            // rotate 3 slit view modes
            if (view()->showSweep() && !view()->showScroll())
            {
                view()->showSweep(false);
                view()->showScroll(true);
            }
            else
            if (!view()->showSweep() && view()->showScroll())
            {
                view()->showSweep(true);
                view()->showScroll(true);
            }
            else
            {
                view()->showSweep(true);
                view()->showScroll(false);
            }

            /*
            if (!view()->sweep())
            {
                int scroll =
                    waves()->pointsRecorded() - static_cast<int>(0.8 * view()->pointsPerScreen());
                if (scroll > 0 &&
                    scroll > view()->pointsScroll())
                    view()->pointsScroll(scroll);
            }
            */
        };
    };

    if (Io::Os::debug())
    {
        //
        // Test: no-hadlers button
        //   (it means no captured back pointers from handlers)
        //
        *ui << [=] (ParentUi& parent)
        {
            Ui<UiButton> ui(parent);

            ui->text("Noop");
        };
    };

    //
    // Tests
    //
    *ui << [=] (ParentUi& parent)
    {
        TestAll::testButton(parent);
    };

    //
    // Spawn button
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text("More");

        ui->onClick = [=] () mutable
        {
            TestAll().spawn();
        };
    };

    //
    // a spacer to force right alignment
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiStatic> ui(parent);

        ui->inflateX();
    };
}
