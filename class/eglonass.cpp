#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include "eglonass.h"
#include "jpsreader.h"
#include "formnumlib.h"
#include "converterjps.h"
#include "rinexreader.h"

using namespace formNum;
//Конструктор класса при конвертации из .jps
EphemeridGLO::EphemeridGLO(jpsReader *Reader, qint32 Num)
{
    inputUTCtime=false;

TauN = - Reader->getTauN(Num);                  //Сдвиг часов спутника (cекунды)
GammaN = Reader->getGammaN(Num);                //Относительный сдвиг частоты
tk = Reader->getTk(Num);                        //Время сообщения
Xcord = Reader->getXcord(Num);                  //X координата спутника
Xspeed = Reader->getXspeed(Num);                //скорость по X
Xacceleration = Reader->getXacc(Num);           //ускорение по X
IsOk = Reader->getIsOk(Num);                    //Исправность
Ycord = Reader->getYcord(Num);                  //Y координата спутника
Yspeed = Reader->getYspeed(Num);                //скорость по Y
Yacceleration = Reader->getYacc(Num);           //ускорение по Y
NumbFreqChan = Reader->getSatLit(Num);          //Номер частотного канала
Zcord = Reader->getZcord(Num);                  //Z координата спутника
Zspeed = Reader->getZspeed(Num);                //скорость по Z
Zacceleration = Reader->getZacc(Num);           //ускорение по Z
AgeOfInf = Reader->getAgeOfInf(Num);            //Возраст информации
NumYearGLO = Reader->getNumYearGLO(Num);        //Номер текущего года в четырёхлетнем цикле (мб день????)
NumSecondGLO = Reader->getNumSecondGLO(Num);    //Секунда в текущем году
NumSatGLO = Reader-> getSatNum(Num);
NumYearGLO_0 = Reader->getNumYearGLO(0);        //Номер текущего года в четырёхлетнем цикле на первую эпоху
NumSecondGLO_0 = Reader->getNumSecondGLO(0);    //Секунда в текущем году на первую эпоху
Year = Reader->year;                            // Год
}

