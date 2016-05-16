#ifndef DELETELINE_HPP
#define DELETELINE_HPP


#include <string>
#include "Command.hpp"

class DeleteLine : public Command
{
public:
    virtual void execute(Editor& editor)
    {
    	// Store the column and line number, so that they can be restored when the 
    	// command is undone
		initialColumn = editor.cursorColumn();
		initialLine = editor.cursorLine();
		// Store the current line, so that it can be restored when the command is undone
		deletedLine = editor.line(initialLine);
		editor.deleteLine();
    }
    
    virtual void undo(Editor& editor)
    {
    	// The function that undoes deleteLine() is insertLine() with the deleted line, 
    	// logically, except if the cursor column had to be reset to the point beyond 
    	// the end of the new line when the command was executed
		editor.insertLine(deletedLine, initialLine);
		// In such case, the cursor is reset to the original column
		editor.setCursorColumn(initialColumn);
    }
    
private:
	int initialColumn;
	int initialLine;
	std::string deletedLine;
};


#endif