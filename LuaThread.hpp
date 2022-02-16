#ifndef LUATHREAD_H
#define LUATHREAD_H

#include <QTimer>
#include <QThread>

#include <Console.hpp>
#include <LuaBackend.hpp>

class LuaThread : public QObject
{
    Q_OBJECT
    public:
        int runInterval;
        LuaBackend::LuaScript* exeScript;

        LuaThread(Console*);

        void start();
        void stop();

    private:
        Console* _console;

        QThread* _thread;
        QTimer* _runTimer;

    private slots:
        void startEvent();
        void runEvent();
};

#endif // LUATHREAD_H
