#include <aeon/optimization/inlining.hpp>

#include <aeon/parsing/algorithm.hpp>
#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>

#include <map>


using namespace aeon;
using namespace optimization;


auto aeon::optimization::inline_all_the_things(parsing::children_t& xs) -> void
{
	// get all function-calls
	parsing::parsemes_t calls;
	parsing::copy_depth_first_if(std::back_inserter(calls), xs,
		[](parsing::parseme_ptr const& x) { return x->id() == parsing::ID::function_call; });

#if 0
	// filter inlining
	calls.erase(
		
	);
#endif
	
	using parsing::parseme_ptr;

	for (auto const& call : calls)
	{
		auto fn = resolve::function_from_function_call(call);
		ATMA_ASSERT(fn);
		auto fn_parameters = marshall::function::parameter_list(fn);

		auto call_parent = call->parent();
		ATMA_ASSERT(call_parent);

		auto statement = parsing::find_ancestor(call, [](parsing::parseme_ptr const& x)
			{ return x->id() == parsing::ID::return_statement; });
		ATMA_ASSERT(statement);

		auto scope = statement->parent();
		ATMA_ASSERT(scope);

		

		// clone the inlining-function's (Fi) body, and:
		//   - find which parameters are used multiple times
		//   - 
		//   - replace all identifiers of the parameters with the arguments
		parsing::children_t body;
		parsing::clone(body, marshall::function::body(fn)->children());

		std::map<parseme_ptr, int> refcounts;
		parsing::for_each_depth_first(body, [&refcounts, &fn_parameters](parseme_ptr const& x)
		{
			if (x->id() != parsing::ID::identifier)
				return;

			auto def = resolve::identifier_to_definition(x);
			if (!def)
				return;

			auto pi = std::find(fn_parameters->children().begin(), fn_parameters->children().end(), def);
			if (pi == fn_parameters->children().end())
				return;

			++refcounts[x];
		});

		// go through scope, replacing all references 

		call_parent->children().detach(call);
	}
}