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
	void copy_depth_first_if(OT& out, children_t const& xs, PR pred)
	{
		for (auto const& x : xs) {
			if (pred(x))
				*out++ = x;
			copy_depth_first_if(out, x->children(), pred);
		}
	}

	template <typename OT, typename PR>
	void copy_direct_upwards_if(OT& out, parseme_ptr const& x, PR pred)
	{
		for (auto y = x; !!y; y = y->parent())
			if (pred(y))
				*out++ = x;
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

