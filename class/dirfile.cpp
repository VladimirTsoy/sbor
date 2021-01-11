#include "dirfile.h"
#include <QtWidgets>

DirFile::DirFile()
{

}

bool DirFile::ClearDir(QString path)
{
 QStringList filelist;
 QDir dir;
 int i;
 QString file;
 QFile rfile;

 dir.setPath(path);
 filelist.clear();

 filelist = dir.entryList(QStringList() << "*.*",QDir::Files);

 for (i=0; i<filelist.count(); i++)
 {
  file=path+"/"+filelist.value(i);
  rfile.setFileName(file);
  if (!rfile.remove()) return false;
 }

 return true;
}

QStringList DirFile::FileInfo(QString file)
{
 QStringList list;
 QFileInfo inf;
 QString str;
 qint32 ind;

 inf.setFile(file);

 str=inf.fileName();
 ind=str.lastIndexOf(".");
 str=str.mid(0,ind);

 list.append(inf.absoluteFilePath());
 list.append(inf.absolutePath());
 list.append(inf.fileName());
 list.append(str);
// list.append((inf.baseName()));
 list.append(inf.suffix());
 list.append(QString::number(inf.size()));

 return list;
}

QString DirFile::FileExt(QString file)
{
 QString ext="";
 QFileInfo inf;

 inf.setFile(file);

 ext=inf.suffix();
// ext=ext.mid(1);
 return ext;
}

QStringList DirFile::SpisAllFile(QString dir)
{
    QStringList rez_spis;
    QStringList rab_spis, rab_spis1;
    int kol;
    bool pr=true;

    rez_spis.clear();
    rez_spis=SpisPathFile(dir);

    rab_spis=SpisAllDir(dir);
    kol=rab_spis.count();
    if (kol == 0) return rez_spis;

    while (pr)
    {
     rab_spis1.clear();
     List2ToList1(rab_spis1,rab_spis,"");
     rab_spis.clear();
     rab_spis=SpisPathFile(rab_spis1);
     kol=List2ToList1(rez_spis,rab_spis,"");
     if (kol == 0) pr=false;
    }

    return rez_spis;
}

QStringList DirFile::SpisDir(QString dir)
{
 QStringList spis_dir;
 QDir tdir;

 spis_dir.clear();

 if (! tdir.cd(dir)) return spis_dir;
 spis_dir=tdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

 return spis_dir;
}

QStringList DirFile::SpisPathDir(QString dir)
{
 QStringList rez_spis;
 QStringList spis_dir;
 QString pref;

 rez_spis.clear();
 pref=dir+"/";

 spis_dir=SpisDir(dir);
 List2ToList1(rez_spis,spis_dir,pref);

 return rez_spis;
}

int DirFile::List2ToList1(QStringList &list1,QStringList &list2,QString pref)
{
 int ret=0;
 QString str;

 for (int i=0; i<list2.count(); i++)
 {
  str=pref+list2[i];
  list1 << str;
  ret++;
 }

 return ret;
}

QStringList DirFile::SpisPathDir(QStringList spis_dir)
{
 QStringList rez_spis;
 QStringList rab_spis;

 rez_spis.clear();

 for(int i=0; i<spis_dir.count(); i++)
 {
  rab_spis.clear();
  rab_spis=SpisDir(spis_dir[i]);
  List2ToList1(rez_spis,rab_spis,spis_dir[i]+"/");
 }

 return rez_spis;
}

QStringList DirFile::SpisAllDir(QString dir)
{
 QStringList rez_spis;
 QStringList rab_spis, rab_spis1;
 int kol;
 bool pr=true;

 rez_spis.clear();

 rab_spis=SpisPathDir(dir);
 kol=List2ToList1(rez_spis,rab_spis,"");
 if (kol == 0) return rez_spis;

 while (pr)
 {
  rab_spis1.clear();
  List2ToList1(rab_spis1,rab_spis,"");
  rab_spis.clear();
  rab_spis=SpisPathDir(rab_spis1);
  kol=List2ToList1(rez_spis,rab_spis,"");
  if (kol == 0) pr=false;
 }

 return rez_spis;
}

