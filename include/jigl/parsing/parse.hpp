//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_PARSING_PARSEME_HPP
#define JIGL_PARSING_PARSEME_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <jigl/lexing/lexemes.hpp>
#include <jigl/parsing/parsemes.hpp>
//=====================================================================
namespace jigl {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t
		{
			parsemes_t root;
		};

		void module(parsemes_t&, lexing::lexemes_t const&);
	}


	auto parse(parsemes_t&, lexing::lexemes_t const&) -> void;

//=====================================================================
} // namespace parsing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

