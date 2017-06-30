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

#include "Exception.h"

using namespace libcasm_ir;

Exception::Exception( const std::string& message )
: m_message( message )
{
}

const char* Exception::what( void ) const noexcept
{
    return m_message.c_str();
}

TypeArgumentException::TypeArgumentException(
    const std::string& message, const std::size_t position )
: Exception( message )
, m_position( position )
{
}

std::size_t TypeArgumentException::position( void ) const noexcept
{
    return m_position;
}

ValidationException::ValidationException( const std::string& message )
: Exception( message )
{
}

UndefinedConstantException::UndefinedConstantException(
    const std::string& message )
: Exception( message )
{
}

AssertionException::AssertionException( const std::string& message )
: Exception( message )
{
}

AbortException::AbortException( const std::string& message )
: Exception( message )
{
}

InternalException::InternalException( const std::string& message )
: Exception( message )
{
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
