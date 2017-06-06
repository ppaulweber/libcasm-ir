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

#include "Builtin.h"

#include "Exception.h"

using namespace libcasm_ir;

static const auto VOID = libstdhl::get< VoidType >();
static const auto BOOLEAN = libstdhl::get< BooleanType >();
static const auto INTEGER = libstdhl::get< IntegerType >();
static const auto FLOATING = libstdhl::get< FloatingType >();
static const auto RATIONAL = libstdhl::get< RationalType >();
static const auto STRING = libstdhl::get< StringType >();

Builtin::Builtin( const Type::Ptr& type, const Value::ID id )
: User( "", type, id )
{
}

std::string Builtin::name( void ) const
{
    return Value::token( id() );
}

void Builtin::accept( Visitor& visitor )
{
    visitor.visit( *this );
}

u1 Builtin::classof( Value const* obj )
{
    return obj->id() == classid() or GeneralBuiltin::classof( obj )
           or CastingBuiltin::classof( obj ) or StringifyBuiltin::classof( obj )
           // or MathBuiltin::classof( obj )
           or OperatorBuiltin::classof( obj ) or BitBuiltin::classof( obj );
}

u1 Builtin::available( const std::string& token, std::size_t argumentSize )
{
    try
    {
        const auto& annotation = Annotation::find( token );

        auto result = annotation.argumentSizes().find( argumentSize );
        if( result == annotation.argumentSizes().end() )
        {
            return false;
        }
    }
    catch( const std::domain_error& e )
    {
        return false;
    }
    catch( const std::exception& e )
    {
        throw e;
    }

    return true;
}

Builtin::Ptr Builtin::find( const std::string& name, const Type::Ptr& type )
{
    if( name.rfind( "as", 0 ) == 0 )
    {
        return asBuiltin( type );
    }
    else if( name.compare( "dec" ) == 0 )
    {
        return libstdhl::get< DecBuiltin >( type );
    }
    else if( name.compare( "hex" ) == 0 )
    {
        return libstdhl::get< HexBuiltin >( type );
    }
    else if( name.compare( "oct" ) == 0 )
    {
        return libstdhl::get< OctBuiltin >( type );
    }
    else if( name.compare( "bin" ) == 0 )
    {
        return libstdhl::get< BinBuiltin >( type );
    }

    else if( name.compare( "addu" ) == 0 )
    {
        return libstdhl::get< AdduBuiltin >( type );
    }
    else if( name.compare( "adds" ) == 0 )
    {
        return libstdhl::get< AddsBuiltin >( type );
    }
    else if( name.compare( "subu" ) == 0 )
    {
        return libstdhl::get< SubuBuiltin >( type );
    }
    else if( name.compare( "subs" ) == 0 )
    {
        return libstdhl::get< SubsBuiltin >( type );
    }
    else if( name.compare( "mulu" ) == 0 )
    {
        return libstdhl::get< MuluBuiltin >( type );
    }
    else if( name.compare( "muls" ) == 0 )
    {
        return libstdhl::get< MulsBuiltin >( type );
    }

    else if( name.compare( "lesu" ) == 0 )
    {
        return libstdhl::get< LesuBuiltin >( type );
    }
    else if( name.compare( "less" ) == 0 )
    {
        return libstdhl::get< LessBuiltin >( type );
    }
    else if( name.compare( "lequ" ) == 0 )
    {
        return libstdhl::get< LequBuiltin >( type );
    }
    else if( name.compare( "leqs" ) == 0 )
    {
        return libstdhl::get< LeqsBuiltin >( type );
    }
    else if( name.compare( "greu" ) == 0 )
    {
        return libstdhl::get< GreuBuiltin >( type );
    }
    else if( name.compare( "gres" ) == 0 )
    {
        return libstdhl::get< GresBuiltin >( type );
    }
    else if( name.compare( "gequ" ) == 0 )
    {
        return libstdhl::get< GequBuiltin >( type );
    }
    else if( name.compare( "geqs" ) == 0 )
    {
        return libstdhl::get< GeqsBuiltin >( type );
    }

    else if( name.compare( "zext" ) == 0 )
    {
        return libstdhl::get< ZextBuiltin >( type );
    }
    else if( name.compare( "sext" ) == 0 )
    {
        return libstdhl::get< SextBuiltin >( type );
    }
    else if( name.compare( "trunc" ) == 0 )
    {
        return libstdhl::get< TruncBuiltin >( type );
    }
    else if( name.compare( "shl" ) == 0 )
    {
        return libstdhl::get< ShlBuiltin >( type );
    }
    else if( name.compare( "shr" ) == 0 )
    {
        return libstdhl::get< ShrBuiltin >( type );
    }
    else if( name.compare( "ashr" ) == 0 )
    {
        return libstdhl::get< AshrBuiltin >( type );
    }
    else if( name.compare( "clz" ) == 0 )
    {
        return libstdhl::get< ClzBuiltin >( type );
    }
    else if( name.compare( "clo" ) == 0 )
    {
        return libstdhl::get< CloBuiltin >( type );
    }
    else if( name.compare( "cls" ) == 0 )
    {
        return libstdhl::get< ClsBuiltin >( type );
    }

    else
    {
        throw std::domain_error(
            "could not find a builtin for '" + name + "'" );
    }
}

