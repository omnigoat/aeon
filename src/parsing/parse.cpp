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
	match_t(aeon::lexing::lexemes_t::const_iterator const& i, aeon::lexing::multichannel_t const& ch)
		: i_(i), ch_(ch), good_(true) {}

	match_t& operator ()(lexid id, char const* token = nullptr)
	{
		if (good_ == false)
			return *this;

		if ((ch_ & i_->channel()) && (i_->id() != id || (id == lexid::punctuation && i_->text() != std::string(token))))
			good_ = false;
		else
			++i_;

		return *this;
	}

	operator bool() const { return good_; }

private:
	aeon::lexing::lexemes_t::const_iterator i_;
	aeon::lexing::multichannel_t ch_;

	bool good_;
};

auto aeon::parsing::detail::function(parsemes_t& parsemes, lexing::lexemes_t const& lexemes, detail::context_t& context) -> bool
{
	if (context.begin->id() == lexid::function_keyword)
	{
		// function keyword
		parseme_ptr fn_node(new parseme_t(parsid::function, &*context.begin++));

		// function name
		if (context.begin->id() != lexid::identifier) unexpected();
		parseme_ptr id_node(new parseme_t(parsid::function_name, &*context.begin++));

		// parameter list
		if (context.begin->id() != lexid::punctuation || context.begin->text() != ":")
			unexpected();

		//context.begin.
		match_t(context.begin, lexing::basic)(lexid::punctuation, ":")(lexid::punctuation, ":");
		//parseme_ptr params  = new parseme_t(parsid::parameter_list,
		
		//fn_node->
		//fn_node->children().push_back(

		parsemes.push_back(fn_node);
	}

	return true;
}