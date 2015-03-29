/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2015 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __REFLECTION_PROPERTY_H__
#define __REFLECTION_PROPERTY_H__

#include "reflection/accessibility.h"

#include <typeindex>
#include <stdint.h>
#include <string>

class Property
{
    template<class tClass>
    friend class TypeDescription;

    template<class tClass>
    friend class Properties;

public:

    enum Type
    {
        Const       = 0x01,
        Volatile    = 0x02
    };

    Property()
        : mType( typeid( void ) ),
          mPtrType( typeid( void * ) )
    {
    }

    Property( const Property &property )
        : mType( property.mType ),
          mPtrType( property.mPtrType ),
          mName( property.mName ),
          mDescription( property.mDescription ),
          mIndex( property.mIndex ),
          mCustomFlags( property.mCustomFlags ),
          mAccessibility( property.mAccessibility )
    {

    }

    Property *operator->()
    {
        return this;
    }

    std::string GetName() const
    {
        if ( mName == nullptr )
        {
            return "";
        }

        return mName;
    }

    const char *GetCName() const
    {
        return mName;
    }

    std::string GetDescription() const
    {
        if ( mDescription == nullptr )
        {
            return "";
        }

        return mDescription;
    }

    const char *GetCDescription() const
    {
        return mDescription;
    }

    uint32_t GetIndex() const
    {
        return mIndex;
    }

    std::type_index GetType() const
    {
        return mType;
    }

    std::type_index GetPtrType() const
    {
        return mType;
    }

protected:

    Property( std::type_index type, std::type_index ptrType, Accessibility accessibility, const char *name,
              const char *description, uint32_t customFlags, uint32_t index )
        : mType( type ),
          mPtrType( ptrType ),
          mName( name ),
          mDescription( description ),
          mIndex( index ),
          mCustomFlags( customFlags ),
          mAccessibility( accessibility )
    {
    }

private:

    std::type_index mType;
    std::type_index mPtrType;
    const char *mName;
    const char *mDescription;
    uint32_t mIndex;
    uint32_t mCustomFlags;
    Accessibility mAccessibility;
};

#endif