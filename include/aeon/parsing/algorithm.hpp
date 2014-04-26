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
	inline void copy_depth_first_if(OT& out, children_t const& xs, PR& pred)
	{
		for (auto const& x : xs) {
			if (pred(x))
				*out++ = x;
			copy_depth_first_if(out, x->children(), pred);
		}
	}

	template <typename OT, typename PR>
	inline void copy_direct_upwards_if(OT& out, parseme_ptr const& x, PR& pred)
	{
		for (auto y = x; !!y; y = y->parent())
			if (pred(y))
				*out++ = y;
	}

	template <typename OT, typename PR>
	inline void copy_breadth_first_upwards_if(OT& out, parseme_ptr const& x, PR& pred)
	{
		for (auto p = x->parent(); !!p; p = p->parent())
			for (auto const& pc : p->children())
				if (pred(pc))
					*out++ = pc;
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

	inline auto find_root(parseme_ptr const& x) -> parseme_ptr
	{
		ATMA_ASSERT(x);

		auto p = x->parent();
		if (!p)
			return x;

		while (p->parent())
			p = p->parent();

		return p;
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

