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

		auto expr_id(parsing::parseme_ptr const&) const -> uint;

		auto push_function(parsing::parseme_ptr const&) -> void;
		auto pop_function() -> void;
		auto mark_expr(parsing::parseme_ptr const&) -> void;

	private:
		// current function
		parsing::parseme_ptr fn_;

		// map from expr -> id
		std::map<parsing::parseme_ptr, uint> tmp_;
		// map from fn -> used-ids
		std::map<parsing::parseme_ptr, uint> tmpcount_;
	};

	namespace analyse
	{
		auto module(genesis_t&, parsing::parseme_ptr const&) -> void;
		auto function(genesis_t&, parsing::parseme_ptr const&) -> void;
		auto statement(genesis_t&, parsing::parseme_ptr const&) -> void;
		auto expression(genesis_t&, parsing::parseme_ptr const&) -> void;
		auto intrinsic(genesis_t&, parsing::parseme_ptr const&) -> void;
	}

} }