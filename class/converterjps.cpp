#include "converterjps.h"
#include <QCoreApplication>
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QRegExp>
#include "jpswriter.h"
#include "formnumlib.h"
#include "rinexreader.h"
#include "renexwriter.h"

using namespace formNum;

bool converterJps::DopplerStatus = true;
bool converterJps::useTextInfFromFile = false;
QString converterJps::programVersion = "CJTR 3.7.0";
QString converterJps::SpaceDefault = " ";
QList<QString> converterJps::Inf;
QList<int> converterJps::errorList;

int rinexReader::countTypeIzmInVersion2 = 0;
int *rinexReader::listTypesIzm;

converterJps::converterJps()
{
    qDebug()<< "+";

}

void converterJps::convert211To302Full (QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                                        QFile *outputObs, QFile *outputNav, QFile *outputNavG){
    rinexReader *Reader = new rinexReader(inputObs, inputNav, inputNavG, 211);
    renexWriter *Write = new renexWriter(Reader,211,converterJps::DopplerStatus,converterJps::SpaceDefault,
                                         inputObs, inputNav, inputNavG,
                                         outputObs, outputNav, outputNavG);
    //Очистка памяти
    delete Reader;
    delete Write;
}

void converterJps::convert302To211Full (QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                                        QFile *outputObs, QFile *outputNav, QFile *outputNavG){
    rinexReader *Reader = new rinexReader(inputObs, inputNav, inputNavG, 302);
    renexWriter *Write = new renexWriter(Reader,302,converterJps::DopplerStatus,converterJps::SpaceDefault,
                                         inputObs, inputNav, inputNavG,
                                         outputObs, outputNav, outputNavG);
    //Очистка памяти
    delete Reader;
    delete Write;
}

/*
void converterJps::convert211To302NoName (QFile *input){
    rinexReader *Reader = new rinexReader(input,211);

    bool *ok=0;
    QString suffix;
    qint16 Year =Reader->YearL.toInt(ok,10);
    suffix = CutYear(Year);

    QString temp;
    temp = MakeNameFileConv(input->fileName(), suffix+"O");
    QFile *RINEX = new QFile(temp);

    renexWriter *Write = new renexWriter(Reader,211,RINEX,converterJps::DopplerStatus);
    //Очистка памяти
    delete Reader;
    delete Write;
    delete RINEX;
}

void converterJps::convert302To211NoName (QFile *input){
    rinexReader *Reader = new rinexReader(input,302);

    bool *ok=0;
    QString suffix;
    qint16 Year =Reader->YearL.toInt(ok,10);
    suffix = CutYear(Year);

    QString temp;
    temp = MakeNameFileConv(input->fileName(), suffix+"O");
    QFile *RINEX = new QFile(temp);

    renexWriter *Write = new renexWriter(Reader,302,RINEX,converterJps::DopplerStatus);
    //Очистка памяти
    delete Reader;
    delete Write;
    delete RINEX;
}
*/

QString converterJps::convertJpsToRinex (QFile *input, QFile *output){
    jpsReader *Reader = new jpsReader(input);
    Reader->jpsReaderTextData(input);
    if(useTextInfFromFile){
        getInfFromFile(Reader);
    }
    //Получение значения текущего года (статическая переменная класса jpsReader - year)
    //Метод "CutYear" преобразовывает номер года в необходимый формат (две последние цифры в виде строки)
    QString suffix;
    suffix = CutYear(Reader->year);
    QString ret;

    QString temp;
    temp = MakeNameFile(output->fileName(), suffix+"O");
    ret=temp;
    QFile *RINEX = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"G");
    QFile *EGLO = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"N");
    QFile *EGPS = new QFile(temp);
    jpsWriter *Write = new jpsWriter(Reader,302,RINEX,EGLO,EGPS,DopplerStatus,SpaceDefault);

    //Очистка памяти
    delete Reader;
    delete Write;
    delete RINEX;
    delete EGLO;
    delete EGPS;

    return ret;
}

