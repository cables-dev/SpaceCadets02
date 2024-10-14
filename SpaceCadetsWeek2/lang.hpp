#pragma once
#include "common.hpp"
#include "statement.hpp"
#include "parse.hpp"
#include "runtime.hpp"

namespace bbones {

class BareBones {
private:
	Parser m_parser{};
	ExecutionState m_cpu{};
	IProgram* m_program{};
	bool m_finished{};

	void Step();
	void Finish();
	bool IsFinished();
	void PrintState();

public:
	BareBones(const Parser& parser, IProgram* program);

	void Execute();
};

}