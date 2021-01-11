#include "statfullfile.h"
//#include "ui_statfullfile.h"

//StatFullFile::StatFullFile(QWidget *parent) :
//    QWidget(parent)
//{
//    deltaT = 3;                 // мсек
//}
StatFullFile::StatFullFile()
{
    deltaT = 3;                 // мсек
}



bool StatFullFile::FindKadr(QVector<CadrePPSI> &cadrs, CadrePPSI &PPSI, int &indFind)
{
    QTime T(0,0,0);
    int dist = 0;
    bool findCadr = false;

    for( int i=0; i<cadrs.size(); ++i )
    {
        dist = qAbs( T.msecsTo( PPSI.T ) - T.msecsTo( cadrs[i].T ) );

        if( ( dist > deltaT ) /*&& ( PPSI.T < cadrs[i].T )*/ )
        {
            break;
        }

        if( dist < deltaT )
        {
            if( (PPSI.NvirtKan == cadrs[i].NvirtKan) && ( PPSI.chkad == cadrs[i].chkad ) && ( PPSI.chvirtkan == cadrs[i].chvirtkan ) && cadrs[i].beaten )
            {
                findCadr = true;
                indFind = i;
                return findCadr;
            }
        }
    }

    return findCadr;
}

bool StatFullFile::FindKadr(qint64 indCur, QVector<CadrePPSI> &cadrs, int &indPrev, unsigned short &NVirtKan, QTime &T, bool &beaten )
{
    bool isFind = false;

    for( int c=indPrev; c<cadrs.size(); c++ )
    {
        if( cadrs[c].indSt == indCur )
        {
            indPrev = c;
            NVirtKan = cadrs[c].NvirtKan;
            T = cadrs[c].T;
            beaten = cadrs[c].beaten;

            if( cadrs[c].beaten )
            {
                isFind = true;
                break;
            }
        }
    }

    return isFind;
}

