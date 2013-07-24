#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/parseme.hpp>
#include <aeon/lexing/id.hpp>
#include <string>

using namespace aeon::parsing;

typedef aeon::lexing::ID lexid;
typedef aeon::parsing::parseme_t::id_t parsid;

auto aeon::parsing::parse(parsemes_t& parsemes, lexing::lexemes_t const& lexemes) -> void
{
	detail::context_t context;
	context.begin = lexemes.begin(lexing::basic);
	detail::module(parsemes, lexemes, context);
}

auto aeon::parsing::detail::module(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	while (function(parsemes, lexemes, context))
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

		if ((ch_ & i_->channel()) && (i_->id() != id || (id == lexid::punctuation && i_->text() != std::string(token)))) {
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

auto aeon::parsing::detail::function(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	parseme_ptr fn_node, id_node, parameter_list_node;

	if (context.begin->id() != lexid::function_keyword)
		goto fail;
	
	// function keyword
	{
		fn_node.reset(new parseme_t(parsid::function, &*context.begin++));
	}

	// identifier
	{
		if (context.begin->id() != lexid::identifier)
			goto fail;
		id_node.reset(new parseme_t(parsid::function_name, &*context.begin++));
		fn_node->children().push_back(id_node);
	}

	// parameter list
	{
		if (!match(context.begin, lexing::all)(lexid::punctuation, ":")(lexid::punctuation, ":"))
			goto fail;
		parameter_list_node.reset(new parseme_t(parsid::parameter_list));
	
		detail::context_t ctx = { context.begin };
		if (!parameters(parameter_list_node->children(), lexemes, ctx))
			goto fail;
	}

	// function body.
	{
	}

	goto done;

fail:
	
	return false;

done:
	parsemes.push_back(fn_node);
	return true;
}

auto aeon::parsing::detail::parameters(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	return true;
}

