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
#ifndef __REFLECTION_MIRROR_H__
#define __REFLECTION_MIRROR_H__

#include "reflection/typeDescription.h"

#include "reflection/reflect.h"

class Mirror
{
    friend class InternalReflection;

public:

    explicit Mirror( ITypeDescription *typeDescription )
        : mTypeDescription( typeDescription )
    {

    }

    template< class tClass, class tProperty >
    void Reflect( tProperty tClass::*variable,
                  size_t index,
                  const char *name = nullptr,
                  const char *description = nullptr )
    {
        GetTypeDescription< tClass >().GetProperties()->Add( variable, Accessibility::Public, index, 0x00, name, description );

        // Reflect the property type as well
        Reflect::GetType< tProperty >();
    }

    template< class tClass, class tProperty >
    void Reflect( tProperty tClass::*variable,
                  size_t index,
                  Accessibility accessibility,
                  const char *name = nullptr,
                  const char *description = nullptr )
    {
        GetTypeDescription< tClass >().GetProperties()->Add( variable, accessibility, index, 0x00, name, description );

        // Reflect the property type as well
        Reflect::GetType< tProperty >();
    }

    template< class tClass, class tProperty >
    void Reflect( size_t tClass::*variable,
                  uint32_t index,
                  Accessibility accessibility,
                  uint32_t customFlags = 0x00,
                  const char *name = nullptr,
                  const char *description = nullptr )
    {
        GetTypeDescription< tClass >().GetProperties().Add( variable, accessibility, index, customFlags, name, description );

        // Reflect the property type as well
        Reflect::GetType< tProperty >();
    }

    template< class tClass, class tProperty >
    void Reflect( tProperty tClass::*variable,
                  size_t index,
                  uint32_t customFlags,
                  Accessibility accessibility = Accessibility::Public,
                  const char *name = nullptr,
                  const char *description = nullptr )
    {
        GetTypeDescription< tClass >().GetProperties().Add( variable, accessibility, index, customFlags, name, description );

        // Reflect the property type as well
        Reflect::GetType< tProperty >();
    }

    template< class tClass, class tProperty >
    void Reflect( tProperty tClass::*variable,
                  uint32_t index,
                  uint32_t customFlags,
                  const char *name = nullptr,
                  const char *description = nullptr )
    {
        GetTypeDescription< tClass >().GetProperties().Add( variable, Accessibility::Public, index, customFlags, name,
                                                            description );

        // Reflect the property type as well
        Reflect::GetType< tProperty >();
    }







    template< class tClass, class tBase >
    void Reflect( uint32_t baseClassIndex )
    {
        GetTypeDescription< tClass >().AddBaseClass< tBase >( baseClassIndex );
    }

private:

    ITypeDescription *mTypeDescription;

    template< class tClass >
    TypeDescription< tClass > &GetTypeDescription()
    {
        return *static_cast< TypeDescription< tClass > * >( mTypeDescription );
    }
};


#endif