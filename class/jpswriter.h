#ifndef JPSWRITER_H
#define JPSWRITER_H
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "jpsreader.h"
#include "epoch.h"


class jpsWriter
{
public:
    jpsWriter(jpsReader *Reader, int Version, QFile *RINEX, QFile *EGLO, QFile *EGPS, bool Doppler, QString SpaceDefault);
};

#endif // JPSWRITER_H