QStringList DirFile::SpisFile(QString dir)
{
 QStringList spis_file;
 QDir tdir;

 spis_file.clear();

 if (! tdir.cd(dir)) return spis_file;
 spis_file=tdir.entryList(QDir::Files);

 return spis_file;
}

QStringList DirFile::SpisPathFile(QString dir)
{
    QStringList rez_spis;
    QStringList spis_dir;
    QString pref;

    rez_spis.clear();
    pref=dir+"/";

    spis_dir=SpisFile(dir);
    List2ToList1(rez_spis,spis_dir,pref);

    return rez_spis;
}

QStringList DirFile::SpisPathFile(QStringList spis_dir)
{
    QStringList rez_spis;
    QStringList rab_spis;

    rez_spis.clear();

    for(int i=0; i<spis_dir.count(); i++)
    {
     rab_spis.clear();
     rab_spis=SpisFile(spis_dir[i]);
     List2ToList1(rez_spis,rab_spis,spis_dir[i]+"/");
    }

    return rez_spis;
}

QStringList DirFile::ReadFileInList(QString NFile)
{
    QStringList list;
    QFile file;
    QString str;

    file.setFileName(NFile);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        WriteLog("ReadFileInList : Ошибка открытия файла: "+NFile);
        return list;
    }

    QTextStream stream(&file);

    do
    {
        str=stream.readLine();
        list.append(str);
        qApp->processEvents();
    } while (!stream.atEnd());

    file.close();
    return list;
}

QStringList DirFile::ReadFileInList(QString NFile,QString str)
{
 QStringList list;
 QFile file;
 QString str1;
 qint32 ind;

 file.setFileName(NFile);

 if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
 {
  WriteLog("ReadFileInList : Ошибка открытия файла: "+NFile);
  return list;
 }

 QTextStream stream(&file);

 do
 {
  str1=stream.readLine();
  ind=str1.indexOf(str);
  list.append(str1);
  if (ind >= 0) break;
 } while (!stream.atEnd());

 file.close();
 return list;
}

bool DirFile::getBit(qint8 dat, qint8 nom) //не проверялось
{
 bool ret=false;

 switch(nom)
 {
  case 1: ret=0&dat; break;
  case 2: ret=2&dat; break;
  case 3: ret=4&dat; break;
  case 4: ret=8&dat; break;
  case 5: ret=16&dat; break;
  case 6: ret=32&dat; break;
  case 7: ret=64&dat; break;
  case 8: ret=128&dat; break;
  default: ret=false;break;
 }

 return ret;
}

QDate DirFile::DayToDate(QString day,QString year)
{
 QDate date,date1;
 int i_year,i_day;
 QString day1;

 i_year=year.toInt();
 if (i_year < 100) i_year=i_year+2000;

 day1=day.trimmed();
 i_day=day1.toInt();
 i_day=i_day-1;

 date.setDate(i_year,1,1);
 date1=date.addDays(i_day);

 return date1;
}

QDate DirFile::DayToDate(int day, int year)
{
 QDate date,date1;

 if (year < 100) year=year+2000;

 day=day-1;

 date.setDate(year,1,1);
 date1=date.addDays(day);

 return date1;
}

QTime DirFile::SecToTime(QString sec)
{
 QTime time;
 QString sec1;
 qlonglong i_sec;

 sec1=sec.trimmed();
 i_sec=sec1.toLongLong();

 time.setHMS(0,0,0);
 time=time.addSecs(i_sec);

 return time;
}

QTime DirFile::SecToTime(quint64 sec, QString &stime)
{
 QTime time;

 time.setHMS(0,0,0);
 time=time.addSecs(sec);
 stime=time.toString("hh:mm:ss");

 return time;
}

