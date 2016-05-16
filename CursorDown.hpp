#ifndef CURSORDOWN_HPP
#define CURSORDOWN_HPP


#include "Command.hpp"

class CursorDown : public Command
{
public:
    virtual void execute(Editor& editor)
    {
		// Store the column number, so that it can be reset in case the command is undone 
		// (it may happen that when the cursor is moved down, the nexy line is too short 
		// and the cursor column needs to be set to the point beyond the end of the next 
		// line)
		prevColNumber = editor.cursorColumn();
		editor.cursorDown();
    }
    
    virtual void undo(Editor& editor)
    {
		// The function that undoes cursorDown() is cursorUp(), logically, except if the 
		// cursor column had to be reset to the point beyond the end of the next line 
		// when the command was executed
		editor.cursorUp();
		// In such case, the cursor is reset to the original column
		editor.setCursorColumn(prevColNumber);
    }
private:
	int prevColNumber;
};


#endif