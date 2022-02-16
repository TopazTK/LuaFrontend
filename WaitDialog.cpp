#include <WaitDialog.hpp>
#include <ui_WaitDialog.h>

#include <MainWindow.hpp>

MainWindow* _rootParent;

WaitDialog::WaitDialog(QWidget *parent) : QDialog(parent), ui(new Ui::WaitDialog)
{
    ui->setupUi(this);

    _rootParent = (MainWindow*)parent;

    connect(this, SIGNAL(rejected()), this, SLOT(rejectEvent()));
    connect(ui->abortButton, SIGNAL(pressed()), this, SLOT(rejectEvent()));

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::rejectEvent()
{
    _rootParent->latchTimer->stop();
    this->close();
}