QString DirFile::DayToDateS(QString day,QString year)
{
 QDate date;
 QString dates;

 date=DayToDate(day,year);
 dates=date.toString("dd-MM-yyyy");

 return dates;
}

QString DirFile::SecToTimeS(QString sec)
{
 QTime time;
 QString times;

 time=SecToTime(sec);
 times=time.toString("hh:mm:ss");

 return times;
}

bool DirFile::WriteFileFromList(QStringList list,QString NFile)
{
 QFile file;
 QString str;

 file.setFileName(NFile);

 if (file.exists()) file.remove();

 if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
 {
  WriteLog("WriteFileFromList : Ошибка открытия файла: "+NFile);
  return false;
 }

 QTextStream stream(&file);

 for (int i=0; i<list.count(); i++)
 {
  str=list[i];
  stream << str+"\n";
 }

 file.close();
 return true;
}

bool DirFile::isValidNameFile(QString &nfile)
{
 QFile f;
 QString str,s1,s2,s3,s4,s5,newfile;
 QDate dt;
 int d1,d2;
 QStringList finfo;

 finfo=FileInfo(nfile);

 f.setFileName(nfile);

 if (! f.open(QIODevice::ReadOnly))
 {
   f.close();
   return false;
 }

 QTextStream stream(&f);

 str=stream.readLine();
 while (str.indexOf("END OF HEADER")<0)
 {
  str=stream.readLine();
  if (str.indexOf("TIME OF FIRST OBS")>0)
  {
   s1=str.mid(0,6).trimmed();
   s2=str.mid(7,6).trimmed();
   s3=str.mid(14,6).trimmed();
   dt=QDate(s1.toInt(),s2.toInt(),s3.toInt());
   d1=dt.dayOfYear(); //из содержимого
   s4=finfo[2];
   d2=s4.mid(4,3).toInt();//из названия
   if (d1 != d2)
   {
    newfile=s4.mid(0,4);
    s5=QString::number(d1);
    if (s5.size() == 1) newfile=newfile+"00"+s5;
    if (s5.size() == 2) newfile=newfile+"0"+s5;
    if (s5.size() == 3) newfile=newfile+s5;
    newfile=newfile+s4.mid(7,5);
    nfile=finfo[1]+"/"+newfile;
    f.close();
    return false;
   }
   break;
  }
 }


 f.close();
 return true;
}

bool DirFile::isValidNameFileN(QString &nfile)
{
 QFile f;
 QString str,s1,s2,s3,s4,s5,newfile;
 QDate dt;
 int d1,d2;
 QStringList finfo;

 finfo=FileInfo(nfile);

 f.setFileName(nfile);

 if (! f.open(QIODevice::ReadOnly))
 {
   f.close();
   return false;
 }

 QTextStream stream(&f);

 d1=0; d2=0;
 str=stream.readLine();
 while (str.indexOf("END OF HEADER")<0)
 {
  str=stream.readLine();
  if (str.indexOf("DATE")>0)
  {
   s1=str.simplified();
   s2=s1.section(" ",0,0);
   if (s2 == "GSA2_RINEX_CONV")
   {
    s2=s1.section(" ",1,1);
    s3=s2.mid(0,4).trimmed();
    s4=s2.mid(4,2).trimmed();
    s5=s2.mid(6,2).trimmed();
    dt=QDate(s3.toInt(),s4.toInt(),s5.toInt());
    if (! dt.isValid()) return true;
    d1=dt.dayOfYear(); //из содержимого
    s4=finfo[2];
    d2=s4.mid(4,3).toInt();//из названия
   }
   if (d1 != d2)
   {
    newfile=s4.mid(0,4);
    s5=QString::number(d1);
    if (s5.size() == 1) newfile=newfile+"00"+s5;
    if (s5.size() == 2) newfile=newfile+"0"+s5;
    if (s5.size() == 3) newfile=newfile+s5;
    newfile=newfile+s4.mid(7,5);
    nfile=finfo[1]+"/"+newfile;
    f.close();
    return false;
   }
   break;
  }
 }


 f.close();
 return true;
}

