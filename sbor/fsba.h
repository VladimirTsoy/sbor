#ifndef FSBA_H
#define FSBA_H

#include <QWidget>
#include "obrizm.h"
#include "feditsba.h"

namespace Ui {
class FSba;
}

class FSba : public QWidget
{
    Q_OBJECT

public:
    explicit FSba(QWidget *parent = 0);
    ~FSba();
    ObrIzm *cOI;

private:
    Ui::FSba *ui;
    QString strsql;
    QSqlQueryModel *model;
    void showForm();
    void showEvent(QShowEvent *event);
    QModelIndex ind;
    FEditSba *formEditSba;

public slots:
    void on_pbRefresh();
    void on_pbAdd();
    void on_pbEdit();
    void on_pbDel();

};

#endif // FSBA_H
