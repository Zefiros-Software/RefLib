/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */

#pragma once
#ifndef __REFLECTION_ABSTRACTPROPERTY_H__
#define __REFLECTION_ABSTRACTPROPERTY_H__

#include "reflection/accessibility.h"

#include <typeindex>
#include <stdint.h>
#include <string>

class AbstractProperty
{
public:

    virtual ~AbstractProperty()
    {
    }

    virtual std::string GetName() const = 0;

    virtual const char *GetCName() const = 0;

    virtual std::string GetDescription() const = 0;

    virtual const char *GetCDescription() const = 0;

    virtual size_t GetIndex() const = 0;

    virtual std::type_index GetType() const = 0;

    virtual std::type_index GetMemberPtrType() const = 0;

    virtual Accessibility GetAccessibility() const = 0;

    template< typename tClass, typename tProperty >
    void Set( tClass &object, const tProperty &propert )
    {
        Set( static_cast< void * >( &object ), ( void * )( &propert ) );
    }

    template<  typename tProperty, typename tClass >
    tProperty &Get( tClass &object ) const
    {
        return *static_cast<tProperty *>( Get( static_cast<void *>( &object ) ) );
    }

    virtual void Set( void *obj, void *val ) = 0;
    virtual void *Get( void *obj ) const = 0;
};

#endif