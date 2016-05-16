#ifndef CURSOREND_HPP
#define CURSOREND_HPP


#include "Command.hpp"

class CursorEnd : public Command
{
public:
    virtual void execute(Editor& editor)
	{
		// Store the column number, so that it can be reset when the command is undone
		prevColNumber = editor.cursorColumn();
		editor.cursorEnd();
	}

    virtual void undo(Editor& editor)
    {
    	editor.setCursorColumn(prevColNumber);
    }
    
private:
	int prevColNumber;
};


#endif