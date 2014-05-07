#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/lexing/id.hpp>
#include <aeon/parsing/xpi.hpp>
#include <string>
#include <stack>

namespace parsing = aeon::parsing;
namespace xpi = aeon::parsing::xpi;

typedef aeon::lexing::ID lexid;
typedef aeon::parsing::parseme_t::id_t parsid;


char const* intrinsic_text = "@int16@int32";

aeon::lexing::position_t intrinsic_position(0, 0, 0);


auto aeon::parsing::errors_t::unexpected(lexing::lexeme_t const* L) -> void
{
	auto id = L->id();
	if (lexing::ID::keyword_lower_bound < id && id < lexing::ID::keyword_upper_bound) {
		errs_.push_back({filename_, L->position(), atma::string("unexpected keyword ") + L->text()});
		//errs_.push_back({filename_, L->position(), text_ + L->position().total, text_ + L->pos})
	}
	else
		errs_.push_back({filename_, L->position(), "unexpected!"});
}

auto aeon::parsing::operator << (std::ostream& stream, errors_t const& rhs) -> std::ostream&
{
	for (auto const& x : rhs.errs_)
		stream << x.file << "(" << x.position.row << "): " << x.message << std::endl;
	return stream;
}


auto aeon::parsing::parse(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes) -> void
{
	parseme_ptr root_node(new parseme_t(parsid::root));

	detail::context_t context(root_node, lexemes.begin(lexing::basic), lexemes.end(lexing::basic));


	//add_prelude(root_node->children());

	detail::module(errors, root_node->children(), lexemes, context);
	parsemes.push_back(root_node);
}

auto aeon::parsing::detail::module(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	parseme_ptr module_node(new parseme_t(parsid::module));

	parsemes.push_back(module_node);

	while (function(errors, module_node->children(), lexemes, context))
		;

	return true;
}

auto aeon::parsing::detail::function(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	parseme_ptr fn_node, id_node, parameter_list_node, function_body_node;

	// function keyword
	{
		fn_node = context.match_make(parsid::function, lexid::function_keyword);
		if (!fn_node)
			goto fail;
	}

	// name expression
	{
		auto pattern = parseme_t::make(parsid::function_pattern, context.current_lexeme());

		for (;;)
		{
			id_node = context.match_make(parsid::identifier, lexid::identifier);
			if (!id_node)
				id_node = context.match_make(parsid::placeholder, lexid::punctuation, "_");

			if (!id_node)
				break;

			pattern->children().push_back(id_node);
		}

		if (pattern->children().empty())
			goto fail;

		fn_node->children().push_back(pattern);
	}

	// parameter list
	{
		parameter_list_node = context.match_make(parsid::parameter_list, lexid::punctuation, "::");
		if (!parameter_list_node)
			goto fail;
		
		if (!parameters(errors, parameter_list_node->children(), lexemes, context))
			goto fail;

		fn_node->children().push_back(parameter_list_node);

		auto r = parameter_list_node->children().back();
		parameter_list_node->children().pop_back();
		fn_node->children().push_back(parseme_t::make(parsing::ID::type_name, r->children()[0]->lexeme()));
	}

	// return type
	if (false) {
		if (!context.skip(lexid::punctuation, "->"))
			goto fail;

		if (!type_name(errors, fn_node->children(), lexemes, context))
			goto fail;
	}

	// function body.
	{
		parseme_ptr function_body_node = context.match_make(parsid::block, lexid::block_begin, lexing::all);
		if (function_body_node) {
			context.align(lexing::basic);
			function_body(errors, function_body_node->children(), lexemes, context);
			fn_node->children().push_back(function_body_node);
		}
		else
			goto fail;
	}

	context.skip(lexid::block_end);
	context.skip(lexid::end_keyword);

	// done!
	parsemes.push_back(fn_node);
	return true;

fail:
	return false;
}

auto aeon::parsing::detail::parameters(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	uint parameters = 0;

	for (;;)
	{
		parseme_ptr parameter_node(new parseme_t(parsid::parameter));
		
		// (<identifier> as)? <typename>
		detail::context_t old_context = context;
		{
			parseme_ptr id_node = context.match_make(parsid::identifier, lexid::identifier);
			if (id_node) {
				if (!context.skip(lexid::as_keyword))
					goto fail;

				parameter_node->children().push_back(id_node);
			}

			if (!type_name(errors, parameter_node->children(), lexemes, context)) {
				goto fail;
			}
			
		}

		parsemes.push_back(parameter_node);
		++parameters;

		if ( !context.skip(lexid::punctuation, "->") )
		{
			//context = old_context;
			break;
		}
	}

	return true;

fail:
	// we may have failed because there were no parameters. try
	// skipping the '->' and doing one more
	if (context.skip(lexid::punctuation, "->"))
	{
		auto p = parseme_t::make(parsid::parameter);
		if (type_name(errors, p->children(), lexemes, context)) {
			parsemes.push_back(p);
			return true;
		}
	}
	
	return false;
}

auto aeon::parsing::detail::type_name(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	if (parseme_ptr type_name = context.match_make(parsid::type_name, lexid::type))
	{
		// check for missing intrinsic integer definitions
		if (is_intrinsic_int_typename(type_name)) {
			context.generate_intrinsic_integer_definition(type_name);
		}

		parsemes.push_back(type_name);
		return true;
	}

	return false;
}


auto aeon::parsing::detail::function_body(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	while (statement(errors, parsemes, lexemes, context))
		;

	return true;
}


