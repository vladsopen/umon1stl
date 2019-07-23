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
#ifndef PROPERTY_TEMPLATE_H
#define PROPERTY_TEMPLATE_H

class Object;

//
// A public class member with functional accessors
//
//     GetSet<int> count;
//     ...
//     int c = x->count();
//     x->count(2);
//

// the base storage, unusable until inherited
template<typename T>
class ClosedProperty
{
public:
    ClosedProperty(const T& init) : m_value(init) {}
    explicit ClosedProperty() : m_value(T()) {}
    ClosedProperty(const ClosedProperty& p) : m_value(p.m_value) {}

private:

    T m_value;

protected:

    ClosedProperty& operator = (const ClosedProperty& assign)
    {
        m_value = assign;
    }

    T get() const
    {
        return m_value;
    }

    void set(const T& assign)
    {
        m_value = assign;
    }
};

// for constructor-initialized constants
template<typename T>
class GetPredefined : private ClosedProperty<T>
{
public:
    using ClosedProperty<T>::ClosedProperty;

    T operator () () const
    {
        return ClosedProperty<T>::get();
    }
};

// for refering to the owner class
template<typename T>
class GetParent : private ClosedProperty<T>
{
public:
    //using ClosedProperty<T>::ClosedProperty;
    GetParent(const T& init) : ClosedProperty<T>(init) {}
    //explicit GetParent() : ClosedProperty<T>(T()) {}
    //GetParent(const GetParent& p) : ClosedProperty<T>(p.get()) {}

    T operator () () const
    {
        T t = ClosedProperty<T>::get();
        rASSERT(t);
        return t;
    }
};

// for refering to dependencies
template<typename T>
class GetRelated : private ClosedProperty<T>
{
public:
    //using ClosedProperty<T>::ClosedProperty;
    GetRelated(const T& init) : ClosedProperty<T>(init) {}
    //explicit GetRelated() : ClosedProperty<T>(T()) {}
    //GetRelated(const GetRelated& p) : ClosedProperty<T>(p.get()) {}

    T operator () () const
    {
        T t = ClosedProperty<T>::get();
        rASSERT(t);
        return t;
    }
};

// a simple get()
template<typename T>
class SetScalar : private ClosedProperty<T>
{
public:
    using ClosedProperty<T>::ClosedProperty;

    void operator () (const T& assign)
    {
        ClosedProperty<T>::set(assign);
    }
};

// generic read/write property
template<typename T>
class GetSet : private ClosedProperty<T>
{
public:
    using ClosedProperty<T>::ClosedProperty;

    T operator () () const
    {
        return ClosedProperty<T>::get();
    }

    void operator () (const T& assign)
    {
        ClosedProperty<T>::set(assign);
    }
};

// with observer attachment support
template<typename T>
class GetSetTell : public GetSet<T>, public WeakMap<Object>
{
public:
    using GetSet<T>::GetSet;

    T operator () () const
    {
        return GetSet<T>::operator () ();
    }

    void operator () (const T& assign)
    {
        GetSet<T>::operator () (assign);
        fire();
    }
};

// for a reference to a content
template<typename T>
class GetUnique : private ClosedProperty<std::shared_ptr<T>>
{
public:
    //using ClosedProperty<std::shared_ptr<T>>::ClosedProperty;
    GetUnique(const std::shared_ptr<T>& init) : ClosedProperty<std::shared_ptr<T>>(init) {}
    //explicit GetUnique() : ClosedProperty<std::shared_ptr<T>>(T()) {}
    //GetUnique(const GetUnique& p) : ClosedProperty<std::shared_ptr<T>>(p.get()) {}

    std::shared_ptr<T> operator () () const
    {
        std::shared_ptr<T> p = ClosedProperty<std::shared_ptr<T>>::get();
        rASSERT(p);
        return p;
    }
};

// for UiXxxx classes with OnFoo handlers
template<typename T>
class AssignOnly : private ClosedProperty<T>
{
public:
    //using ClosedProperty<T>::ClosedProperty;
    //AssignOnly(const T& init) : ClosedProperty<T>(init) {}
    /* explicit */ AssignOnly() : ClosedProperty<T>(T()) {}
    //AssignOnly(const AssignOnly& p) : ClosedProperty<T>(p.get()) {}

    void operator = (const T& assign)
    {
        ClosedProperty<T>::set(assign);
    }

    bool valid() const
    {
        return !!get();
    }

protected:

    // NOTE: you need to inherit from this and make
    //   a friend who can call get()

    T get() const
    {
        return ClosedProperty<T>::get();
    }

};

#endif // PROPERTY_TEMPLATE_H
