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

#ifndef _LIB_CASMIR_RULE_H_
#define _LIB_CASMIR_RULE_H_

#include "User.h"

namespace libcasm_ir
{
    class ParallelBlock;

    class Rule : public User
    {
      public:
        Rule( const std::string& name, const Type::Ptr& type );

        ~Rule( void );

        ParallelBlock* context( void ) const;

        void setContext( ParallelBlock* scope );

        void addParameter( Value* value );

        const std::vector< Value* >& parameters( void ) const;

        static inline Value::ID classid( void )
        {
            return Value::RULE;
        }

        static u1 classof( Value const* obj );

      private:
        ParallelBlock* m_context;
        std::vector< Value* > m_parameter;
    };
}

#endif // _LIB_CASMIR_RULE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
