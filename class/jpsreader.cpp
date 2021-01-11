#include "jpsreader.h"
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
#include "formnumlib.h"

using namespace formNum;

//Спецификация класса jpsReader

//====== ЭФЕМЕРИДЫ ГЛОНАСС ======
//Методы* для получения параметров эфемерид ГЛОНАСС реализованные в классе jpsReader:
//*Аргумент методов (Num) - порядковый номер блока данных эфемерид ГЛОНАСС в файле .jps
//getSatNum - возвращает номер спутника
//getSatLit - возвращает литеру спутника
//getTk - возвращает время сообщения в секундах
//getXcord - возвращает Х (км.) в СК ПЗ-90
//getYcord - возвращает Y (км.) в СК ПЗ-90
//getYcord - возвращает Z (км.) в СК ПЗ-90
//getXspeed - возвращает составляющую скорости по оси Х (км./c.) в СК ПЗ-90
//getYspeed - возвращает составляющую скорости по оси Y (км./c.) в СК ПЗ-90
//getZspeed - возвращает составляющую скорости по оси Z (км./c.) в СК ПЗ-90
//getXacc - возвращает составляющую ускорения по оси Х (км./c.^2) в СК ПЗ-90
//getYacc - возвращает составляющую ускорения по оси Y (км./c.^2) в СК ПЗ-90
//getZacc - возвращает составляющую ускорения по оси Z (км./c.^2) в СК ПЗ-90
//getTimeCorr1 -
//getTimeCorr1 -
//getTimeCorr1 -

quint16 jpsReader::year = 0;
quint32 jpsReader::numEp = 0;

