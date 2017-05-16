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

#include "Type.h"

#include "Constant.h"
#include "Enumeration.h"
#include "Range.h"

#include "../stdhl/cpp/Random.h"

using namespace libcasm_ir;

static const auto VOID_TYPE = libstdhl::get< VoidType >();

Type::Type( Type::ID id )
: m_id( id )
{
}

Type::ID Type::id( void ) const
{
    return m_id;
}

const Type& Type::result( void ) const
{
    if( isRelation() or isRange() )
    {
        return *m_result.get();
    }

    return *this;
}

Type::Ptr Type::ptr_result( void ) const
{
    if( isRelation() or isRange() )
    {
        return m_result;
    }

    return ptr_this< Type >();
}

const Types& Type::arguments( void ) const
{
    return m_arguments;
}

std::string Type::make_hash( void ) const
{
    return "t:" + std::to_string( id() ) + ":" + description();
}

u1 Type::isVoid( void ) const
{
    return id() == Type::VOID;
}

u1 Type::isLabel( void ) const
{
    return id() == Type::LABEL;
}

u1 Type::isLocation( void ) const
{
    return id() == Type::LOCATION;
}

u1 Type::isBoolean( void ) const
{
    return id() == Type::BOOLEAN;
}

u1 Type::isInteger( void ) const
{
    return id() == Type::INTEGER;
}

u1 Type::isBit( void ) const
{
    return id() == Type::BIT;
}

u1 Type::isString( void ) const
{
    return id() == Type::STRING;
}

u1 Type::isFloating( void ) const
{
    return id() == Type::FLOATING;
}

u1 Type::isRational( void ) const
{
    return id() == Type::RATIONAL;
}

u1 Type::isEnumeration( void ) const
{
    return id() == Type::ENUMERATION;
}

u1 Type::isRange( void ) const
{
    return id() == Type::RANGE;
}

u1 Type::isRelation( void ) const
{
    return id() == Type::RELATION;
}

u1 Type::isReference( void ) const
{
    return isRuleReference() or isFunctionReference();
}

u1 Type::isRuleReference( void ) const
{
    return id() == Type::RULE_REFERENCE;
}

u1 Type::isFunctionReference( void ) const
{
    return id() == Type::FUNCTION_REFERENCE;
}

std::string Type::token( const Type::ID id )
{
    switch( id )
    {
        case _BOTTOM_:
        {
            return "_BOTTOM_";
        }
        case VOID:
        {
            return "Void";
        }
        case LABEL:
        {
            return "Label";
        }
        case LOCATION:
        {
            return "Location";
        }
        case BOOLEAN:
        {
            return "Boolean";
        }
        case INTEGER:
        {
            return "Integer";
        }
        case BIT:
        {
            return "Bit";
        }
        case STRING:
        {
            return "String";
        }
        case FLOATING:
        {
            return "Floating";
        }
        case RATIONAL:
        {
            return "Rational";
        }
        case ENUMERATION:
        {
            return "Enumeration";
        }
        case RANGE:
        {
            return "Range";
        }
        case RELATION:
        {
            return "Relation";
        }
        case RULE_REFERENCE:
        {
            return "RuleRef";
        }
        case FUNCTION_REFERENCE:
        {
            return "FuncRef";
        }
        case _TOP_:
        {
            return "_TOP_";
        }
    }

    assert( !" internal error " );
    return "";
}

//
//
// Primitive Type
//

PrimitiveType::PrimitiveType( Type::ID id )
: Type( id )
{
}

//
// Void Type
//

VoidType::VoidType()
: PrimitiveType( Type::VOID )
{
}

std::string VoidType::name( void ) const
{
    return "v";
}

std::string VoidType::description( void ) const
{
    return token( id() );
}

void VoidType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant VoidType::choose( void ) const
{
    return VoidConstant();
}

//
// Label Type
//

LabelType::LabelType()
: PrimitiveType( Type::LABEL )
{
}

std::string LabelType::name( void ) const
{
    return "label";
}

std::string LabelType::description( void ) const
{
    return token( id() );
}

void LabelType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant LabelType::choose( void ) const
{
    return VoidConstant();
}

//
// Location Type
//

LocationType::LocationType()
: PrimitiveType( Type::LOCATION )
{
}

std::string LocationType::name( void ) const
{
    return "location";
}

std::string LocationType::description( void ) const
{
    return token( id() );
}

void LocationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has no range to process
}

Constant LocationType::choose( void ) const
{
    return VoidConstant();
}

//
// Boolean Type
//

BooleanType::BooleanType()
: PrimitiveType( Type::BOOLEAN )
{
}

std::string BooleanType::name( void ) const
{
    return "b";
}

std::string BooleanType::description( void ) const
{
    return token( id() );
}

void BooleanType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    callback( BooleanConstant( false ) );
    callback( BooleanConstant( true ) );
}

Constant BooleanType::choose( void ) const
{
    auto const value = ( u1 )( libstdhl::Random::uniform< u64 >() % 2 );
    return BooleanConstant( value );
}

//
// Integer Type
//

IntegerType::IntegerType( void )
: PrimitiveType( Type::INTEGER )
, m_range( nullptr )
{
}

