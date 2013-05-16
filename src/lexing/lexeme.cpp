#include <jigl/lexing/lexeme.hpp>
//=====================================================================
using jigl::lexing::lexeme_t;
using jigl::lexing::position_t;
using jigl::lexing::channel_t;
//=====================================================================

lexeme_t::lexeme_t( id_t id, char const* begin, char const* end, position_t const& position, channel_t const& channel)
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

auto lexeme_t::channel() const -> channel_t const&
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