bool DirFile::ArxFile(QString nfile,qint8 pr,qulonglong &idlf,qint16 &iks,qulonglong &adlf,qint16 &aks,QByteArray &buf)
{
 idlf=0;
 iks=0;
 adlf=0;
 aks=0;

 buf.clear();

 if (!ReadFile(nfile,buf)) return false;

 if (! ArxFileInMem(pr,idlf,iks,adlf,aks,buf)) return false;

 return true;
}

/*!
 * \brief DirFile::ArxFileInMem
 * выполняет операции упаковки(распаковки), расчёта исходной и архивной длины и КС содержимого файла в области памяти
 * \param pr[in] признак работы: 1-архивировать файл; 2-разархивировать файл
 * \param idlf[out] длина исходного файла (до помещения)
 * \param iks[out] КС исходного файла
 * \param adlf[out] длина архивного файла (хранимого)
 * \param aks[out] КС архивного файла
 * \param buf[in] область памяти с содержимым архивного файла
 * \return возвращает false при неудачной обработке; иначе true
*/
bool DirFile::ArxFileInMem(qint8 pr,qulonglong &idlf,qint16 &iks,qulonglong &adlf,qint16 &aks,QByteArray &buf)
{
 idlf=0;
 iks=0;
 adlf=0;
 aks=0;

 idlf=buf.length();
 iks=qChecksum(buf,idlf);

 if (idlf == 0) return false;

 if (pr == 1) CompressBuf(buf,9);

 if (pr == 2) UncompressBuf(buf);

 adlf=buf.length();
 aks=qChecksum(buf,adlf);

 return true;
}

QByteArray DirFile::ReadFile(QString file)
{
 QByteArray buf;
 QFile f;

 buf.clear();

 f.setFileName(file);
 if (! f.open(QIODevice::ReadOnly))
 {
  WriteLog("ReadFile : Ошибка открытия файла: "+file);
  return buf;
 }
 buf=f.readAll();
 f.close();

 return buf;
}

qint16 DirFile::KS(QByteArray buf)
{
 qint16 ks=0;

 int len=buf.length();
 ks=qChecksum (buf,len);

 return ks;
}

/*!
 * \brief DirFile::WriteFile
 * записывает содержимое области памяти на диск
 * \param file[in] - название создаваемого файла
 * \param buf[in] - область памяти с содержимым файла
 * \param prud[in] - признак перезаписи файла: true - файл можно перезаписать; false - файл не перезаписывается
 * \return в случае успешного завершения возвращает 0; иначе -1.
 */
int DirFile::WriteFile(QString file,QByteArray buf,bool prud)
{
    QFile f;
    int ret=0;

    f.setFileName(file);

    if (f.exists())
    {
        if (prud) DelFileM(file);
        else return -1;
    }
    if (! f.open(QIODevice::WriteOnly))
    {
        WriteLog("WriteFile : Ошибка открытия файла: "+file);
        return -1;
    }
    if (f.write(buf) == -1)
    {
        WriteLog("WriteFile : Ошибка записи в файл: "+file);
        ret=-1;
    }
    f.close();

    return ret;
}

void DirFile::CompressBuf(QByteArray &buf,int level)
{
 QByteArray buf1;

 buf1=qCompress(buf,level);
 buf.clear();
 buf=buf1.repeated(1);

 return;
}

void DirFile::UncompressBuf(QByteArray &buf)
{
 QByteArray buf1;

 buf1=qUncompress(buf);
 buf.clear();
 buf=buf1.repeated(1);

 return;
}

int DirFile::FindStrInList(QStringList list,QString par)
{
 return FindStrInList(list,par,0,1);
}

