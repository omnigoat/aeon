#pragma once

#include <aeon/lexing/lexical_analysis.hpp>
#include <aeon/parsing/parseme.hpp>

namespace aeon { namespace parsing {

	struct syntactic_analysis_t
	{
		syntactic_analysis_t(lexing::lexical_analysis_t&);

		auto parsemes() -> children_t&;

	private:
		// incoming lexical analysis
		auto lxa_peek() -> lexing::lexeme_t const*;
		auto lxa_mk_if(parsing::ID, lexing::ID) -> parseme_ptr;
		auto lxa_mk_if(parsing::ID, lexing::ID, char const*, uint) -> parseme_ptr;
		auto lxa_skip(lexing::ID) -> bool;
		auto lxa_skip(lexing::ID, char const*, uint) -> bool;

		// parsing
		auto parse_module(children_t&) -> bool;
		auto parse_function(children_t&) -> bool;
		auto parse_parameters(children_t&) -> bool;
		auto parse_typename(children_t&) -> bool;
		auto parse_function_body(children_t&) -> bool;
		auto parse_statement(children_t&) -> bool;
		auto parse_expr(children_t&) -> bool;
		auto parse_expr_logical(children_t&) -> bool;
		auto parse_expr_additive(children_t&) -> bool;
		auto parse_expr_multiplicative(children_t&) -> bool;
		auto parse_expr_function_call(children_t&) -> bool;

		// prelude generation
		auto generate_prelude() -> bool;
		auto generate_int_type(uint bitsize) -> bool;
		auto generate_void_type() -> bool;
		auto generate_bool_type() -> bool;

	private:
		lexing::lexical_analysis_t& lxa_;
		lexing::lexemes_t::const_iterator lxa_iter_;

		children_t parsemes_;
	};

} }
