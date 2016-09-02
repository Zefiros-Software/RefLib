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
#ifndef __ANY_H__
#define __ANY_H__

#include <string>
#include <vector>
#include <functional>
#include <iosfwd>

struct Any
{

    Any()
    {
    }

    Any( std::string );
    Any( std::vector<std::string> );

    explicit Any( bool );

    explicit Any( int64_t );

    explicit Any( int32_t v )
        : Any( static_cast<int64_t>( v ) )
    {
    }

    ~Any();
    Any( Any const & );
    Any( Any && ) noexcept;
    Any &operator=( Any const & );
    Any &operator=( Any && ) noexcept;

    // Test if this object has any contents at all
    explicit operator bool() const
    {
        return kind != Kind::Empty;
    }

    // Test the type contained by this value object
    bool isBool()       const
    {
        return kind == Kind::Bool;
    }
    bool isString()     const
    {
        return kind == Kind::String;
    }
    bool isLong()       const
    {
        return kind == Kind::Long;
    }
    bool isStringList() const
    {
        return kind == Kind::StringList;
    }

    // Throws std::invalid_argument if the type does not match
    bool asBool() const;
    long asLong() const;
    std::string const &asString() const;
    std::vector<std::string> const &asStringList() const;

    size_t hash() const noexcept;

    // equality is based on hash-equality
    friend bool operator==( value const &, value const & );
    friend bool operator!=( value const &, value const & );

private:
    enum class Kind
    {
        Empty,
        Bool,
        Long,
        String,
        StringList
    };

    union Variant
    {
        Variant() {}
        ~Variant()
        {
            /* do nothing; will be destroyed by ~value */
        }

        bool boolValue;
        long longValue;
        std::string strValue;
        std::vector<std::string> strList;
    };

    static const char *kindAsString( Kind kind )
    {
        switch ( kind )
        {
        case Kind::Empty:
            return "empty";

        case Kind::Bool:
            return "bool";

        case Kind::Long:
            return "long";

        case Kind::String:
            return "string";

        case Kind::StringList:
            return "string-list";
        }

        return "unknown";
    }

    void throwIfNotKind( Kind expected ) const
    {
        if ( kind == expected )
        {
            return;
        }

        std::string error = "Illegal cast to ";
        error += kindAsString( expected );
        error += "; type is actually ";
        error += kindAsString( kind );
        throw std::runtime_error( std::move( error ) );
    }

private:
    Kind kind = Kind::Empty;
    Variant variant{};
};

/// Write out the contents to the ostream
std::ostream &operator<<( std::ostream &, value const & );

namespace std
{
    template <>
    struct hash<docopt::value>
    {
        size_t operator()( docopt::value const &val ) const noexcept
        {
            return val.hash();
        }
    };
}

namespace docopt
{
    inline
    value::value( bool v )
        : kind( Kind::Bool )
    {
        variant.boolValue = v;
    }

    inline
    value::value( long v )
        : kind( Kind::Long )
    {
        variant.longValue = v;
    }

    inline
    value::value( std::string v )
        : kind( Kind::String )
    {
        new( &variant.strValue ) std::string( std::move( v ) );
    }

    inline
    value::value( std::vector<std::string> v )
        : kind( Kind::StringList )
    {
        new( &variant.strList ) std::vector<std::string>( std::move( v ) );
    }

    inline
    value::value( value const &other )
        : kind( other.kind )
    {
        switch ( kind )
        {
        case Kind::String:
            new( &variant.strValue ) std::string( other.variant.strValue );
            break;

        case Kind::StringList:
            new( &variant.strList ) std::vector<std::string>( other.variant.strList );
            break;

        case Kind::Bool:
            variant.boolValue = other.variant.boolValue;
            break;

        case Kind::Long:
            variant.longValue = other.variant.longValue;
            break;

        case Kind::Empty:
        default:
            break;
        }
    }

    inline
    value::value( value &&other ) noexcept
        : kind( other.kind )
    {
        switch ( kind )
        {
        case Kind::String:
            new( &variant.strValue ) std::string( std::move( other.variant.strValue ) );
            break;

        case Kind::StringList:
            new( &variant.strList ) std::vector<std::string>( std::move( other.variant.strList ) );
            break;

        case Kind::Bool:
            variant.boolValue = other.variant.boolValue;
            break;

        case Kind::Long:
            variant.longValue = other.variant.longValue;
            break;

        case Kind::Empty:
        default:
            break;
        }
    }

    inline
    value::~value()
    {
        switch ( kind )
        {
        case Kind::String:
            variant.strValue.~basic_string();
            break;

        case Kind::StringList:
            variant.strList.~vector();
            break;

        case Kind::Empty:
        case Kind::Bool:
        case Kind::Long:
        default:
            // trivial dtor
            break;
        }
    }

    inline
    value &value::operator=( value const &other )
    {
        // make a copy and move from it; way easier.
        return *this = value{ other };
    }

    inline
    value &value::operator=( value &&other ) noexcept
    {
        // move of all the types involved is noexcept, so we dont have to worry about
        // these two statements throwing, which gives us a consistency guarantee.
        this->~value();
        new( this ) value( std::move( other ) );

        return *this;
    }

    template <class T>
    void hash_combine( std::size_t &seed, const T &v );

    inline
    size_t value::hash() const noexcept
    {
        switch ( kind )
        {
        case Kind::String:
            return std::hash<std::string>()( variant.strValue );

        case Kind::StringList:
            {
                size_t seed = std::hash<size_t>()( variant.strList.size() );

                for ( auto const &str : variant.strList )
                {
                    hash_combine( seed, str );
                }

                return seed;
            }

        case Kind::Bool:
            return std::hash<bool>()( variant.boolValue );

        case Kind::Long:
            return std::hash<long>()( variant.longValue );

        case Kind::Empty:
        default:
            return std::hash<void *>()( nullptr );
        }
    }

    inline
    bool value::asBool() const
    {
        throwIfNotKind( Kind::Bool );
        return variant.boolValue;
    }

    inline
    long value::asLong() const
    {
        // Attempt to convert a string to a long
        if ( kind == Kind::String )
        {
            const std::string &str = variant.strValue;
            std::size_t pos;
            const long ret = stol( str, &pos ); // Throws if it can't convert

            if ( pos != str.length() )
            {
                // The string ended in non-digits.
                throw std::runtime_error( str + " contains non-numeric characters." );
            }

            return ret;
        }

        throwIfNotKind( Kind::Long );
        return variant.longValue;
    }

    inline
    std::string const &value::asString() const
    {
        throwIfNotKind( Kind::String );
        return variant.strValue;
    }

    inline
    std::vector<std::string> const &value::asStringList() const
    {
        throwIfNotKind( Kind::StringList );
        return variant.strList;
    }

    inline
    bool operator==( value const &v1, value const &v2 )
    {
        if ( v1.kind != v2.kind )
        {
            return false;
        }

        switch ( v1.kind )
        {
        case value::Kind::String:
            return v1.variant.strValue == v2.variant.strValue;

        case value::Kind::StringList:
            return v1.variant.strList == v2.variant.strList;

        case value::Kind::Bool:
            return v1.variant.boolValue == v2.variant.boolValue;

        case value::Kind::Long:
            return v1.variant.longValue == v2.variant.longValue;

        case value::Kind::Empty:
        default:
            return true;
        }
    }

    inline
    bool operator!=( value const &v1, value const &v2 )
    {
        return !( v1 == v2 );
    }
}
#endif