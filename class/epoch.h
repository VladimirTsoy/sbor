#ifndef EPOCH
#define EPOCH
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include <QStringList>
#include "epoch.h"
#include "jpsreader.h"
#include "formnumlib.h"
#include "rinexreader.h"

//Родительский клаcc формирования классов SatGPS и SatGLO
class sattelite
{
public:
    QString NumToNameG(quint32 n)
   {    QString SatTypeStr = "G";
        QString SatTypeStr0 = "G0";
        QString NameStr;
        if(n<=9)
        {NameStr = QString("%1%2").arg(SatTypeStr0).arg(n);}
        else
        {NameStr = QString("%1%2").arg(SatTypeStr).arg(n);}
        return NameStr ;
    }
    QString NumToNameR(quint32 n)
   {    QString SatTypeStr = "R";
        QString SatTypeStr0 = "R0";
        QString NameStr;
        if(n<=9)
        {NameStr = QString("%1%2").arg(SatTypeStr0).arg(n);}
        else
        {NameStr = QString("%1%2").arg(SatTypeStr).arg(n);}
        return NameStr ;
    }
qreal Phase_L1;
qreal Phase_L1P;
qreal Phase_L1W;   //change

qreal Phase_L2P;
qreal Phase_L2W;  //change

qreal Code_L1P;
qreal Code_L1;
qreal Code_L1W; //change

qreal Code_L2;
qreal Code_L2P;
qreal Code_L2W;  //change

qreal Doppler_L1;
qreal Doppler_L1P;
qreal Doppler_L1W; //change

qreal Doppler_L2P;
qreal Doppler_L2W; //change

qreal Noise_L1;
qreal Noise_L1P;
qreal Noise_L1W; //change

qreal Noise_L2P;
qreal Noise_L2W; //change

quint32 NumOfSat = 0;

qreal Phase_L5;
qreal Code_L5;
qreal Code_L5P;
qreal Doppler_L5;
qreal Noise_L5;

bool flag_of_empty;
sattelite ()
{
    flag_of_empty = false;
}
};

class SatGPS : public sattelite   // производный класс GPS
{
public:
    SatGPS(quint32 Num,
           qreal C1,
           qreal C1P,
           qreal C2,
           qreal P2,
           qreal C5,
           qreal P5,
           qreal L1,
           qreal L1P,
           qreal L2,
           qreal L5,
           qreal D1,
           qreal D1P,
           qreal D2,
           qreal D5,
           qreal N_L1,
           qreal N_L1P,
           qreal N_L2P,
           qreal N_L5
           //change
//           qreal C1W,
//           qreal L1W,
//           qreal D1W,
//           qreal S1W,

//           qreal C1W,
//           qreal C1W,
//           qreal C1W,
//           qreal C1W
    )
    {
        NumOfSat = Num;
        //Кодовые измерения псевдодальностей
        Code_L1 = C1;
        Code_L1P = C1P;
        Code_L2 = C2;
        Code_L2P = P2;
        Code_L5 = C5;
        Code_L5P = P5;
        //Фазовые измерения псевдодальностей
        Phase_L1 = L1;
        Phase_L1P = L1P;
        Phase_L2P = L2;
        Phase_L5 = L5;
        //Доплеровский сдвиг частоты
        Doppler_L1 = D1;
        Doppler_L1P = D1P;
        Doppler_L2P = D2;
        Doppler_L5 = D5;
        //Шум сигнала
        Noise_L1 = N_L1;
        Noise_L1P = N_L1P;
        Noise_L2P = N_L2P;
        Noise_L5 = N_L5;

    flag_of_empty = true;

    }
};

