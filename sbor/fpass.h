#ifndef FPASS_H
#define FPASS_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FPass;
}

class FPass : public QWidget
{
    Q_OBJECT

public:
    explicit FPass(QWidget *parent = 0);
    ~FPass();
    ObrIzm *cOI;
    QString title;

private:
    Ui::FPass *ui;
    void showEvent(QShowEvent *event);

public slots:
    void on_btnConnect();
    void on_btnExit();

signals:
    void ok();
    void no();
    void ret();

};

#endif // FPASS_H