jpsReader::jpsReader(QFile *file)
{
    QByteArray data;                            // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file->open(QIODevice::ReadOnly))       // Проверяем, возможно ли открыть наш файл для чтения
    {
        //qDebug() << "1001";
    }
    else
    {
        //qDebug() << "1002";
    }

    data = file->readAll();                     //ПОЛНОСТЬЮ СЧИТЫВАЕМ ФАЙЛ В МАССИВ БАЙТ
    qDebug() << data.length();
    file->close();

    //Список идентификаторов сообщений
    QList<QByteArray> symbol;
    symbol << "\nRD";       //Сообщение содержит текущую дату (день, месяц, год)
    symbol << "\n~~";       //Сообщение содержит текущее время (секунд от начала суток)
    symbol << "\nSI";       //Индексы спутников (Номера аппаратов во внутренней системе счисления)
    symbol << "\nNN";       //Номера спутников ГЛОНАСС
    symbol << "\nRC";       //Кодовые псевдодальности [C1] по частоте L1 (полное значение в секундах)
    symbol << "\n1R";       //Разница P1-C1 (в секундах)
    symbol << "\n2R";       //Разница P2-C1 (в секундах)
    symbol << "\nCP";       //Разница L1-C1 (в секундах)
    symbol << "\n1P";       //Разница L1P-C1 (в секундах)
    symbol << "\n2P";       //Разница L2-C1 (в секундах)
    symbol << "\nDC";       //Доплеровский сдвиг [D1] по L1
    symbol << "\nD1";       //Доплеровский сдвиг [D1P] по L1P
    symbol << "\nD2";       //Доплеровский сдвиг [D2] по L2
    symbol << "\nEC";       //Шум
    symbol << "\nE1";       //Шум
    symbol << "\nE2";       //Шум
    symbol << "\nNE";       //Эфемериды ГЛОНАСС
    symbol << "\nGE";       //Эфемериды GPS
    symbol << "\nIO";       //Параметры модели ионосферы GPS
    symbol << "\nPV";       //Приближённые координаты
    symbol << "\nUO";       //Сообщение содержит текущие параметры даты GPS-UTC
    symbol << "\nTO";       //Сообщение содержит текущие параметры даты GPS-UTC

    QByteArray Log;         //Временный массив байт используемый для хранения сообщений
    int HexTemp = 0;        //Временная переменная для хранения длины сообщения в 10ричном виде
    bool ok;
    QString StrTemp;        //Временная переменная для хранения длины сообщения в текстовом формате
    int num = 0;            //Переменная для хранения числа сообщений в файле
    int i = 0;              //Переменная для хранения позиции начала сообщения в массиве байт
    QByteArray LengthTemp;
    quint8 check;
    quint8 CRC;

    //ОТЛАДКА

    int ii=0;
    QString ident, dls;
    qint32 dl;
    QByteArray soob, soob1;
    qint32 ch_crc, ch_mes;

    num = data.size();      // My.
    QString name = file->fileName();

    ch_crc=0; ch_mes=0;
    while ( ii < data.length() )
    {
        ++i;
        StrTemp = data.at(ii);  // My.
        ident.clear();
        if ( (data.at(ii) == 10 ) || ( data.at(ii) == 13 ) )        // Номера ASCII симловов \n и \r соответственно
        {
            ii++;
            continue;
        }

        ident = data.mid(ii,2);
        if (ident == "TT")
            qDebug()<<ii;

        dls = data.mid(ii+2,3);
        dl = dls.toInt(&ok,16);
        soob = data.mid(ii+5,dl);
        soob1 = data.mid(ii,dl+5);
        QDataStream dat( soob.right(1) );
        dat >> check;
        //       qDebug() << "check->" << check;
        u1 test [dl+4];
        for (int n = 0; n < dl+4; n++)
        {
            test [n] = soob1.at(n);
            //      qDebug() << n <<test[n];
        }
        CRC = cs(test,dl+4);
        //    qDebug() << "CRC->" << CRC;

        if(check==CRC)
        {
            ch_mes++;
            if (ident == "RD") {ARD << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "~~") {ATT << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "SI") {ASI << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "NN") {ANN << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "RC") {ARC << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "1R") {A1R << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "2R") {A2R << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "CP") {ACP << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "1P") {A1P << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "2P") {A2P << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "DC") {ADC << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "D1") {AD1 << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "D2") {AD2 << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "EC") {AEC << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "E1") {AE1 << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "E2") {AE2 << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "NE") {ANE << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "GE") {AGE << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "IO") {AIO << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "PV") {APV << soob.mid(0,soob.length()-1); ch_mes--;}
            if (ident == "TO") {ATO << soob.mid(0,soob.length()-1); ch_mes--;}
        }
        else ch_crc++;

        ii=ii+5+dl;
    }

    /*
        for(int j=0; j<symbol.length(); j++){
            num = data.count(symbol.at(j));             //Получаем число сообщений данного типа в файле
            i = 0;
            //Вывод длинны сообщений в консоль
//            qDebug()<<num;
            for(int q = 0; q<num; q++){
                i = data.indexOf(symbol.at(j),i+3);     //Получаем положение начала текущего сообщения

                LengthTemp.append(data.at(i+3));
                LengthTemp.append(data.at(i+4));
                LengthTemp.append(data.at(i+5));
                StrTemp = LengthTemp;                   //Получаем длину байт в текстовом виде
                LengthTemp.clear();                     //Очищаем временный массив байт

                //ОТЛАДКА
                StrTemp=data.mid(i+3,3);


                HexTemp = StrTemp.toInt(&ok,16);        //Преобразовываем значение длины сообщения в число
                for(int z = 1; z<5+HexTemp; z++){
                    Log += data.at(i+z);
                }

                LengthTemp.append(data.at(i+5+HexTemp));
                QDataStream dat(LengthTemp);
                LengthTemp.clear();
                dat >> check;
            //    qDebug() << "check->" << check;
                u1 test [HexTemp+4];
                for (int n = 0; n < HexTemp+4; n++){
                    test [n] = data.at(i+n+1);
             //       qDebug() << i+n+1 <<test[n];
                }
                CRC = cs(test,HexTemp+4);
         //       qDebug() << "CRC->" << CRC;

                Log = Log.remove(0,5);
                if(check==CRC){
                if (j==0) ARD << Log;
                if (j==1) ATT << Log;
                if (j==2) ASI << Log;
                if (j==3) ANN << Log;
                if (j==4) ARC << Log;
                if (j==5) A1R << Log;
                if (j==6) A2R << Log;
                if (j==7) ACP << Log;
                if (j==8) A1P << Log;
                if (j==9) A2P << Log;
                if (j==10) ADC << Log;
                if (j==11) AD1 << Log;
                if (j==12) AD2 << Log;
                if (j==13) AEC << Log;
                if (j==14) AE1 << Log;
                if (j==15) AE2 << Log;
                if (j==16) ANE << Log;
                if (j==17) AGE << Log;
                if (j==18) AIO << Log;
                if (j==19) APV << Log;
                if (j==21) ATO << Log;
                }
                Log.clear();
            }
        }
*/
    qDebug() << "jpsReader by created";
    qDebug() << "RD = " << ARD.length();
    qDebug() << "TT = " << ATT.length();
    qDebug() << "SI = " << ASI.length();
    qDebug() << "NN = " << ANN.length();
    qDebug() << "RC = " << ARC.length();
    qDebug() << "1R = " << A1R.length();
    qDebug() << "2R = " << A2R.length();
    qDebug() << "CP = " << ACP.length();
    qDebug() << "1P = " << A1P.length();
    qDebug() << "2P = " << A2P.length();
    qDebug() << "D1 = " << AD1.length();
    qDebug() << "D2 = " << AD2.length();
    qDebug() << "EC = " << AEC.length();
    qDebug() << "E1 = " << AE1.length();
    qDebug() << "E2 = " << AE2.length();
    qDebug() << "NE = " << ANE.length();
    qDebug() << "GE = " << AGE.length();
    qDebug() << "IO = " << AIO.length();
    qDebug() << "PV = " << APV.length();
    qDebug()  << "TO = "  << ATO.length();
    //qDebug() << ch_crc << ch_mes;

    QByteArray Array;
    Array += ARD.at(0).at(1);
    Array += ARD.at(0).at(0);
    QDataStream dat(Array);
    dat >> year;

    numEp = ARD.length();

}   //КОНЕЦ КОНСТРУКТОРА

