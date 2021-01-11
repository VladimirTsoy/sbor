#ifndef FPOLZEDIT_H
#define FPOLZEDIT_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FPolzEdit;
}

class FPolzEdit : public QWidget
{
    Q_OBJECT

public:
    explicit FPolzEdit(QWidget *parent = 0);
    ~FPolzEdit();
    QString naz_polz;
    ObrIzm *cOI;

private:
    Ui::FPolzEdit *ui;
    void showEvent(QShowEvent *event);

public slots:
    void on_btnOk();

signals:
    void ok();
};

#endif // FPOLZEDIT_H
