#include <aeon/parsing/syntactic_analysis.hpp>

#include <aeon/lexing/id.hpp>


using namespace aeon;
using namespace aeon::parsing;
using aeon::lexing::lexical_analysis_t;
using aeon::parsing::syntactic_analysis_t;


syntactic_analysis_t::syntactic_analysis_t(lexical_analysis_t& lexical_analysis)
	: lxa_(lexical_analysis), lxa_iter_(lexical_analysis.lexemes().begin(lexing::basic))
{
}

auto syntactic_analysis_t::parsemes() -> parsing::children_t&
{
	return parsemes_;
}

auto syntactic_analysis_t::lxa_peek() -> lexing::lexeme_t const*
{
	ATMA_ASSERT(lxa_iter_ != lxa_.lexemes().end());
	return &*lxa_iter_;
}

auto syntactic_analysis_t::lxa_mk_if(parsing::ID psid, lexing::ID lxid) -> parseme_ptr
{
	auto L = lxa_peek();
	if (L->id() == lxid) {
		++lxa_iter_;
		return parsing::parseme_t::make(psid, L);
	}

	return parsing::null_parseme_ptr;
}

auto syntactic_analysis_t::lxa_mk_if(parsing::ID psid, lexing::ID lxid, char const* text, uint textlen) -> parseme_ptr
{
	auto L = lxa_peek();
	if (L->id() == lxid && lxa_peek()->streq(text, text + textlen)) {
		++lxa_iter_;
		return parsing::parseme_t::make(psid, L);
	}

	return parsing::null_parseme_ptr;
}

auto syntactic_analysis_t::lxa_skip(lexing::ID lxid) -> bool
{
	if (lxa_peek()->id() == lxid) {
		++lxa_iter_;
		return true;
	}

	return false;
}

auto syntactic_analysis_t::lxa_skip(lexing::ID lxid, char const* text, uint textlen) -> bool
{
	if (lxa_peek()->id() == lxid && lxa_peek()->streq(text, text + textlen)) {
		++lxa_iter_;
		return true;
	}

	return false;
}

auto syntactic_analysis_t::parse_module(children_t& dest) -> bool
{
	parse_function(dest);
	return true;
}

auto syntactic_analysis_t::parse_function(children_t& dest) -> bool
{
	parseme_ptr id_node, parameter_list_node, function_body_node;

	typedef parsing::ID psid;
	typedef lexing::ID lxid;

	// function keyword
	auto fn_node = lxa_mk_if(psid::function, lxid::function_keyword);
	if (!fn_node)
		return false;

	// name expression
	{
		auto name = lxa_mk_if(psid::identifier, lxid::identifier);
		if (!name)
			name = lxa_mk_if(psid::placeholder, lxid::punctuation, "_", 1);

		fn_node->children().push_back(name);
	}

	// parameter list
	{
		parameter_list_node = lxa_mk_if(psid::parameter_list, lxid::punctuation, "::", 2);
		if (!parameter_list_node)
			return false;

		if (!parse_parameters(parameter_list_node->children()))
			return false;

		fn_node->children().push_back(parameter_list_node);

#if 0
		auto r = parameter_list_node->children().back();
		parameter_list_node->children().pop_back();
		fn_node->children().push_back(parseme_t::make(psid::type_name, r->children()[0]->lexeme()));
#endif
	}

	// return type
#if 1
	{
		if (!lxa_skip(lxid::punctuation, "->", 2))
			return false;

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
	dest.push_back(fn_node);
	return true;
}

auto syntactic_analysis_t::parse_parameters(children_t&) -> bool
{
	return true;
}

auto syntactic_analysis_t::parse_typename(children_t&) -> bool
{
	return true;
}

auto syntactic_analysis_t::parse_function_body(children_t&) -> bool
{
	return true;
}

