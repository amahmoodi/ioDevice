#include "signature.h"
#include "ui_signature.h"
#include "mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QFile>
#include <QMessageBox>
#include <QIODevice>
#include <QSocketNotifier>
#include <wiringPi.h>
#include <qtimer.h>
#include <QTimer>




//static int s;
//void myinterupt()
//{s++;}



//extern QSerialPort SerialPort2;
//extern QSerialPort SerialPort1;


char Finger_IdL;
char Finger_IdH;
int FingerDone;

Signature::Signature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signature)
{

    ui->setupUi(this);

//************** Config Serial Port ****************************
    SerialPort31.setPortName("/dev/ttyUSB1"); //COM-port that arduino is connected to
    SerialPort31.open(QIODevice::ReadWrite);
    //SerialPort11.close();
    SerialPort31.setStopBits(QSerialPort::OneStop);
    SerialPort31.setBaudRate(115200);
    SerialPort31.setDataBits(QSerialPort::Data8);
    //SerialPort1.setStopBits(QSerialPort::OneStop);
    SerialPort31.setParity(QSerialPort::NoParity);

 //*****************************************************************
FingerDone=0;
Finger_IdL=0;
//***************************************



//char number=0x03;
//enrol_permit(number);
FingerDone=0;
state=0;
step=0;


P1.load("/home/pi/Sh2/P1.jpg");
P2.load("/home/pi/Sh2/P2.jpg");
P3.load("/home/pi/Sha2/P3.jpg");



connect(tgetid, SIGNAL(timeout()), this, SLOT(f_get_id()));
QTimer::singleShot(100000,tgetid,SLOT(stop()));
f_get_id_write();
tgetid->start(100);


connect(tgetstatus, SIGNAL(timeout()), this, SLOT(f_get_status()));
//QTimer::singleShot(200000,tgetstatus,SLOT(stop()));

connect(tgetimage, SIGNAL(timeout()), this, SLOT(f_get_image()));
//QTimer::singleShot(200000,tgetimage,SLOT(stop()));

connect(tgenerate1, SIGNAL(timeout()), this, SLOT(f_generate_1()));
//QTimer::singleShot(200000,tgenerate1,SLOT(stop()));

connect(tgenerate2, SIGNAL(timeout()), this, SLOT(f_generate_2()));
//QTimer::singleShot(200000,tgenerate2,SLOT(stop()));

connect(tgenerate3, SIGNAL(timeout()), this, SLOT(f_generate_3()));
//QTimer::singleShot(20000,tgenerate3,SLOT(stop()));

connect(tmerge, SIGNAL(timeout()), this, SLOT(f_merge()));
//QTimer::singleShot(200000,tmerge,SLOT(stop()));

connect(tstore, SIGNAL(timeout()), this, SLOT(f_store()));
//QTimer::singleShot(200000,tstore,SLOT(stop()));

connect(twriteimage, SIGNAL(timeout()), this, SLOT(f_get_image_write()));
//QTimer::singleShot(200000,twriteimage,SLOT(stop()));



//*****************************************************************************

//******************* Config GPIO *******************************************

}

Signature::~Signature()
{

     delete ui;

}

void Signature::Reader()
{


QString ss=QString::number(state);
//ui->lineEdit_2->setText(ss);

}

int Signature::Checksum(QByteArray input)
{

 //char chs[];
int xo=0x000;
for (int i=0;i<=23 ;i++)
    xo+=input[i] ;
return xo;


}
void Signature::on_pushButton_clicked()
{

    SerialPort31.close();
    tgetimage->stop();
    tstore->stop();
    tmerge->stop();
    tgenerate1->stop();
    tgenerate2->stop();
    tgenerate3->stop();
    tgetid->stop();
    tgetstatus->stop();
    tstore->stop();
    tgetstatus->stop();
    twriteimage->stop();
    this->close();
}

void Signature::fTestConnection()
{


ConnectionTest[0]=0x55;
ConnectionTest[1]=0xaa;
ConnectionTest[2]=0x00;
ConnectionTest[3]=0x00;
ConnectionTest[4]=0x01;
for (int i=5;i<=24;i++)
    ConnectionTest[i]=0x00;
ConnectionTest[25]=0x01;
SerialPort31.write(ConnectionTest,26);
//ui->lineEdit->setText(ConnectionTest.toHex());

Re_ConnectionTest=SerialPort31.readAll();
}


void Signature::f_get_id()
{
     state++;

     Re_GetId=SerialPort31.readAll();


     if(Re_GetId[6]==0x04){

         ui->l_GetId_Ans->setText("ID Determined  ");
         state=0;   tgetid->stop(); step=1; f_get_status_write(); tgetstatus->start(100); }

     if(state==100){
         ui->l_GetId_Ans->setText("Can not Assign ID");  state=0; tgetid->stop(); f_get_id_write(); tgetid->start(100);}
}


