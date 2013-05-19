//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_ID_HPP
#define JIGL_LEXING_ID_HPP
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================
	
	#define JIGL_LEXING_IDS() \
		X(identifier, "", 0) \
		X(integer_literal, "", 0) \
		X(real_literal, "", 0)  \
		X(punctuation, "", 0) \
		X(block_begin, "", 0) \
		X(block_end, "", 0) \
		X(type, "", 0) \
		X(string_literal, "", 0) \
		X(character_literal, "", 0) \
		X(keyword_lower_bound, "", 0) \
		X(if_keyword, "if", 2) \
		X(as_keyword, "as", 2) \
		X(end_keyword, "end", 3) \
		X(else_keyword, "else", 4) \
		X(type_keyword, "type", 4) \
		X(function_keyword, "function", 8) \
		X(keyword_upper_bound, "", 0)

	#define X(name, string, strlen) name,
	enum class ID
	{
		JIGL_LEXING_IDS()
	};
	#undef X


//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

