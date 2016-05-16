// BooEdit.cpp
//
// ICS 45C Fall 2014
// Project #4: People Just Love to Play with Words
//
// DO NOT MODIFY THIS FILE
//
// There are some fairly hairy-looking things going on here, in an
// attempt to be sure that the program -- in most cases -- crashes
// in a way that leaves the Linux shell window available afterward.
// For example, I'm calling into functions from the C Standard Library
// that are called just before the program crashes due to problems
// like segmentation faults.

#include "BooEdit.hpp"
#include <csignal>
#include "Editor.hpp"
#include "EditorView.hpp"
#include "CommandProcessor.hpp"


namespace
{
    class BooEdit
    {
    public:
        BooEdit();
        ~BooEdit();

        void run();

    private:
        Editor editor;
        EditorView view;
        CommandProcessor processor;
    };


    BooEdit::BooEdit()
        : editor{}, view{editor}, processor{editor, view}
    {
    }


    void BooEdit::run()
    {
        view.start();
        processor.run();
    }


    BooEdit::~BooEdit()
    {
        view.stop();
    }




    BooEdit* booEdit = nullptr;


    void startBooEdit()
    {
        booEdit = new BooEdit;
        booEdit->run();
    }


    void stopBooEdit()
    {
        if (booEdit != nullptr)
        {
            delete booEdit;
            booEdit = nullptr;
        }
    }


    int signals[6] { SIGTERM, SIGSEGV, SIGILL, SIGINT, SIGABRT, SIGFPE };


    void unregisterSignalHandlers()
    {
        for (int signal : signals)
        {
            std::signal(signal, SIG_DFL); 
        }
    }


    void signalHandler(int signum)
    {
        unregisterSignalHandlers();
        stopBooEdit();
    }


    void registerSignalHandlers()
    {
        for (int signal : signals)
        {
            std::signal(signal, signalHandler);
        }
    }
}


void runBooEdit()
{
    try
    {
        registerSignalHandlers();
        startBooEdit();

        unregisterSignalHandlers();
        stopBooEdit();
    }
    catch (...)
    {
        unregisterSignalHandlers();
        stopBooEdit();

        throw;
    }
}

