#include <aeon/parsing/syntactic_analysis.hpp>

#include <aeon/lexing/id.hpp>
#include <aeon/parsing/xpi.hpp>

using namespace aeon;
using namespace aeon::parsing;
using aeon::lexing::lexical_analysis_t;
using aeon::parsing::syntactic_analysis_t;
namespace xpi = aeon::parsing::xpi;

typedef parsing::ID psid;
typedef lexing::ID lxid;


auto aeon::parsing::operator << (std::ostream& stream, error_t const& error) -> std::ostream&
{
	// get line from analysis
	stream << "<filename>" << error.begin->position().row << ": " << std::endl;
	
	for (auto const& m : error.messages)
		stream << '\t' << m.msg << std::endl;

	return stream;
}




syntactic_analysis_t::syntactic_analysis_t(lexical_analysis_t& lexical_analysis)
	: lxa_(lexical_analysis), lxa_iter_(lexical_analysis.lexemes().begin(lexing::basic))
{
	parse_module(parsemes_);
	generate_prelude();
}

auto syntactic_analysis_t::parsemes() -> parsing::children_t&
{
	return parsemes_;
}

auto syntactic_analysis_t::errors() const -> errors_t const&
{
	return errors_;
}

auto syntactic_analysis_t::lxa_peek() -> lexing::lexeme_t const*
{
	auto E = lxa_.lexemes().end(lxa_iter_.channel());
	if (lxa_iter_ == E)
		return nullptr;

	return &*lxa_iter_;
}

auto syntactic_analysis_t::lxa_mk_if(psid Pid, lxid Lid) -> parseme_ptr
{
	auto L = lxa_peek();
	if (L != nullptr && L->id() == Lid) {
		++lxa_iter_;
		return parsing::parseme_t::make(Pid, L);
	}

	return parsing::null_parseme_ptr;
}

auto syntactic_analysis_t::lxa_mk_if(psid Pid, lxid Lid, char const* text, uint textlen) -> parseme_ptr
{
	auto L = lxa_peek();
	if (L && L->id() == Lid && lxa_peek()->streq(text, text + textlen)) {
		++lxa_iter_;
		return parsing::parseme_t::make(Pid, L);
	}

	return parsing::null_parseme_ptr;
}

auto syntactic_analysis_t::lxa_skip(lxid Lid) -> bool
{
	auto L = lxa_peek();
	if (L && L->id() == Lid) {
		++lxa_iter_;
		return true;
	}

	return false;
}

auto syntactic_analysis_t::lxa_skip(lxid Lid, char const* text, uint textlen) -> bool
{
	auto L = lxa_peek();
	if (L && L->id() == Lid && L->streq(text, text + textlen)) {
		++lxa_iter_;
		return true;
	}

	return false;
}

auto syntactic_analysis_t::parse_module(children_t& xs) -> bool
{
	//xs.push_back(parseme_t::make(psid::module));
	xpi::insert_into(xs,
		//xpi::make(psid::root) [
			xpi::make(psid::module)
		//]
		);

	while (parse_function(xs.back()->children()))
		;

	return true;
}

auto syntactic_analysis_t::parse_function(children_t& xs) -> bool
{
	parseme_ptr id_node, parameter_list_node, function_body_node;

	// function keyword
	if (!lxa_skip(lxid::function_keyword))
		return false;

	// name expression
		auto fn_node = lxa_mk_if(psid::function, lxid::identifier);
		if (!fn_node)
			fn_node = lxa_mk_if(psid::function, lxid::punctuation);

	// parameter list
	{
		parameter_list_node = lxa_mk_if(psid::parameter_list, lxid::punctuation, "::", 2);
		if (!parameter_list_node)
			return false;

		if (!parse_parameters(parameter_list_node->children()))
			return false;

		fn_node->children().push_back(parameter_list_node);
	}

	// return type
#if 1
	{
		//if (!lxa_skip(lxid::punctuation, "->", 2))
			//return false;

		if (!parse_typename(fn_node->children()))
			return false;
	}
#endif

	// function body.
	{
		lxa_iter_.set_channel(lexing::all);
		parseme_ptr function_body_node = lxa_mk_if(psid::block, lxid::block_begin);
		lxa_iter_.set_channel(lexing::basic);
		if (!function_body_node)
			return false;

		if (!parse_function_body(function_body_node->children()))
			return false;

		fn_node->children().push_back(function_body_node);

		lxa_iter_.set_channel(lexing::all);
		lxa_skip(lxid::block_end);
		lxa_iter_.set_channel(lexing::basic);
	}

	// end keyword
	lxa_skip(lxid::end_keyword);
	xs.push_back(fn_node);
	return true;
}

