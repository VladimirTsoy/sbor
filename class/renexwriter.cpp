#include "renexwriter.h"
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

renexWriter::renexWriter(rinexReader *Reader,int Version, bool Doppler, QString SpaceDefault,
                         QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                         QFile *outputObs, QFile *outputNav, QFile *outputNavG)
{
    quint16 mas0[24];
    bool pr_zap=false;
    //Запись файлов наблюдений RINEX версии 3.02
    if(Version==211){

        /*  Коневертация файла наблюдений RINEX версии 2.10 в 3.02
         *  QTextStream *stream - поток для чтения файла наблюдений
         *  QStringList *A - блок текстовой информации содержащий одну эпоху
         *  QList<Epoch*> LE - лист объектов "Epoch"
         */

        if(!outputObs->fileName().isEmpty()){
//        if(Reader->noObservationFile==false){
            //Открываем файл
            qDebug() << outputObs->fileName();
            if (!outputObs->open(QIODevice::WriteOnly)) qDebug() << "open error";
            //ЗАПИСЬ ЗАГОЛОВКА
            qDebug()<< "Header started";
            Epoch::WriteRNX302HeaderORinexRead(outputObs,Reader);
            qDebug()<< "Header finished";

            int N = 0;
            QList<Epoch*> LE;
            qDebug()<<Reader->FILE->fileName();

//            outputObs->close();
//            Reader->FILE->close();
//            inputObs->close();
//            return;

            //ЗАПИСЬ НАБЛЮДЕНИЙ
            if (!Reader->FILE->open(QIODevice::ReadOnly)) qDebug() << "open error";
            QTextStream *stream = new QTextStream(Reader->FILE);
            qDebug()<< "RINEX epoch creation started";
            //for(int q=0; q<10000; N++){
            for( ; ; ){
                //qDebug()<< "n211"<< N;
                QStringList *A = new QStringList (Reader->readInputStream(stream));
                if(A->length()==0){
                    break;
                }
                Epoch *E = new Epoch(A, Reader->NumbInOrderTypepsObs,mas0,mas0,Reader->TypesOfObsCheck,
                                     Reader->TypesOfObsCheckGPS,Reader->TypesOfObsCheckGLO,
                                     211,Reader->NofTypes);
                LE.append(E);
                N++;
            }
            qDebug()<< "RINEX epoch creation finished";
            for(int q=0; q<N; q++){
                LE.at(q)->WriteRNX302Eph(outputObs, Doppler,SpaceDefault);
            }

            //Чистка памяти
            for(int i = 0; i < LE.size(); i++){
                LE[i]->destroyer();
                delete LE[i];
            }
            if(stream != nullptr)
                delete stream;

            qDebug()<< "WriteRNX302Eph finished";
            Reader->FILE->close();
            outputObs->close();
            inputObs->close();
            qDebug()<< "close";
            pr_zap=true;
        }

        /*  Конвертация навигационного файла RINEX версии 2.10 в 3.02
         *  inputNav - входной файл в формате RINEX 2.10
         *  outputNav - выходной файл в формате RINEX 3.02
         */

        if(!outputNav->fileName().isEmpty()){
//        if(Reader->noNavigationFile==false){
            int NAV = 0;
            qDebug() << inputNav->fileName();
            if (!inputNav->open(QIODevice::ReadOnly)) qDebug() << "open error";

            qDebug() << outputNav->fileName();
            if (!outputNav->open(QIODevice::WriteOnly)) qDebug() <<"open error";

            EphemeridGPS::WriteRNX3HeaderG(outputNav, Reader);
  //          outputNav->close();
  //          qDebug() << "outputNav->close()";

  //          inputNav->close();
  //          qDebug() << "inputNav->close()";
  //          return;

            QList<EphemeridGPS*> LEGPS;
            QTextStream *streamNav = new QTextStream(inputNav);
            for(int q=0; q<10000; NAV++){
                QStringList *A = new QStringList (Reader->readInputStreamNav210(streamNav));
                if(A->length()==0){
                    break;
                }
                EphemeridGPS *E = new EphemeridGPS(A,2);
                LEGPS.append(E);
            }

            qDebug() << "form list";

            for(int q=0; q<NAV; q++){
                LEGPS.at(q)->WriteRNXEphG(outputNav);
                qDebug() << "write " << q;
            }
            //Чистка памяти
            for(int i = 0; i < LEGPS.size(); i++){
                delete LEGPS[i];
            }

            if(streamNav != nullptr)
                delete streamNav;

            outputNav->close();
            qDebug() << "outputNav->close()";

            inputNav->close();
            qDebug() << "inputNav->close()";
            pr_zap=true;
        } //КОНЕЦ УСЛОВИЯ

        /*  Конвертация навигационного ГЛОНАСС файла RINEX версии 2.10 в 3.02
         *  inputNavG - входной файл в формате RINEX 2.10
         *  outputNavG - выходной файл в формате RINEX 3.02
         */

        if(!outputNavG->fileName().isEmpty()){
//        if(Reader->noNavigationFileG==false){
            int GLO = 0;

            qDebug() << inputNavG->fileName();
            if (!inputNavG->open(QIODevice::ReadOnly)) qDebug() << "open error";

            qDebug() << outputNavG->fileName();
            if (!outputNavG->open(QIODevice::WriteOnly)) qDebug() << "open error";

            EphemeridGLO::WriteRNX3HeaderN(outputNavG,Reader);
 //           outputNavG->close();
 //           inputNavG->close();
//return;
            QList<EphemeridGLO*> LEGLO;
            QTextStream *streamNavG = new QTextStream(inputNavG);
            for(int q=0; q<10000; GLO++){
                QStringList *A = new QStringList (Reader->readInputStreamNavG210(streamNavG));
                if(A->length()==0){
                    break;
                }
                EphemeridGLO *E = new EphemeridGLO(A,2);
                LEGLO.append(E);
            }
            //чистка памяти
            for(int q=0; q<GLO; q++){
                LEGLO.at(q)-> WriteRNXEphN_302(outputNavG);
            }
            for(int i = 0; i < LEGLO.size(); i++){
                delete LEGLO[i];
            }
            if(streamNavG != nullptr)
                delete streamNavG;

            outputNavG->close();
            inputNavG->close();
            pr_zap=true;
        } //КОНЕЦ УСЛОВИЯ

     if (!pr_zap) qDebug() << "не записано файлов";
     return;
    } //КОНЕЦ УСЛОВИЯ ДЛЯ 211


    //Запись файлов наблюдений RINEX версии 2.10
    if(Version==302){

        /*  Коневертация файла наблюдений RINEX версии 3.02 в 2.11
         *  QTextStream *stream - поток для чтения файла наблюдений
         *  QStringList *A - блок текстовой информации содержащий одну эпоху
         *  QList<Epoch*> LE - лист объектов "Epoch"
         */

        if(!outputObs->fileName().isEmpty()){
//        if(Reader->noObservationFile==false){
            //Открываем файл
            outputObs->open(QIODevice::WriteOnly);
            //ЗАПИСЬ ЗАГОЛОВКА
            qDebug()<< "Header started";
            Epoch::WriteRNX211HeaderORinexRead(outputObs,Reader);
            qDebug()<< "Header finished";
            int N = 0;
            QList<Epoch*> LE;
            qDebug()<<Reader->FILE->fileName();

            //ЗАПИСЬ НАБЛЮДЕНИЙ
            Reader->FILE->open(QIODevice::ReadOnly);
            QTextStream *stream = new QTextStream(Reader->FILE);
            qDebug()<< "RINEX epoch creation started";
            //for(int q=0; q<1000; q++){
            for( ; ; ){
                //qDebug() << "<n302=" << N;
                QStringList *A = new QStringList (Reader->readInputStream302(stream));
                if(A->length()==0){
                    break;
                }
                Epoch *E = new Epoch(A, Reader->NumbInOrderTypepsObs,Reader->NumbInOrderTypepsObsGPS,
                                     Reader->NumbInOrderTypepsObsGLO,Reader->TypesOfObsCheck,
                                     Reader->TypesOfObsCheckGPS,Reader->TypesOfObsCheckGLO,
                                     302,Reader->NofTypes);
                LE.append(E);
                N++;
            }
            qDebug()<< "RINEX epoch creation finished";

            for(int q=0; q<N; q++){
                LE.at(q)->WriteRNX211Eph(outputObs, Doppler,SpaceDefault);
            }
            qDebug()<< "WriteRNX211Eph finished";

            //Чистка памяти
            for(int i = 0; i < LE.size(); i++){
                LE[i]->destroyer();
                delete LE[i];
            }
            if(stream != nullptr)
                delete stream;

            //Закрываем файл
            Reader->FILE->close();
            outputObs->close();
            inputObs->close();
        } //КОНЕЦ УСЛОВИЯ

        /*  Коневертация навигационного файла RINEX версии 3.02 в 2.10
         *  inputNav - входной файл в формате RINEX 3.02
         *  outputNav - выходной файл в формате RINEX 2.10
         */

        if(!outputNav->fileName().isEmpty()){
//        if(Reader->noNavigationFile==false){
            int NAV = 0;
            inputNav->open(QIODevice::ReadOnly);
            outputNav->open(QIODevice::WriteOnly);
            EphemeridGPS::setIonGpsArgumentsRNX(Reader,0);
            EphemeridGPS::WriteRNX2HeaderG(outputNav,Reader);
            QList<EphemeridGPS*> LEGPS;
            QTextStream *streamNav = new QTextStream(inputNav);
            for(int q=0; q<10000; NAV++){
                QStringList *A = new QStringList (Reader->readInputStreamNav210(streamNav));
                if(A->length()==0){
                    break;
                }
                EphemeridGPS *E = new EphemeridGPS(A,3);
                LEGPS.append(E);
            }
            for(int q=0; q<NAV; q++){
                LEGPS.at(q)->WriteRNXEphG211(outputNav);
            }

            for(int i = 0; i < LEGPS.size(); i++){
                delete LEGPS[i];
            }

            if(streamNav != nullptr)
                delete streamNav;

            outputNav->close();
            inputNav->close();
            qDebug()<< "Write GPS eph finished: " << outputNav->fileName();
        } //КОНЕЦ УСЛОВИЯ

        /*  Коневертация навигационного ГЛОНАСС файла RINEX версии 2.10 в 3.02
         *  inputNavG - входной файл в формате RINEX 2.10
         *  outputNavG - выходной файл в формате RINEX 3.02
         */

        if(!outputNavG->fileName().isEmpty()){
//        if(Reader->noNavigationFileG==false){
            int GLO = 0;
            inputNavG->open(QIODevice::ReadOnly);
            outputNavG->open(QIODevice::WriteOnly);
            EphemeridGLO::WriteRNX2HeaderN(outputNavG,Reader);
            QList<EphemeridGLO*> LEGLO;
            QTextStream *streamNavG = new QTextStream(inputNavG);
            for(int q=0; q<10000; GLO++){
                QStringList *A = new QStringList (Reader->readInputStreamNavG210(streamNavG));
                if(A->length()==0){
                    break;
                }
                EphemeridGLO *E = new EphemeridGLO(A,3);
                LEGLO.append(E);
            }
            for(int q=0; q<GLO; q++){
                LEGLO.at(q)-> WriteRNXEphN_211(outputNavG);
            }
            // удаление
            for(int i = 0; i < LEGLO.size(); i++){
                delete LEGLO[i];
            }
            if(streamNavG != nullptr)
                delete streamNavG;

            outputNavG->close();
            inputNavG->close();
            qDebug()<< "Write GLONASS eph finished: " << outputNavG->fileName();
        } //КОНЕЦ УСЛОВИЯ
    } //КОНЕЦ УСЛОВИЯ ДЛЯ 302
} //КОНЕЦ КОНСТРУКТОРА
