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

#include "Constant.h"

#include "../stdhl/cpp/Log.h"

using namespace libcasm_ir;

static constexpr const char* undef_str = "undef";

Constant::Constant( const std::string& name, const Type::Ptr& type, u1 defined,
    u1 symbolic, Value::ID id )
: Value( name, type, id )
, m_defined( defined )
, m_symbolic( symbolic )
{
}

u1 Constant::defined( void ) const
{
    return m_defined;
}

u1 Constant::symbolic( void ) const
{
    return m_symbolic;
}

u1 Constant::classof( Value const* obj )
{
    return obj->id() == classid() or VoidConstant::classof( obj )
           or AgentConstant::classof( obj )
           or RuleReferenceConstant::classof( obj )
           or BooleanConstant::classof( obj ) or IntegerConstant::classof( obj )
           or BitConstant::classof( obj ) or StringConstant::classof( obj )
           or RationalConstant::classof( obj )
           or EnumerationConstant::classof( obj );
}

//
//
// Constants
//

VoidConstant::VoidConstant( void )
: Constant( "void", libstdhl::get< VoidType >(), true, false, classid() )
{
}

u1 VoidConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AgentConstant
//

AgentConstant::AgentConstant( const Agent::Ptr& value, u1 defined, u1 symbolic )
: Constant( ( defined ? "self" : undef_str ), libstdhl::get< AgentType >(),
      defined, symbolic, classid() )
{
    m_value_ptr = value;
}

AgentConstant::AgentConstant( const Agent::Ptr& value )
: AgentConstant( value, true, false )
{
}

AgentConstant::AgentConstant( void )
: AgentConstant( nullptr, false, false )
{
}

Agent::Ptr AgentConstant::value( void ) const
{
    return std::static_pointer_cast< Agent >( m_value_ptr );
}

u1 AgentConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rule Reference Constant
//

RuleReferenceConstant::RuleReferenceConstant(
    const Rule::Ptr& value, u1 defined, u1 symbolic )
: Constant( ( defined ? value->name() : undef_str ),
      libstdhl::get< RuleReferenceType >(), defined, symbolic, classid() )
{
    m_value_ptr = value;
}

RuleReferenceConstant::RuleReferenceConstant( const Rule::Ptr& value )
: RuleReferenceConstant( value, true, false )
{
}

RuleReferenceConstant::RuleReferenceConstant( void )
: RuleReferenceConstant( nullptr, false, false )
{
}

Rule::Ptr RuleReferenceConstant::value( void ) const
{
    return std::static_pointer_cast< Agent >( m_value_ptr );
}

u1 RuleReferenceConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Boolean Constant
//

BooleanConstant::BooleanConstant( u1 value, u1 defined, u1 symbolic )
: Constant( ( defined ? ( value ? "true" : "false" ) : undef_str ),
      libstdhl::get< BooleanType >(), defined, symbolic, classid() )
{
    m_value.m_u1 = value;
}

BooleanConstant::BooleanConstant( u1 value )
: BooleanConstant( value, true, false )
{
}

BooleanConstant::BooleanConstant( void )
: BooleanConstant( false, false, false )
{
}

u1 BooleanConstant::value( void ) const
{
    return m_value.m_u1;
}

u1 BooleanConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Integer Constant
//

IntegerConstant::IntegerConstant( i64 value, u1 defined, u1 symbolic )
: Constant( ( defined ? std::to_string( value ) : undef_str ),
      libstdhl::get< IntegerType >(), defined, symbolic, classid() )
{
    m_value.m_u64 = value;
}

IntegerConstant::IntegerConstant( i64 value )
: IntegerConstant( value, true, false )
{
}

IntegerConstant::IntegerConstant( void )
: IntegerConstant( 0, false, false )
{
}

i64 IntegerConstant::value( void ) const
{
    return (i64)m_value.m_u64;
}

u1 IntegerConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Bit Constant
//

