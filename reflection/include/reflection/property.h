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
#ifndef __REFLECTION_PROPERTYIMPL_H__
#define __REFLECTION_PROPERTYIMPL_H__

#include "reflection/abstract/abstractProperty.h"

#include "reflection/accessibility.h"

#include <typeindex>
#include <stdint.h>
#include <string>

template< class tClass, typename tProperty>
class Property
    : public AbstractProperty
{
    template<class tTypeClass>
    friend class TypeDescription;

    template<class tPropertyClass>
    friend class Properties;

public:

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

    virtual std::string GetName() const override
    {
        if ( mName == nullptr )
        {
            return "";
        }

        return mName;
    }

    virtual const char *GetCName() const override
    {
        return mName;
    }

    virtual std::string GetDescription() const override
    {
        return mDescription != nullptr ? mDescription : "";
    }

    virtual const char *GetCDescription() const override
    {
        return mDescription;
    }

    virtual size_t GetIndex() const override
    {
        return mIndex;
    }

    virtual std::type_index GetType() const override
    {
        return mType;
    }

    virtual std::type_index GetMemberPtrType() const override
    {
        return mPtrType;
    }

    virtual Accessibility GetAccessibility() const override
    {
        return mAccessibility;
    }

    tProperty &Get( tClass &object ) const
    {
        return object.*mMemberPtr;
    }

    void Set( tClass &object, const tProperty &property )
    {
        object.*mMemberPtr = property;
    }

protected:

    Property( tProperty tClass::*variable, Accessibility accessibility, const char *name,
              const char *description, uint32_t customFlags, size_t index )
        : mMemberPtr( variable ),
          mType( typeid( tProperty ) ),
          mPtrType( typeid( variable ) ),
          mName( name ),
          mDescription( description ),
          mIndex( index ),
          mCustomFlags( customFlags ),
          mAccessibility( accessibility )
    {
    }
	
private:

    tProperty tClass::*mMemberPtr;
    std::type_index mType;
    std::type_index mPtrType;
    const char *mName;
    const char *mDescription;
    size_t mIndex;
    uint32_t mCustomFlags;
    Accessibility mAccessibility;
};

#endif