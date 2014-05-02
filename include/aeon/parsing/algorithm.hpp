#pragma once
//=====================================================================
#include <aeon/parsing/parseme.hpp>

#include <set>
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

	template <typename FN>
	inline void for_each_depth_first(children_t& xs, FN fn) {
		for (auto& x : xs) {
			fn(x);
			for_each_depth_first(x->children(), fn);
		}
	}

	template <typename FN>
	inline void for_each_unique(children_t& xs, FN const& fn) {
		std::set<parseme_t const*> visited;
		for_each_depth_first(xs, [&visited, &fn](parseme_ptr const& x) {
			if (visited.find(x.get()) != visited.end())
				return;
			visited.insert(x.get());
			fn(x);
		});
	}

	template <typename FN>
	inline auto find_ancestor(parseme_ptr const& x, FN const& fn) -> parseme_ptr {
		if (!x)
			return null_parseme_ptr;
		else if (fn(x))
			return x;
		else
			return find_ancestor(x->parent(), fn);
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
	inline auto find(parseme_ptr const& x, FN const& fn) -> parseme_ptr const&
	{
		if (fn(x))
			return x;

		for (auto const& xc : x->children())
			if (auto const& xcr = find(xc, fn))
				return xcr;

		return parsing::null_parseme_ptr;
	}

	namespace detail
	{
		template <typename F>
		inline auto upwards_enclosing(children_t const& xs, F const& f) -> void
		{
			for (auto const& x : xs)
				if (f(xs, x))
					return;

			if (auto const* o = xs.owner())
				if (auto p = o->parent())
					upwards_enclosing(p->children(), f);
		}

		template <typename F>
		inline auto upwards_enclosing(parseme_ptr const& x, F const& f) -> void
		{
			if (auto p = x->parent())
				upwards_enclosing(p->children(), f);
		}
	}
	
	template <typename F>
	inline auto upwards_enclosing_find(parseme_ptr const& x, F const& fn) -> parseme_ptr const&
	{
		parseme_ptr const* result = &null_parseme_ptr;

		detail::upwards_enclosing(x, [&result, &fn](children_t const& xs, parseme_ptr const& x){
			if (fn(x)) {
				result = &x;
				return true;
			}

			return false;
		});

		return *result;
	}

	template <typename F>
	inline auto upwards_enclosing_for_each(parseme_ptr const& x, F const& f) -> void
	{
		detail::upwards_enclosing(x, [](children_t const&, parseme_ptr const& x) {
			f(x);
			return false;
		});
	}

	template <typename FN>
	inline void ex_test_for_each_mutate(children_t& xs, FN fn) {
		for (auto& x : xs) {
			fn(xs, x);
			ex_test_for_each_mutate(x->children(), fn);
		}
	}

	auto const passthrough = [](children_t&, parseme_ptr const&) {};

	template <typename FN, typename FNUP = decltype(passthrough)>
	inline auto transform_depth_first(children_t& xs, FN const& fn, FNUP const& fnup = passthrough) -> void {
		for (auto& x : xs) {
			fn(xs, (parseme_ptr const&)x);
			transform_depth_first(x->children(), fn, fnup);
			fnup(xs, (parseme_ptr const&)x);
		}
	}

	struct id_equals_t
	{
		id_equals_t(ID id)
			: id_(id)
		{}

		auto operator ()(parseme_ptr const& x) const -> bool { return x->id() == id_; }

	private:
		ID id_;
	};

} }
