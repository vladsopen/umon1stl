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
#include <QDir>
#include <QPainter>
#include <QWidget>
#include "application.h"
#include "qtui.h"
#include "qttesthelpers.h"
#include "storage.h"

static void prepareCurrentDir()
{
    const QString resourceDir = "public/resources";

    QString exeDir = QCoreApplication::applicationDirPath();

    // search for resource directory when starting from build dir
    //   change the current directory to a top level directory containing
    //   our resource dir
    QString goUp = "";
    bool isFound = false;
    for (int i NOTUSED : FromTo(0, 10)) // some reasonable limit
    {
        QString tryResources = exeDir + "/" + goUp + resourceDir;
        if (QFileInfo::exists(tryResources))
        {
            QDir::setCurrent(tryResources);

            Io::Os::say(qPrintable("Current dir: " + QDir::currentPath() + "\n"));

            // read the original log from file
            //std::ifstream file("all.testlog", std::ios::binary);
            //if (file.is_open())
            //    Io::Os::say("Open!\n");

            rASSERT(QFileInfo::exists("about-dir.txt"));

            isFound = true;
            break;
        }

        goUp += "../";
    }

    rASSERT(isFound);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    prepareCurrentDir();

    App::runFromMain();

    /*
    QWidget* floater = new QWidget(NULL);
    floater->show();
    QPushButton* playButton = new QPushButton("Direct QT test", floater);
    UNUSED(playButton);
    playButton->show();
    */

    return a.exec();
}