auto aeon::parsing::detail::statement(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	if (parseme_ptr return_statement_node = context.match_make(parsid::return_statement, lexid::return_keyword)) {
		expression(errors, return_statement_node->children(), lexemes, context);
		parsemes.push_back(return_statement_node);
		return true;
	}

	return false;
}

auto aeon::parsing::detail::expression(errors_t& errors, children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	logical_expression(errors, parsemes, context);

	return true;
}


// multiplicative_expr: Term (*|/ Term)*
auto aeon::parsing::detail::multiplicative_expression(errors_t& errors, children_t& parsemes, context_t& ctx) -> bool
{
	if (!function_call_expression(errors, parsemes, ctx))
		return false;

	// this is fine
	for (;;)
	{
		auto op = ctx.match_make(parsid::addition_expr, lexid::punctuation, "*");
		if (!op)
			op = ctx.match_make(parsid::function_call, lexid::punctuation, "/");

		if (!op)
			return true;

		if (!function_call_expression(errors, parsemes, ctx)) {
			errors.unexpected(ctx.current_lexeme());
			parsemes.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = parsemes.back();
		parsemes.pop_back();
		auto lhs = parsemes.back();
		parsemes.pop_back();

		// just a function-call
		xpi::insert_into(parsemes,
			xpi::make(parsid::function_call, op->lexeme())[
				xpi::make(parsid::function_pattern)[
					xpi::make(parsid::placeholder, lhs->lexeme()),
						xpi::make(parsid::identifier, op->lexeme()),
						xpi::make(parsid::placeholder, rhs->lexeme())
				],

				xpi::make(parsid::argument_list)[
					xpi::insert(lhs),
					xpi::insert(rhs)
				]
			]);
	}

	return true;
}

auto aeon::parsing::detail::logical_expression(errors_t& errors, children_t& xs, context_t& ctx) -> bool
{
	if (!additive_expression(errors, xs, ctx))
		return false;

	for (;;)
	{
		auto op_fn = ctx.match_make(parsid::function_call, lexid::punctuation, "==");
		if (!op_fn)
			op_fn = ctx.match_make(parsid::function_call, lexid::punctuation, "!=");

		if (!op_fn)
			return true;

		if (!additive_expression(errors, xs, ctx)) {
			errors.unexpected(ctx.current_lexeme());
			xs.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = xs.back();
		xs.pop_back();
		auto lhs = xs.back();
		xs.pop_back();

		// just a function-call
		xpi::insert_into(xs,
			xpi::insert(op_fn) [
				xpi::make(parsid::function_pattern) [
					xpi::make(parsid::placeholder, lhs->lexeme()),
					xpi::make(parsid::identifier, op_fn->lexeme()),
					xpi::make(parsid::placeholder, rhs->lexeme())
				],

				xpi::make(parsid::argument_list)[
					xpi::insert(lhs),
					xpi::insert(rhs)
				]
			]);
	}

	return true;
}

// additive_expr: Term (+|- Term)*
auto aeon::parsing::detail::additive_expression(errors_t& errors, children_t& parsemes, context_t& ctx) -> bool
{
	if (!multiplicative_expression(errors, parsemes, ctx))
		return false;

	// this is fine
	for (;;)
	{
		auto op = ctx.match_make(parsid::addition_expr, lexid::punctuation, "+");
		if (!op)
			op = ctx.match_make(parsid::function_call, lexid::punctuation, "-");

		if (!op)
			return true;

		if (!multiplicative_expression(errors, parsemes, ctx)) {
			errors.unexpected(ctx.current_lexeme());
			parsemes.push_back(parseme_t::make(ID::error_term));
		}

		auto rhs = parsemes.back();
		parsemes.pop_back();
		auto lhs = parsemes.back();
		parsemes.pop_back();

		// just a function-call
		xpi::insert_into(parsemes,
			xpi::make(parsid::function_call, op->lexeme()) [
				xpi::make(parsid::function_pattern) [
					xpi::make(parsid::placeholder, lhs->lexeme()),
					xpi::make(parsid::identifier, op->lexeme()),
					xpi::make(parsid::placeholder, rhs->lexeme())
				],

				xpi::make(parsid::argument_list) [
					xpi::insert(lhs),
					xpi::insert(rhs)
				]
			]);
	}

	return true;
}

auto aeon::parsing::detail::function_call_expression(errors_t& errors, children_t& parsemes, context_t& ctx) -> bool
{
	auto iden = ctx.match_make(parsid::identifier, lexid::identifier);
	if (!iden) {
		if (auto intlit = ctx.match_make(parsid::integer_literal, lexid::integer_literal)) {
			parsemes.push_back(intlit);
			return true;
		}

		return false;
	}

	// bam, function call
	if (ctx.skip(lexid::punctuation, "("))
	{
		parsing::children_t args;

		for (;;)
		{
			if (!additive_expression(errors, args, ctx))
				break;
			ctx.skip(lexid::punctuation, ",");
		}

		ctx.skip(lexid::punctuation, ")");

		xpi::insert_into(parsemes,
			xpi::make(parsid::function_call, iden->lexeme()) [
				xpi::make(parsid::function_pattern) [
					xpi::insert(iden)
				],

				xpi::make(parsid::argument_list) [
					xpi::insert(args.begin(), args.end())
				]
			]);
	}
	else
	{
		parsemes.push_back(iden);
	}

	return true;
}
