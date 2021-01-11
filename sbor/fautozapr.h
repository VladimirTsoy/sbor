#ifndef FAUTOZAPR_H
#define FAUTOZAPR_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FAutoZapr;
}

class FAutoZapr : public QWidget
{
    Q_OBJECT

public:
    explicit FAutoZapr(QWidget *parent = 0);
    ~FAutoZapr();
    ObrIzm *cOI;

private:
    Ui::FAutoZapr *ui;
    void showEvent(QShowEvent *event);
    QList<qlonglong> nz_zapr;
    QList<qlonglong> list_nz;
    bool pr_close;
    bool pr_obr;
    void closeEvent(QCloseEvent *event);

public slots:
    void on_Otm();
    void on_Obr();
    void on_Abort();

};

#endif // FAUTOZAPR_H
