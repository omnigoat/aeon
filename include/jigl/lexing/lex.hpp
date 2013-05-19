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

	struct state_t
	{
		state_t();

		auto push_back(lexeme_t::id_t, char const* begin, char const* end, position_t const&) -> void;
		auto non_whitespace_token() -> void;
		auto reset_whitespace() -> void;
		auto increment_tabs() -> void;

	private:
		uint32_t tabs_, previous_tabs_;
		bool empty_line_;
		lexemes_t lexemes_;
	};


	auto lex(state_t&, stream_t&) -> void;
	

//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

