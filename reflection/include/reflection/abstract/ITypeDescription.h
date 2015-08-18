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
#ifndef __REFLECTION_ITYPEDESCRIPTION_H__
#define __REFLECTION_ITYPEDESCRIPTION_H__

#include <stdint.h>

class ITypeDescription
{
public:

    enum Type
    {
        IsArray                 = ( 1 << 0 ),
        IsClass                 = ( 1 << 1 ),
        IsEnum                  = ( 1 << 2 ),
        IsFloatingPoint         = ( 1 << 3 ),
        IsIntegral              = ( 1 << 4 ),
        IsMemberFunctionPointer = ( 1 << 5 ),
        IsMemberObjectPointer   = ( 1 << 6 ),
        IsPointer               = ( 1 << 7 ),
        IsUnion                 = ( 1 << 8 ),
        IsArithmetic            = ( 1 << 9 ),
        IsCompound              = ( 1 << 10 ),
        IsFundamental           = ( 1 << 11 ),
        IsMemberPointer         = ( 1 << 12 ),
        IsObject                = ( 1 << 13 ),
        IsReference             = ( 1 << 14 ),
        IsScalar                = ( 1 << 15 ),
        IsConst                 = ( 1 << 16 ),
        IsEmpty                 = ( 1 << 17 ),
        IsLiteral               = ( 1 << 18 ),
        IsPOD                   = ( 1 << 19 ),
        IsPolymorphic           = ( 1 << 20 ),
        IsSigned                = ( 1 << 21 ),
        IsStandardLayout        = ( 1 << 22 ),
        IsTrivial               = ( 1 << 23 ),
        IsUnsigned              = ( 1 << 24 ),
        IsVolatile              = ( 1 << 25 )
    };

    virtual ~ITypeDescription()
    {
    }

    virtual bool IsBaseClass() const = 0;

    virtual int32_t GetBaseClassIndex() const = 0;
};

#endif