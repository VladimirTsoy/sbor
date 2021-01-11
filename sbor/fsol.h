#ifndef FSOL_H
#define FSOL_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"

namespace Ui {
class FSol;
}

class FSol : public QWidget
{
    Q_OBJECT

public:
    explicit FSol(QWidget *parent = 0);
    ~FSol();
    ObrIzm *cOI;

private:
    Ui::FSol *ui;
    QSqlQueryModel *model;
    QString select_query;
    QModelIndex ind;
    QString sort_query;
    FTxtFileView *formTxtFileView;
    QString filtr_dat;
    void showForm();
    void showEvent(QShowEvent *event);

public slots:
    void on_btnRefresh();
    void on_btnList();
    void on_btnDel();
    void on_btnVigr();

};

#endif // FSOL_H
