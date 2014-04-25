#include <aeon/generation/genesis.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>

using namespace aeon;
using namespace generation;
using aeon::generation::genesis_t;
using parsing::ID;

auto genesis_t::module(parsing::parseme_ptr const& x) -> void
{
	for (auto const& f : x->children()) {
		if (f->id() == ID::function)
			function(f);
	}
}

auto genesis_t::function(parsing::parseme_ptr const& x) -> void
{
	auto const& body = marshall::function::body(x);

	for (auto const& s : body->children())
		statement(s);
}

auto genesis_t::statement(parsing::parseme_ptr const& x) -> void
{
	switch (x->id())
	{
		case ID::return_statement:
			expression(marshall::unary_expr::child(x));
			break;
	}
}

auto genesis_t::expression(parsing::parseme_ptr const& x) -> void
{
	switch (x->id())
	{
		case ID::addition_expr:
			break;
	}
}

