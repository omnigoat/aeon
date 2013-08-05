#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/lexing/id.hpp>
#include <aeon/parsing/ape.hpp>
#include <string>

namespace parsing = aeon::parsing;
namespace ape = aeon::parsing::ape;

typedef aeon::lexing::ID lexid;
typedef aeon::parsing::parseme_t::id_t parsid;


char const* intrinsic_text = "@int16@int32";

aeon::lexing::position_t intrinsic_position(0, 0);
aeon::lexing::lexeme_t int16_lexeme(aeon::lexing::ID::intrinsic_type_int16, intrinsic_text, intrinsic_text + 6, intrinsic_position);
aeon::lexing::lexeme_t int32_lexeme(aeon::lexing::ID::intrinsic_type_int32, intrinsic_text + 6, intrinsic_text + 12, intrinsic_position);


auto add_prelude(parsing::children_t& parsemes) -> void
{
	typedef parsing::parseme_t::id_t id;

	parsing::children_t xs;

	ape::insert_into(xs, (
		ape::make(id::type_definition) [
			ape::make(id::identifier, int16_lexeme),
			ape::make(id::intrinsic_type_int16)
		],

		ape::make(id::type_definition) [
			ape::make(id::identifier, int32_lexeme),
			ape::make(id::intrinsic_type_int32)
		]
	));

	parsemes.insert(parsemes.begin(), xs.begin(), xs.end());
}





auto aeon::parsing::parse(children_t& parsemes, lexing::lexemes_t const& lexemes) -> void
{
	detail::context_t context(lexemes.begin(lexing::basic));

	parseme_ptr root_node(new parseme_t(parsid::root));

	add_prelude(root_node->children());

	detail::module(root_node->children(), lexemes, context);
	parsemes.push_back(root_node);
}

auto aeon::parsing::detail::module(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	parseme_ptr module_node(new parseme_t(parsid::module));

	parsemes.push_back(module_node);

	while (function(module_node->children(), lexemes, context))
		;

	return true;
}

struct match_t
{
	match_t(aeon::lexing::lexemes_t::const_iterator& i, aeon::lexing::multichannel_t const& ch)
		: orig_(i), i_(i), ch_(ch), good_(true)
	{
		i_.set_channel(ch);
	}

	match_t& operator ()(lexid id, char const* token = nullptr)
	{
		if (good_ == false)
			return *this;

		if ((ch_ & i_->channel()) && (i_->id() != id || (id == lexid::punctuation && !i_->streq(token)))) {
			i_ = orig_;
			good_ = false;
		}
		else
			++i_;

		return *this;
	}

	operator bool() const { return good_; }

private:
	aeon::lexing::lexemes_t::const_iterator orig_;
	aeon::lexing::lexemes_t::const_iterator& i_;
	aeon::lexing::multichannel_t ch_;

	bool good_;
};

match_t match(aeon::lexing::lexemes_t::const_iterator& i, aeon::lexing::multichannel_t const& ch) {
	return match_t(i, ch);
}

auto aeon::parsing::detail::function(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	parseme_ptr fn_node, id_node, parameter_list_node, function_body_node;

	// function keyword
	{
		fn_node = context.match_make(parsid::function, lexid::function_keyword);
		if (!fn_node)
			goto fail;
	}

	// identifier
	{
		id_node = context.match_make(parsid::identifier, lexid::identifier);
		if (!id_node)
			goto fail;
		fn_node->children().push_back(id_node);
	}

	// parameter list
	{
		parameter_list_node = context.match_make(parsid::parameter_list, lexid::punctuation, "::");
		if (!parameter_list_node)
			goto fail;
		
		if (!parameters(parameter_list_node->children(), lexemes, context))
			goto fail;

		parseme_ptr return_type_node = context.match_make(parsid::type_name, lexid::type);
		if (!return_type_node)
			goto fail;

		fn_node->children().push_back(parameter_list_node);
		fn_node->children().push_back(return_type_node);
	}

	// function body.
	{
		parseme_ptr function_body_node = context.match_make(parsid::block, lexid::block_begin, lexing::all);
		if (function_body_node) {
			context.align(lexing::basic);
			function_body(function_body_node->children(), lexemes, context);
			fn_node->children().push_back(function_body_node);
		}
	}


	// done!
	parsemes.push_back(fn_node);
	return true;

fail:
	return false;
}

auto aeon::parsing::detail::parameters(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	for (;;)
	{
		parseme_ptr parameter_node(new parseme_t(parsid::parameter));
		
		// <optional identifier> as <typename>
		detail::context_t old_context = context;
		{
			parseme_ptr id_node = context.match_make(parsid::identifier, lexid::identifier);
			if (id_node) {
				if (!context.skip(lexid::as_keyword))
					goto fail;

				parameter_node->children().push_back(id_node);
			}

			parseme_ptr param_type_node = context.match_make(parsid::type_name, lexid::type);
			if (!param_type_node)
				goto fail;

			parameter_node->children().push_back(param_type_node);
		}

		if ( !context.skip(lexid::punctuation, "->") ) //!match(context.begin, lexing::basic)(lexid::punctuation, "->") )
		{
			context = old_context;
			break;
		}

		parsemes.push_back(parameter_node);
	}

	return true;

fail:
	return false;
}

auto aeon::parsing::detail::function_body(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	while (statement(parsemes, lexemes, context))
		;

	return true;
}


auto aeon::parsing::detail::statement(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	if (parseme_ptr return_statement_node = context.match_make(parsid::return_statement, lexid::return_keyword)) {
		expression(return_statement_node->children(), lexemes, context);
		parsemes.push_back(return_statement_node);
		return true;
	}

	return false;
}

auto aeon::parsing::detail::expression(children_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	return additive_expression(parsemes, context);
}

auto aeon::parsing::detail::additive_expression(children_t& parsemes, detail::context_t& context) -> bool
{
	parseme_ptr lhs = context.match_make(parsid::identifier, lexid::identifier);
	bool plus = context.skip(lexid::punctuation, "+");
	parseme_ptr rhs = context.match_make(parsid::identifier, lexid::identifier);
	if (lhs && plus && rhs) {
		parseme_ptr k(new parseme_t(parsid::addition_expr));
		k->children().push_back(lhs);
		k->children().push_back(rhs);
		parsemes.push_back(k);
		return true;
	}

	return false;
}