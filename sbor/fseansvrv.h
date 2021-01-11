#ifndef FSEANSVRV_H
#define FSEANSVRV_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FSeansVrv;
}

class FSeansVrv : public QWidget
{
    Q_OBJECT

public:
    explicit FSeansVrv(QWidget *parent = 0);
    ~FSeansVrv();
    ObrIzm *cOI;

private:
    Ui::FSeansVrv *ui;
    QSqlQueryModel *model;
    QModelIndex ind;
    void ViewSpis(qint32 n,qint32 k);
    void ViewSpisMes(qint32 god);
    QString str_nka;

public slots:
    void on_btnRefresh();
};

#endif // FSEANSVRV_H
