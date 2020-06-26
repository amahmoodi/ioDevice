#ifndef BLANKER_H
#define BLANKER_H

#include <QWidget>

namespace Ui {
class blanker;
}

class blanker : public QWidget
{
    Q_OBJECT

public:
    explicit blanker(QWidget *parent = 0);
    ~blanker();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_pressed();

    void on_pushButton_2_clicked();

private:
    Ui::blanker *ui;
};

#endif // BLANKER_H
