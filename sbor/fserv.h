#ifndef FSERV_H
#define FSERV_H

#include <QWidget>
#include "obrizm.h"
#include "feditserv.h"

namespace Ui {
class FServ;
}

class FServ : public QWidget
{
    Q_OBJECT

public:
    explicit FServ(QWidget *parent = 0);
    ~FServ();
    ObrIzm *cOI;

private:
    Ui::FServ *ui;
    QString strsql;
    QModelIndex ind;
    QSqlQueryModel *model;
    QSqlQueryModel *model1;
    void showForm();
    void showEvent(QShowEvent *event);
    FEditServ *formEditServ;

public slots:
    void on_pbAdd();
    void on_pbRefresh();
    void on_pbEdit();
    void on_pbDel();
    void on_pbSave();
    void on_pbLoad();
};

#endif // FSERV_H
