#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <signature.h>
#include <allsignature.h>
#include <sqlite.h>
#include <list.h>
#include <delete.h>
#include <wiringPi.h>
#include <QFile>
#include <QMessageBox>
#include <QIODevice>
#include <QDateTime>
#include <QPixmap>
#include <QSize>
#include <QTime>
#include <keyboard.h>
#include <blanker.h>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>

//#include "digitalclock.h"
//QSerialPortInfo SerialPortInfo;



int i;
int s;
int BlankCounter;
void myinterupt()
{s++; }


QSerialPort SerialPort1;
QSerialPort SerialPort2;
char QFinger_IdL;
char QFinger_IdH;
int controller;
int counter;
int max=40;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //this->setStyleSheet("background-color: rgb(226, 230, 255)");
    //*************** Setup GPIO in Linux **********************************


        QFile exportFile("/sys/class/gpio/export");
        exportFile.open(QIODevice::WriteOnly);
        exportFile.write("5");

        //unexportFile.setFileName("/sys/class/gpio/unexport");
        //unexportFile.open(QIODevice::WriteOnly);

        QFile directionFile("/sys/class/gpio/gpio5/direction");
        directionFile.open(QIODevice::WriteOnly);
        directionFile.write("in");

        QFile valFile("/sys/class/gpio/gpio5/value");
        valFile.open(QIODevice::WriteOnly);
        valFile.write("0");
        valFile.write("true");

        QFile edgeFile("/sys/class/gpio/gpio5/edge");
        edgeFile.open(QIODevice::ReadWrite);
        edgeFile.write("body");

        //system ("/usr/local/bin/gpio edge 5 rising") ;
    //******************** Setup Serial Ports *********************************************************
        SerialPort1.setPortName("/dev/ttyUSB1"); //COM-port that arduino is connected to
        SerialPort1.open(QIODevice::ReadWrite);
        SerialPort1.setBaudRate(115200);
        SerialPort1.setStopBits(QSerialPort::OneStop);
        SerialPort1.setParity(QSerialPort::NoParity);
        SerialPort1.setDataBits(QSerialPort::Data8);

        SerialPort2.setPortName("/dev/ttyUSB0"); //COM-port that arduino is connected to
        SerialPort2.open(QIODevice::ReadWrite);
        SerialPort2.setBaudRate(9600);

    //*******************keyboard************************88888
        lineEditKeyboard1=new Keyboard();

        connect(ui->lineEdit, SIGNAL(selectionChanged()), this, SLOT(run_keyboard_lineEdit1()));

        //******************* Config GPIO *******************************************


        counter=0;
        controller=1;
        BlankCounter=0;
        b=new blanker();
        //***************media***********************
        unknown.load("/home/pi/Sh2/Unknown.gif");
        RFID.load("/home/pi/Sh2/RFID.jpg");
        RFID_Title.load("/home/pi/Sh2/RFID_Reader.jpg");
        Finger.load("/home/pi/Sh2/Finger_Ok.png");
        Finger_Title.load("/home/pi/Sh2/Finger_Title.jpg");
        plok=new QMediaPlayer;
        plno=new QMediaPlayer;
        plok->setMedia(QUrl::fromLocalFile("/home/pi/Sh2/1.m4a"));
        plno->setMedia(QUrl::fromLocalFile("/home/pi/Sh2/2.m4a"));
        //******************************************************

        RFID_Title.scaled(QSize(20,20),  Qt::KeepAspectRatio);
        ui->label_6->setPixmap(Finger_Title);
        ui->label_7->setPixmap(RFID_Title);
        ui->l_Pic->setPixmap(unknown);

        connect(tgetReader, SIGNAL(timeout()), this, SLOT(V_Get_Reader()));
        //QTimer::singleShot(200000,tgetReader,SLOT(stop()));

        connect(tgenerateReader, SIGNAL(timeout()), this, SLOT(V_Generate_Reader()));
        //QTimer::singleShot(200000,tgenerateReader,SLOT(stop()));

        connect(tsearchReader, SIGNAL(timeout()), this, SLOT(V_Search_Reader()));
        //QTimer::singleShot(200000,tsearchReader,SLOT(stop()));

       // connect(tAll, SIGNAL(timeout()), this, SLOT(FingerReader()));
        //QTimer::singleShot(200000,tAll,SLOT(stop()));

        connect(tclear, SIGNAL(timeout()), this, SLOT(clear()));
        //QTimer::singleShot(200000,tclear,SLOT(stop()));


        connect(tclock, SIGNAL(timeout()), this, SLOT(clock()));
        //QTimer::singleShot(5000000,tclock,SLOT(stop()));




    setenv("WIRINGPI_GPIOMEM","1",1);
    wiringPiSetupGpio();
    wiringPiISR(5,INT_EDGE_RISING, &myinterupt);
    pinMode(5,INPUT);
    digitalWrite(5,LOW);
    watcher.addPath("/sys/class/gpio/gpio5/value");
    QObject::connect(&watcher, SIGNAL (fileChanged(QString)), \
                                    this, SLOT(center()));

