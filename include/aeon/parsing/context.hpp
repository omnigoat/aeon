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
//#include <aeon/parsing/parsemes.hpp>
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t
		{
			context_t(lexing::lexemes_t::const_iterator const&);

			auto id() const -> lexing::ID;
			
			auto align(lexing::multichannel_t const&) -> void;

			auto match_make(parseme_t::id_t, lexing::ID) -> parseme_ptr;
			auto match_make(parseme_t::id_t, lexing::ID, char const*) -> parseme_ptr;
			auto match_make(parseme_t::id_t, lexing::ID, lexing::multichannel_t const&) -> parseme_ptr;

			auto skip(lexing::ID) -> bool;
			auto skip(lexing::ID, char const*) -> bool;

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

