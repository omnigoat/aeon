//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_LEXEME_HPP
#define JIGL_LEXING_LEXEME_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================
	
	//=====================================================================
	// position in file
	//=====================================================================
	struct position_t
	{
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
		channel_t() : bits_(1) {}
		explicit channel_t(uint32_t ch) : bits_(1 | (1 << ch)) {}

		auto as_int() const -> uint32_t { return bits_; }

		auto operator == (channel_t const& rhs) const -> bool {
			return bits_ == rhs.bits_;
		}

	private:
		uint32_t bits_;
	};


	//=====================================================================
	// lexeme
	//=====================================================================
	struct lexeme_t
	{
		typedef size_t id_t;
		static std::string empty_text;
		
		lexeme_t(id_t, char const* begin, char const* end, position_t const&, channel_t const& = channel_t());

		auto id() const -> id_t const&;
		auto position() const -> position_t const&;
		auto channel() const -> channel_t const&;
		auto begin() const -> char const*;
		auto end() const -> char const*;
		auto text() const -> std::string;
		
	private:
		id_t id_;
		char const* begin_;
		char const* end_;
		position_t position_;
		channel_t channel_;
	};
	
	inline std::ostream& operator << (std::ostream& stream, lexeme_t const& L) {
		return stream << L.id() << "[" << L.position().row << ":" << L.position().column << "]: " << L.text().c_str();
	}
	
	
	
//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

