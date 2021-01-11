#ifndef FKOS_H
#define FKOS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FKos;
}

class FKos : public QWidget
{
    Q_OBJECT

public:
    explicit FKos(QWidget *parent = 0);
    ~FKos();
    ObrIzm *cOI;

private:
    Ui::FKos *ui;
    QSqlQueryModel *model;
    QString query, order;
    QModelIndex ind;
    void showEvent(QShowEvent *event);
    QStringList list_npn;
    QString filtr_npn;
    QString filtr_dat;


public slots:
    void on_btnRefresh();
    void on_btnFiltr();
    void on_btnDel();
    void on_btnVigr();
};

#endif // FKOS_H
