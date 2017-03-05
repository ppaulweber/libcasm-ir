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

#ifndef _LIB_CASMIR_CONSISTENCY_CHECK_PASS_H_
#define _LIB_CASMIR_CONSISTENCY_CHECK_PASS_H_

#include "libpass.h"

#include "../Specification.h"
#include "../Visitor.h"

/**
   @brief    TODO

   TODO
*/

namespace libcasm_ir
{
    class ConsistencyCheckPass final : public libpass::Pass, public Visitor
    {
      public:
        static char id;

        u1 run( libpass::PassResult& pr ) override;

        LIB_CASMIR_VISITOR_INTERFACE;

        class Data : public libpass::PassData
        {
          public:
            using Ptr = std::shared_ptr< Data >;

            Data( const Specification::Ptr& specification )
            : m_specification( specification )
            {
            }

            Specification::Ptr specification( void ) const
            {
                return m_specification;
            }

          private:
            Specification::Ptr m_specification;
        };
    };
}

#endif // _LIB_CASMIR_CONSISTENCY_CHECK_PASS_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//