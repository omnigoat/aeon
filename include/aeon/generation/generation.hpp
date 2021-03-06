#pragma once
//=====================================================================
#include <aeon/parsing/parseme.hpp>
#include <aeon/generation/genesis.hpp>
#include <atma/string.hpp>
#include <map>
//=====================================================================
namespace aeon {
namespace generation {
//=====================================================================

	struct abstract_output_stream_t
	{
		abstract_output_stream_t() : tabs() {}
		virtual ~abstract_output_stream_t() {}

		virtual auto put(atma::string const& str) -> void = 0;
		virtual auto putc(char) -> void = 0;

		uint32_t tabs;
	};

	inline abstract_output_stream_t& line_begin(abstract_output_stream_t& stream) {
		for (uint32_t i = 0; i != stream.tabs; ++i)
			stream.putc('\t');
		return stream;
	};

	inline auto operator << (abstract_output_stream_t& lhs, atma::string const& rhs) -> abstract_output_stream_t&
	{
		for (auto c : rhs)
			for (auto ci = c.begin; ci != c.end; ++ci)
				lhs.putc(*ci);
		return lhs;
	}

	inline auto operator << (abstract_output_stream_t& lhs, std::string const& rhs) -> abstract_output_stream_t&
	{
		for (auto c : rhs)
			lhs.putc(c);
		return lhs;
	}

	inline auto operator << (abstract_output_stream_t& lhs, uint rhs) -> abstract_output_stream_t&
	{
		return lhs << std::to_string(rhs);
	}

	inline auto operator << (abstract_output_stream_t& lhs, atma::utf8_string_range_t const& rhs) -> abstract_output_stream_t&
	{
		for (char x : rhs)
			lhs.putc(x);
		
		return lhs;
	}

	template <int N>
	inline auto operator << (abstract_output_stream_t& lhs, char const (&rhs)[N]) -> abstract_output_stream_t&
	{
		// minus 1 because we don't do null-terminator
		for (int i = 0; i != N - 1; ++i)
			lhs.putc(rhs[i]);

		return lhs;
	}

	inline auto operator << (abstract_output_stream_t& lhs, abstract_output_stream_t& (*rhs)(abstract_output_stream_t&)) -> abstract_output_stream_t&
	{
		return (*rhs)(lhs);
	}



	struct scoped_stream_tabify_t
	{
		scoped_stream_tabify_t(abstract_output_stream_t& stream)
		: stream_(stream)
		{
			++stream_.tabs;
		}

		~scoped_stream_tabify_t()
		{
			--stream_.tabs;
		}

	private:
		abstract_output_stream_t& stream_;
	};

	struct file_output_stream_t : abstract_output_stream_t
	{
		file_output_stream_t(FILE* file)
		: file_(file)
		{
		}

		auto put(atma::string const& str) -> void override
		{
			static char tabbuf[] = "\t\t\t\t\t\t\t\t";

			if (str.empty())
				return;

			ATMA_ASSERT(tabs <= 8);
			fwrite(tabbuf, 1, tabs, file_);
			fwrite(str.bytes_begin(), 1, str.bytes(), file_);
		}

		auto putc(char c) -> void override
		{
			fputc(c, file_);
		}

	private:
		FILE* file_;
	};

	

	auto function_name_mangle(parsing::parseme_ptr const&) -> atma::string;
	auto type_name_mangle(parsing::parseme_ptr const&) -> atma::string;
	auto type_structure(parsing::parseme_ptr const&) -> atma::string;

	auto module(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;
	auto function(abstract_output_stream_t&, genesis_t&, parsing::parseme_ptr const&) -> void;
	auto function_body(abstract_output_stream_t&, genesis_t&, parsing::parseme_ptr const&) -> void;
	auto statement(abstract_output_stream_t&, genesis_t&, parsing::parseme_ptr const&) -> void;
	auto return_statement(abstract_output_stream_t&, genesis_t&, parsing::parseme_ptr const&) -> void;
	auto expression(abstract_output_stream_t&, genesis_t&, parsing::parseme_ptr const&) -> void;

	namespace llvm
	{
		auto storage_typename(genesis_t&, parsing::parseme_ptr const&) -> atma::string;
		auto lvalue_name(genesis_t&, parsing::parseme_ptr const&) -> atma::string;
	}
	

//=====================================================================
} // namespace generation
} // namespace aeon
//=====================================================================
