//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_PARSE_HPP
#define AEON_PARSING_PARSE_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/children.hpp>
#include <aeon/parsing/context.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t;

		auto module(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto parameters(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function_body(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto statement(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto expression(children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto additive_expression(children_t&, context_t&) -> bool;
	}


	auto parse(children_t&, lexing::lexemes_t const&) -> void;

	auto mixfix_resolution(children_t&) -> void;

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

