#pragma once
//=====================================================================
#include <aeon/lexing/lexemes.hpp>
#include <aeon/parsing/children.hpp>
#include <aeon/parsing/context.hpp>

#include <atma/string.hpp>

#include <cstdint>
#include <ostream>
//=====================================================================
namespace aeon { namespace parsing {

#if 0
	struct error_t
	{
		error_t(atma::string const& f, lexing::position_t const& p, atma::string const& m)
			: file(f), position(p), message(m)
		{}

		atma::string file;
		lexing::position_t position;
		atma::string message;
	};

	//typedef std::vector<error_t> errors_t;
	
	struct errors_t
	{
		errors_t(atma::string const& filename, char const* text)
			: filename_(filename), text_(text)
		{}

		auto size() const -> size_t { return errs_.size(); }

		auto unexpected(parseme_t const&) -> void;
		auto unexpected(lexing::lexeme_t const*) -> void;

	private:
		atma::string filename_;
		char const* text_;
		std::vector<error_t> errs_;

		friend auto operator << (std::ostream&, errors_t const&) -> std::ostream&;
	};

	auto operator << (std::ostream&, errors_t const&) -> std::ostream&;
#endif
	typedef std::vector<int> errors_t;

	namespace detail
	{
		struct context_t;

		auto module(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto type_name(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto parameters(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto function_body(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto statement(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto expression(errors_t&, children_t&, lexing::lexemes_t const&, context_t&) -> bool;
		auto multiplicative_expression(errors_t&, children_t&, context_t&) -> bool;
		auto additive_expression(errors_t&, children_t&, context_t&) -> bool;
		auto logical_expression(errors_t&, children_t&, context_t&) -> bool;
		auto function_call_expression(errors_t&, children_t&, context_t&) -> bool;
	}

	
	auto parse(errors_t&, children_t&, lexing::lexemes_t const&) -> void;

} }