void jpsReader::jpsReaderTextData(QFile *file)
{
    QByteArray data;                            // Создаем объект класса QByteArray, куда мы будем считывать данные
    file->open(QIODevice::ReadOnly);

    data = file->readAll();                     //ПОЛНОСТЬЮ СЧИТЫВАЕМ ФАЙЛ В МАССИВ БАЙТ
    //qDebug() << data.length();
    file->close();

    QList<QByteArray> symbol;
    symbol << "\nPM";       //Строки с текстовой информацией
    symbol << "\nMF";

    QByteArray Log;         //Временный массив байт используемый для хранения сообщений
    int HexTemp = 0;        //Временная переменная для хранения длины сообщения в 10ричном виде
    bool ok;
    QString StrTemp;        //Временная переменная для хранения длины сообщения в текстовом формате
    int num = 0;            //Переменная для хранения числа сообщений в файле
    int i = 0;              //Переменная для хранения позиции начала сообщения в массиве байт
    QByteArray LengthTemp;

    for(int j=0; j<symbol.length(); j++)
    {
        num = data.count( symbol.at(j) );             //Получаем число сообщений данного типа в файле
        i = 0;

        for(int q = 0; q<num; q++)
        {
            i = data.indexOf(symbol.at(j),i+3);     //Получаем положение начала текущего сообщения
            LengthTemp.append(data.at(i+3));
            LengthTemp.append(data.at(i+4));
            LengthTemp.append(data.at(i+5));
            StrTemp = LengthTemp;                   //Получаем длину байт в текстовом виде
            LengthTemp.clear();                     //Очищаем временный массив байт
            HexTemp = StrTemp.toInt(&ok,16);        //Преобразовываем значение длины сообщения в число

            for(int z = 1; z<5+HexTemp; z++)
            {
                Log += data.at(i+z);
            }

            Log = Log.remove(0,5);
            if (j==0) APM << Log;
            if (j==1) AMF << Log;
            Log.clear();
        }
    }

    qDebug() << "Reader JPS text data";
    qDebug() << "PM = " << APM.length();
    qDebug() << "MF = " << AMF.length();
}

#define ROT_LEFT(val) ((val << lShift) | (val >> rShift))
u1 jpsReader::cs(u1 src[], int count)
{
    u1 res = 0;
    for (int i = 0; i < count; i++){
        res = ROT_LEFT(res) ^ src[i];
    }
    return ROT_LEFT(res);
}

//Метод для получения времени от начала суток по шкале GPST
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Число секунд прошедшее от начала суток
qreal jpsReader::getTimeGPS(int NumEpoch){
    QByteArray Array;
    quint32 temp;

    ///REDACTION: 6.11.2019

    if(NumEpoch < ATT.length() && !ATT.at(NumEpoch).isEmpty()){
        Array += ATT.at(NumEpoch).at(3);
        Array += ATT.at(NumEpoch).at(2);
        Array += ATT.at(NumEpoch).at(1);
        Array += ATT.at(NumEpoch).at(0);
    }
    ///ENDREDACTION

    QDataStream dat(Array);
    dat >> temp;
    qreal time;
    time = temp;
    time = time/1000.0;
    return time;
}

//Метод для получения номера текущего года н.э.
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер года
quint32 jpsReader::getYear(int NumEpoch){
    QByteArray Array;
    quint16 temp;
    QByteArray array = ARD.at(NumEpoch);
    if(!array.isEmpty()){
        Array += ARD.at(NumEpoch).at(1);
        Array += ARD.at(NumEpoch).at(0);
    }
//    Array += ARD.at(NumEpoch).at(1);
//    Array += ARD.at(NumEpoch).at(0);
    QDataStream dat(Array);
    dat >> temp;
    quint32 Year;
    Year = temp;
    return Year;
}

//Метод для получения номера месяца в текущем году
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер месяца в году
quint32 jpsReader::getMonth(int NumEpoch){
    QByteArray Array;
    quint8 temp;
    if(!ARD.at(NumEpoch).isEmpty()){
        Array += ARD.at(NumEpoch).at(2);
    }
    QDataStream dat(Array);
    dat >> temp;
    quint32 Month;
    Month = temp;
    return Month;
}

//Метод для получения номера дня в текущем месяце
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер дня в текущем месяце
quint32 jpsReader::getDay(int NumEpoch){
    QByteArray Array;
    quint8 temp;
    if(!ARD.at(NumEpoch).isEmpty()){
        Array += ARD.at(NumEpoch).at(3);
    }

    QDataStream dat(Array);
    dat >> temp;
    quint32 Day;
    Day = temp;
    return Day;
}

//Метод для получения номера часа в текущих сутках
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер часа в текущих сутках
quint32 jpsReader::getHour(int NumEpoch){
    qreal temp;

    temp = getTimeGPS(NumEpoch);

    temp = temp/3600;
    temp = qFloor(temp);
    quint32 Hour;
    Hour = temp;
    return Hour;
}

//Метод для получения номера минуты в текущем часу
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер минуты в текущем часу
quint32 jpsReader::getMinute(int NumEpoch){
    quint32 temp;
    temp = getTimeGPS(NumEpoch);
    temp = temp%3600;
    temp = temp/60;
    quint32 Minute = temp;
    return Minute;
}

//Метод для получения номера секунды в текущей минуте
//Входные данные:
//NumEpoch - порядковый номер эпохи измерений в файле .L/.jps
//Выходные данные:
//Номер секунды в текущей минуте
qreal jpsReader::getSecond(int NumEpoch){
    quint32 Minute;
    Minute = getTimeGPS(NumEpoch);
    Minute = Minute%3600;
    Minute = Minute/60;
    qreal temp;
    qreal Hour;
    qreal GPSSec = getTimeGPS(NumEpoch);
    temp = GPSSec/3600;
    Hour = qFloor(temp);
    qreal Second;
    Second = GPSSec - Hour*3600.0 - Minute*60.0;
    return Second;
}

//Метод для получения числа спутников в эпохе
quint32 jpsReader::getNumSat(int NumEpoch){
    quint32 temp = ASI.at(NumEpoch).length();
    return temp;
}

