#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QDate>
#include <QTime>
#include "egps.h"
#include "jpsreader.h"
#include "formnumlib.h"
#include "converterjps.h"

using namespace formNum;

bool EphemeridGPS::inputIONcorr=false;

qreal EphemeridGPS::alpha0;
qreal EphemeridGPS::alpha1;
qreal EphemeridGPS::alpha2;
qreal EphemeridGPS::alpha3;

qreal EphemeridGPS::beta0;
qreal EphemeridGPS::beta1;
qreal EphemeridGPS::beta2;
qreal EphemeridGPS::beta3;

qreal EphemeridGPS::A0;
qreal EphemeridGPS::A1;
quint32 EphemeridGPS::W;
quint32 EphemeridGPS::T;

//класс описывающий необходимые значения для описания эфемерид  GPS в файлах RINEX
EphemeridGPS::EphemeridGPS(jpsReader *Reader, int Num)
{
    inputUTCtime = false;

    SVClockBias = Reader->getSVClockBias(Num);                  //Время КА
    SVClockDrift= Reader->getSVClockDrift(Num);                 //Уход часов КА
    SVClockBiasRate= Reader->getSVClockDriftRate(Num);          //Частота смещения часов КА
    IODE= Reader->getIODE(Num);                                 //Проблема данных навигационного сигнала
    Crs= Reader->getCrs(Num);
    DeltaN= Reader->getDeltaN(Num)*3.14159265359;
    M0= Reader->getM0(Num)*3.14159265359;
    Cuc= Reader->getCuc(Num);
    Eccentricity= Reader->getEccentricity(Num);                 //Экцентриситет орбиты
    Cus= Reader->getCus(Num);
    sqrtA= Reader->getSqrtA (Num);
    ToE= Reader->getToE(Num);
    Cic= Reader->getCic(Num);
    Omega0= Reader->getOmega0(Num)*3.14159265359;
    Cis= Reader->getCis(Num);
    i0= Reader->geti0(Num)*3.14159265359;
    Crc= Reader->getCrc(Num);
    Omega= Reader->getOmega(Num)*3.14159265359;
    OmegaDot = Reader->getOmegaDot(Num)*3.14159265359;
    IDOT  = Reader->getIDOT(Num)*3.14159265359;
    SVaccuracy = uraToSVaccuracy(Reader->getSVaccuracy(Num));
    TGD = Reader->getTGD(Num);
    IODC = Reader->getIODC(Num);
    CodesL2 = Reader->getCodesL2(Num);
    L2PdataFlag = Reader->getL2PdataFlag(Num);
    SVhealth = Reader->getSVhealth (Num);
    FitInterval = Reader->getFitInterval (Num);
    GPSWeek = Reader->getGPSWeek(Num);
    GPSSec = Reader-> getToE(Num);
    TransTime = Reader-> getGPSt0e(Num);
    NumSatGPS = Reader-> getSatPRN(Num);

    A0 = SVClockBias;
    A1 = SVClockDrift;

    quint16 Year = Reader->getYear(0);
    quint16 Month = Reader->getMonth(0);
    quint16 Day = Reader->getDay(0);
    quint16 Hour = Reader->getHour(0);
    quint16 Minute = Reader->getMinute(0);
    qreal Second = Reader->getSecond(0);
    QDate temp;
    temp.setDate(Year,Month,Day);
    QTime time;
    quint32 tempSec = Second;
    quint32 tempMSec = Second*1000;
    tempMSec = tempMSec-tempSec*1000;
    time.setHMS(Hour,Minute,tempSec,tempMSec);
    QDateTime *ddd = new QDateTime(temp,time,Qt::OffsetFromUTC);
    if(ddd->isValid())
        timeOfFirst = ddd;
}

//класс описывающий необходимые значения для описания эфемерид  GPS в файлах RINEX
EphemeridGPS::~EphemeridGPS()
{
    if(alist != nullptr)
        delete alist;
    if(timeOfFirst != nullptr)
        delete timeOfFirst;
    timeOfFirst = nullptr;
}