int DirFile::FindStrInList(QStringList list,QString par,int ind,short reg)
{
 int ret;
 QString str;
 int nstr;
 qlonglong kolstr;

 kolstr=list.count();

 if (reg <3)
 {
  if (reg == 1) nstr=0;
  if (reg == 2) nstr=ind;

  for (int i=nstr; i<kolstr; i++)
  {
   str=list[i];
   ret=str.indexOf(par);
   if (ret>=0) return i;
  }
 }

 if (reg == 3)
 {
  for (int i=kolstr-1; i>=0; i--)
  {
   str=list[i];
   ret=str.indexOf(par);
   if (ret>=0) return i;
  }
 }

 return -1;
}

QDateTime DirFile::SetDTFromStr(QString str)
{
 QDateTime dt;
 QDate dat;
 QTime t;
 int g,mm,d;
 int h,m,s,ms;
 double sd;
 QString str1;

 str=str.simplified();

 g=str.section(" ",0,0).toInt();
 if (g < 2000) g=g+2000;
 mm=str.section(" ",1,1).toInt();
 d=str.section(" ",2,2).toInt();
 h=str.section(" ",3,3).toInt();
 m=str.section(" ",4,4).toInt();
 str1=str.section(" ",5,5);
 s=str1.section(".",0,0).toInt();
 ms=str1.section(".",1,1).toInt();

 dat.setDate(g,mm,d);
 t.setHMS(h,m,s,ms);
 dt.setDate(dat);
 dt.setTime(t);

 return dt;
}

QStringList DirFile::CopyList(QStringList list,int ind,int col)
{
 QStringList ret;
 int colstr;
 qlonglong kolstr;

 kolstr=list.count();

 colstr=ind+col;
 if (col == 0) colstr=kolstr;

 if (kolstr < colstr) colstr=kolstr;

 for (int i=ind; i<colstr; i++)
 {
  ret << list[i];
 }

 return ret;
}

int DirFile::AddList(QStringList &list,QStringList list1)
{
 int kolzap=0;
 qlonglong kolstr;

 kolstr=list1.count();

 for (int i=0; i<kolstr; i++)
 {
  list << list1[i];
  kolzap++;
 }

 return kolzap;
}

int DirFile::AddList(QList<qlonglong> &list, QList<qlonglong> list1)
{
 int kolzap=0;
 qlonglong kolstr;

 kolstr=list1.count();

 for (int i=0; i<kolstr; i++)
 {
  list << list1[i];
  kolzap++;
 }

 return kolzap;
}

int DirFile::AddMap(QMap<QString,QString> &map,QMap<QString,QString> map1,bool pr_multi)
{
 int kolzap=0;

 QString key,value;
 QMap<QString,QString>::const_iterator i;

 for (i=map1.constBegin(); i!=map1.constEnd();++i)
 {
  key=i.key();
  value=i.value();
  if (!pr_multi) map.insert(key,value);
  else map.insertMulti(key,value);
  kolzap++;
 }

 return kolzap;
}

bool DirFile::DelFile(QString nfile)
{
    QFile f;

    f.setFileName(nfile);
    if (f.isOpen()) f.close();
    return f.remove();
}

bool DirFile::DelFile(QStringList spis_file)
{
 bool ret=true;
 qlonglong kolstr;
 QString str;

 kolstr=spis_file.count();
 if (kolstr == 0) return false;

 for (int i=0; i<kolstr; i++)
 {
  str=spis_file.at(i);
  if (! DelFile(str)) ret=false;
 }

 return ret;
}

void DirFile::DateToDay(QDate dat,quint32 &day,quint16 &year)
{
 qint32 st;

 year=dat.year();
 st=(year/100)*100;
 year=year-st;
 day=dat.dayOfYear();

 return;
}

quint32 DirFile::TimeToSec(QTime time)
{
 quint32 sec=0;
 QTime ntime;

 ntime.setHMS(0,0,0,0);

 sec=ntime.secsTo(time);

 return sec;
}

