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

#ifndef _LIB_CASMIR_AGENT_H_
#define _LIB_CASMIR_AGENT_H_

#include "User.h"

namespace libcasm_ir
{
    class Rule;
    class RuleReferenceConstant;

    class Agent : public User
    {
      private:
        RuleReferenceConstant* rule_ptr_init;

      public:
        Agent( void );

        ~Agent( void );

        RuleReferenceConstant* getInitRuleReference( void ) const;

        void setInitRuleReference( Value* init );

        Rule* getInitRule( void );

        static inline Value::ID classid( void )
        {
            return Value::AGENT;
        };
        static bool classof( Value const* obj );

        virtual const char* getLabel( void ) override final
        {
            return getName();
        }
    };
}

#endif /* _LIB_CASMIR_AGENT_H_ */

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
