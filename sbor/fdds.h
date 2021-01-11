#ifndef FDDS_H
#define FDDS_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"
#include "fstatdds.h"
#include "fddsview.h"

namespace Ui {
class FDds;
}

class FDds : public QWidget
{
    Q_OBJECT

public:
    explicit FDds(QWidget *parent = 0);
    ~FDds();
    ObrIzm *cOI;

private:
    Ui::FDds *ui;
    QString sql_select;
    QString sql_where;
    QString sql_order;
    QString sql_where_tip;
    QString sql_where_obr;
    QString sql_where_nka;
    QSqlQueryModel *m_dds;
    FTxtFileView *formTxtFileView;
    QModelIndex ind;
    FStatDDS *formStatDDS;
    FDdsView *formDdsView;

    void showEvent(QShowEvent *event);
    void showForm();
    void FormFiltrTip();
    void FormFiltrObr();
    void FormFiltrNka();

public slots:
    void on_btnRefresh();
    void on_btnList();
    void on_btnStat();
    void on_btnKonv();
    void on_btnDel();
    void on_btnVigr();
    void on_btnProt();
    void on_btnGraf();
};

#endif // FDDS_H