//Метод для получения числа спутников ГЛОНАСС в эпохе
quint32 jpsReader::getNumSatGLO(int NumEpoch){
    quint32 temp = ANN.at(NumEpoch).length();
    return temp;
}

//Метод для получения числа спутников GPS в эпохе
quint32 jpsReader::getNumSatGPS(int NumEpoch){
    quint32 temp = ASI.at(NumEpoch).length();
    quint32 GLO = getNumSatGLO(NumEpoch);
    temp=temp-GLO;
    return temp;
}

//Метод для получения номера спутника
quint32 jpsReader::getCurrentSatNum (int NumEpoch, int Current){
    QByteArray Array;
    quint8 temp;
    Array += ASI.at(NumEpoch).at(Current);
    QDataStream dat(Array);
    dat >> temp;
    quint32 CurrentSatNum;
    CurrentSatNum = temp;
    return CurrentSatNum;
}

quint32 jpsReader::getCurrentSatGLO (int NumEpoch, int Current){
    QByteArray Array;
    quint8 temp;
    Array += ANN.at(NumEpoch).at(Current);
    QDataStream dat(Array);
    dat >> temp;
    quint32 CurrentSatNum;
    CurrentSatNum = temp;
    if(CurrentSatNum == 255){
        qDebug()<<"X";
    }
    return CurrentSatNum;
}

qreal jpsReader::getDeltaT(int NumEpoch){
QByteArray Array;
qreal temp;
if(NumEpoch < ATO.length() && !ATO.at(NumEpoch).isEmpty()){
    Array += ATO.at(NumEpoch).at(7);
    Array += ATO.at(NumEpoch).at(6);
    Array += ATO.at(NumEpoch).at(5);
    Array += ATO.at(NumEpoch).at(4);
    Array += ATO.at(NumEpoch).at(3);
    Array += ATO.at(NumEpoch).at(2);
    Array += ATO.at(NumEpoch).at(1);
    Array += ATO.at(NumEpoch).at(0);
}
QDataStream dat(Array);
dat >> temp;
return temp;
}


//Метод для получения кодовых псевдодальностей по частоте L1 [С1]
//[C1] - опорное значение для получения всех псевдодальностей
//Возвращает значение [С1] в метрах
qreal jpsReader::getC1(int NumEpoch, int Current){
    int a = Current*8;
    QByteArray Array;
    qreal temp;
    Array += ARC.at(NumEpoch).at(7+a);
    Array += ARC.at(NumEpoch).at(6+a);
    Array += ARC.at(NumEpoch).at(5+a);
    Array += ARC.at(NumEpoch).at(4+a);
    Array += ARC.at(NumEpoch).at(3+a);
    Array += ARC.at(NumEpoch).at(2+a);
    Array += ARC.at(NumEpoch).at(1+a);
    Array += ARC.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    temp = temp*299792458.0;
    //qDebug() << temp;
    return temp;
}

//Метод для получения кодовых псевдодальностей по частоте L1 [С1]
//Возвраящает значение [С1] в секундах (используется для вычисления фазовых псевдодальностей)
qreal jpsReader::getC1Sec(int NumEpoch, int Current){
    int a = Current*8;
    QByteArray Array;
    qreal temp;
    Array += ARC.at(NumEpoch).at(7+a);
    Array += ARC.at(NumEpoch).at(6+a);
    Array += ARC.at(NumEpoch).at(5+a);
    Array += ARC.at(NumEpoch).at(4+a);
    Array += ARC.at(NumEpoch).at(3+a);
    Array += ARC.at(NumEpoch).at(2+a);
    Array += ARC.at(NumEpoch).at(1+a);
    Array += ARC.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

//Доплеровский сдвиг по L1 [D1]
qreal jpsReader::getD1(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    qint32 temp;
    Array += ADC.at(NumEpoch).at(3+a);
    Array += ADC.at(NumEpoch).at(2+a);
    Array += ADC.at(NumEpoch).at(1+a);
    Array += ADC.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    qreal D1 = -temp*0.0001;
    return D1;
}

//Доплеровский сдвиг по L1P [D1P]
qreal jpsReader::getD1P(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    qint32 temp;
    Array += AD1.at(NumEpoch).at(3+a);
    Array += AD1.at(NumEpoch).at(2+a);
    Array += AD1.at(NumEpoch).at(1+a);
    Array += AD1.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    qreal D1 = -temp*0.0001;
    return D1;
}

//Доплеровский сдвиг по L2 [D2]
qreal jpsReader::getD2(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    qint32 temp;
    try{
        if(a>=AD2.at(NumEpoch).length()){
            throw 0;
        }
    Array += AD2.at(NumEpoch).at(3+a);
    Array += AD2.at(NumEpoch).at(2+a);
    Array += AD2.at(NumEpoch).at(1+a);
    Array += AD2.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat >> temp;
    qreal D2 = -temp*0.0001;
    return D2;
}

//Кодовая псевдодальность P2 [P2]
qreal jpsReader::getP2(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A2R.at(NumEpoch).length()){
            throw 0;
        }
        Array += A2R.at(NumEpoch).at(3+a);
        Array += A2R.at(NumEpoch).at(2+a);
        Array += A2R.at(NumEpoch).at(1+a);
        Array += A2R.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal R2 = getC1(NumEpoch, Current) + temp*299792458.0;
    return R2;
}

//Кодовая псевдодальность P1 [P1]
qreal jpsReader::getP1(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A1R.at(NumEpoch).length()){
            throw 0;
        }
    Array += A1R.at(NumEpoch).at(3+a);
    Array += A1R.at(NumEpoch).at(2+a);
    Array += A1R.at(NumEpoch).at(1+a);
    Array += A1R.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal R1 = getC1(NumEpoch, Current) + temp*299792458.0;
    return R1;
}