//Конструктор класса при конвертации из RINEX
EphemeridGLO::EphemeridGLO (QStringList *AList, quint8 Version) : alist{AList}
{
    inputUTCtime = true;

    int p1;
    int p2;
    int p3;
    int p4;

    if(Version==2){
        p1 = 3;
        p2 = 22;
        p3 = 41;
        p4 = 60;
    }
    if(Version==3){
        p1 = 4;
        p2 = 23;
        p3 = 42;
        p4 = 61;
    }

    bool ok;
    //Сдвиг часов спутника (cекунды)
    QString Buff = AList->at(0);
    Buff = Buff.mid(p2,19);
    TauN  = StrToD(Buff);

    //Относительный сдвиг частоты
    QString Buff2 = AList->at(0);
    Buff2 = Buff2.mid(p3,19);
    GammaN = StrToD(Buff2);
    QString Buff3 = AList->at(0);
    Buff3 = Buff3.mid(p4,19);
    //Время сообщения
    tk  = StrToD(Buff3);

    QString Buff4 = AList->at(1);
    Buff4 = Buff4.mid(p1,19);
    //X координата спутника
    Xcord  = StrToD(Buff4);
    QString Buff5 = AList->at(1);
    Buff5 = Buff5.mid(p2,19);
    //скорость по X
    Xspeed  = StrToD(Buff5);
    QString Buff6 = AList->at(1);
    Buff6 = Buff6.mid(p3,19);
    //ускорение по X
    Xacceleration  = StrToD(Buff6);
    QString Buff7 = AList->at(1);
    Buff7 = Buff7.mid(p4,19);
    //Исправность
    IsOk  = StrToD(Buff7);

    QString Buff8 = AList->at(2);
    Buff8 = Buff8.mid(p1,19);
    //Y координата спутника
    Ycord  = StrToD(Buff8);
    QString Buff9 = AList->at(2);
    Buff9 = Buff9.mid(p2,19);
    //скорость по Y
    Yspeed  = StrToD(Buff9);
    QString Buff10 = AList->at(2);
    Buff10 = Buff10.mid(p3,19);
    //ускорение по Y
    Yacceleration  = StrToD(Buff10);
    QString Buff11 = AList->at(2);
    Buff11 = Buff11.mid(p4,19);
    //Номер частотного канала
    NumbFreqChan  = StrToD(Buff11);

    QString Buff12 = AList->at(3);
    Buff12 = Buff12.mid(p1,19);
    //Z координата спутника
    Zcord  = StrToD(Buff12);
    QString Buff13 = AList->at(3);
    Buff13 = Buff13.mid(p2,19);
    //скорость по Z
    Zspeed  = StrToD(Buff13);
    QString Buff14 = AList->at(3);
    Buff14 = Buff14.mid(p3,19);
    //ускорение по Z
    Zacceleration  = StrToD(Buff14);
    QString Buff15 = AList->at(3);
    Buff15 = Buff15.mid(p4,19);
    //Возраст информации
    AgeOfInf  = StrToD(Buff15);

    //NumYearGLO = Reader->getNumYearGLO(Num);        //Номер текущего года в четырёхлетнем цикле (мб день????)
    //NumSecondGLO = Reader->getNumSecondGLO(Num);    //Секунда в текущем году
    if (Version==2)
    {
        QString BuffN = AList->at(0);
        BuffN = BuffN.mid(0,2);
        NumSatGLO  = BuffN.toInt(&ok, 10);
        QString BuffT = AList->at(0);
        QString BuffY = BuffT.mid(3,2);
        QString BuffM = BuffT.mid(6,2);
        QString BuffD = BuffT.mid(9,2);
        QString BuffH = BuffT.mid(12,2);
        QString BuffMi = BuffT.mid(15,2);
        QString BuffS = BuffT.mid(18,4);
        YearEphInput = BuffY.toInt(&ok, 10)+2000;
        MonthEphInput = BuffM.toInt(&ok, 10);
        DayEphInput = BuffD.toInt(&ok, 10);
        HourEphInput = BuffH.toInt(&ok, 10);
        MinEphInput = BuffMi.toInt(&ok, 10);
        SecEphInput = BuffS.toDouble();
    }

    if(Version==3)
    {
        QString BuffN = AList->at(0);
        BuffN = BuffN.mid(1,2);
        NumSatGLO  = BuffN.toInt(&ok, 10);
        QString BuffT = AList->at(0);
        QString BuffY = BuffT.mid(4,4);
        QString BuffM = BuffT.mid(9,2);
        QString BuffD = BuffT.mid(12,2);
        QString BuffH = BuffT.mid(15,2);
        QString BuffMi = BuffT.mid(18,2);
        QString BuffS = BuffT.mid(21,2);
        YearEphInput = BuffY.toInt(&ok, 10)-2000;
        MonthEphInput = BuffM.toInt(&ok, 10);
        DayEphInput = BuffD.toInt(&ok, 10);
        HourEphInput = BuffH.toInt(&ok, 10);
        MinEphInput = BuffMi.toInt(&ok, 10);
        SecEphInput = BuffS.toDouble();
    }
    //NumYearGLO_0 = Reader->getNumYearGLO(0);        //Номер текущего года в четырёхлетнем цикле на первую эпоху
    //NumSecondGLO_0 = Reader->getNumSecondGLO(0);    //Секунда в текущем году на первую эпоху
} //КОНЕЦ КОНСТРУКТОРА

// функция создания и записи в файл header Rinex GLONASS NAV DATA версии 2.11
void EphemeridGLO::WriteRNX211HeaderN(QFile *RINEX)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString CorrToSys("CORR TO SYSTEM TIME");
    QString LeapSec("LEAP SECONDS");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    int LeapSeconds=0;
    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy hh:mm");
    QTextStream outRINEX(RINEX);
    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     2.11           G                  ";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20) <<progName;
    outRINEX << qSetFieldWidth(20)<<  AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    /*
    outRINEX << "  "<<qSetFieldWidth(9)<<yearCorr<<qSetFieldWidth(5)<<monthCorr<<qSetFieldWidth(6)<<dayCorr<<right<<qSetFieldWidth(11)<<dToStr12DObs(SecCorr);
    outRINEX << left<<qSetFieldWidth(20)<<" "<<qSetFieldWidth(20)<<CorrToSys<< qSetFieldWidth(0) << endl;
    outRINEX << "     "<<left<<qSetFieldWidth(55)<<LeapSeconds<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    */
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}   //Конец метода записи заголовка файла

