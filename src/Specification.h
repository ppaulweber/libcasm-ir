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

#ifndef _LIB_CASMIR_SPECIFICATION_H_
#define _LIB_CASMIR_SPECIFICATION_H_

#include "Value.h"

namespace libcasm_ir
{
    class Specification : public Value
    {
      private:
        std::unordered_map< u8, std::unordered_map< std::string, Value* > >
            m_content;

      public:
        Specification( const char* name );

        ~Specification( void );

        void add( Value* value );

        template < typename C >
        u1 has( void ) const
        {
            return m_content.count( C::classid() ) > 0;
        }

        template < typename C >
        const std::unordered_map< std::string, Value* >& get( void ) const
        {
            auto result = m_content.find( C::classid() );
            assert( result != m_content.end() );
            return result->second;
        }

        static inline Value::ID classid( void )
        {
            return Value::SPECIFICATION;
        }

        static u1 classof( Value const* obj );
    };
}

#endif /* _LIB_CASMIR_SPECIFICATION_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
