#ifndef FEDITFULL_H
#define FEDITFULL_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FEditFull;
}

class FEditFull : public QWidget
{
    Q_OBJECT

public:
    explicit FEditFull(QWidget *parent = 0);
    ~FEditFull();
    ObrIzm *cOI;
    quint64 nz_full;

private:
    Ui::FEditFull *ui;
    void showEvent(QShowEvent *event);

public slots:
    void Read();
    void Write();

signals:
    void ok();
};

#endif // FEDITFULL_H
