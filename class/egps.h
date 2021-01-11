#ifndef EPHEMERIDSGPS
#define EPHEMERIDSGPS

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include "jpsreader.h"
#include "rinexreader.h"

class EphemeridGPS//класс описывающий необходимые значения для описания эфемерид  GPS в файлах RINEX
{
    public:
    ~EphemeridGPS();
    qreal SVClockBias;
    qreal SVClockDrift;
    qreal SVClockBiasRate;
    qreal IODE;
    qreal Crs;
    qreal DeltaN;
    qreal M0;
    qreal Cuc;
    qreal Eccentricity;
    qreal Cus;
    qreal sqrtA;
    qreal ToE;
    qreal Cic;
    qreal Omega0;
    qreal Cis;
    qreal i0;
    qreal Crc;
    qreal Omega;
    qreal OmegaDot;
    qreal IDOT;
    qreal CodesL2;
    qreal L2PdataFlag;
    qreal SVaccuracy;
    qreal SVhealth;
    qreal TGD;
    qreal IODC;
    qreal TransTime;
    qreal FitInterval;

    qreal GPSWeek;              //неделя GPS
    qreal GPSSec;               //неделя GPS

    //Флаг указывающий на наличие или отсутствие входного момента времени в формате Г.М.Д.Ч.М.С
    //true при формированиее объекта с использованием файла в формате RINEX
    bool inputUTCtime;
    quint32 YearEphInput;
    quint32 MonthEphInput;
    quint32 DayEphInput;
    quint32 HourEphInput;
    quint32 MinEphInput;
    qreal SecEphInput;

    static bool inputIONcorr;
    static qreal alpha0;
    static qreal alpha1;
    static qreal alpha2;
    static qreal alpha3;
    static qreal beta0;
    static qreal beta1;
    static qreal beta2;
    static qreal beta3;
    static qreal A0;
    static qreal A1;
    static quint32 W;           //номер недели
    static quint32 T;           //time of week в секундах

    quint32 NumSatGPS;
    QDateTime *timeOfFirst = nullptr;     //Момент начала измерений  - необходимо для проверки недель GPS

    //Конструктор класса для конвертации из .jps
    EphemeridGPS(jpsReader *Reader, qint32 Num);
    //Конструктор класса для конвертации из RINEX
    EphemeridGPS(QStringList *AList, int Version);

    //запись в файл .G версии 211 и версии 302 эфемерид
    void WriteRNXEphG(QFile *RINEX);
    void WriteRNXEphG211(QFile *RINEX);

    // функция создания и записи в файл header Rinex GPS NAV DATA версии 2.11
    static void WriteRNX211HeaderG(QFile *RINEX);

    // функция создания и записи в файл header Rinex GPS NAV DATA версии 3.02
    static void WriteRNX302HeaderG(QFile *RINEX);

    // функция создания и записи в файл header Rinex GPS NAV DATA версии 2.11
    static void WriteRNX2HeaderG(QFile *RINEX, rinexReader *Reader);

    // функция создания и записи в файл header Rinex GPS NAV DATA версии 3.02
    static void WriteRNX3HeaderG(QFile *RINEX, rinexReader *Reader);

    static void setIonGpsArguments(jpsReader *Reader, int NumIonLog);
    static void setIonGpsArgumentsRNX(rinexReader *Reader,int NumIonLog);

private:
    //QList <EphemeridGPS*> ArraySatGPS;
    QStringList *alist = nullptr;
};

#endif // EPHEMERIDSGPS

