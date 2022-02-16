#include "AboutPatreon.hpp"
#include "ui_AboutPatreon.h"

AboutPatreon::AboutPatreon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPatreon)
{
    ui->setupUi(this);
}

AboutPatreon::~AboutPatreon()
{
    delete ui;
}
