#pragma once
//=====================================================================
#include <aeon/lexing/lexeme.hpp>
#include <aeon/parsing/children.hpp>
#include <aeon/parsing/id.hpp>

#include <cstdint>
#include <ostream>
#include <vector>
#include <memory>
//=====================================================================
namespace aeon { namespace parsing {
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	typedef std::weak_ptr<parseme_t> parseme_wptr;
	typedef std::vector<parseme_ptr> parsemes_t;
	typedef std::vector<parseme_ptr const*> parseme_ptr_refs_t;

	extern const parseme_ptr null_parseme_ptr;

	struct parseme_t : std::enable_shared_from_this<parseme_t>
	{
		typedef children_t children_t;
		typedef lexing::lexeme_t::text_t text_t;
		typedef ID id_t;

		static auto make(id_t) -> parseme_ptr;
		static auto make(id_t, lexing::lexeme_t const*) -> parseme_ptr;
		static auto make(id_t, lexing::lexeme_t const*, lexing::lexeme_t const*) -> parseme_ptr;
		
		auto id() const -> id_t;
		auto text() const -> lexing::lexeme_t::text_t const&;
		auto position() const -> lexing::position_t const&;
		auto parent() const -> parseme_ptr;
		auto children() const -> children_t const&;
		auto children() -> children_t&;
		auto lexeme() const -> lexing::lexeme_t const*;
		auto lexeme2() const -> lexing::lexeme_t const*;
		auto siblings() const -> children_t const*;

		auto set_parent(parseme_ptr const&) -> void;
		auto set_siblings(children_t const*) -> void;

	private:
		parseme_t(id_t);
		parseme_t(id_t, lexing::lexeme_t const*);
		parseme_t(id_t, lexing::lexeme_t const*, lexing::lexeme_t const*);

	private:
		id_t id_;
		parseme_wptr parent_;
		children_t const* siblings_;
		children_t children_;

		// not every parseme has a lexeme2, but if it does:
		//  - lexeme2_ signifies the terminal (i.e: end)
		//  - neither lexeme_ and lexeme2_ may be auxililary
		//  - lexeme2_ must occur after lexeme_
		lexing::lexeme_t const* lexeme_;
		lexing::lexeme_t const* lexeme2_;
	};
	
	std::ostream& operator << (std::ostream& stream, parseme_t const& x);

	// recursive
	auto clone(parseme_ptr const&) -> parseme_ptr;
	auto clone(parsemes_t&, parsemes_t const&) -> void;

	inline auto is_intrinsic_int_typename(parsing::parseme_ptr const& x) -> bool
	{
		auto i = x->text().begin();
		return *i == '@' && *++i == 'i' && *++i == 'n' && *++i == 't' && isdigit(*++i);
	}

} }
