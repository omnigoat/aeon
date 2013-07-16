//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_LEXING_LEXEMES_HPP
#define AEON_LEXING_LEXEMES_HPP
//=====================================================================
#include <vector>
//=====================================================================
#include <atma/assert.hpp>
//=====================================================================
#include <aeon/lexing/lexeme.hpp>
//=====================================================================
namespace aeon {
namespace lexing {
//=====================================================================
	
	struct lexemes_t;


	namespace detail
	{
		typedef std::vector<lexeme_t> elements_t;

		template <typename T>
		struct elements_iterator_pred
		 : std::conditional<std::is_const<T>::value, typename elements_t::const_iterator, typename elements_t::iterator>
		  {};

		template <typename T>
		struct owner_ptr_pred
		 : std::conditional<std::is_const<T>::value, lexemes_t const*, lexemes_t*>
		  {};

		
		//=====================================================================
		// iterator
		//=====================================================================
		template <typename T>
		struct iterator
		{
			typedef int32_t difference_type;
			typedef typename std::remove_const<T>::type value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;

			// constructors
			iterator();
			template <typename Y, typename enable = typename std::enable_if<std::is_convertible<Y, T>::value, void>::type>
			iterator(iterator<Y> const& rhs);

			// operators
			auto operator * () -> T&;
			auto operator -> () -> T*;
			auto operator ++ () -> iterator&;
			auto operator -- () -> iterator&;

		private:
			typedef typename elements_iterator_pred<T>::type elements_iterator_t;
			typedef typename owner_ptr_pred<T>::type owner_ptr;

			// constructors
			
			iterator(owner_ptr, elements_iterator_t const& lbound, elements_iterator_t const& ubound, elements_iterator_t const& iter, channel_t const&);

			// members
			owner_ptr owner_;
			elements_iterator_t lbound_, ubound_, iterator_;
			channel_t channel_;

			// friends
			friend struct lexemes_t;
			friend struct iterator<typename std::add_const<T>::type>;
			template <typename T> friend auto operator == (iterator<T> const& lhs, iterator<T> const& rhs) -> bool;
		};



		template <typename T>
		inline auto operator == (iterator<T> const& lhs, iterator<T> const& rhs) -> bool {
			return
			  lhs.owner_ == rhs.owner_ &&
			  lhs.lbound_ == rhs.lbound_ &&
			  lhs.ubound_ == rhs.ubound_ &&
			  lhs.iterator_ == rhs.iterator_ &&
			  lhs.channel_ == rhs.channel_
			  ;
		}

		template <typename T>
		inline auto operator != (iterator<T> const& lhs, iterator<T> const& rhs) -> bool {
			return !operator == (lhs, rhs);
		}
	}

	


	//=====================================================================
	// lexemes_t
	//=====================================================================
	struct lexemes_t
	{
		typedef detail::iterator<lexeme_t> iterator;
		typedef detail::iterator<lexeme_t const> const_iterator;

		auto empty() const -> bool { return elements_.empty(); }

		auto push_back(lexeme_t const& L) -> void {
			elements_.push_back(L);
		}

		auto begin(channel_t const& channel = multichannel_t::all) -> iterator {
			auto i = begin_of(channel);
			return iterator(this, i, end_of(channel), i, channel);
		}

		auto end(channel_t const& channel = multichannel_t::all) -> iterator {
			auto i = end_of(channel);
			return iterator(this, begin_of(channel), i, i, channel);
		}

		auto begin(channel_t const& channel = multichannel_t::all) const -> const_iterator {
			auto i = begin_of(channel);
			return const_iterator(this, i, end_of(channel), i, channel);
		}

		auto end(channel_t const& channel = multichannel_t::all) const -> const_iterator {
			auto i = end_of(channel);
			return const_iterator(this, begin_of(channel), i, i, channel);
		}

	private:
		auto begin_of(channel_t const& channel) -> detail::elements_t::iterator {
			auto i = elements_.begin();
			while (i != elements_.end() && !(i->channel() & channel))
				++i;
			return i;
		}

		auto end_of(channel_t const& channel) -> detail::elements_t::iterator {
			auto i = elements_.rbegin();
			while (i != elements_.rend() && !(i->channel() & channel))
				++i;
			return i.base();
		}

		auto begin_of(channel_t const& channel) const -> detail::elements_t::const_iterator {
			auto i = elements_.begin();
			while (i != elements_.end() && !(i->channel() & channel))
				++i;
			return i;
		}

		auto end_of(channel_t const& channel) const -> detail::elements_t::const_iterator {
			auto i = elements_.rbegin();
			while (i != elements_.rend() && !(i->channel() & channel))
				++i;
			return i.base();
		}

		detail::elements_t elements_;

		template <typename T>
		friend struct detail::iterator;
	};
	
	
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
	// iterator implementation
	//=====================================================================
	template <typename T>
	detail::iterator<T>::iterator()
		: owner_(), iterator_(), channel_()
	{
	}

	template <typename T>
	detail::iterator<T>::iterator(owner_ptr owner, elements_iterator_t const& lbound, elements_iterator_t const& ubound, elements_iterator_t const& iter, channel_t const& channel)
		: owner_(owner), lbound_(lbound), ubound_(ubound), iterator_(iter), channel_(channel)
	{
		ATMA_ASSERT(iterator_ == owner_->elements_.end() || (iterator_->channel() & channel));
	}
	
	template <typename T>
	template <typename Y, typename enable>
	detail::iterator<T>::iterator(iterator<Y> const& rhs)
		: owner_(rhs.owner_), lbound_(rhs.lbound_), ubound_(rhs.ubound_), iterator_(rhs.iterator_), channel_(rhs.channel_)
	{
	}

	template <typename T>
	auto detail::iterator<T>::operator * () -> T& {
		return *iterator_;
	}
	
	template <typename T>
	auto detail::iterator<T>::operator -> () -> T* {
		return &*iterator_;
	}

	template <typename T>
	auto detail::iterator<T>::operator ++ () -> iterator& {
		ATMA_ASSERT(iterator_ != ubound_);
		while (++iterator_ != ubound_ && !(iterator_->channel() & channel_))
			;
		return *this;
	}

	template <typename T>
	auto detail::iterator<T>::operator -- () -> iterator& {
		ATMA_ASSERT(iterator_ != lbound_);
		while (--iterator_ != lbound_ && !(iterator_->channel() & channel_))
			;
		return *this;
	}



	

//=====================================================================
} // namespace lexing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

