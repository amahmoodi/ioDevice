#include "delete.h"
#include "ui_delete.h"
#include <QDebug>
#include <QSqlQuery>
#include <sqlite.h>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlQueryModel>
#include <QSerialPort>
#include <keyboard.h>
#include <QtDebug>

QSerialPort SerialPort41;
QSqlDatabase myDB2;
Delete::Delete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Delete)
{
    ui->setupUi(this);

    PushDelete=ui->pushButton_2;

    lineEditKeyboard3=new Keyboard();
    connect(ui->lineEdit, SIGNAL(selectionChanged()), this, SLOT(run_keyboard_lineEdit3()));


    myDB2=QSqlDatabase::database("myConnection",true);
    if(myDB2.isOpen())
        qDebug() << ("DataBase open");

    SerialPort41.setPortName("/dev/ttyUSB1"); //COM-port that arduino is connected to
    SerialPort41.open(QIODevice::ReadWrite);
    SerialPort41.setBaudRate(115200);
    SerialPort41.setStopBits(QSerialPort::OneStop);
    SerialPort41.setParity(QSerialPort::NoParity);
    SerialPort41.setDataBits(QSerialPort::Data8);


    QSqlQueryModel *model=new QSqlQueryModel;
    QSqlQuery query(myDB2);
    query.prepare("SELECT * FROM Person");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal, "ID no");
    model->setHeaderData(1,Qt::Horizontal, "Name");
    model->setHeaderData(2,Qt::Horizontal, "Card No");
    model->setHeaderData(3,Qt::Horizontal, "Access Type");
    model->setHeaderData(4,Qt::Horizontal, "Finger ID");
    model->setHeaderData(5,Qt::Horizontal, "FIDH ");

   ui->tableView->setModel(model);

}

Delete::~Delete()
{
    delete ui;

}

void Delete::on_pushButton_2_clicked()
{
    SerialPort41.close();
    this->close();
}

void Delete::on_pushButton_clicked()
{

    QString number,number2;
    int number3;
    QByteArray de,answer;
    number=ui->lineEdit->text();
     QSqlQuery query(myDB2);
           if(query.exec("select * from Person where id='"+number+"'" ) );
            while (query.next()) {
                 number2 = query.value(4).toString();}
            ui->lineEdit->setText(number2);
    number3=number2.toInt();
     de[0]=0x55;     de[1]=0xaa;  de[2]=0x00;   de[3]=0x00;    de[4]=0x44;    de[5]=0x00;
     de[6]=0x04;     de[7]=0x00;  de[8]=number3;   de[9]=0x00;    de[10]=number3;   de[11]=0x00;

      int i;
     for(i=12;i<=23;i++)
         de[i]=0x00;


     //char chs[];
    int xo=0x000;
    for (int i=0;i<=23 ;i++)
        xo+=de[i] ;


    de[24]=xo & 0xff;
    de[25]=xo >> 8;



    SerialPort41.write(de,26);
    SerialPort41.waitForBytesWritten(200);
    SerialPort41.waitForReadyRead(4000);
    answer=SerialPort41.readAll();
//ui->lineEdit_2->setText(answer.toHex());

   query.prepare("DELETE from Person where id='"+number+"'" );
   if( query.exec() )
       qDebug() << "yes";

   query.prepare("DELETE from RecordLog where id='"+number+"'" );
   if( query.exec() )
       qDebug() << "yes";



     QSqlQueryModel *model=new QSqlQueryModel;
     query.prepare("SELECT * FROM Person");
     query.exec();
     model->setQuery(query);
     model->setHeaderData(0,Qt::Horizontal, "ID no");
     model->setHeaderData(1,Qt::Horizontal, "Name");
     model->setHeaderData(2,Qt::Horizontal, "Card No");
     model->setHeaderData(3,Qt::Horizontal, "Access Type");
     model->setHeaderData(4,Qt::Horizontal, "Finger ID");
     model->setHeaderData(5,Qt::Horizontal, "FIDH ");

     ui->tableView->setModel(model);

}

void Delete::run_keyboard_lineEdit3()
{
QLineEdit *line=(QLineEdit *)sender();
lineEditKeyboard3->setLineEdit(line);
lineEditKeyboard3->show();


}