void DirFile::DelDublForList(QStringList &list)
{
 QStringList list1;
 qlonglong kol;
 QString str;
 int ind;

 kol=list.count();
 if (kol == 0) return;

 for (int i=0; i<kol; i++)
 {
  str=list.at(i);

  ind=FindStrInList(list1,str);
  if (ind == -1) list1 << str;
 }

 list.clear();
 AddList(list,list1);

 return;
}

bool DirFile::isVisYear(qint32 year)
{
 bool ret=false;

 if (year%4 == 0)
 {
  ret=true;
  if ((year%100 == 0) && (year%400 != 0)) ret=false;
 }

 return ret;
}

QBitArray DirFile::BytesToBits(QByteArray bytes)
{
 QBitArray bits(bytes.count()*8);

 for(int i=0; i<bytes.count(); ++i)
  for(int b=0; b<8; ++b)
   bits.setBit(i*8+b,bytes.at(i)&(1<<b));

 return bits;
}

unsigned short DirFile::Crc16(unsigned char * pcBlock, unsigned short len)
{
    unsigned short crc = 0xFFFF;

    while (len--)
        crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *pcBlock++];

    return crc;
}

/*!
 * \brief DirFile::Crc8
 * Расчёт КС CRC8
 * \param buf[in] область памяти с данными
 * \param ind[in] смещение в памяти с которого производится расчёт
 * \param len[in] длина области памяти для расчёта
 * \param crc[out] расчитанная КС
 * \return если КС не равна 255, возвращается false; иначе true
*/
bool DirFile::Crc8(QByteArray &buf,qint64 ind,qint64 len,uchar &crc)
{
 uchar ks=(uchar)(0);

 crc=0;

 for ( int i = 0; i < len; ++i )
     ks = ks + (uchar)buf.at(ind+i);

 crc=ks;

 if ( (int)ks != 255 ) return false;

 return true;
}

bool DirFile::DelDublInList(QStringList &list)
{
 qint32 kols;
 QString str,str1;

 if (list.isEmpty()) return false;
 qSort(list);
 kols=list.count();

 for (int i=0; i<kols-1; i++)
 {
  str=list.at(i);
  str1=list.at(i+1);
  if (str != str1) continue;
  list.removeAt(i+1);
  kols--;
  i--;
 }

 return true;
}

qint32 DirFile::AddArray(QByteArray &buf1,QByteArray &buf2,qint32 ind)
{
 qint32 dl1,dl2,dlo;
 QByteArray buf3;

 dl1=buf1.size();
 dl2=buf2.size();
 dlo=dl1+(dl2-ind);

 buf3.reserve(dlo);
 buf3.append(buf1);
 buf3.append(buf2.mid(ind));

 buf1.clear();
 buf1.reserve(dlo);
 buf1.append(buf3);

 return dlo;
}

/*!
 * \brief DirFile::ReadFile
 * читает файл в область памяти
 * \param file[in] название файла
 * \param buf[out] область памяти с содержимым файла
 * \return возвращает false при неудачном чтении файла; иначе - true
*/
bool DirFile::ReadFile(QString file,QByteArray &buf)
{
 QFile f;

 buf.clear();

 f.setFileName(file);
 if (! f.open(QIODevice::ReadOnly))
 {
  WriteLog("ReadFile : Ошибка открытия файла: "+file);
  return false;
 }
 buf=f.readAll();
 if (buf.isEmpty()) {f.close();return false;}
 f.close();

 return true;
}

bool DirFile::AppendFile(QString file, QByteArray &buf)
{
 QFile f;
 bool ret=true;

 f.setFileName(file);

 if (!f.exists()) return false;
 if (! f.open(QIODevice::Append))
 {
  WriteLog("AppendFile : Ошибка открытия файла: "+file);
  return false;
 }
 if (f.write(buf) == -1)
 {
  WriteLog("AppendFile : Ошибка записи в файл: "+file);
  ret=false;
 }
 f.close();

 return ret;
}

