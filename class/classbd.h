#ifndef CLASSBD_H
#define CLASSBD_H

#include <QtWidgets>
#include <QtSql>
#include <QTableView>
#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include "rinexfile.h"

class ClassBd : public RinexFile
{

public:
    ClassBd();

  /*
    struct ZapEduch
    {
     quint64 nz;
     QString naz;
     QString npn;
     quint16 godn;
     quint16 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString tipis;
     QString nkis;
     QString prsv;
     QDate datn;
     QTime vrn;
     QDate datk;
     QTime vrk;
     quint32 kodobr;
     quint16 probr;
     qint16 strk;
    };
*/

    struct ZapEdxr
    {
     quint64 nz;
     QString isxf;
     QString arxf;
     quint64 idlf;
     qint16 iks;
     quint64 adlf;
     qint16 aks;
     QDate datp;
     QDate datr;
     quint16 prarx;
     quint64 nz_nos;
    };

    struct ZapSlu
    {
     QString path_temp;
     QString path_obm;
    };

  /*
    struct ZapEdvigr
    {
     quint64 nz;
     quint64 nz_edxr;
     QDate dtvigr;
    };
*/

    struct ZapNos
    {
     quint64 nz;
     QString naz;
     qint32 prblok;
    };

/*
    struct ZapEducho
    {
     quint64 nz;
     QString naz;
     QString npn;
     qint16 godn;
     qint16 godk;
     qint32 sutn;
     qint32 sutk;
     qint64 sekn;
     qint64 sekk;
     QString nka;
     QString tipis;
     QDate datn;
     QTime vrn;
     QDate datk;
     QTime vrk;
     qint32 kodobr;
     qint16 probr;
     qint16 strk;
     QString form;
     qint16 prsut;
     qint32 inter;
    };
*/

    struct ZapSxema
    {
     quint64 nz;
     quint64 nz_educh;
     quint64 nz_educho1;
     quint64 nz_educho2;
     qint32 pr_isx;
    };

    struct ZapStat313
    {
     quint64 nz;
     quint64 nz_educh;
     int kizm;
     int dlsek;
     int kizmn;
     int dlsekn;
     int shf1max;
     int shf1min;
     int shf1med;
     int shf2max;
     int shf2min;
     int shf2med;
     int kizmnf1;
     int kizmnf2;
    };

    struct ZapStatDDS
    {
     quint64 nz;
     QDate dat;
     qint32 eppl;
     qint32 eppol;
     qint32 ep_4mglo;
     qint32 ep_4rglo;
     qint32 ep_4bglo;
     quint64 nz_dds;
     qint32 mas_ep[24];
    };

    struct ZapStatrDDS
    {
     quint64 nz;
     quint64 nz_statdds;
     QTime vrn;
     QTime vrr;
     qint32 kolep;
    };

/*
    struct ZapStatPPSI
    {
     quint64 nz;
     QString naz;
     QDate dat;
     QTime vr;
     qint16 kan;
     QDateTime dtn_dds;
     QDateTime dtk_dds;
     qint32 k_dds;
     qint32 pk_dds;
     qint32 oks_dds;
     qint32 och_dds;
     qint32 obr_dds;
     QDateTime dtn_vrv;
     QDateTime dtk_vrv;
     qint32 k_vrv;
     qint32 obr_vrv;
     qint32 och_vrv;
    };
*/

    struct ZapDs
    {
     quint64 nz;
     QString naz;
     QString npn;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString nkis;
     QString dtn;
     QString dtk;
     qint32 sv;
     QString nazis;
     quint64 nz_edxr;
    };


    struct ZapFull
    {
     quint64 nz;
     QString naz;
     qint32 kan;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString dtn;
     QString dtk;
     qint32 strk;
     qint32 kodobr;
     quint64 nz_edxr;
     QString npn;
    };


    struct QueryReadEdxr
    {
     quint64 nz_edxr;
     QString isxf;
     QString arxf;
     quint64 idlf;
     qint32 iks;
     quint64 adlf;
     qint32 aks;
     qint16 prarx;
     QString path_arx;
    };

    struct ZapObr
    {
     QString naz_komp;
     quint64 nz_edxr;
    };

    struct ZapDDS
    {
     quint64 nz;
     QString naz;
     QString tip;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString dtn;
     QString dtk;
     qint32 strk;
     qint32 kodobr;
     quint64 nz_edxr;
    };

    struct ZapUst
    {
     quint64 nz;
     QString nazkomp;
     QString path_temp;
     QString path_si;
     bool pravt;
     qint32 km;
     bool pr_obrsi;
     bool pr_obr2;
     bool pr_obr3;
     bool pr_obr4;
     bool pr_obr7;
     bool pr_obr8;
     bool pr_obr9;
     bool pr_obr10;
     bool pr_obr11_15;
     bool pr_obr12;
     bool pr_obr14;
     bool pr_obr13;
     bool pr_obr17;
     bool pr_obr16;
     bool pr_obr19;
     bool pr_obr18;
     qint32 kol_obrsi;
     qint32 kol_obr2;
     qint32 kol_obr3;
     qint32 kol_obr4;
     qint32 kol_obr7;
     qint32 kol_obr8;
     qint32 kol_obr9;
     qint32 kol_obr10;
     qint32 kol_obr11_15;
     qint32 kol_obr12;
     qint32 kol_obr14;
     qint32 kol_obr13;
     qint32 kol_obr17;
     qint32 kol_obr16;
     qint32 kol_obr19;
     qint32 kol_obr18;
     qint32 pr_obrzapr;
     QString path_err;
    };

