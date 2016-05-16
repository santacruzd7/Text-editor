// EditorView.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// DO NOT MODIFY THIS FILE
//
// There are some low-level gymnastics being done here, particularly in dealing
// with problems like resizing the terminal window and refreshing various areas
// of the view.  The resizing functionality, in particular, required the use of
// signal handlers from the C Standard Library.

#include <csignal>
#include <sstream>
#include <string>
#include <ncurses.h>
#include "EditorView.hpp"
#include "EditorException.hpp"


EditorView::EditorView(Editor& editor)
    : editor{editor}, topVisibleLine{1}, leftVisibleColumn{1}, currentErrorMessage{""}
{
}


namespace
{
    EditorView* registeredView = nullptr;


    void handleWindowSizeChange(int signum)
    {
        if (registeredView != nullptr)
        {
            endwin();
            ::refresh();
            registeredView->refresh();
        }
    }


    void registerSignalHandlers(EditorView* view)
    {
        registeredView = view;
        std::signal(SIGWINCH, handleWindowSizeChange);
    }


    void unregisterSignalHandlers()
    {
        std::signal(SIGWINCH, SIG_DFL);
        registeredView = nullptr;
    }
}


void EditorView::start()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);
    registerSignalHandlers(this);
}


void EditorView::stop()
{
    unregisterSignalHandlers();
    echo();
    endwin();
}


void EditorView::showErrorMessage(const std::string& errorMessage)
{
    currentErrorMessage = errorMessage;
}


void EditorView::clearErrorMessage()
{
    currentErrorMessage = "";
}


namespace
{
    struct Coordinate
    {
        int x;
        int y;
    };


    struct Area
    {
        Coordinate tlxy;
        Coordinate brxy;
    };


    struct EditorAreas
    {
        Area lineNumberArea;
        Area statusBarArea;
        Area editorArea;
        Area scrollArea;
    };


    EditorAreas determineEditorAreas(int lineCount)
    {
        int xsize;
        int ysize;
        getmaxyx(stdscr, ysize, xsize);

        int lineNumberLength = std::to_string(lineCount).length();

        EditorAreas areas;

        areas.lineNumberArea.tlxy = {0, 0};
        areas.lineNumberArea.brxy = {lineNumberLength + 2, ysize - 3};

        areas.statusBarArea.tlxy = {lineNumberLength + 3, ysize - 2};
        areas.statusBarArea.brxy = {xsize - 1, ysize - 1};

        areas.editorArea.tlxy = {lineNumberLength + 3, 0};
        areas.editorArea.brxy = {xsize - 1, ysize - 3};

        areas.scrollArea.tlxy = {0, ysize - 2};
        areas.scrollArea.brxy = {lineNumberLength + 2, ysize - 1};

        return areas;
    }


    void redrawLineNumberArea(const Area& lineNumberArea, const Editor& editor, int topVisibleLine)
    {
        int currentLine = topVisibleLine;

        for (int y = lineNumberArea.tlxy.y; y <= lineNumberArea.brxy.y; ++y)
        {
            mvaddch(y, lineNumberArea.brxy.x, '|');

            for (int x = lineNumberArea.tlxy.x; x <= lineNumberArea.brxy.x - 1; ++x)
            {
                mvaddch(y, x, ' ');
            }

            if (currentLine <= editor.lineCount())
            {
                std::string lineNumber = std::to_string(currentLine);

                mvaddstr(
                    y, lineNumberArea.brxy.x - 1 - lineNumber.length(),
                    lineNumber.c_str());
            }

            ++currentLine;
        }
    }


    void redrawStatusBar(
        const Area& statusBarArea, const Editor& editor,
        const std::string& currentErrorMessage)
    {
        for (int y = statusBarArea.tlxy.y; y <= statusBarArea.brxy.y; ++y)
        {
            for (int x = statusBarArea.tlxy.x; x <= statusBarArea.brxy.x; ++x)
            {
                if (y == statusBarArea.tlxy.y)
                {
                    mvaddch(y, x, '-');
                }
                else
                {
                    mvaddch(y, x, ' ');
                }
            }
        }

        std::ostringstream pos;
        pos << "Ln " << editor.cursorLine() << " Col " << editor.cursorColumn();

        std::string posStr = pos.str();

        mvaddstr(
            statusBarArea.brxy.y, statusBarArea.brxy.x - posStr.length(),
            posStr.c_str());

        if (currentErrorMessage.length() > 0)
        {
            int maxErrorMessageWidth =
                (statusBarArea.brxy.x - statusBarArea.tlxy.x + 1) - (posStr.length() + 4);

            mvaddstr(
                statusBarArea.brxy.y, statusBarArea.tlxy.x,
                currentErrorMessage.substr(0, maxErrorMessageWidth).c_str());
        }
    }


