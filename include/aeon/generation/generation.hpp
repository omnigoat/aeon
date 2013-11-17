//=====================================================================
//
//
//
//=====================================================================
#ifndef AEON_GENERATION_HPP
#define AEON_GENERATION_HPP
//=====================================================================
#include <aeon/parsing/parseme.hpp>
#include <atma/string.hpp>
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
		lhs.put(rhs);
		return lhs;
	}

	inline auto operator << (abstract_output_stream_t& lhs, atma::utf8_string_range_t const& rhs) -> abstract_output_stream_t&
	{
		for (char x : rhs)
			lhs.putc(x);
		
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
			if (str.empty())
				return;
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
	auto function(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;
	auto function_body(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;
	auto statement(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;
	auto return_statement(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;
	auto expression(abstract_output_stream_t&, parsing::parseme_ptr const&) -> void;

//=====================================================================
} // namespace generation
} // namespace aeon
//=====================================================================
#endif
//=====================================================================

