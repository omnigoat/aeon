#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::parsemes_t;

parseme_t::parseme_t(id_t id)
	: id_(id)
{
}

parseme_t::parseme_t(id_t id, aeon::lexing::lexeme_t const* lexeme)
	: id_(id), children_(this), lexeme_(lexeme)
{
}

auto parseme_t::id() const -> id_t
{
	return id_;
}

auto parseme_t::parent() const -> parseme_ptr const&
{
	return parent_;
}

auto parseme_t::children() const -> aeon::parsing::parsemes_t const&
{
	return children_;
}

auto parseme_t::set_parent(parseme_ptr const& p) -> void
{
	ATMA_ASSERT(!p || !parent_);
	parent_ = p;
}

#if 0
auto parseme_t::add_child(parseme_ptr const& child) -> void
{
	children_.push_back(child);
}

auto parseme_t::remove_child(parsemes_t::const_iterator const& child) -> void
{
	children_.erase(child);
}

auto parseme_t::replace_child(parsemes_t::const_iterator const& old_child, parseme_ptr const& new_child) -> void
{
	// what a weird way to get an iterator from a const_iterator
	*children_.erase(old_child, old_child) = new_child;
}
#endif
