//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_GENERATION_HPP
#define AEON_GENERATION_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
#include <atma/string.hpp>
//=====================================================================
namespace aeon {
namespace generation {
//=====================================================================
	
	
	auto function_name_mangle(parsing::parseme_ptr const&) -> atma::string;
	auto type_name_mangle(parsing::parseme_ptr const&) -> atma::string;

	auto module(parsing::parseme_ptr const&) -> void;

//=====================================================================
} // namespace generation
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

