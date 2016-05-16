#ifndef CURSORLEFT_HPP
#define CURSORLEFT_HPP


#include "Command.hpp"

class CursorLeft : public Command
{
public:
    virtual void execute(Editor& editor)
	{
		editor.cursorLeft();
	}

    virtual void undo(Editor& editor)
	{
		// The function that undoes cursorLeft() is cursorRight(), logically
		editor.cursorRight();
	}
};


#endif