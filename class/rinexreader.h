#ifndef RINEXREADER_H
#define RINEXREADER_H
#include <QCoreApplication>
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include "jpsreader.h"
#include "formnumlib.h"
#include "rinexreader.h"

class rinexReader
{
public:
    QString version;
    QString MrkName;
    QString MrkNmb;
    QString Agency;
    QString RecNumb;
    QString TypeName;
    QString VersName;
    QString AntNumb;
    QString TypeName2;
    QString Xstr;
    QString Ystr;
    QString Zstr;
    qreal X, Y, Z, H, E, N;
    QString Hstr;
    QString Estr;
    QString Nstr;
    QString Interval;
    QString NofTypes;
    int NofTypesInt;
    QString NofTypesR;
    QString NofTypesG;
    QString TypesData;
    QString FirstObsData;
    QString LastObsData;
    QString NmbOfGLONASS;
    QString LitOfGLONASSBuff;
    QString BuffStr;
    QString NofRsat;

    //Момент начала измерений
    QDateTime *timeOfFirst = nullptr;
    //Момент окончания измерений
    QDateTime *timeOfLast = nullptr;

    QString YearF, MonthF, DayF, HourF, MinuteF, SecondF;
    QString YearL, MonthL, DayL, HourL, MinuteL, SecondL;

    quint16 NumbInOrderTypepsObs[15];
    bool TypesOfObsCheck[15];
    quint16 NumbInOrderTypepsObsGPS[36];
    bool TypesOfObsCheckGPS[36];
    quint16 NumbInOrderTypepsObsGLO[17];
    bool TypesOfObsCheckGLO[17];

    //Переменные для считывания шапки навигационного файла (.N/.G)
    QString versionNav;
    QString versionNavG;
    bool noNavigationFile;
    bool noNavigationFileG;
    bool noObservationFile;
    QString a0_TauC,a0,a1;
    QString ion_a0,ion_a1,ion_a2,ion_a3;
    QString ion_b0,ion_b1,ion_b2,ion_b3;
    QString W;
    QString T;
    qreal a0_TauCr,a0R,a1R;
    qreal ion_a0R,ion_a1R,ion_a2R,ion_a3R;
    qreal ion_b0R,ion_b1R,ion_b2R,ion_b3R;
    qreal Wr;
    qreal Tr;
    qreal LeapSecR;
    QString LeapSec;
    QString TimeSysCorr;
    QString TypeIonCorr;
    bool ionAOn=false,ionBOn=false,TgpsOn=false,TgloOn=false,leapSecOn=false;

    //Счётчики строк
    quint64 strNum;     //Номер текущей строки в потоке (.О)
    quint64 hedNum;     //Номер последней строки заголовка (.О)
    quint64 strNumNav;  //Номер текущей строки в потоке (.N)
    quint64 hedNumNav;  //Номер последней строки заголовка (.N)
    quint64 strNumNavG;  //Номер текущей строки в потоке (.G)
    quint64 hedNumNavG;  //Номер последней строки заголовка (.G)

    QFile *FILE;
    int VERS;
    bool slipVersion;

    rinexReader(QFile *Obs, QFile *Nav, QFile *NavG, int version);
    ~rinexReader() {
        if(FILE->exists())
            FILE->close();
        if(timeOfFirst != nullptr)
            delete timeOfFirst;
        if(timeOfLast != nullptr)
            delete timeOfLast;
    }
    void readRinexFileHeader(QFile *file);
    QStringList readRinexFileEpochGNSSData(QFile *file);
    QStringList readRinex302FileEpochGNSSData(QFile *file);
    QStringList readInputStream(QTextStream *stream);
    QStringList readInputStream302(QTextStream *stream);
    void RRFH210(QFile *file);
    void RRFH302(QFile *file);
    void RRFHnav(QFile *file);
    void RRFHnavG(QFile *file);
    QStringList readInputStreamNav210(QTextStream *stream);
    QStringList readInputStreamNavG210(QTextStream *stream);
    quint32 ConstLitGlo(int A);
    int lit [40];

    static int countTypeIzmInVersion2;  //Кол-во типов измерений в версии 2.Х
    static int *listTypesIzm;
};

#endif // RINEXREADER_H