EphemeridGPS::EphemeridGPS(QStringList *AList, int Version) : alist{AList}
{
    bool ok;
    inputUTCtime = true;
    inputIONcorr = false;

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

    //Время КА
    QString Buff = AList->at(0);
    Buff = Buff.mid(p2,19);
    SVClockBias = StrToD(Buff);

    //Уход часов КА
    QString Buff2 = AList->at(0);
    Buff2 = Buff2.mid(p3,19);
    SVClockDrift = StrToD(Buff2);
    //Частота смещения часов КА
    QString Buff3 = AList->at(0);
    Buff3 = Buff3.mid(p4,19);
    SVClockBiasRate= StrToD(Buff3);

    QString Buff4 = AList->at(1);
    Buff4 = Buff4.mid(p1,19);
    IODE= StrToD(Buff4);

    QString Buff5 = AList->at(1);
    Buff5 = Buff5.mid(p2,19);
    Crs= StrToD(Buff5);

    QString Buff6 = AList->at(1);
    Buff6 = Buff6.mid(p3,19);
    DeltaN= StrToD(Buff6);

    QString Buff7 = AList->at(1);
    Buff7 = Buff7.mid(p4,19);
    M0= StrToD(Buff7);

    QString Buff8 = AList->at(2);
    Buff8 = Buff8.mid(p1,19);
    Cuc= StrToD(Buff8);

    QString Buff9 = AList->at(2);
    Buff9 = Buff9.mid(p2,19);
    Eccentricity= StrToD(Buff9);

    QString Buff10 = AList->at(2);
    Buff10 = Buff10.mid(p3,19);
    Cus= StrToD(Buff10);

    QString Buff11 = AList->at(2);
    Buff11 = Buff11.mid(p4,19);
    sqrtA= StrToD(Buff11);

    QString Buff12 = AList->at(3);
    Buff12 = Buff12.mid(p1,19);
    ToE= StrToD(Buff12);

    QString Buff13 = AList->at(3);
    Buff13 = Buff13.mid(p2,19);
    Cic= StrToD(Buff13);

    QString Buff14 = AList->at(3);
    Buff14 = Buff14.mid(p3,19);
    Omega0= StrToD(Buff14);

    QString Buff15 = AList->at(3);
    Buff15 = Buff15.mid(p4,19);
    Cis= StrToD(Buff15);

    QString Buff16 = AList->at(4);
    Buff16 = Buff16.mid(p1,19);
    i0= StrToD(Buff16);

    QString Buff17 = AList->at(4);
    Buff17 = Buff17.mid(p2,19);
    Crc= StrToD(Buff17);

    QString Buff18 = AList->at(4);
    Buff18 = Buff18.mid(p3,19);
    Omega= StrToD(Buff18);

    QString Buff19 = AList->at(4);
    Buff19 = Buff19.mid(p4,19);
    OmegaDot = StrToD(Buff19);

    QString Buff20 = AList->at(5);
    Buff20 = Buff20.mid(p1,19);
    IDOT= StrToD(Buff20);

    QString Buff21 = AList->at(5);
    Buff21 = Buff21.mid(p2,19);
    CodesL2 = StrToD(Buff21) ;

    QString Buff22 = AList->at(5);
    Buff22 = Buff22.mid(p3,19);
    GPSWeek = StrToD(Buff22) ;

    QString Buff23 = AList->at(5);
    Buff23 = Buff23.mid(p4,19);
    L2PdataFlag = StrToD(Buff23);

    QString Buff24 = AList->at(6);
    Buff24 = Buff24.mid(p1,19);
    SVaccuracy= StrToD(Buff24);

    QString Buff25 = AList->at(6);
    Buff25 = Buff25.mid(p2,19);
    SVhealth= StrToD(Buff25);

    QString Buff26 = AList->at(6);
    Buff26 = Buff26.mid(p3,19);
    TGD= StrToD(Buff26);

    QString Buff27 = AList->at(6);
    Buff27 = Buff27.mid(p4,19);
    IODC= StrToD(Buff27);

    QString Buff28 = AList->at(7);
    Buff28 = Buff28.mid(p1,19);
    TransTime= StrToD(Buff28);

    FitInterval = 0;  

    A0 = SVClockBias;
    A1 = SVClockDrift;

    if(Version ==2)
    {
        QString BuffSt = AList->at(0);
        BuffSt = BuffSt.mid(0,2);
        NumSatGPS = BuffSt.toInt(&ok, 10);

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
    if(Version ==3)
    {
        QString BuffSt = AList->at(0);
        BuffSt = BuffSt.mid(1,2);
        NumSatGPS = BuffSt.toInt(&ok, 10);

        QString BuffT = AList->at(0);               //
        QString BuffY = BuffT.mid(4,4);             //
        QString BuffM = BuffT.mid(9,2);             //
        QString BuffD = BuffT.mid(12,2);            //
        QString BuffH = BuffT.mid(15,2);            //
        QString BuffMi = BuffT.mid(18,2);           //
        QString BuffS = BuffT.mid(21,2);            //
        YearEphInput = BuffY.toInt(&ok, 10);        //
        MonthEphInput = BuffM.toInt(&ok, 10);       //
        DayEphInput = BuffD.toInt(&ok, 10);         //
        HourEphInput = BuffH.toInt(&ok, 10);        //
        MinEphInput = BuffMi.toInt(&ok, 10);        //
        SecEphInput = BuffS.toDouble();             //
    }
}

void EphemeridGPS::setIonGpsArguments(jpsReader *Reader,int NumIonLog){
    if(Reader->AIO.length()!=0){
        alpha0 = Reader->getIonAlpha0(NumIonLog);
        alpha1 = Reader->getIonAlpha1(NumIonLog);
        alpha2 = Reader->getIonAlpha2(NumIonLog);
        alpha3 = Reader->getIonAlpha3(NumIonLog);
        beta0 = Reader->getIonBeta0(NumIonLog);
        beta1 = Reader->getIonBeta0(NumIonLog);
        beta2 = Reader->getIonBeta0(NumIonLog);
        beta3 = Reader->getIonBeta0(NumIonLog);

        quint16 Ye = Reader->getYear(0);
        quint16 Mo = Reader->getMonth(0);
        quint16 Da = Reader->getDay(0);
        quint16 Ho = Reader->getHour(0);
        quint16 Min = Reader->getMinute(0);
        qreal Sec = Reader->getSecond(0);
        QDate temp;
        temp.setDate(Ye,Mo,Da);
        QTime time;
        quint32 tempSe = Sec;
        quint32 tempMSe = Sec*1000;
        tempMSe = tempMSe-tempSe*1000;
        time.setHMS(Ho,Min,tempSe,tempMSe);
        QDateTime *tOfFirst = new QDateTime(temp,time,Qt::OffsetFromUTC);

        W = Reader->getIonW(NumIonLog);
        W = GPSwRecount(*tOfFirst,W);
        T = Reader->getIonT(NumIonLog);
        inputIONcorr = true;
    }
    else{
        qDebug()<< "There are no messages in the file containing the parameters of the ionosphere model";
    }
}

void EphemeridGPS::setIonGpsArgumentsRNX(rinexReader *Reader,int NumIonLog){
    //if(Reader->){
        alpha0 = Reader->ion_a0R;
        alpha1 = Reader->ion_a1R;
        alpha2 = Reader->ion_a2R;
        alpha3 = Reader->ion_a3R;
        beta0 = Reader->ion_b0R;
        beta1 = Reader->ion_b1R;
        beta2 = Reader->ion_b2R;
        beta3 = Reader->ion_b3R;
        W = Reader->Wr;
        T = Reader->Tr;
        inputIONcorr = true;
    }
   // else{
   //     qDebug()<< "There are no messages in the file containing the parameters of the ionosphere model";
   // }
//}

//запись в файл .G версии 302
void EphemeridGPS::

WriteRNXEphG(QFile *RINEX)
    {
    QString NameofSat = "G";
    QString Num = QString("%1" ).arg(NumSatGPS);
    if(NumSatGPS<10){
        NameofSat.append("0");
        NameofSat.append(Num);
    }
    if(NumSatGPS>=10){
        NameofSat.append(Num);
    }

    quint32 YearEph;
    quint32 MonthEph;
    quint32 DayEph;
    quint32 HourEph;
    quint32 MinEph;
    qreal SecEph;

    if(inputUTCtime==false){
        GPSWeek = GPSwRecount(*timeOfFirst,GPSWeek);
        DateUTC A = ConvertDatToUTC(GPSWeek, GPSSec);
        YearEph     = A.Date[0];
        MonthEph    = A.Date[1];
        DayEph      = A.Date[2];
        HourEph     = A.Date[3];
        MinEph      = A.Date[4];
        SecEph      = A.Seconds;
    }
    else{
        YearEph = YearEphInput;
        MonthEph = MonthEphInput;
        DayEph = DayEphInput;
        HourEph = HourEphInput;
        MinEph = MinEphInput;
        SecEph = SecEphInput;
    }

    //Создание потока для записи данных в файл
    QTextStream outRINEX(RINEX);
    //Запись данных. 1) Имя спутника
    outRINEX<<right<<fixed<<qSetFieldWidth(0)<<qSetRealNumberPrecision(0) << NameofSat;
    //Запись данных. 2) Временные параметры
    outRINEX<<qSetFieldWidth(5)<< qSetRealNumberPrecision(0) << YearEph;
    outRINEX<<qSetFieldWidth(3)<< ToStandartNum(MonthEph);
    outRINEX<<qSetFieldWidth(3)<< DayEph;
    outRINEX<<qSetFieldWidth(3)<< HourEph;
    outRINEX<<qSetFieldWidth(3)<< MinEph;
    outRINEX<<qSetFieldWidth(3)<< SecEph;
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockBias);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockDrift);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockBiasRate);
    outRINEX<<qSetFieldWidth(0) << endl;
    //Запись данных. 3) параметры эфемерид и альманаха GPS по эпохе
    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IODE);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Crs);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(DeltaN);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(M0);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cuc);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Eccentricity);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cus);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(sqrtA);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(ToE);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cic);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Omega0);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cis);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(i0);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Crc);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Omega);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(OmegaDot);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IDOT);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(CodesL2);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(GPSWeek);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(L2PdataFlag);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVaccuracy);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVhealth);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(TGD);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IODC);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "    ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(TransTime);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(FitInterval);
    outRINEX<<qSetFieldWidth(0) << endl;
}

