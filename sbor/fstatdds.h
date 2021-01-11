#ifndef FSTATDDS_H
#define FSTATDDS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FStatDDS;
}

class FStatDDS : public QWidget
{
    Q_OBJECT

public:
    explicit FStatDDS(QWidget *parent = 0);
    ~FStatDDS();
    quint64 nz_dds;
    ObrIzm *cOI;

private:
    Ui::FStatDDS *ui;
    QString query;
    QSqlQueryModel *m_statrdds;
    QModelIndex ind;
    void showEvent(QShowEvent *event);
    void showForm();
};

#endif // FSTATDDS_H
