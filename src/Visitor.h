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

#ifndef _LIB_CASMIR_VISITOR_H_
#define _LIB_CASMIR_VISITOR_H_

#include "CasmIR.h"

namespace libcasm_ir
{
    class Specification;

    class Agent;
    class Function;
    class Derived;
    class Rule;
    class Builtin;

    class Enumeration;

    class ParallelBlock;
    class SequentialBlock;

    class TrivialStatement;
    class BranchStatement;

    class SkipInstruction;

    class ForkInstruction;
    class MergeInstruction;

    class LookupInstruction;
    class UpdateInstruction;

    class LocalInstruction;
    class LocationInstruction;

    class AssertInstruction;
    class SelectInstruction;
    class SymbolicInstruction;

    class CallInstruction;

    class AddInstruction;
    class SubInstruction;
    class MulInstruction;
    class ModInstruction;
    class DivInstruction;

    class AndInstruction;
    class OrInstruction;
    class XorInstruction;
    class NotInstruction;

    class EquInstruction;
    class NeqInstruction;
    class LthInstruction;
    class LeqInstruction;
    class GthInstruction;
    class GeqInstruction;

    class VoidConstant;
    class RuleReferenceConstant;
    class BooleanConstant;
    class IntegerConstant;
    class BitConstant;
    class StringConstant;
    class RationalConstant;
    class FloatingConstant;
    class EnumerationConstant;
    class AgentConstant;

    enum Traversal : u8
    {
        PREORDER = 0,
        POSTORDER
    };

    class Visitor : public CasmIR
    {
      public:
        virtual ~Visitor( void ) = default;

        //
        // General
        //

        virtual void visit( Specification& value ) = 0;
        virtual void visit( Agent& value ) = 0;
        virtual void visit( Function& value ) = 0;
        virtual void visit( Derived& value ) = 0;
        virtual void visit( Rule& value ) = 0;
        virtual void visit( Builtin& value ) = 0;

        virtual void visit( Enumeration& value ) = 0;

        virtual void visit( ParallelBlock& value ) = 0;
        virtual void visit( SequentialBlock& value ) = 0;

        virtual void visit( TrivialStatement& value ) = 0;
        virtual void visit( BranchStatement& value ) = 0;

        //
        // Instructions
        //

        virtual void visit( SkipInstruction& value ) = 0;

        virtual void visit( ForkInstruction& value ) = 0;
        virtual void visit( MergeInstruction& value ) = 0;

        virtual void visit( LookupInstruction& value ) = 0;
        virtual void visit( UpdateInstruction& value ) = 0;

        virtual void visit( LocalInstruction& value ) = 0;
        virtual void visit( LocationInstruction& value ) = 0;
        virtual void visit( CallInstruction& value ) = 0;

        virtual void visit( AssertInstruction& value ) = 0;
        virtual void visit( SelectInstruction& value ) = 0;
        virtual void visit( SymbolicInstruction& value ) = 0;

        virtual void visit( AddInstruction& value ) = 0;
        virtual void visit( SubInstruction& value ) = 0;
        virtual void visit( MulInstruction& value ) = 0;
        virtual void visit( ModInstruction& value ) = 0;
        virtual void visit( DivInstruction& value ) = 0;

        virtual void visit( AndInstruction& value ) = 0;
        virtual void visit( XorInstruction& value ) = 0;
        virtual void visit( OrInstruction& value ) = 0;
        virtual void visit( NotInstruction& value ) = 0;

        virtual void visit( EquInstruction& value ) = 0;
        virtual void visit( NeqInstruction& value ) = 0;
        virtual void visit( LthInstruction& value ) = 0;
        virtual void visit( LeqInstruction& value ) = 0;
        virtual void visit( GthInstruction& value ) = 0;
        virtual void visit( GeqInstruction& value ) = 0;

        //
        // Constants
        //

        virtual void visit( VoidConstant& value ) = 0;
        virtual void visit( RuleReferenceConstant& value ) = 0;
        virtual void visit( BooleanConstant& value ) = 0;
        virtual void visit( IntegerConstant& value ) = 0;
        virtual void visit( BitConstant& value ) = 0;
        virtual void visit( StringConstant& value ) = 0;
        virtual void visit( FloatingConstant& value ) = 0;
        virtual void visit( RationalConstant& value ) = 0;
        virtual void visit( EnumerationConstant& value ) = 0;
        virtual void visit( AgentConstant& value ) = 0;
    };

    class RecursiveVisitor : public Visitor
    {
      public:
        //
        // Visitor General
        //

        void visit( Specification& value ) override;
        void visit( Agent& value ) override;
        void visit( Function& value ) override;
        void visit( Derived& value ) override;
        void visit( Rule& value ) override;
        void visit( Builtin& value ) override;

        void visit( Enumeration& value ) override;

        void visit( ParallelBlock& value ) override;
        void visit( SequentialBlock& value ) override;

        void visit( TrivialStatement& value ) override;
        void visit( BranchStatement& value ) override;

