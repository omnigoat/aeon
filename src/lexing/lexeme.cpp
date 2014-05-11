#include <aeon/lexing/lexeme.hpp>
#include <atma/assert.hpp>
//=====================================================================
using aeon::lexing::lexeme_t;
using aeon::lexing::position_t;
using aeon::lexing::multichannel_t;
//=====================================================================

lexeme_t::text_t const lexeme_t::empty_text;
position_t const aeon::lexing::position_t::zero;

lexeme_t::lexeme_t(id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel, uint flags)
: id_(id), text_(begin, end), position_(position), channel_(channel), synthetic_(flags & 1)
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



auto lexeme_t::make(id_t id, char const* begin, char const* end, position_t const& position) -> lexeme_t
{
	return lexeme_t(id, begin, end, position, multichannel_t::all, 0);
}

auto lexeme_t::make(id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& chan) -> lexeme_t
{
	return lexeme_t(id, begin, end, position, chan, 0);
}

auto lexeme_t::make_aux(id_t id, char const* begin, char const* end, position_t const& position) -> lexeme_t
{
	return lexeme_t(id, begin, end, position, multichannel_t::all, 1);
}





std::ostream& aeon::lexing::operator << (std::ostream& stream, lexeme_t const& L) {
	return stream << static_cast<int>(L.id()) << "[" << L.position().row << ":" << L.position().column << "]: " << L.text();
}

