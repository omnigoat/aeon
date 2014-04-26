#pragma once
//=====================================================================
#include <aeon/parsing/children.hpp>

#include <cstdint>
#include <ostream>
#include <map>
//=====================================================================
namespace aeon { namespace semantics {

	struct error_t
	{
		error_t(std::string const& text) : text(text) {}
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

} }
