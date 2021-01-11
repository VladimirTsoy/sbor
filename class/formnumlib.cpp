#include <QString>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <QByteArray>
#include <QBitArray>
#include "formnumlib.h"
#include <QDateTime>

QString formNum::dToStr4D(qreal d)      //функция по переводу числа qreal в вид .0000D00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 4);
     QString Point = ".";
     QString E = "E";
     QString D = "D";
     dStr.remove(0, dStr.lastIndexOf(Point));
     dStr.replace(dStr.lastIndexOf(E), 1,D);
     if(d>=0)
     {dStr.prepend("   ");}
     else
     {dStr.prepend("  -");}
     return dStr;
}
QString formNum::dToStr10D(qreal d)     //функция по переводу числа qreal в вид .0000000000D00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 10);
     QString Point = ".";
     QString E = "E";
     QString D = "D";
     dStr.remove(0, dStr.lastIndexOf(Point));
     dStr.replace(dStr.lastIndexOf(E), 1,D);
     if(d>=0)
     {dStr.prepend("  ");}
     else
     {dStr.prepend(" -");}
     return dStr;
}
QString formNum::dToStr12D(qreal d)     //функция по переводу числа qreal в вид .000000000000D00
{
qint32 N;
bool ok;
QString Nstr("");
QString Point = ".";
QString E = "E";
QString D = "D";
QString dStr2;
QString n2;
if(d>=0)
{
if((d<1.0e-99) || (d>1.0e+99) )
{d=0;}
}
else
{
if((d>-1.0e-99) || (d<-1.0e+99) )
{d=0;}
}
QString dStr = QString("%1").arg(d, 0, 'E', 11);
dStr.replace(dStr.lastIndexOf(E), 1,E);
QStringList FirstList = dStr.split(QRegExp("\\b"));
dStr2.append(FirstList[0]);
dStr2.append(FirstList[2]);
dStr2.append(FirstList[1]);
dStr2.append(FirstList[3]);
Nstr.append(FirstList[5]);
N=0;
N= Nstr.toInt(&ok,10);

if(d!=0)
{
if(N>=0)
{
N=N+1;
}
else
{
N=N-1;
}
}
if(FirstList[4]=="-")
{N=N-2;}
n2=QString("%1").arg(N);
if(N>0)
{
if(N>=10)
{
dStr2.append(FirstList[4]);
dStr2.append(n2);
}
if((N>0)&&(N<10))
{
dStr2.append(FirstList[4]);
dStr2.append("0");
dStr2.append(n2);
}
}
if(N<0)
{
{
if(N<=-10)
{
dStr2.append("-");
n2.remove(0,1);
dStr2.append(n2);}
if((N>-10)&&(N<0))
{
dStr2.append("-");
n2.remove(0,1);
dStr2.append("0");
dStr2.append(n2);
}
}
}
if(N==0)
{
dStr2.append("+0");
dStr2.append(n2);
}
dStr2.prepend(" ");
return dStr2;
}
QString formNum::dToStr4E(qreal d)      //функция по переводу числа qreal в вид 0.0000E00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 4);
     return dStr;
}
QString formNum::dToStr12E(qreal d)     //функция по переводу числа qreal в вид 0.000000000000E00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 12);
     return dStr;
}
QString formNum::dToStr10E(qreal d)     //функция по переводу числа qreal в вид 0.0000000000E00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 10);
     return dStr;
}
QString formNum::dToStr12DObs(qreal d)  //функция по переводу числа qreal в вид 0.000000000000D00
{
     QString dStr = QString("%1").arg(d, 0, 'E', 12);
     QString E = "E";
     QString D = "D";
     dStr.replace(dStr.lastIndexOf(E), 1,D);
     return dStr;
}
qreal formNum::StrToD(QString Str)      //функция по переводу строки в число qreal
{
    bool ok;
    //Извлекаем второй символ из последовательности (либо 0, либо пробел/-)
    QString TestNull = Str.mid(1,1);
    bool NullIn = TestNull.contains("0");
    //Извлекаем символы соержащие степень
    QString strStepen = Str.right(3);
    //Получаем степень в формате "qreal"
    qreal Stepen = strStepen.toDouble();
    //Условие проверяет наличие нуля в записи
    if(NullIn==false){
        //Если нуля нет, то добавляем его к строке
        //Str.insert(2, QString("0"));
        QString temp = Str.at(1);
        Str.replace(0,1,temp);
        Str.replace(1,1,QString("0"));
    }
    //Обрезаем строку, оставляя только основное число
    Str = Str.remove(15,4);
    //Получаем число в формате "qreal"
    qreal d = Str.toDouble() * qPow(10.0, Stepen);
    return d;
}
QString formNum::NoiseToSigma(quint32 i)
{
    QString dStr;
    if(i<12)
    {dStr = "1";}
    if(i<1)
    {dStr = " ";}
    if(i>=12&&i<18)
    {dStr = "2";}
    if(i>=18&&i<24)
    {dStr = "3";}
    if(i>=24&&i<30)
    {dStr = "4";}
    if(i>=30&&i<36)
    {dStr = "5";}
    if(i>=36&&i<42)
    {dStr = "6";}
    if(i>=42&&i<48)
    {dStr = "7";}
    if(i>=48&&i<54)
    {dStr = "8";}
    if(i>=54)
    {dStr = "9";}
    return dStr;
}
qreal formNum::SigmaToNoise(quint32 i)
{
    qreal result=0;
    if(i==1){result=11;}
    if(i==2){result=17;}
    if(i==3){result=23;}
    if(i==4){result=29;}
    if(i==5){result=35;}
    if(i==6){result=41;}
    if(i==7){result=47;}
    if(i==8){result=53;}
    if(i==9){result=55;}
    return result;
}
QString formNum::ToStandartNum(quint32 a)
{

    quint32 Int = a;
    QString dStr;
    if(Int<10)
    {
    dStr= QString("0%1").arg(Int);
    }
    if(Int>=10)
    {dStr= QString("%1").arg(Int);}
    return dStr;
}
qint32 formNum::uraToSVaccuracy(qint32 i)
{
qint32 dStr;
if(i==0)
{dStr = 2.4;}
if(i==1)
{dStr = 3.4;}
if(i==2)
{dStr = 4.85;}
if(i==3)
{dStr = 6.85;}
if(i==4)
{dStr = 9.65;}
if(i==5)
{dStr = 13.65;}
if(i==6)
{dStr = 24.00;}
if(i==7)
{dStr = 48.00;}
if(i==8)
{dStr = 96.00;}
if(i==9)
{dStr = 192.00;}
if(i==10)
{dStr = 384.00;}
if(i==11)
{dStr = 768.00;}
if(i==12)
{dStr = 1536.00;}
if(i==13)
{dStr = 3072.00;}
if(i==14)
{dStr = 6144.00;}
if(i>=15)
{dStr = 0.00;} //нет точности (см.ИКД GPS)
return dStr;
}
QBitArray formNum::toBitArray(QByteArray fromArray){
QBitArray outArray;
outArray.resize(fromArray.size()*8);//! resize array
for(int i=0;i<outArray.size();i++){
    if(fromArray[i/8]>>i & 1){outArray.setBit(i,true);}
}//for i
QString str="";
for(int i=0;i<outArray.size();i++){
   // if(outArray.testBit(i)){qDebug()<<1;}else{qDebug()<<0;}
    if(outArray.testBit(i)){str=str.insert(outArray.size()-i,"1");}else{str=str.insert(outArray.size()-i,"0");}
}//for
return outArray;
}

