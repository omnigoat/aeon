//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_ALGORITHM_HPP
#define AEON_PARSING_ALGORITHM_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	template <typename OT, typename PR>
	inline void copy_depth_first_if(OT& out, children_t const& xs, PR pred)
	{
		for (auto const& x : xs) {
			if (pred(x))
				*out++ = x;
			copy_depth_first_if(out, x->children(), pred);
		}
	}

	template <typename OT, typename PR>
	inline void copy_direct_upwards_if(OT& out, parseme_ptr const& x, PR pred)
	{
		for (auto y = x; !!y; y = y->parent())
			if (pred(y))
				*out++ = y;
	}

	template <typename FN>
	inline void for_each_direct_upwards(parseme_ptr const& x, FN fn) {
		for (auto y = x; !!y; y = y->parent())
			fn(y);
	}

	template <typename FN>
	inline void for_each_depth_first(children_t const& xs, FN fn) {
		for (auto const& x : xs) {
			fn(x);
			for_each_depth_first(x->children(), fn);
		}
	}

	template <typename FN>
	inline void ex_test_for_each_mutate(children_t& xs, FN fn) {
		for (auto& x : xs) {
			fn(xs, x);
			ex_test_for_each_mutate(x->children(), fn);
		}
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

