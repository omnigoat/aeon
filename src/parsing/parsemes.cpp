#include <jigl/parsing/parsemes.hpp>
#include <jigl/parsing/parseme.hpp>
#include <atma/assert.hpp>

using jigl::parsing::parseme_t;
using jigl::parsing::parseme_ptr;
using jigl::parsing::parseme_wptr;
using jigl::parsing::parsemes_t;

parsemes_t::parsemes_t()
	: owner_()
{
}

parsemes_t::parsemes_t(parseme_t* owner)
	: owner_(owner)
{
	ATMA_ASSERT(owner_);
}

auto parsemes_t::begin() const -> const_iterator
{
	return elements_.cbegin();
}

auto parsemes_t::end() const -> const_iterator
{
	return elements_.cend();
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
