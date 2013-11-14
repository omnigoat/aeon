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

//=====================================================================
} // namespace resolve
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

