#pragma once

#include <aeon/lexing/lexical_analysis.hpp>
#include <aeon/parsing/parseme.hpp>

namespace aeon { namespace parsing {

	struct syntactic_analysis_t
	{
		syntactic_analysis_t(lexing::lexical_analysis_t&);

		auto parsemes() -> children_t&;

	private:
		// incoming lexical analysis
		auto lxa_peek() -> lexing::lexeme_t const*;
		auto lxa_match_mk(parsing::ID, lexing::ID) -> parseme_ptr;
		auto lxa_skip(lexing::ID) -> bool;
		auto lxa_skip(lexing::ID, char const*, uint) -> bool;

		//auto module()
	private:
		lexing::lexical_analysis_t& lxa_;
		lexing::lexemes_t::const_iterator lxa_iter_;

		children_t parsemes_;
	};

} }
