#ifndef FKOLSEANSDS_H
#define FKOLSEANSDS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FKolSeansDs;
}

class FKolSeansDs : public QWidget
{
    Q_OBJECT

public:
    explicit FKolSeansDs(QWidget *parent = 0);
    ~FKolSeansDs();
    ObrIzm *cOI;
    QString tip;

private:
    Ui::FKolSeansDs *ui;
    QString str_query;
    QSqlQueryModel *model;
    QModelIndex ind;
    QString filtr;
    QStringList list_npn;
    void showEvent(QShowEvent *event);
    void FormSpis1();
    void spis_gr_sut();
    void spis_gr_mes();
    void spis_gr_god();
public slots:
    void on_btnRefresh();
    void Obr_cb_ns();
//    void on_btnFiltr();
};

#endif // FKOLSEANSDS_H
