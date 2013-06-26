//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_PARSING_PARSEME_HPP
#define JIGL_PARSING_PARSEME_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
#include <vector>
#include <memory>
//=====================================================================
#include <jigl/lexing/lexeme.hpp>
//=====================================================================
namespace jigl {
namespace parsing {
//=====================================================================
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	typedef std::vector<parseme_ptr> parsemes_t;

	struct parseme_t
	{
		enum class id_t;

		parseme_t(id_t);
		parseme_t(parseme_ptr const& parent, id_t id, lexing::lexeme_t* = nullptr);
		
		auto id() const -> id_t;
		auto parent() const -> parseme_ptr const&;
		auto children() const -> parsemes_t const&;

		// child mutators
		auto add_child(parseme_ptr const&) -> void;
		auto remove_child(parsemes_t::const_iterator const&) -> void;
		auto replace_child(parsemes_t::const_iterator const&, parseme_ptr const&) -> void;
		

	private:
		id_t id_;
		parseme_ptr parent_;
		parsemes_t children_;
		lexing::lexeme_t* lexeme_;
	};
	
	enum class parseme_t::id_t{
		module,
		function,
		type
	};



//=====================================================================
} // namespace parsing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

