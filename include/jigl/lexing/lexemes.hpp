//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_LEXEMES_HPP
#define JIGL_LEXING_LEXEMES_HPP
//=====================================================================
#include <vector>
//=====================================================================
#include <jigl/lexing/lexeme.hpp>
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================

	typedef std::vector<lexeme_t> lexemes_t;
	
	inline std::ostream& operator << (std::ostream& stream, lexemes_t const& rhs) 
	{
		stream << "lexemes\n---------" << std::endl;
		if (rhs.empty())
			stream << "  empty!" << std::endl;
		
		for (auto const& x : rhs)
			stream << "  " << x << std::endl;
		
		return stream << std::endl;
	}
	
//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