qint64 DirFile::WriteFile(QString file, QByteArray &buf, bool prud,QString &str_err)
{
 QFile f;
 qint64 ret;

 str_err="";

 f.setFileName(file);

 if (f.exists())
 {
  if (prud) f.remove();
  else
  {
   str_err="Файл "+file+" уже существует.";
   return -1;
  }
 }
 if (! f.open(QIODevice::WriteOnly))
 {
  str_err="WriteFile : Ошибка открытия файла "+file+" "+f.errorString();
  WriteLog(str_err);
  return -1;
 }
 ret=f.write(buf);
 if (ret == -1)
 {
  str_err="WriteFile : Ошибка записи файла "+file+" "+f.errorString();
  WriteLog(str_err);
  return -1;
 }

 f.close();

 return ret;
}

qreal DirFile::TimeToHD(qint32 h,qint32 m,qint32 s)
{
 qreal hd,dh;

 dh=((m*60.0)+s)/3600.0;

 hd=h+dh;

 return hd;
}

bool DirFile::AppendFileFromList(QStringList list,QString nfile)
{
 QFile file;
 QStringList nlist;
 bool ret=true;

 file.setFileName(nfile);
 nlist.clear();

 if (file.exists()) nlist=ReadFileInList(nfile);

 AddList(nlist,list);

 ret=WriteFileFromList(nlist,nfile);

 return ret;
}

bool DirFile::RenameFile(QString isxf,QString mask,QString &vixf)
{
 QString name,nname;
 QString mname,mext;
 QStringList fi;
 qint32 indm;
 qint32 j;

 indm=mask.lastIndexOf(".");
 if (indm == -1) return false;
 mname=mask.mid(0,indm);
 mext=mask.mid(indm+1);

 fi=FileInfo(isxf);
 name=fi[3];

 nname=NameMask(name,mname);
 if (nname.isEmpty()) return false;

 vixf=fi[1]+"/"+nname;

 name=fi[4];
 nname=NameMask(name,mext);
 if (nname.isEmpty()) return false;

 vixf=vixf+"."+nname;

 return true;
}

QString DirFile::NameMask(QString name,QString mask)
{
 QString nname;
 qint32 j,dlname;
 QString symb;
 qint32 nz,kol,n,k;

 nname.clear();
 j=0; dlname=name.count();
 nz=mask.indexOf("*");
 for (qint32 i=0; i<mask.count(); i++)
 {
  symb=mask.at(i);
  if (symb == "*")
  {
   if (i+1 >= mask.count()) {nname=nname+name;continue;}
   symb=mask.at(i+1);
   if (symb != "[") {nname=nname+name;continue;}
   kol=mask.indexOf("]",i+1);
   if (kol == -1) {nname=nname+name;continue;}
   symb=mask.mid(i+2,kol-(i+2));
   n=symb.section(",",0,0).toInt();
   k=symb.section(",",1,1).toInt();
   name.remove(0,n);
   name.resize(name.count()-k);
   nname=nname+name;
   i=kol;
   continue;
  }
  if (symb == "<")
  {
   if (nz != -1) continue;
   if (j == dlname) continue;
   j++;
   continue;
  }
  if (symb == "?")
  {
   if (nz != -1) continue;
   if (j == dlname) continue;
   nname=nname+name.mid(j,1);
   j++;
   continue;
  }
  if (symb == ">")
  {
   if (nz != -1) continue;
   if (j == dlname) continue;
   nname=nname+name.mid(j);
   j=dlname;
   continue;
  }
  nname=nname+symb;
 }

 return nname;
}

qint32 DirFile::List2ToList1NoEmpty(QStringList &list1,QStringList list2)
{
 qint32 ret=0;
 QString str,str1,szn;
 qint64 zn;

 list1.clear();

 for (qint32 i=0; i<list2.count(); i++)
 {
  str=list2.at(i);
  if (str.isEmpty()) continue;
  for(qint32 j=i+1; j<list2.count(); j++)
  {
   str1=list2.at(j);
   szn=str1.section(",",0,0);
   zn=szn.toLong();
   if (zn > 0) break;
   str=str+str1;
   i++;
  }

  list1 << str;
  ret++;
 }

 return ret;
}

