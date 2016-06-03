//  
//  Copyright (c) 2015-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/libcasm-ir
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
#include "User.h"
#include "Block.h"

namespace libcasm_ir
{
	class ParallelBlock;
	
	class Specification : public User
	{
	private:
		std::unordered_map< u32, std::vector< Value* > > content;
		std::vector< Value* > content_list;
		
	public:
		Specification( const char* name );
		
		~Specification( void );
	    
		void add( Value* value );

		template< class C >
		bool has( void ) const
		{
			return content.count( C::classid() ) > 0;
		}
		
		template< class C >
		const std::vector< Value* >& get( void ) const
		{
			auto result = content.find( C::classid() );
			assert( result != content.end() );
			return result->second;
		}
		
		void dump( void ) const;
		
		static inline Value::ID classid( void ) { return Value::SPECIFICATION; };
		static bool classof( Value const* obj );
	};
}


#endif /* _LIB_CASMIR_SPECIFICATION_H_ */

//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  