#ifndef FCOGI_H
#define FCOGI_H

#include <QWidget>
#include "obrizm.h"
#include "fnfview.h"

namespace Ui {
class FCogi;
}

class FCogi : public QWidget
{
    Q_OBJECT

public:
    explicit FCogi(QWidget *parent = 0);
    ~FCogi();
    ObrIzm *cOI;

private:
    Ui::FCogi *ui;
    QString sql,where,order;
    QSqlQueryModel *model;
    QModelIndex ind;
    FNfView *formNfView;
    void showForm();
    void showEvent(QShowEvent *event);
    QString FiltrSost();
    QString FiltrDate();

public slots:
    void on_pbRefresh();
    void on_pbView();
    void on_cb();
    void on_dat();
    void on_pbDel();

};

#endif // FCOGI_H