bool DirFile::ReadFile(QString file,QByteArray &buf,qint64 kolb)
{
 QFile f;

 buf.clear();

 f.setFileName(file);
// if (! f.open(QIODevice::ReadWrite)) return false;
 if (! f.open(QIODevice::ReadOnly))
 {
  WriteLog("ReadFile : Ошибка открытия файла: "+file);
  return false;
 }
 buf=f.read(kolb);
 if (buf.isEmpty()) {f.close();return false;}
 f.close();

 return true;
}

void DirFile::DateToDay(QDate dat, qint32 &day, qint32 &year)
{
 quint32 nday;
 quint16 nyear;

 DateToDay(dat,nday,nyear);

 day=nday; year=nyear;

 if (year <= 99) year=year+2000;

 return;
}

void DirFile::Beep(int Hz,int msec)
{
 fprintf(stderr,"\033[10;%d]\033[11;%d]\a",Hz,msec);
 usleep(msec*1000);
 return;
}

QString DirFile::findString(QTextStream *_stream, QString _string){
    QString str = _stream->readLine();
    int it = 1;
    while(!str.contains(_string)){
        str = _stream->readLine();
        it++;
        if (_stream->atEnd()) return "";
    }
    if(str.contains(_string)){
        return str;
    }
    return "";
}

bool DirFile::VariantIsDigit(QVariant var)
{
 switch (var.userType())
 {
  case QMetaType::Int:
  case QMetaType::UInt:
  case QMetaType::Long:
  case QMetaType::LongLong:
  case QMetaType::Short:
  case QMetaType::UShort:
  case QMetaType::ULong:
  case QMetaType::ULongLong:
  case QMetaType::Float:
  case QMetaType::Double:
  return true;
 }
 return false;
}

QStringList DirFile::MapToList(QMap<QString, QString> &map)
{
 QStringList list;
 QMap<QString,QString>::const_iterator i;

 if (map.isEmpty()) return list;

 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
  list << i.key();
  list << i.value();
 }

 return list;
}

bool DirFile::DelDublInList(QList<QString> &list)
{
 qint32 kols;
 QString str,str1;

 if (list.isEmpty()) return false;
 qSort(list);
 kols=list.count();

 for (int i=0; i<kols-1; i++)
 {
  str=list.at(i);
  str1=list.at(i+1);
  if (str != str1) continue;
  list.removeAt(i+1);
  kols--;
  i--;
 }

 return true;
}

void DirFile::WriteLog(QString text)
{
 QString path_log;
 QFile f_log;

 path_log=QApplication::applicationDirPath()+"/sbor.log";
 f_log.setFileName(path_log);

 if (!f_log.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)) return;

 QTextStream stream(&f_log);

 stream << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") << " :" <<text+"\n";

 f_log.close();

 return;
}

bool DirFile::CopyErr(QString path_si,QString nfile)
{
 QDir dir;
 QByteArray buf;

 dir.setPath(path_si+"/ERR");
 if (!dir.exists())
     if (!dir.mkdir(path_si+"/ERR")) return false;

 DelFileM(path_si+"/ERR/"+nfile);
 if (!ReadFile(path_si+"/"+nfile,buf)) return false;
 if (WriteFile(path_si+"/ERR/"+nfile,buf,true) < 0) return false;
 DelFileM(path_si+"/"+nfile);

// if (!QFile::copy(path_si+"/"+nfile,path_si+"/ERR/"+nfile))
// {
//  WriteLog("Ошибка копирования файла: "+path_si+"/"+nfile+" в файл: "+path_si+"/ERR/"+nfile);
//  return false;
// }
// else DelFile(path_si+"/"+nfile);

 return true;
}

bool DirFile::DelFileM(QString nfile)
{
 for (qint32 i=0; i<10; i++)
 {
  if (DelFile(nfile)) return true;
 }

 return false;
}

