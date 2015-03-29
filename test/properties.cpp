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

#include "reflection/reflection.h"

#include "helper.h"

#include "gtest/gtest.h"

namespace
{
    class SimplePropertiesBasicRegistration
    {
    public:

        uint32_t publicU32Property;
        uint8_t publicU8Property;

        static void Reflect( Mirror &mirror )
        {
            mirror.Reflect( &SimplePropertiesBasicRegistration::publicU32Property, 0 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::publicU8Property, 1 );

            mirror.Reflect( &SimplePropertiesBasicRegistration::mProtectedU32Property, 2 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::mPropertyU8Property, 3 );

            mirror.Reflect( &SimplePropertiesBasicRegistration::mPrivateU32Property, 4 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::mPrivateU8Property, 5 );
        }

    protected:

        uint32_t mProtectedU32Property;
        uint8_t mPropertyU8Property;

    private:

        uint32_t mPrivateU32Property;
        uint8_t mPrivateU8Property;
    };

    TEST( P( SimplePropertiesBasicRegistration ), SanityCheck )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        Reflect::GetType<SimplePropertiesBasicRegistration>();
        Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties();
    }

    TEST( P( SimplePropertiesBasicRegistration ), NoNames )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        std::set< std::string > names = Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetNames();

        EXPECT_EQ( 0, names.size() );
    }

    TEST( P( SimplePropertiesBasicRegistration ), CorrectIndices )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        std::set< uint32_t > indices = Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetIndices();

        std::vector< uint32_t > idVec( indices.begin(), indices.end() );
        EXPECT_EQ( 6, indices.size() );
        EXPECT_EQ( 0, idVec[0] );
        EXPECT_EQ( 1, idVec[1] );
        EXPECT_EQ( 2, idVec[2] );
        EXPECT_EQ( 3, idVec[3] );
        EXPECT_EQ( 4, idVec[4] );
        EXPECT_EQ( 5, idVec[5] );
    }

    TEST( P( SimplePropertiesBasicRegistration ), CorrectProperties )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        std::vector< Property > properties = Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetAll();

        EXPECT_EQ( 6, properties.size() );

        EXPECT_EQ( 0, properties[0]->GetIndex() );
        EXPECT_EQ( 1, properties[1]->GetIndex() );
        EXPECT_EQ( 2, properties[2]->GetIndex() );
        EXPECT_EQ( 3, properties[3]->GetIndex() );
        EXPECT_EQ( 4, properties[4]->GetIndex() );
        EXPECT_EQ( 5, properties[5]->GetIndex() );

        EXPECT_EQ( "", properties[0]->GetName() );
        EXPECT_EQ( "", properties[1]->GetName() );
        EXPECT_EQ( "", properties[2]->GetName() );
        EXPECT_EQ( "", properties[3]->GetName() );
        EXPECT_EQ( "", properties[4]->GetName() );
        EXPECT_EQ( "", properties[5]->GetName() );

        EXPECT_EQ( "", properties[0]->GetDescription() );
        EXPECT_EQ( "", properties[1]->GetDescription() );
        EXPECT_EQ( "", properties[2]->GetDescription() );
        EXPECT_EQ( "", properties[3]->GetDescription() );
        EXPECT_EQ( "", properties[4]->GetDescription() );
        EXPECT_EQ( "", properties[5]->GetDescription() );

        EXPECT_EQ( nullptr, properties[0]->GetCName() );
        EXPECT_EQ( nullptr, properties[1]->GetCName() );
        EXPECT_EQ( nullptr, properties[2]->GetCName() );
        EXPECT_EQ( nullptr, properties[3]->GetCName() );
        EXPECT_EQ( nullptr, properties[4]->GetCName() );
        EXPECT_EQ( nullptr, properties[5]->GetCName() );

        EXPECT_EQ( nullptr, properties[0]->GetCDescription() );
        EXPECT_EQ( nullptr, properties[1]->GetCDescription() );
        EXPECT_EQ( nullptr, properties[2]->GetCDescription() );
        EXPECT_EQ( nullptr, properties[3]->GetCDescription() );
        EXPECT_EQ( nullptr, properties[4]->GetCDescription() );
        EXPECT_EQ( nullptr, properties[5]->GetCDescription() );

        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), properties[0]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), properties[1]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), properties[2]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), properties[3]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), properties[4]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), properties[5]->GetType() );

        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ), properties[0]->GetPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ), properties[1]->GetPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ), properties[2]->GetPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ), properties[3]->GetPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ), properties[4]->GetPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ), properties[5]->GetPtrType() );
    }
}