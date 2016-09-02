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
#ifndef __REFLECTION_TYPEDESCRIPTION_H__
#define __REFLECTION_TYPEDESCRIPTION_H__

#include "reflection/abstract/ITypeDescription.h"

#include "reflection/property.h"
#include "reflection/defines.h"
#include "reflection/util.h"

#include <typeindex>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <array>
#include <map>
#include <set>

template<class tClass>
class TypeDescription;

class AbstractProperties
{
public:

    enum class AccessibilityType : uint8_t
    {
        UpTo = 0x00,
        DownTo = 0x01,
        Exclusive = 0x02,
        NotEqual = 0x03
    };

    virtual ~AbstractProperties()
    {
    }

    virtual std::vector<AbstractProperty *> GetAll() const = 0;
    virtual std::vector<AbstractProperty *> GetAll( Accessibility accessibility,
                                                    AccessibilityType type = AccessibilityType::DownTo ) const = 0;

    virtual std::set<std::string> GetNames() const = 0;

    virtual std::set<uint32_t> GetIndices() const = 0;
};

template<class tClass>
class Properties
    : public AbstractProperties
{
    friend class Mirror;

public:

    explicit Properties( TypeDescription<tClass> *type )
        : mType( type )
    {

    }

    ~Properties()
    {
        for ( auto &property : mProperties )
        {
            delete property.second;
        }
    }

    template< class tClass2, typename tProperty, typename = typename std::enable_if< std::is_class<tClass2>::value >::type >
    Property< tClass, tProperty> *GetByMemberPtr( tProperty tClass2::*variable ) const
    {
        return mProperties.at( MemberPtrToArray( variable ) );
    }

    template<typename tProperty>
    Property< tClass, tProperty> *Get( size_t index ) const
    {
        Property<tClass, tProperty> *property = static_cast<Property<tClass, tProperty> *>( mPropertyIndices.at( index ) );
        assert( property->GetType() == std::type_index( typeid( tProperty ) ) );
        return property;
    }

    template<typename tProperty>
    std::vector<Property<tClass, tProperty> *> GetAll() const
    {
        std::vector<Property<tClass, tProperty> *> properties = GetBaseClassProperties< tProperty >();

        for ( const auto &property : mProperties )
        {
            if ( property.second->GetType() == std::type_index( typeid( tProperty ) ) )
            {
                properties.push_back( static_cast<Property<tClass, tProperty> *>( property.second ) );
            }
        }

        return properties;
    }

    template<typename tProperty>
    std::vector<Property<tClass, tProperty> *> GetAll( Accessibility accessibility,
                                                       AccessibilityType type = AccessibilityType::DownTo ) const
    {
        std::vector<Property<tClass, tProperty> *> properties = GetBaseClassProperties< tProperty >();

        for ( const auto &property : mProperties )
        {
            if ( property.second->GetType() == std::type_index( typeid( tProperty ) ) &&
                    IsCorrectAccessibilityType( property.second, accessibility, type ) )
            {
                properties.push_back( static_cast<Property<tClass, tProperty> *>( property.second ) );
            }
        }

        return properties;
    }

    std::vector<AbstractProperty *> GetAll() const override
    {
        std::vector<AbstractProperty *> properties = GetBaseClassProperties();

        for ( const auto &property : mProperties )
        {
            properties.push_back( property.second );
        }

        return properties;
    }

    std::vector<AbstractProperty *> GetAll( Accessibility accessibility,
                                            AccessibilityType type = AccessibilityType::DownTo ) const override
    {
        std::vector<AbstractProperty *> properties = GetBaseClassProperties();

        for ( const auto &property : mProperties )
        {
            if ( IsCorrectAccessibilityType( property.second, accessibility, type ) )
            {
                properties.push_back( property.second );
            }
        }

        return properties;
    }

    std::set<std::string> GetNames() const override
    {
        std::set<std::string> names = GetBaseClassNames();

        for ( const auto &name : mPropertyNames )
        {
            names.insert( name.first );
        }

        return names;
    }

    std::set<uint32_t> GetIndices() const override
    {
        std::set<uint32_t> indices;

        for ( const auto &index : mPropertyIndices )
        {
            indices.insert( index.first );
        }

        return indices;
    }

    const Properties<tClass> *operator->() const
    {
        return this;
    }

protected:

    template< class tClass2, typename tProperty, typename = typename std::enable_if< std::is_class<tClass2>::value >::type >
    Property< tClass2, tProperty > *Add( tProperty tClass2::*variable, Accessibility accessibility, size_t index,
                                         uint32_t customFlags, const char *name, const char *description )
    {
        static_assert( sizeof( variable ) <= REFLECTION_MAX_MEMBER_PTR_SIZE,
                       "It seems the member pointer is greater than we did expect, please adjust the maximum member ptr size." );

        Property<tClass2, tProperty> *property = new Property<tClass2, tProperty>( variable, accessibility, name,
                                                                                   description, customFlags, index );

        mProperties.insert( std::make_pair( MemberPtrToArray( variable ), property ) );

        mPropertyIndices[index] = property;

        if ( name != nullptr && strcmp( name, "" ) != 0 )
        {
            mPropertyNames[name] = property;
        }

        return property;
    }

    std::vector<AbstractProperty *> GetProperties() const
    {
        std::vector<AbstractProperty *> properties = GetBaseClassProperties();

        for ( const auto &property : mProperties )
        {
            properties.push_back( property.second );
        }

        return properties;
    }

private:

    template<typename tProperty>
    union MemberPtr
    {
        uint8_t data[REFLECTION_MAX_MEMBER_PTR_SIZE];
        tProperty tClass::*memberPtr;
    };

    std::map<std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE>, AbstractProperty *> mProperties;
    std::map<std::string, AbstractProperty *> mPropertyNames;
    std::map<uint32_t, AbstractProperty *> mPropertyIndices;

    TypeDescription<tClass> *mType;

    std::set<std::string> GetBaseClassNames() const
    {
        std::set<std::string> names;

        for ( const TypeDescription<tClass> &parent : mType->GetBaseClasses() )
        {
            std::set<std::string> parentNames = parent->GetProperties()->GetNames();
            names.insert( parentNames.begin(), parentNames.end() );
        }

        return names;
    }

    std::vector<AbstractProperty *> GetBaseClassProperties() const
    {
        std::vector<AbstractProperty *> properties;

        for ( const TypeDescription<tClass> &parent : mType->GetBaseClasses() )
        {
            std::vector<AbstractProperty *> parentProperties = parent->GetProperties()->GetAll();
            properties.insert( properties.end(), parentProperties.begin(), parentProperties.end() );
        }

        return properties;
    }

    template<typename tProperty>
    std::vector<Property<tClass, tProperty> * > GetBaseClassProperties() const
    {
        std::vector<Property<tClass, tProperty> *> properties;

        for ( const TypeDescription<tClass> &parent : mType->GetBaseClasses() )
        {
            std::vector<Property<tClass, tProperty> *> parentProperties = parent->GetProperties()->GetAll<tProperty>();
            properties.insert( properties.end(), parentProperties.begin(), parentProperties.end() );
        }

        return properties;
    }

    template< class tClass2, typename tProperty, typename = typename std::enable_if< std::is_class<tClass2>::value >::type >
    static inline std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE> MemberPtrToArray( tProperty tClass2::* variable )
    {
        MemberPtr<tProperty> ptr = {};
        ptr.memberPtr = variable;

        std::array<uint8_t, REFLECTION_MAX_MEMBER_PTR_SIZE> array;
        std::copy( std::begin( ptr.data ), std::end( ptr.data ), std::begin( array ) );

        return array;
    }

    static bool IsCorrectAccessibilityType( AbstractProperty *property, Accessibility accessibility,
                                            AccessibilityType type )
    {
        switch ( type )
        {
        case AccessibilityType::UpTo:

            if ( property->GetAccessibility() >= accessibility )
            {
                return true;
            }

            break;

        case AccessibilityType::DownTo:
            if ( property->GetAccessibility() <= accessibility )
            {
                return true;
            }

            break;

        case AccessibilityType::Exclusive:
            if ( property->GetAccessibility() == accessibility )
            {
                return true;
            }

            break;

        case AccessibilityType::NotEqual:

            if ( property->GetAccessibility() != accessibility )
            {
                return true;
            }

            break;

        }

        return false;
    }

};

