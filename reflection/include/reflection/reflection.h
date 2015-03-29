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

static class InternalReflection
{
public:

    ~InternalReflection()
    {
        for ( auto &types : mTypes )
        {
            delete types.second;
        }
    }

    template< class tClass >
    TypeDescription< tClass > *ReflectType()
    {
        std::type_index typeId = typeid( tClass );

        auto type = mTypes.find( typeId );

        if ( type == mTypes.end() )
        {
            TypeDescription< tClass > *typeDescription = new TypeDescription< tClass >();
            Mirror mirror( typeDescription );

            ReflectionHelper::Reflect< tClass >( mirror );

            mTypes[ typeId ] = typeDescription;

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
        std::type_index typeId = typeid( tClass );

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
        std::type_index typeId = typeid( tClass );

        return mTypes.find( typeId ) != mTypes.end();
    }

private:

    std::unordered_map< std::type_index, ITypeDescription * > mTypes;

} gInternalReflection;



namespace ReflectionHelper
{
    template< class tClass >
    static inline void Reflect( Mirror &mirror )
    {
        tClass::Reflect( mirror );
    }
}

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
}

#endif