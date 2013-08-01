#include <aeon/semantics/resolve.hpp>
#include <aeon/parsing/marshall.hpp>
#include <aeon/parsing/algorithm.hpp>

namespace parsing = aeon::parsing;
namespace marshall = aeon::parsing::marshall;

typedef parsing::parseme_t::id_t id;

parsing::parseme_ptr const& aeon::semantics::resolve::type_of(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x);

	switch (x->id())
	{
		case id::return_statement:
			return type_of( marshall::unary_expr::child(x) );

		case id::addition_expr:
			return type_of( marshall::binary_expr::lhs(x) );

		case id::identifier:
			return type_of( identifier_to_definition(x) );

		case id::parameter:
			return type_of( marshall::parameter::type_name(x) );
	}

	return x;
}

parsing::parseme_ptr const& aeon::semantics::resolve::identifier_to_definition(parsing::parseme_ptr const& x)
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
			if ( marshall::parameter::identifier(param)->text() == x->text() )
				return param;
		}
	}

	return parsing::null_parseme_ptr;
}