//************************* Config DataBase ************************************************************

    //del();
    //tAll->start(30000);
        atmDB=QSqlDatabase::addDatabase("QSQLITE","myConnection");
        atmDB.setDatabaseName("/home/pi/IOdb.db");
        atmDB.open();

      if(atmDB.isOpen())
        qDebug() << ("DataBase open");
     else
         qDebug() <<("DataBase close");
    atmDB.close();



    if (SerialPort2.isOpen())
       qDebug() << ("Port2  open");
    else

       qDebug() << ("Port 2 close");
//**********************************************************************************************************
   connect(&SerialPort2,SIGNAL(readyRead()),this,SLOT(CardReader()));
//************************************************************
   //ui->lcdNumber->setDigitCount(8);
   clock();
   tclock->start(15000);

  }







MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::center()
 {
    if(controller==1){
        controller=0; FingerReader();}
    BlankCounter=0;
    b->close();


 }


void MainWindow::CardReader()
{

b->close();
BlankCounter=0;
//**********************************************
QByteArray DataRead=SerialPort2.readAll();
ui->lineEdit_6->setText(DataRead);
//Clear Lin Edit////////////////////////////////


       //
   atmDB=QSqlDatabase::database("myConnection",true);

   QString cardnumber,Pid,Fullname,AccessType;
   QDateTime eventdatetime;
   QDateTime Currentdt;
   Currentdt=QDateTime::currentDateTime();
   cardnumber=ui->lineEdit_6->text();

  QSqlQuery query(this->atmDB);

 if(query.exec("select * from Person where CardNo='"+cardnumber+"'" ) )
 { while (query.next()) {
         Pid = query.value(0).toString();
         Fullname = query.value(1).toString();
         cardnumber = query.value(2).toString();
         AccessType = query.value(3).toString();

         plok->play();
         ui->lineEdit_3->setText(Fullname);
         ui->lineEdit_4->setText(Pid);
         ui->lineEdit_5->setText(Currentdt.toString());
         ui->lineEdit_7->setText(cardnumber);
         ui->lineEdit_7->setText(cardnumber);
         ui->lineEdit_2->setText(AccessType);
         ui->l_Pic->setPixmap(RFID);
         query.prepare("INSERT INTO RecordLog (id, Personid, Cardno,DateTime) "
              "VALUES (:id, :Personid, :Cardno,:DateTime)");
         query.bindValue(":id", Pid);
         query.bindValue(":Personid", Fullname);
         query.bindValue(":Cardno", cardnumber);
         query.bindValue(":DateTime", Currentdt);
         query.exec();
     }
 }

 if(Pid.isEmpty()) {
     ui->lineEdit_3->setText("User Not Exist"); plno->play();}

tclear->start(3000);

}


void MainWindow::on_Btn_Exit_clicked()
{
    QString pass=ui->lineEdit->text();
    if (pass=="end")
        this->close();
}



void MainWindow::on_pushButton_clicked()
{}


