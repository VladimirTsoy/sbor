#include <QString>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include "jpswriter.h"
#include "formnumlib.h"
#include "integritycontrol.h"
#include "jpsreader.h"

using namespace formNum;
QDateTime *integrityControl::timeOfFirst;
QDateTime *integrityControl::timeOfLast;

integrityControl::integrityControl(QFile *input, QFile *log)
{
    jpsReader *Reader = new jpsReader(input);
    setMetaData (Reader);
    fileQualityFlag = true;
    controlNumber = controlNumberMessages(Reader);
    if(controlNumber!=0){
        fileQualityFlag = false;
    }

    //получить текущую дату, выраженную в секундах
    QDateTime LogDate = QDateTime::currentDateTime();
    QString strLogDate = LogDate.toString("dd.MM.yyyy HH:mm:ss");
    //Открываем файл
    log->open(QIODevice::WriteOnly);
    //Подготовка к записи файла
    QTextStream out(log);
    out << QString("Время создания log-файла: ") << strLogDate << endl;
    out << QString("Имя файла .L: ") << input->fileName() << endl;
    out << QString("Первая эпоха: ") << timeOfFirst->toString("dd.MM.yyyy hh:mm:ss.zzz") << endl;
    out << QString("Последняя эпоха: ") << timeOfLast->toString("dd.MM.yyyy hh:mm:ss.zzz") << endl;
    out << QString("Флаг качества файла: ") << fileQualityFlag << endl;
    out << QString("Контрольная сумма числа сообщений: ") << controlNumber << endl;
    out << QString("Контрольная сумма отличная от нуля означает что в файле есть лишние \n"
                   "или недостающие сообщения нарушающие структуру файла. Если результат \n"
                   "меньше 0, то вероятно часть сообщений потеряна. Если результат больше 0,\n"
                   "то вероятно потеряны сообщения типа RD или есть лишние сообщения других типов.\n"
                   "Файлы с нарушеной структурой не подлежат конвертации.") << endl;
    out << QString("Найдено сообщений типа RD: ") << Reader->ARD.length()<< endl;
    out << QString("Найдено сообщений типа TT: ") << Reader->ATT.length()<< endl;
    out << QString("Найдено сообщений типа SI: ") << Reader->ASI.length()<< endl;
    out << QString("Найдено сообщений типа NN: ") << Reader->ANN.length()<< endl;
    out << QString("Найдено сообщений типа RC: ") << Reader->ARC.length()<< endl;
    out << QString("Найдено сообщений типа 1R: ") << Reader->A1R.length()<< endl;
    out << QString("Найдено сообщений типа 2R: ") << Reader->A2R.length()<< endl;
    out << QString("Найдено сообщений типа CP: ") << Reader->ACP.length()<< endl;
    out << QString("Найдено сообщений типа 1P: ") << Reader->A1P.length()<< endl;
    out << QString("Найдено сообщений типа 2P: ") << Reader->A2P.length()<< endl;
    out << QString("Найдено сообщений типа D1: ") << Reader->AD1.length()<< endl;
    out << QString("Найдено сообщений типа D2: ") << Reader->AD2.length()<< endl;
    out << QString("Найдено сообщений типа EC: ") << Reader->AEC.length()<< endl;
    out << QString("Найдено сообщений типа E1: ") << Reader->AE1.length()<< endl;
    out << QString("Найдено сообщений типа E2: ") << Reader->AE2.length()<< endl;
    out << QString("Найдено сообщений типа NE: ") << Reader->ANE.length()<< endl;
    out << QString("Найдено сообщений типа GE: ") << Reader->AGE.length()<< endl;
    out << QString("Найдено сообщений типа IO: ") << Reader->AIO.length()<< endl;
    out << QString("Найдено сообщений типа PV: ") << Reader->APV.length()<< endl;

    log->close();

    delete Reader;
    input->close();
}

