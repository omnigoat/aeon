//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_MARSHALL_HPP
#define AEON_MARSHALL_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace marshall {
//=====================================================================
	
	namespace type_definition {
		using parsing::parseme_ptr;

		inline parseme_ptr const& name(parseme_ptr const& x) { return x->children()[0]; }
		inline parseme_ptr const& definition(parseme_ptr const& x) { return x->children()[1]; }
		inline parseme_ptr const& intrinsic_info(parseme_ptr const& x) { return x->children()[2]; }
	}

	namespace function {
		using parsing::parseme_ptr;

		inline parseme_ptr const& pattern(parseme_ptr const& x) { return x->children()[0]; }
		inline parseme_ptr const& parameter_list(parseme_ptr const& x) { return x->children()[1]; }
		inline parseme_ptr const& return_type(parseme_ptr const& x) { return x->children()[2]; }
		inline parseme_ptr const& body(parseme_ptr const& x) { return x->children()[3]; }
	}

	namespace parameter {
		using parsing::parseme_ptr;

		inline parseme_ptr const& identifier(parseme_ptr const& x) { return x->children()[0]; }
		inline parseme_ptr const& type_name(parseme_ptr const& x) { return x->children()[1]; }
	}

	namespace unary_expr {
		using parsing::parseme_ptr;

		inline parseme_ptr const& child(parseme_ptr const& x) { return x->children()[0]; }
	}

	namespace binary_expr {
		using parsing::parseme_ptr;

		inline parseme_ptr const& lhs(parseme_ptr const& x) { return x->children()[0]; }
		inline parseme_ptr const& rhs(parseme_ptr const& x) { return x->children()[1]; }
	}

	namespace return_statement {
		using parsing::parseme_ptr;

		inline parseme_ptr const& expression(parseme_ptr const& x) { return x->children()[0]; }
	}

//=====================================================================
} // namespace marshall
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

