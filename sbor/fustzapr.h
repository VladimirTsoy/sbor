#ifndef FUSTZAPR_H
#define FUSTZAPR_H

#include <QWidget>
#include "obrizm.h"
#include "fpotr.h"

namespace Ui {
class FUstZapr;
}

class FUstZapr : public QWidget
{
    Q_OBJECT

public:
    explicit FUstZapr(QWidget *parent = 0);
    ~FUstZapr();
    ObrIzm *cOI;

private:
    Ui::FUstZapr *ui;
    FPotr *formPotr;
    void showEvent(QShowEvent *event);

public slots:
    void Read();
    void Write();
    void on_tbtnPotr();
    void IzmPotr(qlonglong nz);
    void on_tbtnPath();
//    void on_dtn();
    void on_rb_ust();
    void on_rb_tek();
    void on_rb_teksut();

signals:
    void SaveUstZapr();
};

#endif // FUSTZAPR_H
