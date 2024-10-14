#include "lang.hpp"
#include <iostream>

bbones::Parser CreateParser()
{
	bbones::ParserBuilder parser = bbones::ParserBuilder{};
	parser.AddMapping("init", new bbones::InitStatement{});
	parser.AddMapping("incr", new bbones::IncrementStatement{});
	parser.AddMapping("decr", new bbones::DecrementStatement{});
	parser.AddMapping("clear", new bbones::ClearStatement{});
	parser.AddMapping("while", new bbones::WhileStatement{});
	parser.AddMapping("copy", new bbones::CopyStatement{});
	parser.AddMapping("end", new bbones::EndStatement{});

	return parser.Finish();
}

std::string GetFilePathFromUser()
{
	std::string result{};
	std::cout << "Enter the file path of a BareBones program: ";
	std::getline(std::cin, result);
	return result;
}

int main()
{
	auto parser = CreateParser();
	auto program_result = bbones::CreateProgramFromFile(GetFilePathFromUser());
	if (!program_result.has_value())
		throw std::runtime_error("Error: the provided filepath could not be opened!");

	auto* program = program_result.value();
	bbones::BareBones bones_instance{ parser, program };

	bones_instance.Execute();
	std::cout << "success!\n";

	return 0;
}
