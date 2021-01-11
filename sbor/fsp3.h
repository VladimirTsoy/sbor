#ifndef FSP3_H
#define FSP3_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"

namespace Ui {
class FSp3;
}

class FSp3 : public QWidget
{
    Q_OBJECT

public:
    explicit FSp3(QWidget *parent = 0);
    ~FSp3();
    ObrIzm *cOI;

private:
    Ui::FSp3 *ui;
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

#endif // FSP3_H
