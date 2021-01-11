#ifndef FCUP_H
#define FCUP_H

#include <QWidget>
#include "obrizm.h"
#include "fnfview.h"

namespace Ui {
class FCup;
}

class FCup : public QWidget
{
    Q_OBJECT

public:
    explicit FCup(QWidget *parent = 0);
    ~FCup();
    ObrIzm *cOI;

private:
    Ui::FCup *ui;
    QString sql,where,order;
    QSqlQueryModel *model;
    QModelIndex ind;
    void showForm();
    void showEvent(QShowEvent *event);
    QString FiltrDate();
    QString FiltrSost();
    FNfView *formNfView;

public slots:
    void on_pbRefresh();
    void on_cb();
    void on_dat();
    void on_pbView();
    void on_pbDel();
    void on_pbAdd();
};

#endif // FCUP_H
