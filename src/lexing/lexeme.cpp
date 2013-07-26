#include <aeon/lexing/lexeme.hpp>
#include <atma/assert.hpp>
//=====================================================================
using aeon::lexing::lexeme_t;
using aeon::lexing::position_t;
using aeon::lexing::multichannel_t;
//=====================================================================

lexeme_t::lexeme_t( id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel)
: id_(id), begin_(begin), end_(end), position_(position), channel_(channel)
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

auto lexeme_t::begin() const -> char const*
{
	return begin_;
}

auto lexeme_t::end() const -> char const*
{
	return end_;
}

auto lexeme_t::text() const -> std::string
{
	return std::string(begin_, end_);
}

auto lexeme_t::streq(char const* begin) const -> bool
{
	char const* x = begin_, *y = begin;
	while (x != end_ && *y)
		++x, ++y;
	return x == end_ && !*y;
}

auto lexeme_t::streq(char const* begin, char const* end) const -> bool
{
	int32_t xsize = end_ - begin_,
		    ysize = end - begin
			;
	ATMA_ASSERT(xsize >= 0 && ysize >= 0);

	return strncmp(begin_, begin, xsize) == 0;
}