void MainWindow::on_actionSetting_triggered()
{}



void MainWindow::on_actionSignature_triggered()
{
    SerialPort1.close();
    SerialPort2.close();
    p=new AllSignature();
   connect(p->push, SIGNAL(clicked()), this, SLOT(opener()));
   // connect(p->push2, SIGNAL(clicked()), this, SLOT(opener()));

   tAll->stop();
   tclock->stop();
   p->show();

}

void MainWindow::FingerReader()
{

    if (SerialPort1.isOpen())
        qDebug() << ("Port 1 open");
    else
        qDebug() << ("Port 1 close");

    Get[0]=0x55;
    Get[1]=0xaa;
    Get[2]=0x00;
    Get[3]=0x00;
    Get[4]=0x20;
    int i=0;
    for (i=5;i<=23;i++)
            Get[i]=0x00;


    Get[24]=0x1F;
    Get[25]=0x01;
    SerialPort1.write(Get,26);
    qDebug() << Get.toHex();
    tgetReader->start(20);

 }






void MainWindow::V_Get_Reader()
{
       counter++;

        Re_Get=SerialPort1.readAll();

        if(Re_Get[24]==0x22){tgetReader->stop();  V_Generate_Write(); counter=0; tgenerateReader->start(20); }
        if(counter==max) {tgetReader->stop(); counter=0; controller=1;  }


}


void MainWindow::V_Generate_Write()
{
    Generation[0]=0x55;
    Generation[1]=0xaa;
    Generation[2]=0x00;
    Generation[3]=0x00;
    Generation[4]=0x60;
    Generation[5]=0x00;
    Generation[6]=0x02;
    Generation[7]=0x00;
    Generation[8]=0x00;

    for (i=9;i<=23;i++)
        Generation[i]=0x00;

    Generation[24]=0x61;
    Generation[25]=0x01;
    SerialPort1.write(Generation,26);

}

void MainWindow::V_Generate_Reader()
 {
    counter++;
    Re_Generation=SerialPort1.readAll();

    if(Re_Generation[24]==0x62){
        tgenerateReader->stop();   V_Search_Write(); counter=0; tsearchReader->start(200);}
    if(counter==max) {
        tgetReader->stop();  controller=1; counter=0;  }


 }

void MainWindow::V_Search_Write()
 {


    Search[0]=0x55;
    Search[1]=0xaa;
    Search[2]=0x00;
    Search[3]=0x00;
    Search[4]=0x63;
    Search[5]=0x00;
    Search[6]=0x06;
    Search[7]=0x00;
    Search[8]=0x00;
    Search[9]=0x00;
    Search[10]=0x01;
    Search[11]=0x00;
    Search[12]=0xFF;
    Search[13]=0x00;

    for (i=14;i<=23;i++)
            Search[i]=0x00;

    Search[24]=0x68;
    Search[25]=0x02;

    SerialPort1.write(Search,26);
}