formNum::DateUTC formNum::ConvertDatToUTC( qreal DatGPSw, qreal DatGPSsec )
{
//    //ОБЯВЛЕНИЕ КОНСТАНТ
    DateUTC UTCDate;

//    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ НЕ ВИСОКОСНОГО ГОДА
//    const quint32 Months [] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

//    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ ВИСОКОСНОГО ГОДА
//    const quint32 MonthsLeapYear [] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

//    //ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
//    const quint32 InitialYear = 1980;

//    //360 - ЧИСЛО ДНЕЙ ДЛЯ 1980, 365 - НЕ ВИСОКОСНЫЙ ГОД, 366 - ВИСОКОСНЫЙ ГОД
//    const quint32 DaysYear [] = { 360, 365, 366 };

//        //ОБЯВЛЕНИЕ ПЕРЕМЕННЫХ
//    //МАССИВ ДЛЯ ВЫВОДА РЕЗУЛЬТАТОВ ВЫЧИСЛЕНИЙ
//    const quint32 HoursPerDayOfWeek [] = { 0, 86400, 172800, 259200, 345600, 432000, 518400, 604800 };
//    /*static double RESULT [3];*/
//    //СЧЁТЧИК ЛЕТ

//    //ЧИСЛО НЕДЕЛЬ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
//    quint32 WeekEPH1;

//    //ЧИСЛО ДНЕЙ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
//    quint32 DaysEPH1 = 0;
//    quint16 Count = 0;
//    quint32 MonthUTC = 0;
//    quint32 UsualYears = 0;
//    quint32 YearEPH = 0;
//    quint32 LeapYearsWithoutInit = 0;
//    quint32 DaysThisYear = 0;
//    quint32 DayCounter = 0;
//    quint32 DayWeekCounter = 0;
//    quint32 DayOfMonth = 0;
//    quint32 HMS_EPH = 0;
//    quint32 HourEPH = 0;
//    quint32 MinuteEPH = 0;
//    qreal SecondsUTC = 0.0;

    QDateTime DT( QDate(1980,1,6), QTime(0,0,0,0) );        // Начало отсчета TGPS
    QDateTime CurDT = DT.addDays( DatGPSw * 7 );
    CurDT = CurDT.addSecs( DatGPSsec );
    CurDT = CurDT.toUTC();

//    // Тест на сбой счёта недель
//    if( DatGPSsec == 0 )
//        ++DatGPSw;

//    WeekEPH1 = DatGPSw;
//    quint32 TestDays = qFloor( DatGPSsec / 3600 / 24 ) + 1;

//    DaysEPH1 = WeekEPH1 * 7;
//    YearEPH = WeekEPH1 / 52 + InitialYear;

//    LeapYearsWithoutInit = (WeekEPH1/52-((WeekEPH1/52)%4))/4;
//    UsualYears=((WeekEPH1/52-LeapYearsWithoutInit)-1);

//    DaysThisYear = DaysEPH1 - LeapYearsWithoutInit * DaysYear[2] - UsualYears * DaysYear[1] - DaysYear[0];

//    if( YearEPH == 1980 )
//        DaysThisYear = 6 + DaysEPH1;

//    for (quint32 i = 0; i<8; i++)
//    {
//        if(( HoursPerDayOfWeek[i] <= DatGPSsec) && ( HoursPerDayOfWeek[i+1] > DatGPSsec ) )
//            DayWeekCounter = i + 1;
//    }

//    //ДЛЯ ВИСОКОСНОГО ГОДА
//    if ( YearEPH % 4 == 0 )
//    {
//        for (quint32 i = 0; i<12; i++)
//        {
//            if( ( MonthsLeapYear[0] <= DaysThisYear ) && ( MonthsLeapYear[1] > DaysThisYear ) )
//                MonthUTC = 1;
//            if( ( MonthsLeapYear[i] < DaysThisYear ) && ( MonthsLeapYear[i+1] >= DaysThisYear ) )
//                MonthUTC = i + 1;
//            if( ( MonthsLeapYear[11] <= DaysThisYear ) && ( DaysThisYear > 367 ) )
//                MonthUTC = 12;
//        }
//    }

//    //ДЛЯ НЕ ВИСОКОСНОГО ГОДА
//    else {
//        for (quint32 i = 0; i<12; i++)
//        {
//            if( ( Months[0] <= DaysThisYear ) && ( Months[1] > DaysThisYear ) )
//                MonthUTC = 1;
//            if( ( Months[i] < DaysThisYear ) && ( Months[i+1] >= DaysThisYear ) )
//                MonthUTC = i + 1;
//            if( ( Months[11]  <= DaysThisYear ) && ( DaysThisYear > 366 ) )
//                MonthUTC = 12;
//        }
//    }

//    for ( quint32 i = 0; i < MonthUTC; i++ )
//    {
//        if (YearEPH % 4 == 0)
//        {
//            DayCounter  = MonthsLeapYear[i];
//            Count++;
//        }
//        else
//        {
//            DayCounter  = Months[i];
//            Count++;
//        }
//    }
//    DaysThisYear = DaysThisYear + DayWeekCounter - 1;

//    DayOfMonth = DaysThisYear - DayCounter;

//    if( YearEPH % 4 != 0 && DayOfMonth == 0 )
//        DayOfMonth = Months[Count-1] - Months[Count-2];

//    if( YearEPH % 4 == 0 && DayOfMonth == 0 )
//        DayOfMonth = MonthsLeapYear[Count-1] - MonthsLeapYear[Count-2];

//    if( YearEPH % 4 != 0 && DayOfMonth > ( Months[Count] - Months[Count-1] ) )
//    {
//        DayOfMonth = DayOfMonth - (Months[Count] - Months[Count-1]);
//        ++MonthUTC;

//        if( DayOfMonth == 0 )
//            ++DayOfMonth;
//    }

//    if( YearEPH % 4 == 0 && DayOfMonth > ( MonthsLeapYear[Count] - MonthsLeapYear[Count-1] ) )
//    {
//        DayOfMonth = DayOfMonth - (MonthsLeapYear[Count]-MonthsLeapYear[Count-1]);
//        ++MonthUTC;

//        if( DayOfMonth == 0 )
//            ++DayOfMonth;
//    }

//    if( TestDays > DayWeekCounter )
//    {
//        DayWeekCounter = TestDays;
//    }

//    HMS_EPH = DatGPSsec - ( DayWeekCounter - 1 ) * 24 * 3600;
//    //
//    HourEPH = qFloor( HMS_EPH / 3600 );
//    MinuteEPH = qFloor( ( HMS_EPH - HourEPH * 3600 ) / 60 );

//    //ВЫЧИСЛЯЕМ СЕКУНДЫ
//    SecondsUTC = HMS_EPH - HourEPH * 3600 - MinuteEPH * 60;
    UTCDate.Date[0] = CurDT.date().year();  //YearEPH;
    UTCDate.Date[1] = CurDT.date().month();  //MonthUTC;
    UTCDate.Date[2] = CurDT.date().day();  //DayOfMonth;
    UTCDate.Date[3] = CurDT.time().hour();  //HourEPH;
    UTCDate.Date[4] = CurDT.time().minute();  //MinuteEPH;
    UTCDate.Seconds = CurDT.time().second();  //SecondsUTC;

    return UTCDate;
}