    struct ZapStatVRV
    {
     quint64 nz;
     quint64 nz_dds;
     QDate dat;
     qint32 plan;
     qint32 vip;
     qint32 vne;
     qint32 nevip;
     qint32 noplan;
    };

    typedef struct
    {
     quint64 k;//количество кадров ППСИ
     quint64 kbm;//количество кадров ППСИ без маркера синхронизации
     quint64 crc;//количество кадров ППСИ с ошибками crc
     quint64 pk;//количество пустых кадров ППСИ
     quint64 vrv1;//количество кадров ППСИ с первым кадром ВРВ
     quint64 vrv2;//количество кадров ППСИ со вторым кадром ВРВ
     quint64 kvi;//количество кадров ППСИ с ДДС КВИ
     quint64 kvno;//количество кадров ППСИ с ДДС КВНО
     quint64 rez;//количество резервных кадров ППСИ
     quint64 opk;//количество пустых кадров ППСИ с ошибками crc
     quint64 ovrv1;//количество ошибочных по crc кадров ППСИ с первым кадром ВРВ
     quint64 ovrv2;//количество ошибочных по crc кадров ППСИ со вторым кадром ВРВ
     quint64 okvi;//количество ошибочных по crc кадров ППСИ с ДДС КВИ
     quint64 okvno;//количество ошибочных по crc кадров ППСИ с ДДС КВНО
     quint64 orez;//количество резервных кадров ППСИ с ошибками crc
     quint64 izm_kvi;//количество кадров ДДС КВИ без ошибок crc
     quint64 k_kvi;//общее количество кадров ДДС КВИ
     quint64 crc_kvi;//количество кадров ДДС КВИ с ошибкой crc
     quint64 och_kvi;//количество отбракованных кадров ДДС КВИ (пустые и ошибки)
     quint64 k_kvno;//общее количество кадров ДДС КВНО
     quint64 crc_kvno;//количество кадров ДДС КВНО с ошибкой crc
     quint64 och_kvno;//количество кадров ДДС КВНО с прочими ошибками
     quint64 k1_vrv1;//количество первых кадров ВРВ по первому каналу
     quint64 k2_vrv1;//количество вторых кадров ВРВ по первому каналу
     quint64 k1_vrv2;//количество первых кадров ВРВ по второму каналу
     quint64 k2_vrv2;//количество вторых кадров ВРВ по второму каналу
     quint64 crc1_vrv1;//количество первых кадров ВРВ с ошибками crc по первому каналу
     quint64 crc2_vrv1;//количество вторых кадров ВРВ с ошибками crc по первому каналу
     quint64 crc1_vrv2;//количество первых кадров ВРВ с ошибками crc по второму каналу
     quint64 crc2_vrv2;//количество вторых кадров ВРВ с ошибками crc по второму каналу
     quint64 och1_vrv1;//количество первых кадров ВРВ с прочими ошибками по первому каналу
     quint64 och2_vrv1;//количество вторых кадров ВРВ с прочими ошибками по первому каналу
     quint64 och1_vrv2;//количество первых кадров ВРВ с прочими ошибками по второму каналу
     quint64 och2_vrv2;//количество вторых кадров ВРВ с прочими ошибками по второму каналу
     quint64 nz_fullppsi;//№ записи в таблице сеансов ППСИ для первого канала
     quint64 k_vrv;//количество полных сформированных кадров ALTI ВРВ
     quint64 k_cal;//количество полных сформированных кадров CAL1,CAL2 ВРВ
     QDate dn_kvi;
     QDate dk_kvi;
     QDate dn_kvno;
     QDate dk_kvno;
     QDate dn_vrv;
     QDate dk_vrv;
    } StrStatFull;

    struct ZapRnx
    {
     quint64 nz;
     QString naz;
     QString ver;
     QString npn;
     QString tip;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString dtn;
     QString dtk;
     qint32 strk;
     qint32 kodobr;
     qint32 interv;
     quint64 nz_edxr;
    };

    struct ZapStat322
    {
     quint64 nz;
     quint64 nz_educh;
     int kizm;
     int dlsek;
     int kizmn;
     int dlsekn;
     int shrcmax;
     int shrcmin;
     int shrcmed;
     int shpsmax;
     int shpsmin;
     int shpsmed;
    };

    struct ZapSist
    {
     quint64 nz;
     QDate dt;
     QTime vr;
     quint64 nz_kodobr;
     QString sost;
     QString naz_komp;
     QString polz;
     QString naz_ed;
    };

    struct ZapZapr
    {
     quint64 nz;
     quint64 nz_potr;
     bool pr_avt;
     QDateTime dat;
     bool ds;
     bool dzs;
     bool kos;
     bool meteo;
     bool ppsi;
     bool kvi;
     bool kvno;
     bool alti;
     bool cal;
     bool bds_l;
     bool bds_o;
     bool bds_n;
     QString naz;
     bool rnx2;
     QString path;
     QDate dtn;
     QDate dtk;
     QDate dtt;
     bool pr_alti;
     qint32 kolsut;
     qint32 pr_dat;
     QString komp;
     bool sp3;
     bool sol;
    };

    struct ZapPotr
    {
     quint64 nz;
     QString naz;
    };

    struct ZapEdvid
    {
     quint64 nz;
     quint64 nz_zapr;
     quint64 nz_edxr;
     QString naz_edxr;
     QDateTime dvid;
     QString tip;
//     qint32 preobr;
    };

