#ifndef FRNXI_H
#define FRNXI_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"

namespace Ui {
class FRnxi;
}

class FRnxi : public QWidget
{
    Q_OBJECT

public:
    explicit FRnxi(QWidget *parent = 0);
    ~FRnxi();
    ObrIzm *cOI;
    bool pr_meteo;

private:
    Ui::FRnxi *ui;
    QString strsql,strsort;
    QSqlQueryModel *model;
    QModelIndex ind;
    FTxtFileView *formTxtFileView;
    QStringList list_npn;
    QString FiltrMeteo();
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
    void on_btnDel();
    void on_btnVigr();
    void on_btnObr();
};

#endif // FRNXI_H
