#include <jigl/lexing/lex.hpp>
#include <jigl/lexing/id.hpp>
#include <atma/assert.hpp>

using jigl::lexing::lexeme_t;
using jigl::lexing::lexemes_t;
using jigl::lexing::stream_t;
using jigl::lexing::state_t;
using jigl::lexing::ID;
using jigl::lexing::position_t;

//namespace LXID = jigl::lexing::ID;

stream_t::stream_t(char const* begin, char const* end)
	: begin_(begin), end_(end), current_(begin), position_(1, 1), marked_position_(1, 1)
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

	++position_.column;
	++current_;

	if (current_ != end_ && (*current_ == '\n' || *current_ == '\r' && current_ + 1 != end_ && current_[1] == '\n')) {
		++position_.row;
		position_.column = 1;
	}
}

auto stream_t::mark() -> char const* {
	marked_position_ = position_;
	return mark_ = current_;
}

auto stream_t::reset(char const* mark) -> void {
	current_ = mark;
}

auto stream_t::marked_position() const -> position_t const& {
	return marked_position_;
}


state_t::state_t()
	: empty_line_(true), previous_tabs_(), tabs_()
{
}

auto state_t::push_back(lexeme_t::id_t id, char const* begin, char const* end, position_t const& position) -> void
{
	ATMA_ASSERT(begin != end);
	lexemes_.push_back( lexeme_t(id, begin, end, position) );
}

auto state_t::non_whitespace_token() -> void
{
	if (empty_line_)
	{
		if (tabs_ > previous_tabs_)
			while (previous_tabs_++ != tabs_)
				lexemes_.push_back(lexeme_t(ID::block_begin, nullptr, nullptr, position_t()));
		else if (tabs_ < previous_tabs_)
			while (previous_tabs_-- != tabs_)
				lexemes_.push_back(lexeme_t(ID::block_end, nullptr, nullptr, position_t()));

		previous_tabs_ = tabs_;
	}

	empty_line_ = false;
}

auto state_t::reset_whitespace() -> void
{
	empty_line_ = true;
	tabs_ = 0;
}

auto state_t::increment_tabs() -> void
{
	++tabs_;
}








#define BLOCK_NEWLINE_PREDICATE \
	case '\n': case '\r':

#define BLOCK_WHITESPACE_PREDICATE \
	case '\t':

#define BLOCK_PREDICATE BLOCK_NEWLINE_PREDICATE BLOCK_WHITESPACE_PREDICATE

#define IDENTIFIER_PREDICATE \
	case 'a': case 'b': case 'c': case 'd': case 'e': \
	case 'f': case 'g': case 'h': case 'i': case 'j': \
	case 'k': case 'l': case 'm': case 'n': case 'o': \
	case 'p': case 'q': case 'r': case 's': case 't': \
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': \
	case '@':

#define NUMBER_PREDICATE \
	case '0': case '1': case '2': case '3': case '4': \
	case '5': case '6': case '7': case '8': case '9':

#define PUNCTUATION_PREDICATE \
	case '-': case '+': case '*': case '/': \
	case '<': case '>': case '=': case '!': \
	case '&': case '|': case '%': case '^': \
	case '.': case '[': case ']': case '(': \
	case ')': case '{': case '}': case ':': \
	case ',':



//
//
//
auto block(state_t& state, stream_t& stream) -> void
{
	while (stream.valid())
	{
		switch (stream.cv()) {
			BLOCK_NEWLINE_PREDICATE
				state.reset_whitespace();
				stream.increment();
				break;

			BLOCK_WHITESPACE_PREDICATE
				state.increment_tabs();
				stream.increment();
				break;

			default:
				goto done;
		}
	}
done:;
}



namespace
{
	#define X(n,s,l) s,
	char const* keywords[] = {
		JIGL_LEXING_IDS()
	};
	#undef X

	#define X(n,s,l) l,
	uint32_t keyword_lengths[] = {
		JIGL_LEXING_IDS()
	};
	#undef X