IntegerType::IntegerType( const RangeType::Ptr& range )
: PrimitiveType( Type::INTEGER )
, m_range( range )
{
    assert( range );

    if( not range->type().isInteger() )
    {
        throw std::domain_error(
            "range '" + range->name() + "' has to be of type 'Integer'" );
    }

    // if( TODO )
    // {
    //     throw std::domain_error(
    //         "range '"
    //         + range->name()
    //         + "' violates monotonically nondecreasing property of 'Integer'
    //         type" );
    // }
}

u1 IntegerType::constrained( void ) const
{
    return m_range ? true : false;
}

RangeType::Ptr IntegerType::range( void ) const
{
    return m_range;
}

std::string IntegerType::name( void ) const
{
    return "i" + ( m_range ? m_range->name() : "" );
}

std::string IntegerType::description( void ) const
{
    if( not m_range )
    {
        return token( id() );
    }
    else
    {
        return token( id() ) + "'" + m_range->name();
    }
}

void IntegerType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    if( constrained() )
    {
        m_range->foreach( callback );
    }
    else
    {
        // this type has an infinite range to process, therefore omitted (for
        // now)
    }
}

Constant IntegerType::choose( void ) const
{
    if( constrained() )
    {
        return m_range->choose();
    }
    else
    {
        return IntegerConstant( libstdhl::Random::uniform< i64 >() );
        // LIMITATION: currently we only address the 64-bit range for
        // this choosing value range, can be extended later even to address
        // bigger
        // randomized values
    }
}

//
// Bit Type
//

BitType::BitType( u16 bitsize )
: PrimitiveType( Type::BIT )
, m_bitsize( bitsize )
{
    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const IntegerConstant::Ptr& bitsize )
: PrimitiveType( Type::BIT )
{
    assert( bitsize );
    if( static_cast< const libstdhl::Type& >( bitsize->value() )
            > BitType::SizeMax
        or bitsize->value().sign() )
    {
        throw std::domain_error(
            "invalid bit size '" + bitsize->name() + "' for 'Bit' type" );
    }

    m_bitsize = bitsize->value_i64();

    if( m_bitsize < 1 )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

BitType::BitType( const std::string& value, const libstdhl::Type::Radix radix )
: PrimitiveType( Type::BIT )
{
    std::string tmp = value;
    tmp.erase( std::remove( tmp.begin(), tmp.end(), '\'' ), tmp.end() );

    m_bitsize = (u16)tmp.size() * std::log2( (double)radix );

    if( m_bitsize < 1 or m_bitsize > BitType::SizeMax )
    {
        throw std::domain_error( "invalid bit size '"
                                 + std::to_string( m_bitsize )
                                 + "' for 'Bit' type" );
    }
}

u16 BitType::bitsize( void ) const
{
    return m_bitsize;
}

std::string BitType::name( void ) const
{
    return "u" + std::to_string( m_bitsize );
}

std::string BitType::description( void ) const
{
    return token( id() ) + "'" + std::to_string( m_bitsize );
}

void BitType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an (depending on the current bit-size) infinite range to
    // process, therefore omitted (for now)
}

Constant BitType::choose( void ) const
{
    return BitConstant( m_bitsize,
        libstdhl::Random::uniform< u64 >()
            % m_bitsize ); // TODO: FIXME: PPA: fix the randomized value modulo
                           // mapping to full range not only the bitsize
}

//
// String Type
//

StringType::StringType()
: PrimitiveType( Type::STRING )
{
}

std::string StringType::name( void ) const
{
    return "s";
}

std::string StringType::description( void ) const
{
    return token( id() );
}

void StringType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant StringType::choose( void ) const
{
    // this is undefined for now
    return StringConstant();
}

//
// Flaoting Type
//

FloatingType::FloatingType()
: PrimitiveType( Type::FLOATING )
{
}

std::string FloatingType::name( void ) const
{
    return "f";
}

std::string FloatingType::description( void ) const
{
    return token( id() );
}

void FloatingType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant FloatingType::choose( void ) const
{
    // this is undefined for now
    return FloatingConstant();
}

//
// Rational Type
//

RationalType::RationalType()
: PrimitiveType( Type::RATIONAL )
{
}

std::string RationalType::name( void ) const
{
    return "r";
}

std::string RationalType::description( void ) const
{
    return token( id() );
}

void RationalType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant RationalType::choose( void ) const
{
    const auto n = libstdhl::Integer( libstdhl::Random::uniform< i64 >() );
    const auto d = libstdhl::Integer( libstdhl::Random::uniform< i64 >() + 1 );
    // d = randomvalue + 1 to avoid that the denominator is zero!

    return RationalConstant( libstdhl::Rational( n, d ) );
}

//
// Enumeration Type
//

EnumerationType::EnumerationType( const Enumeration::Ptr& kind )
: PrimitiveType( Type::ENUMERATION )
, m_kind( kind )
{
}

Enumeration& EnumerationType::kind( void ) const
{
    return *m_kind.get();
}

Enumeration::Ptr EnumerationType::ptr_kind( void ) const
{
    return m_kind;
}

