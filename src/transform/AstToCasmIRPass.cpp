//  
//  Copyright (c) 2015 Philipp Paulweber
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                https://github.com/ppaulweber/casm-ir
//  
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal with the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//  
//  * Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimers.
//  
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimers in the 
//    documentation and/or other materials provided with the distribution.
//  
//  * Neither the names of the copyright holders, nor the names of its 
//    contributors may be used to endorse or promote products derived from 
//    this Software without specific prior written permission.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//  WITH THE SOFTWARE.
//  

#include "AstToCasmIRPass.h"



char libcasm_ir::AstToCasmIRPass::id = 0;

static libpass::PassRegistration< libcasm_ir::AstToCasmIRPass > PASS
( "AST to CASM IR"
, "internal: translates the AST to the CASM intermeditate representation"
, 0
, 0
);


extern Driver* global_driver;

static libcasm_ir::Type* getType( Type* type )
{
	assert( type && "not initialized type used" );
	
	switch( type->t )
	{
	    case TypeType::SELF: // Agent!
			return new libcasm_ir::Type( libcasm_ir::Type::ID::AGENT );
	    case TypeType::RULEREF:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::RULE_POINTER );
	    case TypeType::BOOLEAN:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::BOOLEAN );
	    case TypeType::INTEGER:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::INTEGER );
	    case TypeType::BIT:
			assert( type->bitsize > 0
				 && type->bitsize <= 256
				 && "invalid Bit size for Bit type" );
			return new libcasm_ir::Type( libcasm_ir::Type::ID::BIT, type->bitsize );
	    case TypeType::STRING:
			return new libcasm_ir::Type( libcasm_ir::Type::ID::STRING );
	    default:
			std::cerr << type->to_str() << "\n";
			assert( 0 && "not implemented function atom identifier type" );
			return 0;
	}
}

bool libcasm_ir::AstToCasmIRPass::run( libpass::PassResult& pr )
{
	AstNode* node = (AstNode*)pr.getResult< TypeCheckPass >();
	
	AstWalker< AstToCasmIRPass, bool > walker( *this );
	
	walker.suppress_calls = true;
	walker.walk_specification( node );

	// PPA: this could be extracted to a 'update and check consistency' Value function?
    Value::SymbolTable& symbols = *Value::getSymbols();
	for( auto value : symbols[".rulepointer"] )
	{
		assert( Value::isa< RulePointerConstant >( value ) );
		((RulePointerConstant*)value)->resolve();
	}
	
	
	casm_frontend_destroy();
	return true;
	
	std::string input = "";
	
	while( input.compare( "q" ) != 0 )
	{
		std::getline( cin, input );

		if( input.compare( "#" ) == 0 )
		{
			assert( 0 );
			return false;
		}
		else
		{
			for( auto r : (*Value::getSymbols())[ input.c_str() ] )
			{
				printf( "dumping '%s':\n", input.c_str() );
				r->dump();
			}
		}
	}
	
    return true;
}

#define VISIT printf( "===--- %s:%i: %s: %p: %s\n", \
__FILE__, __LINE__, __FUNCTION__, node, node->to_str().c_str() )

#define FIXME											  \
	printf( "+++ FIXME +++: '%s:%i' in '%s'\n",			  \
			__FILE__, __LINE__, __FUNCTION__ );			  \
	fflush( stdout );									  \
	assert( 0 );



