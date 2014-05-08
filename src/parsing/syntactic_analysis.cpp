#include <aeon/parsing/syntactic_analysis.hpp>

using namespace aeon;
using namespace aeon::parsing;
using aeon::lexing::lexical_analysis_t;
using aeon::parsing::syntactic_analysis_t;

syntactic_analysis_t::syntactic_analysis_t(lexical_analysis_t& lexical_analysis)
	: lxa_(lexical_analysis)
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

auto syntactic_analysis_t::lxa_match_mk(parsing::ID psid, lexing::ID lxid) -> parseme_ptr
{
	auto L = lxa_peek();
	if (L->id() == lxid) {
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
