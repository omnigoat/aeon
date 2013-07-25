//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_CONTEXT_HPP
#define AEON_PARSING_CONTEXT_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/parsemes.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t
		{
			context_t(lexing::lexemes_t::const_iterator const&);

			auto match_make(lexing::ID, parseme_t::id_t) -> parseme_ptr;

		private:
			lexing::lexemes_t::const_iterator begin_;
		};
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

