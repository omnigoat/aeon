#include <jigl/parsing/parseme.hpp>

using jigl::parsing::parseme_t;
using jigl::parsing::parseme_ptr;
using jigl::parsing::parsemes_t;

parseme_t::parseme_t(id_t id)
	: id_(id)
{
}

parseme_t::parseme_t(parseme_ptr const& parent, id_t id, jigl::lexing::lexeme_t* lexeme)
	: id_(id), parent_(parent), lexeme_(lexeme)
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

auto parseme_t::children() const -> jigl::parsing::parsemes_t const&
{
	return children_;
}

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
