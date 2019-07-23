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
#include "application.h"

struct TestStatics
{
    TestStatics() { printf("Static constructors called.\n"); }
    ~TestStatics() { printf("Static destructors called.\n"); }
} test;

int main(void)
{
    printf("Emscripten main started. Build: " __DATE__ " " __TIME__ "\n");

    // prepare fixed resource directory (a lot more needs to be done for test logs)
    EM_ASM(
        FS.mkdir(Pointer_stringify($0)), 
        Log::getLogDirToEnsureMounted());

    App::runFromMain();

    std::cout << "App main done.\n";

    std::cout << "Welcome to the JavaScript message loop.\n";
    //emscripten_set_main_loop(one_iter, 60, 1);
        // we'll hang on timer for now...

    return 0;
}