    struct ZapAlt
    {
     quint64 nz;
     QDate datn;
     QTime vrn;
     qint64 prod;
     qint32 reg;
     qint64 nvit;
     QDate datk;
     QTime vrk;
     QString nfile;
     QString nka;
    };

    struct ZapStatrvrv
    {
     quint64 nz;
     quint64 nz_statvrv;
     QTime tm;
     qint32 kolpak;
     qint32 reg;
    };

    struct ZapServ
    {
     quint64 nz;
     QString isdir;
     QString vixdir;
     QString copdir;
     QString opis;
     QString maska;
     quint8 kobr;
     qint32 karx;
     QString nazkomp;
     qint32 prvd;
     qint32 prcd;
     QString maskvd;
     QString maskcd;
     qint32 sdir;
     qint32 sdirc;
    };

    struct ZapStatServ
    {
     quint64 nz;
     QString ifile;
     QDate dat;
     QTime vr;
     quint64 nz_serv;
    };

    struct ZapSba
    {
     quint64 nz;
     quint64 nz_kodba;
     QDate dvik;
     QTime vvik;
     QDate dvkl;
     QTime vvkl;
     qint32 nka;
    };

    struct ZapPlppsi
    {
     quint64 nz;
     QString naz;
     quint16 godn;
     quint32 sutn;
     qint32 sekn;
     qint32 nka;
     QString npn;
     QString tip;
    };

    struct ZapSplppsi
    {
     quint64 nz;
     qint64 sek;
     qreal az;
     qreal um;
     quint64 nz_plppsi;
    };

    struct ZapPla
    {
     quint64 nz;
     QString naz;
     QString nka;
     QString npn;
     QDate dat;
     QTime vrem;
     qint32 vrs;
     qint32 ds;
     qint32 dzs;
     qint32 kos;
     qint32 ppsi;
     qint32 vit;
     qreal az;
     qreal um;
    };

    struct ZapCogi
    {
     quint64 nz;
     QString naz;
     qint32 tip;
     qint32 sost;
     QDate dat;
     QTime vrem;
     quint64 nz_edxr;
     quint64 nz_cup;
    };

    struct ZapCup
    {
     quint64 nz;
     QString naz;
     qint32 tip;
     QDate dat;
     QTime vrem;
     quint64 nz_edxr;
     qint32 sost;
    };

    struct ZapUstnf
    {
     quint64 nz;
     QString komp;
     QString isx;
     QString vxod;
     qint32 zvon;
    };

    struct ZapSp3
    {
     quint64 nz;
     QString naz;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString dtn;
     QString dtk;
     quint64 nz_edxr;
     qint32 kolep;
    };

    struct ZapSol
    {
     quint64 nz;
     QString naz;
     quint32 godn;
     quint32 godk;
     quint32 sutn;
     quint32 sutk;
     quint64 sekn;
     quint64 sekk;
     QString nka;
     QString dtn;
     QString dtk;
     quint64 nz_edxr;
     qint32 kolizm;
     qint32 kolk;
     qint32 kolo;
     qreal sko;
    };

    struct ZapLeap
    {
     quint32 nz;
     QDate dat1;
     QDate dat2;
     qint32 sec;
    };

    //подключение к БД PSQL (PostgresSQL)
    //namedb - имя БД
    //name - логин
    //password - пароль
    //host - имя хоста
    //str - строка сообщения об ошибке
    //при удачном подключении возвращает true
    //при неудачном подключении возвращает false, в строке сообщения об ошибке формируется текст сообщения
    bool createConnectionPSQL(QString namedb,QString name, QString password,QString host,QSqlError &str);

    //настройка модели представления и отображения таблицы
    //model - указатель на модель
    //tableView - указатель на виджет отображения
    //naz_table - название таблицы
    //ClassBd::kol_htable - количество отображаемых столбцов
    //ClassBd::hidcol[100] - признак отображения столбца в виджете (true/false)
    //ClassBd::htable[100] - массив заголовков столбцов
    bool setModel(QSqlTableModel *model,QTableView *tableView,QString naz_table);
    bool setRelModel(QSqlRelationalTableModel *model,QTableView *tableView,QString naz_table);
    bool setQueryModel(QSqlQueryModel *model, QTableView *tableView, QString query, bool prsoob);

    //возвращает индекс в модели model
    //для записи в таблице с № id
    QModelIndex IndModel(qlonglong id,QSqlTableModel *model);
    QModelIndex IndModel(qlonglong id,QSqlQueryModel *model);

    //удаление записи в таблице
    //удаляется текущая запись в представлении tableView
    //из таблицы, связанной с моделью model
    bool DeleteRow(QSqlTableModel *model,QTableView *tableView);
    bool DeleteRow(QString table, qulonglong nz, bool prsoob, QSqlQuery query);
    bool DeleteRow(QString table, qulonglong nz, bool prsoob);

    bool DeleteRow(QString table, QString naz_col, qlonglong zn, bool prsoob, QSqlQuery query);
    bool DeleteRow(QString table, QString naz_col, qlonglong zn, bool prsoob);

    bool DeleteRow(QString table, QString naz_col, QString zn, bool prsoob, QSqlQuery query);
    bool DeleteRow(QString table, QString naz_col, QString zn, bool prsoob);