// функция создания и записи в файл header Rinex GNSS NAV DATA версии 3.02
void EphemeridGLO::WriteRNX302HeaderN(QFile *RINEX){
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonCorr("IONOSPHERIC CORR");
    QString TimeCorr("TIME SYSTEM CORR");
    QString LeapSec("LEAP SECONDS");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);

    /*
    qreal A0 = 0.000034567876;
    qreal A1 = 0.0000000000000023456;
    qint32 W = 236;
    qint32 T = 753642;

    qreal A0_GL = 0.000034567876;
    qreal A1_GL = 0.0000000000000023456;
    qint32 W_GL = 236;
    qint32 T_GL = 753642;
    */

    int LeapSeconds=0;
    // получить текущую дату, выраженную в секундах

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("yyyyMMdd hhmmss");
    CurrentDate .append(" UTC");
    QTextStream outRINEX(RINEX);

    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(20)<< "     3.02           ";
    outRINEX << qSetFieldWidth(20)<< "N: GNSS NAV DATA   ";
    outRINEX << qSetFieldWidth(20)<< "R: GLONASS";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20)<<progName;
    outRINEX << qSetFieldWidth(20)<<AggencyName;
    outRINEX << qSetFieldWidth(20)<<CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    /*
    outRINEX << "GPUT  "<< qSetFieldWidth(17)<<dToStr10E(A0)<< qSetFieldWidth(17)<<dToStr10E(A1)<< qSetFieldWidth(7)<<right<<T<< qSetFieldWidth(5)<<W<<left<< qSetFieldWidth(6)<<" ";
    outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<TimeCorr<< qSetFieldWidth(0) << endl;
    outRINEX << "GLUT  "<< qSetFieldWidth(17)<<dToStr10E(A0_GL)<< qSetFieldWidth(17)<<dToStr10E(A1_GL)<< qSetFieldWidth(7)<<right<<T_GL<< qSetFieldWidth(5)<<W_GL<<left<< qSetFieldWidth(6)<<" ";
    outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<TimeCorr<< qSetFieldWidth(0) << endl;
    outRINEX << "     "<<left<<qSetFieldWidth(55)<<LeapSeconds<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    */
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

//запись в файл .N версии 211 эфемерид
void EphemeridGLO::WriteRNXEphN_211(QFile *RINEX)
{
    if( NumSatGLO != 255)
    {
        //quint32 Int = NumSatGLO;
        QString NameofSat;
        if( NumSatGLO < 10 )
        {
            NameofSat = QString("0%1").arg(NumSatGLO);
        }

        if( NumSatGLO >= 10 )
        {
            NameofSat = QString("%1").arg(NumSatGLO);
        }

        quint32 YearEph=0;
        quint32 MonthEph=0;
        quint32 DayEph=0;
        quint32 HourEph=0;
        quint32 MinEph=0;
        qreal SecEph=0;

        quint32 YearEph0=0;
        quint32 MonthEph0=0;
        quint32 DayEph0=0;
        quint32 HourEph0=0;
        quint32 MinEph0=0;
        qreal SecEph0=0;

        qreal DayTK;

        if ( inputUTCtime )
        {
            YearEph = YearEphInput;
            if( YearEph >= 2000 )
                YearEph = YearEph - 2000;
            if( YearEph < 2000 && YearEph >= 1900 )
                YearEph = YearEph - 1900;
            MonthEph = MonthEphInput;
            DayEph = DayEphInput;
            HourEph = HourEphInput;
            MinEph = MinEphInput;
            SecEph = SecEphInput;
        }

        else
        {

            DateUTC A = ConvertGLODatToUTC(NumYearGLO, NumSecondGLO,Year);
            YearEph = A.Date[0];
            if( YearEph >= 2000 )
                YearEph = YearEph - 2000;
            if( YearEph < 2000 && YearEph >= 1900 )
                YearEph = YearEph - 1900;
            MonthEph = A.Date[1];
            DayEph = A.Date[2];
            HourEph = A.Date[3];
            MinEph = A.Date[4];
            SecEph = A.Seconds;

            DateUTC Date0 = ConvertGLODatToUTC(NumYearGLO_0, NumSecondGLO_0,Year);
            YearEph0 = Date0.Date[0];
            MonthEph0 = Date0.Date[1];
            DayEph0 = Date0.Date[2];
            HourEph0 = Date0.Date[3];
            MinEph0 = Date0.Date[4];
            SecEph0 = Date0.Seconds;
            DayTK = FromUTCDatToUTCDayofWeek(YearEph0, MonthEph0,DayEph0);
            tk = tk+DayTK*86400-3*3600;

            if( tk < 0 )
            {
                tk=604800+tk;
            }
        }

        //Запись данных. 2) Временные параметры

        QString SecEphStr;
        SecEphStr = SecEphStr.setNum(SecEph, 'f', 1);

        QTextStream outRINEX(RINEX);
        //Запись данных. 1) Имя спутника
        outRINEX<<right<<fixed<<qSetFieldWidth(0)<<qSetRealNumberPrecision(0) << NameofSat;
        outRINEX<<qSetFieldWidth(3)<< qSetRealNumberPrecision(0) << YearEph;
        outRINEX<<qSetFieldWidth(3) << ToStandartNum(MonthEph);
        outRINEX<<qSetFieldWidth(3) << DayEph;
        outRINEX<<qSetFieldWidth(3) << HourEph;
        outRINEX<<qSetFieldWidth(3) << MinEph;
        outRINEX<<qSetFieldWidth(5) << SecEphStr;
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(TauN);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(GammaN);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(tk);
        outRINEX<<qSetFieldWidth(0) << endl;
        //Запись данных. 3) параметры эфемерид и альманаха ГЛОНАСС по эпохе
        outRINEX<<qSetFieldWidth(0) << "   ";
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xcord);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xspeed);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xacceleration);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(IsOk);
        outRINEX<<qSetFieldWidth(0) << endl;
        outRINEX<<qSetFieldWidth(0) << "   ";
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Ycord);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Yspeed);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Yacceleration);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(NumbFreqChan);
        outRINEX<<qSetFieldWidth(0) << endl;
        outRINEX<<qSetFieldWidth(0) << "   ";
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zcord);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zspeed);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zacceleration);
        outRINEX<<qSetFieldWidth(19)<< dToStr12D(AgeOfInf);
        outRINEX<<qSetFieldWidth(0) << endl;
        //}
    }

}

