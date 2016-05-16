// main.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// This is the main() function that launches BooEdit.
//
// DO NOT MAKE CHANGES TO THIS FILE

#include <sstream>
#include "BooEdit.hpp"
#include "BooEditLog.hpp"



namespace
{
    void logStarted(const char* programName)
    {
        std::ostringstream oss;
        oss << "Started " << programName;
        
        booEditLog(oss.str());
    }


    void logStopped(const char* programName)
    {
        std::ostringstream oss;
        oss << "Stopped " << programName;

        booEditLog(oss.str());
    }
}



int main(int argc, char** argv)
{
    try
    {
        logStarted(argv[0]);

        runBooEdit();

        logStopped(argv[0]);
    }
    catch (...)
    {
        logStopped(argv[0]);
        throw;
    }

    return 0;
}