std::string EnumerationType::name( void ) const
{
    return m_kind->name();
}

std::string EnumerationType::description( void ) const
{
    return m_kind->name();
}

void EnumerationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    for( auto element : m_kind->elements() )
    {
        callback( EnumerationConstant( m_kind, element ) );
    }
}

Constant EnumerationType::choose( void ) const
{
    const auto e = libstdhl::Random::uniform< std::size_t >(
        0, m_kind->elements().size() - 1 );

    return EnumerationConstant( m_kind, m_kind->elements()[ e ] );
}

//
//
// Range Type
//

RangeType::RangeType( const Range::Ptr& range )
: PrimitiveType( Type::RANGE )
, m_range( range )
{
    m_result = range->ptr_type();
}

RangeType::RangeType( const Type::Ptr& type )
: PrimitiveType( Type::RANGE )
, m_range( nullptr )
{
    m_result = type;
}

Range& RangeType::range( void ) const
{
    assert( m_range );
    return *m_range.get();
}

Range::Ptr RangeType::ptr_range( void ) const
{
    return m_range;
}

void RangeType::setRange( const Range::Ptr& range )
{
    assert( range->type() == type() );
    m_range = range;
}

Type& RangeType::type( void ) const
{
    return *m_result;
}

Type::Ptr RangeType::ptr_type( void ) const
{
    return m_result;
}

std::string RangeType::name( void ) const
{
    if( m_range )
    {
        return m_range->name();
    }
    else
    {
        return "[" + type().description() + ".." + type().description() + "]";
    }
}

std::string RangeType::description( void ) const
{
    return name();
}

void RangeType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    assert( m_range );
    if( type().isInteger() )
    {
        const auto& a
            = static_cast< IntegerConstant& >( *range().from() ).value_i64();
        const auto& b
            = static_cast< IntegerConstant& >( *range().to() ).value_i64();

        for( i64 i = a; i <= b; i++ )
        {
            callback( IntegerConstant( i ) );
        }
    }
    else
    {
        throw std::domain_error(
            "unimplemented 'foreach' of range type '" + name() + "'" );
    }
}

Constant RangeType::choose( void ) const
{
    assert( m_range );
    if( type().isInteger() )
    {
        const auto& a
            = static_cast< IntegerConstant& >( *range().from() ).value_i64();
        const auto& b
            = static_cast< IntegerConstant& >( *range().to() ).value_i64();

        return IntegerConstant( libstdhl::Random::uniform< i64 >( a, b ) );
    }
    else
    {
        throw std::domain_error(
            "unimplemented 'foreach' of range type '" + name() + "'" );
        return VoidConstant();
    }
}

//
//
// Relation Type
//

RelationType::RelationType( const Type::Ptr& result, const Types& arguments )
: Type( Type::RELATION )
{
    m_result = result;
    m_arguments = arguments;

#ifndef NDEBUG
    assert( result );
    for( auto argument : arguments )
    {
        assert( argument );
    }
#endif
}

std::string RelationType::name( void ) const
{
    std::string tmp = "<";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += ", ";
        }

        tmp += argument->name();

        first = false;
    }

    tmp += " -> " + m_result->name() + ">";

    return tmp;
}

std::string RelationType::description( void ) const
{
    std::string tmp = "< ";

    u1 first = true;
    for( auto argument : m_arguments )
    {
        if( not first )
        {
            tmp += " * ";
        }

        tmp += argument->description();

        first = false;
    }

    tmp += " -> " + m_result->description() + " >";

    return tmp;
}

void RelationType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant RelationType::choose( void ) const
{
    return VoidConstant();
}

//
// Reference Type
//

ReferenceType::ReferenceType( const Type::ID id, const RelationType::Ptr& type )
: Type( id )
{
    m_result = type;
}

std::string ReferenceType::name( void ) const
{
    return token( id() ) + m_result->name();
}

std::string ReferenceType::description( void ) const
{
    return token( id() ) + m_result->description();
}

void ReferenceType::foreach(
    const std::function< void( const Constant& constant ) >& callback ) const
{
    // this type has an infinite range to process, therefore omitted (for now)
}

Constant ReferenceType::choose( void ) const
{
    return VoidConstant();
}

//
// Rule Reference Type
//

RuleReferenceType::RuleReferenceType( const RelationType::Ptr& type )
: ReferenceType( Type::RULE_REFERENCE, type )
{
}

RuleReferenceType::RuleReferenceType(
    const Type::Ptr& result, const Types& arguments )
: RuleReferenceType( libstdhl::make< RelationType >( result, arguments ) )
{
}

RuleReferenceType::RuleReferenceType( void )
: RuleReferenceType(
      libstdhl::make< RelationType >( libstdhl::make< VoidType >() ) )
{
}

//
// Function Reference Type
//

FunctionReferenceType::FunctionReferenceType( const RelationType::Ptr& type )
: ReferenceType( Type::FUNCTION_REFERENCE, type )
{
}

FunctionReferenceType::FunctionReferenceType(
    const Type::Ptr& result, const Types& arguments )
: FunctionReferenceType( libstdhl::make< RelationType >( result, arguments ) )
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
