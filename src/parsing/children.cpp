#include <aeon/parsing/children.hpp>
#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::parseme_wptr;
using aeon::parsing::children_t;

children_t::children_t()
	: owner_()
{
}

children_t::children_t(parseme_t* owner)
	: owner_(owner)
{
	ATMA_ASSERT(owner_);
}

auto children_t::operator[] (int i) -> parseme_ptr const&
{
	return elements_[i];
}

auto children_t::owner() const -> parseme_t const*
{
	return owner_;
}

auto children_t::size() const -> uint32_t
{
	return elements_.size();
}

auto children_t::empty() const -> bool
{
	return elements_.empty();
}

auto children_t::front() const -> parseme_ptr const&
{
	return elements_.front();
}

auto children_t::back() const -> parseme_ptr const&
{
	return elements_.back();
}

auto children_t::begin() const -> const_iterator
{
	return elements_.cbegin();
}

auto children_t::end() const -> const_iterator
{
	return elements_.cend();
}

auto children_t::begin() -> iterator
{
	return elements_.begin();
}

auto children_t::end() -> iterator
{
	return elements_.end();
}

auto children_t::push_back(parseme_ptr const& x) -> void
{
	ATMA_ASSERT(!x->parent());
	elements_.push_back(x);
	if (owner_)
		x->set_parent(owner_->shared_from_this());
}

auto children_t::replace(const_iterator const& i, parseme_ptr const& n) -> void
{
	parseme_ptr p = *i;
	p->set_parent(nullptr);
	n->set_parent(owner_ ? owner_->shared_from_this() : parseme_ptr());
	*unconst(i) = n;
}

auto children_t::replace(parseme_ptr const& p, parseme_ptr const& n) -> void
{
	auto i = std::find(elements_.begin(), elements_.end(), p);
	ATMA_ASSERT(i != elements_.end());

	p->set_parent(nullptr);
	n->set_parent(owner_ ? owner_->shared_from_this() : parseme_ptr());
	*unconst(i) = n;
}

auto children_t::detach(const_iterator const& i) -> parseme_ptr
{
	parseme_ptr p = *i;
	elements_.erase( unconst(i) );
	p->set_parent(nullptr);
	return p;
}

auto children_t::detach(parseme_ptr const& x) -> bool
{
	auto i = std::find(elements_.begin(), elements_.end(), x);
	if (i == elements_.end())
		return false;

	(*i)->set_parent(nullptr);
	elements_.erase(i);
	return true;
}

auto children_t::insert(iterator const& where_, iterator const& begin, iterator const& end) -> iterator
{
	auto ni = elements_.insert(where_, begin, end);
	if (owner_)
		for (auto i = begin; i != end; ++i, ++ni)
			(*i)->set_parent(owner_->shared_from_this());
	return ni;
}

auto children_t::unconst(const_iterator const& i) -> iterator
{
	return elements_.erase(i, i);
}

auto children_t::pop_back() -> void
{
	elements_.back()->set_parent(nullptr);
	elements_.pop_back();
}

auto aeon::parsing::detail::print_parsemes(std::ostream& stream, children_t const& xs, uint32_t spaces) -> std::ostream&
{
	for (auto const& x : xs) {
		stream << std::string(spaces, ' ') << *x << std::endl;
		print_parsemes(stream, x->children(), spaces + 2);
	}

	return stream;
}

auto aeon::parsing::clone(children_t& dest, children_t const& src) -> void
{
	for (auto const& x : src)
		dest.push_back(clone(x));
}