class SatGLO : public sattelite   // производный класс ГЛОНАСС
{
public:
    qint32 litera;
    SatGLO (quint32 Num,
            qreal C1,
            qreal C1P,
            qreal C2,
            qreal P2,
            qreal C5,
            qreal P5,
            qreal L1,
            qreal L1P,
            qreal L2,
            qreal L5,
            qreal D1,
            qreal D1P,
            qreal D2,
            qreal D5,
            qint32 lit,
            qreal N_L1,
            qreal N_L1P,
            qreal N_L2P,
            qreal N_L5
            )
    {
    NumOfSat = Num;
    litera = lit;
    //Кодовые измерения псевдодальностей
    Code_L1 = C1;
    Code_L1P = C1P;
    Code_L2 = C2;
    Code_L2P = P2;
    Code_L5 = C5;
    Code_L5P = P5;
    //Фазовые измерения псевдодальностей
    Phase_L1 = L1;
    Phase_L1P = L1P;
    Phase_L2P = L2;
    Phase_L5 = L5;
    //Доплеровский сдвиг частоты
    Doppler_L1 = D1;
    Doppler_L1P = D1P;
    Doppler_L2P = D2;
    Doppler_L5 = D5;
    //Шум сигнала
    Noise_L1 = N_L1;
    Noise_L1P = N_L1P;
    Noise_L2P = N_L2P;
    Noise_L5 = N_L5;

    flag_of_empty = true;
    }
};

class Epoch // класс эпоха, отвечает за дату эпохи, количество спутников в эпохе и запись данных в файлы
{
public:
    ~Epoch();
    //переменные хранения времени
    quint32 YearEph;//год эпохи
    quint32 MonthEph;//месяц эпохи
    quint32 DayEph;//день эпохи
    quint32 HourEph;//час эпохи
    quint32 MinEph;//мин эпохи
    qreal SecEph;//сек эпохи
    qreal ReceiverClockOffset; //ошибка часов приемника

    //bool mesOn[36]; //red
    bool mesOn[44];   //red

    bool mesO[25];    /// общий

    bool isLfile;

    //конструктор класса Epoch при конвертации из jps
    Epoch (jpsReader *Reader, int Num);
    //конструктор класса Epoch при конвертации из RINEX
    Epoch (QStringList *AList,
           quint16 NumbInOrderTypepsObs[15],
           quint16 NumbInOrderTypepsObsGPS[25],
           quint16 NumbInOrderTypepsObsGLO[16],
           bool TypesOfObsCheck[15],
           bool TypesOfObsCheckGPS[34],
           bool TypesOfObsCheckGLO[17],
           int version,QString NofTypes);

    /*  Код ошибки перехваченный блоком перехвата исключений
     *  0 - нет ошибок (исключения не перехватывались
     *  101 - ошибка связанная с обращением к неверному элементу AList
     *  может быть связа со следующими причинами:
     *  1) Потеря строк в блоке эпохи;
     *  2) Неверно указано число спутников наблюдаемых в эпохе;
     *  3) Ошибка в работе метода rinexReader::readInputStream
     *
     */
    int errorCode;

    void destroyer();

    void WriteRNX302Eph(QFile *RINEX, bool Doppler,QString SpaceDefault); //запись в файл .O версии 302 эпохи измерений
    void WriteRNX211Eph(QFile *RINEX, bool Doppler,QString SpaceDefault); //запись в файл .O версии 211 эпохи измерений
    static void WriteRNX302HeaderORinexRead(QFile *RINEX,rinexReader *Reader);
    static void WriteRNX211HeaderORinexRead(QFile *RINEX,rinexReader *Reader);
    static void WriteRNX302HeaderO(QFile *RINEX,
                                   double deltaH,
                                   double deltaN,
                                   double deltaE,
                                   bool Doppler,
                                   jpsReader *Reader);
    static void WriteRNX211HeaderO(QFile *RINEX,
                                   double deltaH,
                                   double deltaN,
                                   double deltaE,
                                   int NumbOFTypes,
                                   bool Doppler,
                                   jpsReader *Reader);
    static void setLitGlo(jpsReader *Reader);

    static int lit [40];
private:
    QList <SatGPS*> ArraySatGPS;
    QList <SatGLO*> ArraySatGLO;
    void educhfunc(QFile *RINEX);
    void divide101(int a, int b);

    QStringList *alist = nullptr; //указатель для очистки памяти
};

#endif // EPOCH