//запись в файл .G версии 211
void EphemeridGPS::WriteRNXEphG211(QFile *RINEX)
    {
    QString NameofSat;
    QString Num = QString("%1" ).arg(NumSatGPS);
    if(NumSatGPS<10){
        NameofSat = "0";
        NameofSat.append(Num);
    }
    if(NumSatGPS>=10){
        NameofSat=Num;
    }

    quint32 YearEph;
    quint32 MonthEph;
    quint32 DayEph;
    quint32 HourEph;
    quint32 MinEph;
    qreal SecEph;

    if(inputUTCtime==false){
        GPSWeek = GPSwRecount(*timeOfFirst,GPSWeek);
        DateUTC A = ConvertDatToUTC(GPSWeek, GPSSec);
        YearEph     = A.Date[0];
        MonthEph    = A.Date[1];
        DayEph      = A.Date[2];
        HourEph     = A.Date[3];
        MinEph      = A.Date[4];
        SecEph      = A.Seconds;
    }
    else{
        YearEph = YearEphInput-2000;
        MonthEph = MonthEphInput;
        DayEph = DayEphInput;
        HourEph = HourEphInput;
        MinEph = MinEphInput;
        SecEph = SecEphInput;
    }

    QString SecEphStr;
    SecEphStr.setNum(SecEph, 'f', 1);

    //Создание потока для записи данных в файл
    QTextStream outRINEX(RINEX);
    //Запись данных. 1) Имя спутника
    outRINEX<<right<<fixed<<qSetFieldWidth(0)<<qSetRealNumberPrecision(0) << NameofSat;
    //Запись данных. 2) Временные параметры
    outRINEX<<qSetFieldWidth(3)<< qSetRealNumberPrecision(0) << YearEph;
    outRINEX<<qSetFieldWidth(3)<< ToStandartNum(MonthEph);
    outRINEX<<qSetFieldWidth(3)<< DayEph;
    outRINEX<<qSetFieldWidth(3)<< HourEph;
    outRINEX<<qSetFieldWidth(3)<< MinEph;
    outRINEX<<qSetFieldWidth(5)<< SecEphStr;
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockBias);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockDrift);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVClockBiasRate);
    outRINEX<<qSetFieldWidth(0) << endl;
    //Запись данных. 3) параметры эфемерид и альманаха GPS по эпохе
    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IODE);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Crs);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(DeltaN);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(M0);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cuc);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Eccentricity);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cus);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(sqrtA);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(ToE);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cic);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Omega0);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Cis);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(i0);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Crc);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(Omega);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(OmegaDot);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IDOT);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(CodesL2);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(GPSWeek);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(L2PdataFlag);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVaccuracy);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(SVhealth);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(TGD);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(IODC);
    outRINEX<<qSetFieldWidth(0) << endl;

    outRINEX<<qSetFieldWidth(0) << "   ";
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(TransTime);
    outRINEX<<qSetFieldWidth(19)<< dToStr12D(FitInterval);
    outRINEX<<qSetFieldWidth(0) << endl;
}


