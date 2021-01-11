#ifndef CONVERTERJPS_H
#define CONVERTERJPS_H
#include "converterjps.h"
#include "jpsreader.h"
#include <QFile>

class converterJps
{
public:
    converterJps();
    //static void convert211To302NoName (QFile *input);
    static void convert211To302Full (QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                              QFile *outputObs, QFile *outputNav, QFile *outputNavG);
    static void convert302To211Full (QFile *inputObs, QFile *inputNav, QFile *inputNavG,
                              QFile *outputObs, QFile *outputNav, QFile *outputNavG);
    //static void convert302To211NoName (QFile *input);
    static QString convertJpsToRinex (QFile *input, QFile *output);
    static void convertJpsToRinexNoName (QFile *input);
    static void convertJpsToRinex211VerNoName (QFile *input);
    static void getDopplerStatus (bool Doppler);
    static void getUseTextInfFromFileStatus (bool Doppler);
    static void getInfFromFile(jpsReader *Reader);
    static void setSpaceDefault(QString SetSpace);
    static void getInf (QString progName,
                        QString AggencyName,
                        QString MrkNme,
                        QString MrkNmb,
                        QString NumbRes,
                        QString TypeRes,
                        QString VerRes,
                        QString NumbAnt,
                        QString TypeAnt);
    static bool DopplerStatus;
    static bool useTextInfFromFile;
    static QString programVersion;
    static QString SpaceDefault;
    static QList<QString> Inf;
    /* Используемые коды ошибок
     * 201 - невозможно открыть файл эфемерид GPS в формате RINEX 2.10
     * 202 - невозможно открыть файл эфемерид ГЛОНАСС в формате RINEX 2.10
     * 203 - версия файла (.O) не соответствует используемой функции convert211To302Full
     * 203 - версия файла (.O) не соответствует используемой функции convert302To211Full
     */
    static QList<int> errorList;
};

#endif // CONVERTERJPS_H
