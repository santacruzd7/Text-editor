// Keypress.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// You can feel free to modify this file, since it's only used in
// CommandProcessor (which you'll also be working on), but no
// modifications are necessary in order to complete the project.
// Be careful, though, that the changes you make don't break critical
// functionality (like distinguishing Ctrl keys from others).

#include "Keypress.hpp"
#include <ncurses.h>


namespace
{
    char ctrlLetter(int keypress)
    {
        if (keypress >= 1 && keypress <= 26)
        {
            return keypress + 'A' - 1;
        }
        else
        {
            return 0;
        }
    }
}


Keypress nextKeypress()
{
    while (true)
    {
        int key = getch();

        char ctrlkey = ctrlLetter(key);

        if (ctrlkey != 0)
        {
            return Keypress{ctrlkey, true};
        }

        if (key >= 32 && key < 127)
        {
            return Keypress{static_cast<char>(key), false};
        }
    }
}