//Фазовая псевдодальность L1
qreal jpsReader::getL1(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=ACP.at(NumEpoch).length()){
            throw 0;
        }
    Array += ACP.at(NumEpoch).at(3+a);
    Array += ACP.at(NumEpoch).at(2+a);
    Array += ACP.at(NumEpoch).at(1+a);
    Array += ACP.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L1 = (getC1Sec(NumEpoch, Current) + temp)*1575420000.0;
    return L1;
}

//Фазовая псевдодальность L1P
qreal jpsReader::getL1P(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A1P.at(NumEpoch).length()){
            throw 0;
        }
    Array += A1P.at(NumEpoch).at(3+a);
    Array += A1P.at(NumEpoch).at(2+a);
    Array += A1P.at(NumEpoch).at(1+a);
    Array += A1P.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L1P = (getC1Sec(NumEpoch, Current) + temp)*1575420000.0;
    return L1P;
}

//Фазовая псевдодальность L2
qreal jpsReader::getL2(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A2P.at(NumEpoch).length()){
            throw 0;
        }
    Array += A2P.at(NumEpoch).at(3+a);
    Array += A2P.at(NumEpoch).at(2+a);
    Array += A2P.at(NumEpoch).at(1+a);
    Array += A2P.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L2 = (getC1Sec(NumEpoch, Current) + temp)*1227600000.0;
    return L2;
}

//Фазовая псевдодальность L1 для ГЛОНАСС
qreal jpsReader::getL1GLO(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=ACP.at(NumEpoch).length()){
            throw 0;
        }
    Array += ACP.at(NumEpoch).at(3+a);
    Array += ACP.at(NumEpoch).at(2+a);
    Array += ACP.at(NumEpoch).at(1+a);
    Array += ACP.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L1 = (getC1Sec(NumEpoch, Current) + temp);
    return L1;
}

//Фазовая псевдодальность L1P для ГЛОНАСС
qreal jpsReader::getL1PGLO(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A1P.at(NumEpoch).length()){
            throw 0;
        }
    Array += A1P.at(NumEpoch).at(3+a);
    Array += A1P.at(NumEpoch).at(2+a);
    Array += A1P.at(NumEpoch).at(1+a);
    Array += A1P.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L1P = (getC1Sec(NumEpoch, Current) + temp);
    return L1P;
}

//Фазовая псевдодальность L2 для ГЛОНАСС
qreal jpsReader::getL2GLO(int NumEpoch, int Current){
    int a = Current*4;
    QByteArray Array;
    float temp;
    try{
        if(a>=A2P.at(NumEpoch).length()){
            throw 0;
        }
    Array += A2P.at(NumEpoch).at(3+a);
    Array += A2P.at(NumEpoch).at(2+a);
    Array += A2P.at(NumEpoch).at(1+a);
    Array += A2P.at(NumEpoch).at(0+a);
    }
    catch(int e){
        return 0;
    }
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal L2 = (getC1Sec(NumEpoch, Current) + temp);
    return L2;
}

//Шум
quint32 jpsReader::getEC (int NumEpoch, int Current){
    int a = Current;
    quint8 temp;
    quint32 res;
    QByteArray Array;
    Array += AEC.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    res = temp;
    return res;
}

quint32 jpsReader::getE1 (int NumEpoch, int Current){
    int a = Current;
    quint8 temp;
    quint32 res;
    QByteArray Array;
    Array += AE1.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    res = temp;
    return res;
}

quint32 jpsReader::getE2 (int NumEpoch, int Current){
    int a = Current;
    quint8 temp;
    quint32 res;
    QByteArray Array;
    Array += AE1.at(NumEpoch).at(0+a);
    QDataStream dat(Array);
    dat >> temp;
    res = temp;
    return res;
}

//====== ЭФЕМЕРИДЫ ГЛОНАСС ======
//Метод получения номера спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
quint32 jpsReader::getSatNum(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    quint8 slot;
    Array += ANE.at(Num).at(0);
    QDataStream dat(Array);
    dat >> slot;
    quint32 res = slot;
    return res;
}
//Метод получения литеры спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qint32 jpsReader::getSatLit(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qint8 slot;
    Array += ANE.at(Num).at(1);
    QDataStream dat(Array);
    dat >> slot;
    qint32 res = slot;
    return res;
}
//Метод получения времени сообщения в секундах от начала текущих суток UTC (0 <= tk < 86400 c)
qint32 jpsReader::getTk(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qint32 temp;
    Array += ANE.at(Num).at(7);
    Array += ANE.at(Num).at(6);
    Array += ANE.at(Num).at(5);
    Array += ANE.at(Num).at(4);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}
//Метод получения координаты Х (км.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getXcord(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qreal temp;
    Array += ANE.at(Num).at(22);
    Array += ANE.at(Num).at(21);
    Array += ANE.at(Num).at(20);
    Array += ANE.at(Num).at(19);
    Array += ANE.at(Num).at(18);
    Array += ANE.at(Num).at(17);
    Array += ANE.at(Num).at(16);
    Array += ANE.at(Num).at(15);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}
