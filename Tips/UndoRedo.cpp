#include "UndoRedo.h"

void i_command::execute()
{
}

void i_command::undo()
{
}

void command_manager::execute_command(const command_ptr& command)
{
	command->execute();
	undo_stack_.push(command);
	// Clear the redo stack when a new command is executed.
	while (!redo_stack_.empty()) {
		redo_stack_.pop();
	}
}

void command_manager::undo()
{
	if (!undo_stack_.empty()) {
		const command_ptr command = undo_stack_.top();
		command->undo();
		undo_stack_.pop();
		redo_stack_.push(command);
	}
	else {
		std::cout << "Nothing to undo." << std::endl;
	}
}

void command_manager::redo()
{
	if (!redo_stack_.empty()) {
		const command_ptr command = redo_stack_.top();
		command->execute();
		redo_stack_.pop();
		undo_stack_.push(command);
	}
	else {
		std::cout << "Nothing to redo." << std::endl;
	}
}

concrete_command::concrete_command(std::string action): action_(std::move(action))
{}

void concrete_command::execute()
{
	previous_action_ = action_;
	std::cout << "Execute: " << action_ << std::endl;
}

void concrete_command::undo()
{
	std::cout << "Undo: " << previous_action_ << std::endl;
}

i_command::~i_command()
= default;