formNum::DateUTC formNum::ConvertGLODatToUTC(quint32 DayGLO,quint32 SecGLO, quint32 ReceiverY)
{
    DateUTC UTCDate;

//    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ НЕ ВИСОКОСНОГО ГОДА
//    const quint32 Months [] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
//    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ ВИСОКОСНОГО ГОДА
//    const quint32 MonthsLeapYear [] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
//    //ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
//    const quint32 InitialYear = 1996;
//    //360 - ЧИСЛО ДНЕЙ ДЛЯ 1980, 365 - НЕ ВИСОКОСНЫЙ ГОД, 366 - ВИСОКОСНЫЙ ГОД
//    const quint32 DaysYear [] = {360, 365, 366};
//    //ОБЯВЛЕНИЕ ПЕРЕМЕННЫХ
//    //МАССИВ ДЛЯ ВЫВОДА РЕЗУЛЬТАТОВ ВЫЧИСЛЕНИЙ
//    const quint32 HoursPerDayOfWeek [] = {0, 86400, 172800,259200,345600,432000,518400,604800};
//    /*static double RESULT [3];*/
//    //СЧЁТЧИК ЛЕТ
//    quint32 LeapYflag;


//    quint32 N4 = qAbs( ReceiverY - DT.date().year() ) / 4 + 1;  // Номер 4х-летки от опорной даты до текущей
    QDateTime tmpDT( QDate(ReceiverY,1,1), QTime(0,0,0,0) );

    while ( !tmpDT.date().isLeapYear( tmpDT.date().year() ) )    // Ищем ближайший високосный год ( в прошлом )
    {
        tmpDT = tmpDT.addYears( -1 );
    }

    tmpDT = tmpDT.addDays( DayGLO-1 );
    tmpDT = tmpDT.addSecs( SecGLO );
    tmpDT = tmpDT.toUTC();

//    qint32 J=0,Y=0,NY=0,M=0,D=0;
//    quint16 Count = 0;
//    quint32 DaysInY=0,DayCounter=0;

//    quint32 TestDays = qFloor(SecGLO/3600/24)+1;

//    if(1<=DayGLO&&DayGLO<=366)
//    {J = 1;}
//    if(367<=DayGLO&&DayGLO<=731)
//    {J = 2; }
//    if(732<=DayGLO&&DayGLO<=1096)
//    {J = 3;}
//    if(1097<=DayGLO&&DayGLO<=1461)
//    {J = 4;}

//    NY = qCeil( (ReceiverY-1996)/4 ) + 1;

//    Y = InitialYear + 4*(NY-1) + (J-1);

//    DaysInY = DayGLO - ((J-1)*365+1);


//    //ДЛЯ ВИСОКОСНОГО ГОДА
//    if (Y % 4 == 0)
//    {
//        LeapYflag = 1;
//        for (quint32 i = 0; i<12; i++)
//        {
//            if((MonthsLeapYear[i]<=DaysInY) && (MonthsLeapYear[i+1]>DaysInY))
//                M = i+1;
//            if((MonthsLeapYear[11]<=DaysInY)&&(DaysInY>367))
//                M = 12;
//            if(MonthsLeapYear[i]==DaysInY)
//                M = M-1;
//        }
//    }
//    //ДЛЯ НЕ ВИСОКОСНОГО ГОДА
//    else {
//        for (quint32 i = 0; i<12; i++)
//        {
//            if((Months[i]<=DaysInY) && (Months[i+1]>DaysInY))
//                M = i+1;
//            if((Months[11])<=DaysInY&&(DaysInY>366))
//                M = 12;
//            if(Months[i]==DaysInY)
//                M = M-1;
//        }
//    }

//    qDebug() << M;

//    if(M<=0)
//    {
//        M=M+12;
//        //    Y=Y-1;
//    }

//    for (quint32 i = 0; i<(M); i++)
//    {
//        if (Y % 4 == 0)
//        {
//            DayCounter = MonthsLeapYear[i];
//            Count++;
//        }
//        else
//        {
//            DayCounter = Months[i];
//            Count++;
//        }
//    }

//    D = DaysInY-DayCounter;

//    if (D==0)
//        D = DaysInY-Months[Count-1];

//    if(Y % 4 != 0 && D>(Months[Count]-Months[Count-1]) )
//    {
//        D = D - (Months[Count]-Months[Count-1]);
//        M =M+1;

//    }

//    if(Y % 4 == 0 && D>(MonthsLeapYear[Count]-MonthsLeapYear[Count-1]) )
//    {

//        D = D - (MonthsLeapYear[Count]-MonthsLeapYear[Count-1]);
//        M =M+1;

//    }

//    qint32 HMS=SecGLO-3*3600;
//    if(HMS<0)
//    {

//        HMS=HMS+86400;      //
//        D=D-1;              //

//        if(D<=0)
//        {
//            if (Y % 4 == 0)
//            {
//                D=D+(MonthsLeapYear [M-1]-MonthsLeapYear [M-2]);
//                M=M-1;
//                if(M<=0)
//                {
//                    M=M+12;
//                    Y=Y-1;
//                }
//            }
//            else
//            {
//                D=D+(Months[M-1]-Months[M-2]);
//                M=M-1;
//                if(M<=0)
//                {
//                    M=M+12;
//                    Y=Y-1;
//                }
//            }
//        }
//    }

//    quint32 Hour = qFloor(HMS/3600);\

//    quint32 Minute = qFloor((HMS-Hour*3600)/60);

//    //ВЫЧИСЛЯЕМ СЕКУНДЫ
//    qreal Seconds = HMS-Hour*3600-Minute*60;

    UTCDate.Date[0] = tmpDT.date().year();        // Y
    UTCDate.Date[1] = tmpDT.date().month();       // M
    UTCDate.Date[2] = tmpDT.date().day();         // D
    UTCDate.Date[3] = tmpDT.time().hour();        // Hour;
    UTCDate.Date[4] = tmpDT.time().minute();      // Minute;
    UTCDate.Seconds = tmpDT.time().second();      // Seconds;
//    UTCDate.Date[0] = Y;
//    UTCDate.Date[1] = M;
//    UTCDate.Date[2] = D;
//    UTCDate.Date[3] = Hour;
//    UTCDate.Date[4] = Minute;
//    UTCDate.Seconds = Seconds;

    return UTCDate;
}

