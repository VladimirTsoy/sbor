#ifndef THREADOBR12_H
#define THREADOBR12_H
#include "obrizm.h"


class ThreadObr12 : public QThread
{
public:
    ThreadObr12();
    ObrIzm *cOI;
    void run();
    qlonglong nz;
    QSqlDatabase db;
    QSqlQuery q;
    QSqlError str;
};

#endif // THREADOBR12_H
