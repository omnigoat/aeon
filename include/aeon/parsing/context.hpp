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
#include <set>
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	namespace detail
	{
		struct context_t
		{
			context_t(parseme_ptr const& root, lexing::lexemes_t::const_iterator const&);

			auto id() const -> lexing::ID;
			
			auto align(lexing::multichannel_t const&) -> void;

			auto match_make(parseme_t::id_t, lexing::ID) -> parseme_ptr;
			auto match_make(parseme_t::id_t, lexing::ID, char const*) -> parseme_ptr;
			auto match_make(parseme_t::id_t, lexing::ID, lexing::multichannel_t const&) -> parseme_ptr;

			auto skip(lexing::ID) -> bool;
			auto skip(lexing::ID, char const*) -> bool;

			auto current_lexeme() const -> lexing::lexeme_t const*;

			auto generate_intrinsic_integer_definition(parseme_ptr const& type_name) -> void;

		private:
			parseme_ptr root_;
			lexing::lexemes_t::const_iterator begin_;
			std::set<uint32_t> intrinsic_integers_;
		};
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

