#ifndef FEDVID_H
#define FEDVID_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FEdvid;
}

class FEdvid : public QWidget
{
    Q_OBJECT

public:
    explicit FEdvid(QWidget *parent = 0);
    ~FEdvid();
    ObrIzm *cOI;

private:
    Ui::FEdvid *ui;
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    QSqlQueryModel *model;
    QModelIndex ind;
    QString str_query;
    bool pr_close;
    bool pr_break;
    void btnVis(bool pr);

public slots:
    void on_btnRefresh();
    void on_btnVid();
    void on_btnClear();
    void on_btnDel();
};

#endif // FEDVID_H
