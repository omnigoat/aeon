//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_PARSE_HPP
#define AEON_PARSING_PARSE_HPP
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

		auto module(parsemes_t&, lexing::lexemes_t const&, detail::context_t&) -> bool;
		auto function(parsemes_t&, lexing::lexemes_t const&, detail::context_t&) -> bool;
		auto parameters(parsemes_t&, lexing::lexemes_t const&, detail::context_t&) -> bool;
		auto function_body(parsemes_t&, lexing::lexemes_t const&, detail::context_t&) -> bool;
	}


	auto parse(parsemes_t&, lexing::lexemes_t const&) -> void;

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

