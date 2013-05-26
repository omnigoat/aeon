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
#include <atma/assert.hpp>
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
			typedef int32_t difference_type;
			typedef typename std::remove_const<T>::type value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;

			auto operator * () -> T&;
			auto operator ++ () -> iterator&;
			auto operator -- () -> iterator&;

		private:
			// if T is a const T, then we need elements_t::const_iterator
			typedef typename std::conditional<std::is_const<T>::value, 
				typename elements_t::const_iterator, typename elements_t::iterator>::type elements_iterator_t;

			typedef typename std::conditional<std::is_const<T>::value, 
				const lexemes_t&, lexemes_t&>::type owner_ref;


			// normal constructor
			iterator(owner_ref, elements_iterator_t const&, channel_t const&);

			// constructor for const_iterator being constructed from iterator
			template <typename Y = typename std::enable_if<std::is_const<T>::value, typename std::remove_const<T>::type>::type>
			iterator(iterator<Y> const&);

			owner_ref owner_;
			elements_iterator_t iterator_;
			channel_t channel_;

			friend struct lexemes_t;

			template <typename T>
			friend auto operator == (iterator<T> const& lhs, iterator<T> const& rhs) -> bool;

			template <typename Y>
			friend struct iterator;
		};

		template <typename T>
		inline auto operator == (iterator<T> const& lhs, iterator<T> const& rhs) -> bool {
			return &lhs.owner_ == &rhs.owner_ && lhs.iterator_ == rhs.iterator_ && lhs.channel_ == rhs.channel_;
		}

		template <typename T>
		inline auto operator != (iterator<T> const& lhs, iterator<T> const& rhs) -> bool {
			return !operator == (lhs, rhs);
		}
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

		auto begin(channel_t const& channel = channel_t::all) const -> const_iterator {
			return const_cast<lexemes_t*>(this)->begin();
		}

		auto end(channel_t const& channel = channel_t::all) const -> const_iterator {
			auto i = elements_.rbegin();
			while (i != elements_.rend() && !(i->channel() & channel))
				++i;
			return const_iterator(*this, i.base(), channel);
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
	detail::iterator<T>::iterator(owner_ref owner, elements_iterator_t const& iter, channel_t const& channel)
		: owner_(owner), iterator_(iter), channel_(channel)
	{
		ATMA_ASSERT(iterator_ == owner_.elements_.end() || iterator_->channel() & channel);
	}

	template <typename T>
	template <typename Y>
	detail::iterator<T>::iterator(iterator<Y> const& rhs)
		: owner_(rhs.owner_), iterator_(rhs.iterator_), channel_(rhs.channel_)
	{
	}

	template <typename T>
	auto detail::iterator<T>::operator * () -> T& {
		return *iterator_;
	}
	

	template <typename T>
	auto detail::iterator<T>::operator ++ () -> iterator& {
		while (++iterator_ != owner_.elements_.end() && !(iterator_->channel() & channel_))
			;
		return *this;
	}

	template <typename T>
	auto detail::iterator<T>::operator -- () -> iterator& {
		while (--iterator_ != owner_.elements_.end() && !(iterator_->channel() & channel_))
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

