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

#include "reflection/reflection.h"

#include "helper.h"

namespace
{
    class TestClass
    {
    public:

        uint32_t publicU32Property;

        TestClass()
            : publicU32Property( 0 ),
              mProtectedU32Property( 0 ),
              mPrivateU32Property( 0 )
        {
        }

        static void Reflect( Mirror &mirror )
        {
            mirror.Reflect( "TestClass" );

            mirror.Reflect( &TestClass::publicU32Property, 0 );
            mirror.Reflect( &TestClass::mProtectedU32Property, 1 );
            mirror.Reflect( &TestClass::mPrivateU32Property, 2 );
        }

    protected:

        uint32_t mProtectedU32Property;

    private:

        uint32_t mPrivateU32Property;
    };

    TEST( P( TestClass ), SanityCheck )
    {
        ReflectionClassTest< TestClass > test;

        // Init
        Reflect::GetType<TestClass>();

        EXPECT_NE( nullptr, Reflect::GetType( "TestClass" ) );
        EXPECT_EQ( "TestClass", Reflect::GetType( "TestClass" )->GetName() );

        EXPECT_FALSE( Reflect::GetType( "TestClass" )->IsBaseClass() );

        EXPECT_EQ( 3, Reflect::GetType( "TestClass" )->GetProperties()->GetAll().size() );
    }

    TEST( P( TestClass ), Prop )
    {
        ReflectionClassTest< TestClass > test;

        // Init
        Reflect::GetType<TestClass>();

        EXPECT_NE( nullptr, Reflect::GetType( "TestClass" ) );
        EXPECT_EQ( "TestClass", Reflect::GetType( "TestClass" )->GetName() );

        EXPECT_FALSE( Reflect::GetType( "TestClass" )->IsBaseClass() );

        EXPECT_EQ( 0, Reflect::GetType( "TestClass" )->GetProperties()->GetAll()[0]->GetIndex() );
        EXPECT_EQ( 1, Reflect::GetType( "TestClass" )->GetProperties()->GetAll()[1]->GetIndex() );
        EXPECT_EQ( 2, Reflect::GetType( "TestClass" )->GetProperties()->GetAll()[2]->GetIndex() );

        TestClass variable;

        auto properties = Reflect::GetType( "TestClass" )->GetProperties();
        EXPECT_EQ( 0, properties->GetAll()[ 0 ]->Get< uint32_t >( variable ) );
        EXPECT_EQ( 0, properties->GetAll()[ 1 ]->Get< uint32_t >( variable ) );
        EXPECT_EQ( 0, properties->GetAll()[2]->Get< uint32_t >( variable ) );

        properties->GetAll()[0]->Set( variable, 1 );
        properties->GetAll()[1]->Set( variable, 2 );
        properties->GetAll()[2]->Set( variable, 3 );

        EXPECT_EQ( 1, properties->GetAll()[0]->Get< uint32_t >( variable ) );
        EXPECT_EQ( 2, properties->GetAll()[1]->Get< uint32_t >( variable ) );
        EXPECT_EQ( 3, properties->GetAll()[2]->Get< uint32_t >( variable ) );
    }
}