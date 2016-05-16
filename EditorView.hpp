// EditorView.hpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// This class implements the user interface for BooEdit
//
// DO NOT MODIFY THIS FILE

#ifndef EDITORVIEW_HPP
#define EDITORVIEW_HPP

#include "Editor.hpp"



class EditorView
{
public:
    EditorView(Editor& editor);

    void start();
    void stop();
    void refresh();

    void showErrorMessage(const std::string& errorMessage);
    void clearErrorMessage();

private:
    Editor& editor;

    int topVisibleLine;
    int leftVisibleColumn;

    std::string currentErrorMessage;
};



#endif // EDITORVIEW_HPP

