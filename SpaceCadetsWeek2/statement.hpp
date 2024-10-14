#pragma once
#include "common.hpp"
#include "runtime.hpp"

namespace bbones {
class IStatement 
{
public:
	virtual ~IStatement() = default;
	virtual void Execute(ExecutionState& state, const std::vector<std::string>& args) = 0;
};

class ClearStatement : public IStatement {
public:
	ClearStatement() = default;
	virtual ~ClearStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class IncrementStatement : public IStatement {
public:
	IncrementStatement() = default;
	virtual ~IncrementStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class DecrementStatement : public IStatement {
public:
	DecrementStatement() = default;
	virtual ~DecrementStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class CopyStatement : public IStatement {
public:
	CopyStatement() = default;
	virtual ~CopyStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class WhileStatement : public IStatement {
public:
	WhileStatement() = default;
	virtual ~WhileStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class EndStatement : public IStatement {
public:
	virtual ~EndStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};

class InitStatement : public IStatement {
public:
	virtual ~InitStatement() = default;

	void Execute(ExecutionState& state, const std::vector<std::string>& args) override;
};
}