void libcasm_ir::AstToCasmIRPass::visit_specification( AstNode* node )
{
	// VISIT;
	// printf( "CASM specification\n" );
	// //FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_init( UnaryNode* node )
{
	// VISIT;
	// printf( "init %s\n", global_driver->init_name.c_str() );
	RulePointerConstant* ir_init =
		RulePointerConstant::create( global_driver->init_name.c_str() );
	
	Agent* ir_agent = new Agent();
	assert( ir_agent );
	
	ir_agent->setInitRulePointer( ir_init );
}

void libcasm_ir::AstToCasmIRPass::visit_body_elements( AstNode* node )
{
	VISIT;
	//FIXME;TODO ASAP!!!
}

void libcasm_ir::AstToCasmIRPass::visit_function_def
( FunctionDefNode* node
, const std::vector<std::pair<T, T>>& inits
)
{
	VISIT;

	string x;
	for( auto& a : node->sym->arguments_ )
	{
		x.append( a->to_str() );
	}
	
	printf( "%s, %lu: %s -> %s, \n"
			, node->sym->name.c_str()
			, node->sym->id
			, x.c_str()
			, node->sym->return_type_->to_str().c_str()
		);

	
	Type* ftype = getType( node->sym->return_type_ );
	for( auto argument : node->sym->arguments_ )
	{
		ftype->addParameter( getType( argument ) );
	}
		
	Function* ir_function =
		new Function( node->sym->name.c_str(), ftype );
	assert( ir_function );
}

    
void libcasm_ir::AstToCasmIRPass::visit_derived_def_pre( FunctionDefNode* node )
{
	VISIT;
}
	
void libcasm_ir::AstToCasmIRPass::visit_derived_def( FunctionDefNode* node, T expr )
{
	VISIT;
	string x;
	for( auto& a : node->sym->arguments_ )
	{
		x.append( a->to_str() );
	}
	printf( "%s, %lu: %s -> %s, \n", node->sym->name.c_str(), node->sym->id, x.c_str(), node->sym->return_type_->to_str().c_str() );
	
	assert( node->sym->type == Symbol::SymbolType::DERIVED );
	
	Value* ir_expr = lookup< Value >( node->sym->derived  );
	assert( ir_expr );
	
	TrivialStatement* ir_stmt = new TrivialStatement();
	assert( ir_stmt );
	ir_stmt->add( ir_expr );

	
	Type* ftype = getType( node->sym->return_type_ );
	for( auto argument : node->sym->arguments_ )
	{
		ftype->addParameter( getType( argument ) );
	}
	
	Derived* ir_derived =
		new Derived( node->sym->name.c_str(), ftype );
	assert( ir_derived );
	ir_derived->setContext( ir_stmt );
}

void libcasm_ir::AstToCasmIRPass::visit_rule( RuleNode* node )
{
	printf( "\n\n" );
	VISIT;
	string x;
	for( auto& a : node->arguments )
	{
		x.append( a->to_str() );
	}
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	Rule* ir_rule = new Rule( node->name.c_str() );
    assert( ir_rule );
    ast2casmir[ node ]        = ir_rule;
	ast2parent[ node->child_ ] = node;
}

void libcasm_ir::AstToCasmIRPass::visit_parblock( UnaryNode* node )
{
	VISIT;
	printf( "{ }\n" );
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	ParallelBlock* ir_scope = new ParallelBlock();
	assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
	ast2parent[ node->child_ ] = node;
	
	Value* parent = lookupParent< Value >( node );
	assert( parent );
	
	if( Value::isa< Rule >( parent ) )
	{
		((Rule*)parent)->setContext( ir_scope );
	}
	else if( Value::isa< ExecutionSemanticsBlock >( parent ) )
	{
		((ExecutionSemanticsBlock*)parent)->add( ir_scope );
	}
	else if( Value::isa< Statement >( parent ) )
	{
		((Statement*)parent)->getScope()->add( ir_scope );
	}
	else
	{
		assert( 0 );
	}
}

void libcasm_ir::AstToCasmIRPass::visit_seqblock( UnaryNode* node )
{	
	VISIT;
	printf( "{| |}\n" );
	printf( "%p -> %p\n", node, node->child_ );
	
	assert( node );
	
	SequentialBlock* ir_scope = new SequentialBlock();
	assert( ir_scope );
    ast2casmir[ node ] = ir_scope;
	ast2parent[ node->child_ ] = node;

	Value* parent = lookupParent< Value >( node );
	assert( parent );
	
    if( Value::isa< ExecutionSemanticsBlock >( parent ) )
	{
		((ExecutionSemanticsBlock*)parent)->add( ir_scope );
	}
	else if( Value::isa< Statement >( parent ) )
	{
		((Statement*)parent)->getScope()->add( ir_scope );
	}
	else
	{
		assert( 0 );
	}
}

void libcasm_ir::AstToCasmIRPass::visit_statements( AstListNode* node )
{
    VISIT;
    printf( "...\n" );
	for( AstNode *s : node->nodes )
	{
		printf( "%p -> %p\n", node, s );
	}
	
	assert( node );
	
	auto parent = ast2parent.find( node );
	assert( parent != ast2parent.end() );
	
	for( AstNode *s : node->nodes )
	{
		ast2parent[ s ] = parent->second;
	}
}
	
void libcasm_ir::AstToCasmIRPass::visit_forall_pre( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_forall_post( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_iterate( AstNode* node )
{
	VISIT;
	FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_update( UpdateNode* node, T func, T expr )
{
	VISIT;
    printf( "%p -> %p\n", node, node->func );
	printf( "%p -> %p\n", node, node->expr_ );
	
	ExecutionSemanticsBlock* ir_scope =
		lookupParent< ExecutionSemanticsBlock >( node );
	assert( ir_scope );
	
	TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
	assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
	ast2parent[ node->func ]   = node;
	ast2parent[ node->expr_ ]  = node;
	
	Value* ir_lhs = lookup< Value >( node->func  );
	Value* ir_rhs = lookup< Value >( node->expr_ );
	
	assert( ir_lhs );
	assert( ir_rhs );

	assert( Value::isa< LookupInstruction >( ir_lhs ) );
	
	ir_stmt->add( new UpdateInstruction(
					  ((Instruction*)ir_lhs)->getValue( 0 ), ir_rhs ) );
}

void libcasm_ir::AstToCasmIRPass::visit_update_dumps( UpdateNode* node, T func, T expr )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_update_subrange( UpdateNode* node, T func, T expr)
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_call_pre( CallNode* node )
{
    //VISIT;
}
	
void libcasm_ir::AstToCasmIRPass::visit_call_pre( CallNode* node, T expr )
{
    //VISIT;
}

void libcasm_ir::AstToCasmIRPass::visit_call( CallNode* node, std::vector< T >& args )
{
	VISIT;

	printf( "call: %s\n", node->rule_name.c_str() );

	FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_call_post( CallNode* node )
{
	//VISIT;
}

void libcasm_ir::AstToCasmIRPass::visit_print( PrintNode* node, std::vector< T >& args )
{
	VISIT;
	
	ExecutionSemanticsBlock* ir_scope =
		lookupParent< ExecutionSemanticsBlock >( node );
	assert( ir_scope );
	
	TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
	assert( ir_stmt );
    ast2casmir[ node ]         = ir_stmt;
	// ast2parent[ node->func ]   = node;
	// ast2parent[ node->expr_ ]  = node;
    
	PrintInstruction* ir_print = new PrintInstruction();
    assert( ir_print );
	for( auto a : node->atoms )
	{
		Value* instr = lookup< Value >( a );
		ir_print->add( instr );
	}
	ir_print->add( StringConstant::create( (const char*)"\n" ) );
	
	ir_stmt->add( ir_print );
}
    
void libcasm_ir::AstToCasmIRPass::visit_diedie( DiedieNode* node, T msg )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_impossible( AstNode* node )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_assert( UnaryNode* node, T expr )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_assure( UnaryNode* node, T expr )
{
	VISIT;
	FIXME;
}
    
void libcasm_ir::AstToCasmIRPass::visit_let( LetNode* node, T var )
{
	VISIT;
	
	ast2parent[ node->expr ] = node;
	ast2parent[ node->stmt ] = node;
	
	Value* ir_expr = lookup< Value >( node->expr );
	assert( ir_expr );
	
	Value* ir_ident =
		Identifier::create( getType( &node->type_ ), node->identifier.c_str() );
	assert( ir_ident );
	
	Value* ir_let = new LetInstruction( ir_ident, ir_expr );
	assert( ir_let );
    
	ExecutionSemanticsBlock* ir_scope = 0;
	Value* parent = lookupParent< Value >( node );
	assert( parent );
	if( Value::isa< ExecutionSemanticsBlock >( parent ) )
	{
		ir_scope = (ExecutionSemanticsBlock*)parent;
		if( ir_scope->isParallel() )
		{
			ExecutionSemanticsBlock* ir_scope_top = ir_scope;
		    ir_scope = new SequentialBlock();
			assert( ir_scope );
			ir_scope->setParent( ir_scope_top );
			ir_scope_top->add( ir_scope );
		}		
	}
	else if( Value::isa< Statement >( parent ) )
	{
		ir_scope = ((Statement*)parent)->getScope();
		printf( "up for %p\n", parent );
	}
	else
	{
		assert( 0 );
	}
	
	TrivialStatement* ir_stmt = new TrivialStatement( ir_scope );
	assert( ir_stmt );
	ast2casmir[ node ] = ir_stmt;
	
	//ir_scope->add( ir_stmt );
	ir_stmt->add( ir_let );
}

void libcasm_ir::AstToCasmIRPass::visit_let_post( LetNode* node )
{
	VISIT;	
	Identifier::forgetSymbol( node->identifier.c_str() );
}

void libcasm_ir::AstToCasmIRPass::visit_push( PushNode* node, T expr, T atom )
{
	VISIT;
	FIXME;
}
	
void libcasm_ir::AstToCasmIRPass::visit_pop( PopNode* node )
{
	VISIT;
	FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_ifthenelse( IfThenElseNode* node, T cond )
{
	VISIT;
	printf( "%p -> %p\n",      node, node->condition_ );
	printf( "%p -> %p | %p\n", node, node->then_, node->else_ );

	ExecutionSemanticsBlock* ir_scope =
		lookupParent< ExecutionSemanticsBlock >( node );
	assert( ir_scope );
	
	BranchStatement* ir_stmt = new BranchStatement( ir_scope );
	assert( ir_stmt );
	
	Value* ir_cond = lookup< Value >( node->condition_ );
	assert( ir_cond );
	assert
	(  Value::isa< Instruction >( ir_cond )
	or Value::isa< BooleanConstant >( ir_cond )
	);
	
	Value* ir_case_true = new ParallelBlock( ir_scope );
	assert( ir_case_true );
	ast2casmir[ node ]             = ir_case_true;
	ast2parent[ node->then_ ]      = node;
	
	Value* ir_case_false = new ParallelBlock( ir_scope );
	assert( ir_case_false );
	ast2casmir[ node->condition_ ] = ir_case_false;
	ast2parent[ node->else_ ]      = node->condition_;
	
	ir_stmt->add( new BranchInstruction( ir_cond, ir_case_true, ir_case_false ) );
	ir_stmt->addBlock( ir_case_true  );
	ir_stmt->addBlock( ir_case_false );
}

void libcasm_ir::AstToCasmIRPass::visit_case_pre( CaseNode* node, T val )
{
	VISIT;
	printf( "%p -> %p\n",      node, node->expr );
	
	ExecutionSemanticsBlock* ir_scope =
		lookupParent< ExecutionSemanticsBlock >( node );
	assert( ir_scope );
	
	BranchStatement* ir_stmt = new BranchStatement( ir_scope );
	assert( ir_stmt );
	
	
	FIXME;
}

void libcasm_ir::AstToCasmIRPass::visit_case( CaseNode* node, T val, const std::vector< T >& case_labels )
{
	VISIT;
	FIXME;
}

T libcasm_ir::AstToCasmIRPass::visit_expression( Expression* node, T lhs, T rhs )
{
	VISIT;
	printf( "%s, %p, %p\n", operator_to_str( node->op ).c_str(), node->left_, node->right_ );
	
	Value* ir_lhs = lookup< Value >( node->left_  );
	Value* ir_rhs = lookup< Value >( node->right_ );
	assert( ir_lhs );
	assert( ir_rhs );
	
	Value* ir_expr = 0;
	switch( node->op )
	{
	    case ExpressionOperation::ADD:
			ir_expr = new AddInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::SUB:
			ir_expr = new SubInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::MUL:
			ir_expr = new MulInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::DIV:
			ir_expr = new DivInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::RAT_DIV:
			ir_expr = new RivInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::MOD:
			ir_expr = new ModInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::EQ:
			ir_expr = new EquInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::NEQ:
			ir_expr = new NeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::LESSER:
			ir_expr = new LthInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::LESSEREQ:
			ir_expr = new LeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::GREATER:
			ir_expr = new GthInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::GREATEREQ:
			ir_expr = new GeqInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::OR:
			ir_expr = new OrInstruction(  ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::XOR:
			ir_expr = new XorInstruction( ir_lhs, ir_rhs );
			break;
		case ExpressionOperation::AND:
			ir_expr = new AndInstruction( ir_lhs, ir_rhs );
			break;
	    default:
			assert( 0 && "unsupported expression operation" );
			FIXME;
	}
	
	assert( ir_expr );
	ast2casmir[ node ] = ir_expr;
	return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_expression_single( Expression* node, T val )
{
	VISIT;
	printf( "%s, %p\n", operator_to_str( node->op ).c_str(), node->left_ );
	
	Value* ir_lhs = lookup< Value >( node->left_  );
	assert( ir_lhs );
	
	Value* ir_expr = 0;
	switch( node->op )
	{
		case ExpressionOperation::NOT:
			ir_expr = new NotInstruction( ir_lhs );
			break;
	    default:
			assert( 0 && "unsupported expression operation" );
			FIXME;
	}
	
	assert( ir_expr );
	ast2casmir[ node ] = ir_expr;
	return 0;
}
    
T libcasm_ir::AstToCasmIRPass::visit_function_atom( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
	printf( "%s, %p", node->name.c_str(), node );
	if( node->arguments )
	{
		for( auto a : *(node->arguments) )
		{
			printf( ", %p", a );
		}
	}
	printf( "\n" );
	
	if( node->symbol_type == FunctionAtom::SymbolType::PARAMETER )
	{
		Value* ir_ident = Identifier::create( getType( &node->type_ ), node->name.c_str() );
		assert( ir_ident );		
		ast2casmir[ node ] = ir_ident;
		return 0;
	}
	
	Type* ty_ident = 0;
	switch( node->symbol_type )
	{
	    case FunctionAtom::SymbolType::FUNCTION:
		{
			assert( node->symbol );
			ty_ident = getType( node->symbol->return_type_ );
			for( auto argument : node->symbol->arguments_ )
			{
				ty_ident->addParameter( getType( argument ) );
			}
			break;
		}
	    default:
			assert( 0 && "not implemented function atom identifier symbol type" );
	}
	
	Value* ir_ident = Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
	
	LocationInstruction* ir_loc	= new LocationInstruction( ir_ident );
    assert( ir_loc );
	if( node->arguments )
	{
		for( auto a : *(node->arguments) )
		{
			Value* instr = lookup< Value >( a );
			ir_loc->add( instr );		
		}
    }
	
	LookupInstruction* ir_lup = new LookupInstruction( ir_loc );
	assert( ir_lup );
    ast2casmir[ node ] = ir_lup;
	
	return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_function_atom_subrange( FunctionAtom* node, T args[], uint16_t argc	)
{
	VISIT;
	FIXME;
	return 0;
}

void libcasm_ir::AstToCasmIRPass::visit_derived_function_atom_pre( FunctionAtom* node, T args[], uint16_t argc )
{
	VISIT;
}

T libcasm_ir::AstToCasmIRPass::visit_derived_function_atom( FunctionAtom* node, T expr )
{
	VISIT;
	string x;
	printf( "derived: %s, %s\n"
			, node->name.c_str()
		    , x.c_str()
	    );

	assert( node->symbol );
	assert( node->symbol_type == FunctionAtom::SymbolType::DERIVED );
	
	Type* ty_ident = getType( node->symbol->return_type_ );
	for( auto argument : node->symbol->arguments_ )
	{
		ty_ident->addParameter( getType( argument ) );
	}
	
	Value* ir_ident = Identifier::create( ty_ident, node->name.c_str() );
    assert( ir_ident );
	
	CallInstruction* ir_call = new CallInstruction( ir_ident );
    assert( ir_call );
    ast2casmir[ node ] = ir_call;
    if( node->arguments )
    {
        for( auto a : *(node->arguments) )
        {
            Value* instr = lookup< Value >( a );
            ir_call->add( instr );		
        }
    }
	
	return 0;
}



T libcasm_ir::AstToCasmIRPass::visit_undef_atom( UndefAtom* node )
{
	VISIT;
	printf( "undef\n" );
	
	Value* ir_const = 0;
    
	switch( node->type_.t )
	{
	    case TypeType::BOOLEAN:
			ir_const = BooleanConstant::create(); break;
	    case TypeType::INTEGER:
			ir_const = IntegerConstant::create(); break;
	    case TypeType::RULEREF:
			ir_const = RulePointerConstant::create(); break;
	    default:
			assert( 0 && "unimplemented undef constant!" );
	}
	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_boolean_atom( BooleanAtom* node )
{
	VISIT;
	printf( "%u\n", node->value );
	
	BooleanConstant* ir_const
		= BooleanConstant::create( (Type::Boolean)node->value );
	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;

	return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_int_atom( IntegerAtom* node )
{
	VISIT;
	printf( "%lu\n", node->val_	);
	
	IntegerConstant* ir_const
		= IntegerConstant::create( (Type::Integer)node->val_ );	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}
	
T libcasm_ir::AstToCasmIRPass::visit_float_atom( FloatAtom* node )
{
	VISIT;
	FIXME;
	return 0;
}
	
T libcasm_ir::AstToCasmIRPass::visit_rational_atom( RationalAtom* node )
{
	VISIT;
	FIXME;
	return 0;
}
	
T libcasm_ir::AstToCasmIRPass::visit_string_atom( StringAtom* node )
{
	VISIT;
	
	Value* ir_const	= StringConstant::create( node->string.c_str() );	
	assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	printf( "%s\n", node->string.c_str() );
	return 0;
}

	
T libcasm_ir::AstToCasmIRPass::visit_self_atom( SelfAtom* node )
{
	VISIT;

	Value* ir_const	= AgentConstant::create( 0 );
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	return 0;
}

T libcasm_ir::AstToCasmIRPass::visit_rule_atom( RuleAtom* node )
{
	VISIT;

	Value* ir_const = RulePointerConstant::create( node->name.c_str() );
    assert( ir_const );
    ast2casmir[ node ] = ir_const;
	
	printf( "rule: @%s\n", node->name.c_str() );
	
	return 0;
}
	
T libcasm_ir::AstToCasmIRPass::visit_list_atom( ListAtom* node, std::vector< T > &args )
{
	VISIT; 
	FIXME;
	return 0;
}
	
T libcasm_ir::AstToCasmIRPass::visit_number_range_atom( NumberRangeAtom* node )
{
	VISIT; 
	FIXME;
	return 0; 
}
	
T libcasm_ir::AstToCasmIRPass::visit_builtin_atom( BuiltinAtom* node, T args[], uint16_t argc ) 
{
	VISIT;
	FIXME;
	return 0;
}



template<class C>
C* libcasm_ir::AstToCasmIRPass::lookupParent( AstNode* node )
{
	//printf( "%s: ??? %p\n", __FUNCTION__, node );
		
	auto result = ast2parent.find( node );
	if( result != ast2parent.end() )
	{
		//printf( "%s: %p, %p\n", __FUNCTION__, result->first, result->second );
		return lookup< C >( result->second );
	}
		
	return 0;
}

template<class C>
C* libcasm_ir::AstToCasmIRPass::lookup( AstNode* node )
{
	//printf( "%s: %p\n", __FUNCTION__, node );
		
	auto result = ast2casmir.find( node );
	if( result != ast2casmir.end() )
	{
		//printf( "%s: %p\n", __FUNCTION__, result->second );
		if( Value::isa< C >( result->second ) )
		{
			return static_cast< C* >( result->second );
		}
		else
		{
			assert( 0 );
		}
	}
		
	return 0;
}


//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  

