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
#ifndef __REFLECTION_REFLECTION_H__
#define __REFLECTION_REFLECTION_H__

#include "reflection/abstract/ITypeDescription.h"
#include "reflection/typeDescription.h"
#include "reflection/mirror.h"

#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <mutex>

namespace ReflectionHelper
{
    template <typename tT>
    class HasReflect
    {
    private:

        template <typename tClass> static uint8_t test( decltype( &tClass::Reflect ) );
        template <typename tClass> static uint32_t test( ... );

    public:
        enum
        {
            value = sizeof( test<tT>( 0 ) ) == sizeof( uint8_t )
        };
    };


    template< class tClass, bool tHasReflect >
    struct Helper
    {
        static inline void Reflect( Mirror & )
        {
        }
    };

    template< class tClass >
    struct Helper< tClass, true >
    {
        static inline void Reflect( Mirror &mirror )
        {
            tClass::Reflect( mirror );
        }
    };

    template< class tClass >
    static inline void Reflect( Mirror &mirror )
    {
        Helper< tClass, HasReflect< tClass >::value >::Reflect( mirror );
    }
}

static class InternalReflection
{
public:

    InternalReflection();

    ~InternalReflection();

    ITypeDescription *ReflectType( const std::string &name ) const
    {
        auto nameIt = mNameCache.find( name );

        if ( nameIt != mNameCache.end() )
        {
            return mTypes.find( nameIt->second )->second;
        }

        return nullptr;
    }

    template< class tClass >
    TypeDescription< tClass > *ReflectType()
    {
        const size_t typeId = GetClassID< tClass >();

        auto type = mTypes.find( typeId );

        if ( type == mTypes.end() )
        {

            TypeDescription< tClass > *typeDescription = new TypeDescription< tClass >();
            Mirror mirror( typeDescription );

            // prevent infinite recursion by adding the pointer already
            mTypes[ typeId ] = typeDescription;

            Helper< tClass, std::is_class< tClass >::value >::Reflect( mirror );

            if ( mirror.mTypeDescription->GetCName() && !mirror.mTypeDescription->IsBaseClass() )
            {
                mNameCache[mirror.mTypeDescription->GetName()] = typeId;
            }

            return static_cast< TypeDescription< tClass > * >( typeDescription );
        }
        else
        {
            return static_cast< TypeDescription< tClass > * >( type->second );
        }
    }

    template< class tClass >
    void ClearType()
    {
        const size_t typeId = GetClassID< tClass >();

        auto type = mTypes.find( typeId );

        if ( type != mTypes.end() )
        {
            auto nameIt = mNameCache.find( type->second->GetName() );

            if ( nameIt != mNameCache.end() )
            {
                mNameCache.erase( nameIt );
            }

            delete type->second;
            mTypes.erase( type );
        }
    }

    template< class tClass >
    bool IsRegistered()
    {
        const size_t typeId = GetClassID< tClass >();

        return mTypes.find( typeId ) != mTypes.end();
    }

    void ClearTypes();

    template< typename tT >
    size_t GetClassID()
    {
        return AssignClassID< tT >( *this );
    }

    static InternalReflection *GetInstance( InternalReflection *reflection = nullptr );

private:

    std::unordered_map< size_t, ITypeDescription * > mTypes;
    std::unordered_map< std::type_index, size_t > mClassIDCache;
    std::unordered_map< std::string, size_t > mNameCache;

    mutable std::mutex mClassIDLock;
    size_t mClassIDCounter;

    template< class tClass, bool tIsClass >
    struct Helper
    {
        static inline void Reflect( Mirror & )
        {
        }
    };

    template< class tClass >
    struct Helper< tClass, true >
    {
        static inline void Reflect( Mirror &mirror )
        {
            ReflectionHelper::Reflect< tClass >( mirror );
        }
    };

    template< typename tT >
    static size_t AssignClassID( InternalReflection &reflection )
    {
        static size_t mClassId = CacheClassID< tT >( reflection );

        assert( mClassId > 0 && "A class ID of 0 is not allowed." );

        return mClassId;
    }

    template< typename tT >
    static size_t CacheClassID( InternalReflection &reflection )
    {
        const std::type_index typeIndex = std::type_index( typeid( tT ) );

        std::lock_guard< std::mutex > lock( reflection.mClassIDLock );

        auto it = reflection.mClassIDCache.find( typeIndex );

        if ( it == reflection.mClassIDCache.end() )
        {
            reflection.mClassIDCache.insert( std::make_pair( typeIndex, ++reflection.mClassIDCounter ) );
            return reflection.mClassIDCounter;
        }

        return it->second;
    }

} gInternalReflection;

#endif