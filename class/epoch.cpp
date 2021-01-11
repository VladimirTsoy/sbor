#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include <epoch.h>
#include <QStringList>
#include "formnumlib.h"
#include "converterjps.h"
#include "rinexreader.h"

using namespace formNum;
int Epoch::lit[40];


Epoch::~Epoch() {
    if(alist != nullptr)
        delete alist;
}

//конструктор класса Эпоха (Измерений)
Epoch::Epoch(jpsReader *Reader, int Num){
    int NumSat = Reader->getNumSat(Num);

    YearEph = Reader->getYear(Num);
    MonthEph = Reader->getMonth(Num);
    DayEph = Reader->getDay(Num);
    HourEph = Reader->getHour(Num);
    MinEph = Reader->getMinute(Num);
    SecEph = Reader->getSecond(Num);
    ReceiverClockOffset = Reader->getDeltaT(Num);
    isLfile = true;

    mesOn[0]  = true; //Измерение C1 (C1C) GPS
    mesOn[1]  = true; //Измерение P1 (C1P) GPS
    mesOn[2]  = true; //Измерение L1 (L1C) GPS
    mesOn[3]  = true; //Измерение D1 (D1C) GPS
    mesOn[4]  = true; //Измерение S1 (S1C) GPS
    mesOn[5]  = true; //Измерение P2 (C2P) GPS
    mesOn[6]  = true; //Измерение L2 (L2P) GPS
    mesOn[7]  = true; //Измерение D2 (D2P) GPS
    mesOn[8]  = true; //Измерение S2 (S2P) GPS
    mesOn[9]  = true; //Измерение C1 (C1C) ГЛОНАСС
    mesOn[10] = true; //Измерение P1 (C1P) ГЛОНАСС
    mesOn[11] = true; //Измерение L1 (L1C) ГЛОНАСС
    mesOn[12] = true; //Измерение D1 (D1C) ГЛОНАСС
    mesOn[13] = true; //Измерение S1 (S1C) ГЛОНАСС
    mesOn[14] = true; //Измерение P2 (C2P) ГЛОНАСС
    mesOn[15] = true; //Измерение L2 (L2P) ГЛОНАСС
    mesOn[16] = true; //Измерение D2 (D2P) ГЛОНАСС
    mesOn[17] = true; //Измерение S2 (S2P) ГЛОНАСС
    //расширенные типы измерений для версии 3
    mesOn[18]  = true; //Измерение L1P (L1P) GPS
    mesOn[19]  = true; //Измерение D1P (D1P) GPS
    mesOn[20]  = true; //Измерение S1P (D1P) GPS
    mesOn[21]  = false; //Измерение C2C (D1P) GPS
    mesOn[22]  = true; //Измерение L1P (L1P) ГЛОНАСС
    mesOn[23]  = true; //Измерение D1P (D1P) ГЛОНАСС
    mesOn[24]  = true; //Измерение S1P (S1P) ГЛОНАСС
    mesOn[25]  = false; //Измерение C2C (C2P) ГЛОНАСС
    //доп типы измерений для 5ой частоты
    mesOn[26]  = false; //Измерение C5 (C5C) GPS
    mesOn[27]  = false; //Измерение P5 (C5P) GPS
    mesOn[28]  = false; //Измерение L5 (L5C) GPS
    mesOn[29]  = false; //Измерение D5 (D5C) GPS
    mesOn[30]  = false; //Измерение S5 (S5C) GPS
    mesOn[31]  = false; //Измерение C5 (C5C) ГЛОНАСС
    mesOn[32]  = false; //Измерение P5 (C5P) ГЛОНАСС
    mesOn[33]  = false; //Измерение L5 (L5C) ГЛОНАСС
    mesOn[34]  = false; //Измерение D5 (D5C) ГЛОНАСС
    mesOn[35]  = false; //Измерение S5 (S5C) ГЛОНАСС


    if(Num==0){
        int NumSatGlo = Reader->getNumSatGLO(0);
        for(int z = 0; z<NumSatGlo; z++){
        }
    }

    if(Num==0){
        for(int z = 0; z<NumSat; z++){
        }
    }

    int z = 0;
    SatGPS *GPS;
    SatGLO *GLO;
    for(int i=0;i<NumSat;i++){
        int CurrentSatNum = Reader->getCurrentSatNum(Num, i);
        if(CurrentSatNum<38){
        GPS = new SatGPS(CurrentSatNum,
                         Reader->getC1(Num,i),
                         Reader->getP1(Num,i),
                         0,
                         Reader->getP2(Num,i),
                         0,
                         0,
                         Reader->getL1(Num,i),
                         Reader->getL1P(Num,i),
                         Reader->getL2(Num,i),
                         0,
                         Reader->getD1(Num,i),
                         Reader->getD1P(Num,i),
                         Reader->getD2(Num,i),
                         0,
                         Reader->getEC(Num,i),
                         Reader->getE1(Num,i),
                         Reader->getE2(Num,i),
                         0
                         );
        ArraySatGPS.append(GPS);
        }       
        if(CurrentSatNum>37){
        //qDebug()<< Reader->getCurrentSatGLO(Num,CurrentSatNum-37)<<" #"<<i;
            quint32 litRead = Reader->getCurrentSatGLO(Num,z);
            if(litRead!=255){
                GLO = new SatGLO(Reader->getCurrentSatGLO(Num,z),
                                 Reader->getC1(Num,i),
                                 Reader->getP1(Num,i),
                                 0,
                                 Reader->getP2(Num,i),
                                 0,
                                 0,
                                 Reader->getL1GLO(Num,i)*(1602000000.0+562500.0*lit[Reader->getCurrentSatGLO(Num,z)]),
                                 Reader->getL1PGLO(Num,i)*(1602000000.0+562500.0*lit[Reader->getCurrentSatGLO(Num,z)]),
                                 Reader->getL2GLO(Num,i)*(1246000000.0+437500.0*lit[Reader->getCurrentSatGLO(Num,z)]),
                                 0,
                                 Reader->getD1(Num,i),
                                 Reader->getD1P(Num,i),
                                 Reader->getD2(Num,i),
                                 0,
                                 lit[CurrentSatNum-37],
                                 Reader->getEC(Num,i),
                                 Reader->getE1(Num,i),
                                 Reader->getE2(Num,i),
                                 0
                                 );
                ArraySatGLO.append(GLO);
                z++;
            }
        }
    }
}       //Конец конструктора

