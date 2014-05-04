#pragma once
//=====================================================================
#include <aeon/lexing/position.hpp>
#include <aeon/lexing/channel.hpp>

#include <atma/utf/utf8_string_range.hpp>

#include <cstdint>
#include <ostream>
#include <string>
//=====================================================================
namespace aeon { namespace lexing {
	
	//=====================================================================
	// forward declares
	//=====================================================================
	enum class ID;
	struct channel_t;
	struct multichannel_t;




	//=====================================================================
	// lexeme
	//=====================================================================
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
	

	inline std::ostream& operator << (std::ostream& stream, lexeme_t const& L) {
		return stream << static_cast<int>(L.id()) << "[" << L.position().row << ":" << L.position().column << "]: " << L.text();
	}

	inline lexeme_t const* make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end, position_t const& position, multichannel_t const& channel = multichannel_t())
	{
		// return a pointer with the lowest bit set, to show that we should delete it
		auto L = new lexeme_t(id, begin, end, position, channel);
		return (lexeme_t const*)(((intptr_t)L) | 1);
	}
	
	inline lexeme_t const* make_synthetic_lexeme(lexeme_t::id_t id, char const* begin, char const* end)
	{
		// return a pointer with the lowest bit set, to show that we should delete it
		auto L = new lexeme_t(id, begin, end, position_t(), multichannel_t());
		return (lexeme_t const*)(((intptr_t)L) | 1);
	}

	inline lexeme_t const* make_synthetic_lexeme(lexeme_t::id_t id, char const* str)
	{
		// return a pointer with the lowest bit set, to show that we should delete it
		auto L = new lexeme_t(id, str, str + strlen(str), position_t(), multichannel_t());
		return (lexeme_t const*)(((intptr_t)L) | 1);
	}

} }
