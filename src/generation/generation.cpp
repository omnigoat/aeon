#include <aeon/generation/generation.hpp>

#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>
#include <aeon/parsing/algorithm.hpp>

using namespace aeon;

using parsing::ID;


auto aeon::generation::function_name_mangle(parsing::parseme_ptr const& fn) -> atma::string
{
	atma::string result;

	// function name
	auto pattern = marshall::function::pattern(fn);

	if (pattern->children().size() == 1 && pattern->children().back()->text() == "main")
		return "main";

	for (auto const& x : pattern->children())
	{
		if (x->id() == parsing::ID::placeholder)
			result += "$";
		else
			result += atma::to_string(x->text().bytes()) + x->text();

		result += "_";
	}

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
		return atma::string("@i") + marshall::type_definition::intrinsic_info(type)->text();
	}

	return atma::string();
}


auto aeon::generation::module(abstract_output_stream_t& stream, parsing::parseme_ptr const& module) -> void
{
	for (auto const& fn : module->children())
	{
		if (fn->id() == aeon::parsing::parseme_t::id_t::function)
		{
			generation::function(stream, fn);
		}
	}
}

auto aeon::generation::function(abstract_output_stream_t& stream, parsing::parseme_ptr const& fn) -> void
{
	auto const& return_type = marshall::function::return_type(fn);
	auto const& return_type_definition = resolve::typename_to_definition(return_type);
	auto const& parameters = marshall::function::parameter_list(fn)->children();
	
	stream << "define " << type_structure(return_type_definition) << " @" << function_name_mangle(fn) << "(";
	for (auto i = parameters.begin(); i != parameters.end(); ++i)
	{
		if (i != parameters.begin())
			stream << ", ";

		auto type_definition = resolve::typename_to_definition(marshall::parameter::type_name(*i));
		
		stream << generation::type_structure(type_definition) << " %" << marshall::parameter::identifier(*i)->text();
	}

	stream << ")\n";
	function_body(stream, marshall::function::body(fn));
}

auto aeon::generation::function_body(abstract_output_stream_t& stream, parsing::parseme_ptr const& body) -> void
{
	stream << "{\n";
	{
		scoped_stream_tabify_t sst(stream);

		for (auto const& x : body->children())
		{
			statement(stream, x);
		}

	}
	stream << "}\n";
}

auto aeon::generation::statement(abstract_output_stream_t& stream, parsing::parseme_ptr const& statement) -> void
{
	switch (statement->id())
	{
		case parsing::parseme_t::id_t::return_statement:
			return_statement(stream, statement);
			break;
	}
}

auto aeon::generation::return_statement(abstract_output_stream_t& stream, parsing::parseme_ptr const& statement) -> void
{
	ATMA_ASSERT(statement->id() == parsing::ID::return_statement);

	auto expr = marshall::return_statement::expression(statement);
	expression(stream, expr);

	auto type = resolve::type_of(expr);
	auto typen = type_name_mangle(type);
	
	stream << "ret " << typen << " " << "4" << "\n";
}



auto aeon::generation::expression(abstract_output_stream_t& stream, parsing::parseme_ptr const& expr) -> void
{
	switch (expr->id())
	{
		case ID::intrinsic_int_add:
		{
			auto lhs = marshall::binary_expr::lhs(expr);
			auto rhs = marshall::binary_expr::rhs(expr);

			stream << "add @i" << expr->text() << " %lhs, %rhs\n";
			break;
		}
	}
}

auto aeon::generation::llvm_typename(parsing::parseme_ptr x) -> parsing::parseme_t::text_t
{
	auto type_definition = resolve::type_of(x);

	if (marshall::type_definition::definition(type_definition)->id() == ID::intrinsic_type_int)
		return "i32";
	
	if (marshall::type_definition::name(type_definition)->text() == "@int32")
		return "i32";

	return "<type-unknown>";
}

auto aeon::generation::llvm_variable_name(parsing::parseme_ptr x) -> parsing::parseme_t::text_t
{
	ATMA_ASSERT(x->id() == ID::identifier);

	return "lulz";
}
