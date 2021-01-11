#ifndef FORMNUMLIB_H
#define FORMNUMLIB_H
#include <QString>
#include <QVector>
#include <QtMath>
#include <QByteArray>
#include <QBitArray>
#include "formnumlib.h"
#include <QDateTime>

namespace formNum {

struct DateUTC
{
       quint32 Date[5]; // Массив данных даты [0]-год,[1]-месяц,[2]-день,[3]-час,[4]-минута
       qreal Seconds; // секунды
};

QString dToStr4D(qreal d);      //функция по переводу числа qreal в вид .0000D00
QString dToStr10D(qreal d);     //функция по переводу числа qreal в вид .0000000000D00
QString dToStr12D(qreal d);     //функция по переводу числа qreal в вид .000000000000D00
QString dToStr4E(qreal d);      //функция по переводу числа qreal в вид 0.0000E00
QString dToStr12E(qreal d);     //функция по переводу числа qreal в вид 0.000000000000E00
QString dToStr10E(qreal d);     //функция по переводу числа qreal в вид 0.0000000000E00
QString dToStr12DObs(qreal d);  //функция по переводу числа qreal в вид 0.000000000000D00
qreal StrToD(QString Str);
QString NoiseToSigma(quint32 i);
qreal SigmaToNoise(quint32 i);
QString ToStandartNum(quint32 a);
DateUTC ConvertDatToUTC(qreal DatGPSw,qreal DatGPSsec);
DateUTC ConvertGLODatToUTC(quint32 DayGLO, quint32 SecGLO, quint32 ReceiverY);
qint32 uraToSVaccuracy(qint32 i);
QBitArray toBitArray(QByteArray fromArray);
quint32 FromUTCDatToUTCDayofWeek(quint32 Y, quint32 M,quint32 D);
QString MakeNameFile(QString A,QString B);
QString MakeNameFileConv(QString A,QString B);
QString CutYear(quint16 A);
qreal GPSwRecount(QDateTime A,qreal B);
QString AddSpace (QString A, quint8 B);
qint8 litCalc (qreal C, qreal L);
int getNoiseOfStr(QString SatMeasString, int it);

}
#endif // FORMNUMLIB_H
