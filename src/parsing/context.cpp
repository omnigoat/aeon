#include <aeon/parsing/context.hpp>

using aeon::parsing::detail::context_t;

auto context_t::context_t(lexing::lexemes_t::const_iterator const& begin)
	: begin_(begin)
{
}