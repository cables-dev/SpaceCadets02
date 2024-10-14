#include "pch.h"
#include "../SpaceCadetsWeek2/statement.hpp"
#include "../SpaceCadetsWeek2/statement.cpp"
#include "../SpaceCadetsWeek2/runtime.hpp"

namespace barebones_tests {
	using namespace bbones;
	TEST(StatementTests, ClearTest) {
		ExecutionState state{};
		auto* clear_me = state.GetScope()->CreateVariable("ClearMe");
		auto* save_me = state.GetScope()->CreateVariable("SaveMe");
		*clear_me = 100;
		*save_me = 200;

		ASSERT_EQ(state.GetScope()->GetVariable("ClearMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("ClearMe").value()->GetValue(), 100);
		ASSERT_EQ(state.GetScope()->GetVariable("SaveMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("SaveMe").value()->GetValue(), 200);

		auto* clear_statement = new ClearStatement{};
		clear_statement->Execute(state, {"ClearMe"});

		ASSERT_EQ(state.GetScope()->GetVariable("ClearMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("ClearMe").value()->GetValue(), 0);
		ASSERT_EQ(state.GetScope()->GetVariable("SaveMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("SaveMe").value()->GetValue(), 200);

		delete clear_statement;
	}

	TEST(StatementTests, IncrementTest) {
		ExecutionState state{};
		auto* incr_me = state.GetScope()->CreateVariable("IncrMe");
		*incr_me = 250;

		ASSERT_EQ(state.GetScope()->GetVariable("IncrMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("IncrMe").value()->GetValue(), 250);

		auto* incr_statement = new IncrementStatement{};
		incr_statement->Execute(state, {"IncrMe"});

		ASSERT_EQ(state.GetScope()->GetVariable("IncrMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("IncrMe").value()->GetValue(), 251);

		delete incr_statement;
	}

	TEST(StatementTests, DecrementTest) {
		ExecutionState state{};
		auto* decr_me = state.GetScope()->CreateVariable("DecrMe");
		*decr_me = 250;

		ASSERT_EQ(state.GetScope()->GetVariable("DecrMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("DecrMe").value()->GetValue(), 250);

		auto* decr_statement = new DecrementStatement{};
		decr_statement->Execute(state, {"DecrMe"});

		ASSERT_EQ(state.GetScope()->GetVariable("DecrMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("DecrMe").value()->GetValue(), 249);

		delete decr_statement;
	}

	TEST(StatementTests, CopyTest) {
		ExecutionState state{};
		auto* copy_me = state.GetScope()->CreateVariable("CopyMe");
		auto* write_me = state.GetScope()->CreateVariable("WriteMe");
		*copy_me = 100;
		*write_me = 200;

		ASSERT_EQ(state.GetScope()->GetVariable("CopyMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("CopyMe").value()->GetValue(), 100);
		ASSERT_EQ(state.GetScope()->GetVariable("WriteMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("WriteMe").value()->GetValue(), 200);

		auto* clear_statement = new CopyStatement{};
		clear_statement->Execute(state, {"CopyMe", "to", "WriteMe"});

		ASSERT_EQ(state.GetScope()->GetVariable("CopyMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("CopyMe").value()->GetValue(), 100);
		ASSERT_EQ(state.GetScope()->GetVariable("WriteMe").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("WriteMe").value()->GetValue(), state.GetScope()->GetVariable("CopyMe").value()->GetValue());

		delete clear_statement;
	}

	TEST(StatementTests, WhileTest) {
		ExecutionState state{};
		ASSERT_EQ(state.GetCursor().GetOrdinal(), 0);

		state.IncrementCursor();
		state.IncrementCursor();
		state.IncrementCursor();
		ASSERT_EQ(state.GetCursor().GetOrdinal(), 3);

		auto* counter = state.GetScope()->CreateVariable("ctr");
		*counter = 100;

		auto* while_statement = new WhileStatement{};
		while_statement->Execute(state, { "ctr", "not", "0", "do" });

		ASSERT_EQ(state.GetLoopCondition().has_value(), true);
		ASSERT_EQ(state.GetLoopCondition().value().GetTop().GetOrdinal(), 3);

		delete while_statement;
	}

	TEST(StatementTests, EndTest) {
		ExecutionState state{};
		ASSERT_EQ(state.GetCursor().GetOrdinal(), 0);

		auto* counter = state.GetScope()->CreateVariable("ctr");
		*counter = 0;

		state.IncrementCursor();
		auto* while_statement = new WhileStatement{};
		while_statement->Execute(state, { "ctr", "not", "0", "do" });

		ASSERT_EQ(state.GetLoopCondition().has_value(), true);

		state.IncrementCursor();
		auto* end_statement = new EndStatement{};
		end_statement->Execute(state, {});

		ASSERT_EQ(state.GetLoopCondition().has_value(), false);

		delete while_statement;
		delete end_statement;
	}


	TEST(StatementTests, FakeLoopTest) {
		ExecutionState state{};
		auto* counter = state.GetScope()->CreateVariable("ctr");
		*counter = 5;

		// Loop statements start at ordinal 1
		state.IncrementCursor();
	
		auto* while_statement = new WhileStatement{};
		while_statement->Execute(state, { "ctr", "not", "0", "do" });

		// Create Fake variable
		state.GetScope()->CreateVariable("disappearing");
		ASSERT_EQ(state.GetScope()->GetVariable("disappearing").has_value(), true);

		auto* decr_statement = new DecrementStatement{};
		auto* end_statement = new EndStatement{};
		for (int i{}; i < 5; i++)
		{
			state.IncrementCursor();
			decr_statement->Execute(state, {"ctr"});
			state.IncrementCursor();
			end_statement->Execute(state, {});

			if (state.GetCursor().GetOrdinal() != 1)
				break;
		}
		delete decr_statement;
		delete end_statement;
		delete while_statement;

		ASSERT_EQ(state.GetScope()->GetVariable("disappearing").has_value(), false);
		ASSERT_EQ(state.GetScope()->GetVariable("ctr").has_value(), true);
		ASSERT_EQ(state.GetScope()->GetVariable("ctr").value()->GetValue(), 0);
	}

}
