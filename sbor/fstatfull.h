#ifndef FSTATFULL_H
#define FSTATFULL_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FStatFull;
}

class FStatFull : public QWidget
{
    Q_OBJECT

public:
    explicit FStatFull(QWidget *parent = 0);
    ~FStatFull();
    ObrIzm *cOI;

    qlonglong nz_full;
    qlonglong nz_full2;

private:
    Ui::FStatFull *ui;
    void showEvent(QShowEvent *event);
    void showForm();
};

#endif // FSTATFULL_H
