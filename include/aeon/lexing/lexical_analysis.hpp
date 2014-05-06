#pragma once
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
//=====================================================================
namespace aeon { namespace lexing {

	struct lexical_analysis_t
	{
		lexical_analysis_t(char const* begin, char const* end);

		auto lexemes() const -> lexemes_t const&;
		//auto errors() const 

		//auto make_synthetic_lexeme()

	private:
		auto run() -> void;
		auto character_literal() -> void;
		auto string_literal() -> void;
		auto number_literal() -> void;
		auto punctuation() -> void;
		auto whitespace_tokeniser() -> void;
		auto identifier() -> void;
		auto block() -> void;

		auto stream_valid() -> bool;
		auto stream_increment() -> void;
		auto cv() const -> char;

		auto state_reset_whitespace() -> void;
		auto state_nonwhitespace_token() -> void;

	private:
		// incoming stream
		char const* begin_;
		char const* end_;
		char const* current_;
		position_t position_;

		// state
		lexemes_t lexemes_;
		uint tabs_, previous_tabs_;
		bool empty_line_;
	};

} }
