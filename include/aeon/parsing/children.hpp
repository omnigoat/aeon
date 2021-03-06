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
namespace aeon {
namespace parsing {
//=====================================================================
	
	struct parseme_t;
	typedef std::shared_ptr<parseme_t> parseme_ptr;
	typedef std::weak_ptr<parseme_t> parseme_wptr;

	struct children_t
	{
		typedef parseme_ptr const& const_reference;
		typedef parseme_ptr value_type;
		typedef std::vector<parseme_ptr>::iterator iterator;
		typedef std::vector<parseme_ptr>::const_iterator const_iterator;

		children_t();

		auto operator [](int) -> parseme_ptr const&;

		auto owner() const -> parseme_t const*;
		auto size() const -> uint32_t;
		auto empty() const -> bool;

		auto front() const -> parseme_ptr const&;
		auto back() const -> parseme_ptr const&;

		auto begin() -> iterator;
		auto end() -> iterator;
		auto begin() const -> const_iterator;
		auto end() const -> const_iterator;
		
		auto replace(const_iterator const&, parseme_ptr const&) -> void;
		auto replace(parseme_ptr const&, parseme_ptr const&) -> void;
		auto detach(const_iterator const&) -> parseme_ptr;
		auto detach(parseme_ptr const&) -> bool;
		auto insert(iterator const& where_, iterator const& begin, iterator const& end) -> iterator;

		auto push_back(parseme_ptr const&) -> void;
		auto pop_back() -> void;

	private:
		children_t(parseme_t*);

		auto unconst(const_iterator const&) -> iterator;

		parseme_t* owner_;
		std::vector<parseme_ptr> elements_;

		friend parseme_t;
	};

	auto clone(children_t&, children_t const&) -> void;

	namespace detail
	{
		auto print_parsemes(std::ostream&, children_t const&, uint32_t = 0) -> std::ostream&;
	}

	inline std::ostream& operator << (std::ostream& stream, children_t const& x) {
		stream << "parsemes\n----------\n";
		return detail::print_parsemes(stream, x);
	}

//=====================================================================
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

