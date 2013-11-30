#include <aeon/resolve.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>
#include <aeon/parsing/xpi.hpp>
#include <aeon/lexing/id.hpp>

namespace parsing = aeon::parsing;

typedef parsing::parseme_t::id_t id;

parsing::parseme_ptr const& aeon::resolve::type_of(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x);

	switch (x->id())
	{
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
	ATMA_ASSERT(root);


	// intrinsic integers are constructed on the fly, as all are acceptable
	
	if ( aeon::parsing::is_intrinsic_int_typename(x) )//atma::strncmp(x->text(), "@int", 4) == 0 && x->text() >= '0' && x->text()[4] <= '9')
	{
		// SERIOUSLY, do we ever need to provide an upper bound to atoi?
		auto bitsize = std::atoi(x->text().begin() + 4);

		namespace xpi = aeon::parsing::xpi;
		typedef parsing::parseme_t::id_t parsid;

		// if this is the first time an integer of this width is encountered, insert it into the prelude
		xpi::insert_into(root->children(), root->children().begin(),
			xpi::make(parsid::type_definition) [
				xpi::make(parsid::identifier, lexing::ID::identifier, x->text()),
				xpi::make(parsid::intrinsic_type_int),
				xpi::make(parsid::intrinsic_bitsize, lexing::make_synthetic_lexeme(lexing::ID::integer_literal, x->text().begin() + 4, x->text().end(), x->position(), lexing::basic))
			]
		);

		xpi::insert_into(root->children(), root->children().begin(),
			xpi::make(parsid::function) [
				xpi::make(parsid::function_pattern) [
					xpi::make(parsid::placeholder),
					xpi::make(parsid::identifier, "+"),
					xpi::make(parsid::placeholder)
				],

				xpi::make(parsid::parameter_list)[
					xpi::make(parsid::parameter)[
						xpi::make(parsid::identifier, "lhs"),
						xpi::make(parsid::type_name, x->text())
					],
					xpi::make(parsid::parameter)[
						xpi::make(parsid::identifier, "rhs"),
						xpi::make(parsid::type_name, x->text())
					]
				],

				xpi::make(parsid::type_name, x->text()),

				xpi::make(parsid::block) [
					xpi::make(parsid::return_statement)[
						xpi::make(parsid::intrinsic_int_add)[
							xpi::make(parsid::identifier, "lhs"),
							xpi::make(parsid::identifier, "rhs")
						]
					]
				]
			]
		);

		// this should now succeed, and certainly not recurse!
		return typename_to_definition(x);
	}

	return x;
}