#pragma once
//=====================================================================
#include <aeon/parsing/parseme.hpp>
#include <map>
//=====================================================================
namespace aeon { namespace generation
{
	struct genesis_t
	{
		genesis_t();

		auto module(parsing::parseme_ptr const&) -> void;
		auto function(parsing::parseme_ptr const&) -> void;
		auto statement(parsing::parseme_ptr const&) -> void;
		auto expression(parsing::parseme_ptr const&) -> void;
		auto intrinsic(parsing::parseme_ptr const&) -> void;

	private:
		std::map<parsing::parseme_ptr, uint> tmp_;
		uint tmpcount_;
	};

} }