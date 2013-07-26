#include <aeon/parsing/context.hpp>

using namespace aeon::parsing;
using aeon::parsing::detail::context_t;

typedef aeon::lexing::ID lexid;
typedef parseme_t::id_t parsid;


context_t::context_t(aeon::lexing::lexemes_t::const_iterator const& begin)
	: begin_(begin)
{
}

auto context_t::id() const -> aeon::lexing::ID
{
	return begin_->id();
}

auto context_t::match_make(parsid pid, lexid lid) -> parseme_ptr
{
	parseme_ptr result;
	
	if (begin_->id() == lid) {
		result.reset(new parseme_t(pid, &*begin_));
		++begin_;
	}

	return result;
}

auto context_t::match_make(parsid pid, lexid lid, char const* str) -> parseme_ptr
{
	parseme_ptr result;
	
	if (begin_->id() == lid && begin_->streq(str)) {
		result.reset(new parseme_t(pid, &*begin_));
		++begin_;
	}

	return result;
}

auto context_t::skip(lexid lid) -> bool
{
	if (begin_->id() != lid)
		return false;
	++begin_;
	return true;
}

auto context_t::skip(lexid lid, char const* str) -> bool
{
	if (begin_->id() != lid || !begin_->streq(str))
		return false;
	++begin_;
	return true;
}
