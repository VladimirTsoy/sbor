#include "threadobr12.h"

ThreadObr12::ThreadObr12()
{

}

void ThreadObr12::run()
{
 db=QSqlDatabase::addDatabase("QPSQL","obr12");
 db.setDatabaseName("geoik");
 db.setUserName(cOI->nikp);
 db.setPassword(cOI->passp);
 db.setHostName(cOI->hostp);
 if (!db.open())
 {
  str=db.lastError();
  return;
 }

 q=QSqlQuery(db);

 cOI->ObrKod12(1,cOI->zapust.path_temp,false,nz,q);
}