        //
        // Visitor Instructions
        //

        virtual void visit( SkipInstruction& value ) override = 0;

        virtual void visit( ForkInstruction& value ) override = 0;
        virtual void visit( MergeInstruction& value ) override = 0;

        virtual void visit( LookupInstruction& value ) override = 0;
        virtual void visit( UpdateInstruction& value ) override = 0;

        virtual void visit( LocalInstruction& value ) override = 0;
        virtual void visit( LocationInstruction& value ) override = 0;
        virtual void visit( CallInstruction& value ) override = 0;

        virtual void visit( AssertInstruction& value ) override = 0;
        virtual void visit( SelectInstruction& value ) override = 0;
        virtual void visit( SymbolicInstruction& value ) override = 0;

        virtual void visit( AddInstruction& value ) override = 0;
        virtual void visit( SubInstruction& value ) override = 0;
        virtual void visit( MulInstruction& value ) override = 0;
        virtual void visit( ModInstruction& value ) override = 0;
        virtual void visit( DivInstruction& value ) override = 0;

        virtual void visit( AndInstruction& value ) override = 0;
        virtual void visit( XorInstruction& value ) override = 0;
        virtual void visit( OrInstruction& value ) override = 0;
        virtual void visit( NotInstruction& value ) override = 0;

        virtual void visit( EquInstruction& value ) override = 0;
        virtual void visit( NeqInstruction& value ) override = 0;
        virtual void visit( LthInstruction& value ) override = 0;
        virtual void visit( LeqInstruction& value ) override = 0;
        virtual void visit( GthInstruction& value ) override = 0;
        virtual void visit( GeqInstruction& value ) override = 0;

        //
        // Visitor Constants
        //

        virtual void visit( VoidConstant& value ) override = 0;
        virtual void visit( RuleReferenceConstant& value ) override = 0;
        virtual void visit( BooleanConstant& value ) override = 0;
        virtual void visit( IntegerConstant& value ) override = 0;
        virtual void visit( BitConstant& value ) override = 0;
        virtual void visit( StringConstant& value ) override = 0;
        virtual void visit( FloatingConstant& value ) override = 0;
        virtual void visit( RationalConstant& value ) override = 0;
        virtual void visit( EnumerationConstant& value ) override = 0;
        virtual void visit( AgentConstant& value ) override = 0;
    };

    class TraversalVisitor : public Visitor
    {
      public:
        TraversalVisitor(
            const Traversal order, std::function< void( Value& ) > callback );

        Traversal order( void ) const;

        std::function< void( Value& ) > callback( void ) const;

      private:
        Traversal m_order;

        std::function< void( Value& ) > m_callback;

      public:
        //
        // Visitor General
        //

        void visit( Specification& value ) override;
        void visit( Agent& value ) override;
        void visit( Function& value ) override;
        void visit( Derived& value ) override;
        void visit( Rule& value ) override;
        void visit( Builtin& value ) override;

        void visit( Enumeration& value ) override;

        void visit( ParallelBlock& value ) override;
        void visit( SequentialBlock& value ) override;

        void visit( TrivialStatement& value ) override;
        void visit( BranchStatement& value ) override;

        //
        // Visitor Instructions
        //

        void visit( SkipInstruction& value ) override;

        void visit( ForkInstruction& value ) override;
        void visit( MergeInstruction& value ) override;

        void visit( LookupInstruction& value ) override;
        void visit( UpdateInstruction& value ) override;

        void visit( LocalInstruction& value ) override;
        void visit( LocationInstruction& value ) override;
        void visit( CallInstruction& value ) override;

        void visit( AssertInstruction& value ) override;
        void visit( SelectInstruction& value ) override;
        void visit( SymbolicInstruction& value ) override;

        void visit( AddInstruction& value ) override;
        void visit( SubInstruction& value ) override;
        void visit( MulInstruction& value ) override;
        void visit( ModInstruction& value ) override;
        void visit( DivInstruction& value ) override;

        void visit( AndInstruction& value ) override;
        void visit( XorInstruction& value ) override;
        void visit( OrInstruction& value ) override;
        void visit( NotInstruction& value ) override;

        void visit( EquInstruction& value ) override;
        void visit( NeqInstruction& value ) override;
        void visit( LthInstruction& value ) override;
        void visit( LeqInstruction& value ) override;
        void visit( GthInstruction& value ) override;
        void visit( GeqInstruction& value ) override;

        //
        // Visitor Constants
        //

        void visit( VoidConstant& value ) override;
        void visit( RuleReferenceConstant& value ) override;
        void visit( BooleanConstant& value ) override;
        void visit( IntegerConstant& value ) override;
        void visit( BitConstant& value ) override;
        void visit( StringConstant& value ) override;
        void visit( FloatingConstant& value ) override;
        void visit( RationalConstant& value ) override;
        void visit( EnumerationConstant& value ) override;
        void visit( AgentConstant& value ) override;
    };
}

#endif // _LIB_CASMIR_VISITOR_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
