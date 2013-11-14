#include <aeon/resolve.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>

namespace parsing = aeon::parsing;

typedef parsing::parseme_t::id_t id;



parsing::parseme_ptr const& aeon::resolve::identifier_to_definition(parsing::parseme_ptr const& x)
{
	// get all locations which may house our variable definition
	parsing::parsemes_t locations;
	parsing::copy_direct_upwards_if(
		std::back_inserter(locations), x->parent(),
		[](parsing::parseme_ptr const& y) { return y->id() == id::function; });

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

