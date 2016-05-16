// CommandProcessor.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// Implementation of the CommandProcessor class

#include <vector>
#include <stack>

#include "CommandProcessor.hpp"
#include "EditorException.hpp"
#include "Keypress.hpp"
#include "CursorUp.hpp"
#include "CursorDown.hpp"
#include "CursorLeft.hpp"
#include "CursorRight.hpp"
#include "CursorHome.hpp"
#include "CursorEnd.hpp"
#include "NewLine.hpp"
#include "Backspace.hpp"
#include "DeleteLine.hpp"
#include "InsertChar.hpp"



CommandProcessor::CommandProcessor(Editor& editor, EditorView& view)
    : editor{editor}, view{view}
{
}


namespace
{
    // An "enum class" defines a type that has one of the constant values
    // specified.  So, in this case, a variable of type UserInteractionType
    // could have the value UserInteractionType::command,
    // UserInteractionType::undo, etc.
    enum class UserInteractionType
    {
        command,
        undo,
        redo,
        quit
    };


    // A UserInteraction describes one interaction that the user undertakes
    // with BooEdit.  There are four different kinds of interactions:
    //
    // * Commands, which attempt to affect some kind of change on the editor
    // * Undo, which asks for the previous change to be undone
    // * Redo, which asks for the most-recently undone change to be redone
    // * Quit, which asks for the editor to be stopped altogether
    //
    // A Command* is included for the case where UserInteractionType::command
    // is the specified type; otherwise, it will be nullptr.
    struct UserInteraction
    {
        UserInteractionType type;
        Command* command;
    };



    // These four functions return four types of UserInteraction, depending on whether 
    // the interaction is a Command, undo, redo or quit
    UserInteraction makeCommandInteraction(Command* command)
    {
        return UserInteraction{UserInteractionType::command, command};
    }


    UserInteraction makeUndoInteraction()
    {
        return UserInteraction{UserInteractionType::undo, nullptr};
    }


    UserInteraction makeRedoInteraction()
    {
        return UserInteraction{UserInteractionType::redo, nullptr};
    }

    
    UserInteraction makeQuitInteraction()
    {
        return UserInteraction{UserInteractionType::quit, nullptr};
    }


    // This function watches for the right keypresses and build the right kinds of 
    // user interactions.  
    // It handles commands, undo, redo and quit
    UserInteraction nextUserInteraction()
    {
        while (true)
        {
            Keypress keypress = nextKeypress();

            if (keypress.ctrl)
            {
                // The user pressed a Ctrl key (e.g., Ctrl+X); react accordingly
                switch (keypress.c)
                {
                case 'I':	// Cursor up
                	return makeCommandInteraction(new CursorUp());
                	break;
                case 'K':	// Cursor down
                	return makeCommandInteraction(new CursorDown());
                	break;
                case 'U':	// Cursor left
                	return makeCommandInteraction(new CursorLeft());
                	break;
                case 'O':	// Cursor right
                	return makeCommandInteraction(new CursorRight());
                	break;
                case 'Y':	// Cursor home
                	return makeCommandInteraction(new CursorHome());
                	break;
               	case 'P':	// Cursor end
                	return makeCommandInteraction(new CursorEnd());
                	break;
                case 'J':	// New line
                case 'M':
                	return makeCommandInteraction(new NewLine());
                	break;
                case 'H':	// Backspace
                	return makeCommandInteraction(new Backspace());
                	break;
                case 'D':	// Delete line
                	return makeCommandInteraction(new DeleteLine());
                	break;
                case 'Z':	// Undo
                	return makeUndoInteraction();
                	break;
                case 'A':	// Redo
                	return makeRedoInteraction();
                	break;
                case 'X':	// Quit/Exit
                    return makeQuitInteraction();
                }
            }
            else
            {
                // The user pressed a normal key (e.g., 'h') without holding
                // down Ctrl; react accordingly
                return makeCommandInteraction(new InsertChar(keypress.c));	// Insert character
            }
        }
    }
    
    
    // The function takes a Command stack as parameter and deletes all the commands from it
    void emptyCommandStack(std::stack<Command*>& commandStack)
    {
    	// As long as the Command stack is not empty, the top element is stored in a 
    	// variable, then popped off the stack and finally deleted
        while(!commandStack.empty())
       	{
			Command* topCommand = commandStack.top();
			commandStack.pop();
			delete topCommand;
		}     	
    }
}


// This function implements command execution, but does not handle "undo"
// and "redo"; you'll need to add that handling.
void CommandProcessor::run()
{
    view.refresh();
	
	std::stack<Command*> undoStack;		// Command stack to store the commands that can be undone
	std::stack<Command*> redoStack;		// Command stack to store the commands that can be redone
	
    while (true)
    {
        UserInteraction interaction = nextUserInteraction();
        
        if (interaction.type == UserInteractionType::quit)
        {
        	// Before quitting, empty the undo and redo Command stacks, in case there 
        	// was any Command left
        	emptyCommandStack(undoStack);
        	emptyCommandStack(redoStack);
            break;
        }
        else if (interaction.type == UserInteractionType::undo && !undoStack.empty())
        {
        	// To undo, a Command is popped out of the undo stack, its undo() function 
        	// is executed and then it is pushed into the redo stack
        	Command* undoCommand = undoStack.top();
			undoStack.pop();
			undoCommand->undo(editor);
			redoStack.push(undoCommand);
			view.clearErrorMessage();
			view.refresh();
        }
        else if (interaction.type == UserInteractionType::redo && !redoStack.empty())
        {
        	// To redo, a Command is popped out of the redo stack, its execute() 
        	// function is executed and then it is pushed into the undo stack
        	Command* redoCommand = redoStack.top();
			redoStack.pop();
			redoCommand->execute(editor);
			undoStack.push(redoCommand);
			view.clearErrorMessage();
			view.refresh();
        }
        else if (interaction.type == UserInteractionType::command)
        {
            try
            {
            	// Execute the current command, store it in the undo stack and empty 
            	// the redo stack (as it is not possible to redo after a new command 
            	// was issued)
                interaction.command->execute(editor);
                undoStack.push(interaction.command);
				emptyCommandStack(redoStack);
                view.clearErrorMessage();
            }
            catch (EditorException& e)
            {
            	// If an exception was thrown, the current command must be deleted as 
            	// it will not be stored in the undo stack and, therefore, it would 
            	// never be deleted otherwise
            	delete interaction.command;
                view.showErrorMessage(e.getReason());
            }

            view.refresh();
        }
    }
}

