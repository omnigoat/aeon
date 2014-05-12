#include <aeon/parsing/parseme.hpp>
#include <atma/assert.hpp>
#include <atma/console.hpp>

#include <map>

namespace lexing = aeon::lexing;

using aeon::parsing::parseme_t;
using aeon::parsing::parseme_ptr;
using aeon::parsing::children_t;

parseme_ptr const aeon::parsing::null_parseme_ptr;

parseme_t::parseme_t(id_t id)
	: id_(id), children_(this), lexeme_(), lexeme2_()
{
}

parseme_t::parseme_t(id_t id, lexing::lexeme_t const* lexeme)
	: id_(id), children_(this), lexeme_(lexeme), lexeme2_()
{
}

parseme_t::parseme_t(id_t id, lexing::lexeme_t const* L0, lexing::lexeme_t const* L1)
	: id_(id), children_(this), lexeme_(L0), lexeme2_(L1)
{
}

auto parseme_t::id() const -> id_t
{
	return id_;
}

auto parseme_t::position() const -> lexing::position_t const&
{
	if (lexeme_)
		return lexeme()->position();
	else
		return lexing::position_t::zero;
}

auto parseme_t::text() const -> lexing::lexeme_t::text_t const&
{
	if (lexeme_)
		return lexeme()->text();
	else
		return lexing::lexeme_t::empty_text;
}

auto parseme_t::parent() const -> parseme_ptr
{
	if (parseme_ptr p = parent_.lock())
		return p;
	else
		return parseme_ptr();
}

auto parseme_t::children() const -> children_t const&
{
	return children_;
}

auto parseme_t::children() -> children_t&
{
	return children_;
}

auto parseme_t::set_parent(parseme_ptr const& p) -> void
{
	ATMA_ASSERT(!p || parent_.expired());
	parent_ = p;
}

auto parseme_t::lexeme() const -> lexing::lexeme_t const*
{
	return lexeme_;
}

auto parseme_t::make(id_t id, lexing::lexeme_t const* L) -> parseme_ptr
{
	return parseme_ptr(new parseme_t(id, L));
}

auto parseme_t::make(id_t id) -> parseme_ptr
{
	return parseme_ptr(new parseme_t(id));
}

auto parseme_t::make(id_t id, lexing::lexeme_t const* begin, lexing::lexeme_t const* end) -> parseme_ptr
{
	return parseme_ptr(new parseme_t(id, begin, end));
}

namespace
{
	using namespace aeon;

	std::map<parsing::ID, std::string> mapped_names =
	{
		{parsing::ID::root, "root"},
		{parsing::ID::module, "module"},
		{parsing::ID::type, "type"},
		{parsing::ID::type_definition, "type-definition"},
		{parsing::ID::intrinsic_type_int, "intrinsic-type-int"},
		{parsing::ID::function_call, "function_call"},
		{parsing::ID::identifier, "identifier"},
		{parsing::ID::type_name, "typename"},
		{parsing::ID::block, "block"},
		{parsing::ID::parameter_list, "parameter-list"},
		{parsing::ID::parameter, "parameter"},
		{parsing::ID::function, "function"},
		{parsing::ID::argument_list, "argument-list"},
		{parsing::ID::variable_declaration, "variable-declaration"},
		{parsing::ID::function_pattern, "function-pattern"},
		{parsing::ID::integer_literal, "integer-literal"},
		{parsing::ID::intrinsic_int_add, "intrinsic-int-add"},
		{parsing::ID::intrinsic_int_sub, "intrinsic-int-sub"},
		{parsing::ID::intrinsic_int_mul, "intrinsic-int-mul"},
		{parsing::ID::intrinsic_int_div, "intrinsic-int-div"},
		{parsing::ID::intrinsic_bitsize, "intrinsic-bitsize"},
		{parsing::ID::placeholder, "placeholder"},
		
		{parsing::ID::return_statement, "return-statement"},
	};

	auto strid(parseme_t::id_t id) -> std::string {
		if (mapped_names.find(id) != mapped_names.end())
			return mapped_names[id];
		else
			return std::to_string(static_cast<int>(id));
	}
}

std::ostream& aeon::parsing::operator << (std::ostream& stream, parseme_t const& x) {
	stream << atma::console::fg_dark_green << strid(x.id()) << atma::console::reset << ": ";
	if (!x.text().empty())
		stream << atma::console::fg_yellow << "\"" << x.text() << "\"" << atma::console::reset << " [" << x.position().row << ":" << x.position().column << "]";
	return stream;
}

auto aeon::parsing::clone(parseme_ptr const& x) -> parseme_ptr
{
	auto r = parseme_t::make(x->id(), x->lexeme());
	for (auto const& y : x->children())
		r->children().push_back(clone(y));
	return r;
}

auto aeon::parsing::clone(parsemes_t& dest, parsemes_t const& src) -> void
{
	for (auto const& x : src)
		dest.push_back(clone(x));
}