void integrityControl::setMetaData (jpsReader *Reader){
    //Получение даты начала измерений (статическая переменная timeOfFirst)
    int Year = Reader->getYear(0);
    int Month = Reader->getMonth(0);
    int Day = Reader->getDay(0);
    int Hour = Reader->getHour(0);
    int Minute = Reader->getMinute(0);
    qreal Second = Reader->getSecond(0);
    QDate temp;
    temp.setDate(Year,Month,Day);
    QTime time;
    int tempSec = Second;
    int tempMSec = Second*1000;
    tempMSec = tempMSec-tempSec*1000;
    time.setHMS(Hour,Minute,tempSec,tempMSec);
    timeOfFirst = new QDateTime(temp,time,Qt::OffsetFromUTC);
    //Получение даты начала измерений (статическая переменная timeOfFirst)
    Year = Reader->getYear(Reader->ARD.length()-1);
    Month = Reader->getMonth(Reader->ARD.length()-1);
    Day = Reader->getDay(Reader->ARD.length()-1);
    Hour = Reader->getHour(Reader->ARD.length()-1);
    Minute = Reader->getMinute(Reader->ARD.length()-1);
    Second = Reader->getSecond(Reader->ARD.length()-1);
    temp.setDate(Year,Month,Day);
    tempSec = Second;
    tempMSec = Second*1000;
    tempMSec = tempMSec-tempSec*1000;
    time.setHMS(Hour,Minute,tempSec,tempMSec);
    timeOfLast = new QDateTime(temp,time,Qt::OffsetFromUTC);
}

//Метод для проверки соответствия числа сообщений разных типов числу эпох
//Если результат меньше 0, то вероятно часть сообщений потеряна
//Если результат больше 0, то вероятно потеряны сообщения типа RD или есть лишние сообщения других типов
qint32 integrityControl::controlNumberMessages(jpsReader *Reader){
    qint32 temp = 0;
    qint32 RD = Reader->ARD.length();                           //Сообщение содержит текущую дату (день, месяц, год)
    if(Reader->A1P.length()!=0)temp += Reader->A1P.length()-RD; //Разница L1P-C1 (в секундах)
    if(Reader->A1R.length()!=0)temp += Reader->A1R.length()-RD; //Разница P1-C1 (в секундах)
    if(Reader->A2P.length()!=0)temp += Reader->A2P.length()-RD; //Разница L2-C1 (в секундах)
    if(Reader->A2R.length()!=0)temp += Reader->A2R.length()-RD; //Разница P2-C1 (в секундах)
    if(Reader->ARC.length()!=0)temp += Reader->ARC.length()-RD; //Кодовые псевдодальности [C1] по частоте L1 (полное значение в секундах)
    if(Reader->ACP.length()!=0)temp += Reader->ACP.length()-RD; //Разница L1-C1 (в секундах)
    if(Reader->AD1.length()!=0)temp += Reader->AD1.length()-RD; //Доплеровский сдвиг [D1P] по L1P
    if(Reader->AD2.length()!=0)temp += Reader->AD2.length()-RD; //Доплеровский сдвиг [D2] по L2
    if(Reader->ADC.length()!=0)temp += Reader->ADC.length()-RD; //Доплеровский сдвиг [D1] по L1
    if(Reader->AE1.length()!=0)temp += Reader->AE1.length()-RD; //Шум
    if(Reader->AE2.length()!=0)temp += Reader->AE2.length()-RD; //Шум
    if(Reader->AEC.length()!=0)temp += Reader->AEC.length()-RD; //Шум
    if(Reader->ATT.length()!=0)temp += Reader->ATT.length()-RD; //Сообщение содержит текущее время (секунд от начала суток)
    if(Reader->ASI.length()!=0)temp += Reader->ASI.length()-RD; //Индексы спутников (Номера аппаратов во внутренней системе счисления)
    if(Reader->ANN.length()!=0)temp += Reader->ANN.length()-RD; //Номера спутников ГЛОНАСС
    return temp;
}

void integrityControl::destroyer(){

}
