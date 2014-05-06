#include <aeon/lexing/lexical_analysis.hpp>

using namespace aeon::lexing;
using aeon::lexing::lexical_analysis_t;

lexical_analysis_t::lexical_analysis_t(char const* begin, char const* end)
	: begin_(begin), end_(end), current_(begin),
	  tabs_(), previous_tabs_(), empty_line_(true)
{
	run();
}

auto lexical_analysis_t::lexemes() const -> lexemes_t const&
{
	return lexemes_;
}

auto lexical_analysis_t::stream_valid() const -> bool
{
	return current_ != end_;
}

auto lexical_analysis_t::stream_increment() const -> bool
{
	ATMA_ASSERT(current_ != end_);

	++position_.column;
	++position_.total;
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

auto lexical_analysis_t::state_reset_whitespace() -> void
{
	empty_line_ = true;
	previous_tabs_ = tabs_;
	tabs_ = 0;
}

auto lexical_analysis_t::state_nonwhitespace_token() -> void
{
	if (empty_line_)
	{
		if (tabs_ > previous_tabs_)
		while (previous_tabs_++ != tabs_)
			lexemes_.push_back(lexeme_t(ID::block_begin, nullptr, nullptr, position_, channels[(uint)ID::block_begin]));
		else if (tabs_ < previous_tabs_)
		while (previous_tabs_-- != tabs_)
			lexemes_.push_back(lexeme_t(ID::block_end, nullptr, nullptr, position_, channels[(uint)ID::block_begin]));
	}

	empty_line_ = false;
}

#if 0
namespace
{
	namespace
	{
		using namespace aeon::lexing;
#define X(n,s,l,c) c,
		aeon::lexing::channel_t const channels[] ={
			AEON_LEXING_IDS()
		};
#undef X
	}

#define X(n,s,l,c) s,
	char const* keywords[] ={
		AEON_LEXING_IDS()
	};
#undef X

#define X(n,s,l,c) l,
	uint32_t keyword_lengths[] ={
		AEON_LEXING_IDS()
	};
#undef X

	uint32_t const keywords_begin = static_cast<uint32_t>(aeon::lexing::ID::keyword_lower_bound) + 1;
	uint32_t const keywords_end = static_cast<uint32_t>(aeon::lexing::ID::keyword_upper_bound);
}
#endif




#define CASE_NEWLINE_CHAR \
	case '\n': case '\r'

#define CASE_PUNCTUATION_CHAR \
	case '-': case '+': case '*': case '/': \
	case '<': case '>': case '=': case '!': \
	case '&': case '|': case '%': case '^': \
	case '.': case '[': case ']': case '(': \
	case ')': case '{': case '}': case ':': \
	case ',': case '_'

#define CASE_IDENTIFIER_CHAR \
	case 'a': case 'b': case 'c': case 'd': case 'e': \
	case 'f': case 'g': case 'h': case 'i': case 'j': \
	case 'k': case 'l': case 'm': case 'n': case 'o': \
	case 'p': case 'q': case 'r': case 's': case 't': \
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': \
	case '@'

#define CASE_NUMBER_CHAR \
	case '0': case '1': case '2': case '3': case '4': \
	case '5': case '6': case '7': case '8': case '9'

auto lexical_analysis_t::run() -> void
{
	while (stream_valid())
	{
		switch (cv())
		{
			case '\n': case '\r':
				state_reset_whitespace();
				break;

			default:
				state_nonwhitespace_token();
		}

		switch (cv())
		{
			case '\n': case '\r': case '\t':
				block();
				break;

			CASE_IDENTIFIER_CHAR:
				identifier();
				break;

			CASE_NUMBER_CHAR:
				number_literal();
				break;

			CASE_PUNCTUATION_CHAR:
				punctuation();
				break;

			case '"':
				string_literal();
				break;

			case '\'':
				character_literal();
				break;

			case ' ':
				whitespace_tokeniser();
				break;

			default:
				stream.increment();
		}
	}
}

auto lexical_analysis_t::identifier() -> void
{
	char const* m = current_;

	ID id = ID::identifier;
	int types = 0;

	while (stream.cv() == '@') {
		stream.increment();
		++types;
	}

	char c;
	while (stream_valid() && (c = stream_cv()) && ('a' <= c && c <='z' || '0' <= c && c <= '9' || c == '-'))
		stream_increment();

	if (types == 1)
		id = ID::type;

	for (auto x = keywords_begin; x != keywords_end; ++x)
	{
		if (keyword_lengths[x] == (current_ - m) && !strncmp(m, keywords[x], keyword_lengths[x])) {
			state.push_back(static_cast<aeon::lexing::ID>(x), m, current_, stream.marked_position(), channels[x]);
			return;
		}
		else if (keyword_lengths[x] > uint32_t(stream.current() - b)) {
			break;
		}
	}

	state.push_back(id, b, stream.current(), stream.marked_position(), channels[static_cast<int>(id)]);
}