//запись в файл .N версии 302 эфемерид
void EphemeridGLO::WriteRNXEphN_302(QFile *RINEX)
{
    if(NumSatGLO!=255)
    {
    QString NameofSat = "R";
    QString Num = QString("%1" ).arg(NumSatGLO);
    if(NumSatGLO<10)
    {NameofSat.append("0");
    NameofSat.append(Num);}
    if(NumSatGLO>=10)
    {NameofSat.append(Num);}

    quint32 YearEph;
    quint32 MonthEph;
    quint32 DayEph;
    quint32 HourEph;
    quint32 MinEph;
    quint32 SecEph;

    quint32 YearEph0;
    quint32 MonthEph0;
    quint32 DayEph0;
    quint32 HourEph0;
    quint32 MinEph0;
    quint32 SecEph0;

    qreal DayTK;

    if (inputUTCtime){
        YearEph = YearEphInput;
        MonthEph = MonthEphInput;
        DayEph = DayEphInput;
        HourEph = HourEphInput;
        MinEph = MinEphInput;
        SecEph = SecEphInput;
    }
    else{
        DateUTC A = ConvertGLODatToUTC(NumYearGLO, NumSecondGLO,Year);          // Теперь без магической константы 2018
        YearEph = A.Date[0];
        MonthEph = A.Date[1];
        DayEph = A.Date[2];
        HourEph = A.Date[3];
        MinEph = A.Date[4];
        SecEph = A.Seconds;
        DateUTC Date0 = ConvertGLODatToUTC(NumYearGLO_0, NumSecondGLO_0,Year);  // Теперь без магической константы 2018
        YearEph0 = Date0.Date[0];
        MonthEph0 = Date0.Date[1];
        DayEph0 = Date0.Date[2];
        HourEph0 = Date0.Date[3];
        MinEph0 = Date0.Date[4];
        SecEph0 = Date0.Seconds;
        DayTK = FromUTCDatToUTCDayofWeek(YearEph0, MonthEph0,DayEph0);
        tk = tk+DayTK*86400-3*3600;
        if(tk<0)
        {tk=604800+tk;}
    }

    QTextStream outRINEX(RINEX);
    outRINEX<<right<<fixed<<qSetFieldWidth(0)<<qSetRealNumberPrecision(0) << NameofSat;
    outRINEX<<qSetFieldWidth(5)<<qSetRealNumberPrecision(0) << YearEph;
    outRINEX<<qSetFieldWidth(3)<< ToStandartNum(MonthEph);
    outRINEX<<qSetFieldWidth(3)<< DayEph;
    outRINEX<<qSetFieldWidth(3)<< HourEph;
    outRINEX<<qSetFieldWidth(3)<< MinEph;
    outRINEX<<qSetFieldWidth(3)<< SecEph;
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(TauN);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(GammaN);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(tk);
    outRINEX<<qSetFieldWidth(0)<< endl;
    outRINEX<<qSetFieldWidth(0)<<"    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xcord);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xspeed);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Xacceleration);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IsOk);
    outRINEX<<qSetFieldWidth(0)<<endl;
    outRINEX<<qSetFieldWidth(0)<<"    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Ycord);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Yspeed);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Yacceleration);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(NumbFreqChan);
    outRINEX<<qSetFieldWidth(0)<<endl;
    outRINEX<<qSetFieldWidth(0)<<"    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zcord);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zspeed);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Zacceleration);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(AgeOfInf);
    outRINEX<<qSetFieldWidth(0)<<endl;
    }  
}


