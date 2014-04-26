#include <aeon/generation/genesis.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>

using namespace aeon;
using namespace generation;
using aeon::generation::genesis_t;
using parsing::ID;

genesis_t::genesis_t()
{
}

auto genesis_t::expr_id(parsing::parseme_ptr const& x) const -> uint
{
	auto i = tmp_.find(x);
	if (i == tmp_.end())
		return 0;

	return i->second;
}

auto genesis_t::push_function(parsing::parseme_ptr const& x) -> void
{
	fn_ = x;
}

auto genesis_t::pop_function() -> void
{
	// lol!
}

auto genesis_t::mark_expr(parsing::parseme_ptr const& x) -> void
{
	tmp_.insert({x, ++tmpcount_[fn_]});
}

auto aeon::generation::analyse::module(genesis_t& genesis, parsing::parseme_ptr const& x) -> void
{
	for (auto const& f : x->children()) {
		if (f->id() == ID::function)
			function(genesis, f);
	}
}

auto aeon::generation::analyse::function(genesis_t& genesis, parsing::parseme_ptr const& x) -> void
{
	auto const& body = marshall::function::body(x);

	genesis.push_function(x);
	for (auto const& s : body->children())
		statement(genesis, s);
	genesis.pop_function();
}

auto aeon::generation::analyse::statement(genesis_t& genesis, parsing::parseme_ptr const& x) -> void
{
	switch (x->id())
	{
		case ID::return_statement:
			expression(genesis, marshall::unary_expr::child(x));
			break;
	}
}

auto aeon::generation::analyse::expression(genesis_t& genesis, parsing::parseme_ptr const& x) -> void
{
	switch (x->id())
	{
		case ID::function_call:
			for (auto const& arg : marshall::function_call::argument_list(x)->children())
				expression(genesis, arg);

			genesis.mark_expr(x);
			break;

		case ID::intrinsic_int_add:
		case ID::intrinsic_int_sub:
			genesis.mark_expr(x);
			break;
	}
}