auto syntactic_analysis_t::parse_parameters(children_t& xs) -> bool
{
	for (;;)
	{
		auto parameter_node = parseme_t::make(psid::parameter);
		auto L = lxa_iter_;

		// (<identifier> as)? <typename>
		{
			if (auto id_node = lxa_mk_if(psid::identifier, lxid::identifier))
			{
				if (!lxa_skip(lxid::as_keyword))
					goto fail;

				parameter_node->children().push_back(id_node);
			}

			if (!parse_typename(parameter_node->children())) {
				goto fail;
			}
		}

		// if we don't have another arrow, then we just consumed the return-type,
		// so let's backtrack a little and finish up
		if (!lxa_skip(lxid::punctuation, "->", 2)) {
			lxa_iter_ = L;
			break;
		}

		xs.push_back(parameter_node);
	}

	return true;


fail:
#if 1
	// we may have failed because there were no parameters. try
	// skipping the '->' and doing one more
	if (lxa_skip(lxid::punctuation, "->", 2))
	{
		return true;
	}
#endif

	return false;
}

auto syntactic_analysis_t::parse_typename(children_t& xs) -> bool
{
	if (auto type_name = lxa_mk_if(psid::type_name, lxid::type)) {
		xs.push_back(type_name);
		return true;
	}

	return false;
}

auto syntactic_analysis_t::parse_function_body(children_t& xs) -> bool
{
	while (parse_statement(xs))
		;

	return true;
}


auto syntactic_analysis_t::parse_statement(children_t& xs) -> bool
{
	if (auto return_statement_node = lxa_mk_if(psid::return_statement, lxid::return_keyword)) {
		parse_expr(return_statement_node->children());
		xs.push_back(return_statement_node);
		return true;
	}

	return false;
}

auto syntactic_analysis_t::parse_expr(children_t& xs) -> bool
{
	return parse_expr_logical(xs);
}