void converterJps::convertJpsToRinexNoName (QFile *input){
    jpsReader *Reader = new jpsReader(input);
    Reader->jpsReaderTextData(input);
    if(useTextInfFromFile){
        getInfFromFile(Reader);
    }
    //Получение значения текущего года (статическая переменная класса jpsReader - year)
    //Метод "CutYear" преобразовывает номер года в необходимый формат (две последние цифры в виде строки)
    QString suffix;
    suffix = CutYear(Reader->year);

    QString temp;
    temp = MakeNameFile(input->fileName(), suffix+"O");
    QFile *RINEX = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"G");
    QFile *EGLO = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"N");
    QFile *EGPS = new QFile(temp);
    jpsWriter *Write = new jpsWriter(Reader,302,RINEX,EGLO,EGPS,DopplerStatus,SpaceDefault);

    //Очистка памяти
    delete Reader;
    delete Write;
    delete RINEX;
    delete EGLO;
    delete EGPS;
}

void converterJps::convertJpsToRinex211VerNoName (QFile *input){
    jpsReader *Reader = new jpsReader(input);
    Reader->jpsReaderTextData(input);
    if(useTextInfFromFile){
        getInfFromFile(Reader);
    }
    //Получение значения текущего года (статическая переменная класса jpsReader - year)
    //Метод "CutYear" преобразовывает номер года в необходимый формат (две последние цифры в виде строки)
    QString suffix;
    suffix = CutYear(Reader->year);

    QString temp;
    temp = MakeNameFile(input->fileName(), suffix+"O");
    QFile *RINEX = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"G");
    QFile *EGLO = new QFile(temp);
    temp = MakeNameFile(RINEX->fileName(), suffix+"N");
    QFile *EGPS = new QFile(temp);
    jpsWriter *Write = new jpsWriter(Reader,211,RINEX,EGLO,EGPS,DopplerStatus,SpaceDefault);

    //Очистка памяти
    delete Reader;
    delete Write;
    delete RINEX;
    delete EGLO;
    delete EGPS;
}

void converterJps::getDopplerStatus (bool Doppler){
    DopplerStatus = Doppler;
}

void converterJps::setSpaceDefault(QString SetSpace)
{SpaceDefault = SetSpace;}

void converterJps::getUseTextInfFromFileStatus (bool Use){
    useTextInfFromFile = Use;
}

void converterJps::getInfFromFile(jpsReader *Reader){
    QString VerRes  = "";
    QString NumbRes = "";
    QString TypeRes = "";
    QByteArray temp = "rcv/ver/main";
    for(int i = 0; i < Reader->APM.length(); i++){
        if(Reader->APM.at(i).indexOf(temp)>=0){
            QByteArray idi = "\"";
            bool test = false;
            for(int j = 0; j < Reader->APM.at(i).length(); j++){
                if(Reader->APM.at(i).at(j)==idi.at(0)){
                    test = !test;
                    continue;
                }
                if(test){
                    VerRes.append(Reader->APM.at(i).at(j));
                }
            }
        }
    }

    temp = "rcv/id";
    for(int i = 0; i < Reader->APM.length(); i++){
        if(Reader->APM.at(i).indexOf(temp)>=0){
            QByteArray idi = "\"";
            bool test = false;
            for(int j = 0; j < Reader->APM.at(i).length(); j++){
                if(Reader->APM.at(i).at(j)==idi.at(0)){
                    test = !test;
                    continue;
                }
                if(test){
                    NumbRes.append(Reader->APM.at(i).at(j));
                }
            }
        }
    }

    temp = "rcv/ver/board";
    for(int i = 0; i < Reader->APM.length(); i++){
        if(Reader->APM.at(i).indexOf(temp)>=0){
            QByteArray idi = "\"";
            bool test = false;
            for(int j = 0; j < Reader->APM.at(i).length(); j++){
                if(Reader->APM.at(i).at(j)==idi.at(0)){
                    test = !test;
                    continue;
                }
                if(test){
                    TypeRes.append(Reader->APM.at(i).at(j));
                }
            }
        }
    }

    Inf[0] = programVersion;
    Inf[4] = NumbRes;
    Inf[5] = TypeRes;
    Inf[6] = VerRes;
}

void converterJps::getInf (QString progName,QString AggencyName,QString MrkNme,QString MrkNmb,
                           QString NumbRes,QString TypeRes,QString VerRes,QString NumbAnt,
                           QString TypeAnt){
    Inf.append(progName);
    Inf.append(AggencyName);
    Inf.append(MrkNme);
    Inf.append(MrkNmb);
    Inf.append(NumbRes);
    Inf.append(TypeRes);
    Inf.append(VerRes);
    Inf.append(NumbAnt);
    Inf.append(TypeAnt);
}


