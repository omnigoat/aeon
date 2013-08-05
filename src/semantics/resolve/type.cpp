#include <aeon/semantics/resolve.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/parsing/marshall.hpp>

namespace parsing = aeon::parsing;
namespace marshall = aeon::parsing::marshall;

typedef parsing::parseme_t::id_t id;



parsing::parseme_ptr const& aeon::semantics::resolve::typename_to_definition(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x->id() == id::type_name);


	// get all locations which may house our type-definition
	parsing::parsemes_t locations;
	parsing::copy_direct_upwards_if(
		std::back_inserter(locations), x->parent(),
		[](parsing::parseme_ptr const& y) { return y->id() == id::root; });
	

	// loop through until we find one
	for (auto const& y : locations) {
		// try root
		if (y->id() == id::root) {
			for (auto const& z : y->children()) {
				if (z->id() == id::type_definition) {
					if ( marshall::type_definition::name(z)->text() == x->text() )
						return marshall::type_definition::definition(z);
				}
			}
		}
	}


	return x;
}