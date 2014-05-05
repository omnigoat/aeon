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
			: row(), column(), total()
		{}

		position_t(uint row, uint column, uint total)
			: row(row), column(column), total(total)
		{
		}

		uint row, column, total;
	};

} }
