#ifndef ABOUTFRONTEND_HPP
#define ABOUTFRONTEND_HPP

#include <QDialog>

namespace Ui {
class AboutFrontend;
}

class AboutFrontend : public QDialog
{
    Q_OBJECT

public:
    explicit AboutFrontend(QWidget *parent = nullptr);
    ~AboutFrontend();

private:
    Ui::AboutFrontend *ui;
};

#endif // ABOUTFRONTEND_HPP
