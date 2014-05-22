#pragma once

#include <aeon/lexing/lexical_analysis.hpp>
#include <aeon/parsing/parseme.hpp>

namespace aeon { namespace parsing {
	
	struct syntactic_analysis_t;

	struct error_t
	{
		error_t(syntactic_analysis_t const* sa, lexing::lexeme_t const* begin, lexing::lexeme_t const* end)
		: analysis_(sa), begin(begin), end(end)
		{}

		struct message_t
		{
			atma::string msg;
			lexing::lexeme_t const* begin;
			lexing::lexeme_t const* end;
		};

		syntactic_analysis_t const* analysis_;

		// range of lexemes spanning the problem. this is used
		// to reconstruct the line of source text
		lexing::lexeme_t const* begin;
		lexing::lexeme_t const* end;

		// messages which may highlight various parts of the line
		std::vector<message_t> messages;
	};

	auto operator << (std::ostream&, error_t const&) -> std::ostream&;


	struct syntactic_analysis_t
	{
		typedef std::vector<error_t> errors_t;
		
		syntactic_analysis_t(lexing::lexical_analysis_t&);

		auto parsemes() -> children_t&;
		auto errors() const -> errors_t const&;

	private:
		// incoming lexical analysis
		auto lxa_peek() -> lexing::lexeme_t const*;
		auto lxa_mk_if(parsing::ID, lexing::ID) -> parseme_ptr;
		auto lxa_mk_if(parsing::ID, lexing::ID, char const*, uint) -> parseme_ptr;
		auto lxa_skip(lexing::ID) -> bool;
		auto lxa_skip(lexing::ID, char const*, uint) -> bool;

		// parsing
		auto parse_module(children_t&) -> bool;
		auto parse_function(children_t&) -> bool;
		auto parse_parameters(children_t&) -> bool;
		auto parse_typename(children_t&) -> bool;
		auto parse_function_body(children_t&) -> bool;
		auto parse_statement(children_t&) -> bool;
		auto parse_expr(children_t&) -> bool;
		auto parse_expr_logical(children_t&) -> bool;
		auto parse_expr_additive(children_t&) -> bool;
		auto parse_expr_multiplicative(children_t&) -> bool;
		auto parse_expr_function_call(children_t&) -> bool;

		// prelude generation
		auto generate_prelude() -> bool;
		auto generate_int_type(uint bitsize) -> bool;
		auto generate_void_type() -> bool;
		auto generate_bool_type() -> bool;

		// error handling
		auto error_unexpected(lexing::lexeme_t const*) -> void;

	private:
		lexing::lexical_analysis_t& lxa_;
		lexing::lexemes_t::const_iterator lxa_iter_;

		children_t parsemes_;

		std::vector<error_t> errors_;
	};

} }
