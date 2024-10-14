#include "lang.hpp"
#include <iostream>

namespace bbones {

void BareBones::Step()
{
	auto rip = m_cpu.GetCursor();
	auto insn_str = m_program->Fetch(rip);
	if (!insn_str.has_value())
	{
		Finish();
		return;
	}

	auto insn_and_args = m_parser.Parse(insn_str.value());
	if (!insn_and_args.has_value())
		throw std::runtime_error{ "BareBones: instruction " + insn_str.value() + " is not recognised!"};

	m_cpu.IncrementCursor();
	auto [insn, args] = insn_and_args.value();
	insn->Execute(m_cpu, args);
}

void BareBones::Finish()
{
	m_finished = true;
}

bool BareBones::IsFinished()
{
	return m_finished;
}

void BareBones::PrintState()
{
	std::cout << m_cpu.GetStateString() << '\n';
}

BareBones::BareBones(const Parser& parser, IProgram* program)
	: m_cpu{}, m_parser{parser}, m_program{program}
{
}

void BareBones::Execute()
{
	while (!IsFinished())
	{
		PrintState();
		Step();
	}
}

}
