#ifndef FSEANSDS_H
#define FSEANSDS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FSeansDs;
}

class FSeansDs : public QWidget
{
    Q_OBJECT

public:
    explicit FSeansDs(QWidget *parent = 0);
    ~FSeansDs();
    ObrIzm *cOI;
    QString tip;

private:
    Ui::FSeansDs *ui;
    QString str_query;
    QSqlQueryModel *model;
    QModelIndex ind;
    QString filtr;
    void FormSpis1();
    void showEvent(QShowEvent *event);
    void spis_gr_no();
    void spis_gr_sut();
    void spis_gr_mes();
    void spis_gr_god();

public slots:
    void on_btnRefresh();
//    void Obr_cb_ns();
    void on_btnFiltr();

};

#endif // FSEANSDS_H
