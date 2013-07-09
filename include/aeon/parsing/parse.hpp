//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_PARSEME_HPP
#define AEON_PARSING_PARSEME_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/parsemes.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t
		{
			parsemes_t root;
		};

		auto module(parsemes_t&, lexing::lexemes_t const&) -> bool;
		auto function(parsemes_t&, lexing::lexemes_t const&) -> bool;
	}


	auto parse(parsemes_t&, lexing::lexemes_t const&) -> void;

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

