#include "AboutFrontend.hpp"
#include "ui_AboutFrontend.h"

AboutFrontend::AboutFrontend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutFrontend)
{
    ui->setupUi(this);
}

AboutFrontend::~AboutFrontend()
{
    delete ui;
}
