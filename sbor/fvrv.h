#ifndef FVRV_H
#define FVRV_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"
#include "fstatvrv.h"

namespace Ui {
class FVrv;
}

class FVrv : public QWidget
{
    Q_OBJECT

public:
    explicit FVrv(QWidget *parent = 0);
    ~FVrv();
    QString path_temp;
    ObrIzm *cOI;

private:
    Ui::FVrv *ui;
    QString sql_select;
    QString sql_where;
    QString sql_order;
    QString sql_where_tip;
    QString sql_where_obr;
    QString sql_where_nka;
    QSqlQueryModel *m_vrv;
    QModelIndex ind;
    FTxtFileView *formTxtFileView;
    FStatVRV *formStatVRV;
    void showForm();
    void showEvent(QShowEvent *event);
    void FormFiltrTip();
    void FormFiltrObr();
    void showTitle();
    void FormFiltrNka();

public slots:
    void on_btnRefresh();
    void on_btnList();
    void on_btnStat();
    void on_btnPlan();
    void on_btnKonva();
    void on_btnDel();
    void on_btnVigr();
};

#endif // FVRV_H
