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
				empty_line_ = true;
				tabs_ = 0;
				break;

			default:
				non_whitespace_token();
		}

		switch (cv())
		{
			case '\n': case '\r': case '\t':
				block(state, stream);
				break;

			CASE_IDENTIFIER_CHAR:
				identifier(state, stream);
				break;

			CASE_NUMBER_CHAR:
				number_literal(state, stream);
				break;

			CASE_PUNCTUATION_CHAR:
				punctuation();
				break;

			case '"':
				string_literal(state, stream);
				break;

			case '\'':
				character_literal(state, stream);
				break;

			case ' ':
				whitespace_tokeniser(state, stream);
				break;

			default:
				stream.increment();
		}
	}
}


