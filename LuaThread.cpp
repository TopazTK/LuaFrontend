#include "LuaThread.hpp"

LuaThread::LuaThread(Console* consoleInput)
{
    _thread = new QThread(this);
    _runTimer = new QTimer(this);

    _console = consoleInput;
}

void LuaThread::start()
{
    this->moveToThread(_thread);

    _runTimer->moveToThread(_thread);

    connect(_thread, SIGNAL(started()), this, SLOT(startEvent()));
    connect(_runTimer, SIGNAL(timeout()), this, SLOT(runEvent()));

    _thread->start();
}

void LuaThread::stop()
{
    _runTimer->stop();
    _thread->exit();
}

void LuaThread::startEvent()
{
    _runTimer->start(runInterval);
}

void LuaThread::runEvent()
{
    if (exeScript->frameFunction)
    {
        auto _result = exeScript->frameFunction();

        if (!_result.valid())
        {
            sol::error _err = _result;

            auto _errStr = QString(_err.what());
            _console->printMessage(_errStr + "<br>", 3);

            _runTimer->stop();
            _thread->exit();
        }
    }

    if (_runTimer->interval() != runInterval)
        _runTimer->setInterval(runInterval);
}
