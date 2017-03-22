#pragma once
#ifndef __MIRROR_H__
#define __MIRROR_H__

#include <string>

#include <rttr/registration>

namespace RefLib
{
    enum class MirrorMetaData
    {
        SerIndex
    };

    class Mirror
    {
    public:

        template<typename ClassType>
        class ClassMirror
        {
        public:

            ClassMirror( const std::string &name ) : mLink( name ) {}

            template< typename... Args, typename acc_level = rttr::detail::public_access >
            void constructor( acc_level level = acc_level() )
            {
                mLink.constructor<Args...>( level );
            }

            template<typename F, typename... Args, typename acc_level = rttr::detail::public_access  >
            void constructor( F f, Args && ... meta, acc_level level = acc_level() )
            {
                mLink.constructor( f, level )( std::forward<Args>( meta )... );
            }

            template<typename A, typename... Args, typename acc_level = rttr::detail::public_access  >
            void property( rttr::string_view name, A accessor, Args && ... meta, acc_level level = acc_level() )
            {
                mLink.property( name, accessor, level )( std::forward<Args>( meta )... );
            }

            template<typename A1, typename A2, typename... Args, typename acc_level = rttr::detail::public_access  >
            void property( rttr::string_view name, A1 accessor1, A2 accessor2, Args &&
                           ... meta, acc_level level = acc_level() )
            {
                mLink.property( name, accessor1, accessor2, level )( std::forward<Args>( meta )... );
            }

            template<typename A, typename... Args, typename acc_level = rttr::detail::public_access  >
            void property_readonly( rttr::string_view name, A accessor, Args && ... meta, acc_level level = acc_level() )
            {
                mLink.property_readonly( name, accessor, level )( std::forward<Args>( meta )... );
            }

            template<typename F, typename... Args, typename acc_level = rttr::detail::public_access  >
            void method( rttr::string_view name, F f, Args && ... meta, acc_level level = acc_level() )
            {
                mLink.method( name, f, level )( std::forward<Args>( meta )... );
            }

        private:

            rttr::registration::class_<ClassType> mLink;
        };

        template < typename A, typename... Args >
        void property( rttr::string_view name, A accessor, Args &&... meta )
        {
            rttr::registration::property( name, accessor )( std::forward<Args>( meta )... );
        }

        template < typename A1, typename A2, typename... Args >
        void property( rttr::string_view name, A1 accessor1, A2 accessor2, Args &&... meta )
        {
            rttr::registration::property( name, accessor1, accessor2 )( std::forward<Args>( meta )... );
        }

        template < typename A, typename... Args >
        void prop_readonly( rttr::string_view name, A accessor, Args &&... meta )
        {
            rttr::registration::prop_readonly( name, accessor )( std::forward<Args>( meta )... );
        }

        template <  typename F, typename... Args >
        void method( rttr::string_view name, F f, Args &&... meta )
        {
            rttr::registration::method( name, f )( std::forward<Args>( meta )... );
        }

        template < typename E, typename... Args >
        void enumeration( rttr::string_view name, Args &&... meta )
        {
            rttr::registration::enumeration< E >( name )( std::forward<Args>( meta )... );
        }

    private:


    };
}

#endif