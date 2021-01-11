#ifndef FEDITSERV_H
#define FEDITSERV_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FEditServ;
}

class FEditServ : public QWidget
{
    Q_OBJECT

public:
    explicit FEditServ(QWidget *parent = 0);
    ~FEditServ();
    ObrIzm *cOI;
    quint64 nz;

private:
    Ui::FEditServ *ui;
    void showEvent(QShowEvent *event);
    void FormZap();

public slots:
    void on_pbSave();
    void on_pbIsdir();
    void on_pbVixdir();
    void on_pbCopdir();

signals:
    void ok();
};

#endif // FEDITSERV_H
