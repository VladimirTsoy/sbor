#ifndef FZAPR_H
#define FZAPR_H

#include <QWidget>
#include "obrizm.h"
#include "fustzapr.h"
#include "fedvid.h"

namespace Ui {
class FZapr;
}

class FZapr : public QWidget
{
    Q_OBJECT

public:
    explicit FZapr(QWidget *parent = 0);
    ~FZapr();
    ObrIzm *cOI;

private:
    Ui::FZapr *ui;
    QString select_query;
    QString select_where;
    QString select_order;
    QSqlQueryModel *model;
    QModelIndex ind;
    FUstZapr *formUstZapr;
    FEdvid *formEdvid;
    void ViewQuery();
    void showEvent(QShowEvent *event);
    void ShowForm();

public slots:
    void on_btnAdd();
    void on_btnDel();
    void on_btnEdit();
    void on_btnRefresh();
    void on_btnForm();
    void on_btnSpis();
};

#endif // FZAPR_H
