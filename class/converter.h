#ifndef CONVERTER_H
#define CONVERTER_H
//#include "reader.h"
#include <QtWidgets>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QMap>
#include "dirfile.h"


enum READ_STATE { READ_EPOCH_LINE,
                  READ_EPOCH_VALUES,
                  WRITE_EPOCH,
                  EXIT_FILE };

/// Таблица перевода типов измерений / сигналов
///=====================================================================================================
/// "C1C L1C D1C S1C C1P L1P D1P S1P C2C L2C D2C S2C C2P L2P D2P S2P C5C L5C D5C S5C C5P L5P D5P S5P";
/// "C1  L1  D1  S1  P1              C2  L2          P2      D2  S2  C5  L5  D5  S5";
///=====================================================================================================


class ConverterBase;

class RenexConverter{
public:
    RenexConverter();
    void convert_rinex_O_to_302(QString _fileName);

    void convert_rinex_O_to_211(QString _fileName);

    void convert_rinex_N_to_302(QString _fileName);

    void convert_rinex_N_to_211(QString _fileName);

    void convert_rinex_G_to_302(QString _fileName);

    void convert_rinex_G_to_211(QString _fileName);

    void setFilePath(QString _filePath);
private:
    QString filePath;
    ConverterBase *converterBase;
};


class ConverterBase{
public:
    ConverterBase(QString _filePath);
    ~ConverterBase();

    virtual void        readHeader() = 0;
    virtual void        writeEpoch() = 0;
    virtual QString     newFileName() = 0;

    void                setFilePath(QString _fName);
    QString             getFilePath();

    /// основной метод конвертации
    void convert();

    /// Установка указателя на конкретный класс наследник от ConverterBase для возможности вызова виртуальных функций
    void    setConverterBaseRealization(ConverterBase *_converterBase);

    void writeVersion(int v);

    /// Поиск и запись строки в header файла
    int findStringAndWrite(QString _str);

    /// Поиск и запись нескольких строк в header файла
    bool findAllStringAndWrite(QString _str);

    /// чтение строки из файла
    void virtual readLine(QString &line);

    QString getSymbolsString(QString symbol, int count);

    void makeHeaderData();

protected:
    ///Установленная директория функцией setFilePath
    QString filePathSet;

    QString filePath;
    QString fileName;
    QFile file;
    QTextStream *stream;
    QStringList dataForWrite;
    QStringList dataHeader;
    int numLineFile;
private:
    ConverterBase *converterBase;
};

///========================================
/// Конвертирование файла G "GLO" 3 в 2
/// =======================================
class ConvertG3To2 : public ConverterBase{
public:
    ConvertG3To2(QString _filePath);

    void        readHeader();
    void        writeEpoch();
    QString     newFileName();
};

///========================================
/// Конвертирование файла G "GLO" 2 в 3
/// =======================================
class ConvertG2To3 : public ConverterBase{
public:
    ConvertG2To3(QString _filePath);

    void        readHeader();
    void        writeEpoch();
    QString     newFileName();
};

///========================================
/// Конвертирование файла N "GPS" 3 в 2
/// =======================================
class ConvertN3To2 : public ConverterBase{
public:
    ConvertN3To2(QString _filePath);

    void        readHeader();
    void        writeEpoch();
    QString     newFileName();

private:

};

///========================================
/// Конвертирование файла N "GPS" 2 в 3
/// =======================================
class ConvertN2To3 : public ConverterBase{
public:
    ConvertN2To3(QString _filePath);

    void        readHeader();
    void        writeEpoch();
    QString     newFileName();
};

///========================================
/// Конвертирование файла O 2 в 3
/// =======================================
class ConvertO2To3 : public ConverterBase{
public:
    ConvertO2To3(QString _filePath);
    void        readHeader();
    void        writeEpoch();
    QString     newFileName();

    void        readLine(QString &line);
    void        getPrnObs(QString _str, QString numSat);
    void        getObsTypes();
private:

    QString templateSat3     = "C1C L1C D1C S1C C1P C2C C2P L2P D2P S2P C5C L5C D5C S5C";
    QString templateSat2     = "C1  L1  D1  S1  P1  C2  P2  L2  D2  S2  C5  L5  D5  S5";

    READ_STATE                  STATE_READER;
    QString                     fileName;

    //lsts для записи данных
    QStringList                 dataEpochs;

    bool                        prnObsTrue;     //флаг записи prnObs в header
    bool                        streamAtEnd;    //флаг окончании файла для чтения

    int                         countSatLineInEpochLines;
    int                         itNumLinesInFile;
    QString                     typesOfObserver;
    int                         countTypesOfObserver;

    struct EpochVer3{
        QString                     sattellites;
        QString                     epochLine;

        int                         epochLines;
        int                         linesInEpoch;

        QMap<QString, QString>      epochValues;
        QMap<QString, QString>      templateMap;
        QMap<QString, QVector<int>> prnOfObs;
    }epoch3;
};

///========================================
/// Конвертирование файла O 3 в 2
/// =======================================
class ConvertO3To2 : public ConverterBase{
public:
    ConvertO3To2(QString _filePath);
    void        readHeader();
    void        writeEpoch();
    QString     newFileName();

    void        writeEpochOne();
    void        getObsTypes();
    void        writeObsTypes();
private:

    /// шаблон перевода типов сигналов
    QString templateSat3     = "C1C L1C D1C S1C C1P C2C C2P L2P D2P S2P C5C L5C D5C S5C";
    QString templateSat2     = "C1  L1  D1  S1  P1  C2  P2  L2  D2  S2  C5  L5  D5  S5";

    bool prnObsTrue = false;
    struct EpochVer2{
        int countLines;
        QString epochLine;
        QMap<QString, QString> sattellites;

        QVector<int> vec;
        QVector<QString> epochLines;

        QString satGPS = "";
        QVector<int> satGPSVecObsBool;
        QVector<int> satGPSVecWriteIzm;


        QString satGLO = "";
        QVector<int> satGLOVecObsBool;
        QVector<int> satGLOVecWriteIzm;

        //Сформированный список обощенных сигналов (GPS и GLONASS) для версии 2.Х
        QString satAll = "";
    }epoch2;
};


#endif // CONVERTER_H
