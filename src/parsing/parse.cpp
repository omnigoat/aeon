#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/lexing/id.hpp>

using namespace aeon::parsing;

typedef aeon::lexing::ID lexid;
typedef aeon::parsing::parseme_t::id_t parsid;

auto aeon::parsing::parse(parsemes_t& parsemes, lexing::lexemes_t const& lexemes) -> void
{
	detail::context_t context;
	context.begin = lexemes.begin_of(lexing::basic);
	detail::module(parsemes, lexemes, context);
}

auto aeon::parsing::detail::module(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	while (function(parsemes, lexemes, context))
		;

	return true;
}

auto aeon::parsing::detail::function(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	if (context.begin->id() == lexid::function_keyword)
	{
		parseme_ptr fn_node = new parseme_t(parsid::function, &*context.begin++);

		// function name
		if (context.begin->id() != lexid::identifier) unexpected();
		parseme_ptr id_node = new parseme_t(parsid::function_name, &*context.begin++);

		// parameter list
		if (context.begin->id() != lexid::punctuation && context.begin->text() == ":")
		//parseme_ptr params  = new parseme_t(parsid::parameter_list,
		
		fn_node->
		//fn_node->children().push_back(

		parsemes.push_back(fn_node);
	}

	return true;
}