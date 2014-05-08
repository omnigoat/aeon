#include <aeon/parsing/syntactic_analysis.hpp>

using namespace aeon;
using aeon::lexing::lexical_analysis_t;
using aeon::parsing::syntactic_analysis_t;

syntactic_analysis_t::syntactic_analysis_t(lexical_analysis_t& lexical_analysis)
	: lexical_analysis_(lexical_analysis)
{
}

auto syntactic_analysis_t::parsemes() -> parsing::children_t&
{
	return parsemes_;
}

#if 0
auto lxa_peek() -> lexing::lexeme_t const*;
auto lxa_match_mk(parsing::ID, lexing::ID) -> parseme_ptr;
auto lxa_skip(lexing::ID) -> bool;
auto lxa_skip(lexing::ID, char const*, uint) -> bool;
#endif