	uint32_t const keywords_begin = static_cast<uint32_t>(jigl::lexing::ID::keyword_lower_bound) + 1;
	uint32_t const keywords_end = static_cast<uint32_t>(jigl::lexing::ID::keyword_upper_bound);
}


//
// identifier/keyword
//
auto identifier(state_t& state, stream_t& stream) -> void
{
	//char const* b = stream.current();
	char const* b = stream.mark();
	ID id = ID::identifier;
	int types = 0;

	while (stream.cv() == '@') {
		stream.increment();
		++types;
	}

	while (stream.valid() && ('a' <= stream.cv() && stream.cv() <='z' || stream.cv() == '-'))
		stream.increment();

	if (types == 1)
		id = ID::type;

	for (auto x = keywords_begin; x != keywords_end; ++x)
	{
		if (keyword_lengths[x] == (stream.current() - b) && !strncmp(b, keywords[x], keyword_lengths[x])) {
			state.push_back(static_cast<jigl::lexing::ID>(x), b, stream.current(), stream.marked_position());
			return;
		}
		else if (keyword_lengths[x] > uint32_t(stream.current() - b)) {
			break;
		}
	}

	state.push_back(id, b, stream.current(), stream.marked_position());
}



//
// numbers
//
auto number_literal(state_t& state, stream_t& stream) -> void
{
	// do the integer literal
	char const* m = stream.mark();
	while (stream.valid() && ('0' <= stream.cv() && stream.cv() <='9'))
		stream.increment();

	// maybe turn it into a real literal?
	if (stream.cv() == '.')
	{
		// do more numbers for decimal places
		char const* dot = stream.current();
		stream.increment();
		uint32_t i = 0;
		while (stream.valid() && ('0' <= stream.cv() && stream.cv() <='9')) {
			stream.increment();
			++i;
		}

		// integer or real
		if (i > 0)
			state.push_back(ID::real_literal, m, stream.current(), stream.marked_position());
		else {
			state.push_back(ID::integer_literal, m, dot, stream.marked_position());
			stream.reset(dot);
		}
	}
	else {
		state.push_back(ID::integer_literal, m, stream.current(), stream.marked_position());
	}
}


//
// punctuation
//
auto punctuation(state_t& state, stream_t& stream) -> void
{
	char const* b = stream.mark();
	
	while (stream.valid())
	{
		switch (stream.cv())
		{
			PUNCTUATION_PREDICATE
				stream.increment();
				break;

			default:
				if (stream.current() != b)
					state.push_back(ID::punctuation, b, stream.current(), stream.marked_position());
				goto done;
		}
	}

done:;
}


auto string_literal(state_t& state, stream_t& stream) -> void
{
	char const* m = stream.mark();
	if (stream.cv() == '"')
	{
		stream.increment();
		while (stream.valid() && stream.cv() != '"')
			stream.increment();
		stream.increment();
	}

	state.push_back(ID::string_literal, m, stream.current(), stream.marked_position());
}


auto character_literal(state_t& state, stream_t& stream) -> void
{
	char const* m = stream.mark();
	if (stream.cv() == '\'')
	{
		stream.increment();
		while (stream.valid() && stream.cv() != '\'')
			stream.increment();
		stream.increment();
	}

	state.push_back(ID::character_literal, m, stream.current(), stream.marked_position());
}



auto jigl::lexing::lex(state_t& state, stream_t& stream) -> void
{
begin:
	if (!stream.valid())
		goto done;

	switch (stream.cv())
	{
		BLOCK_NEWLINE_PREDICATE
			state.reset_whitespace();
			break;

		default:
			state.non_whitespace_token();
	}

	switch (stream.cv())
	{
		BLOCK_PREDICATE
			block(state, stream);
			break;

		IDENTIFIER_PREDICATE
			identifier(state, stream);
			break;

		NUMBER_PREDICATE
			number_literal(state, stream);
			break;

		PUNCTUATION_PREDICATE
			punctuation(state, stream);
			break;

		case '"':
			string_literal(state, stream);
			break;

		case '\'':
			character_literal(state, stream);
			break;

		default:
			stream.increment();
	}

	

	goto begin;

done:;
}
