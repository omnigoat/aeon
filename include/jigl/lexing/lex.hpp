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
	
	struct stream_t
	{
		stream_t(char const* begin, char const* end);

		auto begin() const -> char const*;
		auto end() const -> char const*;
		auto current() const -> char const*;
		auto cv() const -> char;
		auto valid() const -> bool;
		auto position() const -> position_t const&;

		auto increment() -> void;
		auto reset(char const*) -> void;

	private:
		char const* begin_;
		char const* end_;
		char const* current_;

		position_t position_;
	};

	auto lex(lexemes_t&, stream_t&) -> void;
	
	namespace ID
	{
		uint32_t const identifier = 1;
		uint32_t const integer_literal = 10;
		uint32_t const real_literal = 11;
	}



//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