Builtin::Ptr Builtin::asBuiltin( const Type::Ptr& type )
{
    switch( type->result().id() )
    {
        case Type::BOOLEAN:
        {
            return libstdhl::get< AsBooleanBuiltin >( type );
        }
        case Type::INTEGER:
        {
            return libstdhl::get< AsIntegerBuiltin >( type );
        }
        case Type::BIT:
        {
            return libstdhl::get< AsBitBuiltin >( type );
        }
        case Type::STRING:
        {
            return libstdhl::get< AsStringBuiltin >( type );
        }
        case Type::FLOATING:
        {
            return libstdhl::get< AsFloatingBuiltin >( type );
        }
        case Type::RATIONAL:
        {
            return libstdhl::get< AsRationalBuiltin >( type );
        }
        case Type::ENUMERATION:
        {
            return libstdhl::get< AsEnumerationBuiltin >( type );
        }
        default:
        {
            throw std::domain_error(
                "could not find a builtin for '" + type->name() + "'" );
        }
    }
}

//------------------------------------------------------------------------------

//
// GeneralBuiltin
//

GeneralBuiltin::GeneralBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 GeneralBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or IsSymbolicBuiltin::classof( obj )
           or AbortBuiltin::classof( obj ) or AssertBuiltin::classof( obj )
           or OutputBuiltin::classof( obj );
}

//
// IsSymbolicBuiltin
//

IsSymbolicBuiltin::IsSymbolicBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

const Annotation IsSymbolicBuiltin::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN,
            } },

        { Type::BOOLEAN,
            {
                Type::INTEGER,
            } },

        { Type::BOOLEAN,
            {
                Type::BIT,
            } },

        { Type::BOOLEAN,
            {
                Type::FLOATING,
            } },

        { Type::BOOLEAN,
            {
                Type::STRING,
            } },

        { Type::BOOLEAN,
            {
                Type::RATIONAL,
            } },

        { Type::BOOLEAN,
            {
                Type::ENUMERATION,
            } },

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return BOOLEAN;
    } );

u1 IsSymbolicBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AbortBuiltin
//

AbortBuiltin::AbortBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

const Annotation AbortBuiltin::info( classid(),
    Annotation::Data{

        { Type::VOID, {} },

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 0 );
        return VOID;
    } );

u1 AbortBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AssertBuiltin
//

AssertBuiltin::AssertBuiltin( const Type::Ptr& type )
: GeneralBuiltin( type, classid() )
{
}

