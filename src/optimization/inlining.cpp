#include <aeon/optimization/inlining.hpp>

#include <aeon/parsing/algorithm.hpp>
#include <aeon/parsing/xpi.hpp>

#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>

#include <atma/string.hpp>

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
	namespace xpi = parsing::xpi;

	for (auto const& call : calls)
	{
		auto fn = resolve::function_from_function_call(call);
		ATMA_ASSERT(fn);

		auto fn_parameters = marshall::function::parameter_list(fn);

		auto call_parent = call->parent();
		ATMA_ASSERT(call_parent);
		auto& call_parent_children = call_parent->children();

		auto statement = parsing::find_ancestor(call, [](parsing::parseme_ptr const& x)
			{ return x->id() == parsing::ID::return_statement; });
		ATMA_ASSERT(statement);

		auto scope = statement->parent();
		ATMA_ASSERT(scope);

		static char const* argnames[6] ={
			"arg0", "arg1", "arg2", "arg3", "arg4", "arg5"
		};

		auto generate_argiden = []() -> char const* {
			static auto idx = 0;
			return argnames[idx++];
		};

		// clone the inlining-function's (Fi) body, and:
		//   - find which parameters are used multiple times
		//   - 
		//   - replace all identifiers of the parameters with the arguments
		auto fn_clone = parsing::clone(fn);
		auto fn_clone_body = marshall::function::body(fn_clone);

		std::map<parseme_ptr, int> refcounts;
		parsing::for_each_depth_first(fn_clone_body->children(), [&refcounts](parseme_ptr const& x)
		{
			if (x->id() != parsing::ID::identifier)
				return;

			auto def = resolve::identifier_to_definition(x);
			if (!def)
				return;

			if (def->id() != parsing::ID::parameter)
				return;

			++refcounts[x];
		});

		// generate statements for multiply-references arguments
		parsing::parsemes_t precaller_statements;
		//std::map<parseme_ptr, parseme_ptr> precaller_statements;
		for (auto const& x : refcounts) {
			if (x.second > 1) {
				auto vd = xpi::reify(
					xpi::make(parsing::ID::variable_declaration) [
						xpi::make(parsing::ID::identifier, lexing::make_synthetic_lexeme(lexing::ID::identifier, generate_argiden())),
						xpi::insert(x.first)
					]);

				precaller_statements.push_back(vd);
			}
		}

		// build a map of parameter -> argument
		//  - 
		std::map<parseme_ptr, parseme_ptr> replacements;
		{
			auto args = marshall::function_call::argument_list(call)->children();
			int i = 0;
			for (auto const& x : marshall::function::parameter_list(fn_clone)->children())
			{
				replacements[x] = args[i];
			}
		}

#if 1
		parsing::transform_depth_first(fn_clone_body->children(),
			[&replacements](parsing::children_t& xs, parseme_ptr const& x)
			{
				if (x->id() != parsing::ID::identifier)
					return;

				auto def = resolve::identifier_to_definition(x);
				if (!def)
					return;

				if (def->id() != parsing::ID::parameter)
					return;

				if (replacements.find(def) != replacements.end())
					xs.replace(x, parsing::clone(replacements[def]));
			});
#endif

		// replace all references to parameters with their arguments
		
		auto i = std::find(call_parent_children.begin(), call_parent_children.end(), call);
		call_parent_children.insert(i, precaller_statements.begin(), precaller_statements.end());
		//call_parent->children().detach(call);
	}
}