#ifndef INSERTCHAR_HPP
#define INSERTCHAR_HPP


#include "Command.hpp"

class InsertChar : public Command
{
public:
	InsertChar(char c)
		: character{c}
	{
	}
    
    virtual void execute(Editor& editor)
    {
   		editor.insertChar(character);
    }
    
    virtual void undo(Editor& editor)
    {
    	// The function that undoes insertChar() is backspace(), logically
		editor.backspace();
    }
    
private:
	char character;
};


#endif