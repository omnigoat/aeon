#pragma once
//=====================================================================
#include <atma/utf/utf8_string_range.hpp>
#include <atma/types.hpp>
//=====================================================================
namespace aeon { namespace lexing {

	//=====================================================================
	// position in file
	//=====================================================================
	struct position_t
	{
		static position_t const zero;

		position_t()
			: row(), column()
		{}

		position_t(uint row, uint column)
			: row(row), column(column)
		{
		}

		uint row, column;
	};

} }