auto syntactic_analysis_t::parse_expr_logical(children_t& xs) -> bool
{
	if (!parse_expr_additive(xs))
		return false;

	for (;;)
	{
		auto op = lxa_mk_if(psid::function_call, lxid::punctuation, "==", 2);
		if (!op)
			op = lxa_mk_if(psid::function_call, lxid::punctuation, "!=", 2);

		if (!op)
			return true;

		if (!parse_expr_additive(xs)) {
			//errors.unexpected(ctx.current_lexeme());
			xs.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = xs.back();
		xs.pop_back();
		auto lhs = xs.back();
		xs.pop_back();

		// just a function-call
		xpi::insert_into(xs,
			xpi::make(psid::function_call, lhs->lexeme(), rhs->lexeme()) [
				xpi::make(psid::identifier, op->lexeme()),

				xpi::make(psid::argument_list) [
					xpi::insert(lhs),
					xpi::insert(rhs)	
				]
			]);
	}

	return true;
}

auto syntactic_analysis_t::parse_expr_additive(children_t& xs) -> bool
{
	if (!parse_expr_multiplicative(xs))
		return false;

	// this is fine
	for (;;)
	{
		auto op = lxa_mk_if(psid::function_call, lxid::punctuation, "+", 1);
		if (!op)
			op = lxa_mk_if(psid::function_call, lxid::punctuation, "-", 1);

		if (!op)
			return true;

		if (!parse_expr_multiplicative(xs)) {
			//errors.unexpected(ctx.current_lexeme());
			xs.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = xs.back();
		xs.pop_back();
		auto lhs = xs.back();
		xs.pop_back();

		// just a function-call
		xpi::insert_into(xs,
			xpi::make(psid::function_call, lhs->lexeme(), rhs->lexeme()) [
				xpi::make(psid::identifier, op->lexeme()),

				xpi::make(psid::argument_list) [
					xpi::insert(lhs),
					xpi::insert(rhs)	
				]
			]);
	}

	return true;
}

auto syntactic_analysis_t::parse_expr_multiplicative(children_t& xs) -> bool
{
	if (!parse_expr_function_call(xs))
		return false;

	for (;;)
	{
		auto op = lxa_mk_if(psid::function_call, lxid::punctuation, "*", 1);
		if (!op)
			op = lxa_mk_if(psid::function_call, lxid::punctuation, "/", 1);

		if (!op)
			return true;

		if (!parse_expr_function_call(xs)) {
			//errors.unexpected(ctx.current_lexeme());
			xs.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = xs.back();
		xs.pop_back();
		auto lhs = xs.back();
		xs.pop_back();

		// just a function-call
		xpi::insert_into(xs,
			xpi::make(psid::function_call, lhs->lexeme(), rhs->lexeme()) [
				xpi::make(psid::identifier, op->lexeme()),

				xpi::make(psid::argument_list) [
					xpi::insert(lhs),
					xpi::insert(rhs)	
				]
			]);
	}

	return true;
}

auto syntactic_analysis_t::parse_expr_function_call(children_t& xs) -> bool
{
	auto iden = lxa_mk_if(psid::identifier, lxid::identifier);
	if (!iden) {
		if (auto intlit = lxa_mk_if(psid::integer_literal, lxid::integer_literal)) {
			xs.push_back(intlit);
			return true;
		}

		return false;
	}

	// bam, function call
	if (lxa_skip(lxid::punctuation, "(", 1))
	{
		//
		auto alist = parseme_t::make(psid::argument_list);
		//fncall->children().push_back(alist);

		for (;;)
		{
			if (!parse_expr(alist->children()))
				break;
			lxa_skip(lxid::punctuation, ",", 1);
		}

		//lxa_skip(lxid::punctuation, ")", 1);
		auto terminal = lxa_mk_if(psid::identifier, lxid::punctuation, ")", 1);

		auto fncall = parseme_t::make(psid::function_call, iden->lexeme(), terminal->lexeme());
		fncall->children().push_back(iden);
		fncall->children().push_back(alist);

		xs.push_back(fncall);
	}
	else
	{
		xs.push_back(iden);
	}

	return true;
}

auto syntactic_analysis_t::generate_prelude() -> bool
{
	return generate_int_type(32);
}

auto syntactic_analysis_t::generate_int_type(uint bitsize) -> bool
{
	// lexemes used
	auto lx_typename = lxa_.make_aux(lxid::identifier, lexing::position_t::zero, "@int" + atma::to_string(bitsize));
	auto lx_bitsize = lxa_.make_aux(lxid::integer_literal, lexing::position_t::zero, atma::to_string(bitsize));
	auto lx_lhs = lxa_.make_aux(lxid::identifier, lexing::position_t::zero, "lhs");
	auto lx_rhs = lxa_.make_aux(lxid::identifier, lexing::position_t::zero, "rhs");
	auto lx_add = lxa_.make_aux(lxid::punctuation, lexing::position_t::zero, "+");
	auto lx_sub = lxa_.make_aux(lxid::punctuation, lexing::position_t::zero, "-");
	auto lx_mul = lxa_.make_aux(lxid::punctuation, lexing::position_t::zero, "*");
	auto lx_div = lxa_.make_aux(lxid::punctuation, lexing::position_t::zero, "/");

	xpi::insert_into(parsemes_, parsemes_.begin(), (
		xpi::make(ID::type_definition) [
			xpi::make(ID::identifier, lx_typename),
			xpi::make(ID::intrinsic_type_int),
			xpi::make(ID::intrinsic_bitsize, lx_bitsize)
		],

		// addition
		xpi::make(ID::function, lx_add) [
			
			xpi::make(ID::parameter_list) [
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_lhs),
					xpi::make(ID::type_name, lx_typename)
				],
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_rhs),
					xpi::make(ID::type_name, lx_typename)
				]
			],

			xpi::make(ID::type_name, lx_typename),

			xpi::make(ID::block) [
				xpi::make(ID::return_statement)[
					xpi::make(ID::intrinsic_int_add, lx_typename)[
						xpi::make(ID::identifier, lx_lhs),
						xpi::make(ID::identifier, lx_rhs)
					]
				]
			],

			xpi::make(ID::attributes) [
				xpi::make(ID::attribute_forceinline)
			]
		],

		// subtraction
		xpi::make(ID::function, lx_sub) [
			
			xpi::make(ID::parameter_list) [
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_lhs),
					xpi::make(ID::type_name, lx_typename)
				],
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_rhs),
					xpi::make(ID::type_name, lx_typename)
				]
			],

			xpi::make(ID::type_name, lx_typename),

			xpi::make(ID::block) [
				xpi::make(ID::return_statement) [
					xpi::make(ID::intrinsic_int_sub, lx_typename) [
						xpi::make(ID::identifier, lx_lhs),
						xpi::make(ID::identifier, lx_rhs)
					]
				]
			],

			xpi::make(ID::attributes) [
				xpi::make(ID::attribute_forceinline)
			]
		],

		// multiplication
		xpi::make(ID::function, lx_mul) [

			xpi::make(ID::parameter_list) [
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_lhs),
					xpi::make(ID::type_name, lx_typename)
				],
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_rhs),
					xpi::make(ID::type_name, lx_typename)
				]
			],

			xpi::make(ID::type_name, lx_typename),

			xpi::make(ID::block) [
				xpi::make(ID::return_statement) [
					xpi::make(ID::intrinsic_int_mul, lx_typename) [
						xpi::make(ID::identifier, lx_lhs),
						xpi::make(ID::identifier, lx_rhs)
					]
				]
			],

			xpi::make(ID::attributes)[
				xpi::make(ID::attribute_forceinline)
			]
		],

		// division
		xpi::make(ID::function, lx_div) [
			
			xpi::make(ID::parameter_list) [
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_lhs),
					xpi::make(ID::type_name, lx_typename)
				],
				xpi::make(ID::parameter) [
					xpi::make(ID::identifier, lx_rhs),
					xpi::make(ID::type_name, lx_typename)
				]
			],

			xpi::make(ID::type_name, lx_typename),

			xpi::make(ID::block) [
				xpi::make(ID::return_statement) [
					xpi::make(ID::intrinsic_int_div, lx_typename) [
						xpi::make(ID::identifier, lx_lhs),
						xpi::make(ID::identifier, lx_rhs)
					]
				]
			],

			xpi::make(ID::attributes)[
				xpi::make(ID::attribute_forceinline)
			]
		]
	));

	return true;
}

auto syntactic_analysis_t::error_unexpected(lexing::lexeme_t const* L) -> void
{
	auto err = error_t{this, L, nullptr};
	err.messages.push_back({"unexpected", L, nullptr});
}
