#include "renexwriter.h"
#include "jpswriter.h"
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "jpsreader.h"
#include "epoch.h"
#include "eglonass.h"
#include "egps.h"
//#include "windows.h"
#include "rnxnconv.h"

RnxNconv::RnxNconv(QFile *RINEX)
{

    bool isGPS =1;
    bool isGLO =1;
    RINEX->open(QIODevice::ReadOnly);
    QStringList H ;
    QStringList AllEph;
    QStringList CurEph;
    AllEph<<" ";
    quint8 NofEofH;
    QTextStream in(RINEX);
       while (!in.atEnd())
       {
           QString line = in.readLine();
           QStringList *A = new QStringList (line);

           if(A->length()==0){
               break;
           }


           for(qint32 i = 0; i<A->size(); i++)
           {
               if (A->at(i).endsWith("END OF HEADER")  || A->at(i).endsWith("END OF HEADER       "))
                   {
                     NofEofH = i;
                   }
              qDebug() << A->at(i);
           }
           /*for(qint32 i = 0; i<A->at(NofEofH); i++)
           {
               H<<A->at(i);

           }
           for(qint32 i = NofEofH+1; i<A->atEnd(); i++)
           {

               AllEph<<A->at(i);
           }

               if(isGPS)
               {
                   for(qint32 il = 1; il<AllEph.atEnd(); il+8)
                   {
                       CurEph<<AllEph.at(il)  <<AllEph.at(il+1)<<AllEph.at(il+2)<<AllEph.at(il+3)
                             <<AllEph.at(il+4)<<AllEph.at(il+5)<<AllEph.at(il+6)<<AllEph.at(il+7);
                   }
               EphemeridGPS *E = new Epoch(H,CurEph,2);
               }
               if(isGLO)
               {
                   for(qint32 il = 1; il<AllEph.atEnd(); il+4)
                   {
                       CurEph<<AllEph.at(il)  <<AllEph.at(il+1)<<AllEph.at(il+2)<<AllEph.at(il+3);
                   }
               EphemeridGLO *E = new Epoch(H,CurEph,2);
               }

           }

       if(Version==2)
       {
           if(isGPS)
           {

           }
           if(isGLO)
           {

           }
       }
       if(Version==3)
       {}
*/
          // RINEX->close();
}
}

