#include "statement.hpp"

namespace bbones {

void ClearStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto target_name = args[0];
	auto* scope = state.GetScope();
	auto var = scope->GetVariable(target_name);

	if (!var.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + target_name + "\" when it does not exist!");

	var.value()->SetValue(0);
}

void IncrementStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto target_name = args[0];
	auto* scope = state.GetScope();
	auto var = scope->GetVariable(target_name);
	if (!var.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + target_name + "\" when it does not exist!");

	auto var_value = var.value()->GetValue();
	var.value()->SetValue(var_value + 1);
}

void DecrementStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto target_name = args[0];
	auto* scope = state.GetScope();
	auto var = scope->GetVariable(target_name);
	if (!var.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + target_name + "\" when it does not exist!");

	auto var_value = var.value()->GetValue();
	var.value()->SetValue(var_value - 1);
}

void CopyStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto src_name = args[0];
	auto dst_name = args[2];
	auto* scope = state.GetScope();

	auto src = scope->GetVariable(src_name);
	if (!src.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + src_name + "\" when it does not exist!");
	auto dst = scope->GetVariable(dst_name);
	if (!dst.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + dst_name + "\" when it does not exist!");

	auto src_val = src.value()->GetValue();
	dst.value()->SetValue(src_val);
}

void WhileStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto var_name = args[0];
	auto* scope = state.GetScope();

	auto var = scope->GetVariable(var_name);
	if (!var.has_value())
		throw std::runtime_error("Error: tried to access variable \"" + var_name + "\" when it does not exist!");

	auto* var_ptr = var.value();
	auto is_finished = [var_ptr]() {
		return var_ptr->GetValue() == 0;
	};
	auto advance = []() {};
	LoopCondition condition{ is_finished, advance, state.GetCursor() };
	state.PushLoopCondition(condition);
	state.PushScope();
}

void EndStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto loop_condition = state.GetLoopCondition();
	if (!loop_condition.has_value())
		throw std::runtime_error("Error: \"end\" used where a valid loop was not previously open.");

	if (loop_condition.value().IsDone())
	{
		state.PopScope();
		state.PopLoopCondition();
		return;
	}
	else
		state.SetCursor(loop_condition.value().GetTop());
}

void InitStatement::Execute(ExecutionState& state, const std::vector<std::string>& args)
{
	auto var_name = args[0];
	auto* scope = state.GetScope();

	auto var = scope->CreateVariable(var_name);
}

}
