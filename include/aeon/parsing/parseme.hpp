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
#include <aeon/parsing/parsemes.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	
	struct parseme_t : std::enable_shared_from_this<parseme_t>
	{
		typedef parsemes_t children_t;
		enum class id_t;

		parseme_t(id_t);
		parseme_t(id_t id, lexing::lexeme_t const*);
		
		auto id() const -> id_t;
		auto parent() const -> parseme_ptr const&;
		auto children() const -> children_t const&;
		auto children() -> children_t&;

		auto set_parent(parseme_ptr const&) -> void;

	private:
		id_t id_;
		parseme_ptr parent_;
		children_t children_;
		lexing::lexeme_t const* lexeme_;
	};
	
	enum class parseme_t::id_t
	{
		module,
		function,
		function_name,
		parameter_list,
		parameter,
		type_name
	};



//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

