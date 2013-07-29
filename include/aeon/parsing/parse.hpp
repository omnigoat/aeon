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
#include <aeon/parsing/parsemes.hpp>
#include <aeon/parsing/context.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t;

		auto module(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto parameters(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function_body(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto statement(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto expression(parsemes_t&, lexing::lexemes_t const&, context_t&) -> bool;
	}


	auto parse(parsemes_t&, lexing::lexemes_t const&) -> void;

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

