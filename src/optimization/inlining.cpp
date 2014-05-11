#include <aeon/optimization/inlining.hpp>

#include <aeon/parsing/algorithm.hpp>
#include <aeon/parsing/xpi.hpp>

#include <aeon/resolve.hpp>
#include <aeon/marshall.hpp>

#include <atma/string.hpp>

#include <map>


using namespace aeon;
using namespace optimization;

namespace
{
	using namespace parsing;

	int count = 0;

	auto inline_function_call(children_t& xs, parseme_ptr const& call) -> void
	{
		//if (count++ == 2)
			//return;

		auto fn = resolve::function_from_function_call(call);
		auto fn_parameters = marshall::function::parameter_list(fn);
		auto statement = parsing::find_ancestor(call, parsing::id_equals_t(ID::return_statement));
		auto scope = statement->parent();
		auto call_parent = call->parent();
		auto& call_parent_children = call_parent->children();
		
		static char const* argnames[6] ={
			"arg0", "arg1", "arg2", "arg3", "arg4", "arg5"
		};

		auto generate_argiden = []() -> char const* {
			static auto idx = 0;
			return argnames[idx++];
		};

		bool nonvoid_function = true;
		

		auto arg_of_param = [&call](parseme_ptr const& param) -> parseme_ptr const&
		{
			auto pl = param->parent();
			ATMA_ASSERT(pl && pl->id() == ID::parameter_list);

			auto pi = std::find(pl->children().begin(), pl->children().end(), param);
			ATMA_ASSERT(pi != pl->children().end());

			auto ai = marshall::function_call::argument_list(call)->children().begin();
			std::advance(ai, std::distance(pl->children().begin(), pi));
			return *ai;
		};



		//
		// clone the inlining-function's body, and:
		//   - find which parameters are used multiple times
		//   - collect all return statements
		//
		auto fn_clone = parsing::clone(fn);
		auto fn_clone_body = marshall::function::body(fn_clone);

		std::map<parseme_ptr, int> param_refcounts;
		std::vector<parseme_ptr> return_statements;
		parsing::for_each_depth_first(fn_clone_body->children(),
		  [&param_refcounts, &return_statements](parseme_ptr const& x)
		{
			if (x->id() == parsing::ID::return_statement) {
				return_statements.push_back(x);
			}

			if (x->id() != parsing::ID::identifier)
				return;

			auto def = resolve::identifier_to_definition(x);
			if (!def || def->id() != parsing::ID::parameter)
				return;

			++param_refcounts[def];
		});


		//
		// generate statements for multiply-references arguments
		//
		parsing::parsemes_t precaller_statements;
		std::map<parseme_ptr, parseme_ptr> arg_to_iden;

		for (auto const& x : param_refcounts)
		{
			if (x.second > 1) {
				char const* iden = generate_argiden();
				auto const& arg = arg_of_param(x.first);

				if (resolve::is_atomic_expr(arg))
					continue;

#if SERIOUSLY_ENABLE_AGAIN
				auto vd = xpi::reify(
					xpi::make(parsing::ID::variable_declaration)[
						xpi::make(parsing::ID::identifier, lexing::make_synthetic_lexeme(lexing::ID::identifier, iden)),
						xpi::insert(parsing::clone(arg))
					]);

				arg_to_iden[arg] = parsing::parseme_t::make(parsing::ID::identifier, lexing::make_synthetic_lexeme(lexing::ID::identifier, iden));

				precaller_statements.push_back(vd);
#endif
			}
		}


		//
		// build a map of parameter -> replacement expression
		//  - this could be an expression
		//  - this could also be an identifier to a temporary variable
		//    because the parameter is referenced multiple times (and we
		//    should only evaluate the argument once)
		//
		std::map<parseme_ptr, parseme_ptr> parameter_replacements;
		{
			auto const& params = marshall::function::parameter_list(fn_clone)->children();
			auto const& args = marshall::function_call::argument_list(call)->children();

			auto ai = args.begin();
			for (auto const& x : params) {
				if (param_refcounts[x] > 1 && arg_to_iden.find(*ai) != arg_to_iden.end())
					parameter_replacements[x] = arg_to_iden[*ai];
				else
					parameter_replacements[x] = arg_of_param(x);
				++ai;
			}
		}


		//
		// go through cloned-function's body and replace all identifiers that
		// reference a parameter with the replacement
		//
		parsing::transform_depth_first(fn_clone_body->children(), parsing::passthrough,
			[&parameter_replacements](parsing::children_t& xs, parseme_ptr const& x)
		{
			if (x->id() != parsing::ID::identifier)
				return;

			auto def = resolve::identifier_to_definition(x);
			if (!def || def->id() != parsing::ID::parameter)
				return;

			if (parameter_replacements.find(def) != parameter_replacements.end())
				xs.replace(x, parsing::clone(parameter_replacements[def]));
		});


		//
		// insert body of cloned-function into place where the function-call
		// originally happened.
		//
		//auto i = std::find(scope->children().begin(), scope->children().end(), call_parent);
		//scope->children().insert(i, precaller_statements.begin(), precaller_statements.end());
		ATMA_ASSERT(return_statements.size() == 1);
		auto p = return_statements.front()->children().front();
		return_statements.front()->children().pop_back();
		call_parent->children().replace(call, p);

		std::cout << marshall::function::pattern(fn)->text() << ": inlined " << scope->children() << std::endl;
	}
}


auto aeon::optimization::inline_all_the_things(parsing::children_t& xs) -> void
{
	using parsing::parseme_ptr;
	namespace xpi = parsing::xpi;

	parsing::transform_depth_first(xs, parsing::passthrough, [](parsing::children_t& xs, parseme_ptr const& x)
	{
		if (x->id() == parsing::ID::function_call)
			if (auto const& f = resolve::function_from_function_call(x))
				if (resolve::is_function_forceinline(f))
					inline_function_call(xs, x);
	});

#if 0
	// get all function-calls
	parsing::parsemes_t calls;
	parsing::copy_depth_first_if(std::back_inserter(calls), xs,
		[](parsing::parseme_ptr const& x) { return x->id() == parsing::ID::function_call; });

	
	std::reverse(calls.begin(), calls.end());
	int count = 0;
	for (auto const& call : calls)
	{
		// only inline calls to binary functions atm
		if (marshall::function_call::pattern(call)->children().size() != 3)
			continue;



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

		bool nonvoid_function = true;
		uint return_count = 0;

		


		

		if (count == 2) {
			std::cout << "cloned fn-body " << fn_clone_body->children() << std::endl;
			std::cout << "existing scope " << scope->children() << std::endl;
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
		
		auto i = std::find(scope->children().begin(), scope->children().end(), call_parent);
		scope->children().insert(i, precaller_statements.begin(), precaller_statements.end());
		//call_parent->children().detach(call);
		ATMA_ASSERT(return_statements.size() == 1);
		auto p = return_statements.front()->children().front();
		return_statements.front()->children().pop_back();
		call_parent->children().replace(call, p);
		
		if (++count == 3)
			break;
	}
#endif

}