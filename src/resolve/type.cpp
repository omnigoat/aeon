#include <aeon/resolve.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>
#include <aeon/parsing/xpi.hpp>
#include <aeon/lexing/id.hpp>

namespace parsing = aeon::parsing;

typedef parsing::parseme_t::id_t id;

auto aeon::resolve::is_atomic_expr(parsing::parseme_ptr const& x) -> bool
{
	switch (x->id())
	{
		case id::integer_literal:
		case id::identifier:
			return true;
	}

	return false;
}

// return type-definition
auto aeon::resolve::type_of(parsing::parseme_ptr const& x) -> parsing::parseme_ptr const&
{
	ATMA_ASSERT(x);

	switch (x->id())
	{
		case id::integer_literal:
		{
			using namespace parsing;
			//auto tds = parseme_ptr_refs_t();
			//upwards_enclosing_copy_rawptr(tds, x->siblings(), [&x](parseme_ptr const& y) {
				//return y->id() == ID::type_definition &&
					//marshall::type_definition::name(y) == "@int32";
			//});
			return parsing::upwards_enclosing_find(x->siblings(), [](parseme_ptr const& y) {
				return y->id() == ID::type_definition &&
					marshall::type_definition::name(y)->text() == "@int32";
			});
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
		case id::intrinsic_int_mul:
		case id::intrinsic_int_div:
		{
			// get to the root
			auto root = parsing::find_root(x);
			
			// find type-definition
			for (auto const& i : *root->siblings())
				if (i->id() == parsing::ID::type_definition)
					if (marshall::type_definition::definition(i)->id() == parsing::ID::intrinsic_type_int)
						if (marshall::type_definition::name(i)->text() == x->text())
							return i;

			return parsing::null_parseme_ptr;
		}
	}

	return x;
}

auto aeon::resolve::typename_to_definition(parsing::parseme_ptr const& x) -> parsing::parseme_ptr const&
{
	using namespace parsing;

	ATMA_ASSERT(x->id() == id::type_name);

	// get all locations which may house our type-definition
	auto type_definitions = parseme_ptr_refs_t();
	upwards_enclosing_copy_rawptr(type_definitions, x->siblings(), id_equals(ID::type_definition));

	for (auto const* y : type_definitions)
		if (marshall::type_definition::name(*y)->text() == x->text())
			return *y;

	return parsing::null_parseme_ptr;
}