BitConstant::BitConstant(
    const BitType::Ptr& type, u64 value, u1 defined, u1 symbolic )
: Constant( ( defined ? std::to_string( value ) : undef_str ), type, defined,
      symbolic, classid() )
{
    m_value.m_u64 = value & ( ( 1 << type->bitsize() ) - 1 );
}

BitConstant::BitConstant( const BitType::Ptr& type, u64 value )
: BitConstant( type, value, true, false )
{
}

BitConstant::BitConstant( const BitType::Ptr& type )
: BitConstant( type, 0, false, false )
{
}

BitConstant::BitConstant( u16 bitsize, u64 value )
: BitConstant( libstdhl::get< BitType >( bitsize ), value )
{
}

BitConstant::BitConstant( u16 bitsize )
: BitConstant( libstdhl::get< BitType >( bitsize ) )
{
}

u64 BitConstant::value( void ) const
{
    return m_value.m_u64;
}

u1 BitConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// String Constant
//

StringConstant::StringConstant(
    const std::string& value, u1 defined, u1 symbolic )
: Constant( ( defined ? value : undef_str ), libstdhl::get< StringType >(),
      defined, symbolic, classid() )
{
}

StringConstant::StringConstant( const std::string& value )
: StringConstant( value, true, false )
{
}

StringConstant::StringConstant( void )
: StringConstant( nullptr, false, false )
{
}

std::string StringConstant::value( void ) const
{
    return name();
}

u1 StringConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Floating Constant
//

FloatingConstant::FloatingConstant(
    const double value, u1 defined, u1 symbolic )
: Constant( ( defined ? std::to_string( value ) : undef_str ),
      libstdhl::get< FloatingType >(), defined, symbolic, classid() )
{
    m_value.m_dfp = value;
}

FloatingConstant::FloatingConstant( const double value )
: FloatingConstant( value, true, false )
{
}

FloatingConstant::FloatingConstant( void )
: FloatingConstant( 0.0, false, false )
{
}

double FloatingConstant::value( void ) const
{
    return m_value.m_dfp;
}

u1 FloatingConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Rational Constant
//

RationalConstant::RationalConstant(
    const std::string& value, u1 defined, u1 symbolic )
: Constant( ( defined ? value : undef_str ), libstdhl::get< RationalType >(),
      defined, symbolic, classid() )
{
    // PPA: TODO: FIXME: add future rational data layout etc. here
}

RationalConstant::RationalConstant( const std::string& value )
: RationalConstant( value, true, false )
{
}

RationalConstant::RationalConstant( void )
: RationalConstant( nullptr, false, false )
{
}

std::string RationalConstant::value( void ) const
{
    return name();
}

u1 RationalConstant::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// Enumeration Constant
//

EnumerationConstant::EnumerationConstant( const EnumerationType::Ptr& type,
    const std::string& value, u1 defined, u1 symbolic )
: Constant(
      ( defined ? value : undef_str ), type, defined, symbolic, classid() )
{
}

EnumerationConstant::EnumerationConstant(
    const EnumerationType::Ptr& type, const std::string& value )
: EnumerationConstant( type, value, true, false )
{
    m_value.m_u64 = type->kind().encode( value );
}

EnumerationConstant::EnumerationConstant( const EnumerationType::Ptr& type )
: EnumerationConstant( type, nullptr, false, false )
{
}

EnumerationConstant::EnumerationConstant(
    const Enumeration::Ptr& kind, const std::string& value )
: EnumerationConstant( libstdhl::get< EnumerationType >( kind ), value )
{
}

EnumerationConstant::EnumerationConstant( const Enumeration::Ptr& kind )
: EnumerationConstant( libstdhl::get< EnumerationType >( kind ), nullptr )
{
}

u64 EnumerationConstant::value( void ) const
{
    return m_value.m_u64;
}

u1 EnumerationConstant::classof( Value const* obj )
{
    return obj->id() == classid();
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
