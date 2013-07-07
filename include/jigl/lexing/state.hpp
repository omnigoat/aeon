//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_LEXING_LEXING_STATE_HPP
#define AEON_LEXING_LEXING_STATE_HPP
//=====================================================================
#include <aeon/lexing/lexeme.hpp>
#include <aeon/lexing/lexemes.hpp>
//=====================================================================
namespace aeon {
namespace lexing {
//=====================================================================
	
	struct state_t
	{
		state_t(lexemes_t& lexemes);

		auto lexemes() const -> lexemes_t const& { return lexemes_; }
		
		auto push_back(lexeme_t::id_t, char const* begin, char const* end, position_t const&, multichannel_t const&) -> void;
		auto non_whitespace_token() -> void;
		auto reset_whitespace() -> void;
		auto increment_tabs() -> void;

	private:
		uint32_t tabs_, previous_tabs_;
		bool empty_line_;
		lexemes_t& lexemes_;
	};

//=====================================================================
} // namespace lexing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

