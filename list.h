#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>


namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT

public:
    explicit List(QWidget *parent = 0);
    QTableView *view;
    QPushButton *pushlist;
    ~List();

public slots:
    void on_pushButton_clicked();

private:
    Ui::List *ui;

};

#endif // LIST_H
