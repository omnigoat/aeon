#include <aeon/semantics/resolve.hpp>
#include <aeon/parsing/ape.hpp>
#include <aeon/lexing/id.hpp>

namespace parsing = aeon::parsing;
//namespace marshall = aeon::parsing::marshall;
namespace ape = aeon::parsing::ape;

typedef parsing::parseme_t::id_t id;

char const* intrinsic_text = "int16int32";

aeon::lexing::position_t intrinsic_position(0, 0);
aeon::lexing::lexeme_t int16_lexeme(aeon::lexing::ID::intrinsic_type_int16, intrinsic_text, intrinsic_text + 5, intrinsic_position);
aeon::lexing::lexeme_t int32_lexeme(aeon::lexing::ID::intrinsic_type_int32, intrinsic_text + 5, intrinsic_text + 10, intrinsic_position);

parsing::parseme_ptr const& aeon::semantics::resolve::typename_to_definition(parsing::parseme_ptr const& x)
{
	ATMA_ASSERT(x->id() == id::type_name);

	//
	parsing::children_t xs;

	ape::insert_into(xs, (
		ape::make(id::type_definition) [
			ape::make(id::identifier, int16_lexeme),
			ape::make(id::intrinsic_type_int16)
		],

		ape::make(id::type_definition) [
			ape::make(id::identifier, int32_lexeme),
			ape::make(id::intrinsic_type_int32)
		]
	));

#if 0
	// get all locations which may house our type-definition
	parsing::parsemes_t locations;
	parsing::copy_direct_upwards_if(
		std::back_inserter(locations), x->parent(),
		[](parsing::parseme_ptr const& y) { return y->id() == id::function; });
#endif
	
	return x;
}