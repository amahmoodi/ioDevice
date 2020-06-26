#include "about.h"
#include "ui_about.h"

ABOUT::ABOUT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABOUT)
{
    ui->setupUi(this);
}

ABOUT::~ABOUT()
{
    delete ui;
}

void ABOUT::on_lineEdit_textChanged(const QString &arg1)
{

}