const Annotation AssertBuiltin::info( classid(),
    Annotation::Data{

        { Type::VOID,
            {
                Type::BOOLEAN,
            } },

        { Type::VOID,
            {
                Type::BOOLEAN, Type::STRING,
            } },
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() >= 1 and types.size() <= 2 );
        return VOID;
    } );

u1 AssertBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OutputBuiltin
//

OutputBuiltin::OutputBuiltin( const Type::Ptr& type, const std::string& channel,
    u1 newline, const Value::ID id )
: GeneralBuiltin( type, classid() )
, m_channel( channel )
, m_newline( newline )
{
}

std::string OutputBuiltin::channel( void ) const
{
    return m_channel;
}

u1 OutputBuiltin::newline( void ) const
{
    return m_newline;
}

u1 OutputBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or PrintBuiltin::classof( obj )
           or PrintLnBuiltin::classof( obj );
}

//
// PrintBuiltin
//

PrintBuiltin::PrintBuiltin( const Type::Ptr& type )
: OutputBuiltin( type, "$stdout$", false, classid() )
{
}

const Annotation PrintBuiltin::info( classid(),
    Annotation::Data{

        { Type::VOID,
            {
                Type::STRING,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        const auto& argumentType = types[ 0 ];
        assert( argumentType );

        if( not argumentType->isString() )
        {
            throw TypeArgumentException( "found '" + argumentType->description()
                                             + "', but expects '"
                                             + STRING->description()
                                             + "'",
                0 );
        }

        return VOID;
    } );

u1 PrintBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// PrintLnBuiltin
//

PrintLnBuiltin::PrintLnBuiltin( const Type::Ptr& type )
: OutputBuiltin( type, "$stdout$", true, classid() )
{
}

const Annotation PrintLnBuiltin::info( classid(),
    Annotation::Data{

        { Type::VOID,
            {
                Type::STRING,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        const auto& argumentType = types[ 0 ];
        assert( argumentType );

        if( not argumentType->isString() )
        {
            throw TypeArgumentException( "found '" + argumentType->description()
                                             + "', but expects '"
                                             + STRING->description()
                                             + "'",
                0 );
        }

        return VOID;
    } );

u1 PrintLnBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//------------------------------------------------------------------------------

//
// CastingBuiltin
//

CastingBuiltin::CastingBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 CastingBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AsBooleanBuiltin::classof( obj )
           or AsIntegerBuiltin::classof( obj ) or AsBitBuiltin::classof( obj )
           or AsEnumerationBuiltin::classof( obj )
           or AsStringBuiltin::classof( obj )
           or AsFloatingBuiltin::classof( obj );
}

//
// AsBooleanBuiltin
//

AsBooleanBuiltin::AsBooleanBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsBooleanBuiltin::info( classid(),
    Annotation::Data{

        { Type::BOOLEAN,
            {
                Type::BOOLEAN,
            } },

        { Type::BOOLEAN,
            {
                Type::INTEGER,
            } },

        { Type::BOOLEAN,
            {
                Type::BIT,
            } },

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return BOOLEAN;
    } );

u1 AsBooleanBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsIntegerBuiltin
//

AsIntegerBuiltin::AsIntegerBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsIntegerBuiltin::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::INTEGER,
            } },

        { Type::INTEGER,
            {
                Type::BOOLEAN,
            } },

        { Type::INTEGER,
            {
                Type::FLOATING,
            } },

        { Type::INTEGER,
            {
                Type::BIT,
            } },

        { Type::INTEGER,
            {
                Type::ENUMERATION,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        const auto& arg = types[ 0 ];
        if( arg->isInteger() )
        {
            const auto& integerType = static_cast< const IntegerType& >( *arg );
            assert( not integerType
                            .range() ); // TODO: PPA: handle integer range case!
        }
        return INTEGER;
    } );

u1 AsIntegerBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsBitBuiltin
//

AsBitBuiltin::AsBitBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsBitBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::INTEGER, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BOOLEAN, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::FLOATING, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::ENUMERATION, Type::INTEGER,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( *lhs == *rhs )
        {
            return lhs;
        }
        else
        {
            return nullptr;
        }
    } );

