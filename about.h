#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>

namespace Ui {
class ABOUT;
}

class ABOUT : public QWidget
{
    Q_OBJECT

public:
    explicit ABOUT(QWidget *parent = 0);
    ~ABOUT();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::ABOUT *ui;
};

#endif // ABOUT_H
