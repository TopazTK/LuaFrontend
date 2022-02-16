#ifndef WAITDIALOG_HPP
#define WAITDIALOG_HPP

#include <QDialog>

namespace Ui {
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit WaitDialog(QWidget *parent = nullptr);
        ~WaitDialog();

    private:
        Ui::WaitDialog *ui;

    private slots:
        void rejectEvent();
};

#endif // WAITDIALOG_HPP
