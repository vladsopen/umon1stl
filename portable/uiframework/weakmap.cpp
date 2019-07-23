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

LOCAL_TEST(TestUiMap)(PassIo io)
{
    //std::shared_ptr<Io> io = Io::construct(Io::batch, "GlobalTests");
    auto w1 = std::make_shared<UiPanel>(CreateUi(CreateUi::notFrame, nullptr, io()));
    auto w2 = std::make_shared<UiPanel>(CreateUi(CreateUi::notFrame, nullptr, io()));
    auto w3 = std::make_shared<UiPanel>(CreateUi(CreateUi::notFrame, nullptr, io()));
    auto w4 = std::make_shared<UiPanel>(CreateUi(CreateUi::notFrame, nullptr, io()));

    WeakMap<UiPanel> m;
    assert(m.size() == 0);

    m += w1;
    assert(m.size() == 1);

    m += w1;
    assert(m.size() == 1);

    m += w2;
    assert(m.size() == 2);

    m += w3;
    assert(m.size() == 3);

    w2 = nullptr;
    assert(m.size() == 2);

    w1 = nullptr;
    assert(m.size() == 1);

    m += w4;
    assert(m.size() == 2);

    w3 = nullptr;
    assert(m.size() == 1);

    w4 = nullptr;
    assert(m.size() == 0);

    LOG(all, "Uimap",
        "All asserted, final size=" << m.size());

}

