//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_SEMANTICS_RESOLVE_HPP
#define AEON_SEMANTICS_RESOLVE_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <aeon/parsing/children.hpp>
//=====================================================================
namespace aeon {
namespace semantics {
namespace resolve {
//=====================================================================
	
	parsing::parseme_ptr const& type_of(parsing::parseme_ptr const&);
	
	//
	parsing::parseme_ptr const& identifier_to_definition(parsing::parseme_ptr const&);

//=====================================================================
} // namespace resolve
} // namespace semantics
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

