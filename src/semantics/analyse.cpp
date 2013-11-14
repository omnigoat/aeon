#include <aeon/semantics/analyse.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/marshall.hpp>
#include <aeon/resolve.hpp>

auto aeon::semantics::analyse::function(parseme_ptr const& fn) -> analysis_t
{
	analysis_t analysis;

	parseme_ptr const& return_type = marshall::function::return_type(fn);
	parseme_ptr const& function_body = marshall::function::body(fn);

	// gather up return statements
	parsing::parsemes_t return_statements;
	parsing::copy_depth_first_if(
		std::back_inserter(return_statements),
		function_body->children(),
		[](parseme_ptr const& x) { return x->id() == parseme_t::id_t::return_statement; });


	// get type of first return-statement encountered
	/*parsing::parsemes_t::const_iterator i = return_statements.begin();
	parseme_ptr const& first_type = resolve::type_of(*i);
	ATMA_ASSERT(first_type);*/

	// compare all subsequent return-statements against return-type
	parseme_ptr const& resolved_return_type = resolve::type_of(return_type);
	for (auto const& x : return_statements) {
		if (resolve::type_of(x) != resolved_return_type)
			analysis.errors.push_back(error_t("bad return type for return statement!"));
	}

	return analysis;
}