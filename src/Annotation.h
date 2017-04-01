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

#ifndef _LIB_CASMIR_ANNOTATION_H_
#define _LIB_CASMIR_ANNOTATION_H_

#include "CasmIR.h"

#include "Type.h"
#include "Value.h"

namespace libcasm_ir
{
    /**
       @extends CasmIR
     */
    class Annotation
    {
      public:
        // stores a list of type relations for possible type unification etc.
        // format is: < result type <-- < arg1_type, arg2_type, ... > >
        struct Relation
        {
            Type::ID result;
            std::vector< Type::ID > argument;
        };

        using Data = std::vector< Relation >;
        using Set = std::set< Type::ID >;

        Annotation( const Value::ID id, const Data& info );

        Value::ID id( void ) const;

        Type::ID resultTypeForRelation(
            const std::vector< const Type* > arguments ) const;

        const Set& resultTypes( void ) const;

        const Set& argumentTypes( u8 pos ) const;

        const std::set< std::size_t >& argumentSizes( void ) const;

      private:
        Value::ID m_id;

        const Data m_info;

        std::vector< Set > m_type_set;

        std::set< std::size_t > m_argument_sizes;

        std::unordered_map< std::string, Type::ID > m_relation_to_type;

        static std::unordered_map< std::string, const Annotation* >& str2obj(
            void )
        {
            static std::unordered_map< std::string, const Annotation* > cache;
            return cache;
        }

        static std::unordered_map< u8, const Annotation* >& id2obj( void )
        {
            static std::unordered_map< u8, const Annotation* > cache;
            return cache;
        }

      public:
        static const Annotation& find( const std::string& token );

        static const Annotation& find( const Value::ID id );

        template < class T >
        static const Annotation& find( void )
        {
            return T::info;
        }
    };
}

#endif // _LIB_CASMIR_ANNOTATION_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//