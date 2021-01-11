#ifndef TIMECONVERSION_H
#define TIMECONVERSION_H

namespace timeConversion {
    struct DateUTC;
    //vector<double> ConvertDatToWeekSec(double Sec, int Dat[5] );
    DateUTC ConvertDatToUTC(double DatGPS[2]);
}

#endif // TIMECONVERSION_H
