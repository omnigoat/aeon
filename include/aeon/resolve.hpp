//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_RESOLVE_HPP
#define AEON_RESOLVE_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace resolve {
//=====================================================================
	
	parsing::parseme_ptr const& type_of(parsing::parseme_ptr const&);
	
	//
	parsing::parseme_ptr const& identifier_to_definition(parsing::parseme_ptr const&);

	//
	parsing::parseme_ptr const& typename_to_definition(parsing::parseme_ptr const&);

	//
	parsing::parseme_ptr const& expression_to_ast(parsing::parseme_ptr const&);

	parsing::parseme_ptr function_from_function_call(parsing::parseme_ptr const&);

#if 0

	struct resolver_t
	{
		auto type_definition_of(parsing::parseme_ptr const&) -> parsing::parseme_ptr;
		auto variable_definition_of(parsing::parseme_ptr const&) -> parsing::parseme_ptr;

	private:
		// map from x to its resultant type-definition
		std::map<parsing::parseme_ptr, parsing::parseme_ptr> type_definitions_;
	};

#endif

//=====================================================================
} // namespace resolve
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

