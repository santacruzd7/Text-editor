#ifndef NEWLINE_HPP
#define NEWLINE_HPP


#include "Command.hpp"

class NewLine : public Command
{
public:
    virtual void execute(Editor& editor)
    {
   		editor.newLine();
    }
    
    virtual void undo(Editor& editor)
    {
    	// The function that undoes newLine() is backspace() [in the case when the cursor 
    	// is at the beginning of the line]
		editor.backspace();
    }
};


#endif