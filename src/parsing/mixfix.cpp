#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/algorithm.hpp>
#include <aeon/parsing/xpi.hpp>

#include <aeon/marshall.hpp>

#include <algorithm>

using namespace aeon;
using namespace aeon::parsing;

namespace
{
	auto pattern_predicate(parseme_ptr const& lhs, parseme_ptr const& rhs) -> bool
	{
		ATMA_ASSERT_ONE_OF(lhs->id(), ID::identifier, ID::placeholder);
		ATMA_ASSERT_ONE_OF(rhs->id(), ID::identifier, ID::placeholder);

		return lhs->id() < rhs->id();
	}

	auto function_precedence_predicate(parseme_ptr const& lhs, parseme_ptr const& rhs) -> bool
	{
		ATMA_ASSERT(lhs->id() == ID::function);
		ATMA_ASSERT(rhs->id() == ID::function);

		// todo: sort by scope
		// ...


		// sort by length of function pattern
		auto lhs_pattern = marshall::function::pattern(lhs);
		auto rhs_pattern = marshall::function::pattern(rhs);
		if (lhs_pattern->children().size() != rhs_pattern->children().size())
			return lhs_pattern->children().size() < rhs_pattern->children().size();

		// sort by pattern (identifiers > placeholders)
		return std::lexicographical_compare(
			lhs_pattern->children().begin(), lhs_pattern->children().end(),
			rhs_pattern->children().begin(), rhs_pattern->children().end(),
			pattern_predicate);

		// todo: more sorting???
		// ...

		return false;
	}

	auto functions_for_scope(parseme_ptr const& expr) -> parsemes_t
	{
		// get all functions for this scope
		parsing::parsemes_t fns;
		parsing::for_each_direct_upwards(expr, [&fns](parsing::parseme_ptr const& x) {
			if (x->id() == parsing::ID::module) {
				parsing::copy_depth_first_if(std::back_inserter(fns), x->children(), [](parsing::parseme_ptr const& y) {
					return y->id() == parsing::ID::function;
				});
			}
		});


		// function lookup predicate.
		//   1. sort by scope
		//   2. sort by mixfix length
		//   3. ??
		//   4. profit
		std::sort(fns.begin(), fns.end(), function_precedence_predicate);

		return fns;
	}

	struct function_pattern_filter_t
	{
		function_pattern_filter_t(parsemes_t const& ast) : ast_(ast) {}

		auto operator() (parseme_ptr const& x) const -> bool
		{
			ATMA_ASSERT(x->id() == ID::function);

			auto pattern = marshall::function::pattern(x);

			if (pattern->children().size() < ast_.size())
				return false;

			return std::equal(ast_.begin(), ast_.end(), pattern->children().begin(), 
				[](parseme_ptr const& lhs, parseme_ptr const& rhs)
				{
					// identifiers can be consumed by placeholders or matching identifiers
					if (lhs->id() == ID::identifier) {
						return rhs->id() == ID::placeholder || rhs->text() == lhs->text();
					}
					else {
						return rhs->id() == ID::placeholder;
					}
				});
		}

	private:
		parsemes_t const& ast_;
	};

	auto shift(parsemes_t& xs, children_t::iterator& x) -> void {
		xs.push_back(*x);
		++x;
	}

	auto reduce(parsemes_t& xs, parseme_ptr const& fn) -> void
	{
		//parseme_ptr k = parseme_t(ID::function_call, xs.front()->lexeme))
		auto const& x = xs.front();

		parsemes_t ys;
		//xpi::insert_into(ys, xpi::make(ID::function_call, x->lexeme()));
	}

	auto mixfix_fixup(parseme_ptr const& expr) -> void
	{
		auto functions = functions_for_scope(expr);

		parsemes_t ast;
		auto expri = expr->children().begin();
		auto choice = parseme_ptr();

		parsemes_t candidates(functions.begin(), functions.end());
		while (expri != expr->children().end() && !choice && !candidates.empty())
		{
			shift(ast, expri);
		
			// filter candidate functions
			parsemes_t filtered_candidates;
			std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(filtered_candidates), function_pattern_filter_t(ast));

			for (auto const& x : filtered_candidates) {
				// fully consumed candidate, use it!
				if (marshall::function::pattern(x)->children().size() == ast.size()) {
					choice = x;
					break;
				}
			}

			// reduce expression (break if there's nothing left)
			if (choice) {
				reduce(ast, choice);
			}

			std::swap(candidates, filtered_candidates);
		}
	}
}

auto aeon::parsing::mixfix_resolution(children_t& xs) -> void
{
	parsing::for_each_depth_first(xs, [](parseme_ptr const& x) {
		if (x->id() == ID::expr)
			mixfix_fixup(x);
	});
}


