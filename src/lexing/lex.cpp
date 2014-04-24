#include <aeon/lexing/lex.hpp>
#include <aeon/lexing/id.hpp>
#include <atma/assert.hpp>

using aeon::lexing::lexeme_t;
using aeon::lexing::lexemes_t;
using aeon::lexing::stream_t;
using aeon::lexing::state_t;
using aeon::lexing::ID;
using aeon::lexing::position_t;
using aeon::lexing::multichannel_t;

aeon::lexing::multichannel_t aeon::lexing::multichannel_t::all(0xffffffff);

namespace
{
	namespace {
		using namespace aeon::lexing;
		#define X(n,s,l,c) c,
		aeon::lexing::channel_t const channels[] = {
			AEON_LEXING_IDS()
		};
		#undef X
	}

	#define X(n,s,l,c) s,
	char const* keywords[] = {
		AEON_LEXING_IDS()
	};
	#undef X

	#define X(n,s,l,c) l,
	uint32_t keyword_lengths[] = {
		AEON_LEXING_IDS()
	};
	#undef X

	uint32_t const keywords_begin = static_cast<uint32_t>(aeon::lexing::ID::keyword_lower_bound) + 1;
	uint32_t const keywords_end = static_cast<uint32_t>(aeon::lexing::ID::keyword_upper_bound);
}



stream_t::stream_t(char const* begin, char const* end)
	: begin_(begin), end_(end), current_(begin), position_(1, 1), marked_position_(1, 1), consumed_newline_()
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

auto stream_t::position() const -> aeon::lexing::position_t const& {
	return position_;
}

auto stream_t::increment() -> void {
	ATMA_ASSERT(current_ != end_);

	++position_.column;
	++current_;

	bool is_newline = *current_ == '\n' || *current_ == '\r';

	// skip past nestled newline characters so we don't erroneously continue
	// changing our logical file position
	if (!consumed_newline_ && is_newline) {
		consumed_newline_ = true;
		++position_.row;
		position_.column = 1;
	}
	else {
		consumed_newline_ = false;
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


state_t::state_t(lexemes_t& lexemes)
	: empty_line_(true), previous_tabs_(), tabs_(), lexemes_(lexemes)
{
}

auto state_t::push_back(lexeme_t::id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel) -> void
{
	ATMA_ASSERT(begin != end);
	lexemes_.push_back( lexeme_t(id, begin, end, position, channel) );
}

auto state_t::non_whitespace_token() -> void
{
	if (empty_line_)
	{
		if (tabs_ > previous_tabs_)
			while (previous_tabs_++ != tabs_)
				lexemes_.push_back(lexeme_t(ID::block_begin, nullptr, nullptr, position_t(), channels[static_cast<uint32_t>(ID::block_begin)]));
		else if (tabs_ < previous_tabs_)
			while (previous_tabs_-- != tabs_)
				lexemes_.push_back(lexeme_t(ID::block_end, nullptr, nullptr, position_t(), channels[static_cast<uint32_t>(ID::block_begin)]));

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
	case ',': case '_':

#define WHITESPACE_PREDICATE \
	case ' ':

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



//
// identifier/keyword
//
auto identifier(state_t& state, stream_t& stream) -> void
{
	char const* b = stream.mark();
	ID id = ID::identifier;
	int types = 0;

	while (stream.cv() == '@') {
		stream.increment();
		++types;
	}

	char c;
	while (stream.valid() && (c = stream.cv()) && ('a' <= c && c <='z' || '0' <= c && c <= '9' || c == '-'))
		stream.increment();

	if (types == 1)
		id = ID::type;

	for (auto x = keywords_begin; x != keywords_end; ++x)
	{
		if (keyword_lengths[x] == (stream.current() - b) && !strncmp(b, keywords[x], keyword_lengths[x])) {
			state.push_back(static_cast<aeon::lexing::ID>(x), b, stream.current(), stream.marked_position(), channels[x]);
			return;
		}
		else if (keyword_lengths[x] > uint32_t(stream.current() - b)) {
			break;
		}
	}

	state.push_back(id, b, stream.current(), stream.marked_position(), channels[static_cast<int>(id)]);
}


//
// whitespace
//
auto whitespace_tokeniser(state_t& state, stream_t& stream) -> void
{
	char const* m = stream.mark();
	while (stream.valid())
	{
		switch (stream.cv()) {
			WHITESPACE_PREDICATE
				stream.increment();
				break;

			default:
				state.push_back(ID::whitespace, m, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::whitespace)]);
				return;
		}
	}
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
			state.push_back(ID::real_literal, m, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::real_literal)]);
		else {
			state.push_back(ID::integer_literal, m, dot, stream.marked_position(), channels[static_cast<int>(ID::integer_literal)]);
			stream.reset(dot);
		}
	}
	else {
		state.push_back(ID::integer_literal, m, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::integer_literal)]);
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
					state.push_back(ID::punctuation, b, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::punctuation)]);
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

	state.push_back(ID::string_literal, m, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::string_literal)]);
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

	state.push_back(ID::character_literal, m, stream.current(), stream.marked_position(), channels[static_cast<int>(ID::character_literal)]);
}



auto aeon::lexing::lex(state_t& state, stream_t& stream) -> void
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

		WHITESPACE_PREDICATE
			whitespace_tokeniser(state, stream);
			break;

		default:
			stream.increment();
	}

	

	goto begin;

done:;
}
