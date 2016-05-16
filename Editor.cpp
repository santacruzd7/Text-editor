// Editor.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// Implementation of the Editor class

#include "Editor.hpp"
#include "EditorException.hpp"

// Editor constructor
Editor::Editor()
	: lineNumber{1}, columnNumber{1}
{
	lines.push_back("");
}


int Editor::cursorLine() const
{
    return lineNumber;
}


int Editor::cursorColumn() const
{
    return columnNumber;
}


int Editor::lineCount() const
{
    return lines.size();
}


const std::string& Editor::line(int lineNumber) const
{
    return lines.at(lineNumber-1);
}


void Editor::cursorUp()
{
	if(lineNumber == 1)	 // If the cursor is already on line 1, there is no cell above, so the command fails and an exception is thrown
	{
		throw EditorException("Already at top!");
	}
	else	// Otherwise, the cursor is moved up one cell (same column on the previous line), except if the previous line is so short that this would place the cursor beyond the end of the previous line, in which case the cursor is placed in the column just beyond the end of the previous line
	{
		lineNumber--;
		if(columnNumber>lines.at(lineNumber-1).size())
		{
			columnNumber = lines.at(lineNumber-1).size()+1;
		}
	}
}


void Editor::cursorDown()
{
	if(lineNumber == lines.size())	 // If the cursor is already on the last line, there is no cell below, so the command fails and an exception is thrown
	{
		throw EditorException("Already at bottom!");
	}
	else	// Otherwise, the cursor is moved down one cell (same column on the next line), except if the next line is so short that this would place the cursor beyond the end of the next line, in which case the cursor is placed in the column just beyond the end of the next line
	{
		lineNumber++;
		if(columnNumber>lines.at(lineNumber-1).size())
		{
			columnNumber = lines.at(lineNumber-1).size()+1;
		}
	}
}


void Editor::cursorLeft()
{
	if(columnNumber == 1 && lineNumber == 1)	 // If the cursor is at the beginning of line 1, the command fails and an exception is thrown
	{
		throw EditorException("Already at beginning!");
	}
	else if(columnNumber == 1)	 // If the cursor is at the beginning of a line, it is moved just beyond the end of the previous line
	{
		lineNumber--;
		columnNumber = lines.at(lineNumber-1).size()+1;
	}
	else	// Otherwise, the cursor is moved left one cell (previous column on the same line)
	{
   		columnNumber--;
    }
}


void Editor::cursorRight()
{
	if((columnNumber == (lines.at(lineNumber-1).size()+1) && lineNumber == lines.size()))	 // If the cursor is just beyond the end of the last line, the command fails and an exception is thrown
	{
		throw EditorException("Already at end!");
	}	
	else if(columnNumber == (lines.at(lineNumber-1).size()+1))	 // If the cursor is just beyond the end of a line, it is moved to the beginning of the next line
	{
		lineNumber++;
		columnNumber = 1;
	}
	else	// Otherwise, the cursor is moved rightft one cell (next column on the same line)
	{
   		columnNumber++;
    }
}


void Editor::cursorHome()
{
	columnNumber = 1;
}


void Editor::cursorEnd()
{
	columnNumber = lines.at(lineNumber-1).size()+1;
}


void Editor::newLine()
{
	// Split the current line in two at the point where the cursor is currently placed
	// Any text before the cursor goes to half1, which will be what remains in the current line
	// Any text at or after the cursor goest to half2, which will be what will be moved to a new line
	std::string half1 = lines.at(lineNumber-1).substr(0, columnNumber-1);
	std::string half2 = lines.at(lineNumber-1).substr(columnNumber-1);
	
	// Vector of string to created to store the new lines after the splitting is done
	std::vector<std::string> newLines;
	
	// Copy all the lines before the current line, if any
	for(int i = 0; i<lineNumber-1; i++)
	{
		newLines.push_back(lines.at(i));
	}
	
	// Add half1 and then half2 to the vector
	// The current line is anything before the cursor
	// The next line - new line - is anything at or after the cursor
	newLines.push_back(half1);
	newLines.push_back(half2);
			
	// Copy the rest of lines after the current line, if any
	for(int i = lineNumber; i<lines.size(); i++)
	{
		newLines.push_back(lines.at(i));
	}
	
	// Cursor moved to the beggining of the new line
	columnNumber = 1;
	lineNumber++;
	
	// The original vector of lines is replaced with the new one
	lines = newLines;
}


