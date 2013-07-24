#include <aeon/parsing/parsemes.hpp>
#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::parseme_wptr;
using aeon::parsing::parsemes_t;

parsemes_t::parsemes_t()
	: owner_()
{
}

parsemes_t::parsemes_t(parseme_t* owner)
	: owner_(owner)
{
	ATMA_ASSERT(owner_);
}

auto parsemes_t::size() const -> uint32_t
{
	return elements_.size();
}

auto parsemes_t::front() const -> parseme_ptr const&
{
	return elements_.front();
}

auto parsemes_t::back() const -> parseme_ptr const&
{
	return elements_.back();
}

auto parsemes_t::begin() const -> const_iterator
{
	return elements_.cbegin();
}

auto parsemes_t::end() const -> const_iterator
{
	return elements_.cend();
}

auto parsemes_t::push_back(parseme_ptr const& x) -> void
{
	elements_.push_back(x);
}

auto parsemes_t::replace(const_iterator const& i, parseme_ptr const& n) -> void
{
	parseme_ptr p = *i;
	p->set_parent(nullptr);
	n->set_parent(owner_ ? owner_->shared_from_this() : parseme_ptr());
	*unconst(i) = n;
}

auto parsemes_t::detach(const_iterator const& i) -> parseme_ptr
{
	parseme_ptr p = *i;
	elements_.erase( unconst(i) );
	p->set_parent(nullptr);
	return p;
}

auto parsemes_t::unconst(const_iterator const& i) -> iterator
{
	return elements_.erase(i, i);
}

auto parsemes_t::pop_back() -> void
{
	elements_.back()->set_parent(nullptr);
	elements_.pop_back();
}

