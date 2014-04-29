#pragma once
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon { namespace marshall {
	
	namespace type_definition {
		using parsing::parseme_ptr;

		inline auto name(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto definition(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
		inline auto intrinsic_info(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[2]; }
	}

	namespace function {
		using parsing::parseme_ptr;

		inline auto pattern(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto parameter_list(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
		inline auto return_type(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[2]; }
		inline auto body(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[3]; }
	}

	namespace parameter {
		using parsing::parseme_ptr;

		inline auto identifier(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto type_name(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
	}

	namespace unary_expr {
		using parsing::parseme_ptr;

		inline auto child(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
	}

	namespace binary_expr {
		using parsing::parseme_ptr;

		inline auto lhs(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto rhs(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
	}

	namespace return_statement {
		using parsing::parseme_ptr;

		inline auto expression(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
	}

	namespace function_call {
		using parsing::parseme_ptr;

		inline auto pattern(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto argument_list(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
	}

	namespace variable_declaration {
		using parsing::parseme_ptr;

		inline auto identifier(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[0]; }
		inline auto expr(parseme_ptr const& x) -> parseme_ptr const& { return x->children()[1]; }
	}

} }

