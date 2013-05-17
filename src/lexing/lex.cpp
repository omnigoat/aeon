#include <jigl/lexing/lex.hpp>
#include <atma/assert.hpp>

using jigl::lexing::lexeme_t;
using jigl::lexing::lexemes_t;
using jigl::lexing::stream_t;
using jigl::lexing::ID;
//namespace LXID = jigl::lexing::ID;

stream_t::stream_t(char const* begin, char const* end)
	: begin_(begin), end_(end), current_(begin), position_(1, 1)
{
}

auto stream_t::begin() const -> char const* {
	return begin_;
}

auto stream_t::end() const -> char const* {
	return end_;
}

auto stream_t::current() const -> char const* {
	return current_;
}

auto stream_t::cv() const -> char {
	return *current_;
}

auto stream_t::valid() const -> bool {
	return current_ != end_;
}

auto stream_t::position() const -> jigl::lexing::position_t const& {
	return position_;
}

auto stream_t::increment() -> void {
	ATMA_ASSERT(current_ != end_);

	
	++current_;

	if (current_ != end_ && (*current_ == '\n' || *current_ == '\r' && current_ + 1 != end_ && current_[1] == '\n')) {
		++position_.row;
		position_.column = 1;
	}

	#if 0
	// skip past windows newlines
	if (current_ != end_ && *current_ == '\r' && current_ + 1 != end_ && current_[1] == '\n')
		++current_;
	#endif
}

auto stream_t::reset(char const* mark) -> void {
	current_ = mark;
}


auto identifier(lexemes_t& result, stream_t& stream) -> void
{
	char const* b = stream.current();
	while (stream.valid() && ('a' <= stream.cv() && stream.cv() <='z' || stream.cv() == '-'))
		stream.increment();

	result.push_back(lexeme_t(ID::identifier, b, stream.current(), stream.position()));
}

auto number(lexemes_t& result, stream_t& stream) -> void
{
	char const* b = stream.current();
	while (stream.valid() && ('0' <= stream.cv() && stream.cv() <='9'))
		stream.increment();

	result.push_back(lexeme_t(ID::integer_literal, b, stream.current(), stream.position()));
}

auto jigl::lexing::lex(lexemes_t& result, stream_t& stream) -> void
{
	while (stream.valid())
	{
		switch (stream.cv())
		{
			case 'a': case 'b': case 'c': case 'd': case 'e':
			case 'f': case 'g': case 'h': case 'i': case 'j':
			case 'k': case 'l': case 'm': case 'n': case 'o':
			case 'p': case 'q': case 'r': case 's': case 't':
			case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
				identifier(result, stream);
				break;

			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				number(result, stream);
				break;

			default:
				stream.increment();
		}
	}
}
