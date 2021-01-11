#ifndef FSTATVRV_H
#define FSTATVRV_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FStatVRV;
}

class FStatVRV : public QWidget
{
    Q_OBJECT

public:
    explicit FStatVRV(QWidget *parent = 0);
    ~FStatVRV();
    quint64 nz_dds;
    quint64 nz_stat;
    ObrIzm *cOI;

private:
    Ui::FStatVRV *ui;
    QString query;
    QSqlQueryModel *m_statrvrv;
    QSqlQueryModel *m_alt;
    QModelIndex ind;
    void showEvent(QShowEvent *event);
};

#endif // FSTATVRV_H