    //вставка записи в таблицу
    //в strsql передаётся SQL-команда на выполнение
    //при неудачном выполнении выводится сообщение и возвращается 0
    //при успешном выполнении возвращается №записи в таблице
    //если prsoob=true в случае ошибки, выдаётся сообщение
    qulonglong InsertRow(QString strsql, bool prsoob, QSqlQuery query);
    qulonglong InsertRow(QString strsql, bool prsoob);

    //поиск в таблице
    //naz_table - название таблицы
    //naz_col - название столбца, по которому ведётся поиск
    //nz,sod - значение в столбце, которое необходимо найти
    //при ошибке выдаёт сообщение и возвращает -1
    //при успешном выполнении возвращает № записи в таблице (0 поле)
    //если запись не найдена возвращает 0
    //если запись найдена, читает содержимое полей в массив spis_field
    //и выставляет число полей в kol_spis_file
    qlonglong FindRow(QString naz_table, QString naz_col, qlonglong sod, bool prsoob, QSqlQuery query);
    qlonglong FindRow(QString naz_table, QString naz_col, qlonglong sod, bool prsoob);

    qlonglong FindRow(QString naz_table, QString naz_col, QString sod, bool prsoob, QSqlQuery query);
    qlonglong FindRow(QString naz_table, QString naz_col, QString sod, bool prsoob);

    qlonglong FindRowFull(QString dtn, QString dtk, QString nka, qint32 kan, bool prsoob, QSqlQuery query);
    qlonglong FindRowFull(QString dtn, QString dtk, QString nka, qint32 kan, bool prsoob);

    qlonglong FindRowFull(qint32 godn, qint32 sutn, qint64 sekn, QString nka, qint32 kan, qint32 kodobr, bool prsoob, QSqlQuery query);
    qlonglong FindRowFull(qint32 godn, qint32 sutn, qint64 sekn, QString nka, qint32 kan, qint32 kodobr, bool prsoob);

    qlonglong FindRowFull(qint32 godn, qint32 sutn, qint64 sekn, QString nka, qint32 kan, bool prsoob, QSqlQuery query);
    qlonglong FindRowFull(qint32 godn, qint32 sutn, qint64 sekn, QString nka, qint32 kan, bool prsoob);

