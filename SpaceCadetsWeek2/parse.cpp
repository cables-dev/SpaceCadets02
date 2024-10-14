#include "parse.hpp"

namespace bbones {

std::optional<IStatement*> Parser::GetStatementFor(const std::string& keyword)
{
	if (m_mappings.find(keyword) == m_mappings.end())
		return std::nullopt;

	return std::optional<IStatement*>(m_mappings.at(keyword));
}

std::vector<std::string> Parser::ParseArgs(int after_pos, const std::string& statement)
{
	std::vector<std::string> result{};
	int l = after_pos;
	int r = after_pos;
	for (; r < statement.size(); r++)
	{
		if (statement[r] == ' ')
		{
			if (r != l)
				result.push_back(statement.substr(l, (r - l)));

			l = r + 1;
		}
	}
	if (l < r && l < statement.size() && r <= statement.size())
		result.push_back(statement.substr(l, r - l));

	return result;
}

void Parser::AddMapping(const std::string& keyword, IStatement* statement)
{
	m_mappings.insert({ keyword, statement });//std::make_shared<IStatement>(statement) });
}

std::optional<Parser::StatementAndArgs_t> Parser::Parse(const std::string& statement)
{
	// Find the first word of the statement
	std::string first_word{};
	int r = 0;

	// Skip leading whitespace
	while (statement[r] == ' ')
		r += 1;
	for (; r < statement.size(); r++)
	{
		if (statement[r] == ' ' && r > 0)
		{
			first_word = statement.substr(0, r);
			break;
		}
	}

	if (first_word.empty())
		first_word = statement;

	auto insn_statement = GetStatementFor(first_word);
	if (!insn_statement.has_value())
		return std::nullopt;

	auto args = ParseArgs(r+1, statement);
	return { StatementAndArgs_t{ insn_statement.value(), args}};
}

void ParserBuilder::AddMapping(const std::string& keyword, IStatement* statement)
{
	// lol
	m_acc.AddMapping(keyword, statement);
}

Parser ParserBuilder::Finish()
{
	auto tmp = m_acc;
	m_acc = {};
	return tmp;
}

}