void StatFullFile::build( QByteArray &buf1, QByteArray &buf2, QVector <CadrePPSI> &PPSI1, QVector <CadrePPSI> &PPSI2  )
{
    qint32 nvk = 0;
    bool pr_crc = true;
    bool pr_r = true;
    qint32 kod = 0;
    CadrePPSI tmpPPSI;
    int count = 0;
    QByteArray bt;
    QString bi;
    bool ok;

            /// Обработка по 1ому каналу
    while( pr_r )
    {
        pr_r = cOI->ReadFULL( buf1, kod, nvk, pr_crc );     ///Определение смещения в области памяти FULL файла для первого(очередного) кадра

        tmpPPSI.indSt = cOI->ind_full;
        tmpPPSI.beaten = pr_crc;
        tmpPPSI.NvirtKan = nvk;
        tmpPPSI.T = cOI->TimeKadrFULL( buf1, cOI->ind_full );
        tmpPPSI.em = pr_r;

        tmpPPSI.chkad = cOI->glzag.fl.chkad;
        tmpPPSI.chvirtkan =  cOI->glzag.fl.chvirtkan;
        tmpPPSI.flocpak = cOI->glzag.fl.flocpak;
        tmpPPSI.flsinxr = cOI->glzag.fl.flsinxr;
        tmpPPSI.iddlseg = cOI->glzag.fl.iddlseg;
        tmpPPSI.ident = cOI->glzag.fl.ident;
        tmpPPSI.nver = cOI->glzag.fl.nver;
        tmpPPSI.ukzag = cOI->glzag.fl.ukzag;
        tmpPPSI.vtzag = cOI->glzag.fl.vtzag;
        tmpPPSI.flopk = cOI->glzag.fl.flopk;

        tmpPPSI.ch = cOI->glzag.bt.ch;
        tmpPPSI.identific = cOI->glzag.bt.ident;
        tmpPPSI.status = cOI->glzag.bt.status;

        bt = buf1.mid( cOI->ind_full+528, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.ARU1 = bi.toUInt( &ok, 2 );

        bt = buf1.mid( cOI->ind_full+530, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.ARU2 = bi.toUInt( &ok, 2 );

        bt = buf1.mid( cOI->ind_full+532, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.SiNoRa1 = bi.toUInt( &ok, 2 );

        bt = buf1.mid( cOI->ind_full+534, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.SiNoRa2 = bi.toUInt( &ok, 2 );

        bt = buf1.mid( cOI->ind_full+541, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.FrLen = bi.toUInt( &ok, 2 );

        tmpPPSI.fract = buf1.mid( cOI->ind_full+543, 1 ).toHex().toUpper().toUInt( &ok, 16 );
        tmpPPSI.NumPort = buf1.mid( cOI->ind_full+574, 1 ).toHex().toUpper().toUInt( &ok, 16 );
        tmpPPSI.RecFr = buf1.mid( cOI->ind_full+575, 1 ).toHex().toUpper().toUInt( &ok, 16 );

        if ( kod == 0 )
            kod = 1;

        if ( !pr_r )
            break;

        ++count;
        PPSI1.push_back( tmpPPSI );
    }

                /// Обработка по 2ому каналу
    count = 0;
    kod = 0;
    pr_r = true;
    while( pr_r )
    {
        pr_r = cOI->ReadFULL( buf2, kod, nvk, pr_crc );     ///Определение смещения в области памяти FULL файла для первого(очередного) кадра

        tmpPPSI.indSt = cOI->ind_full;
        tmpPPSI.beaten = pr_crc;
        tmpPPSI.NvirtKan = nvk;
        tmpPPSI.T = cOI->TimeKadrFULL( buf2, cOI->ind_full );
        tmpPPSI.em = pr_r;

        tmpPPSI.chkad = cOI->glzag.fl.chkad;
        tmpPPSI.chvirtkan =  cOI->glzag.fl.chvirtkan;
        tmpPPSI.flocpak = cOI->glzag.fl.flocpak;
        tmpPPSI.flsinxr = cOI->glzag.fl.flsinxr;
        tmpPPSI.iddlseg = cOI->glzag.fl.iddlseg;
        tmpPPSI.ident = cOI->glzag.fl.ident;
        tmpPPSI.nver = cOI->glzag.fl.nver;
        tmpPPSI.ukzag = cOI->glzag.fl.ukzag;
        tmpPPSI.vtzag = cOI->glzag.fl.vtzag;
        tmpPPSI.flopk = cOI->glzag.fl.flopk;

        tmpPPSI.ch = cOI->glzag.bt.ch;
        tmpPPSI.identific = cOI->glzag.bt.ident;
        tmpPPSI.status = cOI->glzag.bt.status;

        bt = buf2.mid( cOI->ind_full+528, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.ARU1 = bi.toUInt( &ok, 2 );

        bt = buf2.mid( cOI->ind_full+530, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.ARU2 = bi.toUInt( &ok, 2 );

        bt = buf2.mid( cOI->ind_full+532, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.SiNoRa1 = bi.toUInt( &ok, 2 );

        bt = buf2.mid( cOI->ind_full+534, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.SiNoRa2 = bi.toUInt( &ok, 2 );

        bt = buf2.mid( cOI->ind_full+541, 2 ).toHex().toUpper();
        bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
             QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
        tmpPPSI.FrLen = bi.toUInt( &ok, 2 );

        tmpPPSI.fract = buf2.mid( cOI->ind_full+543, 1 ).toHex().toUpper().toUInt( &ok, 16 );
        tmpPPSI.NumPort = buf2.mid( cOI->ind_full+574, 1 ).toHex().toUpper().toUInt( &ok, 16 );
        tmpPPSI.RecFr = buf2.mid( cOI->ind_full+575, 1 ).toHex().toUpper().toUInt( &ok, 16 );

        if ( kod == 0 )
            kod = 1;

        if ( !pr_r )
            break;

        ++count;
        PPSI2.push_back( tmpPPSI );
    }
}

void StatFullFile::Record(QTextStream &out)
{
    out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(9) << center << "iter" <<  qSetFieldWidth(3) << center << "|"        // Номер итерации
        << qSetFieldWidth(12) << center << "ind_full" <<  qSetFieldWidth(3) << center << "|"                                          // Индекс фулл
        << qSetFieldWidth(14) << center << "Time" << qSetFieldWidth(3) << center << "|"                                               // Время кадра
        << qSetFieldWidth(5) << center << "CC" << qSetFieldWidth(3) << center << "|"                                                  // Счетчик кадров
        << qSetFieldWidth(5) << center << "VCC" << qSetFieldWidth(3) << center << "|"                                                 // Счетчик виртуальных каналов
        << qSetFieldWidth(6) << center << "pr_crc" << qSetFieldWidth(3) << center << "|"                                              // Признак CRC
        << qSetFieldWidth(7) << center << "flocpak" << qSetFieldWidth(3) << center << "|"                                             // Флаг очередности пакетов
        << qSetFieldWidth(7) << center << "flsinxr" << qSetFieldWidth(3) << center << "|"                                             // Флаг синхронизации
        << qSetFieldWidth(7) << center << "iddlseg" << qSetFieldWidth(3) << center << "|"                                             // Идентификатор длины сегмента
        << qSetFieldWidth(7) << center << "ukzag" << qSetFieldWidth(3) << center << "|"                                               // Указатель 1ого заголовка
        << qSetFieldWidth(11) << center << "identific" << qSetFieldWidth(3) << center << "|"                                          // Идентификатор кадра передачи
        << qSetFieldWidth(8) << center << "status" << qSetFieldWidth(3) << center << "|"                                              // Статус
        << qSetFieldWidth(7) << center << "bt.ch" << qSetFieldWidth(3) << center << "|"                                               // Канал (bt.ch)
        << qSetFieldWidth(6) << center << "ARU1" << qSetFieldWidth(3) << center << "|"                                                // ARU1
        << qSetFieldWidth(6) << center << "ARU2" << qSetFieldWidth(3) << center << "|"                                                // ARU2
        << qSetFieldWidth(6) << center << "S/N1" << qSetFieldWidth(3) << center << "|"                                                // Отношение сигнал/шум 1
        << qSetFieldWidth(6) << center << "S/N2" << qSetFieldWidth(3) << center << "|"                                                // Отношение сигнал/шум 2
        << qSetFieldWidth(10) << center << "Cadr len" << qSetFieldWidth(3) << center << "|"                                           // Длина принятого кадра
        << qSetFieldWidth(5) << center << "fract" << qSetFieldWidth(3) << center << "|"                                               // Счетчик кадров(наземный)
        << qSetFieldWidth(6) << center << "port" << qSetFieldWidth(3) << center << "|"                                                // Номер потока(порта)
        << qSetFieldWidth(7) << center << "RecFr" << qSetFieldWidth(3) << center << "|"                                               // Счетчик записанных кадров
        << qSetFieldWidth(5) << center << "VKan" << qSetFieldWidth(3) << center << "|"                                                // Виртуальный канал
        << qSetFieldWidth(1) << center << endl;
}

void StatFullFile::Record(QTextStream &out, CadrePPSI &ppsi, int c)
{
    out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(9) << center << c <<  qSetFieldWidth(3) << center << "|"  // Номер итерации
        << qSetFieldWidth(12) << center << ppsi.indSt <<  qSetFieldWidth(3) << center << "|"                            // Индекс фулл
        << qSetFieldWidth(14) << center << ppsi.T.toString("hh:mm:ss.zzz") << qSetFieldWidth(3) << center << "|"        // Время кадра
        << qSetFieldWidth(5)  << center << ppsi.chkad << qSetFieldWidth(3)  << center << "|"                             // Счетчик кадров
        << qSetFieldWidth(5)  << center << ppsi.chvirtkan << qSetFieldWidth(3) << center << "|"                          // Счетчик виртуальных каналов
        << qSetFieldWidth(6)  << center << ppsi.beaten << qSetFieldWidth(3) << center << "|"                            // Признак CRC
        << qSetFieldWidth(7)  << center << ppsi.flocpak << qSetFieldWidth(3) << center << "|"                           // Флаг очередности пакетов
        << qSetFieldWidth(7)  << center << ppsi.flsinxr << qSetFieldWidth(3) << center << "|"                           // Флаг синхронизации
        << qSetFieldWidth(7)  << center << ppsi.iddlseg << qSetFieldWidth(3) << center << "|"                           // Идентификатор длины сегмента
        << qSetFieldWidth(7)  << center << ppsi.ukzag << qSetFieldWidth(3) << center << "|"                             // Указатель 1ого заголовка
        << qSetFieldWidth(11) << center << ppsi.identific << qSetFieldWidth(3) << center << "|"                         // Идентификатор кадра передачи
        << qSetFieldWidth(8)  << center << ppsi.status << qSetFieldWidth(3) << center << "|"                            // Статус
        << qSetFieldWidth(7)  << center << ppsi.ch << qSetFieldWidth(3) << center << "|"                                // Канал(bt.ch)
        << qSetFieldWidth(6)  << center << ppsi.ARU1 << qSetFieldWidth(3) << center << "|"                              // ARU1
        << qSetFieldWidth(6)  << center << ppsi.ARU2 << qSetFieldWidth(3) << center << "|"                              // ARU2
        << qSetFieldWidth(6)  << center << ppsi.SiNoRa1 << qSetFieldWidth(3) << center << "|"                           // Отношение сигнал/шум 1
        << qSetFieldWidth(6)  << center << ppsi.SiNoRa2 << qSetFieldWidth(3) << center << "|"                           // Отношение сигнал/шум 2
        << qSetFieldWidth(10) << center << ppsi.FrLen << qSetFieldWidth(3) << center << "|"                            // Длина прятого кадра
        << qSetFieldWidth(5)  << center << ppsi.fract << qSetFieldWidth(3) << center << "|"                             // Счетчик кадров(наземный)
        << qSetFieldWidth(6)  << center << ppsi.NumPort << qSetFieldWidth(3) << center << "|"                           // Номер потока(порта)
        << qSetFieldWidth(7)  << center << ppsi.RecFr << qSetFieldWidth(3) << center << "|"                             // Счетчик записанных кадров
        << qSetFieldWidth(5)  << center << ppsi.NvirtKan << qSetFieldWidth(3) << center << "|"                          // Виртуальный канал
        << qSetFieldWidth(1)  << center << endl;
}

bool operator ==( const CadrePPSI& left, const CadrePPSI& right )
{
    if( (left.NvirtKan == right.NvirtKan) && (left.chkad == right.chkad) && (left.chvirtkan == right.chvirtkan) )
        return true;

    return false;
}

void StatFullFile::Collation3( QVector<CadrePPSI> &cadrs1, QVector<CadrePPSI> &cadrs2, QVector <Cadrs> &Cad )
{
    Cadrs tmpCad;
    int dist = 0;
    int match = 0;

    if(!cadrs1.isEmpty() && cadrs2.isEmpty())
    {
//        qDebug() << "cadrs1 is empty !!!";
        for(QVector<CadrePPSI>::const_iterator it=cadrs1.begin(); it<cadrs1.end(); ++it)
        {
            if(!it->em)
                continue;

            // По 1ому каналу
            tmpCad.T1 = it->T;
            tmpCad.em1 = it->em;
            if(it->beaten)
            {
                tmpCad.beaten1 = true;
                tmpCad.cad1 = 1;
                tmpCad.cadr1 = true;
            }

            else
            {
                tmpCad.beaten1 = false;
                tmpCad.cad1 = 0;
                tmpCad.cadr1 = false;
            }

            tmpCad.indSt1 = it->indSt;
            tmpCad.chkad1 = it->chkad;
            tmpCad.chvirtkan1 = it->chvirtkan;
            tmpCad.NVirtKan1 = it->NvirtKan;

            // По 2ому каналу
            tmpCad.T2 = it->T;
            tmpCad.em2 = false;
            tmpCad.beaten2 = false;
            tmpCad.cad2 = 0;
            tmpCad.cadr2 = false;
            tmpCad.indSt2 = it->indSt;;
            tmpCad.chkad2 = it->chkad;
            tmpCad.chvirtkan2 = it->chvirtkan;
            tmpCad.NVirtKan2 = it->NvirtKan;

            Cad.push_back(tmpCad);
        }

        return;
    }

    if(cadrs1.isEmpty() && !cadrs2.isEmpty())
    {
//        qDebug() << "cadrs2 is empty !!!";
        for(QVector<CadrePPSI>::const_iterator it=cadrs2.begin(); it<cadrs2.end(); ++it)
        {
            if(!it->em)
                continue;

            // По 1ому каналу
            tmpCad.T1 = it->T;
            tmpCad.em1 = false;
            tmpCad.beaten1 = false;
            tmpCad.cad1 = 0;
            tmpCad.cadr1 = false;
            tmpCad.indSt1 = it->indSt;;
            tmpCad.chkad1 = it->chkad;
            tmpCad.chvirtkan1 = it->chvirtkan;
            tmpCad.NVirtKan1 = it->NvirtKan;

            // По 2ому каналу
            tmpCad.T2 = it->T;
            tmpCad.em2 = it->em;
            if(it->beaten)
            {
                tmpCad.beaten2 = true;
                tmpCad.cad2 = 1;
                tmpCad.cadr2 = true;
            }

            else
            {
                tmpCad.beaten2 = false;
                tmpCad.cad2 = 0;
                tmpCad.cadr2 = false;
            }

            tmpCad.indSt2 = it->indSt;
            tmpCad.chkad2 = it->chkad;
            tmpCad.chvirtkan2 = it->chvirtkan;
            tmpCad.NVirtKan2 = it->NvirtKan;

            Cad.push_back(tmpCad);
        }

        return;
    }

    if(cadrs1.isEmpty() && cadrs2.isEmpty())
        return;

    QList<CadrePPSI> list_cad1;
    QList<CadrePPSI> list_cad2;

    for(QVector<CadrePPSI>::const_iterator iter=cadrs1.begin(); iter<cadrs1.end(); ++iter)
    {
        if(iter->em == true)
            list_cad1.push_back(*iter);
    }

    for(QVector<CadrePPSI>::const_iterator iter=cadrs2.begin(); iter<cadrs2.end(); ++iter)
    {
        if(iter->em == true)
            list_cad2.push_back(*iter);
    }

//    qDebug() << "It's Collation3 !!! deltaT = " << deltaT << " Sz list_cad1 = " << list_cad1.size() << " Sz list_cad2 = " << list_cad2.size();

    for(QList<CadrePPSI>::iterator iter1 = list_cad1.begin(); iter1<list_cad1.end(); ++iter1)
    {
        for(QList<CadrePPSI>::iterator iter2 = list_cad2.begin(); iter2<list_cad2.end(); ++iter2)
        {
            dist = qAbs(iter1->T.msecsSinceStartOfDay()-iter2->T.msecsSinceStartOfDay());
            if(dist <= deltaT)
            {
                if(*iter1 == *iter2)
                {
                    // По 1ому каналу
                    tmpCad.T1 = iter1->T;
                    tmpCad.em1 = iter1->em;
                    if(iter1->beaten)
                    {
                        tmpCad.beaten1 = true;
                        tmpCad.cad1 = 1;
                        tmpCad.cadr1 = true;
                    }
                    else
                    {
                        tmpCad.beaten1 = false;
                        tmpCad.cad1 = 0;
                        tmpCad.cadr1 = false;
                    }
                    tmpCad.indSt1 = iter1->indSt;
                    tmpCad.chkad1 = iter1->chkad;
                    tmpCad.chvirtkan1 = iter1->chvirtkan;
                    tmpCad.NVirtKan1 = iter1->NvirtKan;

                    // По 2ому каналу
                    tmpCad.T2 = iter2->T;
                    tmpCad.em2 = iter2->em;
                    if( iter2->beaten )
                    {
                        tmpCad.beaten2 = true;
                        tmpCad.cad2 = 1;
                        tmpCad.cadr2 = true;
                    }
                    else
                    {
                        tmpCad.beaten2 = false;
                        tmpCad.cad2 = 0;
                        tmpCad.cadr2 = false;
                    }
                    tmpCad.indSt2 = iter2->indSt;
                    tmpCad.chkad2 = iter2->chkad;
                    tmpCad.chvirtkan2 = iter2->chvirtkan;
                    tmpCad.NVirtKan2 = iter2->NvirtKan;

                    Cad.push_back(tmpCad);
                    list_cad1.erase(iter1);
                    list_cad2.erase(iter2);
                    ++match;

                    if(iter1 > list_cad1.begin())
                        --iter1;

                    if(iter2 > list_cad2.begin())
                         --iter2;

                    break;
                }
            }

            else if(iter2->T.msecsSinceStartOfDay() - iter1->T.msecsSinceStartOfDay() > deltaT)
                break;
        }
    }

//    qDebug() << "Size of list_cad1(after erase) = " << list_cad1.size() << "\t Size of list_cad2(after erase) = " << list_cad2.size();
//    qDebug() << "match = " << match;

    if(!list_cad1.isEmpty())
    {
//        qDebug() << "Run treatment for list_cad1 !";
        for(QList<CadrePPSI>::const_iterator iter=list_cad1.begin(); iter<list_cad1.end(); ++iter)
        {
            tmpCad.T1 = iter->T;
            if(iter->beaten)
            {
                tmpCad.beaten1 = true;
                tmpCad.cad1 = 1;
                tmpCad.cadr1 = true;
            }

            else
            {
                tmpCad.beaten1 = false;
                tmpCad.cad1 = 0;
                tmpCad.cadr1 = false;
            }

            tmpCad.indSt1 = iter->indSt;
            tmpCad.chkad1 = iter->chkad;
            tmpCad.chvirtkan1 = iter->chvirtkan;
            tmpCad.NVirtKan1 = iter->NvirtKan;

            tmpCad.T2 = iter->T;
            tmpCad.beaten2 = false;
            tmpCad.cad2 = 0;
            tmpCad.cadr2 = false;
            tmpCad.indSt2 = iter->indSt;;
            tmpCad.chkad2 = iter->chkad;
            tmpCad.chvirtkan2 = iter->chvirtkan;
            tmpCad.NVirtKan2 = iter->NvirtKan;

            Cad.push_back(tmpCad);
        }
    }

    if(!list_cad2.isEmpty())
    {
//        qDebug() << "Run treatment for list_cad2 !";
        for(QList<CadrePPSI>::const_iterator iter=list_cad2.begin(); iter<list_cad2.end(); ++iter)
        {
            tmpCad.T2 = iter->T;
            if( iter->beaten )
            {
                tmpCad.beaten2 = true;
                tmpCad.cad2 = 1;
                tmpCad.cadr2 = true;
            }
            else
            {
                tmpCad.beaten2 = false;
                tmpCad.cad2 = 0;
                tmpCad.cadr2 = false;
            }
            tmpCad.indSt2 = iter->indSt;
            tmpCad.chkad2 = iter->chkad;
            tmpCad.chvirtkan2 = iter->chvirtkan;
            tmpCad.NVirtKan2 = iter->NvirtKan;

            tmpCad.T1 = iter->T;
            tmpCad.beaten1 = false;
            tmpCad.cad1 = 0;
            tmpCad.cadr1 = false;
            tmpCad.indSt1 = iter->indSt;;
            tmpCad.chkad1 = iter->chkad;
            tmpCad.chvirtkan1 = iter->chvirtkan;
            tmpCad.NVirtKan1 = iter->NvirtKan;

            Cad.push_back(tmpCad);
        }
    }

//    QFile f( "Cadrs.txt" );
//    f.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
//    QTextStream out(&f);

//    out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(12) << center << "IndSt1" <<  qSetFieldWidth(3) << center << "|"     // Индекс фулл1
//        << qSetFieldWidth(12) << center << "IndSt2" <<  qSetFieldWidth(3) << center << "|"                                            // Индекс фулл2
//        << qSetFieldWidth(6) << center << "cad1" << qSetFieldWidth(3) << center << "|"
//        << qSetFieldWidth(6) << center << "cad2" << qSetFieldWidth(3) << center << "|"
//        << qSetFieldWidth(7) << center << "pr_crc1" << qSetFieldWidth(3) << center << "|"                                             // Признак CRC1
//        << qSetFieldWidth(7) << center << "pr_crc2" << qSetFieldWidth(3) << center << "|"                                             // Признак CRC2
//        << qSetFieldWidth(14) << center << "Time1" << qSetFieldWidth(3) << center << "|"                                              // Время кадра1
//        << qSetFieldWidth(14) << center << "Time2" << qSetFieldWidth(3) << center << "|"                                              // Время кадра2
//        << qSetFieldWidth(5) << center << "VKan1" << qSetFieldWidth(3) << center << "|"                                               // Виртуальный канал1
//        << qSetFieldWidth(5) << center << "VKan2" << qSetFieldWidth(3) << center << "|"                                               // Виртуальный канал2
//        << qSetFieldWidth(5) << center << "CC1" << qSetFieldWidth(3) << center << "|"                                                 // Счетчик кадров1
//        << qSetFieldWidth(5) << center << "CC2" << qSetFieldWidth(3) << center << "|"                                                 // Счетчик кадров2
//        << qSetFieldWidth(6) << center << "VCC1" << qSetFieldWidth(3) << center << "|"                                                // Счетчик виртуальных каналов1
//        << qSetFieldWidth(6) << center << "VCC2" << qSetFieldWidth(3) << center << "|"                                                // Счетчик виртуальных каналов2
//        << qSetFieldWidth(1) << center << endl;

//    for( int c=0; c<Cad.size(); c++ )
//    {
//        out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(12) << center << Cad.at(c).indSt1 << qSetFieldWidth(3) << center << "|"    // Индекс фулл2
//            << qSetFieldWidth(12) << center << Cad.at(c).indSt2 <<  qSetFieldWidth(3) << center << "|"                                          // Индекс фулл2
//            << qSetFieldWidth(7) << center << Cad.at(c).cad1 << qSetFieldWidth(3) << center << "|"
//            << qSetFieldWidth(7) << center << Cad.at(c).cad2 << qSetFieldWidth(3) << center << "|"
//            << qSetFieldWidth(7) << center << Cad.at(c).beaten1 << qSetFieldWidth(3) << center << "|"                                           // Признак CRC1
//            << qSetFieldWidth(7) << center << Cad.at(c).beaten2 << qSetFieldWidth(3) << center << "|"                                           // Признак CRC2
//            << qSetFieldWidth(14) << center << Cad.at(c).T1.toString("hh:mm:ss.zzz") << qSetFieldWidth(3) << center << "|"                      // Время кадра1
//            << qSetFieldWidth(14) << center << Cad.at(c).T2.toString("hh:mm:ss.zzz") << qSetFieldWidth(3) << center << "|"                      // Время кадра2
//            << qSetFieldWidth(5) << center << Cad.at(c).NVirtKan1 << qSetFieldWidth(3) << center << "|"                                         // Виртуальный канал1
//            << qSetFieldWidth(5) << center << Cad.at(c).NVirtKan2 << qSetFieldWidth(3) << center << "|"                                         // Виртуальный канал2
//            << qSetFieldWidth(5) << center << Cad.at(c).chkad1 << qSetFieldWidth(3) << center << "|"                                            // Счетчик кадров1
//            << qSetFieldWidth(5) << center << Cad.at(c).chkad2 << qSetFieldWidth(3) << center << "|"                                            // Счетчик кадров2
//            << qSetFieldWidth(6) << center << Cad.at(c).chvirtkan1 << qSetFieldWidth(3) << center << "|"                                        // Счетчик виртуальных каналов1
//            << qSetFieldWidth(6) << center << Cad.at(c).chvirtkan2 << qSetFieldWidth(3) << center << "|"                                        // Счетчик виртуальных каналов2
//            << qSetFieldWidth(1) << center << endl;
//    }

//    f.close();
}

Cadrs::Cadrs()
{
    cad1 = 0, cad2 = 0;
    NVirtKan1 = 0, NVirtKan2 = 0;
    T1.setHMS(0,0,0), T2.setHMS(0,0,0);
    indSt1 = 0, indSt2 = 0;
    em1 = false, em2 = false;
    cadr1 = false, cadr2 = false;
    beaten1 = true, beaten2 = true;
    chkad1 = 0, chkad2 = 0;
    chvirtkan1 = 0, chvirtkan2 = 0;
    delta_t = 3;
}

bool operator==(const Cadrs &left, const Cadrs &right)
{
    bool res = false;

    res  = (left.beaten1 == right.beaten1);         res &= (left.beaten2 == right.beaten2);
    res &= (left.cad1 == right.cad1);               res &= (left.cad2 == right.cad2);
    res &= (left.cadr1 == right.cadr1);             res &= (left.cadr2 == right.cadr2);
    res &= (left.chkad1 == right.chkad1);           res &= (left.chkad2 == right.chkad2);
    res &= (left.chvirtkan1 == right.chvirtkan1);   res &= (left.chvirtkan2 == right.chvirtkan2);
    res &= (left.em1 == right.em1);                 res &= (left.em2 == right.em2);
    res &= (left.NVirtKan1 == right.NVirtKan1);     res &= (left.NVirtKan2 == right.NVirtKan2);

    return res;
}

CadrePPSI::CadrePPSI()
{
    indSt = 0;
    indSinxr = 0;
    T.setHMS(0,0,0);
    NvirtKan = 0;
    em = false;
    beaten = false;
    count = 0;
    chek_sum = 0;
}
