#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDialog>

namespace Ui { class Console; }

class Console : public QDialog
{
    Q_OBJECT

    public:
        void printMessage(QString, int type = 0);

        explicit Console(QWidget *parent = nullptr);
        ~Console();

    private:
        Ui::Console *ui;
};

#endif // CONSOLE_H