    QList<qlonglong> FindRowForObDDS(quint64 nz_dds, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForObDDS(quint64 nz_dds, bool prsoob);

    QList<qlonglong> FindRowForObKVNO(quint64 nz_dds, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForObKVNO(quint64 nz_dds, bool prsoob);

    QList<qlonglong> FindRowForObAlti(quint64 nz_dds, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForObAlti(quint64 nz_dds, bool prsoob);

    QList<qlonglong> FindRowForEdvidDs(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvidDs(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvidDds(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvidDds(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvidPpsi(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvidPpsi(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvidRnxi(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvidRnxi(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvidRnxv(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvidRnxv(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvid(quint64 nz_zapr, bool prsoob, QSqlQuery query);
    QList<qlonglong> FindRowForEdvid(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowPpsiForDat(QDate dat, qint32 kod, bool prsoob, QSqlQuery query, QString nka);
    QList<qlonglong> FindRowPpsiForDat(QDate dat, qint32 kod, bool prsoob, QString nka);

    QList<qlonglong> FindRowForOtprNf(bool prsoob,QSqlQuery query);
    QList<qlonglong> FindRowForOtprNf(bool prsoob);

    QList<qlonglong> FindRowAvtZapr(bool prsoob, QSqlQuery query, QString komp);
    QList<qlonglong> FindRowAvtZapr(bool prsoob, QString komp);

    qlonglong FindRowRnxi(QString naz, QString ver, QString npn, QString tip, qint32 godn, qint32 sutn, qint32 sekn, bool prsoob, QSqlQuery query);
    qlonglong FindRowRnxi(QString naz,QString ver,QString npn,QString tip,qint32 godn,qint32 sutn,qint32 sekn,bool prsoob);

    qlonglong FindRowDs(QString naz, QString npn, qint32 godn, qint32 sutn, qint32 sekn, QString nazis, bool prsoob, QSqlQuery query);
    qlonglong FindRowDs(QString naz, QString npn, qint32 godn, qint32 sutn, qint32 sekn, QString nazis, bool prsoob);

    qlonglong FindRowFullppsi(QString naz,qint32 kan,qint32 godn,qint32 sutn,qint32 sekn,QString nka,QString npn,bool prsoob,QSqlQuery query);
    qlonglong FindRowFullppsi(QString naz,qint32 kan,qint32 godn,qint32 sutn,qint32 sekn,QString nka,QString npn,bool prsoob);

    quint64 FindRowSp3(QString naz,bool prsoob);

    quint64 FindRowSol(QString naz,bool prsoob);


    quint64 FindSp3ForDds(quint64 nz_dds,bool prsoob);

    //поиск и чтение записи плана АСПСИ (таблица plppsi)
    //при неудачном поиске возвращает false
    //при удачном - true и в sn,sk секунды начала и конца сеанса по плану,tn,tk время начала и конца сеанса по плану
    bool FindPlanForFull(quint64 nz_full, qint32 &sn, qint32 &sk, QTime &tn, QTime &tk, bool prsoob);
    //поиск и чтение записи плана АСПСИ (таблица plppsi)
    //при неудачном поиске возвращает false
    //при удачном - true и в sn,sk секунды начала и конца сеанса по плану,tn,tk время начала и конца сеанса по плану
    bool FindPlanForFull(qint32 godn,qint32 sutn,qint32 sekn,QString nka,qint32 &sn,qint32 &sk,QTime &tn,QTime &tk,bool prsoob);

    //возвращает индекс в таблице, связанной с моделью model
    //для текущей строки в представлении table
    //в id возвращается значение ячейки с № col
    QModelIndex getCurrentIndex(QTableView *table,QSqlTableModel *model,int col,qlonglong &id);
    QModelIndex getCurrentIndex(QTableView *table,QSqlQueryModel *model,int col,qlonglong &id);
    QModelIndex getCurrentIndex(QTableView *table,QSqlQueryModel *model,int col,QString &id);

    //перезапись данных в таблице
    //naz_table - название таблицы
    //naz_col - название столбца, где нужно перезаписать данные
    //zn - новое значение, которое необходимо перезаписать
    //nz - № строки в которой перезаписывваются данные
    bool UpdateRow(QString naz_table, QString naz_col, qlonglong zn, qlonglong nz, bool prsoob, QSqlQuery query);
    bool UpdateRow(QString naz_table, QString naz_col, qlonglong zn, qlonglong nz, bool prsoob);

    bool UpdateRows(QString naz_table, QString naz_col, qlonglong new_zn, QString otb_col, qlonglong otb_zn, bool prsoob, qint32 pr_isx, QSqlQuery query);
    bool UpdateRows(QString naz_table, QString naz_col, qlonglong new_zn, QString otb_col, qlonglong otb_zn, bool prsoob, qint32 pr_isx);

    bool UpdateRowStatFull(quint64 p1, quint64 p2, quint64 p3, QString naz_p1, QString naz_p2, QString naz_p3, quint64 nz_fullppsi, bool prsoob, QSqlQuery query);
    bool UpdateRowStatFull(quint64 p1, quint64 p2, quint64 p3, QString naz_p1, QString naz_p2, QString naz_p3, quint64 nz_fullppsi, bool prsoob);

    bool UpdateRowStatFullVRV(quint64 k11, quint64 k12, quint64 k21, quint64 k22, quint64 crc11, quint64 crc12, quint64 crc21, quint64 crc22,
                              quint64 och11, quint64 och12, quint64 och21, quint64 och22, quint64 k, quint64 nz_fullppsi, bool prsoob, QSqlQuery query);
    bool UpdateRowStatFullVRV(quint64 k11, quint64 k12, quint64 k21, quint64 k22, quint64 crc11, quint64 crc12, quint64 crc21, quint64 crc22,
                              quint64 och11, quint64 och12, quint64 och21, quint64 och22, quint64 k, quint64 nz_fullppsi, bool prsoob);


    bool UpdateRowStatFull(quint64 zn, QString naz, quint64 nz_fullppsi, bool prsoob, QSqlQuery query);
    bool UpdateRowStatFull(quint64 zn, QString naz, quint64 nz_fullppsi, bool prsoob);


    bool UpdateErr(QString naz_table, qint32 zn_strk, qint32 zn_kodobr, qlonglong nz, bool prsoob, QSqlQuery query);
    bool UpdateErr(QString naz_table, qint32 zn_strk, qint32 zn_kodobr, qlonglong nz, bool prsoob);

    bool UpdateZapr(bool prsoob, QSqlQuery query);
    bool UpdateZapr(bool prsoob);

    bool UpdateRow(QString naz_table, QString naz_col, QString zn, qlonglong nz, bool prsoob, QSqlQuery query);
    bool UpdateRow(QString naz_table, QString naz_col, QString zn, qlonglong nz, bool prsoob);

    bool UpdateFull(bool prsoob, QSqlQuery query);
    bool UpdateFull(bool prsoob);

    bool UpdateRowStatFull(QDate p1, QDate p2, QString naz_p1, QString naz_p2, quint64 nz_fullppsi, bool prsoob, QSqlQuery query);
    bool UpdateRowStatFull(QDate p1, QDate p2, QString naz_p1, QString naz_p2, quint64 nz_fullppsi, bool prsoob);

    bool UpdateUst(bool prsoob, QSqlQuery query);
    bool UpdateUst(bool prsoob);

    bool UpdateServ(bool prsoob, QSqlQuery query);
    bool UpdateServ(bool prsoob);

    bool UpdateSba(bool prsoob, QSqlQuery query);
    bool UpdateSba(bool prsoob);

    bool UpdateNos(bool prsoob);

    bool UpdateUstnf(bool prsoob,QSqlQuery query);
    bool UpdateUstnf(bool prsoob);

    //возвращает список названий таблиц в текущей БД
    QStringList SpisTable();

    //поиск формата для расширения файла ext
    //при ошибке возвращает false
    //возвращает в nz - №записи формата; naz - название формата
//    bool FindFormForExt(QString ext,qlonglong &nz,QString &naz);

    //возвращает значение поля в запросе с индексом nom
    //количество полей выставляется в kol_spis_field
    //если записи нет в kol_spis_field=0
    qlonglong GetField(QSqlQuery query,int nom);

    //читает в массив spis_field содержимое полей
    void GetFieldAll(QSqlQuery query);

    //добавляет запись в таблицу educh
    //file - название файла измерений с полным путём
    //npn .....nka - значения полей таблицы в строковом виде
    //возвращает номер новой записи, или 0 при неудачном завершении
//    qulonglong InsertEduch(bool prsoob);
    qulonglong InsertEdxr(bool prsoob, QSqlQuery query);
    qulonglong InsertEdxr(bool prsoob);

//    qulonglong InsertEdvigr(bool prsoob);
//    qulonglong InsertEducho(bool prsoob);
    qulonglong InsertSxema(QString ntable, bool prsoob, QSqlQuery query);
    qulonglong InsertSxema(QString ntable, bool prsoob);

    qulonglong InsertStat313(bool prsoob, QSqlQuery query);
    qulonglong InsertStat313(bool prsoob);

    qulonglong InsertStatDDS(bool prsoob, QSqlQuery query);
    qulonglong InsertStatDDS(bool prsoob);

    qulonglong InsertStatrDDS(bool prsoob, QSqlQuery query);
    qulonglong InsertStatrDDS(bool prsoob);

//    qulonglong InsertStatPPSI(bool prsoob);
    quint64 InsertDs(bool prsoob, QSqlQuery query);
    quint64 InsertDs(bool prsoob);

    quint64 InsertFull(bool prsoob, QSqlQuery query);
    quint64 InsertFull(bool prsoob);

    bool InsertObr(QString nkomp, quint64 nz_edxr, QSqlQuery query);
    bool InsertObr(QString nkomp, quint64 nz_edxr);

    quint64 InsertDDS(bool prsoob, QSqlQuery query);
    quint64 InsertDDS(bool prsoob);

    quint64 InsertStatFull(bool prsoob, QSqlQuery query);
    quint64 InsertStatFull(bool prsoob);

    quint64 InsertUst(bool prsoob);
    quint64 InsertRnx(QString ntable, bool prsoob, QSqlQuery query);
    quint64 InsertRnx(QString ntable, bool prsoob);

    quint64 InsertStatVRV(bool prsoob, QSqlQuery query);
    quint64 InsertStatVRV(bool prsoob);

    qulonglong InsertStat322(bool prsoob, QSqlQuery query);
    qulonglong InsertStat322(bool prsoob);

//    quint64 InsertSist(bool prsoob);
    quint64 InsertZapr(bool prsoob);

    quint64 InsertPotr(bool prsoob, quint64 nz);

    quint64 InsertEdvid(bool prsoob);

    quint64 InsertAlt(bool prsoob, QSqlQuery query);
    quint64 InsertAlt(bool prsoob);

    quint64 InsertStatrvrv(bool prsoob, QSqlQuery query);
    quint64 InsertStatrvrv(bool prsoob);

    quint64 InsertServ(bool prsoob,QSqlQuery query);
    quint64 InsertServ(bool prsoob);

    quint64 InsertStatServ(bool prsoob,QSqlQuery query);
    quint64 InsertStatServ(bool prsoob);

    quint64 InsertSba(bool prsoob,QSqlQuery query);
    quint64 InsertSba(bool prsoob);

    quint64 InsertSist(bool prsoob,QSqlQuery query);
    quint64 InsertSist(bool prsoob);

    quint64 InsertNos(bool prsoob);

    quint64 InsertPPSI(bool prsoob, QSqlQuery query);
    quint64 InsertPPSI(bool prsoob);

    quint64 InsertSPLPPSI(bool prsoob, QSqlQuery query);
    quint64 InsertSPLPPSI(bool prsoob);

    quint64 InsertPla(bool prsoob, QSqlQuery query);
    quint64 InsertPla(bool prsoob);

    quint64 InsertCogi(bool prsoob, QSqlQuery query);
    quint64 InsertCogi(bool prsoob);

    quint64 InsertCup(bool prsoob, QSqlQuery query);
    quint64 InsertCup(bool prsoob);

    quint64 InsertUstnf(bool prsoob, QSqlQuery query);
    quint64 InsertUstnf(bool prsoob);

    quint64 InsertSP3(bool prsoob);

    quint64 InsertSol(bool prsoob);

    quint64 InsertLeap(bool prsoob);


    //запускает на выполнение запрос query, содержимое запроса передаётся в strsql
    //при ошибке выдаёт сообщение и возвращает -1
    //при успешном выполнении возвращает № записи в таблице (0 поле)
    //если запись не найдена возвращает 0
    //если запись найдена, читает содержимое полей  первой записи в массив spis_field
    //и выставляет число полей в kol_spis_file
    qlonglong ExecQuery(QSqlQuery query, QString strsql, bool prsoob);

    //выполняет запрос и заполняет список №записи найденных записей
    QList<qlonglong> ExecQueryList(QSqlQuery query, QString strsql, bool prsoob);

    //производит настройку модели
    //model - указатель на модель
    //naz_table - название таблицы
    //ClassBd::kol_htable - количество отображаемых столбцов
    //ClassBd::htable[100] - массив заголовков столбцов
    void UstModel(QSqlTableModel *model,QString naz_table);
    void UstRelModel(QSqlRelationalTableModel *model,QString naz_table);
    void UstQueryModel(QSqlQueryModel *model, QString query, bool prsoob);

    //настройка представления
    //model - указатель на модель
    //tableView - указатель на виджет отображения
    //ClassBd::hidcol[100] - признак отображения столбца в виджете (true/false)
    void UstView(QSqlTableModel *model,QTableView *tableView);
    void UstRelView(QSqlRelationalTableModel *model,QTableView *tableView);
    void UstQueryView(QSqlQueryModel *model,QTableView *tableView);

    //активирование модели
    //model - указатель на модель
    //до активирования, необходимо настроить модель
    //при ошибке выводит окно ошибки и возвращает false
    bool SelectModel(QSqlTableModel *model);
    bool SelectRelModel(QSqlRelationalTableModel *model);
    bool SelectQueryModel(QSqlQueryModel *model, QString query, bool prsoob);

    //определение № носителя для помещения
    //в случае успешного выполнения заполняет переменные класса
    //tek_nz_nos и tek_path_arx
    bool GetNos(bool prsoob, QSqlQuery query);
    bool GetNos(bool prsoob);

    //возвращает список №записи (поле 0) для запроса query
    QList<qlonglong> GetIdRowAll(QSqlQuery query);
    QStringList GetRowAll(QSqlQuery query,qint32 nkol);

    //возвращает список ID найденных записей
    //по запросу в таблице ntab для полей kodobr и probr
    QList<qlonglong> GetKodobr(QString ntab, qint32 kodobr, qint16 probr, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetKodobr(QString ntab, qint32 kodobr, qint16 probr, bool prsoob);

    QList<qlonglong> GetKodobr(QString ntab, qint32 kodobr, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetKodobr(QString ntab, qint32 kodobr, bool prsoob);

    QList<qlonglong> GetKodobrKan(QString ntab, qint32 kodobr, qint32 kan, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetKodobrKan(QString ntab, qint32 kodobr, qint32 kan, bool prsoob);

    //возвращает список ключей единиц хранения в таблице educho
    //в списке содержатся единицы хранения для объединения с исходным (ключ - nz)
    //имеющие одинаковые поля с передаваемыми в параметрах
    //исходная единица хранения в список не заносится
//    QList<qlonglong> GetRawForOb(QString npn, qint16 godn, qint32 sutn, quint64 nz, QString tipis, qint32 inter, bool prsoob);
//    QList<qlonglong> GetRawForOb(qint16 godn, qint32 sutn, quint64 nz, QString tipis, bool prsoob);

    QList<qlonglong> GetRawForFull(qint16 kan, qint32 godn, qint32 sutn, quint64 sekn, QString nka, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetRawForFull(qint16 kan, qint32 godn, qint32 sutn, quint64 sekn, QString nka, bool prsoob);

    QList<qlonglong> GetRawForO(QString npn, qint16 godn, qint32 sutn, quint64 nz, qint32 inter, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetRawForO(QString npn, qint16 godn, qint32 sutn, quint64 nz, qint32 inter, bool prsoob);

    QList<qlonglong> GetRawForN(qint16 godn, qint32 sutn, quint64 nz, QString tip, bool prsoob, QSqlQuery query);
    QList<qlonglong> GetRawForN(qint16 godn, qint32 sutn, quint64 nz, QString tip, bool prsoob);

    //читает содержимое строки таблицы в структуры
    bool ReadEdxr(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadEdxr(qlonglong nz, bool prsoob);

    bool ReadEdxr(QString ntab, quint64 nz_tab, bool prsoob, QSqlQuery query);
    bool ReadEdxr(QString ntab, quint64 nz_tab, bool prsoob);

//    bool ReadEduch(qlonglong nz, bool prsoob);
    bool ReadNos(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadNos(qlonglong nz, bool prsoob);

//    bool ReadEducho(qlonglong nz);
    bool ReadSxema(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadSxema(qlonglong nz, bool prsoob);

//    bool ReadEdxrForEduch(qint8 kod,qlonglong nz);
    //чтение записи nz из fullppsi
    //заполнение структуры zapfull
    bool ReadFullppsi(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadFullppsi(qlonglong nz, bool prsoob);

    /*!
     * \brief ClassBd::ReadDDS
     * чтение записи из таблицы dds и заполнение структуры ZapDDS
     * \param nz[in] ключ записи в таблице
     * \param prsoob[in] признак вывода сообщений об ошибках
     * \return возвращает false - если запись не найдена; иначе - true
    */
    bool ReadDDS(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadDDS(qlonglong nz, bool prsoob);

    bool ReadUst(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadUst(qlonglong nz, bool prsoob);

    bool ReadRnx(QString ntab, qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadRnx(QString ntab, qlonglong nz, bool prsoob);

    bool ReadStatFull(qlonglong nz_full, bool prsoob, QSqlQuery query);
    bool ReadStatFull(qlonglong nz_full, bool prsoob);

    bool ReadStatDDS(qlonglong nz_dds, bool prsoob, QSqlQuery query);
    bool ReadStatDDS(qlonglong nz_dds, bool prsoob);

    bool ReadZapr(qlonglong nz, bool prsoob);

    bool ReadPotr(qlonglong nz, bool prsoob);
    bool ReadEdvid(qlonglong nz, bool prsoob);

    bool ReadDs(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadDs(qlonglong nz, bool prsoob);

    bool ReadAlt(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadAlt(qlonglong nz, bool prsoob);

    bool ReadStatVrv(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadStatVrv(qlonglong nz, bool prsoob);

    bool ReadServ(qlonglong nz, bool prsoob, QSqlQuery query);
    bool ReadServ(qlonglong nz, bool prsoob);

    bool ReadSba(qlonglong nz,bool prsoob,QSqlQuery query);
    bool ReadSba(qlonglong nz,bool prsoob);

    bool ReadSist(qlonglong nz,bool prsoob,QSqlQuery query);
    bool ReadSist(qlonglong nz,bool prsoob);

    bool ReadUstnf(qlonglong nz,bool prsoob,QSqlQuery query);
    bool ReadUstnf(qlonglong nz,bool prsoob);

    bool ReadCogi(qlonglong nz,bool prsoob,QSqlQuery query);
    bool ReadCogi(qlonglong nz,bool prsoob);

    bool ReadCup(qlonglong nz,bool prsoob,QSqlQuery query);
    bool ReadCup(qlonglong nz,bool prsoob);

    bool ReadSP3(qlonglong nz,bool prsoob);

    bool ReadSol(qlonglong nz,bool prsoob);

    //очищает структуры записей таблиц
//    void ClearEduch();
    void ClearNos();
//    void ClearEducho();
//    void ClearEdvigr();
    void ClearSlu();
    void ClearEdxr();
    void ClearSxema();
    void ClearStat313();
    void ClearStatDDS();
    void ClearStatrDDS();
//    void ClearStatPPSI();
    void ClearDs();
    void ClearFullppsi();
    void ClearDDS();
    void ClearStatFull();
    void ClearUst();
    void ClearRnx();
    void ClearStatVRV();
    void ClearStat322();
//    void UstSist(QString komp,QString nfile,qint32 kodsist);
    void ClearZapr();
    void ClearEdvid();
    void ClearAlt();
    void ClearStatrvrv();
    void ClearServ();
    void ClearStatServ();
    void ClearSba();
    void ClearSist();
    void ClearPPSI();
    void ClearSPLPPSI();
    void ClearPla();
    void ClearCogi();
    void ClearCup();
    void ClearUstnf();
    void ClearSP3();
    void ClearSol();
    void ClearLeap();

//    bool DelEduch();
//    bool DelEducho();
    bool DelEdxr(bool prsoob, QSqlQuery query);
    bool DelEdxr(bool prsoob);

    bool DelSxema(bool prsoob, QSqlQuery query);
    bool DelSxema(bool prsoob);

    bool DelStat313(bool prsoob, QSqlQuery query);
    bool DelStat313(bool prsoob);

    bool DelStatDDS(bool prsoob, QSqlQuery query);
    bool DelStatDDS(bool prsoob);

//    bool DelStatPPSI(bool prsoob);
    bool DelDs(bool prsoob, QSqlQuery query);
    bool DelDs(bool prsoob);

    bool DelFull(bool prsoob, QSqlQuery query);
    bool DelFull(bool prsoob);

    bool DelDDS(bool prsoob, QSqlQuery query);
    bool DelDDS(bool prsoob);

    bool DelRnx(bool prsoob, QSqlQuery query);
    bool DelRnx(bool prsoob);

    bool DelSist(bool prsoob, QSqlQuery query);
    bool DelSist(bool prsoob);

    bool DelSxema1(bool prsoob, QSqlQuery query);
    bool DelSxema1(bool prsoob);

    bool DelPotr(bool prsoob);
    bool DelEdvid(quint64 nz_zapr, bool prsoob);
    bool DelTable(QString ntable, bool prsoob, QSqlQuery query);
    bool DelTable(QString ntable, bool prsoob);

    bool CreateBD();

    QStringList SpisNPN(QString table,QString nazis,bool prsoob);

    QStringList StatDDSInList(QDate dtn, QDate dtk, QString nka, bool prsoob);

    QList<qlonglong> FindRowForEdvidSp3(quint64 nz_zapr, bool prsoob,QSqlQuery query);
    QList<qlonglong> FindRowForEdvidSp3(quint64 nz_zapr, bool prsoob);

    QList<qlonglong> FindRowForEdvidSol(quint64 nz_zapr, bool prsoob,QSqlQuery query);
    QList<qlonglong> FindRowForEdvidSol(quint64 nz_zapr, bool prsoob);

    void Report(QTableView *table, QString zag, QString &path_prot);

    qint32 FindLeap(QDate dat);


    QString nikp; //логин подключения к БД
    QString passp; //пароль подключения к БД
    QString hostp;
    QSqlDatabase db; //ссылка на БД
    QString htable[100]; //массив заголовков
    bool hidcol[100];
    int kol_htable; //количество заголовков
    QVariant spis_field[100];
    int kol_spis_field;
    qulonglong tek_nz_nos;
    QString tek_path_arx;
//    ZapEduch zapeduch;
    ZapEdxr zapedxr;
    ZapSlu zapslu;
//    ZapEdvigr zapedvigr;
    ZapNos zapnos;
//    ZapEducho zapeducho;
    ZapSxema zapsxema;
    ZapStat313 zapstat313;
    ZapStatDDS zapstatdds;
    ZapStatrDDS zapstatrdds;
//    ZapStatPPSI zapstatppsi;
    ZapDs zapds;
    ZapFull zapfull;
    QueryReadEdxr readedxr;
    ZapObr zapobr;
    ZapDDS zapdds;
    StrStatFull statfull;
    ZapUst zapust;
    ZapRnx zaprnx;
    ZapStatVRV zapstatvrv;
    ZapStat322 zapstat322;
    ZapSist zapsist;
    ZapZapr zapzapr;
    ZapPotr zappotr;
    ZapEdvid zapedvid;
    ZapAlt zapalt;
    ZapStatrvrv zapstatrvrv;
    QString os;
    ZapServ zapserv;
    ZapStatServ zapstatserv;
    ZapSba zapsba;
    ZapPlppsi zapplppsi;
    ZapSplppsi zapsplppsi;
    ZapPla zappla;
    ZapCogi zapcogi;
    ZapCup zapcup;
    ZapUstnf zapustnf;
    ZapSp3 zapsp3;
    ZapSol zapsol;
    ZapLeap zapleap;
};

#endif // CLASSBD_H
