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
#include "maketoolbarbelow.h"

MakeToolbarBelow::MakeToolbarBelow(App* app) :
    Context(app->io()),
    app(app)
{
}

void MakeToolbarBelow::create(
        ParentUi& parent)
{
    auto view = [=] { return app()->view(); };

    Ui<UiPanel> ui(parent);

    ui->spacingX(Design::xSpaceBetweenButtons);

    ui->inflateX(); // take whole bottom row

    //*ui << [=] (ParentUi& parent)
    //{ Ui<UiStatic> ui(parent); }; // a small pad

    //
    // Status bar
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiStatic> ui(parent);

        // force right alignment for the rest of controls
        ui->inflateX();

        //app()->incrementer()->uiTimeReport += ui;
        app()->incrementer()->timeReport[ui] = [=] () mutable
            { ui->update(); };

        ui->onUpdate = [=] () mutable
        {
            ui->text(app()->incrementer()->timeReport());
        };
    };

    /*
    //
    // a spacer to force right alignment
    //
    {
        Ui<UiStatic> ui(parent);

        ui->inflateX();
    }
    */

    //
    //     Time scale
    // <--- [-]/[+] --->
    //
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text(Io::Os::unicode(0x2b16)); // "<" or Io::Os::unicode(0x25c0)

        ui->width(Design::xIconButton);

        ui->onUpdate = [=] () mutable
        {
            ui->disable(view()->whyCannotChangePointsPerScreen(View::increase));
        };

        ui->onClick = [=] () mutable
        {
            view()->changePointsPerScreen(View::increase);
        };
    };
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text(Io::Os::unicode(0x2b17)); // ">" or Io::Os::unicode(0x25c1)

        ui->width(Design::xIconButton);

        ui->onUpdate = [=] () mutable
        {
            ui->disable(view()->whyCannotChangePointsPerScreen(View::decrease));
        };

        ui->onClick = [=] () mutable
        {
            view()->changePointsPerScreen(View::decrease);
        };
    };

    *ui << [=] (ParentUi& parent)
    { Ui<UiStatic> ui(parent); }; // a small pad

    //        |
    //    Amplitude
    //     [-]|[+]
    //        |
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text(Io::Os::unicode(0x2b18)); // "^"

        ui->width(Design::xIconButton);

        ui->onUpdate = [=] () mutable
        {
            ui->disable(view()->whyCannotChangeAmplitudeScale(View::increase));
        };

        ui->onClick = [=] () mutable
        {
            view()->changeAmplitudeScale(View::increase);
        };
    };
    *ui << [=] (ParentUi& parent)
    {
        Ui<UiButton> ui(parent);

        ui->text(Io::Os::unicode(0x2b19)); // "v"

        ui->width(Design::xIconButton);

        ui->onUpdate = [=] () mutable
        {
            ui->disable(view()->whyCannotChangeAmplitudeScale(View::decrease));
        };

        ui->onClick = [=] () mutable
        {
            view()->changeAmplitudeScale(View::decrease);
        };
    };

    //*ui << [=] (ParentUi& parent)
    //{ Ui<UiStatic> ui(parent); }; // a small pad
}
