//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_MARSHALL_HPP
#define AEON_PARSING_MARSHALL_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
namespace marshall {
//=====================================================================
	
	namespace function {
		parseme_ptr const& name(parseme_ptr const& x) { return x->children()[0]; }
		parseme_ptr const& parameter_list(parseme_ptr const& x) { return x->children()[1]; }
		parseme_ptr const& return_type(parseme_ptr const& x) { return x->children()[2]; }
		parseme_ptr const& body(parseme_ptr const& x) { return x->children()[3]; }
	}

//=====================================================================
} // namespace marshall
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

