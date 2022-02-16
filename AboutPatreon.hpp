#ifndef ABOUTPATREON_HPP
#define ABOUTPATREON_HPP

#include <QDialog>

namespace Ui {
class AboutPatreon;
}

class AboutPatreon : public QDialog
{
    Q_OBJECT

public:
    explicit AboutPatreon(QWidget *parent = nullptr);
    ~AboutPatreon();

private:
    Ui::AboutPatreon *ui;
};

#endif // ABOUTPATREON_HPP