template<class tClass>
class TypeDescription
    : public ITypeDescription
{
    template<class tTypeClass>
    friend class Properties;
    friend class Mirror;

public:

    enum ParentType
    {
        NoParent = -1
    };

    TypeDescription()
        : mProperties( nullptr ),
          mDescription( nullptr ),
          mName( nullptr ),
          mBaseClassIndex( NoParent ),
          mFlags( 0 ),
          mIsBaseClass( false )
    {
        SetFlags< tClass >();
        Helper< tClass, std::is_class< tClass >::value >::SetProperties( this );
    }

    ~TypeDescription()
    {
        Helper< tClass, std::is_class< tClass >::value >::DeleteProperties( this );
    }

    const TypeDescription *operator->() const
    {
        return this;
    }

    virtual bool IsBaseClass() const override
    {
        return mIsBaseClass;
    }

    virtual int32_t GetBaseClassIndex() const override
    {
        assert( mIsBaseClass );
        return mBaseClassIndex;
    }

    bool HasProperties() const
    {
        return mProperties;
    }

    const Properties<tClass> *GetProperties() const override
    {
        return mProperties;
    }

    std::vector<TypeDescription<tClass>> GetBaseClasses() const
    {
        return mBaseClasses;
    }

    virtual std::string GetDescription() const override
    {
        if ( mDescription )
        {
            return mDescription;
        }

        return nullptr;
    }

    virtual const char *GetCDescription() const override
    {
        return mDescription;
    }

    virtual std::string GetName() const override
    {
        if ( mName )
        {
            return mName;
        }

        return nullptr;
    }

    virtual const char *GetCName() const override
    {
        return mName;
    }

protected:

    explicit TypeDescription( int32_t baseClassIndex )
        : mProperties( nullptr ),
          mDescription( nullptr ),
          mName( nullptr ),
          mBaseClassIndex( baseClassIndex ),
          mFlags( 0 ),
          mIsBaseClass( true )
    {
    }

    template< typename tBaseClass >
    TypeDescription Create( int32_t baseClassIndex )
    {
        TypeDescription description( baseClassIndex );
        description.SetFlags< tBaseClass >();
        Helper< tBaseClass, std::is_class< tBaseClass >::value >::SetProperties( &description );

        return description;
    }

    template< typename tBaseClass >
    void AddBaseClass( int32_t baseClassIndex )
    {
        mBaseClasses.emplace_back( Create< tBaseClass >( baseClassIndex ) );
    }

    void Declare( const char *name, const char *description ) override
    {
        mDescription = description;
        mName = name;
    }

    Properties<tClass> *GetProperties()
    {
        return mProperties;
    }

private:

    template< class tHelperClass, bool tIsClass >
    struct Helper
    {
        static inline void SetProperties( TypeDescription< tHelperClass > * )
        {
        }

        static inline void DeleteProperties( TypeDescription< tHelperClass > * )
        {

        }
    };

    template< class tHelperClass >
    struct Helper< tHelperClass, true >
    {
        static inline void SetProperties( TypeDescription< tHelperClass > *description )
        {
            description->mProperties = new Properties<tHelperClass>( description );
        }

        static inline void DeleteProperties( TypeDescription< tHelperClass > *description )
        {
            delete description->mProperties;
        }
    };

    Properties<tClass> *mProperties;

    std::vector<TypeDescription<tClass>> mBaseClasses;

    const char *mDescription;
    const char *mName;

    int32_t mBaseClassIndex;
    uint32_t mFlags;
    bool mIsBaseClass;

    template< typename tFlagClass >
    void SetFlags()
    {
        mFlags = std::is_array< tFlagClass >::value ? Type::IsArray : 0x00 |
                 std::is_class< tFlagClass >::value ? Type::IsClass : 0x00 |
                 std::is_enum< tFlagClass >::value ? Type::IsEnum : 0x00 |
                 std::is_floating_point< tFlagClass >::value ? Type::IsFloatingPoint : 0x00 |
                 std::is_integral< tFlagClass >::value ? Type::IsIntegral : 0x00 |
                 std::is_member_function_pointer< tFlagClass >::value ? Type::IsMemberFunctionPointer : 0x00 |
                 std::is_member_object_pointer< tFlagClass >::value ? Type::IsMemberObjectPointer : 0x00 |
                 std::is_pointer< tFlagClass >::value ? Type::IsPointer : 0x00 |
                 std::is_union< tFlagClass >::value ? Type::IsUnion : 0x00 |
                 std::is_arithmetic< tFlagClass >::value ? Type::IsArithmetic : 0x00 |
                 std::is_compound< tFlagClass >::value ? Type::IsCompound : 0x00 |
                 std::is_fundamental< tFlagClass >::value ? Type::IsFundamental : 0x00 |
                 std::is_member_pointer< tFlagClass >::value ? Type::IsMemberPointer : 0x00 |
                 std::is_object< tFlagClass >::value ? Type::IsClass : 0x00 |
                 std::is_reference< tFlagClass >::value ? Type::IsReference : 0x00 |
                 std::is_scalar< tFlagClass >::value ? Type::IsScalar : 0x00 |
                 std::is_const< tFlagClass >::value ? Type::IsConst : 0x00 |
                 std::is_empty< tFlagClass >::value ? Type::IsEmpty : 0x00 |
                 std::is_literal_type< tFlagClass >::value ? Type::IsLiteral : 0x00 |
                 std::is_pod< tFlagClass >::value ? Type::IsPOD : 0x00 |
                 std::is_polymorphic< tFlagClass >::value ? Type::IsPolymorphic : 0x00 |
                 std::is_signed< tFlagClass >::value ? Type::IsSigned : 0x00 |
                 std::is_standard_layout< tFlagClass >::value ? Type::IsStandardLayout : 0x00 |
                 std::is_trivial< tFlagClass>::value ? Type::IsTrivial : 0x00 |
                 std::is_unsigned< tFlagClass>::value ? Type::IsUnsigned : 0x00 |
                 std::is_volatile< tFlagClass >::value ? Type::IsVolatile : 0x00;
    }
};

#endif