#ifndef JPSREADER_H
#define JPSREADER_H
#include "jpsreader.h"
#include <QCoreApplication>
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QString>

typedef unsigned char u1;
enum
{
bits = 8,
lShift = 2,
rShift = bits - lShift
};

class jpsReader
{
public:
    QList<QByteArray> ARD;
    QList<QByteArray> ATT;
    QList<QByteArray> AUO;
    QList<QByteArray> ATO;
    QList<QByteArray> ASI;
    QList<QByteArray> ANN;
    QList<QByteArray> ARC;
    QList<QByteArray> A1R;
    QList<QByteArray> A2R;
    QList<QByteArray> ACC;
    QList<QByteArray> AC1;
    QList<QByteArray> AC2;
    QList<QByteArray> ACP;
    QList<QByteArray> A1P;
    QList<QByteArray> A2P;
    QList<QByteArray> ADC;
    QList<QByteArray> AD1;
    QList<QByteArray> AD2;
    QList<QByteArray> AEC;
    QList<QByteArray> AE1;
    QList<QByteArray> AE2;
    QList<QByteArray> ANA;
    QList<QByteArray> AFC;
    QList<QByteArray> AF1;
    QList<QByteArray> AF2;
    QList<QByteArray> ANE;
    QList<QByteArray> AGE;
    QList<QByteArray> AIO;
    QList<QByteArray> APV;
    QList<QByteArray> APM; //Сроки с текстовой информацией
    QList<QByteArray> AMF;
    QList<QByteArray> AJP;

    static quint16 year;
    static quint32 numEp;

    jpsReader(QFile *file);
    void jpsReaderTextData(QFile *file);

    u1 cs(u1 src[], int count);

    //Параметры времени
    qreal getTimeGPS(int NumEpoch);
    quint32 getYear(int NumEpoch);
    quint32 getMonth(int NumEpoch);
    quint32 getDay(int NumEpoch);
    quint32 getHour(int NumEpoch);
    quint32 getMinute(int NumEpoch);
    qreal getSecond(int NumEpoch);
    //Номера спутников
    quint32 getNumSat(int NumEpoch);
    quint32 getNumSatGPS(int NumEpoch);
    quint32 getNumSatGLO(int NumEpoch);
    quint32 getCurrentSatNum (int NumEpoch, int Current);
    quint32 getCurrentSatGLO (int NumEpoch, int Current);
    //Наблюдения
    //Кодовые псевдодальности
    qreal getC1(int NumEpoch, int Current);
    qreal getC1Sec(int NumEpoch, int Current);
    qreal getP1(int NumEpoch, int Current);
    qreal getP2(int NumEpoch, int Current);
    //Фазовые псевдодальности
    qreal getL1(int NumEpoch, int Current);
    qreal getL1P(int NumEpoch, int Current);
    qreal getL2(int NumEpoch, int Current);
    qreal getL1GLO(int NumEpoch, int Current);
    qreal getL1PGLO(int NumEpoch, int Current);
    qreal getL2GLO(int NumEpoch, int Current);
    //Доплеровский сдвиг
    qreal getD1(int NumEpoch, int Current);
    qreal getD1P(int NumEpoch, int Current);
    qreal getD2(int NumEpoch, int Current);
    //Шум
    quint32 getEC (int NumEpoch, int Current);
    quint32 getE1 (int NumEpoch, int Current);
    quint32 getE2 (int NumEpoch, int Current);
    //Ошибка часов приёмника
    qreal getDeltaT(int NumEpoch);

    //Эфемериды ГЛОНАСС
    quint32 getSatNum(int Num);         //Номер спутника ГЛОНАСС
    qint32 getSatLit(int Num);          //Литера спутника ГЛОНАСС
    qint32 getTk(int Num);              //Время сообщения
    qreal getXcord(int Num);            //X координата спутника
    qreal getYcord(int Num);            //Y координата спутника
    qreal getZcord(int Num);            //Z координата спутника
    qreal getXspeed(int Num);           //скорость по X
    qreal getYspeed(int Num);           //скорость по Y
    qreal getZspeed(int Num);           //скорость по Z
    qreal getXacc(int Num);             //ускорение по X
    qreal getYacc(int Num);             //ускорение по Y
    qreal getZacc(int Num);             //ускорение по Z
    qreal getTimeCorr1(int Num);        //Значение из диапазона байт 63-70
    qreal getTauN(int Num);             //Значение из диапазона байт 71-74
    qreal getGammaN(int Num);           //Значение из диапазона байт 75-78
    quint32 getIsOk(int Num);
    quint32 getAgeOfInf(int Num);
    qint32 getNumYearGLO(int Num);
    qint32 getNumSecondGLO(int Num);

    //Эфемериды GPS
    quint32 getSatPRN(int Num);         //Номер спутника GPS
    quint32 getGPSt0e(int Num);         //Время в секундах от начала недели
    qint32 getIODC(int Num);
    qint32 getToC(int Num);
    qint32 getGPSWeek(int Num);
    qreal getTGD(int Num);
    qreal getSVClockDriftRate(int Num);
    qreal getSVClockDrift(int Num);
    qreal getSVClockBias(int Num);
    qint32 getToE(int Num);
    qint32 getIODE(int Num);
    qreal getSqrtA (int Num);
    qreal getEccentricity(int Num);
    qreal getM0(int Num);
    qreal getOmega0(int Num);
    qreal geti0(int Num);
    qreal getOmega(int Num);
    qreal getDeltaN(int Num);
    qreal getOmegaDot(int Num);
    qreal getIDOT(int Num);
    qreal getCrc(int Num);
    qreal getCrs(int Num);
    qreal getCuc(int Num);
    qreal getCus(int Num);
    qreal getCic(int Num);
    qreal getCis(int Num);
    qint32 getCodesL2(int Num);
    qint32 getSVaccuracy (int Num);
    qint32 getL2PdataFlag (int Num);
    qint32 getFitInterval (int Num);
    qint32 getSVhealth (int Num);
    //параметры модели ионосферы GPS
    qint32 getIonW(int Num);
    quint32 getIonT(int Num);
    qreal getIonAlpha0(int Num);
    qreal getIonAlpha1(int Num);
    qreal getIonAlpha2(int Num);
    qreal getIonAlpha3(int Num);
    qreal getIonBeta0(int Num);
    qreal getIonBeta1(int Num);
    qreal getIonBeta2(int Num);
    qreal getIonBeta3(int Num);
    //координаты
    qreal getApproxX(int Num);
    qreal getApproxY(int Num);
    qreal getApproxZ(int Num);
};

#endif // JPSREADER_H
