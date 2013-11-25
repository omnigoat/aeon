#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>

namespace lexing = aeon::lexing;

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::children_t;

parseme_ptr const aeon::parsing::null_parseme_ptr;

parseme_t::parseme_t(id_t id)
	: id_(id), children_(this), lexeme_()
{
}

parseme_t::parseme_t(id_t id, lexing::lexeme_t const* lexeme)
	: id_(id), children_(this), lexeme_(lexeme)
{
}

parseme_t::~parseme_t()
{
	if ((intptr_t)lexeme_ & 1)
		delete lexeme();
}

auto parseme_t::id() const -> id_t
{
	return id_;
}

auto parseme_t::position() const -> lexing::position_t const&
{
	if (lexeme_)
		return lexeme()->position();
	else
		return lexing::position_t::zero;
}

auto parseme_t::text() const -> lexing::lexeme_t::text_t const&
{
	if (lexeme_)
		return lexeme()->text();
	else
		return lexing::lexeme_t::empty_text;
}

auto parseme_t::parent() const -> parseme_ptr
{
	if (parseme_ptr p = parent_.lock())
		return p;
	else
		return parseme_ptr();
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
	ATMA_ASSERT(!p || parent_.expired());
	parent_ = p;
}

auto parseme_t::lexeme() const -> lexing::lexeme_t const*
{
	return (lexing::lexeme_t const*)((intptr_t)lexeme_ & intptr_t(-2));
}

auto parseme_t::make(id_t id, lexing::lexeme_t const* L) -> parseme_ptr
{
	return parseme_ptr(new parseme_t(id, L));
}