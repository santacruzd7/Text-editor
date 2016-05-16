#ifndef CURSORRIGHT_HPP
#define CURSORRIGHT_HPP


#include "Command.hpp"

class CursorRight : public Command
{
public:
    virtual void execute(Editor& editor)
	{
		editor.cursorRight();
	}
	
    virtual void undo(Editor& editor)
	{
		// The function that undoes cursorRight() is cursorLeft(), logically
		editor.cursorLeft();
	}
};


#endif