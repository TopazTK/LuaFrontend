#include <Console.hpp>
#include <ui_Console.h>

Console::Console(QWidget *parent) : QDialog(parent), ui(new Ui::Console)
{
    ui->setupUi(this);

    ui->output->setStyleSheet("font: 9pt \"Consolas\";");

    ui->output->append("========================================");
    ui->output->append("== LuaFrontend v1.15S - Powered by QT ==");
    ui->output->append("======= Copyright 2021 - TopazTK =======");
    ui->output->append("========================================");
    ui->output->append("==== Compatible with LuaEngine v5.1 ====");
    ui->output->append("========================================");
    ui->output->append("");

    connect(this, SIGNAL(rejected()), parent, SLOT(consoleToggle()));
}

Console::~Console()
{
    delete ui;
}

void Console::printMessage(QString inputTxt, int type)
{
    QString _colors[] = { "CADETBLUE", "GREEN", "ORANGE", "RED" };
    QString _titles[] = { "MESSAGE", "SUCCESS", "WARNING", "ERROR" };

    QString _prefix = "<span style='color: %1'>%2: </span>";
    _prefix = _prefix.arg(_colors[type], _titles[type]);

    inputTxt = inputTxt.replace("\n", "<br>");
    ui->output->append(_prefix + inputTxt);
}
