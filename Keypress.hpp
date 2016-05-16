// Keypress.hpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// Function for reading keypresses from the terminal.  This module is
// already fully equipped to read normal keypresses (letters, digits,
// symbols, as well as Ctrl keys like Ctrl+C), but does not make an
// attempt to support any kind of special keys like arrow keys, etc.,
// which can be trickier to support than you might think.
//
// You can feel free to modify this file, since it's only used in
// CommandProcessor (which you'll also be working on), but no
// modifications are necessary in order to complete the project.
// Be careful, though, that the changes you make don't break critical
// functionality (like distinguishing Ctrl keys from others).

#ifndef KEYPRESS_HPP
#define KEYPRESS_HPP



// A Keypress struct contains a character that indicates what key
// was pressed, along with a bool value that indicates whether the
// Ctrl key was down at the time or not.  (No other special keys
// on the keyboard are supported.)
//
// So, for example, if the 'd' key is pressed, c would be 'd' and
// ctrl would be false; if Ctrl+D is pressed, c would be 'D' and
// ctrl would be true.

struct Keypress
{
    char c;
    bool ctrl;
};



// Waits for the user to press a recognized key and then returns a
// Keypress struct that describes it.

Keypress nextKeypress();



#endif // KEYPRESS_HPP

