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

#include "Statement.h"

#include "Constant.h"
#include "Instruction.h"

using namespace libcasm_ir;

Statement::Statement(
    const char* name, Type* type, ExecutionSemanticsBlock* scope, Value::ID id )
: Block( name, type, id )
, scope( scope )
{
}

ExecutionSemanticsBlock* Statement::getScope( void ) const
{
    return scope;
}

const std::vector< Value* >& Statement::getInstructions( void ) const
{
    return instructions;
}

void Statement::add( Value* instruction )
{
    // printf( "%s: %p\n", __FUNCTION__, instruction );
    assert( instruction and Value::isa< Instruction >( instruction ) );

    // if( Value::isa< Constant >( instruction ) )
    // {
    //     // printf( "%s: %p --> Constant, omitted\n", __FUNCTION__,
    //     instruction
    //     // );
    //     return;
    // }

    // if( Value::isa< Instruction >( instruction ) )
    {
        // printf( "%s: %p --> Instruction\n", __FUNCTION__, instruction );
        static_cast< Instruction* >( instruction )->setStatement( this );
    }

    instructions.push_back( instruction );
    // printf( "[Stmt] add: %p\n", instruction );
}

void Statement::addBlock( ExecutionSemanticsBlock* block )
{
    assert( block );

    if( Value::isa< TrivialStatement >( this ) )
    {
        assert(
            !" trivial statements are not allowed to have inside blocks! " );
    }
    // else if( Value::isa< ForallStatement >( this ) )
    // {
    //     assert( blocks.size() < 1 );
    // }

    blocks.push_back( block );

    if( block->getParent() == 0 )
    {
        block->setParent( this );
    }

    assert( block->getParent() == this && " inconsistent block nesting! " );
}

const std::vector< ExecutionSemanticsBlock* >& Statement::getBlocks(
    void ) const
{
    if( Value::isa< TrivialStatement >( this ) )
    {
        assert( !" trivial statements do not contain inside blocks! " );
    }

    return blocks;
}

void Statement::dump( void ) const
{
    for( auto instr : instructions )
    {
        static_cast< Value* >( instr )->dump();
    }
}

bool Statement::classof( Value const* obj )
{
    return obj->getValueID() == classid() or TrivialStatement::classof( obj )
           or BranchStatement::classof( obj );
}

TrivialStatement::TrivialStatement( ExecutionSemanticsBlock* scope )
: Statement( ".statement", 0, scope, Value::TRIVIAL_STATEMENT )
{
}

void TrivialStatement::dump( void ) const
{
    printf( "[TrStm] %p", this );
    if( scope )
    {
        printf( " @ %lu (%p)", scope->getPseudoState(), scope );
    }
    printf( "\n" );

    ( (Statement*)this )->dump();
}

bool TrivialStatement::classof( Value const* obj )
{
    return obj->getValueID() == classid();
}

BranchStatement::BranchStatement( ExecutionSemanticsBlock* scope )
: Statement( ".branch", 0, scope, Value::BRANCH_STATEMENT )
{
}

void BranchStatement::dump( void ) const
{
    printf( "[BranchStatement] %p\n", this );

    ( (Statement*)this )->dump();

    // TODO: here the branches etc.
}

bool BranchStatement::classof( Value const* obj )
{
    return obj->getValueID() == classid();
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
