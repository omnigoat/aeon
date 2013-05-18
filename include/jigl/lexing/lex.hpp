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
		auto marked_position() const -> position_t const&;
		auto increment() -> void;
		auto mark() -> char const*;
		auto reset(char const*) -> void;
		
	private:
		char const* begin_;
		char const* end_;
		char const* current_;
		char const* mark_;

		position_t position_;
		position_t marked_position_;
	};

	auto lex(lexemes_t&, stream_t&) -> void;
	
	#define JIGL_LEXING_IDS() \
		X(identifier, "", 0) \
		X(integer_literal, "", 0) \
		X(real_literal, "", 0)  \
		X(punctuation, "", 0) \
		X(lower_bound, "", 0) \
		X(if_keyword, "if", 2) \
		X(else_keyword, "else", 4) \
		X(type_keyword, "type", 4) \
		X(function_keyword, "function", 8) \
		X(upper_bound, "", 0)

	#define X(name, string, strlen) name,
	enum class ID
	{
		JIGL_LEXING_IDS()
	};
	#undef X


//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

