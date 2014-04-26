#pragma once
//=====================================================================
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
	// position in file
	//=====================================================================
	struct position_t
	{
		static position_t zero;

		position_t() : row(), column() {}

		position_t(uint32_t row, uint32_t column)
			: row(row), column(column)
		{
		}

		uint32_t row, column;
	};
	
	
	//=====================================================================
	// channel
	//=====================================================================
	struct channel_t
	{
		channel_t() : bits_() {}
		explicit channel_t(uint32_t ch) : bits_(1 << ch) {}
		channel_t(multichannel_t const&);

		auto as_int() const -> uint32_t { return bits_; }

		auto operator == (channel_t const& rhs) const -> bool {
			return bits_ == rhs.bits_;
		}
		
		friend auto operator & (channel_t const& lhs, channel_t const& rhs) -> bool {
			return (lhs.bits_ & rhs.bits_) != 0;
		}

		friend auto operator | (channel_t const& lhs, channel_t const& rhs) -> multichannel_t;


	private:
		uint32_t bits_;

		friend struct multichannel_t;
	};

	struct multichannel_t
	{
		multichannel_t() : bits_() {}
		explicit multichannel_t(uint32_t chs) : bits_(chs) {}
		multichannel_t(channel_t const& ch) : bits_(ch.bits_) {}
		static multichannel_t all;

		auto as_int() const -> uint32_t { return bits_; }

		operator bool() const { return bits_ != 0; }

	private:
		uint32_t bits_;

		friend struct channel_t;
	};

	inline channel_t::channel_t(multichannel_t const& rhs)
	 : bits_(rhs.bits_)
	{
	}

	inline auto operator | (channel_t const& lhs, channel_t const& rhs) -> multichannel_t {
		return multichannel_t(lhs.as_int() | rhs.as_int());
	}
	
	inline auto operator & (multichannel_t const& lhs, channel_t const& rhs) -> multichannel_t {
		return multichannel_t(lhs.as_int() & rhs.as_int());
	}


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