// функция создания и записи в файл header Rinex GPS NAV DATA версии 2.11
void EphemeridGPS::WriteRNX211HeaderG(QFile *RINEX)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonAlphaCom("ION ALPHA");
    QString IonBetaCom("ION BETA");
    QString DELTA_UTC_com("DELTA-UTC: A0,A1,T,W");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    quint32 LeapSeconds=0;
    // получить текущую дату, выраженную в секундах
    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy hh:mm");
    //создание файла и запись в него
    QTextStream outRINEX(RINEX);
    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     2.11           N: GPS NAV DATA    ";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20) <<progName;
    outRINEX << qSetFieldWidth(20)<<  AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    if(inputIONcorr){
    outRINEX << "  "<<dToStr4D(alpha0)<<dToStr4D(alpha1)<<dToStr4D(alpha2)<<qSetFieldWidth(22)<<dToStr4D(alpha3)<<qSetFieldWidth(20)<<IonAlphaCom<< qSetFieldWidth(0) << endl;
    outRINEX << "  "<<dToStr4D(beta0)<<dToStr4D(beta1)<<dToStr4D(beta2)<<qSetFieldWidth(22)<<dToStr4D(beta3)<<qSetFieldWidth(20)<<IonBetaCom<< qSetFieldWidth(0) << endl;
    outRINEX <<  "   "<<dToStr12D(A0)<<dToStr12D(A1)<< qSetFieldWidth(9)<<right<<T<< qSetFieldWidth(9)<<W<<left<< qSetFieldWidth(0)<<"  ";
    outRINEX<<left<<qSetFieldWidth(20)<<DELTA_UTC_com<< qSetFieldWidth(0) << endl;
    }
    //outRINEX<<"     "<<left<<qSetFieldWidth(55)<<LeapSeconds<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    outRINEX<< left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

