#ifndef CURSORUP_HPP
#define CURSORUP_HPP


#include "Command.hpp"

class CursorUp : public Command
{
public:
    virtual void execute(Editor& editor)
    {
		// Store the column number, so that it can be reset in case the command is 
		// undone (it may happen that when the cursor is moved up, the previous line is 
		// too short and the cursor column needs to be set to the point beyond the end 
		// of the previous line)
		prevColNumber = editor.cursorColumn();
		editor.cursorUp();    
    }
    
    virtual void undo(Editor& editor)
    {
		// The function that undoes cursorUp() is cursorDown(), logically, except if 
		//the cursor column had to be reset to the point beyond the end of the previous 
		//line when the command was executed
		editor.cursorDown();
		// In such case, the cursor is reset to the original column
		editor.setCursorColumn(prevColNumber);
    }
    
private:
	int prevColNumber;
};


#endif