Epoch::Epoch(QStringList *AList,
             quint16 NumbInOrderTypepsObs[15],
             quint16 NumbInOrderTypepsObsGPS[25],
             quint16 NumbInOrderTypepsObsGLO[17],
             bool TypesOfObsCheck[15],
             bool TypesOfObsCheckGPS[34],
             bool TypesOfObsCheckGLO[17],
             int version,QString NofTypes) : alist{AList}
{   
    //Инициализация переменно хранящей код ошибки
    errorCode = 0;

    if (version == 211){
    //переменные для хранения эпохи(момента времени) в формате QString
    QString YearEpoch;
    QString MonthEpoch;
    QString DayEpoch;
    QString HourEpoch;
    QString MinuteEpoch;
    QString SecondsEpoch;

    // =====================================================================
    // | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14
    // | N  | C1 | P1 | L1 | D1 | S1 | C2 | P2 | L2 | D2 | S2 | C5 | L5 | D5 | S5
    // =====================================================================
    mesOn[0]  = TypesOfObsCheck[1];  //Измерение C1 GPS
    mesOn[1]  = TypesOfObsCheck[2];  //Измерение P1 GPS
    mesOn[2]  = TypesOfObsCheck[3];  //Измерение L1 GPS
    mesOn[3]  = TypesOfObsCheck[4];  //Измерение D1 GPS
    mesOn[4]  = TypesOfObsCheck[5];  //Измерение S1 GPS
    mesOn[5]  = TypesOfObsCheck[7];  //Измерение P2 GPS
    mesOn[6]  = TypesOfObsCheck[8];  //Измерение L2 GPS
    mesOn[7]  = TypesOfObsCheck[9];  //Измерение D2 GPS
    mesOn[8]  = TypesOfObsCheck[10]; //Измерение S2 GPS
    mesOn[9]  = TypesOfObsCheck[1];  //Измерение C1 ГЛОНАСС
    mesOn[10] = TypesOfObsCheck[2];  //Измерение P1 ГЛОНАСС
    mesOn[11] = TypesOfObsCheck[3];  //Измерение L1 ГЛОНАСС
    mesOn[12] = TypesOfObsCheck[4];  //Измерение D1 ГЛОНАСС
    mesOn[13] = TypesOfObsCheck[5];  //Измерение S1 ГЛОНАСС
    mesOn[14] = TypesOfObsCheck[7];  //Измерение P2 ГЛОНАСС
    mesOn[15] = TypesOfObsCheck[8];  //Измерение L2 ГЛОНАСС
    mesOn[16] = TypesOfObsCheck[9];  //Измерение D2 ГЛОНАСС
    mesOn[17] = TypesOfObsCheck[10]; //Измерение S2 ГЛОНАСС

    mesOn[18]  = false;               //Измерение L1P (L1P) GPS
    mesOn[19]  = false;               //Измерение D1P (D1P) GPS
    mesOn[20]  = false;               //Измерение S1P (D1P) GPS
    mesOn[21]  = TypesOfObsCheck[6]; //Измерение C2C (D1P) GPS
    mesOn[22]  = false;               //Измерение L1P (L1P) ГЛОНАСС
    mesOn[23]  = false;               //Измерение D1P (D1P) ГЛОНАСС
    mesOn[24]  = false;               //Измерение S1P (S1P) ГЛОНАСС
    mesOn[25]  = TypesOfObsCheck[6]; //Измерение C2C (C2P) ГЛОНАСС

    mesOn[26]  = TypesOfObsCheck[11]; //Измерение C5 (C5C) GPS
    mesOn[27]  = false;              //Измерение P5 (C5P) GPS
    mesOn[28]  = TypesOfObsCheck[12]; //Измерение L5 (L5C) GPS
    mesOn[29]  = TypesOfObsCheck[13]; //Измерение D5 (D5C) GPS
    mesOn[30]  = TypesOfObsCheck[14]; //Измерение S5 (S5C) GPS
    mesOn[31]  = TypesOfObsCheck[11]; //Измерение C5 (C5C) ГЛОНАСС
    mesOn[32]  = false;               //Измерение P5 (C5P) ГЛОНАСС
    mesOn[33]  = TypesOfObsCheck[12]; //Измерение L5 (L5C) ГЛОНАСС
    mesOn[34]  = TypesOfObsCheck[13]; //Измерение D5 (D5C) ГЛОНАСС
    mesOn[35]  = TypesOfObsCheck[14]; //Измерение S5 (S5C) ГЛОНАСС



    //Служебные переменные для обработки текста
    QString SecShiftEpoch;
    QString NumberOfSats;
    QString AllSatsEpoch;
    QString AllSatsEpochAdd;
    quint8 NofTypesInt;
    int NsatsEpoch;
    //Переменная для хранения номера строки в списке
    qint8 NofSatStrings;
    qint8 NofMeasStrings;

    QString CurrentSat;
    QString CurrentSatSys;
    QString SatMeasString;
    QString SatMeas;
    QString AllSatMeas;
    int CurrentSatNum;
    QString Measure;

    bool ok;
    //quint16 NumbInOrderTypepsObs[14];
    quint8 TypeOfMeasure;

    //Временные переменные для хранения измерений соответствующих типов
    qreal C1 = 0;
    qreal P1 = 0;
    qreal S1 = 0;
    qreal S1P1 = 0;
    qreal L1 = 0;
    qreal D1 = -200000;
    qreal C2 = 0;
    qreal L2 = 0;
    qreal P2 = 0;
    qreal D2 = -200000;
    qreal S2 = 0;
    qreal C5 = 0;
    qreal P5 = 0;
    qreal L5 = 0;
    qreal D5 = -200000;
    qreal S5 = 0;

    //qDebug() << "KT0001" << AList->at(0);
    //qDebug()<<"KP 002";

    //Главный цикл метода считывания эпохи измерений в формате RINEX 2.10
    //Цикл повторяется пока не закончится входной список строк "AList"
    //Преобразует данные эпохи в строковом формате в соответствующие массивы объектов

    YearEpoch = AList->at(0);
    YearEpoch.truncate(3); //Убираем из строки символы после 3ого символа
    YearEpoch = YearEpoch.right(2); //Сохраняем последние 2 символа в строке
    MonthEpoch = AList->at(0);
    MonthEpoch.truncate(6); //Убираем из строки символы после 6ого символа
    MonthEpoch = MonthEpoch.right(2); //Сохраняем последние 2 символа в строке
    DayEpoch = AList->at(0);
    DayEpoch.truncate(9); //Убираем из строки символы после 9ого символа
    DayEpoch = DayEpoch.right(2); //Сохраняем последние 2 символа в строке
    HourEpoch = AList->at(0);
    HourEpoch.truncate(12); //Убираем из строки символы после 12ого символа
    HourEpoch = HourEpoch.right(2); //Сохраняем последние 2 символа в строке
    MinuteEpoch = AList->at(0);
    MinuteEpoch.truncate(15); //Убираем из строки символы после 15ого символа
    MinuteEpoch = MinuteEpoch.right(2); //Сохраняем последние 2 символа в строке
    SecondsEpoch = AList->at(0);
    SecondsEpoch.truncate(26); //Убираем из строки символы после 26ого символа
    SecondsEpoch = SecondsEpoch.right(10); //Сохраняем последние 10 символа в строке
    NumberOfSats = AList->at(0);
    NumberOfSats.truncate(32); //Убираем из строки символы после 32ого символа
    NumberOfSats = NumberOfSats.right(2); //Сохраняем последние 3 символа в строке
    SecShiftEpoch = AList->at(0);
    SecShiftEpoch = SecShiftEpoch.right(11); //Сохраняем последние 10 символа в строке

    //Инициализируев переменные класса для хранения момента времени
    YearEph = YearEpoch.toInt(&ok, 10);
    YearEph = 2000+YearEph;
    MonthEph = MonthEpoch.toInt(&ok, 10);
    DayEph = DayEpoch.toInt(&ok, 10);
    HourEph = HourEpoch.toInt(&ok, 10);
    MinEph = MinuteEpoch.toInt(&ok, 10);
    SecEph = SecondsEpoch.toDouble(&ok);
    ReceiverClockOffset = SecShiftEpoch.toDouble(&ok);

    //Преобразуем число спутников наблюдаемых в данной эпохи из строки в целое число
    NsatsEpoch = NumberOfSats.toInt(&ok, 10);
    //qDebug() << "NsatsEpoch " <<NsatsEpoch;
    //Вычисляем число строк содержащих номера спутников ГНСС
    //В одной строке максимально может быть записано 12 идентификаторов спутников
    //Если их больше 12 то используется 2 строки
    //Если их больше 24 то используется 3 строки
    //Если их больше 36 то используется 4 строки
    NofSatStrings = 1;
    if(NsatsEpoch>12)NofSatStrings = 2;
    if(NsatsEpoch>24)NofSatStrings = 3;
    if(NsatsEpoch>36)NofSatStrings = 4;
    //Обрабатываем первую строку эпохи
    AllSatsEpoch = AList->at(0);
    AllSatsEpoch.truncate(68); //Убираем из строки символы после 68ого символа
    AllSatsEpoch = AllSatsEpoch.mid(32,-1);
    //AllSatsEpoch = AllSatsEpoch.right(36); //Сохраняем последние 36 символов в строке
    //Проверка наличая второй строки содержащей номера спутников
    if(NofSatStrings > 1)
    {
        for(qint8 j = 1; j < NofSatStrings; j++)
        {
            //Формируется общая строка номеров спутников
            AllSatsEpochAdd = AList->at(j);
            AllSatsEpochAdd.truncate(68); //Убираем из строки символы после 68ого символа

            if(AllSatsEpochAdd.size() == 68){
                //Сохраняем последние 36 символов в строке
                AllSatsEpochAdd = AllSatsEpochAdd.right(36);
            }
            else{
                //Сохраняем последние 36 символов в строке
                AllSatsEpochAdd = AllSatsEpochAdd.remove(0,32);
            }
            AllSatsEpoch.append(AllSatsEpochAdd);//добавляем строку спутников к основной строке списка спутников
        }
    }

    AllSatsEpoch.remove(NsatsEpoch*3, 1000); // убираем лишние пробелы в строке списка спутников

    NofTypesInt = NofTypes.toInt(&ok, 10);
    NofMeasStrings = NofTypesInt/5+1;
    //Создание указателей на объекты
    SatGPS *GPS;
    SatGLO *GLO;

    try //Блок try №1
    {
    for(int i=0;i<NsatsEpoch+1;i++)
    {
        int k=i*3;
        //выбираем из списка спутников один спутник по порядку
        CurrentSat = AllSatsEpoch.mid(k,3);
        //определяем его систему
        CurrentSatSys = CurrentSat.mid(0,1);
        //определяем номер спутника
        CurrentSatNum = CurrentSat.mid(1,2).toInt(&ok, 10);

        //Формируем общую строку измерений
        for(quint8 m=0; m<NofMeasStrings;m++)
        {
            //где:
            //NofMeasStrings - число строк хронящих измерения одного спутника
            //NofSatStrings - число строк хронящих список спутников
            //i - номер спутника в списке
            //m - текущая строка из NofSatStrings строк
            divide101(AList->length(), ((i*NofMeasStrings+m)+ NofSatStrings));
            SatMeas = AList->at((i*NofMeasStrings+m)+ NofSatStrings);
            if(SatMeas.size()!=80){
                SatMeas = AddSpace(SatMeas,80-SatMeas.size());
            }
            AllSatMeas.append(SatMeas);
        }
        //цикл извлечения отдельных измерений из строки
        for(quint8 it=1;it<=NofTypesInt;it++)
        {
            //qDebug()<<"AllSatMeas"<<AllSatMeas;
                if(it==1){Measure = AllSatMeas.mid(0,14);}
                else{ Measure = AllSatMeas.mid(((it-1))*16,14);}
                //Запись измерения в соответствующую временную переменную
                    if(NumbInOrderTypepsObs[it] == 1)
                    { C1 = Measure.toDouble(&ok);
                        //qDebug()<<"Measure"<<Measure;
                        //qDebug()<<"NumbInOrderTypepsObs[it]"<<NumbInOrderTypepsObs[it];
                        //qDebug()<<"it"<<it;
                        //Measure = AllSatMeas.mid((it-1)*16+15,1);
                        if(mesOn[4]==false){S1 = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObs[it] == 2)
                    { P1 = Measure.toDouble(&ok);
                        //Measure = AllSatMeas.mid((it-1)*16+15,1);
                        if(mesOn[20]==false){S1P1 = SigmaToNoise(Measure.toInt(&ok,10));}
                    }
                    if(NumbInOrderTypepsObs[it] == 3)
                    { L1 = Measure.toDouble(&ok);
                        if(mesOn[4]==false)
                        {//Measure = SatMeasString.mid(3 + (it-1)*16 + 15,1);
                            S1 = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObs[it] == 4)
                    { D1 = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObs[it] == 5)
                    { S1 = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObs[it] == 6)
                    { C2 = Measure.toDouble(&ok);
                        if(mesOn[8]==false){S2 = getNoiseOfStr(SatMeasString, it);};
                    }
                    if(NumbInOrderTypepsObs[it] == 7)
                    { P2 = Measure.toDouble(&ok);
                        //Measure = AllSatMeas.mid((it-1)*16+15,1);
                        if(mesOn[8]==false){S2 = getNoiseOfStr(SatMeasString, it);};
                    }
                    if(NumbInOrderTypepsObs[it] == 8)
                    { L2 = Measure.toDouble();
                        if(mesOn[8]==false)
                        {//Measure = SatMeasString.mid(3 + (it-1)*16 + 15,1);
                            S2 = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObs[it] == 9)
                    { D2 = Measure.toDouble(&ok);
                        if(D2==0){
                            D2=-200000;
                        }
                    }
                    if(NumbInOrderTypepsObs[it] == 10)
                    { S2 = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObs[it] == 11)
                    { C5 = Measure.toDouble(&ok);
                      //if(mesOn[4]==false){S1 = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObs[it] == 12)
                    { L5 = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObs[it] == 13)
                    { D5 = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObs[it] == 14)
                    { S5 = Measure.toDouble(&ok);
                    }
                    //Конец цикла обработки одного измерения
                    if(CurrentSatSys == "R")
                    {
                        lit[CurrentSatNum] = litCalc(C1,L1);
                    }
            }
        //Проверка типа спутника, создание соответствующего объекта и добавление его в список
        if(CurrentSatSys.lastIndexOf("G",-1)==0)
        {
          GPS = new SatGPS(CurrentSatNum,C1,P1,C2,P2,C5,0,L1,0.0,L2,L5,D1,-2000000.0,D2,D5,S1,S1P1,S2,S5);
        //GPS = new SatGPS(CurrentSatNum,C1C,C1P,C2C,C2P,L1C,L1P,L2P,D1C,D1P,D2P,S1C,S1P,S2P);
        ArraySatGPS.append(GPS);
        }
        if(CurrentSatSys.lastIndexOf("R",-1)==0)
        {
        GLO = new SatGLO(CurrentSatNum,C1,P1,C2,P2,C5,0,L1,0.0,L2,L5,D1,-2000000.0,D2,D5,0,S1,S1P1,S2,S5);
        ArraySatGLO.append(GLO);
        }
        AllSatMeas.clear();
        }//Конец цикла обработки одного спутника
    }//Конец блока try №1
    catch (const char* msg)
        {
            //qDebug()<< "Error " << errorCode;
        }
    //qDebug()<<"KP 005";
    }//Конец условия

    if (version == 302){
        bool ok;
        //переменные для хранения эпохи(момента времени) в формате QString
        QString YearEpoch;
        QString MonthEpoch;
        QString DayEpoch;
        QString HourEpoch;
        QString MinuteEpoch;
        QString SecondsEpoch;
        //Служебные переменные для обработки текста
        QString SecShiftEpoch;
        QString NumberOfSats;

        /* ПРАВИЛА СОРТИРОВКИ ИЗМЕРЕНИЙ
         * ДЛЯ СИСТЕМЫ GPS - "TypesOfObsCheckGPS" */

         /// **** ТАБЛИЦА ИЗМЕНЕНА 19.12.19!!!!!

         /* ==================================================================================================
         * | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  |
         * | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
         * ---------------------------------------------------------------------------
         * | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  | 27  | 28  | 29  | 30  | 31  | 32  |
         * | C5C | L5C | D5C | S5C | C5P | L5P | D5P | S5P | --- | --- | --- | --- | --- | --- | --- | --- |
         * ==================================================================================================
         * ДЛЯ СИСТЕМЫ ГЛОНАСС - "TypesOfObsCheckGLO"
         * ==================================================================================================
         * | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  |
         * | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
         * ==================================================================================================
         */


        mesOn[0]  = TypesOfObsCheckGPS[1]; //Измерение C1 GPS
        mesOn[1]  = TypesOfObsCheckGPS[5]; //Измерение P1 GPS
        mesOn[2]  = TypesOfObsCheckGPS[2]; //Измерение L1 GPS
        mesOn[3]  = TypesOfObsCheckGPS[3]; //Измерение D1 GPS
        mesOn[4]  = TypesOfObsCheckGPS[4]; //Измерение S1 GPS
        mesOn[5]  = TypesOfObsCheckGPS[13]; //Измерение P2 GPS
        mesOn[6]  = TypesOfObsCheckGPS[14]; //Измерение L2 GPS
        mesOn[7]  = TypesOfObsCheckGPS[15]; //Измерение D2 GPS
        mesOn[8]  = TypesOfObsCheckGPS[16]; //Измерение S2 GPS



        mesOn[9]  = TypesOfObsCheckGLO[1]; //Измерение C1 ГЛОНАСС
        mesOn[10] = TypesOfObsCheckGLO[5]; //Измерение P1 ГЛОНАСС
        mesOn[11] = TypesOfObsCheckGLO[2]; //Измерение L1 ГЛОНАСС
        mesOn[12] = TypesOfObsCheckGLO[3]; //Измерение D1 ГЛОНАСС
        mesOn[13] = TypesOfObsCheckGLO[4]; //Измерение S1 ГЛОНАСС
        mesOn[14] = TypesOfObsCheckGLO[13]; //Измерение P2 ГЛОНАСС
        mesOn[15] = TypesOfObsCheckGLO[14]; //Измерение L2 ГЛОНАСС
        mesOn[16] = TypesOfObsCheckGLO[15]; //Измерение D2 ГЛОНАСС
        mesOn[17] = TypesOfObsCheckGLO[16]; //Измерение S2 ГЛОНАСС

        mesOn[18]  = TypesOfObsCheckGPS[6]; //Измерение L1P (L1P) GPS
        mesOn[19]  = TypesOfObsCheckGPS[7]; //Измерение D1P (D1P) GPS
        mesOn[20]  = TypesOfObsCheckGPS[8]; //Измерение S1P (S1P) GPS

        mesOn[21]  = TypesOfObsCheckGPS[9]; //Измерение C2C (C2C) GPS

        mesOn[22]  = TypesOfObsCheckGLO[6]; //Измерение L1P (L1P) ГЛОНАСС
        mesOn[23]  = TypesOfObsCheckGLO[7]; //Измерение D1P (D1P) ГЛОНАСС
        mesOn[24]  = TypesOfObsCheckGLO[8]; //Измерение S1P (S1P) ГЛОНАСС
        mesOn[25]  = TypesOfObsCheckGLO[9]; //Измерение C2C (C2C) ГЛОНАСС

        mesOn[26]  = TypesOfObsCheckGPS[17]; //Измерение C5 (C5C) GPS
        mesOn[27]  = TypesOfObsCheckGPS[21]; //Измерение P5 (C5P) GPS
        mesOn[28]  = TypesOfObsCheckGPS[18]; //Измерение L5 (L5C) GPS
        mesOn[29]  = TypesOfObsCheckGPS[19]; //Измерение D5 (D5C) GPS
        mesOn[30]  = TypesOfObsCheckGPS[20]; //Измерение S5 (S5C) GPS


        mesOn[31]  = false;                     //Измерение C5 (C5C) ГЛОНАСС
        mesOn[32]  = false;                     //Измерение P5 (C5P) ГЛОНАСС
        mesOn[33]  = false;                     //Измерение L5 (L5C) ГЛОНАСС
        mesOn[34]  = false;                     //Измерение D5 (D5C) ГЛОНАСС
        mesOn[35]  = false;                     //Измерение S5 (S5C) ГЛОНАСС

//        mesOn[31]  = false;                     //Измерение C5 (C5C) ГЛОНАСС
//        mesOn[32]  = false;                     //Измерение P5 (C5P) ГЛОНАСС
//        mesOn[33]  = false;                     //Измерение L5 (L5C) ГЛОНАСС
//        mesOn[34]  = false;                     //Измерение D5 (D5C) ГЛОНАСС
//        mesOn[35]  = false;                     //Измерение S5 (S5C) ГЛОНАСС

        mesOn[36]  = TypesOfObsCheckGPS[24 + 8];  //S5P

        quint8 NofTypesInt;
        NofTypesInt = NofTypes.toInt(&ok, 10);

        int NsatsEpoch;
        QString CurrentSat;
        QString CurrentSatSys;
        QString SatMeasString;
        QString AllSatMeas;
        int CurrentSatNum;
        QString Measure;

        /* Временные переменные для хранения измерений соответствующих типов
         * =====================================================================
         * | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  |
         * | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
         * ---------------------------------------------------------------------------
         * | 16  | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  | 27  | 28  | 29  | 30  | 31  |
         * | C5C | L5C | D5C | S5C | C5P | L5P | D5P | S5P | --- | --- | --- | --- | --- | --- | --- | --- |
         */

        YearEpoch = AList->at(0);
        YearEpoch.truncate(6); //Убираем из строки символы после 3ого символа
        YearEpoch = YearEpoch.right(4); //Сохраняем последние 2 символа в строке
        MonthEpoch = AList->at(0);
        MonthEpoch.truncate(9); //Убираем из строки символы после 6ого символа
        MonthEpoch = MonthEpoch.right(2); //Сохраняем последние 2 символа в строке
        DayEpoch = AList->at(0);
        DayEpoch.truncate(12); //Убираем из строки символы после 9ого символа
        DayEpoch = DayEpoch.right(2); //Сохраняем последние 2 символа в строке
        HourEpoch = AList->at(0);
        HourEpoch.truncate(15); //Убираем из строки символы после 12ого символа
        HourEpoch = HourEpoch.right(2); //Сохраняем последние 2 символа в строке
        MinuteEpoch = AList->at(0);
        MinuteEpoch.truncate(18); //Убираем из строки символы после 15ого символа
        MinuteEpoch = MinuteEpoch.right(2); //Сохраняем последние 2 символа в строке
        SecondsEpoch = AList->at(0);
        SecondsEpoch.truncate(29); //Убираем из строки символы после 26ого символа
        SecondsEpoch = SecondsEpoch.right(10); //Сохраняем последние 10 символа в строке
        NumberOfSats = AList->at(0);
        NumberOfSats.truncate(35); //Убираем из строки символы после 32ого символа
        NumberOfSats = NumberOfSats.right(2); //Сохраняем последние 3 символа в строке
        SecShiftEpoch = AList->at(0);
        SecShiftEpoch = SecShiftEpoch.right(15); //Сохраняем последние 10 символа в строке

        //Инициализируев переменные класса для хранения момента времени
        YearEph = YearEpoch.toInt(&ok, 10);
        MonthEph = MonthEpoch.toInt(&ok, 10);
        DayEph = DayEpoch.toInt(&ok, 10);
        HourEph = HourEpoch.toInt(&ok, 10);
        MinEph = MinuteEpoch.toInt(&ok, 10);
        SecEph = SecondsEpoch.toDouble(&ok);
        ReceiverClockOffset = SecShiftEpoch.toDouble(&ok);

        //Преобразуем число спутников наблюдаемых в данной эпохи из строки в целое число
        NsatsEpoch = NumberOfSats.toInt(&ok, 10);
        SatGPS *GPS;
        SatGLO *GLO;
        qreal C1C,S1C,L1C,D1C,C1P,S1P,L1P,D1P;
        qreal C1W,S1W,L1W,D1W,C2W,S2W,L2W,D2W;
        qreal C2C,S2C,L2C,D2C,C2P,S2P,L2P,D2P;
        qreal C5C,S5C,L5C,D5C,C5P;
        //Цикл считывания измерений из списка строк AList
        //результат работы цикла: массивы объектов SatGPS и SatGLO
        for(int i=1;i<NsatsEpoch+1;i++)
        {
            C1C=0;S1C=0;L1C=0;D1C=-200000;
            C1P=0;S1P=0;L1P=0;D1P=-200000;
            C2C=0;S2C=0;L2C=0;D2C=-200000;
            C2P=0;S2P=0;L2P=0;D2P=-200000;
            //redaction
            C1W=0;S1W=0;L1W=0;D1W=-200000;
            C2W=0;S2W=0;L2W=0;D2W=-200000;
            SatMeasString = AList->at(i);
            //выбираем из списка спутников один спутник по порядку
            CurrentSat = SatMeasString.mid(0,3);
            //определяем его систему
            CurrentSatSys = SatMeasString.mid(0,1);
            //определяем номер спутника
            CurrentSatNum = SatMeasString.mid(1,2).toInt(&ok, 10);
            //цикл извлечения отдельных измерений из строки
            for(quint8 it=1;it<=NofTypesInt;it++)
            {
                if(it==1){Measure = SatMeasString.mid(3 + 0,14);}
                else{Measure = SatMeasString.mid(3 + (it-1)*16,14);}

                //Запись измерения в соответствующую временную переменную
                //Для системы GPS
                if(CurrentSatSys == "G")
                {
                    //========= ЧАСТОТА L1 =========
                    if(NumbInOrderTypepsObsGPS[it] == 1)    //Измерение C1C
                    { C1C = Measure.toDouble(&ok);
                      //Считываем признак шума для C1C, используется если в файле нет измерений S1C
                      if(mesOn[4]==false){S1C = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 2)    //Измерение L1C
                    { L1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 3)    //Измерение D1C
                    { D1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 4)    //Измерение S1C
                    { S1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 5)    //Измерение C1P
                    { C1P = Measure.toDouble(&ok);
                        //Считываем признак шума для C1P, используется если в файле нет измерений S1P
                        if(mesOn[20]==false){S1C = getNoiseOfStr(SatMeasString, it);}
                      }
                    if(NumbInOrderTypepsObsGPS[it] == 6)    //Измерение L1P
                    { L1P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 7)    //Измерение D1P
                    { D1P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 8)    //Измерение S1P
                    { S1P = Measure.toDouble(&ok);}
                    //========= ЧАСТОТА L2 =========
                    if(NumbInOrderTypepsObsGPS[it] == 9 + 4)    //Измерение C2C
                    { C2C = Measure.toDouble(&ok);
                        //Считываем признак шума для C2C, используется если в файле нет измерений S2C
                        //if(mesOn[0]==false){S2C = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 10 + 4)   //Измерение L2C
                    { L2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 11 + 4)   //Измерение D2C
                    { D2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 12 + 4)   //Измерение S2C
                    { S2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 13 + 4)   //Измерение C2P
                    { C2P = Measure.toDouble(&ok);
                      //Считываем признак шума для C2P, используется если в файле нет измерений S2P
                      if(mesOn[8]==false){S2P = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 14 + 4)   //Измерение L2P
                    { L2P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 15 + 4)   //Измерение D2P
                    { D2P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 16 + 4)   //Измерение S2P
                    { S2P = Measure.toDouble(&ok);}

                    if(NumbInOrderTypepsObsGPS[it] == 17 + 8)   //Измерение C5C
                    { C5C = Measure.toDouble(&ok);
                      //Считываем признак шума для C2P, используется если в файле нет измерений S2P
                      if(mesOn[30]==false){S5C = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 18 + 8)   //Измерение L5C
                    { L5C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 19 + 8)   //Измерение D5C
                    { D5C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 20 + 8)   //Измерение S5C
                    { S5C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGPS[it] == 21 + 8)   //Измерение C2P
                    { C5P = Measure.toDouble(&ok);
                    }

                    //redaction
                    if(NumbInOrderTypepsObsGPS[it] == 9){  //Измерение C1W
                        C1W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 10){  //Измерение L1W
                        L1W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 11){  //Измерение D1W
                        D1W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 12){  //Измерение S1W
                        S1W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 21){  //Измерение C2W
                        C2W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 22){  //Измерение L2W
                        L2W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 23){  //Измерение D2W
                        D2W = Measure.toDouble(&ok);
                    }
                    if(NumbInOrderTypepsObsGPS[it] == 24){  //Измерение S2W
                        S2W = Measure.toDouble(&ok);
                    }

                }//Конец условия для GPS
                //Для системы ГЛОНАСС
                if(CurrentSatSys == "R")
                {
                    //========= ЧАСТОТА L1 =========
                    if(NumbInOrderTypepsObsGLO[it] == 1)    //Измерение C1C
                    { C1C = Measure.toDouble(&ok);
                      //Считываем признак шума для C1C, используется если в файле нет измерений S1C
                      if(mesOn[13]==false){S1C = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGLO[it] == 2)    //Измерение L1C
                    { L1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 3)    //Измерение D1C
                    { D1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 4)    //Измерение S1C
                    { S1C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 5)    //Измерение C1P
                    { C1P = Measure.toDouble(&ok);
                        //Считываем признак шума для C1P, используется если в файле нет измерений S1P
                        if(mesOn[20]==false){S1C = getNoiseOfStr(SatMeasString, it);}
                      }
                    if(NumbInOrderTypepsObsGLO[it] == 6)    //Измерение L1P
                    { L1P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 7)    //Измерение D1P
                    { D1P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 8)    //Измерение S1P
                    { S1P = Measure.toDouble(&ok);}
                    //========= ЧАСТОТА L2 =========
                    if(NumbInOrderTypepsObsGLO[it] == 9)    //Измерение C2C
                    { C2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 10)   //Измерение L2C
                    { L2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 11)   //Измерение D2C
                    { D2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 12)   //Измерение S2C
                    { S2C = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 13)   //Измерение C2P
                    { C2P = Measure.toDouble(&ok);
                      //Считываем признак шума для C2P, используется если в файле нет измерений S2P
                      if(mesOn[17]==false){S2P = getNoiseOfStr(SatMeasString, it);}
                    }
                    if(NumbInOrderTypepsObsGLO[it] == 14)   //Измерение L2P
                    { L2P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 15)   //Измерение D2P
                    { D2P = Measure.toDouble(&ok);}
                    if(NumbInOrderTypepsObsGLO[it] == 16)   //Измерение S2P
                    { S2P = Measure.toDouble(&ok);}


                }//Конец условия для ГЛОНАСС
                if(CurrentSatSys == "R")
                {
                    Epoch::lit[CurrentSatNum] = litCalc(C1C,L1C);
                }
            } //Конец цикла обработки одного измерения
        //Проверка типа спутника, создание соответствующего объекта и добавление его в список
        if(CurrentSatSys.lastIndexOf("G",-1)==0)
        {
            GPS = new SatGPS(CurrentSatNum,C1C,C1P,C2C,C2P,C5C,C5P,L1C,L1P,L2P,L5C,D1C,D1P,D2P,D5C,S1C,S1P,S2P,S5C);
            ArraySatGPS.append(GPS);
        }
        if(CurrentSatSys.lastIndexOf("R",-1)==0)
        {
            GLO = new SatGLO(CurrentSatNum,C1C,C1P,C2C,C2P,0,0,L1C,L1P,L2P,0,D1C, D1P,D2P,0,0,S1C,S1P,S2P,0);
            ArraySatGLO.append(GLO);
        }
        AllSatMeas.clear();
        }//Конец цикла
    }//Конец условия
}//Конец конструктора (rinex)

void Epoch::divide101(int a, int b){
    if (b>=a){
        errorCode = 101;
        throw "Обращение к несуществующей строке массива Alist";
    }
}

void Epoch::destroyer(){
    for(int i=0; i<ArraySatGPS.length();i++){
        delete ArraySatGPS.at(i);
    }
    for(int i=0; i<ArraySatGLO.length();i++){
        delete ArraySatGLO.at(i);
    }
}

//запись в файл .O версии 302 эпохи измерений
void Epoch::WriteRNX302Eph (QFile *RINEX, bool Doppler, QString SpaceDefault)
{
    //Подготовка к записи файла
    QTextStream outRINEX(RINEX);
    int NumbOFSats = ArraySatGPS.length()+ ArraySatGLO.length();
    QString EpochFlag = "0";

    //Запись первой строки
    if(ArraySatGPS.length() > 0 || ArraySatGLO.length() > 0){
        outRINEX << ">";
        outRINEX<<qSetFieldWidth(5)<<right<<qSetRealNumberPrecision(0) << YearEph;//год
        outRINEX<<qSetFieldWidth(3)<< ToStandartNum(MonthEph);//месяц
        outRINEX<<qSetFieldWidth(3)<< ToStandartNum(DayEph);//день
        outRINEX<<qSetFieldWidth(3)<< ToStandartNum(HourEph);//час
        outRINEX<<qSetFieldWidth(3)<< ToStandartNum(MinEph);//минуты
        outRINEX<<qSetFieldWidth(11)<<fixed<< qSetPadChar(' ') <<qSetRealNumberPrecision(7) << SecEph;//секунды
        outRINEX<<qSetFieldWidth(3)<< EpochFlag;//флаг эпохи
        outRINEX<<qSetFieldWidth(3)<< NumbOFSats;//кол-во спутников
        outRINEX <<qSetFieldWidth(21)<<qSetRealNumberPrecision(12) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
    }
    //Запись измерений GPS
    for(int i=0;i<ArraySatGPS.length(); i++)
    {
        //C1C L1C D1C С1P L1P D1P C2P L2P D2P
        //Идентификатор спутниковой системы и номер спутника
        outRINEX<<qSetFieldWidth(3)<<ArraySatGPS[i]->NumToNameG(ArraySatGPS[i]->NumOfSat);
        //Измерение C1C
                if(mesOn[0])
                {
                    if(ArraySatGPS[i]->Code_L1>0)
                    {
                    outRINEX<<qSetFieldWidth(14)<<right<<fixed<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);
                    }
                    else
                    {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
                }

        //Измерение L1C
                if(mesOn[2])
                {
                if(ArraySatGPS[i]->Phase_L1>0)
                {
                outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);
                }
                else
                { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
                }
                //qDebug()<<"phaza L1"<<ArraySatGPS[i]->Phase_L1;
        //Измерение D1C
            if(mesOn[3] & Doppler){
            if((ArraySatGPS[i]->Doppler_L1>-200000)&(ArraySatGPS[i]->Doppler_L1!=0))
            {
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);
            }
            else{outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            }
        //Измерение S1C
        if(mesOn[4]){
        if((ArraySatGPS[i]->Noise_L1>0)&(ArraySatGPS[i]->Noise_L1!=255)){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L1<<qSetFieldWidth(1)<<" "<<" ";
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }

        //Измерение С1P
        if(mesOn[1]){
        if(ArraySatGPS[i]->Code_L1P>0)
        {
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }

        //Измерение L1P
        if(mesOn[18]){
        if(ArraySatGPS[i]->Phase_L1P>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1P);
        }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение D1P

        if(mesOn[19]&Doppler){
            if((ArraySatGPS[i]->Doppler_L1P>-200000)&(ArraySatGPS[i]->Doppler_L1P!=0))
                {
                        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1P);
                }
            else{outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение S1P
        if(mesOn[20]){
        if((ArraySatGPS[i]->Noise_L1P>0)&(ArraySatGPS[i]->Noise_L1P!=255))
        {
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L1P<<qSetFieldWidth(1)<<" "<<" ";
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение C2C
        if(mesOn[21]){
        if(ArraySatGPS[i]->Code_L2>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L2<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);}
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение C2P
        if(mesOn[5]){
        if(ArraySatGPS[i]->Code_L2P>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение L2P
        if(mesOn[6]){
        if(ArraySatGPS[i]->Phase_L2P>0){
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение D2P
        if(mesOn[7]&Doppler){
            if((ArraySatGPS[i]->Doppler_L2P>-200000)&(ArraySatGPS[i]->Doppler_L2P!=0))
            {
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);
            }
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение S2P
        if(mesOn[8]){
        if((ArraySatGPS[i]->Noise_L2P>0)&(ArraySatGPS[i]->Noise_L2P!=255)){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L2P<<qSetFieldWidth(1)<<" "<<" ";
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //**************************************************************
        //Измерение C5C
        if(mesOn[26 + 8]){
        if(ArraySatGPS[i]->Code_L5>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);}
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение C5P
        if(mesOn[27 + 8]){
        if(ArraySatGPS[i]->Code_L5P>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L5P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение L5P(C) ///opechatka
        if(mesOn[28 + 8]){
        if(ArraySatGPS[i]->Phase_L5>0){
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение D5P(C) ///opechatka
        if(mesOn[29 + 8]&Doppler){
            if((ArraySatGPS[i]->Doppler_L5>-200000)&(ArraySatGPS[i]->Doppler_L5!=0))
            {
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);
            }
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение S5P
        if(mesOn[36 + 8]){
        if((ArraySatGPS[i]->Noise_L5>0)&(ArraySatGPS[i]->Noise_L5!=255)){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L5<<qSetFieldWidth(1)<<" "<<" ";
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }

        //Конец строки
        outRINEX<<qSetFieldWidth(0)<<endl;
    }

    //Запись измерений ГЛОНАСС
    for(int i=0;i<ArraySatGLO.length(); i++)
    {
        if(ArraySatGLO.length() > 20){
            //
            qDebug()<<"";
        }
        //C1C L1C D1C С1P L1P D1P C2P L2P D2P
        //Идентификатор спутниковой системы и номер спутника
        if(ArraySatGLO[i]->NumOfSat==255){
            qDebug()<<endl<<ArraySatGLO[i]->NumOfSat<<endl;
        }
        outRINEX<<qSetFieldWidth(3)<<ArraySatGLO[i]->NumToNameR(ArraySatGLO[i]->NumOfSat);

        //Измерение C1C
        if(mesOn[9]){
        if(ArraySatGLO[i]->Code_L1>0){
        outRINEX<<qSetFieldWidth(14)<<right<<fixed<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault; }
        }

        //Измерение L1C
        if(mesOn[11]){
        if(ArraySatGLO[i]->Phase_L1>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Phase_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);
        }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault; }
        }
                //Измерение D1C
        if(mesOn[12]&Doppler){
        if((ArraySatGLO[i]->Doppler_L1>-200000)&(ArraySatGLO[i]->Doppler_L1!=0))
                {
                        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Doppler_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);
                }
            else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение S1C
        if(mesOn[13]){
            if((ArraySatGLO[i]->Noise_L1>0)&(ArraySatGLO[i]->Noise_L1!=255))
            {
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Noise_L1<<qSetFieldWidth(1)<<" "<<" ";
            }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault; }
        }

        //Измерение С1P
        if(mesOn[10]){
        if(ArraySatGLO[i]->Code_L1P>1){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1P);
        }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }

        //Измерение L1P
        if(mesOn[22]){
        if(ArraySatGLO[i]->Phase_L1P>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Phase_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1P);
        }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }

        //Измерение D1P
        if(mesOn[23]&Doppler){
        if((ArraySatGLO[i]->Doppler_L1P>-200000)&(ArraySatGLO[i]->Doppler_L1P!=0))
                {
                        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Doppler_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1P);
                }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение S1P
        if(mesOn[24]){
                    if((ArraySatGLO[i]->Noise_L1P>0)&(ArraySatGLO[i]->Noise_L1P!=255))
                    {
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Noise_L1P<<qSetFieldWidth(1)<<" "<<" ";
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение C2C
        if(mesOn[25]){
        if(ArraySatGLO[i]->Code_L2>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L2<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault; }
        }
        //Измерение C2P
        if(mesOn[14]){
        if(ArraySatGLO[i]->Code_L2P>0){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение L2P
        if(mesOn[15]){
        if(ArraySatGLO[i]->Phase_L2P>0){
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Phase_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);
        }
        else {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Измерение D2P
        if(mesOn[16]&Doppler){
            if((ArraySatGLO[i]->Doppler_L2P>-200000)&(ArraySatGLO[i]->Doppler_L2P!=0))
            {
            outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Doppler_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);
            }
            else{outRINEX<<qSetFieldWidth(16)<<SpaceDefault; }
        }
        //Измерение S2P
        if(mesOn[17]){
        if((ArraySatGLO[i]->Noise_L2P>0)&(ArraySatGLO[i]->Noise_L2P!=255)){
        outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Noise_L2P<<qSetFieldWidth(1)<<" "<<" ";
        }
        else { outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
        }
        //Конец строки
        outRINEX<<qSetFieldWidth(0)<<endl;
    }
}//Конец метода WriteRNX302Eph

//запись в файл .O версии 211 эпохи измерений
void Epoch::WriteRNX211Eph(QFile *RINEX, bool Doppler, QString SpaceDefault)
{
    //Подготовка к записи
    QTextStream outRINEX(RINEX);
    quint32 NumbOFSats = ArraySatGPS.length() + ArraySatGLO.length();
    QString EpochFlag = "0";
    QString AllSatLineStr,AllSatLineStr2;
    quint32 Y;

    if(YearEph<2000)
    {Y = YearEph-1900;}
    if(YearEph>=2000||YearEph<2100)
    {Y = YearEph-2000;}

    //Запись первой строки
    outRINEX<<qSetFieldWidth(1)<< " ";
    outRINEX<<qSetFieldWidth(2)<< right<<qSetRealNumberPrecision(0) << Y;//год
    outRINEX<<qSetFieldWidth(3)<< MonthEph;//месяц
    outRINEX<<qSetFieldWidth(3)<< DayEph;//день
    outRINEX<<qSetFieldWidth(3)<< HourEph;//час
    outRINEX<<qSetFieldWidth(3)<< MinEph;//минуты
    outRINEX<<qSetFieldWidth(11)<<fixed<< qSetPadChar(' ') <<qSetRealNumberPrecision(7) << SecEph;//секунды
    outRINEX<<qSetFieldWidth(3)<< EpochFlag;//флаг эпохи
    outRINEX<<qSetFieldWidth(3)<< NumbOFSats;//кол-во спутников

    //Запись
    if(NumbOFSats<=12)
    {
        for(int i=0;i<ArraySatGPS.length(); i++)
        {
            AllSatLineStr.append(ArraySatGPS[i]->NumToNameG(ArraySatGPS[i]->NumOfSat));
        }
        for(int i=0;i<ArraySatGLO.length(); i++)
        {
            AllSatLineStr.append(ArraySatGLO[i]->NumToNameR(ArraySatGLO[i]->NumOfSat));
        }
    outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr;
    outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
    }

    if(NumbOFSats>12&&NumbOFSats<=24)
    {
        if(ArraySatGPS.length()<12)
        {
            for(int i=0;i<(ArraySatGPS.length()); i++)
            {
                AllSatLineStr.append(ArraySatGPS[i]->NumToNameG(ArraySatGPS[i]->NumOfSat));
            }
            for(int i=0;i<(12-ArraySatGPS.length()); i++)//!*!
            {
                AllSatLineStr.append(ArraySatGLO[i]->NumToNameR(ArraySatGLO[i]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
            for(int i=(12-ArraySatGPS.length());i<(ArraySatGLO.length()); i++)//!*!
            {
                AllSatLineStr2.append(ArraySatGLO[i]->NumToNameR(ArraySatGLO[i]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(32)<<" ";
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr2;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
        }
        if(ArraySatGPS.length()==12)
        {
            for(int i=0;i<12; i++)
            {
                AllSatLineStr.append(ArraySatGPS[i]->NumToNameG(ArraySatGPS[i]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов

            for(int k=0;k<ArraySatGLO.length(); k++)
            {
                AllSatLineStr2.append(ArraySatGLO[k]->NumToNameR(ArraySatGLO[k]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(32)<<" ";
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr2;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
       }
        if(ArraySatGPS.length()>12)
        {
            for(int i=0;i<12; i++)
            {
                AllSatLineStr.append(ArraySatGPS[i]->NumToNameG(ArraySatGPS[i]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
            for(int k=12;k<ArraySatGPS.length(); k++)
            {
                AllSatLineStr2.append(ArraySatGPS[k]->NumToNameG(ArraySatGPS[k]->NumOfSat));
            }
            for(int i=0;i<ArraySatGLO.length(); i++)
            {
                AllSatLineStr2.append(ArraySatGLO[i]->NumToNameR(ArraySatGLO[i]->NumOfSat));
            }
            outRINEX<<left<<qSetFieldWidth(32)<<" ";
            outRINEX<<left<<qSetFieldWidth(36)<<AllSatLineStr2;
            outRINEX <<right<<qSetFieldWidth(12)<<qSetRealNumberPrecision(9) << ReceiverClockOffset<<qSetFieldWidth(0)<<endl;//уход часов
        }
    }

    int mesInString = 0;


    ///Подсчет количества типов измерений для GPS
    int countTypeIzmForGPS = 0;

    int valuesMesOn[13] = {0,1,2,3,4,5,6,7,8,34,36,37,38};
    for(int i = 0; i < 13; i++){
        if(mesOn[valuesMesOn[i]] == true)
            countTypeIzmForGPS++;
    }

    //Запись измерений GPS
    for(int i=0;i<ArraySatGPS.length(); i++)
    {
        //Измерение C1
        if(mesOn[0] || mesOn[9]){
            if(ArraySatGPS[i]->Code_L1>0)
            {outRINEX<<qSetFieldWidth(14)<<right<<fixed<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение P1
        if(mesOn[1] || mesOn[10]){
            if(ArraySatGPS[i]->Code_L1P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение L1
        if(mesOn[2] || mesOn[11]){
            if(ArraySatGPS[i]->Phase_L1>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение D1
        if((mesOn[3]&Doppler) || (mesOn[12]&Doppler)){
            if((ArraySatGPS[i]->Doppler_L1>-200000)&(ArraySatGPS[i]->Doppler_L1!=0))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение S1
        if(mesOn[4] || mesOn[13]){
            if((ArraySatGPS[i]->Noise_L1>0)&(ArraySatGPS[i]->Noise_L1!=255))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L1<<qSetFieldWidth(1)<<" "<<" ";}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение P2
        if(mesOn[5] || mesOn[14]){
            if(ArraySatGPS[i]->Code_L2P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение L2
        if(mesOn[6] || mesOn[15]){
            if(ArraySatGPS[i]->Phase_L2P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение D2
        if((mesOn[7]&Doppler) || (mesOn[16]&Doppler)){
            if((ArraySatGPS[i]->Doppler_L2P>-200000)&(ArraySatGPS[i]->Doppler_L2P!=0))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0))
        {outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение S2
        if(mesOn[8] || mesOn[17]){
            if((ArraySatGPS[i]->Noise_L2P>0)&(ArraySatGPS[i]->Noise_L2P!=255))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L2P<<qSetFieldWidth(1)<<" "<<" ";}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }

        //C1W

        //Измерение C5
        if(mesOn[26]){
            if(ArraySatGPS[i]->Code_L5>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Code_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение L5
        if(mesOn[28]){
            if(ArraySatGPS[i]->Phase_L5>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Phase_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение D5
        if(mesOn[29]&Doppler){
            if((ArraySatGPS[i]->Doppler_L5>-200000)&(ArraySatGPS[i]->Doppler_L5!=0))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Doppler_L5<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGPS[i]->Noise_L5);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0))
        {outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение S5
        if(mesOn[30]){
            if((ArraySatGPS[i]->Noise_L5>0)&(ArraySatGPS[i]->Noise_L5!=255))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGPS[i]->Noise_L5<<qSetFieldWidth(1)<<" "<<" ";}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }

        ///Печать пробелов при необходимости
//        if(rinexReader::countTypeIzmInVersion2 > countTypeIzmForGPS){
//            int countString = rinexReader::countTypeIzmInVersion2 / 6 + 1;
//            for(int stepString = countTypeIzmForGPS / 6 + 1; stepString < countString; stepString++){
//                outRINEX<<qSetFieldWidth(1)<<endl<<" ";
//            }
//        }

        if((mesInString%5==0)&(mesInString!=0)){
            outRINEX<<qSetFieldWidth(0)<<endl;
            mesInString = 0;
        }
        if((mesInString%5!=0)&(mesInString!=0)){
            outRINEX<<qSetFieldWidth(0)<<endl;
        }

        mesInString = 0;
        //Конец строки
    }


    //Запись измерений ГЛОНАСС


    ///Подсчет количества типов измерений для GLO
    int countTypeIzmForGLO = 0;

    int valuesMesOnGLO[9] = {9,10,11,12,13,14,15,16,17};
    for(int i = 0; i < 9; i++){
        if(mesOn[valuesMesOnGLO[i]] == true)
            countTypeIzmForGLO++;
    }

    for(int i=0;i<ArraySatGLO.length(); i++)
    {
        //Измерение C1
        if(mesOn[9] || mesOn[0]){
            if(ArraySatGLO[i]->Code_L1>0)
            {outRINEX<<qSetFieldWidth(14)<<right<<fixed<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение P1
        if(mesOn[10] || mesOn[1]){
            if(ArraySatGLO[i]->Code_L1P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L1P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение L1
        if(mesOn[11] || mesOn[2]){
            if(ArraySatGLO[i]->Phase_L1>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Phase_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение D1
        if((mesOn[12]&Doppler) || (mesOn[3]&Doppler)){
            if((ArraySatGLO[i]->Doppler_L1>-200000)&(ArraySatGLO[i]->Doppler_L1!=0))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Doppler_L1<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L1);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение S1
        if(mesOn[13] || mesOn[4]){
            if((ArraySatGLO[i]->Noise_L1>0)&(ArraySatGLO[i]->Noise_L1!=255))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Noise_L1<<qSetFieldWidth(1)<<" "<<" ";}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение P2
        if(mesOn[14] || mesOn[5]){
            if(ArraySatGLO[i]->Code_L2P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Code_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение L2
        if(mesOn[15] || mesOn[6]){
            if(ArraySatGLO[i]->Phase_L2P>0)
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Phase_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение D2
        if((mesOn[16]&Doppler) || (mesOn[7]&Doppler)){
            if((ArraySatGLO[i]->Doppler_L2P>-200000)&(ArraySatGLO[i]->Doppler_L2P!=0))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Doppler_L2P<<qSetFieldWidth(1)<<" "<<NoiseToSigma(ArraySatGLO[i]->Noise_L2P);}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }
        if((mesInString%5==0)&(mesInString!=0))
        {outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        //Измерение S2
        if(mesOn[17] || mesOn[8]){
            if((ArraySatGLO[i]->Noise_L2P>0)&(ArraySatGLO[i]->Noise_L2P!=255))
            {outRINEX<<qSetFieldWidth(14)<<qSetRealNumberPrecision(3)<<ArraySatGLO[i]->Noise_L2P<<qSetFieldWidth(1)<<" "<<" ";}
            else
            {outRINEX<<qSetFieldWidth(16)<<SpaceDefault;}
            mesInString ++;
        }

        ///Печать пробелов при необходимости
//        if(rinexReader::countTypeIzmInVersion2 > countTypeIzmForGLO){
//            int countString = rinexReader::countTypeIzmInVersion2 / 6 + 1;
//            for(int stepString = countTypeIzmForGLO / 6 + 1; stepString < countString; stepString++){
//                outRINEX<<qSetFieldWidth(1)<<endl<<" ";
//            }
//        }


        if((mesInString%5==0)&(mesInString!=0)){outRINEX<<qSetFieldWidth(0)<<endl;
        mesInString = 0;}
        if((mesInString%5!=0)&(mesInString!=0))
        {
            outRINEX<<qSetFieldWidth(0)<<endl;
            mesInString = 0;
        }

        //Конец строки
    }
} //Конец метода WriteRNX211Eph

// функция создания и записи в файл header Rinex Obs версии 3.02 (при конвертации из RINEX 2.11)
void Epoch::WriteRNX302HeaderORinexRead(QFile *RINEX,rinexReader *Reader)
{
    QTextStream outRINEX(RINEX);

    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString Marker("MARKER NAME");
    QString MarkType("MARKER TYPE");
    QString MarkNumber("MARKER NUMBER");
    QString Observer("OBSERVER / AGENCY");
    QString RecCharects("REC # / TYPE / VERS");
    QString ANTCharects("ANT # / TYPE");
    QString APRXPosition("APPROX POSITION XYZ");
    QString AntDelta("ANTENNA: DELTA H/E/N");
    QString TypeObs("SYS / # / OBS TYPES");
    QString WavelengthF("WAVELENGTH FACT L1/2");
    QString Interval("INTERVAL");
    QString TimeFirstObs("TIME OF FIRST OBS");
    QString TimeLastObs("TIME OF LAST OBS");
    QString RCVClockOffs("RCV CLOCK OFFS APPL");
    QString LeapSec("LEAP SECONDS");
    QString GloSlot("GLONASS SLOT / FRQ #");
    QString GloCOD("GLONASS COD/PHS/BIS");
    QString NumbSats("# OF SATELLITES");
    QString PhaseShiftComm("SYS / PHASE SHIFT");
    QString progName;
    QString AggencyName;
    QString MrkNme;
    QString MrkNmb;
    QString ObsName;
    QString NumbRes;
    QString TypeRes;
    QString VerRes;
    QString NumbAnt;
    QString TypeAnt;
    bool ok;

    progName = converterJps::Inf.at(0);
    AggencyName = converterJps::Inf.at(1);
    MrkNme = converterJps::Inf.at(2);
    MrkNmb = converterJps::Inf.at(3);
    NumbRes = converterJps::Inf.at(4);
    TypeRes = converterJps::Inf.at(5);
    VerRes = converterJps::Inf.at(6);
    NumbAnt = converterJps::Inf.at(7);
    TypeAnt = converterJps::Inf.at(8);
    quint32 NumberR=0;

    qreal ApprX = 0;
    qreal ApprY = 0;
    qreal ApprZ = 0;

    QDate FirstD = Reader->timeOfFirst->date();
    QTime FirstT = Reader->timeOfFirst->time();
    QDate LastD = Reader->timeOfLast->date();
    QTime LastT = Reader->timeOfLast->time();

    //время первой эпохи
    quint32 FirstYear =  FirstD.year();
    quint32 FirstMonth =  FirstD.month();
    quint32 FirstDay = FirstD.day();
    quint32 FirstHour = FirstT.hour();
    quint32 FirstMinute =  FirstT.minute();
    qreal FirstSeconds = FirstT.second()+FirstT.msec()/1000.0;

    //время последней эпохи
    quint32 LastYear = LastD.year();
    quint32 LastMonth = LastD.month();
    quint32 LastDay = LastD.day();
    quint32 LastHour = LastT.hour();
    quint32 LastMinute = LastT.minute();
    qreal LastSeconds = LastT.second()+FirstT.msec()/1000.0;

    ApprX = Reader->X;
    ApprY = Reader->Y;
    ApprZ = Reader->Z;

    qreal IntervalCount;
    IntervalCount = Reader->Interval.toDouble();

    double GloBISCorr[4] = {0,0,0,0};

    qreal PhaseShiftGPSL1C=0.0;
    qreal PhaseShiftGPSL2W=0.0;
    qreal PhaseShiftGLOL1C=0.0;
    qreal PhaseShiftGLOL2P=-0.25;

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("yyyyMMdd HHmmss");
    //получить текущую дату, выраженную в секундах
    //ЗАПИСЬ ФАЙЛА. ЗАГОЛОВОК

    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     3.02           OBSERVATION DATA    M"<< qSetFieldWidth(0);

    outRINEX <<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX <<  qSetFieldWidth(20) <<progName<<qSetFieldWidth(0);

    outRINEX << qSetFieldWidth(20)<<  AggencyName<<qSetFieldWidth(0);
    outRINEX << qSetFieldWidth(16)<< CurrentDate<<qSetFieldWidth(0);

    outRINEX << qSetFieldWidth(4)<< "UTC"<<qSetFieldWidth(0);
    outRINEX << PGMDATE << qSetFieldWidth(0) << endl;

    outRINEX <<  qSetFieldWidth(60) <<"G = GPS       R = GLONASS"<<qSetFieldWidth(0);
    outRINEX <<  Comment<< qSetFieldWidth(0) << endl;

    outRINEX << qSetFieldWidth(60)<< MrkNme<<qSetFieldWidth(0);
    outRINEX << Marker<< qSetFieldWidth(0) << endl;//

    outRINEX << qSetFieldWidth(60)<< "GEODETIC"<<qSetFieldWidth(0);
    outRINEX << MarkType<< qSetFieldWidth(0) << endl;//

    //outRINEX << qSetFieldWidth(60)<<MrkNmb;
    //outRINEX << MarkNumber<< qSetFieldWidth(0) << endl; //
    outRINEX <<right <<qSetFieldWidth(20)<<ObsName;
    outRINEX << left<<qSetFieldWidth(40)<<AggencyName<<qSetFieldWidth(0)
        <<  Observer<< qSetFieldWidth(0) << endl; //

    outRINEX << qSetFieldWidth(20)<<left<<NumbRes<<
    qSetFieldWidth(20)<< left<<  TypeRes <<
    qSetFieldWidth(20)<<left<<   VerRes<< left <<qSetFieldWidth(19)<<RecCharects<< qSetFieldWidth(0) << endl; //

    outRINEX <<left<<  qSetFieldWidth(20)<<NumbAnt<<
        left<<  qSetFieldWidth(40)<< TypeAnt << qSetFieldWidth(12)<< left << ANTCharects<< qSetFieldWidth(0) << endl; //

    outRINEX<<right<<fixed <<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprX; //
    outRINEX<<right<<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprY; //
    outRINEX<<right<<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprZ; //
    outRINEX<< left<<  qSetFieldWidth(18)<<" "<<APRXPosition<< qSetFieldWidth(0) << endl; //

    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<Reader->H; //
    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<Reader->E; //
    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<Reader->N; //
    outRINEX<< left<< qSetFieldWidth(18)<<" "<<AntDelta<< qSetFieldWidth(0) << endl;//



    /*ФОРМИРОВАНИЕ СТРОК "TYPES OF OBSERV"
     *Т.к RINEX версии 2.xx не поддерживает раздельные типы измерений для GPS и GLONASS
     *выбирается МАКСИМАЛЬНОЕ количество типов измерений (из 2х вариантов).
     *При формировании строки типов, для добавления типа ДОСТАТОЧНО наличая измерений для ОДНОЙ системы ГНСС.
     *Пример: В файле RINEX 3.xx есть тип данных L2P для GPS но нет для ГЛОНАСС.
     *После конвертации в файле RINEX 2.xx будет тип измерений L2
     */
    int missingTypes = 0;
    quint8 TypesOn = 0;
    int availableTypes = 0;
    int missingTypesR = 0;
    quint8 TypesOnR = 0;
    int availableTypesR = 0;
    int numbOfSpace = 13*4+1;


        availableTypes = Reader->NumbInOrderTypepsObs[0];
        availableTypesR = Reader->NumbInOrderTypepsObs[0];
        //qDebug()<<"Kol-vo of types"<<Reader->NumbInOrderTypepsObs[0];

    outRINEX <<"G"<<right<<qSetFieldWidth(5)<<  qSetRealNumberPrecision(0)  << availableTypes;
    if(Reader->TypesOfObsCheck[1])
    {outRINEX << qSetFieldWidth(4)  <<  "C1C";
    TypesOn++;}
    else{missingTypes++;}

    if((Reader->TypesOfObsCheck[3]))
    {outRINEX << qSetFieldWidth(4)  <<  "L1C";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[4]))
    {outRINEX << qSetFieldWidth(4)  <<  "D1C";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[5]))
    {outRINEX << qSetFieldWidth(4)  <<  "S1C";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[2]))
    {outRINEX << qSetFieldWidth(4)  <<  "C1P";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[6]))
    {outRINEX << qSetFieldWidth(4)  <<  "C2C";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[7]))
    {outRINEX << qSetFieldWidth(4)  <<  "C2P";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[8]))
    {outRINEX << qSetFieldWidth(4)  <<  "L2P";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[9]))
    {outRINEX << qSetFieldWidth(4)  <<  "D2P";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[10]))
    {outRINEX << qSetFieldWidth(4)  <<  "S2P";
    TypesOn++;}
    else{missingTypes++;}
    //if((Reader->TypesOfObsCheckGPS[6]))
    //{outRINEX << qSetFieldWidth(4)  <<  "L1P";}
    //else{missingTypes++;}
    //if((Reader->TypesOfObsCheckGPS[7]))
    //{outRINEX << qSetFieldWidth(4)  <<  "D1P";}
    //else{missingTypes++;}
    //if((Reader->TypesOfObsCheckGPS[8]))
    //{outRINEX << qSetFieldWidth(4)  <<  "S1P";}
    //else{missingTypes++;}
    if((Reader->TypesOfObsCheck[11]))
    {outRINEX << qSetFieldWidth(4)  <<  "C5C";
    TypesOn++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheck[12]))
    {outRINEX << qSetFieldWidth(4)  <<  "L5P";
    TypesOn++;}
    else{missingTypes++;}
    if(TypesOn==13)
    {    outRINEX << qSetFieldWidth(numbOfSpace-(TypesOn*4))  <<  " ";
        outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;
        outRINEX <<"G"<<right<<qSetFieldWidth(5)<<  qSetRealNumberPrecision(0)  << availableTypes;
    TypesOn=0;}
    if((Reader->TypesOfObsCheck[13]))
    {outRINEX << qSetFieldWidth(4)  <<  "D5P";
    TypesOn++;}
    else{missingTypes++;}
    if(TypesOn==13)
    {    outRINEX << qSetFieldWidth(numbOfSpace-(TypesOn*4))  <<  " ";
        outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;
        outRINEX <<"G"<<right<<qSetFieldWidth(5)<<  qSetRealNumberPrecision(0)  << availableTypes;
    TypesOn=0;}
    if((Reader->TypesOfObsCheck[14]))
    {outRINEX << qSetFieldWidth(4)  <<  "S5P";
    TypesOn++;}
    else{missingTypes++;}


    outRINEX << qSetFieldWidth(numbOfSpace-(TypesOn*4))  <<  " ";
    outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;

    outRINEX <<"R"<<right<<qSetFieldWidth(5)<<  qSetRealNumberPrecision(0)  << availableTypesR;
    if(Reader->TypesOfObsCheck[1])
    {outRINEX << qSetFieldWidth(4)  <<  "C1C";
    TypesOnR++;}
    else{missingTypesR++;}

    if((Reader->TypesOfObsCheck[3]))
    {outRINEX << qSetFieldWidth(4)  <<  "L1C";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[4]))
    {outRINEX << qSetFieldWidth(4)  <<  "D1C";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[5]))
    {outRINEX << qSetFieldWidth(4)  <<  "S1C";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[2]))
    {outRINEX << qSetFieldWidth(4)  <<  "C1P";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[6]))
    {outRINEX << qSetFieldWidth(4)  <<  "C2C";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[7]))
    {outRINEX << qSetFieldWidth(4)  <<  "C2P";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[8]))
    {outRINEX << qSetFieldWidth(4)  <<  "L2P";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[9]))
    {outRINEX << qSetFieldWidth(4)  <<  "D2P";
    TypesOnR++;}
    else{missingTypesR++;}
    if((Reader->TypesOfObsCheck[10]))
    {outRINEX << qSetFieldWidth(4)  <<  "S2P";
    TypesOnR++;}
    else{missingTypesR++;}
    //if((Reader->TypesOfObsCheckGLO[6]))
    //{outRINEX << qSetFieldWidth(4)  <<  "L1P";}
    //else{missingTypesR++;}
    //if((Reader->TypesOfObsCheckGLO[7]))
    //{outRINEX << qSetFieldWidth(4)  <<  "D1P";}
    //else{missingTypesR++;}
    //if((Reader->TypesOfObsCheckGLO[8]))
    //{outRINEX << qSetFieldWidth(4)  <<  "S1P";}
    //else{missingTypesR++;}
    outRINEX << qSetFieldWidth(numbOfSpace-(TypesOnR*4))  <<  " ";
    outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;

    //ВЫВОД СТРОКИ "INTERVAL"

    outRINEX << qSetFieldWidth(10)<< qSetPadChar(' ') <<qSetRealNumberPrecision(3)<<IntervalCount;
    outRINEX << qSetFieldWidth (58)<< qSetPadChar(' ') <<Interval<< qSetFieldWidth(0) << endl;;

    //ВЫВОД СТРОКИ "TIME OF First OBS"

    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << FirstYear;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << FirstMonth;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstDay;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstHour;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstMinute;
    outRINEX << qSetFieldWidth(13)<<qSetRealNumberPrecision(7) << FirstSeconds;
    outRINEX << qSetFieldWidth(8)<<qSetRealNumberPrecision(0)<< "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX << qSetFieldWidth (9) <<TimeFirstObs<< qSetFieldWidth(0) << endl;;
    //TIME OF LAST OBS
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << LastYear<<qSetFieldWidth(6)<< qSetRealNumberPrecision(0);
    outRINEX << LastMonth;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastDay;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastHour;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastMinute;
    outRINEX << qSetFieldWidth(13)<< qSetRealNumberPrecision(7) << LastSeconds;
    outRINEX << qSetFieldWidth(8) << qSetRealNumberPrecision(0) << "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX << qSetFieldWidth(9) << TimeLastObs<< qSetFieldWidth(0) << endl;
    //=========================# OF SATELLITES
    outRINEX <<"G"<<right
                        <<qSetFieldWidth(4)<<"L1C"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGPSL1C
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;
    outRINEX <<"G"<<right
                        <<qSetFieldWidth(4)<<"L2W"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGPSL2W
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;

    outRINEX <<"R"<<right
                        <<qSetFieldWidth(4)<<"L1C"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGLOL1C
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;
    outRINEX <<"R"<<right
                        <<qSetFieldWidth(4) <<"L2P"
                        <<qSetFieldWidth(9) << qSetRealNumberPrecision(5) <<PhaseShiftGLOL2P
                        <<qSetFieldWidth(43) <<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;

    //Вычисление числа спутников ГЛОНАСС наблюдаемых в сеансе
    for (int i = 0; i<40; i++){
        //Если значение литеры не равно 255(маркер отсутствия данных), увеличиваем на 1 переменную NumberR
        if(Reader->lit[i]!=255){
            NumberR++;
        }
    }
    //Запись числа наблюдаемых спутников ГЛОНАСС
    if (NumberR != 0)
    {outRINEX<<right<<qSetFieldWidth(3)<<NumberR;}
    //Объевляем и иницализируем служебные переменные
    int k =0,l=0,check =0;
    //Открываем цикл для записи литер
    for (int i = 1; i<40; i++)
    {
        //Если значение литеры не равно 255(маркер отсутствия данных)
        if(Reader->lit[i]!=255)
        {   if(i<9){
            outRINEX<< right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i) << qSetFieldWidth(3)<<Reader->lit[i];
            k++;}
            if(i>=9){
            outRINEX<<qSetFieldWidth(3)<<right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i)<< qSetFieldWidth(3)<<Reader->lit[i];
            k++;}
            if(k%8==0){
            outRINEX << qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;
            l++;
            outRINEX<< right <<qSetFieldWidth(3)<<" ";}}
    }
    if(k%8!=0)
    {check = (8-(k-l*8))*7;
     outRINEX << qSetFieldWidth(check) <<" "<< qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;}

    //GLONASS COD/PHS/BIS
    outRINEX << qSetFieldWidth(0) << left << " "<<"C1C"<< " ";
    outRINEX<< right <<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[0];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C1P"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[1];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C2C"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[2];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C2P"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[3];
    outRINEX<< left << qSetFieldWidth(0) <<"        "<<qSetFieldWidth(20)<<GloCOD<< qSetFieldWidth(0) << endl;
    //Конец заголовка
    outRINEX<< left << qSetFieldWidth(60)<<" "<<qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;

}

// функция создания и записи в файл header Rinex Obs версии 3.02
void Epoch::WriteRNX302HeaderO(QFile *RINEX,double deltaH, double deltaN, double deltaE, bool Doppler,jpsReader *Reader)
{
    QTextStream outRINEX(RINEX);

    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString Marker("MARKER NAME");
    QString MarkNumber("MARKER NUMBER");
    QString MarkType("MARKER TYPE");
    QString Observer("OBSERVER / AGENCY");
    QString RecCharects("REC # / TYPE / VERS");
    QString ANTCharects("ANT # / TYPE");
    QString APRXPosition("APPROX POSITION XYZ");
    QString AntDelta("ANTENNA: DELTA H/E/N");
    QString TypeObs("SYS / # / OBS TYPES");
    QString WavelengthF("WAVELENGTH FACT L1/2");
    QString Interval("INTERVAL");
    QString TimeFirstObs("TIME OF FIRST OBS");
    QString TimeLastObs("TIME OF LAST OBS");
    QString RCVClockOffs("RCV CLOCK OFFS APPL");
    QString LeapSec("LEAP SECONDS");
    QString GloSlot("GLONASS SLOT / FRQ #");
    QString GloCOD("GLONASS COD/PHS/BIS");
    QString NumbSats("# OF SATELLITES");
    QString PhaseShiftComm("SYS / PHASE SHIFT");
    QString progName;
    QString AggencyName;
    QString MrkNme;
    QString MrkNmb;
    QString ObsName;
    QString NumbRes;
    QString TypeRes;
    QString VerRes;
    QString NumbAnt;
    QString TypeAnt;

    progName = converterJps::Inf.at(0);
    AggencyName = converterJps::Inf.at(1);
    MrkNme = converterJps::Inf.at(2);
    MrkNmb = converterJps::Inf.at(3);
    NumbRes = converterJps::Inf.at(4);
    TypeRes = converterJps::Inf.at(5);
    VerRes = converterJps::Inf.at(6);
    NumbAnt = converterJps::Inf.at(7);
    TypeAnt = converterJps::Inf.at(8);

    double ApprX = 0;
    double ApprY = 0;
    double ApprZ = 0;

    //Число спутников ГЛОНАСС наблюдаемых в сеансе
    //Используется для записи строки литер спутников ГЛОНАСС
    quint32 NumberR=0;

    //время первой эпохи
    quint32 FirstYear =  Reader->getYear(0);
    quint32 FirstMonth =  Reader->getMonth(0);
    quint32 FirstDay = Reader->getDay(0);
    quint32 FirstHour = Reader->getHour(0);
    quint32 FirstMinute =  Reader->getMinute(0);
    qreal FirstSeconds = Reader->getSecond(0);

    //время последней эпохи
    quint32 LastYear = Reader->getYear(Reader->numEp-1);
    quint32 LastMonth = Reader->getMonth(Reader->numEp-1);
    quint32 LastDay = Reader->getDay(Reader->numEp-1);
    quint32 LastHour = Reader->getHour(Reader->numEp-1);
    quint32 LastMinute = Reader->getMinute(Reader->numEp-1);
    qreal LastSeconds = Reader->getSecond(Reader->numEp-1);

    if(ApprX==0){
        ApprX = Reader->getApproxX(0);
    }
    if(ApprY==0){
        ApprY = Reader->getApproxY(0);
    }
    if(ApprZ==0){
        ApprZ = Reader->getApproxZ(0);
    }

    if(ApprX==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxX(i);
        }
        temp = temp/Reader->numEp;
        ApprX = temp;
    }
    if(ApprY==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxY(i);
        }
        temp = temp/Reader->numEp;
        ApprY = temp;
    }
    if(ApprZ==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxZ(i);
        }
        temp = temp/Reader->numEp;
        ApprZ = temp;
    }

    qreal iner1 = 0;
    qreal iner2 = 0;
    for(int i =0;i<Reader->numEp-1;i++){
        if(Reader->getDay(i)== Reader->getDay(+1)& Reader->getMonth(i)== Reader->getMonth(i+1)){
            iner1 = Reader->getSecond(i) + Reader->getMinute(i)*60.0 + Reader->getHour(i)*1440.0;
            iner2 = Reader->getSecond(i+1) + Reader->getMinute(i+1)*60.0 + Reader->getHour(i+1)*1440.0;
        break;
        }
    }
    qreal IntervalCount;
    IntervalCount = iner2-iner1;

    double GloBISCorr[4] = {0,0,0,0};

    qreal PhaseShiftGPSL1C=0.0;
    qreal PhaseShiftGPSL2W=0.0;
    qreal PhaseShiftGLOL1C=0.0;
    qreal PhaseShiftGLOL2P=-0.25;

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("yyyyMMdd HHmmss");
    //получить текущую дату, выраженную в секундах
    //ЗАПИСЬ ФАЙЛА. ЗАГОЛОВОК

    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     3.02           OBSERVATION DATA    M"<< qSetFieldWidth(0);
    outRINEX <<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX <<  qSetFieldWidth(20) <<progName<<qSetFieldWidth(0);
    outRINEX << qSetFieldWidth(20)<<  AggencyName<<qSetFieldWidth(0);
    outRINEX << qSetFieldWidth(16)<< CurrentDate<<qSetFieldWidth(0);
    outRINEX << qSetFieldWidth(4)<< "UTC"<<qSetFieldWidth(0);
    outRINEX << PGMDATE << qSetFieldWidth(0) << endl;

    outRINEX <<  qSetFieldWidth(60) <<"G = GPS       R = GLONASS"<<qSetFieldWidth(0);
    outRINEX <<  Comment<< qSetFieldWidth(0) << endl;

    outRINEX << qSetFieldWidth(60)<< MrkNme<<qSetFieldWidth(0);
    outRINEX << Marker<< qSetFieldWidth(0) << endl;//

    outRINEX << qSetFieldWidth(60)<< "GEODETIC"<<qSetFieldWidth(0);
    outRINEX << MarkType<< qSetFieldWidth(0) << endl;//
    //outRINEX << qSetFieldWidth(60)<<MrkNmb;
    //outRINEX << MarkNumber<< qSetFieldWidth(0) << endl; //
    outRINEX <<right <<qSetFieldWidth(20)<<ObsName;
    outRINEX << left<<qSetFieldWidth(40)<<AggencyName<<qSetFieldWidth(0)
        <<  Observer<< qSetFieldWidth(0) << endl; //

    outRINEX << qSetFieldWidth(20)<<left<<NumbRes<<
    qSetFieldWidth(20)<< left<<  TypeRes <<
    qSetFieldWidth(20)<<left<<   VerRes<< left <<qSetFieldWidth(19)<<RecCharects<< qSetFieldWidth(0) << endl; //

    outRINEX <<left<<  qSetFieldWidth(20)<<NumbAnt<<
        left<<  qSetFieldWidth(40)<< TypeAnt << qSetFieldWidth(12)<< left << ANTCharects<< qSetFieldWidth(0) << endl; //

    outRINEX<<right<<fixed <<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprX; //
    outRINEX<<right<<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprY; //
    outRINEX<<right<<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprZ; //
    outRINEX<< left<<  qSetFieldWidth(18)<<" "<<APRXPosition<< qSetFieldWidth(0) << endl; //

    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<deltaH; //
    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<deltaE; //
    outRINEX<<right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<deltaN; //
    outRINEX<< left<< qSetFieldWidth(18)<<" "<<AntDelta<< qSetFieldWidth(0) << endl;//


    if(Doppler){
    outRINEX <<"G"<<right<<qSetFieldWidth(5)<<"12"
            <<qSetFieldWidth(4)<<"C1C"
            <<qSetFieldWidth(4)<<"L1C"
            <<qSetFieldWidth(4)<<"D1C"
            <<qSetFieldWidth(4)<<"S1C"
            <<qSetFieldWidth(4)<<"C1P"
            <<qSetFieldWidth(4)<<"L1P"
            <<qSetFieldWidth(4)<<"D1P"
            <<qSetFieldWidth(4)<<"S1P"
            //<<qSetFieldWidth(4)<<"С2С"
            <<qSetFieldWidth(4)<<"C2P"
            <<qSetFieldWidth(4)<<"L2P"
            <<qSetFieldWidth(4)<<"D2P"
            <<qSetFieldWidth(4)<<"S2P"
            <<qSetFieldWidth(5)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;
    }
    else{
        outRINEX <<"G"<<right<<qSetFieldWidth(5)<<"6"
                            <<qSetFieldWidth(4)<<"C1C"
                            <<qSetFieldWidth(4)<<"L1C"
                            <<qSetFieldWidth(4)<<"C1P"
                            <<qSetFieldWidth(4)<<"L1P"
                            <<qSetFieldWidth(4)<<"C2P"
                            <<qSetFieldWidth(4)<<"L2P"
                            <<qSetFieldWidth(29)<<" "<<qSetFieldWidth(0);
        outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;
    }

    if(Doppler){
    outRINEX <<"R"<<right<<qSetFieldWidth(5)<<"12"
            <<qSetFieldWidth(4)<<"C1C"
            <<qSetFieldWidth(4)<<"L1C"
            <<qSetFieldWidth(4)<<"D1C"
            <<qSetFieldWidth(4)<<"S1C"
            <<qSetFieldWidth(4)<<"C1P"
            <<qSetFieldWidth(4)<<"L1P"
            <<qSetFieldWidth(4)<<"D1P"
            <<qSetFieldWidth(4)<<"S1P"
            //<<qSetFieldWidth(4)<<"С2С"
            <<qSetFieldWidth(4)<<"C2P"
            <<qSetFieldWidth(4)<<"L2P"
            <<qSetFieldWidth(4)<<"D2P"
            <<qSetFieldWidth(4)<<"S2P"
            <<qSetFieldWidth(5)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;;
    }
    else{
        outRINEX <<"R"<<right<<qSetFieldWidth(5)<<"6"
                                <<qSetFieldWidth(4)<<"C1C"
                                <<qSetFieldWidth(4)<<"L1C"
                                <<qSetFieldWidth(4)<<"C1P"
                                <<qSetFieldWidth(4)<<"L1P"
                                <<qSetFieldWidth(4)<<"C2P"
                                <<qSetFieldWidth(4)<<"L2P"
                                <<qSetFieldWidth(29)<<" "<<qSetFieldWidth(0);
        outRINEX <<qSetFieldWidth (20)<<TypeObs<< qSetFieldWidth(0) << endl;;
    }

    outRINEX << qSetFieldWidth(10)<< qSetPadChar(' ') <<qSetRealNumberPrecision(3)<<IntervalCount;
    outRINEX << qSetFieldWidth (58)<< qSetPadChar(' ') <<Interval<< qSetFieldWidth(0) << endl;;
    //TIME OF First OBS
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << FirstYear;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << FirstMonth;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstDay;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstHour;
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< FirstMinute;
    outRINEX << qSetFieldWidth(13)<<qSetRealNumberPrecision(7) << FirstSeconds;
    outRINEX << qSetFieldWidth(8)<<qSetRealNumberPrecision(0)<< "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX << qSetFieldWidth (9) <<TimeFirstObs<< qSetFieldWidth(0) << endl;;
    //TIME OF LAST OBS
    outRINEX << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << LastYear<<qSetFieldWidth(6)<< qSetRealNumberPrecision(0);
    outRINEX << LastMonth;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastDay;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastHour;
    outRINEX << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << LastMinute;
    outRINEX << qSetFieldWidth(13)<< qSetRealNumberPrecision(7) << LastSeconds;
    outRINEX << qSetFieldWidth(8) << qSetRealNumberPrecision(0) << "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX << qSetFieldWidth(9) << TimeLastObs<< qSetFieldWidth(0) << endl;;
    //=========================# OF SATELLITES
    outRINEX <<"G"<<right
                        <<qSetFieldWidth(4)<<"L1C"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGPSL1C
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;
    outRINEX <<"G"<<right
                        <<qSetFieldWidth(4)<<"L2W"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGPSL2W
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;

    outRINEX <<"R"<<right
                        <<qSetFieldWidth(4)<<"L1C"
                        <<qSetFieldWidth(9)<< qSetRealNumberPrecision(5) <<PhaseShiftGLOL1C
                        <<qSetFieldWidth(43)<<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;
    outRINEX <<"R"<<right
                        <<qSetFieldWidth(4) <<"L2P"
                        <<qSetFieldWidth(9) << qSetRealNumberPrecision(5) <<PhaseShiftGLOL2P
                        <<qSetFieldWidth(43) <<" "<<qSetFieldWidth(0);
    outRINEX <<qSetFieldWidth (20)<<PhaseShiftComm<< qSetFieldWidth(0) << endl;

    //Вычисление числа спутников ГЛОНАСС наблюдаемых в сеансе
    for (int i = 0; i<40; i++){
        //Если значение литеры не равно 255(маркер отсутствия данных), увеличиваем на 1 переменную NumberR
        if(lit[i]!=255){
            NumberR++;
        }
    }
    //Запись числа наблюдаемых спутников ГЛОНАСС
    outRINEX<<right<<qSetFieldWidth(3)<<NumberR;
    //Объевляем и иницализируем служебные переменные
    int k =0,l=0,check =0, testlit=0;
    for (int i = 1; i<40; i++)
    {
        if(lit[i]!=255)
        {
                      k++;

        }

    }
            testlit = qFloor(k /8);
            k=0;
    //Открываем цикл для записи литер
    for (int i = 1; i<40; i++)
    {
        //Если значение литеры не равно 255(маркер отсутствия данных)
        if(lit[i]!=255)
        {
            if(i<9)
            outRINEX<< right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i) << qSetFieldWidth(3)<<lit[i];
            if(i>=9)
            outRINEX<<qSetFieldWidth(3)<<right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i)<< qSetFieldWidth(3)<<lit[i];
            k++;
            if(k%8==0)
                {
                outRINEX << qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;
                l++;
                if(testlit!=l)
                outRINEX << right <<qSetFieldWidth(3)<<" ";
                }
        }
        else{
            i++;
        }
    }
    if(k%8!=0)
    {
        check = (8-(k-l*8))*7;
        outRINEX << qSetFieldWidth(check) <<" "<< qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;
    }

    //GLONASS COD/PHS/BIS
    outRINEX << qSetFieldWidth(0) << left << " "<<"C1C"<< " ";
    outRINEX<< right <<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[0];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C1P"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[1];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C2C"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[2];
    outRINEX<< qSetFieldWidth(0) <<" "<<"C2P"<< " ";
    outRINEX<<qSetFieldWidth(8)<<qSetRealNumberPrecision(3)<<GloBISCorr[3];
    outRINEX<< left << qSetFieldWidth(0) <<"        "<<qSetFieldWidth(20)<<GloCOD<< qSetFieldWidth(0) << endl;
    //Конец заголовка
    outRINEX<< left << qSetFieldWidth(60)<<" "<<qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

// функция создания и записи в файл header Rinex Obs версии 2.11 (при конвертации из RINEX 3.02)
void Epoch::WriteRNX211HeaderORinexRead(QFile *RINEX,rinexReader *Reader)
{
    QTextStream outRINEX211_O(RINEX);
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString Marker("MARKER NAME");
    QString MarkNumber("MARKER NUMBER");
    QString Observer("OBSERVER / AGENCY");
    QString RecCharects("REC # / TYPE / VERS");
    QString ANTCharects("ANT # / TYPE");
    QString APRXPosition("APPROX POSITION XYZ");
    QString AntDelta("ANTENNA: DELTA H/E/N");
    QString TypeObs("# / TYPES OF OBSERV");
    QString WavelengthF("WAVELENGTH FACT L1/2");
    QString Interval("INTERVAL");
    QString TimeFirstObs("TIME OF FIRST OBS");
    QString TimeLastObs("TIME OF LAST OBS");
    QString GloSlot("GLONASS SLOT / FRQ #");
    QString RCVClockOffs("RCV CLOCK OFFS APPL");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
    //строки названий
    QString progName;
    QString AggencyName;
    QString MrkNme;
    QString MrkNmb;
    QString ObsName;
    QString NumbRes;
    QString TypeRes;
    QString VerRes;
    QString NumbAnt;
    QString TypeAnt;

    progName = converterJps::Inf.at(0);
    AggencyName = converterJps::Inf.at(1);
    MrkNme = converterJps::Inf.at(2);
    MrkNmb = converterJps::Inf.at(3);
    NumbRes = converterJps::Inf.at(4);
    TypeRes = converterJps::Inf.at(5);
    VerRes = converterJps::Inf.at(6);
    NumbAnt = converterJps::Inf.at(7);
    TypeAnt = converterJps::Inf.at(8);

    quint32 NumberR=0;
    bool FlagNextString = false;

    double ApprX = 0;
    double ApprY = 0;
    double ApprZ = 0;

    QDate FirstD = Reader->timeOfFirst->date();
    QTime FirstT = Reader->timeOfFirst->time();
    QDate LastD = Reader->timeOfLast->date();
    QTime LastT = Reader->timeOfLast->time();

    //время первой эпохи
    quint32 FirstYear =  FirstD.year();
    quint32 FirstMonth =  FirstD.month();
    quint32 FirstDay = FirstD.day();
    quint32 FirstHour = FirstT.hour();
    quint32 FirstMinute =  FirstT.minute();
    qreal FirstSeconds = FirstT.second()+FirstT.msec()/1000.0;

    //время последней эпохи
    quint32 LastYear = LastD.year();
    quint32 LastMonth = LastD.month();
    quint32 LastDay = LastD.day();
    quint32 LastHour = LastT.hour();
    quint32 LastMinute = LastT.minute();
    qreal LastSeconds = LastT.second()+FirstT.msec()/1000.0;

    ApprX = Reader->X;
    ApprY = Reader->Y;
    ApprZ = Reader->Z;

    qreal IntervalCount;
    IntervalCount = Reader->Interval.toDouble();

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy HH:mm");
    //получить текущую дату, выраженную в секундах

    //ЗАПИСЬ ФАЙЛА. ЗАГОЛОВОК
    outRINEX211_O << qSetFieldWidth(60)<< "     2.11           OBSERVATION DATA    M (MIXED)           " << qSetFieldWidth(20)<< TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX211_O  <<left<< qSetFieldWidth(20)<< progName<< qSetFieldWidth(20)<<AggencyName <<qSetFieldWidth(20)<<  CurrentDate << qSetFieldWidth(0)<<PGMDATE << qSetFieldWidth(0) << endl;
    outRINEX211_O <<"       G = GPS       R = GLONASS                            "<<Comment<< qSetFieldWidth(0) << endl;
    outRINEX211_O <<left<< qSetFieldWidth(60)<< MrkNme<< qSetFieldWidth(0)<<Marker<< qSetFieldWidth(0) << endl;
    outRINEX211_O <<left<< qSetFieldWidth(60)<<MrkNmb<< qSetFieldWidth(0)<<MarkNumber<< qSetFieldWidth(0) << endl;
    outRINEX211_O <<left<< qSetFieldWidth(20)<<ObsName<<
    left<< qSetFieldWidth(40)<<AggencyName<< qSetFieldWidth(0)<<Observer<<qSetFieldWidth(0) << endl;
    outRINEX211_O <<left<< qSetFieldWidth(20)<<NumbRes<<
    left<< qSetFieldWidth(20)<< TypeRes <<
    left<< qSetFieldWidth(20)<< VerRes<< qSetFieldWidth(0)<<RecCharects<<qSetFieldWidth(0) << endl;
    outRINEX211_O <<left << qSetFieldWidth(20)<<NumbAnt<<
    left<< qSetFieldWidth(40)<< TypeAnt <<  qSetFieldWidth(0)   <<  ANTCharects<<qSetFieldWidth(0)  << endl;
    outRINEX211_O << right  <<  fixed   <<  qSetFieldWidth(14)  <<  qSetRealNumberPrecision(4)      <<ApprX;
    outRINEX211_O << right  <<  qSetFieldWidth(14)<<    qSetRealNumberPrecision(4)  <<ApprY;
    outRINEX211_O << right  <<  qSetFieldWidth(14)<<    qSetRealNumberPrecision(4)  <<ApprZ;
    outRINEX211_O << left   <<  qSetFieldWidth(18)<<    " " <<APRXPosition<<qSetFieldWidth(0)   << endl;
    outRINEX211_O << right  <<  qSetFieldWidth(14)<<    qSetRealNumberPrecision(4)  <<Reader->H;
    outRINEX211_O << right  <<  qSetFieldWidth(14)<<    qSetRealNumberPrecision(4)  <<Reader->E;
    outRINEX211_O << right  <<  qSetFieldWidth(14)<<    qSetRealNumberPrecision(4)  <<Reader->N;
    outRINEX211_O << left   <<  qSetFieldWidth(18)<<    " " <<AntDelta<<qSetFieldWidth(0)       << endl;


    /*ФОРМИРОВАНИЕ СТРОК "TYPES OF OBSERV"
     *Т.к RINEX версии 2.xx не поддерживает раздельные типы измерений для GPS и GLONASS
     *выбирается МАКСИМАЛЬНОЕ количество типов измерений (из 2х вариантов).
     *При формировании строки типов, для добавления типа ДОСТАТОЧНО наличая измерений для ОДНОЙ системы ГНСС.
     *Пример: В файле RINEX 3.xx есть тип данных L2P для GPS но нет для ГЛОНАСС.
     *После конвертации в файле RINEX 2.xx будет тип измерений L2
     */
    int missingTypes = 0;
    int availableTypes = 0;
    int NofTy = 0;
        int numbOfSpaceInStr = 9*6;

    if(Reader->NumbInOrderTypepsObsGPS[0]>=Reader->NumbInOrderTypepsObsGLO[0]){
        availableTypes = Reader->NumbInOrderTypepsObsGPS[0];
    }
    else{
        availableTypes = Reader->NumbInOrderTypepsObsGLO[0];
    }
    outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;
    if((Reader->TypesOfObsCheckGPS[1])|Reader->TypesOfObsCheckGLO[1])
    {outRINEX211_O << qSetFieldWidth(6)  <<  "C1";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[5])|(Reader->TypesOfObsCheckGLO[5]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "P1";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[2])|(Reader->TypesOfObsCheckGLO[2]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "L1";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[3])|(Reader->TypesOfObsCheckGLO[3]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "D1";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[4])|(Reader->TypesOfObsCheckGLO[4]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "S1";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[13 + 4])|(Reader->TypesOfObsCheckGLO[13])) //19.12.19: Redaction [13 + 4] add "TYPES OF OBSERV" C1W...
    {outRINEX211_O << qSetFieldWidth(6)  <<  "P2";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[14 + 4])|(Reader->TypesOfObsCheckGLO[14]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "L2";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[15 + 4])|(Reader->TypesOfObsCheckGLO[15]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "D2";
    NofTy++;}
    else{missingTypes++;}
    if((Reader->TypesOfObsCheckGPS[16 + 4])|(Reader->TypesOfObsCheckGLO[16]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "S2";
    NofTy++;}
    else{missingTypes++;}
      /*  if(NofTy==9)
        {    FlagNextString =true;
            if(FlagNextString !=true)
            {
            outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
            outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;
           }
        }*/

        if((Reader->TypesOfObsCheckGPS[17 + 8]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "C5";
    NofTy++;}
    else{missingTypes++;}
 /*   if(NofTy==9 || FlagNextString !=true)
    {    FlagNextString =true;
        outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;}*/
    if((Reader->TypesOfObsCheckGPS[18 + 8]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "L5";
    NofTy++;}
    else{missingTypes++;}
  /*  if(NofTy==9|| FlagNextString !=true)
    {   FlagNextString =true;
        outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;}*/
    if((Reader->TypesOfObsCheckGPS[19 + 8]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "D5";
    NofTy++;}
    else{missingTypes++;}
   /* if(NofTy==9|| FlagNextString !=true)
    {   FlagNextString =true;
        outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;}*/
    if((Reader->TypesOfObsCheckGPS[20 + 8]))
    {outRINEX211_O << qSetFieldWidth(6)  <<  "S5";
    NofTy++;}
    else{missingTypes++;}
   /* if(NofTy==9|| FlagNextString !=true)
    {   FlagNextString =true;
        outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << availableTypes;}*/
    outRINEX211_O << qSetFieldWidth(numbOfSpaceInStr-NofTy*6)  <<  " ";
    outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;

    //количество типов измерений

    rinexReader::countTypeIzmInVersion2 = NofTy - 1;

    //ВЫВОД СТРОКИ "INTERVAL"
    outRINEX211_O << right  <<  qSetFieldWidth(10)<<qSetRealNumberPrecision(3) <<IntervalCount;
    outRINEX211_O << qSetFieldWidth (58)<<Interval<<qSetFieldWidth(0) << endl;
    //ВЫВОД СТРОКИ "TIME OF First OBS"
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << FirstYear;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << FirstMonth;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << FirstDay;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << FirstHour;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << FirstMinute;
    outRINEX211_O << qSetFieldWidth(13) <<qSetRealNumberPrecision(7)    << FirstSeconds;
    outRINEX211_O << qSetFieldWidth(8)  <<qSetRealNumberPrecision(0)    << "GPS" <<qSetFieldWidth(9)<< " ";
    outRINEX211_O << qSetFieldWidth(9)  <<TimeFirstObs<<qSetFieldWidth(0)   << endl;
    //ВЫВОД СТРОКИ "TIME OF LAST OBS"
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0) << LastYear<<qSetFieldWidth(6)<< qSetRealNumberPrecision(0);
    outRINEX211_O << LastMonth;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << LastDay;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << LastHour;
    outRINEX211_O << qSetFieldWidth(6)  <<qSetRealNumberPrecision(0)    << LastMinute;
    outRINEX211_O << qSetFieldWidth(13) <<qSetRealNumberPrecision(7)    << LastSeconds;
    outRINEX211_O << qSetFieldWidth(8)  <<qSetRealNumberPrecision(0)    << "GPS"<< qSetFieldWidth(9)<< " ";
    outRINEX211_O << qSetFieldWidth(9)  <<TimeLastObs<<qSetFieldWidth(0)    << endl;

    //Вычисление числа спутников ГЛОНАСС наблюдаемых в сеансе
    for (int i = 0; i<40; i++){
        //Если значение литеры не равно 255(маркер отсутствия данных), увеличиваем на 1 переменную NumberR
        if(Reader->lit[i]!=255){
            NumberR++;
        }
    }
    //Запись числа наблюдаемых спутников ГЛОНАСС
    if (NumberR != 0)
    {outRINEX211_O<<right<<qSetFieldWidth(3)<<NumberR;}
    //Объевляем и иницализируем служебные переменные
    int k =0,l=0,check =0;
    //Открываем цикл для записи литер
    for (int i = 1; i<40; i++)
    {
        //Если значение литеры не равно 255(маркер отсутствия данных)
        if(Reader->lit[i]!=255)
        {   if(i<9){
            outRINEX211_O<< right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i) << qSetFieldWidth(3)<<Reader->lit[i];
            k++;}
            if(i>=9){
            outRINEX211_O<<qSetFieldWidth(3)<<right << qSetFieldWidth(2)<<"R"<< qSetFieldWidth(0) << ToStandartNum(i)<< qSetFieldWidth(3)<<Reader->lit[i];
            k++;}
            if(k%8==0){
            outRINEX211_O << qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;
            l++;
            outRINEX211_O << right <<qSetFieldWidth(3)<<" ";}}
    }
    if(k%8!=0)
    {check = (8-(k-l*8))*7;
     outRINEX211_O << qSetFieldWidth(check) <<" "<< qSetFieldWidth(0) <<" "<< qSetFieldWidth(20) <<GloSlot<< qSetFieldWidth(0) << endl;}

    //ВЫВОД ПОСЛЕДНЕЙ СТРОКИ ЗАГОЛОВКА
    outRINEX211_O << qSetFieldWidth(0) << left << qSetFieldWidth(60)<<" " <<qSetFieldWidth(0)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

// функция создания и записи в файл header Rinex Obs .О версии  2.11
void Epoch::WriteRNX211HeaderO(QFile *RINEX, double deltaH, double deltaN, double deltaE, int NumbOFTypes,
                                 bool Doppler,jpsReader *Reader)
{
    QTextStream outRINEX211_O(RINEX);
    //строки КОМЕНТАРИЕВ, которые могут быть в заголовке файла
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString Marker("MARKER NAME");
    QString MarkNumber("MARKER NUMBER");
    QString Observer("OBSERVER / AGENCY");
    QString RecCharects("REC # / TYPE / VERS");
    QString ANTCharects("ANT # / TYPE");
    QString APRXPosition("APPROX POSITION XYZ");
    QString AntDelta("ANTENNA: DELTA H/E/N");
    QString TypeObs("# / TYPES OF OBSERV");
    QString WavelengthF("WAVELENGTH FACT L1/2");
    QString Interval("INTERVAL");
    QString TimeFirstObs("TIME OF FIRST OBS");
    QString TimeLastObs("TIME OF LAST OBS");
    QString GloSlot("GLONASS SLOT / FRQ #");
    QString RCVClockOffs("RCV CLOCK OFFS APPL");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
    //строки названий
    QString progName;
    QString AggencyName;
    QString MrkNme;
    QString MrkNmb;
    QString ObsName;
    QString NumbRes;
    QString TypeRes;
    QString VerRes;
    QString NumbAnt;
    QString TypeAnt;

    progName    = converterJps::Inf.at(0);
    AggencyName = converterJps::Inf.at(1);
    MrkNme      = converterJps::Inf.at(2);
    MrkNmb      = converterJps::Inf.at(3);
    NumbRes     = converterJps::Inf.at(4);
    TypeRes     = converterJps::Inf.at(5);
    VerRes      = converterJps::Inf.at(6);
    NumbAnt     = converterJps::Inf.at(7);
    TypeAnt     = converterJps::Inf.at(8);

    quint32 NumberR=0;

    double ApprX = 0;
    double ApprY = 0;
    double ApprZ = 0;

    //время первой эпохи
    quint32 FirstYear =  Reader->getYear(0);
    quint32 FirstMonth =  Reader->getMonth(0);
    quint32 FirstDay = Reader->getDay(0);
    quint32 FirstHour = Reader->getHour(0);
    quint32 FirstMinute =  Reader->getMinute(0);
    qreal   FirstSeconds = Reader->getSecond(0);

    //время последней эпохи
    quint32 LastYear = Reader->getYear(Reader->numEp-1);
    quint32 LastMonth = Reader->getMonth(Reader->numEp-1);
    quint32 LastDay = Reader->getDay(Reader->numEp-1);
    quint32 LastHour = Reader->getHour(Reader->numEp-1);
    quint32 LastMinute = Reader->getMinute(Reader->numEp-1);
    qreal   LastSeconds = Reader->getSecond(Reader->numEp-1);

    if(ApprX==0){
        ApprX = Reader->getApproxX(0);
    }
    if(ApprY==0){
        ApprY = Reader->getApproxY(0);
    }
    if(ApprZ==0){
        ApprZ = Reader->getApproxZ(0);
    }

    if(ApprX==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxX(i);
        }
        temp = temp/Reader->numEp;
        ApprX = temp;
    }
    if(ApprY==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxY(i);
        }
        temp = temp/Reader->numEp;
        ApprY = temp;
    }
    if(ApprZ==1){
        qreal temp = 0;
        for(int i =0;i<Reader->numEp;i++){
            temp += Reader->getApproxZ(i);
        }
        temp = temp/Reader->numEp;
        ApprZ = temp;
    }

    qreal iner1 = 0;
    qreal iner2 = 0;
    for(int i =0;i<Reader->numEp-1;i++){
        if(Reader->getDay(i)== Reader->getDay(+1)& Reader->getMonth(i)== Reader->getMonth(i+1)){
            iner1 = Reader->getSecond(i) + Reader->getMinute(i)*60.0 + Reader->getHour(i)*1440.0;
            iner2 = Reader->getSecond(i+1) + Reader->getMinute(i+1)*60.0 + Reader->getHour(i+1)*1440.0;
        break;
        }
    }
    qreal IntervalCount;
    IntervalCount = iner2-iner1;
    NumbOFTypes = 7;

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy HH:mm");
    //получить текущую дату, выраженную в секундах

    //ЗАПИСЬ ФАЙЛА. ЗАГОЛОВОК
    outRINEX211_O << qSetFieldWidth(60)<< "     2.11           OBSERVATION DATA    M (MIXED)           " << qSetFieldWidth(20)<< TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX211_O  <<left<< qSetFieldWidth(20)<< progName<< qSetFieldWidth(20)<<AggencyName <<qSetFieldWidth(20)<<  CurrentDate << qSetFieldWidth(0)<<PGMDATE << qSetFieldWidth(0) << endl;
    outRINEX211_O <<"       G = GPS       R = GLONASS                            "<<Comment<< qSetFieldWidth(0) << endl;
    outRINEX211_O <<left<< qSetFieldWidth(60)<< MrkNme<< qSetFieldWidth(0)<<Marker<< qSetFieldWidth(0) << endl;//
    outRINEX211_O <<left<< qSetFieldWidth(60)<<MrkNmb<< qSetFieldWidth(0)<<MarkNumber<< qSetFieldWidth(0) << endl;//
    outRINEX211_O <<left<< qSetFieldWidth(20)<<ObsName<<
    left<< qSetFieldWidth(40)<<AggencyName<< qSetFieldWidth(0)<<Observer<<qSetFieldWidth(0) << endl;//
    outRINEX211_O <<left<< qSetFieldWidth(20)<<NumbRes<<
    left<< qSetFieldWidth(20)<< TypeRes <<
    left<< qSetFieldWidth(20)<< VerRes<< qSetFieldWidth(0)<<RecCharects<<qSetFieldWidth(0) << endl;//
    outRINEX211_O <<left << qSetFieldWidth(20)<<NumbAnt<<
    left<< qSetFieldWidth(40)<< TypeAnt << qSetFieldWidth(0)<< ANTCharects<<qSetFieldWidth(0) << endl;//
    outRINEX211_O << right<<fixed<<  qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprX; //
    outRINEX211_O << right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprY; //
    outRINEX211_O << right<< qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<ApprZ; //
    outRINEX211_O << left << qSetFieldWidth(18)<<" "<<APRXPosition<<qSetFieldWidth(0) << endl;//
    outRINEX211_O << right<<qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<deltaH; //
    outRINEX211_O << right<<qSetFieldWidth(14)<<qSetRealNumberPrecision(4) <<deltaE; //
    outRINEX211_O << right<<qSetFieldWidth(14)<<qSetRealNumberPrecision(4)<<deltaN; //
    outRINEX211_O << left << qSetFieldWidth(18)<<" "<<AntDelta<<qSetFieldWidth(0) << endl;//


    //ВЫВОД СТРОКИ "# / TYPES OF OBSERV"
    if(Doppler){
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << 9;
        outRINEX211_O << qSetFieldWidth(6)  <<  "C1"
                      << qSetFieldWidth(6)  <<  "P1"
                      << qSetFieldWidth(6)  <<  "L1"
                      << qSetFieldWidth(6)  <<  "D1"
                      << qSetFieldWidth(6)  <<  "S1"
                      << qSetFieldWidth(6)  <<  "P2"
                      << qSetFieldWidth(6)  <<  "L2"
                      << qSetFieldWidth(6)  <<  "D2"
                      << qSetFieldWidth(6)  <<  "S2";
    }
    else{
        outRINEX211_O << right  <<  qSetFieldWidth(6)  <<  qSetRealNumberPrecision(0)  << 5;
        outRINEX211_O << right  << qSetFieldWidth(6)  <<  "C1"
                      << qSetFieldWidth(6)  <<  "P1"
                      << qSetFieldWidth(6)  <<  "L1"
                      << qSetFieldWidth(6)  <<  "P2"
                      << qSetFieldWidth(6)  <<  "L2";
        outRINEX211_O << right  << qSetFieldWidth(6)  <<  " "
                      << qSetFieldWidth(6)  <<  " "
                      << qSetFieldWidth(6)  <<  " "
                      << qSetFieldWidth(6)  <<  " ";
    }
    outRINEX211_O << left   <<  TypeObs<<qSetFieldWidth(0) << endl;
    //ВЫВОД СТРОКИ "INTERVAL"
    outRINEX211_O << right<<qSetFieldWidth(10)<<qSetRealNumberPrecision(3) <<IntervalCount;
    outRINEX211_O << qSetFieldWidth (58)<<Interval<<qSetFieldWidth(0) << endl;
    //TIME OF First OBS
    outRINEX211_O << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << FirstYear;
    outRINEX211_O << qSetFieldWidth(6) << qSetRealNumberPrecision(0) << FirstMonth;
    outRINEX211_O << qSetFieldWidth(6) << qSetRealNumberPrecision(0)<< FirstDay;
    outRINEX211_O << qSetFieldWidth(6) << qSetRealNumberPrecision(0)<< FirstHour;
    outRINEX211_O << qSetFieldWidth(6) << qSetRealNumberPrecision(0)<< FirstMinute;
    outRINEX211_O << qSetFieldWidth(13)<< qSetRealNumberPrecision(7) << FirstSeconds;
    outRINEX211_O << qSetFieldWidth(8) << qSetRealNumberPrecision(0)<< "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX211_O << qSetFieldWidth (9) <<TimeFirstObs<<qSetFieldWidth(0) << endl;
    //TIME OF LAST OBS
    outRINEX211_O << qSetFieldWidth(6)<<qSetRealNumberPrecision(0) << LastYear<<qSetFieldWidth(6)<< qSetRealNumberPrecision(0);
    outRINEX211_O << LastMonth;
    outRINEX211_O << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< LastDay;
    outRINEX211_O << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<< LastHour;
    outRINEX211_O << qSetFieldWidth(6)<<qSetRealNumberPrecision(0)<<LastMinute;
    outRINEX211_O << qSetFieldWidth(13)<< qSetRealNumberPrecision(7) << LastSeconds;
    outRINEX211_O << qSetFieldWidth(8)<<qSetRealNumberPrecision(0)<< "GPS"<<qSetFieldWidth(9)<< " ";
    outRINEX211_O << qSetFieldWidth (9)<<TimeLastObs<<qSetFieldWidth(0) << endl;

    //Вычисление числа спутников ГЛОНАСС наблюдаемых в сеансе
    for (int i = 0; i<40; i++){
        //Если значение литеры не равно 255(маркер отсутствия данных), увеличиваем на 1 переменную NumberR
        if(lit[i]!=255){
            NumberR++;
        }
    }
    //Запись числа наблюдаемых спутников ГЛОНАСС

    //Конец заголовка
    outRINEX211_O<< qSetFieldWidth(0) << left << qSetFieldWidth(60)<<" " <<qSetFieldWidth(0)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

void Epoch::setLitGlo(jpsReader *Reader){
    for(int i=0;i<40;i++){
        lit[i]=255;
    }
    for(int i=0; i<Reader->ANE.length(); i++){
        int tempNum;
        int tempLit;
        tempNum = Reader->getSatNum(i);
        tempLit = Reader->getSatLit(i);
        if(tempNum<40){
            if(tempLit>-8&tempLit<7){
                lit[tempNum] = tempLit;
            }
        }
    }
}
