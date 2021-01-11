#ifndef FNFVIEW_H
#define FNFVIEW_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FNfView;
}

class FNfView : public QWidget
{
    Q_OBJECT

public:
    explicit FNfView(QWidget *parent = 0);
    ~FNfView();
    ObrIzm *cOI;
    quint64 nz,nz1;
    qint32 reg; //0-просмотр cogi; 1-просмотр cup; 2-создание новой
    QString nf_podt;

private:
    Ui::FNfView *ui;
    QStringList list,list1;
    QString ntable;
    QString nfile;
    QString cod;
//    void Zagr();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void ObrVxod();
    void ObrIsxod();
    void ObrNew();

public slots:
    void on_pbPodt();
    void on_pbOtpr();
    void on_pbCod();
    void on_pbOtpr2();
    void on_pbApodt();
    void on_pbOpodt();
    void on_Err();

signals:
    void Ret();
    void Err();
};

#endif // FNFVIEW_H