//Метод получения координаты Y (км.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getYcord(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qreal temp;
    Array += ANE.at(Num).at(30);
    Array += ANE.at(Num).at(29);
    Array += ANE.at(Num).at(28);
    Array += ANE.at(Num).at(27);
    Array += ANE.at(Num).at(26);
    Array += ANE.at(Num).at(25);
    Array += ANE.at(Num).at(24);
    Array += ANE.at(Num).at(23);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}
//Метод получения координаты Z (км.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getZcord(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qreal temp;
    Array += ANE.at(Num).at(38);
    Array += ANE.at(Num).at(37);
    Array += ANE.at(Num).at(36);
    Array += ANE.at(Num).at(35);
    Array += ANE.at(Num).at(34);
    Array += ANE.at(Num).at(33);
    Array += ANE.at(Num).at(32);
    Array += ANE.at(Num).at(31);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}
//Метод получения составляющей скорости по оси Х (км./c.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getXspeed(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(42);
    Array += ANE.at(Num).at(41);
    Array += ANE.at(Num).at(40);
    Array += ANE.at(Num).at(39);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}
//Метод получения составляющей скорости по оси Y (км./c.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getYspeed(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(46);
    Array += ANE.at(Num).at(45);
    Array += ANE.at(Num).at(44);
    Array += ANE.at(Num).at(43);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}
//Метод получения составляющей скорости по оси Z (км./c.) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getZspeed(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(50);
    Array += ANE.at(Num).at(49);
    Array += ANE.at(Num).at(48);
    Array += ANE.at(Num).at(47);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}
//Метод получения составляющей ускорения по оси X (км./c.^2) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getXacc(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(54);
    Array += ANE.at(Num).at(53);
    Array += ANE.at(Num).at(52);
    Array += ANE.at(Num).at(51);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}
//Метод получения составляющей ускорения по оси Y (км./c.^2) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getYacc(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(58);
    Array += ANE.at(Num).at(57);
    Array += ANE.at(Num).at(56);
    Array += ANE.at(Num).at(55);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}
