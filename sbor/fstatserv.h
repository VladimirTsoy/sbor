#ifndef FSTATSERV_H
#define FSTATSERV_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FStatServ;
}

class FStatServ : public QWidget
{
    Q_OBJECT

public:
    explicit FStatServ(QWidget *parent = 0);
    ~FStatServ();
    ObrIzm *cOI;

private:
    Ui::FStatServ *ui;
    QString strsql, strsort;
    QSqlQueryModel *model;
    void FormSpis();
    void FormShow();
    QModelIndex ind;
    void showEvent(QShowEvent *event);

public slots:
    void on_pbRefresh();
};

#endif // FSTATSERV_H
