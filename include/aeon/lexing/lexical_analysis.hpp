#pragma once
//=====================================================================
#include <aeon/lexing/lexemes.hpp>

#include <atma/string.hpp>

#include <list>
//=====================================================================
namespace aeon { namespace lexing {

	struct lexical_analysis_t
	{
		lexical_analysis_t(char const* begin, char const* end);

		auto lexemes() const -> lexemes_t const&;
		
		auto make_aux(ID, position_t const&, atma::utf8_string_range_t const&) -> lexeme_t const*;
		auto make_aux(ID, position_t const&, atma::utf8_string_t const&) -> lexeme_t const*;
		auto make_aux(ID, position_t const&, multichannel_t const&, atma::utf8_string_range_t const&) -> lexeme_t const*;
		auto make_aux(ID, position_t const&, multichannel_t const&, atma::utf8_string_t const&) -> lexeme_t const*;

	private:
		auto run() -> void;
		auto character_literal() -> void;
		auto string_literal() -> void;
		auto number_literal() -> void;
		auto punctuation() -> void;
		auto whitespace() -> void;
		auto identifier() -> void;
		auto block() -> void;

		auto stream_valid() const -> bool;
		auto stream_cv() const -> char;
		auto stream_increment() -> void;

		auto state_reset_whitespace() -> void;
		auto state_nonwhitespace_token() -> void;

	private:
		char const* begin_;
		char const* end_;
		char const* current_;
		position_t position_;

		std::vector<std::pair<char const*, char const*>> lines_;
		uint tabs_, previous_tabs_;
		bool empty_line_;
		bool consumed_newline_;

		lexemes_t lexemes_;
		std::list<atma::string> aux_texts_;
		std::list<lexeme_t> aux_lexemes_;
	};

} }
