#ifndef BACKSPACE_HPP
#define BACKSPACE_HPP


#include "Command.hpp"

class Backspace : public Command
{
public:
    virtual void execute(Editor& editor)
    {
    	// Store the column number, so that it can be used to determine the action to 
    	// be taken when the command is undone
		initialColumn = editor.cursorColumn();
		
		// If the column is not the first one, we store the character to be deleted in 
		// the member variable (so that it can be restored when the command is undone). 
		// Otherwise, no character is deleted, but the change of line instead
		if(initialColumn != 1)
		{
			deletedChar = editor.line(editor.cursorLine()).at(initialColumn-2);
		}
		editor.backspace();
    }
    
    virtual void undo(Editor& editor)
    {
    	// If the cursor was at the beginning of the line, the way to undo backspace() 
    	// is using newLine()
    	if(initialColumn == 1)	
		{
			editor.newLine();
		}
		// Otherwise, the way to undo backspace() is using insertChar() with the 
		// deleted character
		else	
		{
			editor.insertChar(deletedChar);
		}
    }
    
private:
	char initialColumn;
	char deletedChar;
};


#endif