//=====================================================================
//
//
//
//=====================================================================
#ifndef JIGL_LEXING_LEXEMES_HPP
#define JIGL_LEXING_LEXEMES_HPP
//=====================================================================
#include <vector>
//=====================================================================
#include <jigl/lexing/lexeme.hpp>
//=====================================================================
namespace jigl {
namespace lexing {
//=====================================================================
	
	struct lexemes_t;


	namespace detail
	{
		typedef std::vector<lexeme_t> elements_t;


		template <typename T>
		struct iterator
		{
			auto operator * () -> T& {
				return *iterator_;
			}

			auto operator ++ () -> iterator&;

		private:
			iterator(lexemes_t&, elements_t::iterator const&, channel_t const&);

			lexemes_t& owner_;
			elements_t::iterator iterator_;
			channel_t channel_;

			friend struct lexemes_t;
		};
	}


	


	struct lexemes_t
	{
		typedef detail::iterator<lexeme_t> iterator;
		typedef detail::iterator<const lexeme_t> const_iterator;

		auto push_back(lexeme_t const& L) -> void {
			elements_.push_back(L);
		}

		auto begin(channel_t const& channel = channel_t::all) -> iterator {
			auto i = elements_.begin();
			while (i != elements_.end() && !(i->channel() & channel))
				++i;
			return iterator(*this, i, channel);
		}

		auto end(channel_t const& channel) -> iterator {
			auto i = elements_.rbegin();
			while (i != elements_.rend() && !(i->channel() & channel))
				++i;
			return iterator(*this, i.base(), channel);
		}

	private:
		
		detail::elements_t elements_;

		template <typename T>
		friend struct detail::iterator;
	};
	

	//=====================================================================
	// iterator implementation
	//=====================================================================
	template <typename T>
	detail::iterator<T>::iterator(lexemes_t& owner, elements_t::iterator const& iter, channel_t const& channel)
		: owner_(owner), iterator_(iter), channel_(channel)
	{
		while (iterator_ != owner_.elements_.end() && !(iterator_->channel() & channel))
			++iterator_;
	}
	

	template <typename T>
	auto detail::iterator<T>::operator ++ () -> iterator& {
		while (++iterator != owner_.elements_.end() && !(iterator->channel() & channel_))
			;
		return *this;
	}
















	template <typename T>
	inline std::ostream& operator << (std::ostream& stream, lexemes_t const& rhs) 
	{
		stream << "lexemes\n---------" << std::endl;
		if (rhs.empty())
			stream << "  empty!" << std::endl;
		
		for (auto const& x : rhs)
			stream << "  " << x << std::endl;
		
		return stream << std::endl;
	}

//=====================================================================
} // namespace lexing
} // namespace jigl
//=====================================================================
#endif
//=====================================================================

