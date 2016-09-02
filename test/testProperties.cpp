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

        SimplePropertiesBasicRegistration()
            : publicU32Property( 0 ),
              publicU8Property( 0 ),
              mProtectedU32Property( 0 ),
              mProtectedU8Property( 0 ),
              mPrivateU32Property( 0 ),
              mPrivateU8Property( 0 )
        {
        }

        static void Reflect( Mirror &mirror )
        {
            mirror.Reflect( "SimplePropertiesBasicRegistration" );

            mirror.Reflect( &SimplePropertiesBasicRegistration::publicU32Property, 0 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::publicU8Property, 1 );

            mirror.Reflect( &SimplePropertiesBasicRegistration::mProtectedU32Property, 2 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::mProtectedU8Property, 3 );

            mirror.Reflect( &SimplePropertiesBasicRegistration::mPrivateU32Property, 4 );
            mirror.Reflect( &SimplePropertiesBasicRegistration::mPrivateU8Property, 5 );
        }

    protected:

        uint32_t mProtectedU32Property;
        uint8_t mProtectedU8Property;

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

        std::vector< AbstractProperty * > properties =
            Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetAll();

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

        EXPECT_EQ( std::type_index( typeid( &SimplePropertiesBasicRegistration::publicU32Property ) ),
                   properties[0]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   properties[1]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ),
                   properties[2]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   properties[3]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ),
                   properties[4]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   properties[5]->GetMemberPtrType() );
    }

    TEST( P( SimplePropertiesBasicRegistration ), GetProperty )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        const Properties< SimplePropertiesBasicRegistration > *properties =
            Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties();

        SimplePropertiesBasicRegistration variable;

        EXPECT_EQ( 0, properties->Get<uint32_t>( 0 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 1 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint32_t>( 2 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 3 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint32_t>( 4 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 5 )->Get( variable ) );

        properties->Get<uint32_t>( 0 )->Get( variable ) = 5;
        properties->Get<uint8_t>( 1 )->Get( variable ) = 4;
        properties->Get<uint32_t>( 2 )->Get( variable ) = 3;
        properties->Get<uint8_t>( 3 )->Get( variable ) = 2;
        properties->Get<uint32_t>( 4 )->Get( variable ) = 1;
        properties->Get<uint8_t>( 5 )->Get( variable ) = 0;

        EXPECT_EQ( 5, properties->Get<uint32_t>( 0 )->Get( variable ) );
        EXPECT_EQ( 4, properties->Get<uint8_t>( 1 )->Get( variable ) );
        EXPECT_EQ( 3, properties->Get<uint32_t>( 2 )->Get( variable ) );
        EXPECT_EQ( 2, properties->Get<uint8_t>( 3 )->Get( variable ) );
        EXPECT_EQ( 1, properties->Get<uint32_t>( 4 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 5 )->Get( variable ) );
    }

    TEST( P( SimplePropertiesBasicRegistration ), SetProperty )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        const Properties< SimplePropertiesBasicRegistration > *properties =
            Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties();

        SimplePropertiesBasicRegistration variable;

        EXPECT_EQ( 0, properties->Get<uint32_t>( 0 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 1 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint32_t>( 2 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 3 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint32_t>( 4 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 5 )->Get( variable ) );

        properties->Get<uint32_t>( 0 )->Set( variable, 5 );
        properties->Get<uint8_t>( 1 )->Set( variable, 4 );
        properties->Get<uint32_t>( 2 )->Set( variable, 3 );
        properties->Get<uint8_t>( 3 )->Set( variable, 2 );
        properties->Get<uint32_t>( 4 )->Set( variable, 1 );
        properties->Get<uint8_t>( 5 )->Set( variable, 0 );

        EXPECT_EQ( 5, properties->Get<uint32_t>( 0 )->Get( variable ) );
        EXPECT_EQ( 4, properties->Get<uint8_t>( 1 )->Get( variable ) );
        EXPECT_EQ( 3, properties->Get<uint32_t>( 2 )->Get( variable ) );
        EXPECT_EQ( 2, properties->Get<uint8_t>( 3 )->Get( variable ) );
        EXPECT_EQ( 1, properties->Get<uint32_t>( 4 )->Get( variable ) );
        EXPECT_EQ( 0, properties->Get<uint8_t>( 5 )->Get( variable ) );
    }

    TEST( P( SimplePropertiesBasicRegistration ), GetPropertiesByType )
    {
        ReflectionClassTest< SimplePropertiesBasicRegistration > test;

        std::vector< Property<SimplePropertiesBasicRegistration, uint32_t> * > propertiesU32 =
            Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetAll<uint32_t>();
        std::vector< Property<SimplePropertiesBasicRegistration, uint8_t> * > propertiesU8 =
            Reflect::GetType<SimplePropertiesBasicRegistration>()->GetProperties()->GetAll<uint8_t>();

        EXPECT_EQ( 3, propertiesU32.size() );
        EXPECT_EQ( 3, propertiesU8.size() );

        EXPECT_EQ( 0, propertiesU32[0]->GetIndex() );
        EXPECT_EQ( 2, propertiesU32[1]->GetIndex() );
        EXPECT_EQ( 4, propertiesU32[2]->GetIndex() );

        EXPECT_EQ( 1, propertiesU8[0]->GetIndex() );
        EXPECT_EQ( 3, propertiesU8[1]->GetIndex() );
        EXPECT_EQ( 5, propertiesU8[2]->GetIndex() );

        EXPECT_EQ( "", propertiesU32[0]->GetName() );
        EXPECT_EQ( "", propertiesU32[1]->GetName() );
        EXPECT_EQ( "", propertiesU32[2]->GetName() );

        EXPECT_EQ( "", propertiesU8[0]->GetName() );
        EXPECT_EQ( "", propertiesU8[1]->GetName() );
        EXPECT_EQ( "", propertiesU8[2]->GetName() );

        EXPECT_EQ( "", propertiesU32[0]->GetDescription() );
        EXPECT_EQ( "", propertiesU32[1]->GetDescription() );
        EXPECT_EQ( "", propertiesU32[2]->GetDescription() );

        EXPECT_EQ( "", propertiesU8[0]->GetDescription() );
        EXPECT_EQ( "", propertiesU8[1]->GetDescription() );
        EXPECT_EQ( "", propertiesU8[2]->GetDescription() );

        EXPECT_EQ( nullptr, propertiesU32[0]->GetCName() );
        EXPECT_EQ( nullptr, propertiesU32[1]->GetCName() );
        EXPECT_EQ( nullptr, propertiesU32[2]->GetCName() );

        EXPECT_EQ( nullptr, propertiesU8[0]->GetCName() );
        EXPECT_EQ( nullptr, propertiesU8[1]->GetCName() );
        EXPECT_EQ( nullptr, propertiesU8[2]->GetCName() );

        EXPECT_EQ( nullptr, propertiesU32[0]->GetCDescription() );
        EXPECT_EQ( nullptr, propertiesU32[1]->GetCDescription() );
        EXPECT_EQ( nullptr, propertiesU32[2]->GetCDescription() );

        EXPECT_EQ( nullptr, propertiesU8[0]->GetCDescription() );
        EXPECT_EQ( nullptr, propertiesU8[1]->GetCDescription() );
        EXPECT_EQ( nullptr, propertiesU8[2]->GetCDescription() );

        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), propertiesU32[0]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), propertiesU32[1]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t ) ), propertiesU32[2]->GetType() );

        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), propertiesU8[0]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), propertiesU8[1]->GetType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t ) ), propertiesU8[2]->GetType() );

        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU32[0]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU32[1]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint32_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU32[2]->GetMemberPtrType() );

        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU8[0]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU8[1]->GetMemberPtrType() );
        EXPECT_EQ( std::type_index( typeid( uint8_t SimplePropertiesBasicRegistration::* ) ),
                   propertiesU8[2]->GetMemberPtrType() );
    }

    class NoProperties
    {
    public:

        static void Reflect( Mirror & )
        {
        }
    };

    TEST( P( NoProperties ), SanityCheck )
    {
        ReflectionClassTest< NoProperties > test;

        Reflect::GetType<NoProperties>();
        auto properties = Reflect::GetType<NoProperties>()->GetProperties();

        EXPECT_TRUE( properties->GetAll().empty() );
        EXPECT_TRUE( properties->GetNames().empty() );
        EXPECT_TRUE( properties->GetIndices().empty() );
    }


}