void EphemeridGLO::WriteRNX2HeaderN(QFile *RINEX, rinexReader *Reader)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString CorrToSys("CORR TO SYSTEM TIME");
    QString LeapSec("LEAP SECONDS");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    //дата начала коррекции GLOtime к UTC
    qint16 yearCorr=1998;
        qint16 monthCorr = 2;
            qint16 dayCorr = 16;

    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy hh:mm");
    QTextStream outRINEX(RINEX);
    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     2.11           G                  ";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20) <<progName;
    outRINEX << qSetFieldWidth(20)<<  AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    if(Reader->TgloOn){
    outRINEX << "  "<<qSetFieldWidth(9)<<yearCorr<<qSetFieldWidth(5)<<monthCorr<<qSetFieldWidth(6)<<dayCorr<<right<<qSetFieldWidth(11)<<Reader->a0_TauC;
    outRINEX << left<<qSetFieldWidth(20)<<" "<<qSetFieldWidth(20)<<CorrToSys<< qSetFieldWidth(0) << endl;
    }
    if(Reader->leapSecOn){
    outRINEX << "     "<<left<<qSetFieldWidth(55)<<Reader->LeapSec<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    }
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}   //Конец метода записи заголовка файла

// функция создания и записи в файл header Rinex GNSS NAV DATA версии 3.02
void EphemeridGLO::WriteRNX3HeaderN(QFile *RINEX, rinexReader *Reader)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonCorr("IONOSPHERIC CORR");
    QString TimeCorr("TIME SYSTEM CORR");
    QString LeapSec("LEAP SECONDS");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    // получить текущую дату, выраженную в секундах
    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("yyyyMMdd hhmmss");
    CurrentDate .append(" UTC");
    QTextStream outRINEX(RINEX);

    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(20)<< "     3.02           ";
    outRINEX << qSetFieldWidth(20)<< "N: GNSS NAV DATA   ";
    outRINEX << qSetFieldWidth(20)<< "R: GLONASS";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20)<<progName;
    outRINEX << qSetFieldWidth(20)<<AggencyName;
    outRINEX << qSetFieldWidth(20)<<CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;

    if(Reader->TgpsOn){
        outRINEX << "GPUT  "<< qSetFieldWidth(17)<<Reader->a0<< qSetFieldWidth(17)<<Reader->a1<< qSetFieldWidth(7)<<right<<Reader->T<< qSetFieldWidth(5)<<Reader->W<<left<< qSetFieldWidth(6)<<" ";
        outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<TimeCorr<< qSetFieldWidth(0) << endl;
    }
    if(Reader->TgloOn){
    outRINEX << "GLUT  "<< qSetFieldWidth(17)<<Reader->a0_TauC<< qSetFieldWidth(17)<<dToStr10E(0)<< qSetFieldWidth(7)<<right<<0<< qSetFieldWidth(5)<<0<<left<< qSetFieldWidth(6)<<" ";
    outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<TimeCorr<< qSetFieldWidth(0) << endl;
    }
    if(Reader->leapSecOn){
    outRINEX << "     "<<left<<qSetFieldWidth(55)<<Reader->LeapSec<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    }
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}
