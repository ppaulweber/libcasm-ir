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

#include "Annotation.h"

using namespace libcasm_ir;

// static std::unordered_map< std::string, Annotation* > str2obj = {};
// static std::unordered_map< u8, Annotation* > id2obj = {};

Annotation::Annotation( const Value::ID id, const Data& info )
: m_id( id )
, m_info( info )
{
    assert( m_info.size() > 0 );

    for( auto relation : m_info )
    {
        Type::ID rt = relation.result;

        m_type_set.emplace_back( Set() );
        m_type_set.back().emplace( rt );

        std::string key;

        m_argument_sizes.emplace( relation.argument.size() );

        for( u32 i = 0; i < relation.argument.size(); i++ )
        {
            Type::ID at = relation.argument[ i ];
            assert( at != libcasm_ir::Type::RELATION );

            m_type_set.emplace_back( Set() );
            m_type_set.back().emplace( at );
            key += std::to_string( at ) + ";";
        }

        assert( m_relation_to_type.find( key ) == m_relation_to_type.end()
                and " result type of relation already exists!" );
        m_relation_to_type[ key ] = rt;
    }

    auto result_str = str2obj().emplace( Value::token( id ), this );
    assert( result_str.second );

    auto result_id = id2obj().emplace( id, this );
    assert( result_id.second );
}

Value::ID Annotation::id( void ) const
{
    return m_id;
}

const Annotation::Set& Annotation::resultTypes( void ) const
{
    return m_type_set[ 0 ];
}

const Annotation::Set& Annotation::argumentTypes( u8 pos ) const
{
    assert( pos < ( m_type_set.size() - 1 ) );

    return m_type_set[ pos + 1 ];
}

const std::set< std::size_t >& Annotation::argumentSizes( void ) const
{
    return m_argument_sizes;
}

Type::ID Annotation::resultTypeForRelation(
    const std::vector< const Type* > arguments ) const
{
    std::string key;

    for( auto arg : arguments )
    {
        libcasm_ir::Type::ID at = arg->result().id();
        assert( at != libcasm_ir::Type::RELATION );

        key += std::to_string( at ) + ";";
    }

    auto result = m_relation_to_type.find( key );
    if( result != m_relation_to_type.end() )
    {
        return result->second;
    }

    assert( !" no result type found for requested relation! " );
    return Type::_BOTTOM_;
}

const Annotation& Annotation::find( const std::string& token )
{
    const auto& mapping = str2obj();
    auto result = mapping.find( token );
    if( result == mapping.end() )
    {
        throw std::domain_error( "no annotation defined for '" + token + "'" );
    }
    return *result->second;
}

const Annotation& Annotation::find( const Value::ID id )
{
    const auto& mapping = id2obj();
    auto result = mapping.find( id );
    if( result == mapping.end() )
    {
        throw std::domain_error( "no annotation defined for Value::ID '"
                                 + std::to_string( id )
                                 + "' (aka. '"
                                 + Value::token( id )
                                 + "')" );
    }
    return *result->second;
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
