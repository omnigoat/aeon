#include <aeon/generation/generation.hpp>

#include <aeon/parsing/marshall.hpp>

using namespace aeon;

auto aeon::generation::function_name_mangle(parsing::parseme_ptr const& fn) -> atma::string
{
	namespace marshall = parsing::marshall;
	
	atma::string result;

	// function name
	auto name = marshall::function::name(fn);
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


auto aeon::generation::module(parsing::parseme_ptr const& module) -> void
{
	for (auto const& fn : module->children()) {
		if (fn->id() == aeon::parsing::parseme_t::id_t::function)
		{
			std::cout << "fn: " << function_name_mangle(fn) << std::endl;
		}
	}
}

