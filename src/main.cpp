#include <cstdint>
#include <jigl/lexing/lex.hpp>
#include <iostream>

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
	if (arg_count <= 1)
		return EXIT_FAILURE;

	input_file_t file(args[1]);

	for (auto x : file)
		std::cout << x;

	jigl::lexing::lexemes_t lexemes;
	jigl::lexing::lex(lexemes, jigl::lexing::stream_t(file.begin(), file.end()));
}
