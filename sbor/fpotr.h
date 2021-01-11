#ifndef FPOTR_H
#define FPOTR_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FPotr;
}

class FPotr : public QWidget
{
    Q_OBJECT

public:
    explicit FPotr(QWidget *parent = 0);
    ~FPotr();
    ObrIzm *cOI;
    bool show_vib;

private:
    Ui::FPotr *ui;
    QSqlTableModel *model;
    qlonglong nz;
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void on_btnInsert();
    void on_btnSave();
    void on_btnCancel();
    void on_btnDel();
    void on_btnRefresh();
    void on_btnVib();

signals:
    void NzPotr(qlonglong);
};

#endif // FPOTR_H
