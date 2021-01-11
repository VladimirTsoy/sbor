#ifndef RNXNCONV
#define RNXNCONV
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "jpsreader.h"
#include "epoch.h"
#include "egps.h"
#include "eglonass.h"

class RnxNconv
{
public:
    RnxNconv( QFile *RINEX);
};

#endif // RNXNCONV