void Signature::f_get_id_write()
{

    GetId[0]=0x55;
    GetId[1]=0xaa;
    GetId[2]=0x00;
    GetId[3]=0x00;
    GetId[4]=0x45;
    GetId[5]=0x00;
    GetId[6]=0x04;
    GetId[7]=0x00;
    GetId[8]=0x01;
    GetId[9]=0x00;
    GetId[10]=0xFF;
    GetId[11]=0x00;
    int i=0;
    for (i=12;i<=23;i++)
        GetId[i]=0x00;

   GetId[24]=Checksum(GetId)& 0xff;
   GetId[25]=Checksum(GetId) >> 8;
   SerialPort31.write(GetId,26);

    }



void Signature::f_get_status()
{

       state++;
    delay(20);
    Re_Permitt=SerialPort31.readAll();
    if(Re_Permitt[6]==0x03)
    {ui->l_GetStatus_Ans->setText("ID ASSIGNED Successfully"); step=2;  tgetid->stop(); tgetstatus->stop();  ui->l_GetFinger1->setText("Put Finger on Sensor For First (3 Sec)");  ui->lPic_1->setPixmap(P1); state=0; step=2;  twriteimage->start(3500);  tgetimage->start(100);}

    if(state==10){ui->l_GetStatus_Ans->setText("ID Assigned Successfully"); tgetstatus->stop(); f_get_status_write(); tgetstatus->start();}

}

void Signature::f_get_status_write()
{

    Permitt[0]=0x55;
    Permitt[1]=0xaa;
    Permitt[2]=0x00;
    Permitt[3]=0x00;
    Permitt[4]=0x46;
    Permitt[5]=0x00;
    Permitt[6]=0x02;
    Permitt[7]=0x00;
    Permitt[8]=Re_GetId[10];
    Permitt[9]=Re_GetId[11];



    int i=0;
    for (i=10;i<=23;i++)
        Permitt[i]=0x00;


    Permitt[24]=Checksum(Permitt)& 0xff;
    Permitt[25]=Checksum(Permitt) >> 8;

    SerialPort31.write(Permitt,26);


    }

void Signature::f_get_image()
{
    state++;

    Re_GetImage=SerialPort31.readAll();

    if ((Re_GetImage[24]==0x22) & (step==2)) { twriteimage->stop(); tgetimage->stop(); ui->l_getimage1_Ans->setText("Thank you");  ui->lPic_1->setPixmap(P2);  state=0; f_generate_1_write(); tgenerate1->start(100);}

    if ((Re_GetImage[24]==0x22) & (step==3)) { twriteimage->stop(); tgetimage->stop(); ui->l_getimage2_ans->setText("Thank you");  ui->lPic_2->setPixmap(P2);  state=0; f_generate_2_write(); tgenerate2->start(100);}

    if ((Re_GetImage[24]==0x22) & (step==4)) { twriteimage->stop(); tgetimage->stop(); ui->l_getimage3_ans->setText("Thank you");   ui->lPic_3->setPixmap(P2); state=0;  f_generate_3_write(); tgenerate3->start(100);}







}


void Signature::f_get_image_write()
{

    GetImage[0]=0x55;
    GetImage[1]=0xaa;
    GetImage[2]=0x00;
    GetImage[3]=0x00;
    GetImage[4]=0x20;
      int i=0;
    for (i=5;i<=23;i++)
        GetImage[i]=0x00;

    //Checksum(Senddata);
    GetImage[24]=Checksum(GetImage)& 0xff;
    GetImage[25]=Checksum(GetImage) >> 8;

    //ui->lineEdit_2->setText(GetImage.toHex());

    SerialPort31.write(GetImage,26);
}



void Signature::f_generate_1()
{
    state++;
    Re_Generate1=SerialPort31.readAll();
  //ui->le_Generate1->setText(Generate1.toHex());
    qDebug()<< Generate1.toHex();
    if(Re_Generate1[24]==0x62){
       tgenerate1->stop();  ui->l_gener_ans->setText("Generation Succeed"); ui->lPic_1->setPixmap(P3);  state=0;  step=3;  delay(2000);  ui->l_GetFinger2->setText("Put Finger on Sensor For Secound (3 Sec)"); ui->lPic_2->setPixmap(P1); twriteimage->start(3500); tgetimage->start(100);
}

   if(state==100){
       tgenerate1->stop(); ui->l_gener_ans->setText("Generation (1) Failed");   ui->l_GetFinger1->setText("Put Finger Again on Sensor"); ui->lPic_1->setPixmap(P1);  state=0; step=2; twriteimage->start(3000);  tgetimage->start(100);
   }
}

void Signature::f_generate_1_write()
{

    Generate1[0]=0x55;
    Generate1[1]=0xaa;
    Generate1[2]=0x00;
    Generate1[3]=0x00;
    Generate1[4]=0x60;
    Generate1[5]=0x00;
    Generate1[6]=0x02;
    Generate1[7]=0x00;
    Generate1[8]=0x00;
    int i=0;
    for (i=9;i<=23;i++)
        Generate1[i]=0x00;

    Generate1[24]=Checksum(Generate1)& 0xff;
    Generate1[25]=Checksum(Generate1) >> 8;
    SerialPort31.write(Generate1,26);

}

