#include "runtime.hpp"
#include <filesystem>
#include <fstream>

namespace bbones {

Variable::Variable(int x)
	: m_val{x}
{
}

int Variable::GetValue() const
{
	return m_val;
}

void Variable::SetValue(int x)
{
	m_val = x;
}

Variable& Variable::operator=(int x)
{
	SetValue(x);
	return *this;
}

Variable& Variable::operator=(const Variable& x)
{
	if (this == &x)
		return *this;

	SetValue(x.GetValue());
	return *this;
}

std::optional<Scope* const> Scope::GetParentScope() const
{
	return m_parent;
}

std::optional<Variable*> Scope::GetVariable(const std::string& name)
{
	// Do we have a variable stored for this identifier?
	if (m_vars.find(name) == m_vars.end())
	{
		auto parent = GetParentScope();

		// If we do not have a parent scope, we return None
		if (!parent.has_value())
			return std::nullopt;
		return parent.value()->GetVariable(name);
	}

	return &m_vars.at(name);
}

Variable* Scope::CreateVariable(const std::string& name)
{
	if (GetVariable(name).has_value())
	{
		throw std::runtime_error{"Tried to create variable \"" + name + "\" when that variable already exists!"};
	}
	m_vars.insert({ name, Variable{} });
	return &m_vars.at(name);
}

std::string Scope::GetStateString()
{
	std::string result{};
	for (auto& [var_name, var] : m_vars)
	{
		result += var_name + " is " + std::to_string(var.GetValue()) + ". ";
	}
	auto parent = GetParentScope();
	if (parent.has_value())
		return result + parent.value()->GetStateString();
	return result;
}

Scope::Scope(std::optional<Scope*> parent)
	: m_parent{(parent.has_value() && parent.value() == nullptr) ? std::nullopt : parent}		// If the nullptr was passed in we treat this as nullopt
{
}


ExecutionState::ExecutionState()
{
	PushScope();
}

Scope* const ExecutionState::PushScope()
{
	m_scope_stack.push_back(std::unique_ptr<Scope>{new Scope{ GetScope() }});
	return GetScope();
}

void ExecutionState::PopScope()
{
	m_scope_stack.pop_back();
}

Scope* const ExecutionState::GetScope()
{
	if (m_scope_stack.empty())
		return nullptr;

	return m_scope_stack.back().get();
}

void ExecutionState::PushLoopCondition(const LoopCondition& cond)
{
	m_loop_conditions.push_back(cond);
}

void ExecutionState::PopLoopCondition()
{
	m_loop_conditions.pop_back();
}

std::optional<LoopCondition> ExecutionState::GetLoopCondition()
{
	if (m_loop_conditions.size() > 0)
		return m_loop_conditions.back();
	return std::nullopt;
}

ExecutionCursor ExecutionState::GetCursor()
{
	return m_ip;
}

void ExecutionState::IncrementCursor()
{
	m_ip = ExecutionCursor{ m_ip.GetOrdinal() + 1 };
}

void ExecutionState::SetCursor(const ExecutionCursor& pos)
{
	m_ip = pos;
}

std::string ExecutionState::GetStateString()
{
	return "IP is " + std::to_string(GetCursor().GetOrdinal()) + ". " + GetScope()->GetStateString();
}

LoopCondition::LoopCondition(const std::function<bool()>& condition, const std::function<void()>& advance, ExecutionCursor beginning)
	: m_pred{ condition }, m_next{ advance }, m_top { beginning }
{
}

bool LoopCondition::IsDone() const
{
	return m_pred();
}

void LoopCondition::Advance()
{
	m_next();
}

ExecutionCursor LoopCondition::GetTop() const
{
	return m_top;
}

ExecutionCursor::ExecutionCursor(size_t ord)
	: m_ord{ord}
{
}

size_t ExecutionCursor::GetOrdinal() const
{
	return m_ord;
}

std::string BaseProgram::CleanInstruction(const std::string& insn)
{
	static const std::unordered_set<char> whitespace{ ' ', '\n', '\r', '\t' };
	int l = 0;
	while (whitespace.contains(insn[l]))
		l += 1;
	int r = insn.size() - 1;
	while (whitespace.contains(insn[r]))
		r -= 1;

	if (l >= r)
		return "";
	return insn.substr(l, (r - l + 1));
}

BaseProgram::BaseProgram(const std::string& program_text)
	: m_text{program_text}
{
}

// O(n) fail :facepalm:
// Get the nth statement from file
std::optional<std::string> BaseProgram::Fetch(const ExecutionCursor& ip)
{
	int num = 0;
	size_t last_pos = 0;
	size_t pos = 0;

	for (pos = m_text.find(';'); pos != std::string::npos; pos = m_text.find(';', last_pos))
	{
		if (ip.GetOrdinal() == num)
			return CleanInstruction(m_text.substr(last_pos, (pos-last_pos)));

		last_pos = pos + 1;
		num += 1;
	}

	if (ip.GetOrdinal() == num)
	{
		auto substr = m_text.substr(last_pos, m_text.size());
		if (substr.size() > 0)
			return CleanInstruction(substr);
	}

	return std::nullopt;
}

std::optional<BaseProgram*> CreateProgramFromFile(const std::string& path)
{
	if (!std::filesystem::exists(path))
		return std::nullopt;

    std::ifstream infile{path};
	std::string file_content{};
	std::string buff{};
	while (std::getline(infile, buff))
		file_content.append(buff);
		
	return { new BaseProgram{ file_content } };
}

}
