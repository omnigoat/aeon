//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_SEMANTICS_ANALYSE_HPP
#define AEON_SEMANTICS_ANALYSE_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
//=====================================================================
#include <aeon/parsing/children.hpp>
//=====================================================================
namespace aeon {
namespace semantics {
//=====================================================================
	
	struct error_t
	{
		std::string text;
	};

	struct analysis_t
	{
		typedef std::vector<error_t> errors_t;

		errors_t errors;
	};

	namespace analyse
	{
		using parsing::parseme_t;
		using parsing::parseme_ptr;
		using parsing::children_t;

		auto function(parseme_ptr const&) -> analysis_t;
	}

//=====================================================================
} // namespace semantics
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

