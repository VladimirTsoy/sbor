#include <QCoreApplication>
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QBitArray>
#include <QString>
#include <QDataStream>
#include <qmath.h>
//#include <QtMath>
#include "formnumlib.h"
#include "rintorinconv.h"

rintorinconv::rintorinconv()
{
    qDebug()<< "+";
}

/*
void rintorinconv::RinConv(QFile *file)
{
    QStringList data;                            // Создаем объект класса , куда мы будем считывать данные
    QString BufferStr;
    if (!file->open(QIODevice::ReadOnly))
    {      // Проверяем, возможно ли открыть наш файл для чтения
        //qDebug() << "1001";
    }
    else
    {
        //qDebug() << "1002";
    }
    //qDebug() <<;
    //читаем строку
    BufferStr = file->readLine();
    while(BufferStr.endsWith("END OF HEADER")|| BufferStr.endsWith("END OF HEADER       ") )
    {
    data << BufferStr;
    }

    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
       QString line = in.readLine();
    }
    inputFile.close();

    //Делим строку на слова разделенные пробелом
    //QStringList lst = str.split(" ");
      // выводим первых три слова

    //==================ТЕСТ СЧИТЫВАНИЯ========================
    for(quint32 i = 0; i<data.size(); i++)
    qDebug() << data.at(i) ;
    //=========================================================
    //qDebug() << lst.at(0) << lst.at(1)<<lst.at(2);

}
*/
