#ifndef FEDITNOS_H
#define FEDITNOS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FEditNos;
}

class FEditNos : public QWidget
{
    Q_OBJECT

public:
    explicit FEditNos(QWidget *parent = 0);
    ~FEditNos();
    ObrIzm *cOI;
    quint64 nz;

private:
    Ui::FEditNos *ui;
    void showEvent(QShowEvent *event);
    void Read();
    void Write();

public slots:
    void on_btnZap();
    void on_btnPath();

signals:
    void ok();
};

#endif // FEDITNOS_H
