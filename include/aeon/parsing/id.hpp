#pragma once

namespace aeon { namespace parsing {

	enum class ID
	{
		root,
		module,

		integer_literal,
		identifier,
		placeholder,

		function,
		function_pattern,
		parameter_list,
		parameter,

		function_call,
		type_name,
		type,
		block,
		return_statement,
		expr,
		addition_expr,
		type_definition,
		argument_list,

		variable_declaration,

		// intrinsics
		intrinsic_type_int,
		intrinsic_bitsize,
		intrinsic_int_add,
		intrinsic_int_sub,
		intrinsic_int_mul,
		intrinsic_int_div,

		// for error recovery
		error_term,
		error_type,

		attributes,
		attribute_forceinline,
	};

} }
