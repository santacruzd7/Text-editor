// Editor.hpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// The Editor class implements the "model" of BooEdit.  In other words, its
// job is to maintain the underlying data -- the current lines of text, where
// the cursor is supposed to be, etc. -- providing operations that allow the
// EditorView to access that data and also providing basic operations that
// allow the various Command objects to make necessary changes.
//
// The member functions already declared in the Editor class are absolutely
// necessary for EditorView to work, so they cannot be removed or have their
// declarations changed (though you may need to implement them; see
// Editor.cpp).
//
// You will absolutely need to implement additional member functions, because I
// haven't declared any functions that change the editor's data (e.g.,
// inserting characters, moving the cursor), which you'll need in order to
// implement the various Command objects.  You may also need additional member
// functions that access the editor's data in ways not provided here.
//
// When adding member functions, focus on adding "primitive" tools that you
// might be able to use in multiple command implementations, as opposed to
// just writing a member function for each commmand.
//
// The Editor class is unaware of the concept of "undo"; it is the job of the
// Command objects to know what they need to do in order to undo a Command.
//
// Note that the notion of "line number" and "column number" here is one-based,
// not zero-based, which means that the top left corner of the file is line 1
// column 1.  You can handle this however you'd like inside of your class, but
// the rest of BooEdit expects Editor objects to take and return one-based
// line and column numbers.

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <string>
#include <vector>



class Editor
{
public:
    // Initializes the editor.  Initially, the cursor is on line 1 column 1
    // and there is one empty line of text in the editor.
    Editor();

    // Returns the line number where the cursor currently should reside.
    int cursorLine() const;

    // Returns the column number where the cursor currently should reside.
    int cursorColumn() const;

    // Returns the number of lines of text currently in the editor.
    int lineCount() const;

    // Returns the text on the given line number.
    const std::string& line(int lineNumber) const;
    
	// Decreases the line number where the cursor resides
	void cursorUp();
	
	// Increases the line number where the cursor resides
	void cursorDown();
	
	// Decreases the column number where the cursor resides
	void cursorLeft();
	
	// Increases the column number where the cursor resides
	void cursorRight();
	
	// Sets the column number to 0
	void cursorHome();
	
	// Set the column number to the point beyond the last character of the current line
	void cursorEnd();
	
	// Set the column number to the given value
    void setCursorColumn(int column);
    
	// Creates a new line under the current line with the text to the right of the current cursor position
	void newLine();
	
	// Deletes the character to the left of the current cursor position
	void backspace();
	
	// Deletes the current line of text
	void deleteLine();
	
	// Inserts a given line of text in the given line position
	void insertLine(const std::string& line, int lineNum);
	
	// Inserts a character
	void insertChar(char c);

	
private:
    // Line number of the cursor
    int lineNumber;
    
    // Column number of the cursor
    int columnNumber;
    
    // Vector containing the lines of the text
    std::vector<std::string> lines;
};



#endif // EDITOR_HPP