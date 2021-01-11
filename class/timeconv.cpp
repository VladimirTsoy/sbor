#include "timeconv.h"
#include <iostream>
#include <vector>
#include <cmath>

struct DateUTC
{
       int Date[5]; // Месяц постройки дома
       double Seconds; // Год
}NewUTCDate;

// ======== МЕТОД ПРЕОБРАЗОВАНИЯ ФОРМАТА ЭПОХИ ========
// ВХОДНОЙ ФОРМАТ: ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА
// ВЫХОДНОЙ ФОРМАТ: НЕДЕЛЯ СЕКУНДА(ОТ НАЧАЛА ТЕКУЩЕЙ НЕДЕЛИ)
// ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
//Dat[0] - ГОД
//Dat[1] - МЕСЯЦ
//Dat[2] - ДЕНЬ
//Dat[3] - ЧАС
//Dat[4] - МИНУТА
//Second - СЕКУНДА

/*

std::vector<double> ConvertDatToWeekSec(double Sec, int Dat[5] )
{
//ОБЯВЛЕНИЕ КОНСТАНТ
    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ НЕ ВИСОКОСНОГО ГОДА
const int Months [] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
//ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ ВИСОКОСНОГО ГОДА
const int MonthsLeapYear [] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
//ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
const int InitialYear = 1980;
 //360 - ЧИСЛО ДНЕЙ ДЛЯ 1980, 365 - НЕ ВИСОКОСНЫЙ ГОД, 366 - ВИСОКОСНЫЙ ГОД
const int DaysYear [] = {360, 365, 366};
//ОБЯВЛЕНИЕ ПЕРЕМЕННЫХ
//СЧЁТЧИК ЛЕТ
int Scorer;
 //ЧИСЛО НЕДЕЛЬ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
int WeekEPH;
//ЧИСЛО ДНЕЙ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
int DaysEPH = 0;
double SecondEPH = 0;
//int* Dat = new int[5];
//ЧИСЛО СЕКУНД ПРШЕДШИХ С НАЧАЛА НОВОЙ НЕДЕЛИ
//ВЫЧИСЛЕНИЕ ЧИСЛА ДНЕЙ
Scorer = Dat[0]-InitialYear;
//ВЫЧИСЛЯЕМ ЧИСЛО ПРОШЕДШИХ ЛЕТ
for (int i = 0; i<Scorer; i++)
{
    //ВЫЧИСЛЯЕМ ЧИСЛО ДНЕЙ В ПРОШЕДШИХ ГОДАХ
    if (i==0){
         //СЧИТАЕМ ЧТО В 1980 ГОДУ 361 ДЕНЬ
        DaysEPH += DaysYear [0];
    }
    else {
        //ПРОВЕРЯЕМ ВИСОКОСНОСТЬ ГОДА
        if ((InitialYear + i) % 4 == 0) {
            DaysEPH += DaysYear[2];
        } else {
            DaysEPH += DaysYear[1];
        }
    }
}
if (Dat[0] == 1980) {
    DaysEPH = (DaysEPH + MonthsLeapYear[Dat[1] - 1] + Dat[2])-6;
    WeekEPH = DaysEPH / 7;
    SecondEPH = (double)Dat[3] * 3600 + (double)Dat[4] * 60 + (double)((DaysEPH % 7) * 86400) + Sec;
    //ВЫВОД РЕЗУЛЬТАТА
    VectorResult[0] = (double)WeekEPH;
    VectorResult[1] = SecondEPH;
}
    else
    {
    if (Dat[0] % 4 == 0)
    {
     //ВЫБИРАЕМ ИЗ СПИСКА ЧИСЛО ДНЕЙ В ПРОШЕДШИХ МЕСЯЦАХ
        DaysEPH = DaysEPH + MonthsLeapYear [Dat[1]-1] + Dat[2];
     //ДЛЯ ВИСОКОСНОГО ГОДА
    }
    else {
    //ДЛЯ НЕ ВИСОКОСНОГО ГОДА
        DaysEPH = DaysEPH + Months [Dat[1]-1] + Dat[2];
    }
//ВЫЧИСЛЕНИЕ ЧИСЛА НЕДЕЛЬ И СЕКУНД
//ВЫЧИСЛЯЕМ НОМЕР НЕДЕЛИ
    WeekEPH = DaysEPH/7;

//ВЫЧИСЛЯЕМ СЕКУНДЫ
    SecondEPH = (double)(Dat[3]*3600.0 +Dat[4]*60.0 + (DaysEPH%7)*86400)+ Sec ;
//ВЫВОД РЕЗУЛЬТАТА
    VectorResult[0] = (double)WeekEPH;
    VectorResult[1] = SecondEPH;
    }
return VectorResult;
}
*/