    void redrawScrollArea(const Area& scrollArea, int leftVisibleColumn)
    {
        for (int y = scrollArea.tlxy.y; y <= scrollArea.brxy.y; ++y)
        {
            if (y == scrollArea.brxy.y)
            {
                for (int x = scrollArea.tlxy.x; x <= scrollArea.brxy.x - 1; ++x)
                {
                    mvaddch(y, x, leftVisibleColumn == 1 ? ' ' : '<');
                }

                mvaddch(y, scrollArea.brxy.x, ' ');
            }
            else
            {
                for (int x = scrollArea.tlxy.x; x <= scrollArea.brxy.x; ++x)
                {
                    mvaddch(y, x, ' ');
                }
            }
        }
    }


    void forceCursorVisible(
        const Area& editorArea, const Editor& editor,
        int& topVisibleLine, int& leftVisibleColumn)
    {
        int bottomVisibleLine = topVisibleLine + (editorArea.brxy.y - editorArea.tlxy.y);

        if (editor.cursorLine() < topVisibleLine)
        {
            topVisibleLine = editor.cursorLine();
        }
        else if (editor.cursorLine() > bottomVisibleLine)
        {
            topVisibleLine = editor.cursorLine() - (editorArea.brxy.y - editorArea.tlxy.y);
        }

        int rightVisibleColumn = leftVisibleColumn + (editorArea.brxy.x - editorArea.tlxy.x);

        if (editor.cursorColumn() < leftVisibleColumn)
        {
            leftVisibleColumn = editor.cursorColumn();
        }
        else if (editor.cursorColumn() > rightVisibleColumn)
        {
            leftVisibleColumn = editor.cursorColumn() - (editorArea.brxy.x - editorArea.tlxy.x);
        }
    }

    
    void placeText(
        const Area& editorArea, const Editor& editor,
        int& topVisibleLine, int& leftVisibleColumn)
    {
        int currentLine = topVisibleLine;

        for (int y = editorArea.tlxy.y; y <= editorArea.brxy.y; ++y)
        {
            if (currentLine <= editor.lineCount())
            {
                int visibleColumns = editorArea.brxy.x - editorArea.tlxy.x + 1;

                const std::string& lineText = editor.line(currentLine);
            
                std::string displayText = leftVisibleColumn <= lineText.length()
                    ? lineText.substr(leftVisibleColumn - 1, visibleColumns)
                    : "";

                mvaddstr(y, editorArea.tlxy.x, displayText.c_str());

                for (int x = editorArea.tlxy.x + displayText.length(); x <= editorArea.brxy.x; ++x)
                {
                    mvaddch(y, x, ' ');
                }
            }
            else
            {
                for (int x = editorArea.tlxy.x; x <= editorArea.brxy.x; ++x)
                {
                    mvaddch(y, x, ' ');
                }
            }

            ++currentLine;
        }
    }


    void placeCursor(
        const Area& editorArea, const Editor& editor,
        int topVisibleLine, int leftVisibleColumn)
    {
        int cursorRelativeLine = editor.cursorLine() - topVisibleLine;
        int cursorRelativeColumn = editor.cursorColumn() - leftVisibleColumn;

        move(cursorRelativeLine + editorArea.tlxy.y, cursorRelativeColumn + editorArea.tlxy.x);
    }
}


void EditorView::refresh()
{
    EditorAreas areas = determineEditorAreas(editor.lineCount());

    forceCursorVisible(areas.editorArea, editor, topVisibleLine, leftVisibleColumn);
    placeText(areas.editorArea, editor, topVisibleLine, leftVisibleColumn);
    redrawLineNumberArea(areas.lineNumberArea, editor, topVisibleLine);
    redrawStatusBar(areas.statusBarArea, editor, currentErrorMessage);
    redrawScrollArea(areas.scrollArea, leftVisibleColumn);
    placeCursor(areas.editorArea, editor, topVisibleLine, leftVisibleColumn);

    ::refresh();
}