// функция создания и записи в файл header Rinex GPS NAV DATA версии 3.02
void EphemeridGPS::WriteRNX302HeaderG(QFile *RINEX)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonAlphaCom("IONOSPHERIC CORR");
    QString IonBetaCom("IONOSPHERIC CORR");
    QString DELTA_UTC_com("TIME SYSTEM CORR");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    int LeapSeconds=0;
    // получить текущую дату, выраженную в секундах
    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("yyyyMMdd hhmmss");
    CurrentDate .append(" UTC");

    QTextStream outRINEX(RINEX);

    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(20)<< "     3.02           ";
    outRINEX << qSetFieldWidth(20)<< "N: GNSS NAV DATA   ";
    outRINEX << qSetFieldWidth(20)<< "G: GPS";
    outRINEX << qSetFieldWidth(20)<< TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20)<< progName;
    outRINEX << qSetFieldWidth(20)<< AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    if(inputIONcorr){
        outRINEX << "GPSA "<<dToStr4D(alpha0)<<dToStr4D(alpha1)<<dToStr4D(alpha2)<<qSetFieldWidth(19)<<dToStr4D(alpha3)<<qSetFieldWidth(20)<<IonAlphaCom<< qSetFieldWidth(0) << endl;
        outRINEX << "GPSB "<<dToStr4D(beta0)<<dToStr4D(beta1)<<dToStr4D(beta2)<<qSetFieldWidth(19)<<dToStr4D(beta3)<<qSetFieldWidth(20)<<IonBetaCom<< qSetFieldWidth(0) << endl;
        outRINEX << "GPUT  "<< qSetFieldWidth(17)<<dToStr10E(A0)<< qSetFieldWidth(17)<<dToStr10E(A1)<< qSetFieldWidth(7)<<right<<T<< qSetFieldWidth(5)<<W<<left<< qSetFieldWidth(6)<<" ";
        outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<DELTA_UTC_com<< qSetFieldWidth(0) << endl;
        outRINEX << "     "<<left<<qSetFieldWidth(55)<<LeapSeconds<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    }
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

