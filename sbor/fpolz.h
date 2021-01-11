#ifndef FPOLZ_H
#define FPOLZ_H

#include <QWidget>
#include "obrizm.h"
#include "fpolzedit.h"

namespace Ui {
class FPolz;
}

class FPolz : public QWidget
{
    Q_OBJECT

public:
    explicit FPolz(QWidget *parent = 0);
    ~FPolz();
    ObrIzm *cOI;

private:
    Ui::FPolz *ui;
    QString strsql;
    QSqlQueryModel *model;
    QModelIndex ind;
    void showEvent(QShowEvent *event);
    FPolzEdit *formPolzEdit;

public slots:
    void on_btnAdd();
    void on_btnDel();
    void on_btnUpd();
    void Refresh();

};

#endif // FPOLZ_H
