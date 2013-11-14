#include <aeon/generation/generation.hpp>

#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>

using namespace aeon;

auto aeon::generation::function_name_mangle(parsing::parseme_ptr const& fn) -> atma::string
{
	atma::string result;

	// function name
	auto name = marshall::function::name(fn);
	if (name->text() == "main")
		return "main";
	result += "_";
	result += atma::to_string(name->text().bytes()) + name->text();

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
	for (auto const& fn : module->children()) {
		if (fn->id() == aeon::parsing::parseme_t::id_t::function)
		{
			generation::function(stream, fn);
			//std::cout << "fn: " << function_name_mangle(fn) << std::endl;
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
		
		stream << generation::type_structure(type_definition) << " " << marshall::parameter::identifier(*i)->text();
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

auto aeon::generation::statement(abstract_output_stream_t& stream, parsing::parseme_ptr const& body) -> void
{
	//stream << line_begin << 
}