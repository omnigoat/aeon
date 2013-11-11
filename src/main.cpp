#include <cstdint>
#include <iostream>

#include <aeon/lexing/lex.hpp>
#include <aeon/lexing/id.hpp>
#include <aeon/parsing/parse.hpp>
#include <aeon/semantics/analyse.hpp>

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
	lexing::lexemes_t lexemes;
	{
		using namespace aeon::lexing;

		state_t state(lexemes);
		lex(state, aeon::lexing::stream_t(file.begin(), file.end()));
		auto B = state.lexemes().begin(aeon::lexing::multichannel_t(0xffff));
		lexemes_t::const_iterator ci = aeon::lexing::lexemes_t::iterator();

		std::cout << lexemes << std::endl;
	}
	
	// syntactic analysis
	parsing::children_t parsemes;
	{
		using namespace aeon::parsing;

		parse(parsemes, lexemes);

		std::cout << parsemes << std::endl;
	}

	ATMA_ASSERT(!parsemes.empty());
	ATMA_ASSERT(parsemes[0]->id() == aeon::parsing::parseme_t::id_t::root);

	FILE* out = fopen(args[2], "w+");

	auto stream = new generation::file_output_stream_t(out);

	// collect the root
	parsing::parsemes_t root_list;
	std::copy_if(parsemes.begin(), parsemes.end(), std::back_inserter(root_list), [](parsing::parseme_ptr const& x){
		return x->id() == parsing::parseme_t::id_t::root;
	});
	auto root = root_list.back();
	ATMA_ASSERT(root);


	// collect the modules
	parsing::parsemes_t modules;
	std::copy_if(root->children().begin(), root->children().end(), std::back_inserter(modules), [](parsing::parseme_ptr const& x) {
		return x->id() == parsing::parseme_t::id_t::module;
	});

	for (auto const& x : modules)
	{
		//if (x->id() == aeon::parsing::parseme_t::id_t::function)
			//semantics::analyse::function(x);
		//}

		generation::module(*stream, x);
	}

	std::cout << parsemes << std::endl;
}

