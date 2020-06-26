#ifndef LOGS_H
#define LOGS_H

#include <QWidget>

namespace Ui {
class Logs;
}

class Logs : public QWidget
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = 0);
    ~Logs();

private slots:
    void on_pushButton_clicked();

    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::Logs *ui;
};

#endif // LOGS_H