u1 AsBitBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsStringBuiltin
//

AsStringBuiltin::AsStringBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsStringBuiltin::info( classid(),
    Annotation::Data{

        { Type::STRING,
            {
                Type::STRING,
            } },

        { Type::STRING,
            {
                Type::INTEGER,
            } },

        { Type::STRING,
            {
                Type::BOOLEAN,
            } },

        { Type::STRING,
            {
                Type::BIT,
            } },

        { Type::STRING,
            {
                Type::FLOATING,
            } },

        { Type::STRING,
            {
                Type::RATIONAL,
            } },

        { Type::STRING,
            {
                Type::ENUMERATION,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return STRING;
    } );

u1 AsStringBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsFloatingBuiltin
//

AsFloatingBuiltin::AsFloatingBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsFloatingBuiltin::info( classid(),
    Annotation::Data{

        { Type::FLOATING,
            {
                Type::FLOATING,
            } },

        { Type::FLOATING,
            {
                Type::INTEGER,
            } },

        { Type::FLOATING,
            {
                Type::BOOLEAN,
            } },

        { Type::FLOATING,
            {
                Type::BIT,
            } },

        { Type::FLOATING,
            {
                Type::ENUMERATION,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return FLOATING;
    } );

u1 AsFloatingBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsRationalBuiltin
//

AsRationalBuiltin::AsRationalBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsRationalBuiltin::info( classid(),
    Annotation::Data{

        { Type::RATIONAL,
            {
                Type::RATIONAL,
            } }

        // TODO: PPA: add more relations for possible input types!
    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return RATIONAL;
    } );

u1 AsRationalBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AsEnumerationBuiltin
//

AsEnumerationBuiltin::AsEnumerationBuiltin( const Type::Ptr& type )
: CastingBuiltin( type, classid() )
{
}

const Annotation AsEnumerationBuiltin::info( classid(),
    Annotation::Data{

        { Type::ENUMERATION,
            {
                Type::INTEGER,
            } },

        { Type::ENUMERATION,
            {
                Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return nullptr; // TODO: PPA: fetch through values a enumeration kind
                        // hint and return its type!
    } );

u1 AsEnumerationBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// StringifyBuiltin
//

StringifyBuiltin::StringifyBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 StringifyBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or DecBuiltin::classof( obj )
           or HexBuiltin::classof( obj ) or OctBuiltin::classof( obj )
           or BinBuiltin::classof( obj );
}

static const Annotation::Data stringify_builtin_data = {

    { Type::STRING,
        {
            Type::BOOLEAN,
        } },

    { Type::STRING,
        {
            Type::INTEGER,
        } },

    { Type::STRING,
        {
            Type::BIT,
        } },

    { Type::STRING,
        {
            Type::FLOATING,
        } },

    { Type::STRING,
        {
            Type::RATIONAL,
        } },

    { Type::STRING,
        {
            Type::ENUMERATION,
        } }

};

static const auto stringify_builtin_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
    assert( types.size() == 1 );
    return STRING;
};

//
// DecBuiltin
//

DecBuiltin::DecBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

const Annotation DecBuiltin::info(
    classid(), stringify_builtin_data, stringify_builtin_inference );

u1 DecBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// HexBuiltin
//

HexBuiltin::HexBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

const Annotation HexBuiltin::info(
    classid(), stringify_builtin_data, stringify_builtin_inference );

u1 HexBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// OctBuiltin
//

OctBuiltin::OctBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

const Annotation OctBuiltin::info(
    classid(), stringify_builtin_data, stringify_builtin_inference );

u1 OctBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BinBuiltin
//

BinBuiltin::BinBuiltin( const Type::Ptr& type )
: StringifyBuiltin( type, classid() )
{
}

const Annotation BinBuiltin::info(
    classid(), stringify_builtin_data, stringify_builtin_inference );

u1 BinBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

// Math built-ins:

// pow  : Integer * Integer -> Integer
// //     base      exponent
// rand : Integer * Integer -> Integer
// //     start     end

//
// OperatorBuiltin
//

OperatorBuiltin::OperatorBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 OperatorBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or ArithmeticBuiltin::classof( obj )
           or CompareBuiltin::classof( obj );
}

//
// ArithmeticBuiltin
//

ArithmeticBuiltin::ArithmeticBuiltin(
    const Type::Ptr& type, const Value::ID id )
: OperatorBuiltin( type, id )
{
}

static const Annotation::Data arithmetic_builtin_data = {

    { Type::INTEGER,
        {
            Type::INTEGER, Type::INTEGER,
        } },

    { Type::BIT,
        {
            Type::BIT, Type::BIT,
        } }
};

static const auto arithmetic_builtin_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
    assert( types.size() == 2 );
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];
    if( *lhs == *rhs )
    {
        return lhs;
    }
    else
    {
        return nullptr;
    }
};

u1 ArithmeticBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or AdduBuiltin::classof( obj )
           or AddsBuiltin::classof( obj ) or SubuBuiltin::classof( obj )
           or SubsBuiltin::classof( obj ) or MuluBuiltin::classof( obj )
           or MulsBuiltin::classof( obj );
}