void Editor::backspace()
{
	if(columnNumber == 1 && lineNumber == 1)	 // If the cursor is already at the beginning of the first line, the command fails and an exception is thrown
	{
		throw EditorException("Already at beginning!");
	} 
	else if (columnNumber == 1)	 // If the cursor is in the first column, the current line of text is moved to the end of the previous line and the cursor 
	{	
		// The new line will have the previous line and the current line concatenated	
		std::string newLine = lines.at(lineNumber-2) + lines.at(lineNumber-1);
		
		// Vector of string to created to store the new lines after the merging is done
		std::vector<std::string> newLines;
		
		// Copy all the lines before the previous line, if any
		for(int i = 0; i<lineNumber-2; i++)
		{
			newLines.push_back(lines.at(i));
		}
		
		// Add newLine to the vector, comprising the previous and current lines of text
		newLines.push_back(newLine);
		
		// Copy the rest of lines after the current line, if any	
		for(int i = lineNumber; i<lines.size(); i++)
		{
			newLines.push_back(lines.at(i));
		}
		
		// The cursor is moved to the beginning of the moved text
		columnNumber = lines.at(lineNumber-2).size()+1;
		lineNumber--;
		
		// The original vector of lines is replaced with the new one
		lines = newLines;
	} 
	else	// Otherwise, it deletes the character to the left of the cursor
	{
		// Delete the character creating two substrings which does not contain that character
		// Any text two positions before the cursor goes to half1
		// Any text at or after the cursor goes to half2
		std::string half1 = lines.at(lineNumber-1).substr(0, columnNumber-2);
		std::string half2 = lines.at(lineNumber-1).substr(columnNumber-1);
		
		// The current line is updated to the concatenation of half1 and half2, any of which contain the character to be deleted
		lines.at(lineNumber-1) = half1 + half2;
		
		// The cursor is moved one cell to the left
		columnNumber--;
	}
}


void Editor::deleteLine()
{
	if (lines.size() == 1 && lines.at(0).empty())	// If there is only one line of text and it is empty, the command fails and an exception is thrown
	{
		throw EditorException("Already empty!");
	}
	else	// Otherwise, the current line of text is deleted
	{
		// Vector of string to created to store the new lines after the deletion is done
		std::vector<std::string> newLines;
		
		if(lines.size() == 1)	// If there is only one line, it is deleted. Thus, the empty string remains
		{
			newLines.push_back("");
		}
		else
		{ 
			// Copy all the lines before and after the current line, omitting the current line
			for(int i = 0; i<lines.size(); i++)
			{
				if(i != (lineNumber-1))
				{
					newLines.push_back(lines.at(i));
				}
			}
			
			// If the line to be deleted was the last one, the cursor is moved one cell up (to the previous line)				
			if (lineNumber == lines.size())
				lineNumber--;
		}
		
		// The original vector of lines is replaced with the new one
		lines = newLines;
		
		// If the new current line is so short that this would place the cursor beyond the end of the new current line, the cursor is placed in the column just beyond the end of the new current line
		if(columnNumber>lines.at(lineNumber-1).size())
		{
			columnNumber = lines.at(lineNumber-1).size()+1;
		}
	}
}


void Editor::insertLine(const std::string& line, int lineNum)
{
	// Vector of string to created to store the new lines after the insertion is done
	std::vector<std::string> newLines;
	
	if(lines.size() == 1 && lines.at(0).empty())
	{
		// Insert the new line
		newLines.push_back(line);
	}
	else
	{
		// Copy all the lines before the current line, if any
		for(int i = 0; i<lineNum-1; i++)
		{
			newLines.push_back(lines.at(i));
		}
		
		// Insert the new line
		newLines.push_back(line);
		
		// Copy all the lines after the current line, if any
		for(int i = lineNum-1; i<lines.size(); i++)
		{
			newLines.push_back(lines.at(i));
		}
		
		// If the line is to be inserted at the last line, move the cursor down
		if (lineNumber == lines.size() && lineNum == lines.size()+1)
		{
			lineNumber++;
		}
	}

	// The original vector of lines is replaced with the new one
	lines = newLines;
	
	// If the new current line is so short that this would place the cursor beyond the end of the new current line, the cursor is placed in the column just beyond the end of the new current line
	if(columnNumber>lines.at(lineNumber-1).size())
	{
		columnNumber = lines.at(lineNumber-1).size()+1;
	}
}


void Editor::insertChar(char c)
{
	// Split the current line in two halves: what will be to the left of the newly inserted char (half1) and what will be to the right of it (half2)
	std::string half1 = lines.at(lineNumber-1).substr(0, columnNumber-1);
	std::string half2 = lines.at(lineNumber-1).substr(columnNumber-1);
	
	// The current line becomes the concatenation of half1, the character to be inserted and half2
	lines.at(lineNumber-1) = half1 + c + half2;
	
	// The cursor is moved to the right
	columnNumber++;
}


void Editor::setCursorColumn(int column)
{
	columnNumber = column;
}