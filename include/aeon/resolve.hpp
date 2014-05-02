#pragma once
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon { namespace resolve {
	
	// identifies atomic expressions
	//  - anything duplicitous: identifiers, integer-literals, etc
	auto is_atomic_expr(parsing::parseme_ptr const&) -> bool;


	auto is_function_forceinline(parsing::parseme_ptr const&) -> bool;


	// returns a type-definition from things:
	//  - identifiers find their definition and corresponding type
	//  - typenames lookup the corresponding type
	//  - expressions have their types inferred
	auto type_of(parsing::parseme_ptr const&) -> parsing::parseme_ptr const&;
	

	auto identifier_to_definition(parsing::parseme_ptr const&) -> parsing::parseme_ptr const&;
	auto typename_to_definition(parsing::parseme_ptr const&) -> parsing::parseme_ptr const&;
	auto function_from_function_call(parsing::parseme_ptr const&) -> parsing::parseme_ptr const&;


	

} }
