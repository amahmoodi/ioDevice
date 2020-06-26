#include "blanker.h"
#include "ui_blanker.h"

extern int BlankCounter;


blanker::blanker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::blanker)
{
    ui->setupUi(this);
}

blanker::~blanker()
{
    delete ui;
}

void blanker::on_pushButton_clicked()
{
    BlankCounter=0;
    this->close();


}

void blanker::on_pushButton_pressed()
{
    BlankCounter=0;
    this->close();
}