//
// AdduBuiltin
//

AdduBuiltin::AdduBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation AdduBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 AdduBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AddsBuiltin
//

AddsBuiltin::AddsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation AddsBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 AddsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubuBuiltin
//

SubuBuiltin::SubuBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation SubuBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 SubuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SubsBuiltin
//

SubsBuiltin::SubsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation SubsBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 SubsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MuluBuiltin
//

MuluBuiltin::MuluBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation MuluBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 MuluBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// MulsBuiltin
//

MulsBuiltin::MulsBuiltin( const Type::Ptr& type )
: ArithmeticBuiltin( type, classid() )
{
}

const Annotation MulsBuiltin::info(
    classid(), arithmetic_builtin_data, arithmetic_builtin_inference );

u1 MulsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// CompareBuiltin
//

CompareBuiltin::CompareBuiltin( const Type::Ptr& type, const Value::ID id )
: OperatorBuiltin( type, id )
{
}

const Annotation::Data compare_builtin_data = {

    { Type::BOOLEAN,
        {
            Type::INTEGER, Type::INTEGER,
        } },

    { Type::BOOLEAN,
        {
            Type::BIT, Type::BIT,
        } },

    { Type::BOOLEAN,
        {
            Type::RATIONAL, Type::RATIONAL,
        } },

    { Type::BOOLEAN,
        {
            Type::STRING, Type::STRING,
        } },

    // TODO: PPA: add more?
};

static const auto compare_builtin_inference
    = []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& ) -> Type::Ptr {
    assert( types.size() == 2 );
    const auto& lhs = types[ 0 ];
    const auto& rhs = types[ 1 ];
    if( *lhs == *rhs )
    {
        return BOOLEAN;
    }
    else
    {
        return nullptr;
    }
};

u1 CompareBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or LesuBuiltin::classof( obj )
           or LessBuiltin::classof( obj ) or LequBuiltin::classof( obj )
           or LeqsBuiltin::classof( obj ) or GreuBuiltin::classof( obj )
           or GresBuiltin::classof( obj ) or GequBuiltin::classof( obj )
           or GeqsBuiltin::classof( obj );
}

//
// LesuBuiltin
//

LesuBuiltin::LesuBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation LesuBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 LesuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LessBuiltin
//

LessBuiltin::LessBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation LessBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 LessBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LequBuiltin
//

LequBuiltin::LequBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation LequBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 LequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// LeqsBuiltin
//

LeqsBuiltin::LeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation LeqsBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 LeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GreuBuiltin
//

