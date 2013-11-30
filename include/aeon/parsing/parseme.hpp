//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_PARSEME_HPP
#define AEON_PARSING_PARSEME_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
#include <vector>
#include <memory>
//=====================================================================
#include <aeon/lexing/lexeme.hpp>
#include <aeon/parsing/children.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	typedef std::weak_ptr<parseme_t> parseme_wptr;
	typedef std::vector<parseme_ptr> parsemes_t;

	extern const parseme_ptr null_parseme_ptr;

	struct parseme_t : std::enable_shared_from_this<parseme_t>
	{
		typedef children_t children_t;
		enum class id_t;

		parseme_t(id_t);
		parseme_t(id_t, lexing::lexeme_t const*);
		~parseme_t();

		auto id() const -> id_t;
		auto text() const -> lexing::lexeme_t::text_t const&;
		auto position() const -> lexing::position_t const&;
		auto parent() const -> parseme_ptr;
		auto children() const -> children_t const&;
		auto children() -> children_t&;
		
		auto set_parent(parseme_ptr const&) -> void;

		static auto make(id_t, lexing::lexeme_t const* = nullptr) -> parseme_ptr;

	private:
		auto lexeme() const -> lexing::lexeme_t const*;

		id_t id_;
		parseme_wptr parent_;
		children_t children_;
		lexing::lexeme_t const* lexeme_;
	};
	
	inline std::ostream& operator << (std::ostream& stream, parseme_t const& x) {
		return stream << static_cast<int>(x.id()) << ": " << x.text();
	}

	enum class parseme_t::id_t
	{
		root,
		module,

		identifier,
		placeholder,

		function,
		function_pattern,
		parameter_list,
		parameter,
		
		type_name,
		type,
		block,
		return_statement,
		expr,
		addition_expr,
		type_definition,
		intrinsic_type_int,
		intrinsic_bitsize,
		intrinsic_int_add
	};

	typedef parseme_t::id_t ID;

	inline auto is_intrinsic_int_typename(parsing::parseme_ptr const& x) -> bool
	{
		auto i = x->text().begin();
		return *i == '@' && *++i == 'i' && *++i == 'n' && *++i == 't' && isdigit(*++i);
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

