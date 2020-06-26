#ifndef DELETE_H
#define DELETE_H

#include <QWidget>
#include <QPushButton>
#include <keyboard.h>
namespace Ui {
class Delete;
}

class Delete : public QWidget
{
    Q_OBJECT

public:
    explicit Delete(QWidget *parent = 0);
    QPushButton *PushDelete;

    ~Delete();

public slots:
    void on_pushButton_2_clicked();
    void run_keyboard_lineEdit3();

private slots:


    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_clicked();

private:
    Keyboard *lineEditKeyboard3;
    Ui::Delete *ui;
};

#endif // DELETE_H
