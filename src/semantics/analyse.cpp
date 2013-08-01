#include <aeon/semantics/analyse.hpp>
#include <aeon/parsing/marshall.hpp>

namespace marshall = aeon::parsing::marshall;

auto aeon::semantics::analyse::function(parseme_ptr const& fn) -> analysis_t
{
	parseme_ptr const& return_type = marshall::function::return_type(fn);
	parseme_ptr const& function_body = marshall::function::body(fn);

	// gather up return statements
	//parsing::raw_parsemes_t return_statements;
	//parsing::copy_depth_first_if(std::back_inserter(return_statements), function_body->children(), 
	return analysis_t();
}