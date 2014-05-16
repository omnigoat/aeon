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



#if 0
lexemes (66)
-------------
  18[1:1]: function
  10[1:9]:
  1[1:10]: maths
  10[1:15]:
  4[1:16]: ::
  10[1:18]:
  1[1:19]: lhs
  10[1:22]:
  13[1:23]: as
  10[1:25]:
  7[1:26]: @int32
  10[1:32]:
  4[1:33]: ->
  10[1:35]:
  1[1:36]: rhs
  10[1:39]:
  13[1:40]: as
  10[1:42]:
  7[1:43]: @int32
  10[1:49]:
  4[1:50]: ->
  10[1:52]:
  7[1:53]: @int32
  5[2:3]:
  17[2:3]: return
  10[2:9]:
  1[2:10]: lhs
  10[2:13]:
  4[2:14]: *
  10[2:15]:
  1[2:16]: lhs
  10[2:19]:
  4[2:20]: -
  10[2:21]:
  1[2:22]: rhs
  10[2:25]:
  4[2:26]: *
  10[2:27]:
  1[2:28]: rhs
  10[2:31]:
  4[2:32]: +
  10[2:33]:
  2[2:34]: 7
  6[3:2]:
  14[3:2]: end
  18[6:2]: function
  10[6:10]:
  1[6:11]: main
  10[6:15]:
  4[6:16]: ::
  10[6:18]:
  4[6:19]: ->
  10[6:21]:
  7[6:22]: @int32
  5[7:3]:
  17[7:3]: return
  10[7:9]:
  1[7:10]: maths
  4[7:15]: (
  2[7:16]: 5
  4[7:17]: ,
  10[7:18]:
  2[7:19]: 4
  4[7:20]: )
  6[8:2]:
  14[8:2]: end


parsemes
----------
root:
  type-definition:
    identifier: "@int32" [0:0]
    intrinsic-type-int:
    intrinsic-bitsize: "32" [0:0]
  function:
    function-pattern:
      placeholder:
      identifier: "+" [0:0]
      placeholder:
    parameter-list:
      parameter:
        identifier: "lhs" [0:0]
        typename: "@int32" [0:0]
      parameter:
        identifier: "rhs" [0:0]
        typename: "@int32" [0:0]
    typename: "@int32" [0:0]
    block:
      return-statement:
        intrinsic-int-add: "@int32" [1:26]
          identifier: "lhs" [0:0]
          identifier: "rhs" [0:0]
    27:
      28:
  function:
    function-pattern:
      placeholder:
      identifier: "-" [0:0]
      placeholder:
    parameter-list:
      parameter:
        identifier: "lhs" [0:0]
        typename: "@int32" [0:0]
      parameter:
        identifier: "rhs" [0:0]
        typename: "@int32" [0:0]
    typename: "@int32" [0:0]
    block:
      return-statement:
        intrinsic-int-sub: "@int32" [1:26]
          identifier: "lhs" [0:0]
          identifier: "rhs" [0:0]
    27:
      28:
  function:
    function-pattern:
      placeholder:
      identifier: "*" [0:0]
      placeholder:
    parameter-list:
      parameter:
        identifier: "lhs" [0:0]
        typename: "@int32" [0:0]
      parameter:
        identifier: "rhs" [0:0]
        typename: "@int32" [0:0]
    typename: "@int32" [0:0]
    block:
      return-statement:
        intrinsic-int-mul: "@int32" [1:26]
          identifier: "lhs" [0:0]
          identifier: "rhs" [0:0]
    27:
      28:
  function:
    function-pattern:
      placeholder:
      identifier: "/" [0:0]
      placeholder:
    parameter-list:
      parameter:
        identifier: "lhs" [0:0]
        typename: "@int32" [0:0]
      parameter:
        identifier: "rhs" [0:0]
        typename: "@int32" [0:0]
    typename: "@int32" [0:0]
    block:
      return-statement:
        intrinsic-int-div: "@int32" [1:26]
          identifier: "lhs" [0:0]
          identifier: "rhs" [0:0]
    27:
      28:
  module:
    function: "function" [1:1]									   function: "function" [1:1]
      function-pattern: "maths" [1:10]							     identifier: "maths" [1:10]
        identifier: "maths" [1:10]								     parameter-list: "::" [1:16]
      parameter-list: "::" [1:16]								       parameter:
        parameter:												         identifier: "lhs" [1:19]
          identifier: "lhs" [1:19]								         typename: "@int32" [1:26]
          typename: "@int32" [1:26]								       parameter:
        parameter:												         identifier: "rhs" [1:36]
          identifier: "rhs" [1:36]								         typename: "@int32" [1:43]
          typename: "@int32" [1:43]								     typename: "@int32" [1:53]
      typename: "@int32" [1:53]									     block:
      block:													       return-statement: "return" [2:3]
        return-statement: "return" [2:3]						         function_call: "+" [2:32]
          function_call: "+" [2:32]								           function-pattern:
            function-pattern:									             placeholder: "-" [2:20]
              placeholder: "-" [2:20]							             identifier: "+" [2:32]
              identifier: "+" [2:32]							             placeholder: "7" [2:34]
              placeholder: "7" [2:34]							           argument-list:
            argument-list:										             function_call: "-" [2:20]
              function_call: "-" [2:20]							               function-pattern:
                function-pattern:								                 placeholder: "*" [2:14]
                  placeholder: "*" [2:14]						                 identifier: "-" [2:20]
                  identifier: "-" [2:20]						                 placeholder: "*" [2:26]
                  placeholder: "*" [2:26]						               argument-list:
                argument-list:									                 function_call: "*" [2:14]
                  function_call: "*" [2:14]						                   function-pattern:
                    function-pattern:							                     placeholder: "lhs" [2:10]
                      placeholder: "lhs" [2:10]					                     identifier: "*" [2:14]
                      identifier: "*" [2:14]					                     placeholder: "lhs" [2:16]
                      placeholder: "lhs" [2:16]					                   argument-list:
                    argument-list:								                     identifier: "lhs" [2:10]
                      identifier: "lhs" [2:10]					                     identifier: "lhs" [2:16]
                      identifier: "lhs" [2:16]					                 function_call: "*" [2:26]
                  function_call: "*" [2:26]						                   function-pattern:
                    function-pattern:							                     placeholder: "rhs" [2:22]
                      placeholder: "rhs" [2:22]					                     identifier: "*" [2:26]
                      identifier: "*" [2:26]					                     placeholder: "rhs" [2:28]
                      placeholder: "rhs" [2:28]					                   argument-list:
                    argument-list:								                     identifier: "rhs" [2:22]
                      identifier: "rhs" [2:22]					                     identifier: "rhs" [2:28]
                      identifier: "rhs" [2:28]					             integer-literal: "7" [2:34]
              integer-literal: "7" [2:34]
    function: "function" [6:2]
      function-pattern: "main" [6:11]
        identifier: "main" [6:11]
      parameter-list: "::" [6:16]
      typename: "@int32" [6:22]
      block:
        return-statement: "return" [7:3]
          function_call: "maths" [7:10]
            function-pattern:
              identifier: "maths" [7:10]
            argument-list:
              integer-literal: "5" [7:16]
              integer-literal: "4" [7:19]

parsemes
----------












































#endif
