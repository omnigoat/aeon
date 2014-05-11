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

	extern const parseme_ptr null_parseme_ptr;

	struct parseme_t : std::enable_shared_from_this<parseme_t>
	{
		typedef children_t children_t;
		typedef lexing::lexeme_t::text_t text_t;
		typedef ID id_t;

		parseme_t(id_t);
		parseme_t(id_t, lexing::lexeme_t const*);
		~parseme_t();

		auto id() const -> id_t;
		auto text() const -> lexing::lexeme_t::text_t const&;
		auto position() const -> lexing::position_t const&;
		auto parent() const -> parseme_ptr;
		auto children() const -> children_t const&;
		auto children() -> children_t&;
		auto lexeme() const -> lexing::lexeme_t const*;

		auto set_parent(parseme_ptr const&) -> void;

		static auto make(id_t, lexing::lexeme_t const* = nullptr) -> parseme_ptr;

	private:
		id_t id_;
		parseme_wptr parent_;
		children_t children_;
		lexing::lexeme_t const* lexeme_;
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
