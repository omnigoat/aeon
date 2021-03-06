#include <cstdint>
#include <iostream>

#include <aeon/lexing/lex.hpp>
#include <aeon/lexing/id.hpp>
#include <aeon/lexing/lexical_analysis.hpp>

#include <aeon/parsing/syntactic_analysis.hpp>
#include <aeon/parsing/parse.hpp>
#include <aeon/parsing/algorithm.hpp>

#include <aeon/semantics/analyse.hpp>
#include <aeon/optimization/inlining.hpp>
#include <aeon/generation/generation.hpp>

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

	auto close() -> void
	{
		fflush(file_);
		fclose(file_);
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

	input_file_t file(args[1]);


	// lexical analysis
	//auto lexemes = lexing::lexemes_t();
	auto lexical_analysis = aeon::lexing::lexical_analysis_t(file.begin(), file.end());
	//{
	//	using namespace aeon::lexing;

	//	auto LA = aeon::lexing::lexical_analysis_t(file.begin(), file.end());

	//	state_t state(lexemes);
	//	lex(state, aeon::lexing::stream_t(file.begin(), file.end()));

	//	//ATMA_ASSERT(lexemes.begin() == LA.lexemes());
	//	std::cout << LA.lexemes() << std::endl << lexemes << std::endl;
	//}
	std::cout << lexical_analysis.lexemes() << std::endl;

	
	// syntactic analysis
	auto syntactic_analysis = aeon::parsing::syntactic_analysis_t(lexical_analysis);
	parsing::children_t& parsemes = syntactic_analysis.parsemes();
	{
		using namespace aeon::parsing;

		if (!syntactic_analysis.errors().empty()) {
			for (auto const& e : syntactic_analysis.errors())
				std::cout << e << std::endl;
		}
		//auto errors = parsing::errors_t(args[1], file.begin());
		//parse(errors, parsemes, lexical_analysis.lexemes());

		//if (errors.size() > 0) {
			//std::cout << errors << std::endl;
			//return EXIT_FAILURE;
		//}

		//std::cout << parsemes << std::endl;
		std::cout << syntactic_analysis.parsemes() << std::endl;
	}

	FILE* out = fopen(args[2], "w");

	auto stream = new generation::file_output_stream_t(out);

	optimization::inline_all_the_things(parsemes);

	// intrinsic functions
	{
		parsing::parsemes_t functions;
		std::copy_if(parsemes.begin(), parsemes.end(), std::back_inserter(functions), parsing::id_equals_t(parsing::ID::function));

		for (auto const& x : functions) {
			generation::genesis_t g;
			generation::analyse::function(g, x);
			generation::function(*stream, g, x);
		}
	}

	// modules
	{
		parsing::parsemes_t modules;
		std::copy_if(parsemes.begin(), parsemes.end(), std::back_inserter(modules), parsing::id_equals_t(parsing::ID::module));

		for (auto const& x : modules)
		{
			generation::module(*stream, x);
		}
	}

	//std::cout << parsemes << std::endl;

	fflush(out);
	fclose(out);

	system("cd");
	printf("generating code...\n");
	system("..\\..\\bin\\llvm-as ../../resource/basic-lexing.out");
	system("..\\..\\bin\\run_and_show_return_value.bat ..\\..\\bin\\lli ../../resource/basic-lexing.out.bc");
	printf("\n");
	system("pause");
}

