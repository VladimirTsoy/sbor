#ifndef FEDITSBA_H
#define FEDITSBA_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FEditSba;
}

class FEditSba : public QWidget
{
    Q_OBJECT

public:
    explicit FEditSba(QWidget *parent = 0);
    ~FEditSba();
    ObrIzm *cOI;
    quint64 nz;

private:
    Ui::FEditSba *ui;
    void showEvent(QShowEvent *event);
    void FormZap();

public slots:
    void on_pbSave();

signals:
    void ok();

};

#endif // FEDITSBA_H
