#ifndef FSIST_H
#define FSIST_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FSist;
}

class FSist : public QWidget
{
    Q_OBJECT

public:
    explicit FSist(QWidget *parent = 0);
    ~FSist();
    ObrIzm *cOI;

private:
    Ui::FSist *ui;
    QString strsql;
    QModelIndex ind;
    QSqlQueryModel *model;
    void showForm();
    void showEvent(QShowEvent *event);
    void FormSql();

public slots:
    void on_pbRefresh();

};

#endif // FSIST_H
