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
#ifndef EMJS_H
#define EMJS_H

#include "common.h"

class EmJs : public Object
{
    explicit EmJs() {} // now unused
public:

    struct predefined
    {
        // this is predefined top parent id in the HTML template
        static constexpr int mainWindowParentId = 1;
    };

    // adds all UI control element required for a tag
    //   into the specified parent
    static void createControl(
            const char* tag,
            uiid idNew,
            uiid idParent,
            const char* text);

    static void setTagAttributeToInt(
          uiid id, 
          const char* subelement, 
          const char* attribute, 
          const char* value);

    static void setStyleAttributeToString(
          uiid id, 
          const char* subelement, 
          const char* attribute, 
          const char* value);

    static void replaceInnerHtml(
            uiid id,
            const char* text);

    static void onPointerEvent(
            int id,
            char eventName,
            char deviceType,
            int pointerId,
            int buttonIndex,
            int clientX,
            int clientY,
            bool withShift,
            bool withCtrl,
            bool withAlt);
};

#endif
