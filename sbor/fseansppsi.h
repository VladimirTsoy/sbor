#ifndef FSEANSPPSI_H
#define FSEANSPPSI_H

#include <QWidget>
#include "obrizm.h"
#include "fstatfull.h"

namespace Ui {
class FSeansPpsi;
}

class FSeansPpsi : public QWidget
{
    Q_OBJECT

public:
    explicit FSeansPpsi(QWidget *parent = 0);
    ~FSeansPpsi();
    ObrIzm *cOI;

private:
    Ui::FSeansPpsi *ui;
    QString str_query;
    QSqlQueryModel *model;
    QModelIndex ind;
    void FormSpis1();
    void showEvent(QShowEvent *event);
    void spis_gr_no();
    void spis_gr_sut();
    void spis_gr_mes();
    void spis_gr_god();
    FStatFull *formStatFull;

public slots:
    void on_btnRefresh();
    void on_btnStat();
};

#endif // FSEANSPPSI_H
