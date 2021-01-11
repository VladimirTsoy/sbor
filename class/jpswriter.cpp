#include "jpswriter.h"
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "jpsreader.h"
#include "epoch.h"
#include "eglonass.h"
#include "egps.h"
//#include "windows.h"

jpsWriter::jpsWriter(jpsReader *Reader, int Version, QFile *RINEX, QFile *EGLO, QFile *EGPS, bool Doppler,QString SpaceDefault)
{

    //Открываем файл
    RINEX->open(QIODevice::WriteOnly);
    EGLO->open(QIODevice::WriteOnly);
    EGPS->open(QIODevice::WriteOnly);

    //Запись файлов наблюдений RINEX версии 3.02
    if(Version==302){
    Epoch::setLitGlo(Reader);
    Epoch::WriteRNX302HeaderO(RINEX,0,0,0,Doppler,Reader);
    int N = Reader->A1P.length();
    QList<Epoch*> LE;
    for(int q=0; q<N; q++){
        Epoch *E = new Epoch(Reader,q);
        LE.append(E);
    }
    for(int q=0; q<N; q++){
        LE.at(q)->WriteRNX302Eph(RINEX, Doppler,SpaceDefault);
    }

    //Запись навигационного файла ГЛОНАСС
    EphemeridGLO::WriteRNX302HeaderN(EGLO);
    int nGlo = Reader->ANE.length();
    qDebug() << nGlo;
    QList<EphemeridGLO*> LEGLO;
    for(int q=0; q<nGlo; q++){
        EphemeridGLO *E = new EphemeridGLO(Reader,q);
        LEGLO.append(E);
    }
    for(int q=0; q<nGlo; q++){
        LEGLO.at(q)->WriteRNXEphN_302(EGLO);
    }

    //Запись навигационного файла GPS
    EphemeridGPS::setIonGpsArguments(Reader,0);
    EphemeridGPS::WriteRNX302HeaderG(EGPS);
    int nGps = Reader->AGE.length();
    qDebug() << nGps;
    QList<EphemeridGPS*> LEGPS;
    for(int q=0; q<nGps; q++){
        EphemeridGPS *E = new EphemeridGPS(Reader,q);
        LEGPS.append(E);
    }
    for(int q=0; q<nGps; q++){
        LEGPS.at(q)->WriteRNXEphG(EGPS);
    }

    //Очистка памяти
    for(int i = 0; i < LE.length(); i++){
        LE.at(i)->destroyer();
        delete LE.at(i);
    }
    for(int i = 0; i < LEGLO.length(); i++){
        delete LEGLO.at(i);
    }
    for(int i = 0; i < LEGPS.length(); i++){
        delete LEGPS.at(i);
    }

    }


    //Запись файлов наблюдений RINEX версии 2.10
    if(Version==211){
        Epoch::setLitGlo(Reader);
        Epoch::WriteRNX211HeaderO(RINEX,0,0,0,0,Doppler,Reader);
        int N =Reader->A1P.length();
        QList<Epoch*> LE;
        for(int q=0; q<N; q++){
            Epoch *E = new Epoch(Reader,q);
            LE.append(E);
        }
        for(int q=0; q<N; q++){
            LE.at(q)->WriteRNX211Eph(RINEX, Doppler,SpaceDefault);
        }
    //Запись навигационного файла ГЛОНАСС
        EphemeridGLO::WriteRNX211HeaderN(EGLO);
        int nGlo = Reader->ANE.length();
        qDebug() << nGlo;
        QList<EphemeridGLO*> LEGLO;
        for(int q=0; q<nGlo; q++){
            EphemeridGLO *E = new EphemeridGLO(Reader,q);
            LEGLO.append(E);
        }
        for(int q=0; q<nGlo; q++){
            LEGLO.at(q)->WriteRNXEphN_211(EGLO);
        }
    //Запись навигационного файла GPS
        EphemeridGPS::setIonGpsArguments(Reader,0);
        EphemeridGPS::WriteRNX211HeaderG(EGPS);
        int nGps = Reader->AGE.length();
        QList<EphemeridGPS*> LEGPS;
        for(int q=0; q<nGps; q++){
            EphemeridGPS *E = new EphemeridGPS(Reader,q);
            LEGPS.append(E);
        }
        for(int q=0; q<nGps; q++){
            LEGPS.at(q)->WriteRNXEphG(EGPS);
        }

        //Очистка памяти
        for(int i = 0; i < LE.length();i++){
            LE.at(i)->destroyer();
            delete LE.at(i);
        }
        for(int i = 0; i < LEGLO.length();i++){
            delete LEGLO.at(i);
        }
        for(int i = 0; i < LEGPS.length();i++){
            delete LEGPS.at(i);
        }
    }

    //Закрываем файл
    RINEX->close();
    EGLO->close();
    EGPS->close();
}
