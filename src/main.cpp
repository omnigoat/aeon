#include <cstdint>
#include <iostream>

#include <aeon/lexing/lex.hpp>
#include <aeon/lexing/id.hpp>
#include <aeon/parsing/parse.hpp>

#define ATMA_ASSERT_RETURN(x) \
	if (!(x)) { \
		__debugbreak(); \
		return; \
	}

struct input_file_t
{
	input_file_t(char const* filename)
		: filename_(filename), file_(), data_()
	{
		file_ = fopen(filename, "rb");
		ATMA_ASSERT_RETURN(file_ != nullptr);
		fseek(file_, 0, SEEK_END);
		file_size_ = ftell(file_);
		fseek(file_, 0, SEEK_SET);
		data_ = new char[file_size_];
		fread(data_, 1, file_size_, file_);
	}

	~input_file_t()
	{
		delete data_;
	}

	auto begin() const -> char* { return data_; }
	auto end() const -> char* { return data_ + file_size_; }

private:
	char const* filename_;
	FILE* file_;
	uint32_t file_size_;
	char* data_;
};


auto main(uint32_t arg_count, char const** args) -> int
{
	using namespace aeon;


	if (arg_count <= 1)
		return EXIT_FAILURE;

	input_file_t file("../../resource/basic-lexing.jigl"); //args[1]);


	// lexical analysis
	lexing::lexemes_t lexemes;
	{
		using namespace aeon::lexing;

		state_t state(lexemes);
		lex(state, aeon::lexing::stream_t(file.begin(), file.end()));
		auto B = state.lexemes().begin(aeon::lexing::multichannel_t(0xffff));
		lexemes_t::const_iterator ci = aeon::lexing::lexemes_t::iterator();

		//for (auto i = state.lexemes().begin(aeon::lexing::basic); i != state.lexemes().end(aeon::lexing::basic); ++i)
			//std::cout << *i << std::endl;
		std::cout << lexemes << std::endl;
	}
	
	// syntactic analysis
	parsing::parsemes_t parsemes;
	{
		using namespace aeon::parsing;

		parse(parsemes, lexemes);

		std::cout << parsemes << std::endl;
	}
}

