#include <aeon/generation/generation.hpp>
#include <aeon/parsing/marshall.hpp>
#include <atma/string.hpp>

using namespace aeon;

auto function_name_mangle(atma::string const& name) -> void
{
	atma::string result;
}

auto aeon::generation::type(parsing::parseme_ptr const& type) -> atma::string
{
	atma::utf::cvt_16t8_
}

auto aeon::generation::function(parsing::parseme_ptr const& fn) -> void
{
	namespace marshall = parsing::marshall;

	
	atma::string definition; 
	
	auto return_type = marshall::function::return_type(fn);
	auto name = marshall::function::name(fn);
	
	// blam blam blam blam
	definition += "define ";
	definition += generation::type(return_type);
	
}



