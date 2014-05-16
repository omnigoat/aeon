#include <aeon/generation/generation.hpp>

#include <aeon/generation/genesis.hpp>
#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>
#include <aeon/parsing/algorithm.hpp>

using namespace aeon;
using namespace generation;

using parsing::ID;
using parsing::parseme_ptr;

auto aeon::generation::function_name_mangle(parsing::parseme_ptr const& fn) -> atma::string
{
	atma::string result = "f";

	// function name
	auto name = marshall::function::name(fn);
	if (name->text() == "main")
		return "main";
	else if (name->text() == "+")
		result += "$add";
	else if (name->text() == "-")
		result += "$sub";
	else if (name->text() == "*")
		result += "$mul";
	else if (name->text() == "/")
		result += "$div";
	else
		result += atma::to_string(name->text().bytes()) + name->text();

	result += "_";

	// return-type
	auto return_type = marshall::function::return_type(fn);
	result += type_name_mangle(return_type);

	// parameters
	result += "_";
	auto parameter_list = marshall::function::parameter_list(fn);
	for (auto const& x : parameter_list->children())
		result += type_name_mangle(marshall::parameter::type_name(x));

	return result;
}

auto aeon::generation::type_name_mangle(parsing::parseme_ptr const& type) -> atma::string
{
	if (type->text() == "@int32")
		return "i32";
	else
		return atma::string("t") + std::to_string(type->text().bytes()) + type->text();
}

auto aeon::generation::type_structure(parsing::parseme_ptr const& type) -> atma::string
{
	typedef parsing::parseme_t::id_t parsid;

	ATMA_ASSERT(type->id() == parsid::type_definition);

	if (marshall::type_definition::definition(type)->id() == parsid::intrinsic_type_int)
	{
		return atma::string("i") + marshall::type_definition::intrinsic_info(type)->text();
	}

	return atma::string();
}


auto aeon::generation::module(abstract_output_stream_t& stream, parsing::parseme_ptr const& module) -> void
{
	genesis_t genesis;

	analyse::module(genesis, module);

	for (auto const& fn : module->children())
	{
		if (fn->id() == aeon::parsing::parseme_t::id_t::function)
		{
			generation::function(stream, genesis, fn);
		}
	}
}

auto aeon::generation::function(abstract_output_stream_t& stream, genesis_t& genesis, parsing::parseme_ptr const& fn) -> void
{
	auto const& return_type = marshall::function::return_type(fn);
	auto const& return_type_definition = resolve::typename_to_definition(return_type);
	auto const& parameters = marshall::function::parameter_list(fn)->children();
	
	line_begin(stream) << "define " << type_structure(return_type_definition) << " @" << function_name_mangle(fn) << "(";
	for (auto i = parameters.begin(); i != parameters.end(); ++i)
	{
		if (i != parameters.begin())
			stream << ", ";

		auto type_definition = resolve::typename_to_definition(marshall::parameter::type_name(*i));
		
		stream << generation::type_structure(type_definition) << " %" << marshall::parameter::identifier(*i)->text();
	}

	stream << ")\n";
	function_body(stream, genesis, marshall::function::body(fn));
}

auto aeon::generation::function_body(abstract_output_stream_t& stream, genesis_t& genesis, parsing::parseme_ptr const& body) -> void
{
	line_begin(stream) << "{\n";
	{
		scoped_stream_tabify_t sst(stream);

		for (auto const& x : body->children())
		{
			statement(stream, genesis, x);
		}
	}
	line_begin(stream) << "}\n\n";
}

auto aeon::generation::statement(abstract_output_stream_t& stream, genesis_t& genesis, parsing::parseme_ptr const& statement) -> void
{
	switch (statement->id())
	{
		case parsing::parseme_t::id_t::return_statement:
			return_statement(stream, genesis, statement);
			break;
	}
}

namespace
{
	auto llvm_expr_result(genesis_t& genesis, parseme_ptr const& expr) -> atma::string
	{
		return atma::string("%") + std::to_string(genesis.expr_id(expr));
	}

	auto llvm_identifier(parseme_ptr const& x) -> atma::string
	{
		return atma::string("%") + x->text();
	}
}

auto aeon::generation::return_statement(abstract_output_stream_t& stream, genesis_t& genesis, parsing::parseme_ptr const& statement) -> void
{
	ATMA_ASSERT(statement->id() == parsing::ID::return_statement);

	auto expr = marshall::return_statement::expression(statement);
	expression(stream, genesis, expr);

	auto type = resolve::type_of(expr);
	auto typen = llvm::storage_typename(genesis, type);
	
	line_begin(stream) << "ret " << typen << " " << llvm_expr_result(genesis, expr) << "\n";
}



