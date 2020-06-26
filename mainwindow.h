#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QSerialPort>
#include <QFileSystemWatcher>
#include <wiringPi.h>
#include <allsignature.h>
#include <list.h>
#include <delete.h>
#include <logs.h>
#include <QTime>
#include <keyboard.h>
#include <blanker.h>
#include <QtMultimedia/QMediaPlayer>
namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSqlDatabase atmDB;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    AllSignature *p;
    blanker *b;
    List *plist;
    Delete *pdelete;
    Logs *plog;
    Keyboard *lineEditKeyboard1;
    QMediaPlayer *plno;
    QMediaPlayer *plok;

    QTimer *tgetReader=new QTimer(this);
    QTimer *tgenerateReader=new QTimer(this);
    QTimer *tsearchReader=new QTimer(this);
    QTimer *tAll=new QTimer(this);
    QTimer *tclear=new QTimer(this);
    QTimer *tclock=new QTimer(this);


    QByteArray Get;
    QByteArray Re_Get;
    QByteArray Generation;
    QByteArray Re_Generation;
    QByteArray Search;
    QByteArray Re_Search;

    QByteArray Dele;

    QFileSystemWatcher watcher;
    QPixmap known;
    QPixmap unknown;
    QPixmap Finger;
    QPixmap Finger_Title;
    QPixmap RFID;
    QPixmap RFID_Title;

    QTime time;








private slots:



    void FingerReader();
    void CardReader();

    void on_Btn_Exit_clicked();

    void on_Btn_RFID_clicked();


    void on_lineEdit_5_cursorPositionChanged(int arg1, int arg2);


    void on_pushButton_clicked();


    void on_Button_Search_clicked();

    void on_pushButton_Save_clicked();

    void on_lineEdit_4_cursorPositionChanged(int arg1, int arg2);


    void on_actionDelete_User_triggered();

    void on_actionAbout_triggered();

    void on_actionUser_Lists_triggered();

    void on_actionDisplay_Logs_triggered();

public slots:

    void V_Get_Reader();
    void V_Generate_Reader();
    void V_Search_Reader();

    void V_Generate_Write();
    void V_Search_Write();
    void Verify();
    void del();
    void center();
    void opener();
    void clear();
    void clock();
    void run_keyboard_lineEdit1();


    void on_pushButton_Register_clicked();

    void on_actionSetting_triggered();

    void on_actionSignature_triggered();

private:

    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
