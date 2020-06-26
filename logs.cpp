#include "logs.h"
#include "ui_logs.h"
#include "QSqlQueryModel"
//#include <sqlite.h>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDebug>

Logs::Logs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);



    QSqlDatabase myDB2;
    myDB2=QSqlDatabase::database("myConnection",true);
    if(myDB2.isOpen())
        qDebug() << ("DataBase open");

    QSqlQueryModel *model=new QSqlQueryModel;
    QSqlQuery query(myDB2);
    query.prepare("SELECT * FROM RecordLog order by DateTime desc");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal, "   ID no  ");
    model->setHeaderData(1,Qt::Horizontal, "   Name   ");
    model->setHeaderData(2,Qt::Horizontal, "   Card No   ");
    model->setHeaderData(3,Qt::Horizontal, "   Date   ");


    ui->tableView->setModel(model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(3,Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->reset();
    ui->tableView->show();

}

Logs::~Logs()
{
    delete ui;
}

void Logs::on_pushButton_clicked()
{
    this->close();
}

void Logs::on_tableView_activated(const QModelIndex &index)
{

}
