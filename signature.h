#ifndef SIGNATURE_H
#define SIGNATURE_H
#include <QFileSystemWatcher>
#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QFont>
#include <mainwindow.h>


namespace Ui {
class Signature;
}

class Signature : public QWidget
{
    Q_OBJECT

public:
    explicit Signature(QWidget *parent = 0);
    ~Signature();
    Ui::Signature *ui;


    //QFileSystemWatcher watcher;

    int counter=0;
    int step=0;
    int state=0;

    QSerialPort SerialPort31;
    QTimer *tgetimage=new QTimer(this);
    QTimer *tgenerate1=new QTimer(this);
    QTimer *tgenerate2=new QTimer(this);
    QTimer *tgenerate3=new QTimer(this);
    QTimer *tgetimage2=new QTimer(this);

    QTimer *tgetid=new QTimer(this);
    QTimer *tgetstatus=new QTimer(this);
    QTimer *tmerge=new QTimer(this);
    QTimer *tstore=new QTimer(this);
    QTimer *twriteimage=new QTimer(this);
    QMessageBox msgBox;
    QPixmap P1;
    QPixmap P2;
    QPixmap P3;


    int Checksum(QByteArray input);

    QByteArray ConnectionTest;
    QByteArray Re_ConnectionTest;
    QByteArray GetId;
    QByteArray Re_GetId;
    QByteArray Permitt;
    QByteArray Re_Permitt;
    QByteArray GetImage;
    QByteArray Re_GetImage;

    QByteArray Generate1;
    QByteArray Re_Generate1;
    QByteArray Generate2;
    QByteArray Re_Generate2;
    QByteArray Generate3;
    QByteArray Re_Generate3;
    QByteArray merge;
    QByteArray Re_merge;
    QByteArray Store;
    QByteArray Re_Store;
    QByteArray del;



    //char Finger_IdL;
    //char Finger_IdH;
    //char a='a';


   // QByteArray temp1;
   // QByteArray temp2;
public slots:
    void Reader();
    void fTestConnection();
    void f_get_id();
    void f_get_id_write();

    void f_get_status();
    void f_get_status_write();

    void f_get_image();
    void f_get_image_write();

    void re_enroll_permit();

    void f_generate_1();
    void f_generate_1_write();
    void f_generate_2();
    void f_generate_2_write();
    void f_generate_3();
    void f_generate_3_write();
    void re_get_image();
    void f_merge();
    void f_merge_write();
    void f_store();
    void f_store_write();



private slots:
    void on_pushButton_clicked();




    void on_pushButton_2_clicked();

    void on_spinBox_editingFinished();

private:



};

#endif // SIGNATURE_H
