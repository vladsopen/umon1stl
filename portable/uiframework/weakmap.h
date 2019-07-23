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
#ifndef UIMAP_H
#define UIMAP_H

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wshadow"
#endif

#include "external/tessil/ordered_map.h"

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

template<class UiX>
class Ui;

// Stores multiple UI pointers for bulk updates
//   and a base for observing GetSetTell properties by UI controls

template<class T>
class WeakMap
{
public:
    WeakMap() {}
    WeakMap (const WeakMap&) = delete;
    WeakMap& operator = (const WeakMap&) = delete;

    // add a T to the map
    void operator += (
            std::shared_ptr<T> p)
    {
        rASSERT(p);
        Callbacks& callbacks = m_map[p.get()];
        callbacks.ptr = std::weak_ptr<T>(p);
    }

    // this could be optimized with fast lookup skipping null items
    bool isEmpty()
            const
    {
        return size() == 0;
    }

    // returns clean count!
    int size()
            const
    {
        clearNulls();
        return useful::size(m_map);
    }

    // example: uiFoo.apply([] (auto ui) { ui->update(); });
    template<class F>
    void apply(
            F f)
    {
        bool needClearNulls = false;
        for (auto& weak : m_map)
        {
            if (auto ui = weak.second.ptr.lock())
                f(ui);
            else
                needClearNulls = true;
        }

        if (needClearNulls)
            clearNulls();
    }

    // notify all callback lambdas
    void fire()
    {
        bool needClearNulls = false;
        for (const auto& weak : m_map)
        {
            const Callbacks& callbacks = weak.second;
            if (auto ptr = callbacks.ptr.lock())
            {
                //for (auto& target : callbacks.subscribers)
                    if (callbacks.subscriber)
                        callbacks.subscriber();
            }
            else
                needClearNulls = true;
        }

        if (needClearNulls)
            clearNulls();
    }

    //
    // support for += syntax: obj->uimapFoo(ui) = [=] { ui->update(); };
    //

    template<class TT>
    friend class Assignable;

    template<class TT>
    class Assignable
    {
    public:
        Assignable(WeakMap& map, std::shared_ptr<TT> ptr) :
            m_map(map), m_ptr(ptr) {}

        void operator = (const std::function<void ()>& callback)
        {
            Callbacks& callbacks = m_map.m_map[m_ptr.get()];
            //callbacks.subscribers.push_back(callback);
            callbacks.subscriber = callback;
            callbacks.ptr = std::weak_ptr<T>(m_ptr);
        }

        /*
        void clear()
        {
            Callbacks& callbacks = m_map.m_map[m_ptr.get()];
            callbacks.clear();
        }
        */

    private:
        WeakMap& m_map;
        const std::shared_ptr<TT> m_ptr;
    };

    Assignable<T> operator [] (std::shared_ptr<T> ptr)
    {
        return Assignable<T>(*this, ptr);
    }

    Assignable<T> operator [] (Ui<T> ui)
    {
        return Assignable<T>(*this, ui.shared_ptr());
    }

private:

    struct Callbacks
    {
        std::weak_ptr<T> ptr;
        std::function<void ()> subscriber;
        //std::vector<std::function<void ()>> subscribers;
    };

    using Map = tsl::ordered_map<T*, Callbacks>;

    //
    // Iterators (copied from ordered_map)
    //

    using iterator = typename Map::iterator;
    using const_iterator = typename Map::const_iterator;
    using reverse_iterator = typename Map::reverse_iterator;
    using const_reverse_iterator = typename Map::const_reverse_iterator;

    iterator begin() noexcept { clearNulls(); return m_map.begin(); }
    const_iterator begin() const noexcept { clearNulls(); return m_map.begin(); }
    const_iterator cbegin() const noexcept { clearNulls(); return m_map.cbegin(); }

    iterator end() noexcept { return m_map.end(); }
    const_iterator end() const noexcept { return m_map.end(); }
    const_iterator cend() const noexcept { return m_map.cend(); }

    reverse_iterator rbegin() noexcept { clearNulls(); return m_map.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { clearNulls(); return m_map.rbegin(); }
    const_reverse_iterator rcbegin() const noexcept { clearNulls(); return m_map.rcbegin(); }

    reverse_iterator rend() noexcept { return m_map.rend(); }
    const_reverse_iterator rend() const noexcept { return m_map.rend(); }
    const_reverse_iterator rcend() const noexcept { return m_map.rcend(); }

private:
    mutable Map m_map;
        // mutable because we clear dead nulls transparently

    // internal (const!)
    void clearNulls()
            const
    {
        // slow(!) but simple cleanup
        for (int iRepeat = 0; iRepeat < useful::size(m_map); ++iRepeat)
        {
            bool foundNull = false;

            for (const auto& p : m_map)
            {
                if (p.second.ptr.expired())
                {
                    m_map.erase(p.first);

                    // depending on map impl we may not be able to continue
                    foundNull = true;
                    break;
                }
            }

            if (!foundNull)
                break;
        }
    }
};

#endif
