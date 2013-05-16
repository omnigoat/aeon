//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_LEX_HPP
#define JIGL_LEXING_LEX_HPP
//=====================================================================
#include <jigl/lexing/lexeme.hpp>
#include <jigl/lexing/lexemes.hpp>
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================
	
	auto lex(lexemes_t&, char const* begin, char const* end) -> void;
	
	
//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

