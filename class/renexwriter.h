#ifndef RENEXWRITER_H
#define RENEXWRITER_H
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "jpsreader.h"
#include "epoch.h"


class renexWriter
{
public:
    renexWriter(rinexReader *Reader, int Version, bool Doppler, QString SpaceDefault,
                QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                QFile *outputObs, QFile *outputNav, QFile *outputNavG);
};

#endif // RENEXWRITER_H
