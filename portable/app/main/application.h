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
#ifndef APPLICATION_H
#define APPLICATION_H

// Common app includes and definitions included everywhere

#include "common.h"
#include "testall.h"

class App;
    class Storage;
        class Wave;
        class Waves;
    class View;
        class Scaler;
    class Incrementer;
    class Feeder;
        class Emulator;
    class MakeAppMainWindow;
    class DrawAll;
    class TestAll;

//
//  MakeAppMainWindow
//    |
//    |--> MakeXxxx... toolbars, status, etc.
//    |
//    '--> MakeGraphics
//          |      |
//          |      v
//          |     View ---- Scaler
//          |      ^    |
//      (onTimer)  |    '-- DrawXxxx
//          |      |         |
//          v      |         |
//        Incrementer        |
//          |                |
//    ------+----------------v-----
//   |      |             Storage  |
//   |      v                      |
//   |    Waves                    |
//   |      |                      |
//   |      v                      |
//   |    Wave[]                   |
//   |      |                      |
//    ------+----------------------
//          |
//          v
//       Feeder  <---- Emulator
//                 |-- Server
//                 |-- Device
//

// Main application singleton

class App : public Context
{
public:

    using CloseCallback = std::function<void (App* app)>;

    explicit App(
            std::shared_ptr<Io> io,
            CloseCallback closeCallback);

    // called once from platform-dependent main()
    static void runFromMain();

    // init/cleanup everything (useful for leak testing)
    void open();
    void terminate();
    bool isOpen() const;

    // this does not terminate direcly, see closeCallback
    void requestClose();

    // memory cache
    const GetUnique<Storage> storage;

    // input stream
    const GetUnique<Feeder> feeder;

    // graphs
    const GetUnique<View> view;

    // data reader and scroll speed auto control
    const GetUnique<Incrementer> incrementer;

    // main UI
    const GetUnique<MakeAppMainWindow> makeAppMainWindow;

    // testing
    GetSet<std::shared_ptr<TestAll>> asyncTestAllOrNull;
        // (the only nullable ptr in the app and only for testing!)

private:
    bool m_initialized = false;
    bool m_terminated = false;

    CloseCallback m_closeCallback;

    // abstract system environment for our app or for a test run
    const GetUnique<Io> m_io;
};

#endif
