#ifndef FUSTNF_H
#define FUSTNF_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FUstnf;
}

class FUstnf : public QWidget
{
    Q_OBJECT

public:
    explicit FUstnf(QWidget *parent = 0);
    ~FUstnf();
    ObrIzm *cOI;

private:
    Ui::FUstnf *ui;
    qlonglong nz;
    void showEvent(QShowEvent *event);

public slots:
    void ShowForm();
    void on_pbok();
    void on_pbisx();
    void on_pbvxod();

};

#endif // FUSTNF_H
