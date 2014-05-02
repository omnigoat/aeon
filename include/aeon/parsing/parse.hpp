#pragma once
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/children.hpp>
#include <aeon/parsing/context.hpp>

#include <cstdint>
#include <ostream>
//=====================================================================
namespace aeon { namespace parsing {
	
	namespace detail
	{
		struct context_t;

		auto module(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto type_name(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto parameters(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function_body(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto statement(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto expression(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto multiplicative_expression(children_t&, context_t&) -> bool;
		auto additive_expression(children_t&, context_t&) -> bool;
		auto logical_expression(children_t&, context_t&) -> bool;
		auto function_call_expression(children_t&, context_t&) -> bool;
	}

	auto parse(children_t&, lexing::lexemes_t const&) -> void;

} }