void Signature::f_generate_2()
{
    state++;


    Re_Generate2=SerialPort31.readAll();


    //ui->lineEdit->setText(Generate1.toHex());

    if(Re_Generate2[24]==0x62){tgenerate2->stop();  ui->l_gener2_ans->setText("Generation Succeed");  ui->lPic_2->setPixmap(P3); delay(2000); ui->l_GetFinger3->setText("Put Finger on Sensor For third (3sec)");   ui->lPic_3->setPixmap(P1); state=0; step=4; twriteimage->start(3500);  tgetimage->start(100);
    }

   if(state==100){
       tgenerate1->stop(); ui->l_gener2_ans->setText("Generation (2) Failed");  state=0; step=3;  ui->l_GetFinger2->setText("Put Finger Again on Sensor");   twriteimage->start(3500);  tgetimage->start(0);}}


void Signature::f_generate_2_write()

{
    Generate2[0]=0x55;
    Generate2[1]=0xaa;
    Generate2[2]=0x00;
    Generate2[3]=0x00;
    Generate2[4]=0x60;
    Generate2[5]=0x00;
    Generate2[6]=0x02;
    Generate2[7]=0x00;
    Generate2[8]=0x01;
    int i=0;
    for (i=9;i<=23;i++)
        Generate2[i]=0x00;


    Generate2[24]=Checksum(Generate2)& 0xff;
    Generate2[25]=Checksum(Generate2) >> 8;

    SerialPort31.write(Generate2,26);

}


void Signature::f_generate_3()
{
    state++;

    Re_Generate3=SerialPort31.readAll();


    if(Re_Generate3[24]==0x62){
        tgenerate3->stop(); ui->l_gener3_ans->setText("Generation Succeed"); ui->lPic_3->setPixmap(P3); state=0; step=5; delay(5); f_merge_write(); tmerge->start(500);}

   if(state==10){
       tgenerate1->stop(); ui->l_gener3_ans->setText("Generation (3) Failed");   state=0; step=4;
        ui->l_GetFinger3->setText("Put Finger again on Sensor");   delay(1000); f_get_image_write();  tgetimage->start(50);}

}

void Signature::f_generate_3_write()
{

    Generate3[0]=0x55;
    Generate3[1]=0xaa;
    Generate3[2]=0x00;
    Generate3[3]=0x00;
    Generate3[4]=0x60;
    Generate3[5]=0x00;
    Generate3[6]=0x02;
    Generate3[7]=0x00;
    Generate3[8]=0x02;
    int i=0;
    for (i=9;i<=23;i++)
        Generate3[i]=0x00;


    Generate3[24]=0x63;
    Generate3[25]=0x01;

    SerialPort31.write(Generate3,26);


}

void Signature::f_merge()
{

   state++;
    Re_merge=SerialPort31.readAll();

    //ui->lineEdit->setText(Generate3.toHex());
    //ui->le_GetImage3->setText(Generate3.toHex());
     //ui->le_Generate3->setText(Re_Generate3.toHex());

    if(Re_merge[24]==0x63){
        ui->l_Merge->setText("Merged Successfully"); state=0; tmerge->stop();  f_store_write(); tstore->start(70);   }

   if(state==300){
       tmerge->stop(); step=0;  ui->l_Merge->setText("Merged Failed"); }

}



void Signature::f_merge_write()
{


    merge[0]=0x55;
    merge[1]=0xaa;
    merge[2]=0x00;
    merge[3]=0x00;
    merge[4]=0x61;
    merge[5]=0x00;
    merge[6]=0x03;
    merge[7]=0x00;
    merge[8]=0x00;
    merge[9]=0x00;
    merge[10]=0x03;

    int i=0;
    for (i=11;i<=23;i++)
        merge[i]=0x00;


    merge[24]=0x66;
    merge[25]=0x01;

    SerialPort31.write(merge,26);

}



void Signature::f_store_write()
{


    Store[0]=0x55;
    Store[1]=0xaa;
    Store[2]=0x00;
    Store[3]=0x00;
    Store[4]=0x40;
    Store[5]=0x00;
    Store[6]=0x04;
    Store[7]=0x00;
    Store[8]=Re_GetId[10];
    Store[9]=Re_GetId[11];
    Store[10]=0x00;

    int i=0;
    for (i=11;i<=23;i++)
        Store[i]=0x00;


 Store[24]= Checksum(Store)& 0xff;
 Store[25]=Checksum(Store) >> 8;

 SerialPort31.write(Store,26);



}


void Signature::f_store()
{
   Re_Store[8]=0x01;

    state++;

    Re_Store=SerialPort31.readAll();


    if(Re_Store[24]==0x42){FingerDone=1; ui->l_Result->setText("Saved Success Fully");  state=0; tstore->stop(); }
    if(Re_Store[8]==0x18){ ui->l_Result->setText("Error Enrolled Finger");  state=0; tstore->stop();  }

   if(state==50){
          msgBox.setText("Store Failed"); msgBox.exec(); state=0;  tstore->stop();  }

  Finger_IdL=Store[8];
  Finger_IdH=Store[9];


  //ui->lineEdit_3->setText(QString::number(Finger_IdH));

}