quint32 formNum::FromUTCDatToUTCDayofWeek(quint32 Y, quint32 M,quint32 D)
{

    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ НЕ ВИСОКОСНОГО ГОДА
    const quint32 Months [] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ ВИСОКОСНОГО ГОДА
    const quint32 MonthsLeapYear [] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
    //ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
    const quint32 InitialYear = 2000;
    //360 - ЧИСЛО ДНЕЙ ДЛЯ 2000, 365 - НЕ ВИСОКОСНЫЙ ГОД, 366 - ВИСОКОСНЫЙ ГОД
    const quint32 DaysYear [] = {365, 366};
    //ОБЯВЛЕНИЕ ПЕРЕМЕННЫХ
    //МАССИВ ДЛЯ ВЫВОДА РЕЗУЛЬТАТОВ ВЫЧИСЛЕНИЙ
    const quint32 HoursPerDayOfWeek [] = {0, 86400, 172800,259200,345600,432000,518400,604800};
    //СЧЁТЧИК ЛЕТ
    quint32 LeapYflag;
    qint32 Yr,Mr,Dr,DrW,W,DrW1;
    qint32 Mf;

    Mf=M;
    Yr=Y-InitialYear ;
    Mr = 0;
    Dr=0;
    DrW=0;
    for(qint32 u=0; u<=Yr;u++)
    {
        if(u % 4 == 0)
        {
            Dr=Dr+366;
        }
        else
        {
            Dr=Dr+365;
        }
    }
    for(qint32 u=0; u<=Mf;u++)
    {
        if(Yr % 4 == 0)
        {
            Mr=Months [u-1];
        }
        else
        {
            Mr=MonthsLeapYear [u-1];
        }
    }
    DrW = Dr+Mr+D;
    W = (DrW-DrW%7)/7;
    DrW1 = DrW -W*7;
    DrW1 = DrW1 +4;
    return DrW1;
}
QString formNum::MakeNameFile(QString A,QString B)
{
    QStringList list2 = A.split('.', QString::SkipEmptyParts);
    QString C = list2 [0];
    C.append(".");
    C.append(B);
    return C;
}
QString formNum::MakeNameFileConv(QString A,QString B)
{
    QStringList list2 = A.split('.', QString::SkipEmptyParts);
    QString C = list2 [0];
    C.append("CONV.");
    C.append(B);
    return C;
}
QString formNum::CutYear(quint16 A)
{
    QString str;
    str = QString("%1").arg(A);
    QString str2 = str.right(2);
    return str2;
}
qreal formNum::GPSwRecount(QDateTime A,qreal B)
{
    quint64 DaysBetweenRollOver = 7167;
    QDateTime GPSStartTime(QDate(1980,1,6),QTime(0,0,0));
    quint16 IntRollOver;
    qreal C ;
    quint64 DaysAfterStartGPS = GPSStartTime.daysTo(A);
    for(quint16 i=0;i<=65534;i++)
    {
        //    qDebug() << " A = " << (i+1)*DaysBetweenRollOver+i*1 << " > " " B = " << DaysAfterStartGPS;
        if ((i+1)*DaysBetweenRollOver+i*1 > DaysAfterStartGPS )
        {
            IntRollOver = i;
            break;
        }
    }
    C = B + IntRollOver*1024;
    return C;
}
QString formNum::AddSpace (QString A, quint8 B)
{
    QString BuffSpace;
    for (quint16 i=1;i<=B;i++)
        BuffSpace.append(" ");
    A.append(BuffSpace);
    return A;
}
qint8 formNum::litCalc (qreal C, qreal L)
{
    qreal CfromL=0;
    qreal deltaC=0;
    qreal dC = 1.7976931348623157E308;
    qint8 lit;
    for(qint8 i=-7;i<8;i++)
    {
        CfromL =(L*299792458)/(1602000000.0+562500.0*i);
        //qDebug() << "CfromL = " << CfromL;
        deltaC = C - CfromL;
        //qDebug() << "deltaC  = " << deltaC ;
        if (qFabs(deltaC)<qFabs(dC))
        {
            dC = deltaC;
            //qDebug() << "dC  = " << dC ;
            lit = i;
            //qDebug() << "lit  = " << lit ;
        }
    }
    return lit;
}
int formNum::getNoiseOfStr(QString SatMeasString, int it) //Метод для получения признака шума измерений из строки
{
    bool ok;
    QString Measure = SatMeasString.mid(3 + (it-1)*16 + 15,1);
    int result = SigmaToNoise(Measure.toInt(&ok,10));
    return result;
}
