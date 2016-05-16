// CommandProcessor.hpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// The job of a CommandProcessor is to wait for the user to enter commands
// (by pressing keys on the keyboard) and then to affect the appropriate
// changes on the Editor and EditorView.
//
// Do not change or remove the signatures of the public member functions
// that have been declared here, since they are called in other parts
// of BooEdit that you won't be working on.  You do have the freedom
// to add anything else to this class that you'd like -- though my own
// solution didn't have anything else.

#ifndef COMMANDPROCESSOR_HPP
#define COMMANDPROCESSOR_HPP

#include "Editor.hpp"
#include "EditorView.hpp"
#include "Command.hpp"



class CommandProcessor
{
public:
    CommandProcessor(Editor& editor, EditorView& view);
    void run();

private:
    Editor& editor;
    EditorView& view;
};



#endif // COMMANDPROCESSOR_HPP

