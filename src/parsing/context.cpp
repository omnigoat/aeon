#include <aeon/parsing/context.hpp>
#include <aeon/parsing/xpi.hpp>
#include <aeon/lexing/id.hpp>

#include <atma/string.hpp>

#include <map>

using namespace aeon::parsing;
using aeon::parsing::detail::context_t;

typedef aeon::lexing::ID lexid;
typedef parseme_t::id_t parsid;


namespace
{
	std::map<uint32_t, atma::string> typename_strings;
}


context_t::context_t(parseme_ptr const& root, aeon::lexing::lexemes_t::const_iterator const& begin, aeon::lexing::lexemes_t::const_iterator const& end)
	: root_(root), begin_(begin), end_(end)
{
}

auto context_t::id() const -> aeon::lexing::ID
{
	return begin_->id();
}

auto context_t::align(aeon::lexing::multichannel_t const& ch) -> void
{
	begin_.set_channel(ch);
}

auto context_t::match_make(parsid pid, lexid lid) -> parseme_ptr
{
	parseme_ptr result;
	
	if (begin_ == end_)
		return parsing::null_parseme_ptr;

	if (begin_->id() == lid) {
		result.reset(new parseme_t(pid, &*begin_));
		++begin_;
	}

	return result;
}

auto context_t::match_make(parsid pid, lexid lid, char const* str) -> parseme_ptr
{
	parseme_ptr result;
	
	if (begin_ == end_)
		return parsing::null_parseme_ptr;

	if (begin_->id() == lid && begin_->streq(str)) {
		result.reset(new parseme_t(pid, &*begin_));
		++begin_;
	}

	return result;
}

auto context_t::match_make(parsid pid, lexid lid, aeon::lexing::multichannel_t const& ch) -> parseme_ptr
{
	if (begin_ == end_)
		return parsing::null_parseme_ptr;

	begin_.set_channel(ch);
	return match_make(pid, lid);
}

auto context_t::skip(lexid lid) -> bool
{
	if (begin_ == end_)
		return false;

	if (begin_->id() != lid)
		return false;
	++begin_;
	return true;
}

auto context_t::skip(lexid lid, char const* str) -> bool
{
	if (begin_ == end_)
		return false;

	if (begin_->id() != lid || !begin_->streq(str))
		return false;
	++begin_;
	return true;
}

auto context_t::current_lexeme() const -> aeon::lexing::lexeme_t const*
{
	return &*begin_;
}


auto context_t::generate_intrinsic_integer_definition(parseme_ptr const& type_name) -> void
{
	ATMA_ASSERT(type_name->id() == ID::type_name);

	uint32_t size = std::atoi(type_name->text().begin() + 4);
	if (intrinsic_integers_.find(size) != intrinsic_integers_.end())
		return;
	intrinsic_integers_.insert(size);

	// if this is the first time an integer of this width is encountered, insert it into the prelude
	xpi::insert_into(root_->children(), root_->children().begin(), (
		xpi::make(ID::type_definition) [
			xpi::make(ID::identifier, lexing::ID::identifier, type_name->text()),
			xpi::make(ID::intrinsic_type_int),
			xpi::make(ID::intrinsic_bitsize, lexing::make_synthetic_lexeme(
				lexing::ID::integer_literal, type_name->text().begin() + 4, type_name->text().end()))
		],

		// addition
		xpi::make(ID::function) [
			xpi::make(ID::function_pattern) [
				xpi::make(ID::placeholder),
				xpi::make(ID::identifier, "+"),
				xpi::make(ID::placeholder)
			],

			xpi::make(ID::parameter_list)[
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "lhs"),
					xpi::make(ID::type_name, type_name->text())
				],
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "rhs"),
					xpi::make(ID::type_name, type_name->text())
				]
			],

			xpi::make(ID::type_name, type_name->text()),

			xpi::make(ID::block) [
				xpi::make(ID::return_statement)[
					xpi::make(ID::intrinsic_int_add, lexing::make_synthetic_lexeme(lexing::ID::integer_literal, type_name->text().begin() + 4, type_name->text().end())) [
						xpi::make(ID::identifier, "lhs"),
						xpi::make(ID::identifier, "rhs")
					]
				]
			]
		],

		// subtraction
		xpi::make(ID::function)[
			xpi::make(ID::function_pattern)[
				xpi::make(ID::placeholder),
				xpi::make(ID::identifier, "-"),
				xpi::make(ID::placeholder)
			],

			xpi::make(ID::parameter_list)[
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "lhs"),
					xpi::make(ID::type_name, type_name->text())
				],
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "rhs"),
					xpi::make(ID::type_name, type_name->text())
				]
			],

			xpi::make(ID::type_name, type_name->text()),

			xpi::make(ID::block)[
				xpi::make(ID::return_statement)[
					xpi::make(ID::intrinsic_int_sub, lexing::make_synthetic_lexeme(lexing::ID::integer_literal, type_name->text().begin() + 4, type_name->text().end()))[
						xpi::make(ID::identifier, "lhs"),
						xpi::make(ID::identifier, "rhs")
					]
				]
			]
		],

		// subtraction
		xpi::make(ID::function)[
			xpi::make(ID::function_pattern)[
				xpi::make(ID::placeholder),
				xpi::make(ID::identifier, "*"),
				xpi::make(ID::placeholder)
			],

			xpi::make(ID::parameter_list)[
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "lhs"),
					xpi::make(ID::type_name, type_name->text())
				],
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "rhs"),
					xpi::make(ID::type_name, type_name->text())
				]
			],

			xpi::make(ID::type_name, type_name->text()),

			xpi::make(ID::block)[
				xpi::make(ID::return_statement)[
					xpi::make(ID::intrinsic_int_mul, lexing::make_synthetic_lexeme(lexing::ID::integer_literal, type_name->text().begin() + 4, type_name->text().end()))[
						xpi::make(ID::identifier, "lhs"),
						xpi::make(ID::identifier, "rhs")
					]
				]
			]
		],

		// subtraction
		xpi::make(ID::function)[
			xpi::make(ID::function_pattern)[
				xpi::make(ID::placeholder),
				xpi::make(ID::identifier, "/"),
				xpi::make(ID::placeholder)
			],

			xpi::make(ID::parameter_list)[
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "lhs"),
					xpi::make(ID::type_name, type_name->text())
				],
				xpi::make(ID::parameter)[
					xpi::make(ID::identifier, "rhs"),
					xpi::make(ID::type_name, type_name->text())
				]
			],

			xpi::make(ID::type_name, type_name->text()),

			xpi::make(ID::block)[
				xpi::make(ID::return_statement)[
					xpi::make(ID::intrinsic_int_div, lexing::make_synthetic_lexeme(lexing::ID::integer_literal, type_name->text().begin() + 4, type_name->text().end()))[
						xpi::make(ID::identifier, "lhs"),
						xpi::make(ID::identifier, "rhs")
					]
				]
			]
		]
	));
}