GreuBuiltin::GreuBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation GreuBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 GreuBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GresBuiltin
//

GresBuiltin::GresBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation GresBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 GresBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GequBuiltin
//

GequBuiltin::GequBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation GequBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 GequBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// GeqsBuiltin
//

GeqsBuiltin::GeqsBuiltin( const Type::Ptr& type )
: CompareBuiltin( type, classid() )
{
}

const Annotation GeqsBuiltin::info(
    classid(), compare_builtin_data, compare_builtin_inference );

u1 GeqsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// BitBuiltin
//

BitBuiltin::BitBuiltin( const Type::Ptr& type, const Value::ID id )
: Builtin( type, id )
{
}

u1 BitBuiltin::classof( Value const* obj )
{
    return obj->id() == classid() or ZextBuiltin::classof( obj )
           or SextBuiltin::classof( obj ) or TruncBuiltin::classof( obj )
           or ShlBuiltin::classof( obj ) or ShrBuiltin::classof( obj )
           or AshrBuiltin::classof( obj ) or ClzBuiltin::classof( obj )
           or CloBuiltin::classof( obj ) or ClsBuiltin::classof( obj );
}

//
// ZextBuiltin
//

ZextBuiltin::ZextBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation ZextBuiltin::info( classid(),

    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        assert( values.size() == 1 );
        return nullptr; // TODO: PPA: fetch through values a integer constant
                        // which defines the new bitsize
    } );

u1 ZextBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// SextBuiltin
//

SextBuiltin::SextBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation SextBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        assert( values.size() == 1 );
        return nullptr; // TODO: PPA: fetch through values a integer constant
                        // which defines the new bitsize
    } );

u1 SextBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// TruncBuiltin
//

TruncBuiltin::TruncBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation TruncBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        assert( values.size() == 1 );
        return nullptr; // TODO: PPA: fetch through values a integer constant
                        // which defines the new bitsize
    } );

u1 TruncBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ShlBuiltin
//

ShlBuiltin::ShlBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation ShlBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } },
        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBit() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    } );

u1 ShlBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ShrBuiltin
//

ShrBuiltin::ShrBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation ShrBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBit() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    } );

u1 ShrBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// AshrBuiltin
//

AshrBuiltin::AshrBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation AshrBuiltin::info( classid(),
    Annotation::Data{

        { Type::BIT,
            {
                Type::BIT, Type::INTEGER,
            } },

        { Type::BIT,
            {
                Type::BIT, Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 2 );
        const auto& lhs = types[ 0 ];
        const auto& rhs = types[ 1 ];
        if( rhs->isBit() )
        {
            if( *lhs != *rhs )
            {
                return nullptr;
            }
        }
        return lhs;
    } );

u1 AshrBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ClzBuiltin
//

ClzBuiltin::ClzBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation ClzBuiltin::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return INTEGER;
    } );

u1 ClzBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// CloBuiltin
//

CloBuiltin::CloBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation CloBuiltin::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return INTEGER;
    } );

u1 CloBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

//
// ClsBuiltin
//

ClsBuiltin::ClsBuiltin( const Type::Ptr& type )
: BitBuiltin( type, classid() )
{
}

const Annotation ClsBuiltin::info( classid(),
    Annotation::Data{

        { Type::INTEGER,
            {
                Type::BIT,
            } }

    },
    []( const std::vector< Type::Ptr >& types,
        const std::vector< Value::Ptr >& values ) -> Type::Ptr {
        assert( types.size() == 1 );
        return INTEGER;
    } );

u1 ClsBuiltin::classof( Value const* obj )
{
    return obj->id() == classid();
}

// List built-ins:

//             NTH_BUILTIN,
//             CONS_BUILTIN,
//             APP_BUILTIN,
//             LEN_BUILTIN,
//             TAIL_BUILTIN,
//             PEEK_BUILTIN,

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
