#ifndef FRPOM_H
#define FRPOM_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FRPom;
}

class FRPom : public QWidget
{
    Q_OBJECT

public:
    explicit FRPom(QWidget *parent = 0);
    ~FRPom();
    ObrIzm *cOI;

private:
    Ui::FRPom *ui;
    void showEvent(QShowEvent *event);
    QStringList sp_file;
    bool pr_close;
    void closeEvent(QCloseEvent *event);

public slots:
    void on_pbVib();
    void on_pbOtbr();
    void on_pbRun();
};

#endif // FRPOM_H
