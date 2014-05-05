#pragma once
//=====================================================================
#include <aeon/lexing/position.hpp>
#include <aeon/lexing/channel.hpp>

#include <atma/utf/utf8_string_range.hpp>

#include <cstdint>
#include <ostream>
//=====================================================================
namespace aeon { namespace lexing {

	enum class ID;

	struct lexeme_t
	{
		typedef ID id_t;
		typedef atma::utf8_string_range_t text_t;
		static text_t const empty_text;

		lexeme_t(id_t, char const* begin, char const* end, position_t const&, multichannel_t const& = multichannel_t());

		auto id() const -> id_t const&;
		auto position() const -> position_t const&;
		auto channel() const -> multichannel_t const&;
		auto text() const -> text_t const&;
		
		auto streq(char const*) const -> bool;
		auto streq(char const*, char const*) const -> bool;

	private:
		id_t id_;
		text_t text_;
		position_t position_;
		multichannel_t channel_;
	};
	

	auto operator << (std::ostream& stream, lexeme_t const& L) -> std::ostream&;

	auto make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel = multichannel_t()) -> lexeme_t const*;
	auto make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end) -> lexeme_t const*;
	auto make_synthetic_lexeme(lexeme_t::id_t id, char const* str) -> lexeme_t const*;

} }
