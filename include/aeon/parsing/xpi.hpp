//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_APE_HPP
#define AEON_PARSING_APE_HPP
//=====================================================================
#include <aeon/lexing/id.hpp>
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
namespace xpi {
//=====================================================================
	
	namespace detail
	{
		struct abstract_expr_t;
		struct make_expr_t;
		struct seq_and_expr_t;
		struct dive_expr_t;
		typedef std::shared_ptr<abstract_expr_t> abstract_expr_ptr;

		struct abstract_expr_t : std::enable_shared_from_this<abstract_expr_t> {
			virtual auto operator ()(parsemes_t&) -> bool = 0;
		};

		struct expr_t
		{
			expr_t(abstract_expr_ptr const& backend)
				: backend_(backend)
			{
			}

			auto operator [](expr_t const&) const -> expr_t;
			auto operator ()(parsemes_t&) const -> bool;

		private:
			abstract_expr_ptr backend_;

			friend auto operator , (expr_t const& lhs, expr_t const& rhs) -> expr_t;
		};



		struct make_expr_t : abstract_expr_t
		{
			make_expr_t(parseme_t::id_t id, lexing::lexeme_t const* lexeme)
				: id_(id), lexeme_(lexeme)
			{
			}

			auto operator ()(parsemes_t& dest) -> bool {
				dest.push_back( parseme_ptr(new parseme_t(id_, lexeme_)) );
				return true;
			}

		private:
			parseme_t::id_t id_;
			lexing::lexeme_t const* lexeme_;
		};

		struct dive_expr_t : abstract_expr_t
		{
			dive_expr_t(abstract_expr_ptr const& parent, abstract_expr_ptr const& child)
				: parent_(parent), child_(child)
			{
				//ATMA_ASSERT( !!dynamic_cast<make_expr_t const*>(parent_.get()) );
			}

			auto operator ()(parsemes_t& dest) -> bool {
				parsemes_t tmp;
				bool result = (*parent_)(dest) && (*child_)(tmp);
				if (result) {
					auto& ch = dest.back()->children();
					ch.insert(ch.end(), tmp.begin(), tmp.end());
				}
				return result;
			}

		private:
			abstract_expr_ptr parent_, child_;
		};

		struct seq_and_expr_t : abstract_expr_t
		{
			seq_and_expr_t(abstract_expr_ptr const& x, abstract_expr_ptr const& y)
				: x_(x), y_(y)
			{
			}

			auto operator ()(parsemes_t& dest) -> bool {
				return (*x_)(dest) && (*y_)(dest);
			}

		private:
			abstract_expr_ptr x_, y_;
		};

		template <typename IT>
		struct insert_expr_t : abstract_expr_t
		{
			insert_expr_t(IT begin, IT end)
				: begin_(begin), end_(end)
			{}

			auto operator ()(parsemes_t& dest) -> bool {
				dest.insert(dest.end(), begin_, end_);
				return true;
			}

		private:
			IT begin_, end_;
		};

		struct insert_single_expr_t : abstract_expr_t
		{
			insert_single_expr_t(parseme_ptr const& x)
				: x_(x)
			{}

			auto operator ()(parsemes_t& dest) -> bool {
				dest.push_back(x_);
				return true;
			}

		private:
			parseme_ptr x_;
		};


		inline auto expr_t::operator [] (expr_t const& child) const -> expr_t
		{
			return expr_t(abstract_expr_ptr(new dive_expr_t(backend_, child.backend_)));
		}

		inline auto expr_t::operator ()(parsemes_t& dest) const -> bool
		{
			return (*backend_)(dest);
		}

		inline auto operator , (expr_t const& lhs, expr_t const& rhs) -> expr_t
		{
			return expr_t(abstract_expr_ptr(new seq_and_expr_t(lhs.backend_, rhs.backend_)));
		}
	}

	inline auto make(parseme_t::id_t id, lexing::lexeme_t const& L) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::make_expr_t(id, &L)));
	}

	inline auto make(parseme_t::id_t id, lexing::lexeme_t const* L) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::make_expr_t(id, L)));
	}

	inline auto make(parseme_t::id_t id) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::make_expr_t(id, nullptr)));
	}
	
	inline auto make(parseme_t::id_t id, lexing::ID lid, lexing::lexeme_t::text_t const& text) -> detail::expr_t {
		return make(id, lexing::make_synthetic_lexeme(lid, text.begin(), text.end(), lexing::position_t::zero));
	}

	inline auto make(parseme_t::id_t id, lexing::lexeme_t::text_t const& text) -> detail::expr_t {
		return make(id, lexing::make_synthetic_lexeme(lexing::ID::dummy, text.begin(), text.end(), lexing::position_t::zero));
	}

	template <typename IT>
	inline auto insert(IT begin, IT end) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::insert_expr_t<IT>(begin, end)));
	}

	inline auto insert(parseme_ptr const& x) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::insert_single_expr_t(x)));
	}

	inline auto reify(detail::expr_t const& x) -> parseme_ptr
	{
		parsemes_t p;
		x(p);
		ATMA_ASSERT(p.size() == 1);
		return p.back();
	}


	inline void insert_into(parsemes_t& dest, detail::expr_t const& x) {
		x(dest);
	}
	
	inline void insert_into(children_t& dest, detail::expr_t const& x) {
		parsemes_t tmp;
		x(tmp);

		dest.insert(dest.end(), tmp.begin(), tmp.end());
	}

	inline void insert_into(parsemes_t& dest, parsemes_t::iterator const& where_, detail::expr_t const& x) {
		parsemes_t c;
		x(c);
		dest.insert(where_, c.begin(), c.end());
	}

	inline void insert_into(children_t& dest, children_t::iterator const& where_, detail::expr_t const& x) {
		parsemes_t c;
		x(c);
		dest.insert(where_, c.begin(), c.end());
	}

	inline void replace(parsing::parseme_ptr& dest, detail::expr_t const& x) {
		parsemes_t c;
		x(c);
		ATMA_ASSERT(!c.empty());
		dest = c.back();
	}

	
//=====================================================================
} // namespace xpi
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

