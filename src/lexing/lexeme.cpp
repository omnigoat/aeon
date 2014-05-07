#include <aeon/lexing/lexeme.hpp>
#include <atma/assert.hpp>
//=====================================================================
using aeon::lexing::lexeme_t;
using aeon::lexing::position_t;
using aeon::lexing::multichannel_t;
//=====================================================================

lexeme_t::text_t const lexeme_t::empty_text;
position_t const aeon::lexing::position_t::zero;

lexeme_t::lexeme_t(id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel)
: id_(id), text_(begin, end), position_(position), channel_(channel)
{
}

auto lexeme_t::id() const -> lexeme_t::id_t const&
{
	return id_;
}

auto lexeme_t::position() const -> position_t const&
{
	return position_;
}

auto lexeme_t::channel() const -> multichannel_t const&
{
	return channel_;
}

auto lexeme_t::text() const -> text_t const&
{
	return text_;
}

auto lexeme_t::streq(char const* begin) const -> bool
{
	char const* x = text_.begin(), *y = begin;
	while (x != text_.end() && *y && *x == *y)
		++x, ++y;
	return x == text_.end() && !*y;
}

auto lexeme_t::streq(char const* begin, char const* end) const -> bool
{
	int32_t xsize = text_.bytes(),
		    ysize = end - begin
			;
	ATMA_ASSERT(xsize >= 0 && ysize >= 0);

	return strncmp(text_.begin(), begin, xsize) == 0;
}


std::ostream& aeon::lexing::operator << (std::ostream& stream, lexeme_t const& L) {
	return stream << static_cast<int>(L.id()) << "[" << L.position().row << ":" << L.position().column << "]: " << L.text();
}

auto aeon::lexing::make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel) -> lexeme_t const*
{
	auto L = new lexeme_t(id, begin, end, position, channel);
	return (lexeme_t const*)(((intptr_t)L) | 1);
}

auto aeon::lexing::make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end) -> lexeme_t const*
{
	auto L = new lexeme_t(id, begin, end, position_t(), multichannel_t());
	return (lexeme_t const*)(((intptr_t)L) | 1);
}

auto aeon::lexing::make_synthetic_lexeme(lexeme_t::id_t id, char const* str) -> lexeme_t const*
{
	auto L = new lexeme_t(id, str, str + strlen(str), position_t(), multichannel_t());
	return (lexeme_t const*)(((intptr_t)L) | 1);
}