// ======== МЕТОД ПРЕОБРАЗОВАНИЯ ФОРМАТА ЭПОХИ ========
// ВХОДНОЙ ФОРМАТ: НЕДЕЛЯ СЕКУНДА(ОТ НАЧАЛА ТЕКУЩЕЙ НЕДЕЛИ)
// ВЫХОДНОЙ ФОРМАТ: ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА
// ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
DateUTC ConvertDatToUTC(double DatGPS[2])
{
//ОБЯВЛЕНИЕ КОНСТАНТ
    //ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ НЕ ВИСОКОСНОГО ГОДА
const int Months [] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
//ЧИСЛО ДНЕЙ СООТВЕТСТВУЮЩЕЕ НАЧАЛУ МЕСЯЦА ДЛЯ ВИСОКОСНОГО ГОДА
const int MonthsLeapYear [] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
//ОПОРНАЯ ЭПОХА: 1980 01 06 00 00 00.00
const int InitialYear = 1980;
 //360 - ЧИСЛО ДНЕЙ ДЛЯ 1980, 365 - НЕ ВИСОКОСНЫЙ ГОД, 366 - ВИСОКОСНЫЙ ГОД
const int DaysYear [] = {360, 365, 366};
//ОБЯВЛЕНИЕ ПЕРЕМЕННЫХ
//МАССИВ ДЛЯ ВЫВОДА РЕЗУЛЬТАТОВ ВЫЧИСЛЕНИЙ
const int HoursPerDayOfWeek [] = {0, 86400, 172800,259200,345600,432000,518400,604800};
/*static double RESULT [3];*/
//СЧЁТЧИК ЛЕТ

 //ЧИСЛО НЕДЕЛЬ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
int WeekEPH1;
//ЧИСЛО ДНЕЙ ПРШЕДШИХ С ЭПОХИ 1980 01 06 00 00 00.00
int DaysEPH1 = 0;

int MonthUTC = 0;
int UsualYears = 0;
int YearEPH = 0;
int LeapYearsWithoutInit = 0;
int DaysThisYear = 0;
int DayCounter = 0;
int DayWeekCounter = 0;
int DayOfMonth = 0;
int HMS_EPH = 0;
int HourEPH = 0;
int MinuteEPH = 0;

double SecondsUTC = 0.0;
/*double */

WeekEPH1 = DatGPS[0];
DaysEPH1 = WeekEPH1*7;
YearEPH = WeekEPH1/52+InitialYear;

LeapYearsWithoutInit = (WeekEPH1/52-((WeekEPH1/52)%4))/4;
UsualYears=((WeekEPH1/52-LeapYearsWithoutInit)-1);

DaysThisYear = DaysEPH1-LeapYearsWithoutInit*DaysYear[2]-UsualYears*DaysYear[1]-DaysYear[0];
   if(YearEPH == 1980)
       DaysThisYear = 6+DaysEPH1;

for (int i = 0; i<8; i++)
    {
   if((HoursPerDayOfWeek [i]<=DatGPS[1]) && (HoursPerDayOfWeek [i+1]>DatGPS[1]))
       DayWeekCounter = i+1;
   }

//ДЛЯ ВИСОКОСНОГО ГОДА
if (YearEPH % 4 == 0)
{
for (int i = 0; i<12; i++)
    {
   if((MonthsLeapYear[i]<=DaysThisYear) && (MonthsLeapYear[i+1]>DaysThisYear))
       MonthUTC = i+1;
   if((MonthsLeapYear[11]<=DaysThisYear)&&(DaysThisYear>367))
           MonthUTC = 12;
   }
}
//ДЛЯ НЕ ВИСОКОСНОГО ГОДА
else {
    for (int i = 0; i<12; i++)
        {
       if((MonthsLeapYear[i]<=DaysThisYear) && (MonthsLeapYear[i+1]>DaysThisYear))
           MonthUTC = i+1;
       if((MonthsLeapYear[11])<=DaysThisYear&&(DaysThisYear>366))
               MonthUTC = 12;
       }
}
std::cout << "MonthUTC \n"<< MonthUTC  << std::endl;
for (int i = 0; i<(MonthUTC); i++)
    {
    if (YearEPH % 4 == 0)
    {
        DayCounter  = MonthsLeapYear[i];
    }
    else
    {
        DayCounter  = Months[i];
    }
    }
DaysThisYear=DaysThisYear+DayWeekCounter-1;
//
DayOfMonth = DaysThisYear-DayCounter;
//
HMS_EPH=DatGPS[1]-(DayWeekCounter-1)*24*3600;
//
HourEPH = floor(HMS_EPH/3600);
MinuteEPH = floor((HMS_EPH-HourEPH*3600)/60);
//ВЫЧИСЛЯЕМ СЕКУНДЫ
SecondsUTC = HMS_EPH-HourEPH*3600-MinuteEPH*60;
NewUTCDate.Date[0]=YearEPH;
NewUTCDate.Date[1]=MonthUTC;
NewUTCDate.Date[2]=DayOfMonth;
NewUTCDate.Date[3]=HourEPH;
NewUTCDate.Date[4]=MinuteEPH;
NewUTCDate.Seconds=SecondsUTC;
return NewUTCDate;
}