//Метод получения составляющей ускорения по оси Z (км./c.^2) в СК ПЗ-90 для спутника ГЛОНАСС к которому относится блок данных с индексом "Num"
qreal jpsReader::getZacc(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(62);
    Array += ANE.at(Num).at(61);
    Array += ANE.at(Num).at(60);
    Array += ANE.at(Num).at(59);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getTimeCorr1(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    qreal temp;
    Array += ANE.at(Num).at(70);
    Array += ANE.at(Num).at(69);
    Array += ANE.at(Num).at(68);
    Array += ANE.at(Num).at(67);
    Array += ANE.at(Num).at(66);
    Array += ANE.at(Num).at(65);
    Array += ANE.at(Num).at(64);
    Array += ANE.at(Num).at(63);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getTauN(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(74);
    Array += ANE.at(Num).at(73);
    Array += ANE.at(Num).at(72);
    Array += ANE.at(Num).at(71);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getGammaN(int Num){
    if(Num>ANE.length()){
        qDebug() << "getTauN(-1)";
    }
    QByteArray Array;
    float temp;
    Array += ANE.at(Num).at(78);
    Array += ANE.at(Num).at(77);
    Array += ANE.at(Num).at(76);
    Array += ANE.at(Num).at(75);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

quint32 jpsReader::getIsOk(int Num){
    QByteArray Array;
    QBitArray Temp;
    qint32 res;
    Array += ANE.at(Num).at(12);
    Temp = toBitArray(Array);
    res = Temp[0];
    return res;
}

quint32 jpsReader::getAgeOfInf(int Num){
    QByteArray Array;
    quint8 slot;
    Array += ANE.at(Num).at(13);
    QDataStream dat(Array);
    dat >> slot;
    quint32 res = slot;
    return res;
}

qint32 jpsReader::getNumYearGLO(int Num){
    QByteArray Array;
    qint16 temp;
    Array += ANE.at(Num).at(3);
    Array += ANE.at(Num).at(2);
    QDataStream dat(Array);
    dat >> temp;
    quint32 res = temp;
    return res;
}

qint32 jpsReader::getNumSecondGLO(int Num){
    QByteArray Array;
    qint32 temp;
    Array += ANE.at(Num).at(11);
    Array += ANE.at(Num).at(10);
    Array += ANE.at(Num).at(9);
    Array += ANE.at(Num).at(8);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

//====== ЭФЕМЕРИДЫ GPS ======
//Номер спутника GPS
quint32 jpsReader::getSatPRN(int Num){
    QByteArray Array;
    quint8 slot;
    Array += AGE.at(Num).at(0);
    QDataStream dat(Array);
    dat >> slot;
    quint32 res = slot;
    return res;
}
//Время в секундах от начала недели
quint32 jpsReader::getGPSt0e(int Num){
    QByteArray Array;
    quint32 temp;
    Array += AGE.at(Num).at(4);
    Array += AGE.at(Num).at(3);
    Array += AGE.at(Num).at(2);
    Array += AGE.at(Num).at(1);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

qint32 jpsReader::getIODC(int Num){
    QByteArray Array;
    qint16 temp;
    Array += AGE.at(Num).at(7);
    Array += AGE.at(Num).at(6);
    QDataStream dat(Array);
    dat >> temp;
    qint32 res = temp;
    return res;
}

qint32 jpsReader::getToC(int Num){
    QByteArray Array;
    quint32 temp;
    Array += AGE.at(Num).at(11);
    Array += AGE.at(Num).at(10);
    Array += AGE.at(Num).at(9);
    Array += AGE.at(Num).at(8);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

qint32 jpsReader::getGPSWeek(int Num){
    QByteArray Array;
    qint16 temp;
    Array += AGE.at(Num).at(15);
    Array += AGE.at(Num).at(14);
    QDataStream dat(Array);
    dat >> temp;
    qint32 res = temp;
    return res;
}

qreal jpsReader::getTGD(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(19);
    Array += AGE.at(Num).at(18);
    Array += AGE.at(Num).at(17);
    Array += AGE.at(Num).at(16);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getSVClockDriftRate(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(23);
    Array += AGE.at(Num).at(22);
    Array += AGE.at(Num).at(21);
    Array += AGE.at(Num).at(20);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getSVClockDrift(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(27);
    Array += AGE.at(Num).at(26);
    Array += AGE.at(Num).at(25);
    Array += AGE.at(Num).at(24);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getSVClockBias(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(31);
    Array += AGE.at(Num).at(30);
    Array += AGE.at(Num).at(29);
    Array += AGE.at(Num).at(28);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qint32 jpsReader::getToE(int Num){
    QByteArray Array;
    quint32 temp;
    Array += AGE.at(Num).at(35);
    Array += AGE.at(Num).at(34);
    Array += AGE.at(Num).at(33);
    Array += AGE.at(Num).at(32);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

qint32 jpsReader::getIODE(int Num){
    QByteArray Array;
    qint16 temp;
    Array += AGE.at(Num).at(37);
    Array += AGE.at(Num).at(36);
    QDataStream dat(Array);
    dat >> temp;
    qint32 res = temp;
    return res;
}

qreal jpsReader::getSqrtA (int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(45);
    Array += AGE.at(Num).at(44);
    Array += AGE.at(Num).at(43);
    Array += AGE.at(Num).at(42);
    Array += AGE.at(Num).at(41);
    Array += AGE.at(Num).at(40);
    Array += AGE.at(Num).at(39);
    Array += AGE.at(Num).at(38);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getEccentricity(int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(53);
    Array += AGE.at(Num).at(52);
    Array += AGE.at(Num).at(51);
    Array += AGE.at(Num).at(50);
    Array += AGE.at(Num).at(49);
    Array += AGE.at(Num).at(48);
    Array += AGE.at(Num).at(47);
    Array += AGE.at(Num).at(46);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getM0(int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(61);
    Array += AGE.at(Num).at(60);
    Array += AGE.at(Num).at(59);
    Array += AGE.at(Num).at(58);
    Array += AGE.at(Num).at(57);
    Array += AGE.at(Num).at(56);
    Array += AGE.at(Num).at(55);
    Array += AGE.at(Num).at(54);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getOmega0(int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(69);
    Array += AGE.at(Num).at(68);
    Array += AGE.at(Num).at(67);
    Array += AGE.at(Num).at(66);
    Array += AGE.at(Num).at(65);
    Array += AGE.at(Num).at(64);
    Array += AGE.at(Num).at(63);
    Array += AGE.at(Num).at(62);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::geti0(int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(77);
    Array += AGE.at(Num).at(76);
    Array += AGE.at(Num).at(75);
    Array += AGE.at(Num).at(74);
    Array += AGE.at(Num).at(73);
    Array += AGE.at(Num).at(72);
    Array += AGE.at(Num).at(71);
    Array += AGE.at(Num).at(70);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getOmega(int Num){
    QByteArray Array;
    qreal temp;
    Array += AGE.at(Num).at(85);
    Array += AGE.at(Num).at(84);
    Array += AGE.at(Num).at(83);
    Array += AGE.at(Num).at(82);
    Array += AGE.at(Num).at(81);
    Array += AGE.at(Num).at(80);
    Array += AGE.at(Num).at(79);
    Array += AGE.at(Num).at(78);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

qreal jpsReader::getDeltaN(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(89);
    Array += AGE.at(Num).at(88);
    Array += AGE.at(Num).at(87);
    Array += AGE.at(Num).at(86);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getOmegaDot(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(93);
    Array += AGE.at(Num).at(92);
    Array += AGE.at(Num).at(91);
    Array += AGE.at(Num).at(90);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIDOT(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(97);
    Array += AGE.at(Num).at(96);
    Array += AGE.at(Num).at(95);
    Array += AGE.at(Num).at(94);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCrc(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(101);
    Array += AGE.at(Num).at(100);
    Array += AGE.at(Num).at(99);
    Array += AGE.at(Num).at(98);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCrs(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(105);
    Array += AGE.at(Num).at(104);
    Array += AGE.at(Num).at(103);
    Array += AGE.at(Num).at(102);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCuc(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(109);
    Array += AGE.at(Num).at(108);
    Array += AGE.at(Num).at(107);
    Array += AGE.at(Num).at(106);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCus(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(113);
    Array += AGE.at(Num).at(112);
    Array += AGE.at(Num).at(111);
    Array += AGE.at(Num).at(110);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCic(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(117);
    Array += AGE.at(Num).at(116);
    Array += AGE.at(Num).at(115);
    Array += AGE.at(Num).at(114);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getCis(int Num){
    QByteArray Array;
    float temp;
    Array += AGE.at(Num).at(121);
    Array += AGE.at(Num).at(120);
    Array += AGE.at(Num).at(119);
    Array += AGE.at(Num).at(118);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qint32 jpsReader::getCodesL2(int Num){
    QByteArray Array;
    QBitArray Temp;
    qint32 res;
    qint32 bit2;
    qint32 bit3;
    Array += AGE.at(Num).at(5);
    Temp = toBitArray(Array);
    bit2 = Temp[3];
    bit3 = Temp[2];
    if((bit2==0)&(bit3==0)){
        res = 0;
    }
    if((bit2==0)&(bit3==1)){
        res = 1;
    }
    if((bit2==1)&(bit3==0)){
        res = 10;
    }
    if((bit2==1)&(bit3==1)){
        res = 11;
    }
    return res;
}

qint32 jpsReader::getSVaccuracy (int Num){
    QByteArray Array;
    qint8 temp;
    qint32 res;
    Array += AGE.at(Num).at(12);
    QDataStream dat(Array);
    dat >> temp;
    res =temp;
    return res;
}

qint32 jpsReader::getL2PdataFlag (int Num){
    QByteArray Array;
    QBitArray Temp;
    qint32 res;
    Array += AGE.at(Num).at(5);
    Temp = toBitArray(Array);
    res = Temp[1];
    return res;
}

qint32 jpsReader::getFitInterval (int Num){
    QByteArray Array;
    QBitArray Temp;
    qint32 res;
    Array += AGE.at(Num).at(5);
    Temp = toBitArray(Array);
    res = Temp[0];
    return res;
}

qint32 jpsReader::getSVhealth (int Num){
    QByteArray Array;
    quint8 temp;
    qint32 res;
    Array += AGE.at(Num).at(13);
    QDataStream dat(Array);
    dat >> temp;
    res =temp;
    return res;
}

//======ПАРАМЕТРЫ МОДЕЛИ ИОНОСФЕРЫ GPS======
qint32 jpsReader::getIonW(int Num){
    QByteArray Array;
    qint16 temp;
    qint32 res;
    Array += AIO.at(Num).at(5);
    Array += AIO.at(Num).at(4);
    QDataStream dat(Array);
    dat >> temp;
    res =temp;
    return res;
}

quint32 jpsReader::getIonT(int Num){
    QByteArray Array;
    qint32 temp;
    Array += AIO.at(Num).at(3);
    Array += AIO.at(Num).at(2);
    Array += AIO.at(Num).at(1);
    Array += AIO.at(Num).at(0);
    QDataStream dat(Array);
    dat >> temp;
    return temp;
}

qreal jpsReader::getIonAlpha0(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(9);
    Array += AIO.at(Num).at(8);
    Array += AIO.at(Num).at(7);
    Array += AIO.at(Num).at(6);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonAlpha1(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(13);
    Array += AIO.at(Num).at(12);
    Array += AIO.at(Num).at(11);
    Array += AIO.at(Num).at(10);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonAlpha2(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(17);
    Array += AIO.at(Num).at(16);
    Array += AIO.at(Num).at(15);
    Array += AIO.at(Num).at(14);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonAlpha3(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(21);
    Array += AIO.at(Num).at(20);
    Array += AIO.at(Num).at(19);
    Array += AIO.at(Num).at(18);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonBeta0(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(25);
    Array += AIO.at(Num).at(24);
    Array += AIO.at(Num).at(23);
    Array += AIO.at(Num).at(22);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonBeta1(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(29);
    Array += AIO.at(Num).at(28);
    Array += AIO.at(Num).at(27);
    Array += AIO.at(Num).at(26);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonBeta2(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(33);
    Array += AIO.at(Num).at(32);
    Array += AIO.at(Num).at(31);
    Array += AIO.at(Num).at(30);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    return res;
}

qreal jpsReader::getIonBeta3(int Num){
    QByteArray Array;
    float temp;
    Array += AIO.at(Num).at(37);
    Array += AIO.at(Num).at(36);
    Array += AIO.at(Num).at(35);
    Array += AIO.at(Num).at(34);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dat >> temp;
    qreal res = temp;
    qDebug() << temp;
    return res;
}

//X
qreal jpsReader::getApproxX(int Num)
{
    if( APV.isEmpty() )
    {
        return 0;
    }

    QByteArray Array;
    qreal temp;
    Array += APV.at(Num).at(7);
    Array += APV.at(Num).at(6);
    Array += APV.at(Num).at(5);
    Array += APV.at(Num).at(4);
    Array += APV.at(Num).at(3);
    Array += APV.at(Num).at(2);
    Array += APV.at(Num).at(1);
    Array += APV.at(Num).at(0);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

//Y
qreal jpsReader::getApproxY(int Num)
{
    if( APV.isEmpty() )
    {
        return 0;
    }

    QByteArray Array;
    qreal temp;
    Array += APV.at(Num).at(15);
    Array += APV.at(Num).at(14);
    Array += APV.at(Num).at(13);
    Array += APV.at(Num).at(12);
    Array += APV.at(Num).at(11);
    Array += APV.at(Num).at(10);
    Array += APV.at(Num).at(9);
    Array += APV.at(Num).at(8);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

//Z
qreal jpsReader::getApproxZ(int Num)
{
    if( APV.isEmpty() )
    {
        return 0;
    }

    QByteArray Array;
    qreal temp;
    Array += APV.at(Num).at(23);
    Array += APV.at(Num).at(22);
    Array += APV.at(Num).at(21);
    Array += APV.at(Num).at(20);
    Array += APV.at(Num).at(19);
    Array += APV.at(Num).at(18);
    Array += APV.at(Num).at(17);
    Array += APV.at(Num).at(16);
    QDataStream dat(Array);
    dat.setFloatingPointPrecision(QDataStream::DoublePrecision);
    dat >> temp;
    return temp;
}

