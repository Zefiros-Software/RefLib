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

#include "gtest/gtest.h"

#include "reflection/reflection.h"

#include <iostream>
#include <cstdio>
#include <stdint.h>

class Test
{
public:

    Test()
    {

    }

    static void Reflect( Mirror &mirror )
    {
        mirror.Reflect( &Test::mMember1, 1, "Member1", "The first member of this class" );
        mirror.Reflect( &Test::mMember2, 2, "Member2", "The second member of this class" );
    }

    int32_t mMember1;
    int32_t mMember2;
};

class Test2
{
public:

    Test2()
    {
    }

    int32_t mMember1;
    int32_t mMember2;
};

// namespace Reflect
// {
//     template<>
//     void Reflect<Test2>( Mirror &mirror )
//     {
//         mirror.Reflect( &Test::mMember1, "Member1", "The first member of this class" );
//         mirror.Reflect( &Test::mMember2, "Member2", "The second member of this class" );
//     }
// }
//
struct A { int a; };
struct B : public virtual A   { int b; };
struct C : public virtual A   { int c; };
struct D : public B, public C { int d; };
//
// namespace Reflect
// {
//     template<>
//     void Reflect<A>( Mirror &mirror )
//     {
//         mirror.Reflect( &A::a, "Member1", "The first member of this class" );
//     }
//     template<>
//     void Reflect<B>( Mirror &mirror )
//     {
//         mirror.Reflect( &B::a, "Member1", "The first member of this class" );
//         mirror.Reflect( &B::b, "Member2", "The second member of this class" );
//     }
// }

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc =  ;
#endif
    {

        std::string name = Reflect::GetType<Test>()->GetProperties()->Get( &Test::mMember2 )->GetName();
        std::string description = Reflect::GetType<Test>()->GetProperties()->Get( &Test::mMember2 )->GetDescription();

        //std::string name3 = Reflect::GetName<Test, int32_t>( test, test.mMember2 );
    }
    //     {
    //
    //         std::string name = Reflect::GetName( &Test2::mMember2 );
    //
    //         Test2 test;
    //
    //         std::string name2 = Reflect::GetName( test, &Test2::mMember2 );
    //
    //         //std::string name3 = Reflect::GetName( test, test.mMember2 );
    //     }
    //     {
    //
    //         std::string name = Reflect::GetName( &A::a );
    //
    //         A test;
    //
    //         std::string name2 = Reflect::GetName( test, &A::a );
    //
    //         std::string name3 = Reflect::GetName( test, test.a );
    //     }
    //     {
    //
    //         std::string name = Reflect::GetName( &B::a );
    //
    //         B test;
    //
    //         std::string name2 = Reflect::GetName( test, &B::a );
    //
    //         std::string name3 = Reflect::GetName( test, test.a );
    //     }

    testing::InitGoogleTest( &argc, argv );

    int result = RUN_ALL_TESTS();

    system( "pause" );

    return result;
}
