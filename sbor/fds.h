#ifndef FDS_H
#define FDS_H

#include <QWidget>
#include "obrizm.h"
#include "ftxtfileview.h"

namespace Ui {
class FDs;
}

class FDs : public QWidget
{
    Q_OBJECT

public:
    explicit FDs(QWidget *parent = 0);
    ~FDs();
    QString nazis;
    ObrIzm *cOI;

private:
    Ui::FDs *ui;
    QSqlQueryModel *m_ds;
    QString select_query;
    QModelIndex ind;
    QString sort_query;
    QString where_query;
    FTxtFileView *formTxtFileView;
    QString filtr_npn;
    QStringList list_npn;
    QString filtr_dat;

    void showEvent(QShowEvent *event);
    void showForm();

public slots:
    void on_btnRefresh();
    void on_btnList();
    void on_btnFiltr();
    void on_btnDel();
    void on_btnVigr();

};

#endif // FDS_H
