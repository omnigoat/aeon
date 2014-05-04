#pragma once
//=====================================================================
#include <atma/utf/utf8_string_range.hpp>
#include <atma/types.hpp>
//=====================================================================
namespace aeon { namespace lexing {

	struct channel_t;
	struct multichannel_t;

	struct channel_t
	{
		channel_t();
		explicit channel_t(uint);
		channel_t(multichannel_t const&);

		auto as_int() const -> uint { return bits_; }

	private:
		uint bits_;

		friend struct multichannel_t;
	};




	struct multichannel_t
	{
		static multichannel_t const all;

		multichannel_t();
		explicit multichannel_t(uint chs);
		multichannel_t(channel_t const& ch);

		auto as_int() const -> uint { return bits_; }

	private:
		uint bits_;

		friend struct channel_t;
	};



	// channel implementation
	inline channel_t::channel_t()
		: bits_()
	{}

	inline channel_t::channel_t(uint ch)
		: bits_(1 << ch)
	{}

	inline channel_t::channel_t(multichannel_t const& rhs)
		: bits_(rhs.bits_)
	{
	}




	// multichannel implementation
	inline multichannel_t::multichannel_t()
		: bits_()
	{}

	inline multichannel_t::multichannel_t(uint chs)
		: bits_(chs)
	{}


	inline multichannel_t::multichannel_t(channel_t const& ch)
		: bits_(ch.bits_)
	{}




	inline auto operator == (channel_t const& lhs, channel_t const& rhs) -> bool {
		return lhs.as_int() == rhs.as_int();
	}

	inline auto operator & (channel_t const& lhs, channel_t const& rhs) -> bool {
		return (lhs.as_int() & rhs.as_int()) != 0;
	}

	inline auto operator | (channel_t const& lhs, channel_t const& rhs) -> multichannel_t {
		return multichannel_t(lhs.as_int() | rhs.as_int());
	}
	
	inline auto operator & (multichannel_t const& lhs, channel_t const& rhs) -> multichannel_t {
		return multichannel_t(lhs.as_int() & rhs.as_int());
	}

} }
