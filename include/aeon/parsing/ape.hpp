//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_PARSING_APE_HPP
#define AEON_PARSING_APE_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
//=====================================================================
namespace aeon {
namespace parsing {
namespace ape {
//=====================================================================
	
	namespace detail
	{
		struct abstract_expr_t;
		struct make_expr_t;
		struct seq_and_expr_t;
		struct dive_expr_t;
		typedef std::shared_ptr<abstract_expr_t> abstract_expr_ptr;

		struct abstract_expr_t : std::enable_shared_from_this<abstract_expr_t> {
			virtual auto operator ()(children_t&) -> bool = 0;
		};

		struct expr_t
		{
			expr_t(abstract_expr_ptr const& backend)
				: backend_(backend)
			{
			}

			auto operator [](expr_t const&) const -> expr_t;
			auto operator ()(children_t&) const -> bool;

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

			auto operator ()(children_t& dest) -> bool {
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
				ATMA_ASSERT( !!dynamic_cast<make_expr_t const*>(parent_.get()) );
			}

			auto operator ()(children_t& dest) -> bool {
				return (*parent_)(dest) && (*child_)(dest.back()->children());
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

			auto operator ()(children_t& dest) -> bool {
				return (*x_)(dest) && (*y_)(dest);
			}

		private:
			abstract_expr_ptr x_, y_;
		};



		auto expr_t::operator [] (expr_t const& child) const -> expr_t
		{
			return expr_t(abstract_expr_ptr(new dive_expr_t(backend_, child.backend_)));
		}

		auto expr_t::operator ()(children_t& dest) const -> bool
		{
			return (*backend_)(dest);
		}

		auto operator , (expr_t const& lhs, expr_t const& rhs) -> expr_t
		{
			return expr_t(abstract_expr_ptr(new seq_and_expr_t(lhs.backend_, rhs.backend_)));
		}



	}

	auto make(parseme_t::id_t id, lexing::lexeme_t const& L) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::make_expr_t(id, &L)));
	}

	auto make(parseme_t::id_t id) -> detail::expr_t {
		return detail::expr_t(detail::abstract_expr_ptr(new detail::make_expr_t(id, nullptr)));
	}
	

	void insert_into(children_t& dest, detail::expr_t const& x) {
		x(dest);
	}
	

//=====================================================================
} // namespace ape
} // namespace parsing
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

