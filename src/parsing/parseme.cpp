#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::parsemes_t;

parseme_t::parseme_t(id_t id)
	: id_(id), children_(this), lexeme_()
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

auto parseme_t::text() const -> aeon::lexing::lexeme_t::text_t const&
{
	if (lexeme_)
		return lexeme_->text();
	else
		return lexing::lexeme_t::empty_text;
}

auto parseme_t::parent() const -> parseme_ptr const&
{
	return parent_;
}

auto parseme_t::children() const -> children_t const&
{
	return children_;
}

auto parseme_t::children() -> children_t&
{
	return children_;
}

auto parseme_t::set_parent(parseme_ptr const& p) -> void
{
	ATMA_ASSERT(!p || !parent_);
	parent_ = p;
}

