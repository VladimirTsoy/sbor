#ifndef FUST_H
#define FUST_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FUst;
}

class FUst : public QWidget
{
    Q_OBJECT

public:
    explicit FUst(QWidget *parent = 0);
    ~FUst();
    ObrIzm *cOI;

private:
    Ui::FUst *ui;
    void showEvent(QShowEvent *event);
    qlonglong nz;
    QSqlQueryModel *model;
    void UpdateUst();
public slots:
    void on_btnOk();
    void on_btnCancel();
    void on_btnSir();
    void on_btnTemp();
    void ShowForm();
    void on_btnSave();
    void on_btnLoad();
};

#endif // FUST_H
