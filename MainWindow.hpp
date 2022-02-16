#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUrl>
#include <QTimer>
#include <QDateTime>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTextStream>
#include <QTranslator>
#include <QApplication>
#include <QDesktopServices>

#include <toml.hpp>
#include <Console.hpp>
#include <WaitDialog.hpp>
#include <LuaBackend.hpp>
#include <LuaThread.hpp>
#include <AboutPatreon.hpp>
#include <AboutFrontend.hpp>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    struct QGameInfo
    {
        QString gameName;
        QString exeName;
        QString scriptPath;
        uint64_t baseAddress;
        uint64_t offset;
        bool isBigEndian;
    };

    Q_OBJECT

    public:
        QTimer* latchTimer;

        LuaBackend* backend;
        LuaBackend* backendFake;

        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void consoleToggle();

    private slots:
        void runEvent();
        void stopEvent();
        void autoToggle();
        void latchEvent();
        void startEvent();
        void darkToggle();
        void reloadEvent();
        void connectEvent();
        void threadToggle();
        void gameClickEvent(int);
        void scriptClickEvent(QTreeWidgetItem*, int);

        void scriptContextEvent(QPoint);

        void newScriptEvent();
        void editScriptEvent();
        void deleteScriptEvent();
        void refreshScriptEvent();
        void folderOpenEvent();
        void languageEvent();

        void showAbout();
        void showPatreon();


private:
        toml::value _prefTable;
        toml::value _gameToml;

        QPalette _darkPal;
        QPalette _lightPal;

        bool _autoBool;
        bool _threadBool;
        bool _consoleBool;
        bool _darkPalBool;

        QString _language;

        Console* _console;
        WaitDialog* _waitWindow;
        AboutFrontend* _aboutDiag;
        AboutPatreon* _patreonDiag;

        QTimer* _runTimer;
        QList<LuaThread*> _threadList;

        QString _basePath;
        QGameInfo _currGame;

        int parseGame();
        int parseScript();

        void serializePref();

        QTranslator _translator;

        Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
