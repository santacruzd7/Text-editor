#ifndef CURSORHOME_HPP
#define CURSORHOME_HPP


#include "Command.hpp"

class CursorHome : public Command
{
public:
    virtual void execute(Editor& editor)
	{
		// Store the column number, so that it can be reset when the command is undone
		prevColNumber = editor.cursorColumn();
		editor.cursorHome();	
	}	
	
    virtual void undo(Editor& editor)
    {
    	editor.setCursorColumn(prevColNumber);
    }
    
private:
	int prevColNumber;
};


#endif