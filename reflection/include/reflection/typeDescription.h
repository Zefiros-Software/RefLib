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
#ifndef __REFLECTION_TYPEDESCRIPTION_H__
#define __REFLECTION_TYPEDESCRIPTION_H__

#include "reflection/abstract/ITypeDescription.h"

#include "reflection/property.h"
#include "reflection/defines.h"
#include "reflection/util.h"

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <array>
#include <map>
#include <set>

template< class tClass >
class TypeDescription;

template< class tClass >
class Properties
{
    friend class Mirror;

public:

    Properties( TypeDescription< tClass > *type )
        : mType( type )
    {

    }

    template< typename tProperty >
    Property Get( tProperty tClass::*variable ) const
    {
        return mProperties.at( MemberPtrToArray( variable ) );
    }

    template< typename tProperty >
    tProperty Get( tClass &obj, const char *name ) const
    {
        return mPropertyNames[name];
    }

    template< typename tProperty >
    tProperty Get( tClass &obj, uint32_t index ) const
    {
        return mPropertyIndices[index];
    }

    std::vector< Property > GetAll() const
    {
        std::vector< Property > properties = GetParentProperties();

        for ( const auto &property : mProperties )
        {
            properties.push_back( property.second );
        }

        return properties;
    }

    std::set< std::string > GetNames() const
    {
        std::set< std::string > names = GetParentNames();

        for ( const auto &name : mPropertyNames )
        {
            names.insert( name.first );
        }

        return names;
    }

    std::set< uint32_t > GetIndices() const
    {
        std::set< uint32_t > indices;

        for ( const auto &index : mPropertyIndices )
        {
            indices.insert( index.first );
        }

        return indices;
    }

    const Properties< tClass > *const operator->() const
    {
        return this;
    }

protected:

    template< typename tProperty >
    Property Add( tProperty tClass::*variable, Accessibility accessibility, uint32_t index, uint32_t customFlags,
                  const char *name, const char *description )
    {
        static_assert( sizeof( variable ) <= REFLECTION_MAX_MEMBER_PTR_SIZE,
                       "It seems the member pointer is greater than we did expect, please adjust the maximum member ptr size." );

        Property property( typeid( tProperty ), typeid( variable ), accessibility, name, description, customFlags, index );

        mProperties.insert( std::make_pair( MemberPtrToArray( variable ), property ) );

        mPropertyIndices[index] = property;

        if ( name != nullptr && strcmp( name, "" ) != 0 )
        {
            mPropertyNames[name] = property;
        }

        return property;
    }

    std::vector< Property > GetProperties() const
    {
        std::vector< Property > properties = GetParentProperties();

        for ( const auto &property : mProperties )
        {
            properties.push_back( property->second );
        }

        return properties;
    }

private:

    template< typename tProperty >
    union MemberPtr
    {
        uint8_t data[REFLECTION_MAX_MEMBER_PTR_SIZE];
        tProperty tClass::*memberPtr;
    };

    std::map< std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE>, Property > mProperties;
    std::map< std::string, Property > mPropertyNames;
    std::map< uint32_t, Property > mPropertyIndices;

    TypeDescription< tClass > *mType;

    std::set< std::string > GetParentNames() const
    {
        std::set< std::string > names;

        for ( const TypeDescription<tClass> &parent : mType->GetParents() )
        {
            std::set<std::string> parentNames = parent->GetProperties()->GetNames();
            names.insert( parentNames.begin(), parentNames.end() );
        }

        return names;
    }

    std::vector< Property > GetParentProperties() const
    {
        std::vector< Property > properties;

        for ( const TypeDescription<tClass> &parent : mType->GetParents() )
        {
            std::vector<Property> parentProperties = parent->GetProperties()->GetAll();
            properties.insert( properties.end(), parentProperties.begin(), parentProperties.end() );
        }

        return properties;
    }
    template< typename tProperty >
    static inline std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE> MemberPtrToArray( tProperty tClass::* variable )
    {
        MemberPtr< tProperty> ptr = {};
        ptr.memberPtr = variable;

        std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE> array;
        std::copy( std::begin( ptr.data ), std::end( ptr.data ), std::begin( array ) );

        return array;
    }

};

template< class tClass >
class TypeDescription
    : public ITypeDescription
{
    template< class tClass >
    friend class Properties;
    friend class Mirror;

public:

    enum ParentType
    {
        NoParent = -1
    };

    TypeDescription( int32_t parentIndex )
        : mProperties( this ),
          mParentIndex( parentIndex ),
          mIsParent( true )
    {
    }

    TypeDescription()
        : mProperties( this ),
          mParentIndex( NoParent ),
          mIsParent( false )
    {
    }

    const TypeDescription *operator->() const
    {
        return this;
    }

    virtual bool IsParent() const override
    {
        return mIsParent;
    }

    virtual int32_t GetParentIndex() const override
    {
        assert( mIsParent );
        return mParentIndex;
    }

    const Properties< tClass > &GetProperties() const
    {
        return mProperties;
    }

    std::vector< TypeDescription< tClass > > GetParents() const
    {
        return mParents;
    }

protected:

    Properties< tClass > &GetProperties()
    {
        return mProperties;
    }

private:

    Properties< tClass > mProperties;

    std::vector< TypeDescription< tClass > > mParents;

    int32_t mParentIndex;
    bool mIsParent;
};

#endif