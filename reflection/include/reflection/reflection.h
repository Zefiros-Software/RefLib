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
    template< class tClass >
    static inline void Reflect( Mirror &mirror )
    {
        tClass::Reflect( mirror );
    }
}

static class InternalReflection
{
public:

    InternalReflection()
        : mClassIDCounter( 0 )
    {

    }

    ~InternalReflection()
    {
        ClearTypes();
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

    void ClearTypes()
    {
        for ( auto &types : mTypes )
        {
            delete types.second;
        }

        mTypes.clear();
    }

    template< typename tT >
    size_t GetClassID()
    {
        return AssignClassID< tT >( *this );
    }

private:

    std::unordered_map< size_t, ITypeDescription * > mTypes;
    std::unordered_map< std::type_index, size_t > mClassIDCache;

    std::mutex mClassIDLock;
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


namespace Reflect
{
    template< class tClass >
    static inline const TypeDescription< tClass > *GetType()
    {
        return gInternalReflection.ReflectType< tClass >();
    }

    template< class tClass >
    static inline bool IsRegistered()
    {
        return gInternalReflection.IsRegistered< tClass >();
    }

    template< class tClass >
    static inline void Clear()
    {
        gInternalReflection.ClearType< tClass >();
    }

    static inline void ClearAll()
    {
        gInternalReflection.ClearTypes();
    }
}

#endif