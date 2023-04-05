#pragma once

#include <iostream>
#include <memory>
#include <stack>

class i_command {
public:
	virtual ~i_command();
	virtual void execute();
	virtual void undo();
};

using command_ptr = std::shared_ptr<i_command>;

class command_manager {
public:
	void execute_command(const command_ptr& command);

	void undo();

	void redo();
private:
	std::stack<command_ptr> undo_stack_;
	std::stack<command_ptr> redo_stack_;
};

class concrete_command final : public i_command
{
public:
	explicit concrete_command(std::string action);

	void execute() override;

	void undo() override;
private:
	std::string action_;
	std::string previous_action_;
};

//int main() {
//	CommandManager manager;
//	CommandPtr command1 = std::make_shared<ConcreteCommand>("Action 1");
//	CommandPtr command2 = std::make_shared<ConcreteCommand>("Action 2");
//	CommandPtr command3 = std::make_shared<ConcreteCommand>("Action 3");
//
//	manager.executeCommand(command1);
//	manager.executeCommand(command2);
//	manager.executeCommand(command3);
//
//	manager.undo();
//	manager.undo();
//	manager.redo();
//	manager.redo();
//	manager.undo();
//
//	return 0;
//}
class UndoRedo
{
};

