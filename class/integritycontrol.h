#ifndef INTEGRITYCONTROL_H
#define INTEGRITYCONTROL_H
#include <QString>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QDateTime>
#include "jpswriter.h"
#include "formnumlib.h"
#include "integritycontrol.h"
#include "jpsreader.h"

class integrityControl
{
public:
    //ПЕРЕМЕННЫЕ КЛАССА
    qint32 controlNumber;
    //Признак качества файла (целостности сообщений)
    bool fileQualityFlag;
    //Момент начала измерений
    static QDateTime *timeOfFirst;
    //Момент окончания измерений
    static QDateTime *timeOfLast;
    //МЕТОДЫ КЛАССА
    //Конструктор класса
    integrityControl(QFile *input, QFile *log);
    //Метод сбора метаданных
    void setMetaData (jpsReader *Reader);
    //Метод контроля целостности сообщений
    qint32 controlNumberMessages(jpsReader *Reader);
    void destroyer();
};

#endif // INTEGRITYCONTROL_H
