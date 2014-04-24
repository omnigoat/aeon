//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_LEXING_LEXING_STREAM_HPP
#define AEON_LEXING_LEXING_STREAM_HPP
//=====================================================================
#include <aeon/lexing/lexeme.hpp>
//=====================================================================
namespace aeon {
namespace lexing {
//=====================================================================
	
	struct stream_t
	{
		stream_t(char const* begin, char const* end);

		auto begin() const -> char const*;
		auto end() const -> char const*;
		auto current() const -> char const*;
		auto cv() const -> char;
		auto valid() const -> bool;
		auto position() const -> position_t const&;
		auto marked_position() const -> position_t const&;
		auto increment() -> void;
		auto mark() -> char const*;
		auto reset(char const*) -> void;
		
	private:
		char const* begin_;
		char const* end_;
		char const* current_;
		char const* mark_;
		bool consumed_newline_;

		position_t position_;
		position_t marked_position_;
	};
	
//=====================================================================
} // namespace lexing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

