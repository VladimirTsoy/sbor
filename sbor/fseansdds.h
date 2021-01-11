#ifndef FSEANSDDS_H
#define FSEANSDDS_H

#include <QWidget>
#include "obrizm.h"
#include "fpotdds.h"

namespace Ui {
class FSeansDds;
}

class FSeansDds : public QWidget
{
    Q_OBJECT

public:
    explicit FSeansDds(QWidget *parent = 0);
    ~FSeansDds();
    ObrIzm *cOI;

private:
    Ui::FSeansDds *ui;
    void ViewSpis(qint32 n, qint32 k);
    void ViewSpisMes(qint32 god);
    QSqlQueryModel *model;
    QModelIndex ind;
    QString str_nka;
    FPotDds *formPotDds;

public slots:
    void on_btnRefresh();
    void on_btnGraf();
};

#endif // FSEANSDDS_H
