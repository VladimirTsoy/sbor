#ifndef EPHEMERIDSGLONAS
#define EPHEMERIDSGLONAS
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include "jpsreader.h"
#include "rinexreader.h"

class EphemeridGLO//класс описывающий необходимые значения для описания эфемерид ГЛОНАСС в файлах RINEX
{
public:

qreal TauN;         //Сдвиг часов спутника (cекунды)
qreal GammaN;       //Относительный сдвиг частоты
qreal tk;           //Время сообщения
qreal Xcord;        //X координата спутника
qreal Xspeed;       //скорость по X
qreal Xacceleration;//ускорение по X
qreal IsOk;         //Исправность
qreal Ycord;        //Y координата спутника
qreal Yspeed;       //скорость по Y
qreal Yacceleration;//ускорение по Y
qreal NumbFreqChan; //Номер частотного канала
qreal Zcord;        //Z координата спутника
qreal Zspeed;       //скорость по Z
qreal Zacceleration;//ускорение по Z
qreal AgeOfInf;     //Возраст информации
qint32 NumYearGLO;  //Номер текущего года в четырёхлетнем цикле
qint32 NumSecondGLO;//Секунда в текущем году
quint32 NumSatGLO;
qint32 NumYearGLO_0; //Номер текущего года в четырёхлетнем цикле на первую эпоху
qint32 NumSecondGLO_0; //...на первую эпоху
qint16 Year;

//Флаг указывающий на наличие или отсутствие входного момента времени в формате Г.М.Д.Ч.М.С
//true при формированиее объекта с использованием файла в формате RINEX
bool inputUTCtime;
quint32 YearEphInput;
quint32 MonthEphInput;
quint32 DayEphInput;
quint32 HourEphInput;
quint32 MinEphInput;
qreal SecEphInput;

//Конструктор класса при конвертации из .jps
EphemeridGLO(jpsReader *Reader, qint32 Num);
//Конструктор класса при конвертации из RINEX
EphemeridGLO (QStringList *AList, quint8 Version);
~EphemeridGLO() { if(alist!=nullptr) delete alist; }

static void WriteRNX211HeaderN(QFile *RINEX);// функция создания и записи в файл header Rinex GLONASS NAV DATA версии 2.11

static void WriteRNX302HeaderN(QFile *RINEX);// функция создания и записи в файл header Rinex GNSS NAV DATA версии 3.02

void WriteRNXEphN_211(QFile *RINEX);//запись в файл .N версии 211 эфемерид

void WriteRNXEphN_302(QFile *RINEX); //запись в файл .N версии 302 эфемерид

static void WriteRNX2HeaderN(QFile *RINEX, rinexReader *Reader);// функция создания и записи в файл header Rinex GLONASS NAV DATA версии 2.11

static void WriteRNX3HeaderN(QFile *RINEX, rinexReader *Reader);// функция создания и записи в файл header Rinex GNSS NAV DATA версии 3.02
private:
    QStringList *alist = nullptr;
};
#endif // EPHEMERIDSGLONAS

