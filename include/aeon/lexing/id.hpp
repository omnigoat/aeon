//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_LEXING_ID_HPP
#define AEON_LEXING_ID_HPP
//=====================================================================
namespace aeon {
namespace lexing {
//=====================================================================
	
	struct channel_t;
	static channel_t const basic(0);
	static channel_t const whitespace(1);
	static channel_t const control(2);
	
	static multichannel_t const all(3);

	#define AEON_LEXING_IDS() \
		X(identifier, "", 0, basic) \
		X(integer_literal, "", 0, basic) \
		X(real_literal, "", 0, basic)  \
		X(punctuation, "", 0, basic) \
		X(block_begin, "", 0, control | whitespace) \
		X(block_end, "", 0, control | whitespace) \
		X(type, "", 0, basic) \
		X(string_literal, "", 0, basic) \
		X(character_literal, "", 0, basic) \
		X(whitespace, "", 0, whitespace) \
		X(keyword_lower_bound, "", 0, basic) \
		X(if_keyword, "if", 2, basic) \
		X(as_keyword, "as", 2, basic) \
		X(end_keyword, "end", 3, basic) \
		X(else_keyword, "else", 4, basic) \
		X(type_keyword, "type", 4, basic) \
		X(function_keyword, "function", 8, basic) \
		X(keyword_upper_bound, "", 0, basic)

	#define X(name, string, strlen, channel) name,
	enum class ID
	{
		AEON_LEXING_IDS()
	};
	#undef X


//=====================================================================
} // namespace lexing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

