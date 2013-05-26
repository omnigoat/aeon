//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_LEXING_STATE_HPP
#define JIGL_LEXING_LEXING_STATE_HPP
//=====================================================================
#include <jigl/lexing/lexeme.hpp>
#include <jigl/lexing/lexemes.hpp>
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================
	
	struct state_t
	{
		state_t();

		auto lexemes() const -> lexemes_t const& { return lexemes_; }
		
		auto push_back(lexeme_t::id_t, char const* begin, char const* end, position_t const&) -> void;
		auto non_whitespace_token() -> void;
		auto reset_whitespace() -> void;
		auto increment_tabs() -> void;

	private:
		uint32_t tabs_, previous_tabs_;
		bool empty_line_;
		lexemes_t lexemes_;
	};

//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

