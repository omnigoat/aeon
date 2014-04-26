#include <aeon/resolve.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>
#include <aeon/parsing/xpi.hpp>
#include <aeon/lexing/id.hpp>

namespace parsing = aeon::parsing;

typedef parsing::parseme_t::id_t id;

// return type-definition
parsing::parseme_ptr const& aeon::resolve::type_of(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x);

	switch (x->id())
	{
		case id::integer_literal:
		{
			// get to the root
			auto root = parsing::find_root(x);

			// find type-definition
			for (auto const& i : root->children())
				if (i->id() == parsing::ID::type_definition)
					if (marshall::type_definition::definition(i)->id() == parsing::ID::intrinsic_type_int)
						return i;
		}

		case id::return_statement:
			return type_of(marshall::unary_expr::child(x));

		case id::addition_expr:
			return type_of(marshall::binary_expr::lhs(x));

		case id::identifier:
			return type_of(identifier_to_definition(x));

		case id::parameter:
			return type_of(marshall::parameter::type_name(x));

		case id::type_name:
			return typename_to_definition(x);

		case id::function_call: {
			auto f = resolve::function_from_function_call(x);
			return type_of(marshall::function::return_type(f));
		}

		case id::intrinsic_int_add:
		case id::intrinsic_int_sub:
		{
			// get to the root
			auto root = parsing::find_root(x);
			
			// find type-definition
			for (auto const& i : root->children())
				if (i->id() == parsing::ID::type_definition)
					if (marshall::type_definition::definition(i)->id() == parsing::ID::intrinsic_type_int)
						if (marshall::type_definition::intrinsic_info(i)->text() == x->text())
							return i;

			return parsing::null_parseme_ptr;
		}
	}

	return x;
}

parsing::parseme_ptr const& aeon::resolve::typename_to_definition(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x->id() == id::type_name);

	
	// get all locations which may house our type-definition
	parsing::parsemes_t locations;
	parsing::copy_direct_upwards_if(
		std::back_inserter(locations), x->parent(),
		[](parsing::parseme_ptr const& y) { return y->id() == id::root; });
	

	// loop through until we find one
	parsing::parseme_ptr root;
	for (auto const& y : locations) {
		// try root
		if (y->id() == id::root) {
			root = y;
			for (auto const& z : y->children()) {
				if (z->id() == id::type_definition) {
					if ( marshall::type_definition::name(z)->text() == x->text() )
						return z;
				}
			}
		}
	}

	return parsing::null_parseme_ptr;
}