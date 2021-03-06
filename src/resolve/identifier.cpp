#include <aeon/resolve.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>

namespace parsing = aeon::parsing;

typedef parsing::parseme_t::id_t id;

namespace
{
	auto is_matching_function_pattern(parsing::parseme_ptr const& lhs, parsing::parseme_ptr const& rhs) -> bool
	{
		// this doesn't take into account types of parameters
		return lhs->text() == rhs->text();
	}
}

auto aeon::resolve::identifier_to_definition(parsing::parseme_ptr const& x) -> parsing::parseme_ptr const&
{
	// get all locations which may house our variable definition
	parsing::parsemes_t locations;
	parsing::copy_direct_upwards_if(
		std::back_inserter(locations), x->parent(),
		[](parsing::parseme_ptr const& y) { return y->id() == id::function; });

	parsing::copy_breadth_first_upwards_if(
		std::back_inserter(locations), *x->siblings(), parsing::id_equals_t(id::function));

	// loop through until we find one
	for (auto const& y : locations) {
		// try parameters
		parsing::parseme_ptr const& params = marshall::function::parameter_list(y);
		for (auto const& param : params->children()) {
			if (marshall::parameter::identifier(param)->text() == x->text())
				return param;
		}
	}

	return parsing::null_parseme_ptr;
}

auto aeon::resolve::function_from_function_call(parsing::parseme_ptr const& x) -> parsing::parseme_ptr const&
{
	ATMA_ASSERT(x->id() == parsing::ID::function_call);

	return parsing::upwards_enclosing_find(x->siblings(), [&x](parsing::parseme_ptr const& f) {
		return f->id() == id::function
			&& is_matching_function_pattern(marshall::function::name(f), marshall::function_call::name(x));
	});
}

auto aeon::resolve::is_function_forceinline(parsing::parseme_ptr const& x) -> bool
{
	if (auto const& attrs = marshall::function::attributes(x)) {
		return parsing::find(attrs, parsing::id_equals_t(parsing::ID::attribute_forceinline)) != nullptr;
	}

	return false;
}