auto aeon::generation::expression(abstract_output_stream_t& stream, genesis_t& genesis, parsing::parseme_ptr const& expr) -> void
{
	switch (expr->id())
	{
		case ID::intrinsic_int_add:
		{
			auto lhs = marshall::binary_expr::lhs(expr);
			auto rhs = marshall::binary_expr::rhs(expr);
			auto type = resolve::type_of(expr);

			expression(stream, genesis, lhs);
			expression(stream, genesis, rhs);
			line_begin(stream) << llvm::lvalue_name(genesis, expr) <<  " = add " << llvm::storage_typename(genesis, type) << " " << llvm::lvalue_name(genesis, lhs) << ", " << llvm::lvalue_name(genesis, rhs) << "\n"; // %lhs, %rhs\n";
			break;
		}

		case ID::intrinsic_int_sub:
		{
			auto lhs = marshall::binary_expr::lhs(expr);
			auto rhs = marshall::binary_expr::rhs(expr);
			auto type = resolve::type_of(expr);

			expression(stream, genesis, lhs);
			expression(stream, genesis, rhs);
			line_begin(stream) << llvm::lvalue_name(genesis, expr) <<  " = sub " << llvm::storage_typename(genesis, type) << " " << llvm::lvalue_name(genesis, lhs) << ", " << llvm::lvalue_name(genesis, rhs) << "\n"; // %lhs, %rhs\n";
			break;
		}

		case ID::intrinsic_int_mul:
		{
			auto lhs = marshall::binary_expr::lhs(expr);
			auto rhs = marshall::binary_expr::rhs(expr);
			auto type = resolve::type_of(expr);

			expression(stream, genesis, lhs);
			expression(stream, genesis, rhs);
			line_begin(stream) << llvm::lvalue_name(genesis, expr) <<  " = mul " << llvm::storage_typename(genesis, type) << " " << llvm::lvalue_name(genesis, lhs) << ", " << llvm::lvalue_name(genesis, rhs) << "\n"; // %lhs, %rhs\n";
			break;
		}

		case ID::intrinsic_int_div:
		{
			auto lhs = marshall::binary_expr::lhs(expr);
			auto rhs = marshall::binary_expr::rhs(expr);
			auto type = resolve::type_of(expr);

			expression(stream, genesis, lhs);
			expression(stream, genesis, rhs);
			line_begin(stream) << llvm::lvalue_name(genesis, expr) <<  " = sdiv " << llvm::storage_typename(genesis, type) << " " << llvm::lvalue_name(genesis, lhs) << ", " << llvm::lvalue_name(genesis, rhs) << "\n"; // %lhs, %rhs\n";
			break;
		}

		case ID::function_call:
		{
			// all parameters
			auto argument_list = marshall::function_call::argument_list(expr);
			auto fn = resolve::function_from_function_call(expr);
			auto fn_return_type = resolve::type_of(marshall::function::return_type(fn));

			for (auto const& arg : argument_list->children())
				expression(stream, genesis, arg);

			line_begin(stream) << llvm::lvalue_name(genesis, expr) << " = call " << llvm::storage_typename(genesis, fn_return_type) << " @" << function_name_mangle(fn) << "(";

			for (auto i = argument_list->children().begin(); i != argument_list->children().end(); ++i)
			{
				if (i != argument_list->children().begin())
					stream << ", ";

				stream << llvm::storage_typename(genesis, *i) << " " << llvm::lvalue_name(genesis, *i);
			}

			stream << ")\n";

			break;
		}
	}
}

auto aeon::generation::llvm::storage_typename(genesis_t& genesis, parsing::parseme_ptr const& x) -> atma::string
{
	auto type_definition = resolve::type_of(x);
	ATMA_ASSERT(type_definition->id() == ID::type_definition);

	if (marshall::type_definition::definition(type_definition)->id() == ID::intrinsic_type_int)
		return "i32";
	
	if (marshall::type_definition::name(type_definition)->text() == "@int32")
		return "i32";

	return "<type-unknown>";
}

auto aeon::generation::llvm::lvalue_name(genesis_t& genesis, parsing::parseme_ptr const& x) -> atma::string
{
	using parsing::ID;

	switch (x->id())
	{
		case ID::integer_literal:
			return atma::string(x->text());

		case ID::identifier:
			return atma::string("%") + x->text();
		
		case ID::function_call:
		case ID::intrinsic_int_add:
		case ID::intrinsic_int_sub:
		case ID::intrinsic_int_mul:
		case ID::intrinsic_int_div:
			return atma::string("%") + std::to_string(genesis.expr_id(x));
	}

	return "<zomg>";
}
