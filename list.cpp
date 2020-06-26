#include "list.h"
#include "ui_list.h"
#include "QSqlQueryModel"
#include <sqlite.h>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlQueryModel>
#include <QDebug>

List::List(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::List)
{

    ui->setupUi(this);
    pushlist=ui->pushButton;
    QSqlDatabase myDB2;
    myDB2=QSqlDatabase::database("myConnection",true);
    if(myDB2.isOpen())
        qDebug() << ("DataBase open");

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
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();

    ui->tableView->show();



}

List::~List()
{
    delete ui;
}

void List::on_pushButton_clicked()
{



    this->close();
}
