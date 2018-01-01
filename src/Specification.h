//
//  Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/casm-lang/libcasm-ir>
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
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-ir give you permission to link libcasm-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-ir. If you modify libcasm-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#ifndef _LIBCASM_IR_SPECIFICATION_H_
#define _LIBCASM_IR_SPECIFICATION_H_

#include <libcasm-ir/Value>

#include <libcasm-ir/Agent>
#include <libcasm-ir/Builtin>
#include <libcasm-ir/Constant>
#include <libcasm-ir/Derived>
#include <libcasm-ir/Function>
#include <libcasm-ir/Instruction>
#include <libcasm-ir/Rule>
#include <libcasm-ir/Statement>

namespace libcasm_ir
{
    class Specification final : public Value
    {
      public:
        using Ptr = std::shared_ptr< Specification >;

        Specification( const std::string& name );

        ~Specification( void ) = default;

        void setAgent( const Agent::Ptr& agent );
        Agent::Ptr agent( void ) const;

        void add( const Type::Ptr& type );
        void add( const Constant::Ptr& constant );
        void add( const Builtin::Ptr& builtin );
        void add( const Function::Ptr& function );
        void add( const Derived::Ptr& derived );
        void add( const Rule::Ptr& rule );

        Types& types( void );
        Constants& constants( void );
        Builtins& builtins( void );
        Functions& functions( void );
        Deriveds& deriveds( void );
        Rules& rules( void );

        template < typename T, typename... Args >
        typename T::Ptr set( Args&&... args )
        {
            auto obj = libstdhl::Memory::make< T >( std::forward< Args >( args )... );
            setAgent( obj );
            return obj;
        }

        template < typename T, typename... Args >
        typename T::Ptr add( Args&&... args )
        {
            auto obj = libstdhl::Memory::make< T >( std::forward< Args >( args )... );
            add( obj );
            return obj;
        }

        std::string name( void ) const override;

        std::size_t hash( void ) const override;

        u1 operator==( const Value& rhs ) const override;

        void accept( Visitor& visitor ) override;

        static inline Value::ID classid( void )
        {
            return Value::SPECIFICATION;
        }

        static u1 classof( Value const* obj );

      private:
        std::string m_name;

        Agent::Ptr m_agent;

        Types m_types;
        Constants m_constants;
        Builtins m_builtins;
        Functions m_functions;
        Deriveds m_deriveds;
        Rules m_rules;
    };
}

#endif  // _LIBCASM_IR_SPECIFICATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
