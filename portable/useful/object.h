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
#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

// Common base for all non-GUI classes

class Object
{
public:
    explicit Object();
    virtual ~Object(); // virtual! for all derived classes

    // usual cleanup
    Object (const Object&) = delete;
    Object& operator = (const Object&) = delete;
};

// Common base for use with shared_ptr

class Context : public Object // (useless:) : public std::enable_shared_from_this<Context>
{
public:
    explicit Context(
            std::shared_ptr<Io> io);
    virtual ~Context(); // virtual! for all derived classes

    // usual cleanup
    //Context (const Context&) = delete;
    //Context& operator = (const Context&) = delete;

    // context and environment reference
    const GetUnique<Io> io;
};


#endif
