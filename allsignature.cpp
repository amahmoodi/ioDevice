#include "allsignature.h"
#include "ui_allsignature.h"
#include "signature.h"
#include "ui_signature.h"
#include <QSerialPort>
#include <QSqlDatabase>
#include <QDateTime>
#include <QSql>
#include <QSqlQuery>
#include <QtSql>
#include <sqlite.h>
#include <QIODevice>
#include <keyboard.h>
#include <QMessageBox>


QString Cnumber;
QDateTime Currentdt;
QString cardnumber,prsonid,Name,Family;

extern char Finger_IdL;
extern char Finger_IdH;
extern int FingerDone;
int Duplicate;


AllSignature::AllSignature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllSignature)
{
    ui->setupUi(this);

    Duplicate=0;
    SerialPort22.setPortName("/dev/ttyUSB0"); //COM-port that arduino is connected to
    SerialPort22.setBaudRate(9600);
    SerialPort22.open(QIODevice::ReadWrite);
    if (SerialPort22.isOpen())
        qDebug() << "Port 2 is open";


push=ui->pushButton_3;
push2=ui->pushButton_4;
lineEditKeyboard2=new Keyboard();
connect(&SerialPort22,SIGNAL(readyRead()),this,SLOT(CardReader2()));

connect(ui->lineEdit, SIGNAL(selectionChanged()), this, SLOT(run_keyboard_lineEdit2()));
connect(ui->lineEdit_3, SIGNAL(selectionChanged()), this, SLOT(run_keyboard_lineEdit2()));


}

AllSignature::~AllSignature()
{
    delete ui;


}

void AllSignature::on_pushButton_clicked()
{

    //SerialPort1.close();


   //mm=0x43;

    QMessageBox msgbox2;
    msgbox2.setText("ID is Empty");
    QString temp=ui->lineEdit_3->text();
    if(!temp.isEmpty()) {
        Signature *d2=new Signature();   d2->show();}
    else
        msgbox2.exec();

}

void AllSignature::on_pushButton_3_clicked()
{

   FingerDone=0;
   Finger_IdL=0;
   Duplicate=0;


   SerialPort22.close();
   this->close();


}
void AllSignature::CardReader2()
    {



    QByteArray DataRead=SerialPort22.readAll();
       ui->lineEdit_4->setText(DataRead);
       Cnumber=ui->lineEdit_4->text();
 }

void AllSignature::run_keyboard_lineEdit2()
{
QLineEdit *line=(QLineEdit *)sender();
lineEditKeyboard2->setLineEdit(line);
lineEditKeyboard2->show();


}

void AllSignature::on_pushButton_4_clicked()
{
    QString Fname,Pid,Adminestration;

    Currentdt=QDateTime::currentDateTime();
    Fname=ui->lineEdit->text();
    Pid=ui->lineEdit_3->text();
    Adminestration=ui->comboBox->currentText();


    char Image='0';

     myDB=QSqlDatabase::database("myConnection",true);
     myDB.open();
//*************************************************************
    if(myDB.isValid())
         qDebug() << myDB.isValid();
//*********************************************************************

    QMessageBox msgbox;
    QSqlQuery query3(this->myDB);
    QString quest;


    query3.exec("select * from Person where id='"+Pid+"'" );
    while (query3.next()) {
         quest= query3.value(0).toString();}


    if ((quest==Pid)&& !Pid.isEmpty()){
        msgbox.setText("Error :Id currently Existed"); msgbox.exec(); Duplicate=1; }
    else
        Duplicate=0;

    if (Pid.isEmpty()){
        msgbox.setText("Error :EMPTY ID "); msgbox.exec(); }

     if (Fname.isEmpty()){
        msgbox.setText("Error :EMPTY Name "); msgbox.exec(); }

    query3.prepare("INSERT INTO Person (id, FullName, CardNo, AccessType, FingerIdL, FingerIdH, Image) "
                    "VALUES (:id, :FullName, :CardNo, :AccessType, :FingerIdL, :FingerIdH, :Image)");


     query3.bindValue(":id", Pid);
     query3.bindValue(":FullName", Fname);
     query3.bindValue(":CardNo", Cnumber);
     query3.bindValue(":AccessType", Adminestration);
     query3.bindValue(":FingerIdL", Finger_IdL);
     query3.bindValue(":FingerIdH", Finger_IdH);
     query3.bindValue(":Image", Image );

    if((Finger_IdL!=0) & (FingerDone==1) & (Duplicate==0) & (!Pid.isEmpty()) & (!Fname.isEmpty()) ){
        if(query3.exec())
            ui->label_4->setText("Saved Successfully"); FingerDone=0;   Finger_IdL=0;  Duplicate=0;   SerialPort22.close();delay(2000); }
    else {
            msgbox.setText("Please Complete Signature"); msgbox.exec();}




    qDebug() << query3.lastError().text();

  //myDB.close();




}