void MainWindow::V_Search_Reader()
 {
    counter++;

 Re_Search=SerialPort1.readAll();


 if(Re_Search[6]==0x05){
     tsearchReader->stop(); QFinger_IdL=Re_Search[10];  QFinger_IdH=Re_Search[11]; Verify(); }
 if(Re_Search[6]==0x02){
     tsearchReader->stop(); ui->lineEdit_3->setText("User Not Exist");  plno->play(); ui->lineEdit->setText("Finger Not Found");  tclear->start(3000);  controller=1;}

 if(counter==max) {
     tgetReader->stop();  controller=1; counter=0;  }

}
void MainWindow::Verify()
{

 ui->lineEdit_7->setText("Finger Id is:  "+QFinger_IdL);
 atmDB=QSqlDatabase::database("myConnection",true);
 QString Pid,Fullname,CardNo,AccessType,FingerIdL,FingerIdH;
 QDateTime Currentdt;
 Currentdt=QDateTime::currentDateTime();


QSqlQuery query(this->atmDB);
QString  temp=QString::number(QFinger_IdL);






        if(query.exec("select * from Person where FingerIdL='"+temp+"'" ) );
        while (query.next()) {
             Pid = query.value(0).toString();
             Fullname = query.value(1).toString();
             CardNo = query.value(2).toString();
             AccessType = query.value(3).toString();
             FingerIdL=query.value(4).toString();
             FingerIdH=query.value(5).toString();

             ui->lineEdit_3 ->setText(Fullname);
             ui->lineEdit_4->setText(Pid);
             ui->lineEdit_5->setText(Currentdt.toString());
             ui->lineEdit_7->setText(CardNo);
             ui->lineEdit_6->setText(CardNo);
             ui->lineEdit_2->setText(AccessType);
             ui->lineEdit->setText("ID Finger is:"+FingerIdL); }

             ui->l_Pic->setPixmap(Finger);
             plok->play();

              query.prepare("INSERT INTO RecordLog (id, Personid, Cardno, DateTime) "
                              "VALUES (:id, :Personid, :Cardno, :DateTime)");


               query.bindValue(":id", Pid);
               query.bindValue(":Personid", Fullname);
               query.bindValue(":Cardno", CardNo);
               query.bindValue(":DateTime", Currentdt);


              query.exec();
 tclear->start(3000);
}




void MainWindow::del()
{

Dele[0]=0x55; Dele[1]=0xaa; Dele[2]=0x00; Dele[3]=0x00; Dele[4]=0x44; Dele[5]=0x00; Dele[6]=0x04; Dele[7]=0x00;
Dele[8]=0x01; Dele[9]=0x00;
Dele[10]=0xFF; Dele[11]=0x00;

int i;
for(i=12;i<=23;i++)
    Dele[i]=0x00;

int xo=0x000;
for (int i=0;i<=23 ;i++)
    xo+=Dele[i] ;


Dele[24]=xo & 0xff;
Dele[25]=xo >> 8;


SerialPort1.write(Dele,26);


}

void MainWindow::opener()
{
    SerialPort1.setPortName("/dev/ttyUSB1"); //COM-port that arduino is connected to
    SerialPort1.open(QIODevice::ReadWrite);
    SerialPort1.setBaudRate(115200);
    SerialPort1.setStopBits(QSerialPort::OneStop);
    SerialPort1.setParity(QSerialPort::NoParity);
    SerialPort1.setDataBits(QSerialPort::Data8);

    SerialPort2.setPortName("/dev/ttyUSB0"); //COM-port that arduino is connected to
    SerialPort2.open(QIODevice::ReadWrite);
    SerialPort2.setBaudRate(9600);
    tclock->start();
}

void MainWindow::clear()
{
    ui->lineEdit_3 ->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_2->setText("");
    ui->l_Pic->setPixmap(unknown);
    tclear->stop();
    controller=1;
    //ui->lineEdit_7->setText();
}

void MainWindow::on_actionDelete_User_triggered()
{
    SerialPort1.close();
    SerialPort2.close();
    pdelete=new Delete();
   connect(pdelete->PushDelete,SIGNAL(clicked()),this,SLOT(opener()));

   tAll->stop();
   tclock->stop();
   pdelete->show();




}

void MainWindow::on_actionAbout_triggered()
{}

void MainWindow::on_actionUser_Lists_triggered()
{
    SerialPort1.close();
    SerialPort2.close();
    plist=new List();
   connect(plist->pushlist,SIGNAL(clicked()),this,SLOT(opener()));

   tAll->stop();
   tclock->stop();
   plist->show();


}

void MainWindow::on_actionDisplay_Logs_triggered()
{
   plog=new Logs();
   tAll->stop();
   BlankCounter=0;
   //tclock->stop();
   plog->show();

}

void MainWindow::clock()
{
    BlankCounter++;
    time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';

    ui->lcdNumber->display(text);

    if(BlankCounter==3){  b->show();}

}


void MainWindow::run_keyboard_lineEdit1()
{
QLineEdit *line=(QLineEdit *)sender();
lineEditKeyboard1->setLineEdit(line);
lineEditKeyboard1->show();


}
