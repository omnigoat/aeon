//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_PARSEMES_HPP
#define AEON_PARSING_PARSEMES_HPP
//=====================================================================
#include <cstdint>
#include <ostream>
#include <vector>
#include <memory>
//=====================================================================
#include <aeon/parsing/parsemes.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
//=====================================================================
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	typedef std::weak_ptr<parseme_t> parseme_wptr;

	struct parsemes_t
	{
		typedef std::vector<parseme_ptr>::iterator iterator;
		typedef std::vector<parseme_ptr>::const_iterator const_iterator;

		parsemes_t();

		auto begin() const -> const_iterator;
		auto end() const -> const_iterator;
		
		auto replace(const_iterator const&, parseme_ptr const&) -> void;
		auto detach(const_iterator const&) -> parseme_ptr;

		auto push_back(parseme_ptr const&) -> void;
		

	private:
		parsemes_t(parseme_t*);

		auto unconst(const_iterator const&) -> iterator;

		parseme_t* owner_;
		std::vector<parseme_ptr> elements_;

		friend parseme_t;
	};

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

