#ifndef FRNXV_H
#define FRNXV_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"

namespace Ui {
class FRnxv;
}

class FRnxv : public QWidget
{
    Q_OBJECT

public:
    explicit FRnxv(QWidget *parent = 0);
    ~FRnxv();
    ObrIzm *cOI;

private:
    Ui::FRnxv *ui;
    QString strsql,strsort;
    QSqlQueryModel *model;
    QModelIndex ind;
    FTxtFileView *formTxtFileView;
    QString FiltrFormat();
    QString FiltrNpn();
    QString FiltrDate();
    QString FiltrObr();
    QString FiltrKontr();

    void showEvent(QShowEvent *event);

public slots:
    void on_btnRefresh();
    void on_btnList();
    void on_btnProt();
    void on_btnVigr();
    void on_btnDel();
};

#endif // FRNXV_H
