#ifndef FNOSD_H
#define FNOSD_H

#include <QWidget>
#include <obrizm.h>
#include "feditnos.h"

namespace Ui {
class FNosd;
}

class FNosd : public QWidget
{
    Q_OBJECT

public:
    explicit FNosd(QWidget *parent = 0);
    ~FNosd();
    ObrIzm *cOI;

private:
    Ui::FNosd *ui;
    QSqlQueryModel *model;
    QString query;
    QModelIndex ind;
    void showEvent(QShowEvent *event);
    FEditNos *formEditNos;
    void closeEvent(QCloseEvent *event);

public slots:
    void on_btnRefresh();
    void on_btnAdd();
    void on_btnEdit();
    void on_btnDel();
};

#endif // FNOSD_H
