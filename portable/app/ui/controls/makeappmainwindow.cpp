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
#include "makeappmainwindow.h"
#include "maketoolbarabove.h"
#include "maketoolbarbelow.h"
#include "makegraphics.h"

MakeAppMainWindow::MakeAppMainWindow(App* app) :
    Context(app->io()),
    app(app),
    makeToolbarAbove(std::make_shared<MakeToolbarAbove>(app)),
    makeToolbarBelow(std::make_shared<MakeToolbarBelow>(app)),
    makeGraphics(std::make_shared<MakeGraphics>(app))
{
}

void MakeAppMainWindow::close()
{
    m_uiMainPanel->destroy();
}

void MakeAppMainWindow::updateAllStates()
{
    if (auto ui = m_uiMainPanel)
    {
        ui->updateAllExistingWindows();
    }
}

void MakeAppMainWindow::create()
{
    //
    // Main window content
    //
    {
        Ui<UiPanel> ui(CreateUi::mainWindow, io());

        m_uiMainPanel = ui;

        ui->spacingY(Design::ySpaceDefault);

        ui->onClose = [=] () mutable
        {
            app()->requestClose();
        };

        //
        // Optional debug/test status line
        //
        *ui << [=] (ParentUi& parent)
        {
            io()->debug()->statusBar(parent);
        };

        ui->br();

        //
        // Toolbar at the top
        //
        *ui << [=] (ParentUi& parent)
        {
            makeToolbarAbove()->create(parent);
        };

        ui->br();

        //
        // Graph panel
        //
        *ui << [=] (ParentUi& parent)
        {
            makeGraphics()->create(parent);
        };

        ui->br();

        //
        // Toolbar at the bottom
        //
        *ui << [=] (ParentUi& parent)
        {
            makeToolbarBelow()->create(parent);
        };
    }
}
