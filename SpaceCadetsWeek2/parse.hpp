#pragma once
#include "common.hpp"
#include "statement.hpp"

namespace bbones {

class Parser {
public:
	std::unordered_map<std::string, IStatement*> m_mappings{};
	std::optional<IStatement*> GetStatementFor(const std::string& keyword);
	std::vector<std::string> ParseArgs(int after_pos, const std::string& statement);

public:
	Parser() = default;

	void AddMapping(const std::string& keyword, IStatement* statement);

	using StatementAndArgs_t = std::pair<IStatement*, std::vector<std::string>>;
	std::optional<StatementAndArgs_t> Parse(const std::string& statement);
};

class ParserBuilder {
private:
	Parser m_acc{};

public:
	ParserBuilder() = default;

	void AddMapping(const std::string& keyword, IStatement* statement);
	Parser Finish();
};
}
