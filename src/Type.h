//
//  Copyright (c) 2015-2017 CASM Organization
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcasm-ir
//
//  This file is part of libcasm-ir.
//
//  libcasm-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcasm-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcasm-ir. If not, see <http://www.gnu.org/licenses/>.
//

/**
   @brief    TODO

   TODO
*/

#ifndef _LIB_CASMIR_TYPE_H_
#define _LIB_CASMIR_TYPE_H_

#include "CasmIR.h"

#include "../stdhl/cpp/List.h"
#include "../stdhl/cpp/Log.h"

namespace libcasm_ir
{
    class Agent;
    class Enumeration;

    class Type;
    using Types = libstdhl::List< Type >;

    class Type : public CasmIR
    {
      public:
        using Ptr = std::shared_ptr< Type >;

        enum ID : u8
        {
            _BOTTOM_ = 0,
            VOID,
            LABEL,
            LOCATION,

            AGENT,
            RULE_REFERENCE,
            BOOLEAN,
            INTEGER,
            BIT,
            STRING,
            FLOATING,
            RATIONAL,
            ENUMERATION,
            RELATION,
            _TOP_
        };

        Type( const std::string& name, const std::string& description, ID id );

        ~Type( void ) = default;

        std::string name( void ) const;

        std::string description( void ) const;

        ID id( void ) const;

        const Type& result( void ) const;

        Type::Ptr ptr_result( void ) const;

        Types arguments( void ) const;

        std::string make_hash( void ) const;

        inline u1 operator==( const Type& rhs ) const
        {
            if( this != &rhs )
            {
                if( this->id() != rhs.id()
                    or this->name().compare( rhs.name() ) )
                {
                    return false;
                }
            }
            return true;
        }

        inline u1 operator!=( const Type& rhs ) const
        {
            return !operator==( rhs );
        }

        u1 isVoid( void ) const;
        u1 isLabel( void ) const;
        u1 isLocation( void ) const;
        u1 isRuleReference( void ) const;
        u1 isBoolean( void ) const;
        u1 isInteger( void ) const;
        u1 isBit( void ) const;
        u1 isString( void ) const;
        u1 isFloating( void ) const;
        u1 isRational( void ) const;
        u1 isEnumeration( void ) const;
        u1 isAgent( void ) const;
        u1 isRelation( void ) const;

      protected:
        std::string m_name;
        std::string m_description;

      private:
        ID m_id;

      public:
        std::unordered_map< std::string, Type::Ptr >& make_cache( void )
        {
            static std::unordered_map< std::string, Type::Ptr > cache;
            return cache;
        }
    };

    class PrimitiveType : public Type
    {
      public:
        PrimitiveType( const std::string& name, const std::string& description,
            Type::ID id );
    };

    class VoidType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< VoidType >;

        VoidType( void );
    };

    class LabelType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LabelType >;

        LabelType( void );
    };

    class LocationType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< LocationType >;

        LocationType( void );
    };

    class RuleReferenceType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RuleReferenceType >;

        RuleReferenceType( void );
    };

    class BooleanType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BooleanType >;

        BooleanType( void );
    };

    class IntegerType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< IntegerType >;

        IntegerType( void );
    };

    class BitType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< BitType >;

        static const u16 SizeMax = 64;

        BitType( u16 bitsize );

        u16 bitsize( void ) const;

      private:
        u16 m_bitsize;
    };

    class StringType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< StringType >;

        StringType( void );
    };

    class FloatingType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< FloatingType >;

        FloatingType( void );
    };

    class RationalType final : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< RationalType >;

        RationalType( void );
    };

    class EnumerationType : public PrimitiveType
    {
      public:
        using Ptr = std::shared_ptr< EnumerationType >;

      protected:
        EnumerationType(
            const std::shared_ptr< Enumeration >& kind, Type::ID id );

      public:
        EnumerationType( const std::shared_ptr< Enumeration >& kind );

        Enumeration& kind( void ) const;

        std::shared_ptr< Enumeration > ptr_kind( void ) const;

      private:
        std::shared_ptr< Enumeration > m_kind;
    };

    class AgentType final : public EnumerationType
    {
      public:
        using Ptr = std::shared_ptr< AgentType >;

        AgentType( const std::shared_ptr< Agent >& kind );
    };

    class RelationType final : public Type
    {
      public:
        using Ptr = std::shared_ptr< RelationType >;

      public:
        RelationType( const Type::Ptr& result, const Types& arguments );

        Type::Ptr result( void ) const;

        Types arguments( void ) const;

      private:
        Type::Ptr m_result;
        Types m_arguments;
    };
}

#endif // _LIB_CASMIR_TYPE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