//функции для записи хедера по итогам считывания RINEX-файлов
// функция создания и записи в файл header Rinex GPS NAV DATA версии 2.11
void EphemeridGPS::WriteRNX2HeaderG(QFile *RINEX, rinexReader *Reader)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonAlphaCom("ION ALPHA");
    QString IonBetaCom("ION BETA");
    QString DELTA_UTC_com("DELTA-UTC: A0,A1,T,W");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
    QString progName;
    QString AggencyName;
    progName = converterJps::programVersion;
    AggencyName = converterJps::Inf.at(1);
    // получить текущую дату, выраженную в секундах
    QDateTime CurDate = QDateTime::currentDateTime();
    QString CurrentDate = CurDate.toString("dd-MM-yy hh:mm");
    //создание файла и запись в него
    QTextStream outRINEX(RINEX);
    outRINEX.setFieldAlignment(QTextStream::AlignLeft);
    outRINEX << qSetFieldWidth(60)<< "     2.11           N: GPS NAV DATA    ";
    outRINEX << qSetFieldWidth(20)<<TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20) <<progName;
    outRINEX << qSetFieldWidth(20)<<  AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    if(Reader->ionAOn){
    outRINEX << "  "<<Reader->ion_a0<<Reader->ion_a1<<Reader->ion_a2<<qSetFieldWidth(22)<<Reader->ion_a3<<qSetFieldWidth(20)<<IonAlphaCom<< qSetFieldWidth(0) << endl;
    }
    if(Reader->ionBOn){
        outRINEX << "  "<<Reader->ion_b0<<Reader->ion_b1<<Reader->ion_b2<<qSetFieldWidth(22)<<Reader->ion_b3<<qSetFieldWidth(20)<<IonBetaCom<< qSetFieldWidth(0) << endl;
    }
    if(Reader->TgpsOn){
        outRINEX <<  "   "<<Reader->a0<<Reader->a1<< qSetFieldWidth(9)<<right<<Reader->T<< qSetFieldWidth(9)<<Reader->W<<left<< qSetFieldWidth(0)<<" ";
        outRINEX<<left<<qSetFieldWidth(20)<<DELTA_UTC_com<< qSetFieldWidth(0) << endl;
    }
    if(Reader->leapSecOn){
    outRINEX<<"     "<<left<<qSetFieldWidth(55)<<Reader->LeapSec<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    }
    outRINEX<< left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}

