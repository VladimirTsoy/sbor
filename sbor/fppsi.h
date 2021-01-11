#ifndef FPPSI_H
#define FPPSI_H

#include <QObject>
#include <QWidget>
#include "obrizm.h"

namespace Ui {
    class FPPsi;
}

class FPPsi : public QWidget
{
    Q_OBJECT
public:
    explicit FPPsi(QWidget *parent = 0);
    QString sqlSelect;

    const QString queryYearSelect = "SELECT tab.godn, count(tab.res), count(NULLIF(tab.res, '')) FROM tab ";
    const QString queryYearGroupBy = "GROUP BY tab.godn ORDER BY tab.godn";

    const QString queryDaySelect = "SELECT tab.godn, tab.date_plan,tab.month_, count(tab.res), count(NULLIF(tab.res, '')) FROM tab ";
    const QString queryDayGroupBy = "GROUP BY tab.date_plan, tab.month_,tab.godn ORDER BY tab.date_plan";

    const QString queryMonthSelect = "SELECT tab.godn, tab.month_, count(tab.res), count(NULLIF(tab.res, '')) FROM tab ";
    const QString queryMonthGroupBy = "GROUP BY tab.month_, tab.godn ORDER BY tab.month_";

    const QString queryAll = "SELECT nz,npn,nka,godn,date_plan,month_,to_char(make_time, 'HH24:MI:SS'),kan_1,kan_2 FROM tab";

    const QString queryBetweenConst = "WHERE tab.date_plan BETWEEN '2000-01-01' AND '2100-01-01'";

    const QString queryBegin = "WITH tFinallSeans as(SELECT fullppsi.nka, plppsi.nka, plppsi.npn, fullppsi.naz, plppsi.sutn AS sut,plppsi.sekn AS sek, fullppsi.sutn, fullppsi.sekn, fullppsi.kan  \
                FROM plppsi, fullppsi                                                                                                                               \
                WHERE (fullppsi.godn * 1000000 + fullppsi.sutn * 100000 + fullppsi.sekn > plppsi.godn * 1000000 + plppsi.sutn * 100000 + plppsi.sekn - 60 AND       \
                    fullppsi.godn * 1000000 + fullppsi.sutn * 100000 + fullppsi.sekn < plppsi.godn * 1000000 + plppsi.sutn * 100000 + plppsi.sekn + 60)),           \
          k1 as(SELECT tFinallSeans.naz AS kan_1,tFinallSeans.sut,tFinallSeans.sek  FROM tFinallSeans                                                               \
                WHERE tFinallSeans.kan = 1),                                                                                                                        \
          k2 as(SELECT tFinallSeans.naz AS kan_2,tFinallSeans.sut,tFinallSeans.sek  FROM tFinallSeans                                                               \
                WHERE tFinallSeans.kan = 2),                                                                                                                        \
          tableT as (SELECT *, (SELECT make_date(plppsi.godn,1,1) + plppsi.sutn-1) AS date_plan,                                                                      \
                    (SELECT make_time((plppsi.sekn / 3600)::int, (plppsi.sekn / 60 % 60)::int, (plppsi.sekn / 3600 % 60)::int)) FROM plppsi                         \                                         \
            LEFT JOIN k1 ON (k1.sut * 100000 + k1.sek > plppsi.sutn * 100000 + plppsi.sekn - 60 AND                                                                 \
                k1.sut * 100000 + k1.sek < plppsi.sutn * 100000 + plppsi.sekn + 60)),                                                                               \
          tableF as (SELECT * FROM tableT                                                                                                                           \
            LEFT JOIN k2 ON (k2.sut * 100000 + k2.sek > tableT.sutn * 100000 + tableT.sekn - 60 AND                                                                 \
                k2.sut * 100000 + k2.sek < tableT.sutn * 100000 + tableT.sekn + 60)),                                                                               \
           tab as (SELECT tableF.nz,tableF.npn,tableF.nka,tableF.godn, tableF.date_plan,SUBSTRING(to_char(tableF.date_plan,'dd-mm-yyyy') from 4 for 2)as month_, tableF.make_time, tableF.kan_1,tableF.kan_2, \
            (SELECT CASE WHEN tableF.kan_1 != '' and tableF.kan_2 != '' THEN 'full' ELSE '' END) as res FROM tableF)                                                                        \
        ";


    ObrIzm *coI;

    void    forDay();
    void    forMonth();
    void    forYear();
    void    forAll();
    void    setQueryBetween();
public slots:
    void    setQuerySet();
    void    setNka(QString nka);
    void    setNpn(QString npn);
private:
    QStringList list_npn;
    QButtonGroup group;
    QString queryNpn;
    QString queryNka;
    QString queryBetween;
    Ui::FPPsi *ui;
    QString queryStr;
    QSqlQueryModel *model;
    QModelIndex index;
    QSqlQuery query;
    void showEvent(QShowEvent *event);
};

#endif // FPPSI_H
