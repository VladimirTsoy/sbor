#ifndef FFULL_H
#define FFULL_H

#include "obrizm.h"
#include "ftxtfileview.h"
#include "fstatfull.h"
#include "feditfull.h"
#include "threadobr12.h"
#include "../ppsi/ppsi.h"

namespace Ui {
class FFull;
}

class FFull : public QWidget
{
    Q_OBJECT

public:
    explicit FFull(QWidget *parent = 0);
    ~FFull();
    ObrIzm *cOI;
private:
    Ui::FFull *ui;
    QSqlQueryModel *m_fullppsi;
    QString query_select;
    QString query_where;
    QString query_where1;
    QString query_order;
    QModelIndex ind;
    FTxtFileView *formTxtFileView;
    FStatFull *formStatFull;
    FEditFull *formEditfull;
    ppsi *PPSI;
    QTimer *timer;
    ThreadObr12 tobr12;
    QStringList list_npn;

    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void focusChanged( QWidget*, QWidget *to );
    void showForm();
    void FormFiltr();
    void Report( QTableView *table, QString zag );
    QString FormFiltrKan();
    QString FormFiltrSost();
    QString FormFiltrDat();
    QString FormFiltrNka();
    QString FormFiltrNpn();

public slots:
    void on_btnRefresh();
    void on_btnListProt();
    void on_btnStat();
    void on_btnEdit();
    void on_btnDel();
    void on_btnKonv();
    void ShowPb();
    void EndKonv();
    void on_btnVigr();
    void DravPPSI();
};

#endif // FFULL_H
