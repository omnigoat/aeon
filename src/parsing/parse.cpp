#include <aeon/parsing/parse.hpp>

using namespace aeon::parsing;

auto aeon::parsing::parse(parsemes_t& parsemes, lexing::lexemes_t const& lexemes) -> void
{
	detail::module(parsemes, lexemes);
}

auto aeon::parsing::detail::function(parsemes_t& parsemes, lexing::lexemes_t const& lexemes) -> bool
{
	while (function(parsemes, lexemes))
		;
}

auto aeon::parsing::detail::function(parsemes_t& parsemes, lexing::lexemes_t const& lexemes) -> bool
{
	
	return true;
}