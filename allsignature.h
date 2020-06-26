#ifndef ALLSIGNATURE_H
#define ALLSIGNATURE_H
#include <QtSql>
#include <QWidget>
#include <QPushButton>
#include <keyboard.h>
#include <QSerialPort>
namespace Ui {
class AllSignature;
}

class AllSignature : public QWidget
{
    Q_OBJECT

public:
    explicit AllSignature(QWidget *parent = 0);
    QSqlDatabase myDB;
    ~AllSignature();
     QPushButton *push;
     QPushButton *push2;
     Keyboard *lineEditKeyboard2;
     QSerialPort SerialPort22;


public slots:
    void on_pushButton_3_clicked();
    void CardReader2();
    void run_keyboard_lineEdit2();

private slots:
    void on_pushButton_clicked();



    void on_pushButton_4_clicked();

signals:



private:


    Ui::AllSignature *ui;






};

#endif // ALLSIGNATURE_H