// функция создания и записи в файл header Rinex GPS NAV DATA версии 3.02
void EphemeridGPS::WriteRNX3HeaderG(QFile *RINEX, rinexReader *Reader)
{
    QString TypeRNX("RINEX VERSION / TYPE");
    QString PGMDATE("PGM / RUN BY / DATE");
    QString Comment("COMMENT");
    QString IonAlphaCom("IONOSPHERIC CORR");
    QString IonBetaCom("IONOSPHERIC CORR");
    QString DELTA_UTC_com("TIME SYSTEM CORR");
    QString LeapSec("LEAP SECONDS");
    QString NumbSats("# OF SATELLITES");
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
    outRINEX << qSetFieldWidth(20)<< "G: GPS";
    outRINEX << qSetFieldWidth(20)<< TypeRNX << qSetFieldWidth(0) << endl;
    outRINEX << qSetFieldWidth(20)<< progName;
    outRINEX << qSetFieldWidth(20)<< AggencyName;
    outRINEX << qSetFieldWidth(20)<< CurrentDate;
    outRINEX << qSetFieldWidth(20)<<PGMDATE << qSetFieldWidth(0) << endl;
    if(Reader->ionAOn){
        outRINEX << "GPSA "<<Reader->ion_a0<<Reader->ion_a1<<Reader->ion_a2<<qSetFieldWidth(19)<<Reader->ion_a3<<qSetFieldWidth(20)<<IonAlphaCom<< qSetFieldWidth(0) << endl;
    }
    if(Reader->ionBOn){
    outRINEX << "GPSB "<<Reader->ion_b0<<Reader->ion_b1<<Reader->ion_b2<<Reader->ion_b2<<qSetFieldWidth(20)<<IonBetaCom<< qSetFieldWidth(0) << endl;
        }
    if(Reader->TgpsOn){
        outRINEX << "GPUT  "<< qSetFieldWidth(17)<<Reader->a0<< qSetFieldWidth(17)<<Reader->a1<< qSetFieldWidth(7)<<right<<Reader->T<< qSetFieldWidth(5)<<Reader->W<<left<< qSetFieldWidth(6)<<" ";
    outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<DELTA_UTC_com<< qSetFieldWidth(0) << endl;
     }
    if(Reader->TgloOn){
        outRINEX << "GLUT  "<< qSetFieldWidth(17)<<Reader->a0_TauC<< qSetFieldWidth(17)<<dToStr4D(0)<< qSetFieldWidth(7)<<right<<0<< qSetFieldWidth(5)<<0<<left<< qSetFieldWidth(6)<<" ";
    outRINEX << left<<qSetFieldWidth(0)<<"  "<<qSetFieldWidth(20)<<DELTA_UTC_com<< qSetFieldWidth(0) << endl;
     }
    if(Reader->leapSecOn){
    outRINEX << "     "<<left<<qSetFieldWidth(55)<<Reader->LeapSec<<qSetFieldWidth(20)<<LeapSec<< qSetFieldWidth(0) << endl;
    }
    outRINEX << left<< qSetPadChar(' ') << qSetFieldWidth(60)<<" "<< qSetFieldWidth(20)<<"END OF HEADER" << qSetFieldWidth(0) << endl;
}


