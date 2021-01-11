#include "rinexfile.h"

RinexFile::RinexFile()
{
 list_m.clear();
 list_m << "JAN";
 list_m << "FEB";
 list_m << "MAR";
 list_m << "APR";
 list_m << "MAY";
 list_m << "JUN";
 list_m << "JUL";
 list_m << "AUG";
 list_m << "SEP";
 list_m << "OCT";
 list_m << "NOV";
 list_m << "DEC";
}

QString RinexFile::RnxVersion(QStringList list)
{
 QString ret="";
 int ind;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"RINEX VERSION");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(0,9).trimmed();

 return ret;
}

QString RinexFile::RnxVersion(QString nfile)
{
 QString ret="";
 int ind;
 QStringList list;

 list=ReadRnxHdr(nfile);
 if (list.count() == 0) return ret;

 ind=FindStrInList(list,"RINEX VERSION");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(0,9).trimmed();

 return ret;
}

QString RinexFile::RnxTip(QStringList list)
{
 QString ret="";
 int ind;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"RINEX VERSION");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(20,1);

 return ret;
}

QString RinexFile::RnxSatSys(QStringList list)
{
 QString ret="";
 int ind;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"RINEX VERSION");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(40,1);

 return ret;
}

QString RinexFile::RnxFirstObs(QStringList list)
{
 QString ret="";
 int ind;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"TIME OF FIRST OBS");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(0,43);

 return ret;
}

QString RinexFile::RnxLastObs(QStringList list)
{
 QString ret="";
 int ind;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"TIME OF LAST OBS");

 if (ind < 0) return ret;

 ret=list[ind];
 ret=ret.mid(0,43);

 return ret;
}

QStringList RinexFile::SpisRnxDataDesc(QStringList list)
{
 QStringList desc;
 QString str;
 qlonglong kolstr;

 kolstr=list.count();
 if (kolstr == 0) return desc;

 for (int i=0; i<kolstr; i++)
 {
  str=list[i];
  if (str.mid(0,1) != ">") continue;
  desc.append(str);
 }

 return desc;
}

QStringList RinexFile::SpisRnxDataDescInd(QStringList list)
{
 QStringList desc;
 QString str;
 qlonglong kolstr;

 kolstr=list.count();
 if (kolstr == 0) return desc;

 for (int i=0; i<kolstr; i++)
 {
  str=list[i];
  if (str.mid(0,1) != ">") continue;
  str=str+";"+QString::number(i);
  desc.append(str);
 }

 return desc;
}

QString RinexFile::DtToStrHdrRnx(QDateTime dt, short pr)
{
 QString sdt="";
 QDate d;
 QTime t;
 QString sg,sm,sd;
 QString sh,smm,ss,sms;
 QString str;

 d=dt.date();
 t=dt.time();

 str.setNum(d.year());
 if (pr == 1) sg=QString("%1").arg(str,6,' ');
 if (pr == 2) sg=QString("%1").arg(str,4,' ');

 str.setNum(d.month());
 if (pr == 1) sm=QString("%1").arg(str,6,' ');
 if (pr == 2) sm=" "+QString("%1").arg(str,2,'0');

 str.setNum(d.day());
 if (pr == 1) sd=QString("%1").arg(str,6,' ');
 if (pr == 2) sd=" "+QString("%1").arg(str,2,'0');

 str.setNum(t.hour());
 if (pr == 1) sh=QString("%1").arg(str,6,' ');
 if (pr == 2) sh=" "+QString("%1").arg(str,2,'0');

 str.setNum(t.minute());
 if (pr == 1) smm=QString("%1").arg(str,6,' ');
 if (pr == 2) smm=" "+QString("%1").arg(str,2,'0');

 str.setNum(t.second());
 if (pr == 1) ss=QString("%1").arg(str,5,' ');
 if (pr == 2) ss=" "+QString("%1").arg(str,2,'0');

 str.setNum(t.msec());
 sms="."+QString("%1").arg(str,7,'0');

 sdt=sg+sm+sd+sh+smm+ss+sms;

 return sdt;
}

bool RinexFile::UpdateRnxDTObs(QStringList &list,QDateTime dt,short pr)
{
 QString str,stri,strk;
 int ind;

 if (list.isEmpty()) return false;

 if (pr == 1) ind=FindStrInList(list,"TIME OF FIRST OBS");
 if (pr == 2) ind=FindStrInList(list,"TIME OF LAST OBS");
 stri=list[ind];

 str=DtToStrHdrRnx(dt,1);
 strk=stri;
 strk.replace(0,43,str);

 list.replace(ind,strk);

 return true;
}

QString RinexFile::RnxFirstObsF(QStringList list_dd)
{
 QString str="";

 if (list_dd.count() == 0) return str;

 str=list_dd[0];
 str=str.mid(1,28);

 return str;
}

QString RinexFile::RnxLastObsF(QStringList list_dd)
{
 QString str="";

 if (list_dd.count() == 0) return str;

 str=list_dd[list_dd.count()-1];
 str=str.mid(1,28);

 return str;
}

bool RinexFile::ProvRnxVer(QStringList list)
{
 QString str;

 if (list.isEmpty()) return false;

 str=RnxVersion(list);
 if ((str != "3.02") || (str != "3.00")) return false;

 str=RnxTip(list);
 if (str != "O") return false;

 return true;
}

int RinexFile::ProvRnxDTObs(QStringList list,QStringList list_dd)
{
 int ret=0;
 QString str;
 QDateTime dtf,dtf1,dtl,dtl1;

 if (list.isEmpty()) return ret;
 if (list_dd.isEmpty()) return ret;

 str=RnxFirstObs(list);
 dtf=SetDTFromStr(str);

 str=RnxLastObs(list);
 dtl=SetDTFromStr(str);

 str=RnxFirstObsF(list_dd);
 dtf1=SetDTFromStr(str);

 str=RnxLastObsF(list_dd);
 dtl1=SetDTFromStr(str);

 if (dtf != dtf1) ret=ret+1;
 if (dtl != dtl1) ret=ret+10;

 return ret;
}

QStringList RinexFile::ReadRnxHdr(QStringList list)
{
 QStringList list_hdr;
 int ind;

 if (list.isEmpty()) return list_hdr;

 ind=FindStrInList(list,"END OF HEADER");
 if (ind == -1) return list_hdr;

 list_hdr=CopyList(list,0,ind+1);

 return list_hdr;
}

QStringList RinexFile::ReadRnxHdr(QString nfile)
{
 QStringList list_hdr;

 list_hdr=ReadFileInList(nfile,"END OF HEADER");

 return list_hdr;
}

bool RinexFile::IsprRnxDTObs(QString nfile)
{
 QStringList list,list_d;
 int ret;
 QString str;
 QDateTime dt;

 list=ReadFileInList(nfile);
 list_d=SpisRnxDataDesc(list);

 if (!IsprRnxDTObs(list,list_d)) return false;

 if (!WriteFileFromList(list,nfile)) return false;

 return true;
}

bool RinexFile::IsprRnxDTObs(QStringList &list,QStringList list_dd)
{
 int ret;
 QString str;
 QDateTime dt;

 if (list.isEmpty()) return false;
 if (list_dd.isEmpty()) return false;

 ret=ProvRnxDTObs(list,list_dd);
 if (ret == -1) return false;
 if (ret == 0) return true;

 if ((ret == 1) || (ret == 11))
 {
  str=RnxFirstObsF(list_dd);
  dt=SetDTFromStr(str);
  if (!UpdateRnxDTObs(list,dt,1)) return false;
 }

 if ((ret == 10) || (ret == 11))
 {
  str=RnxLastObsF(list_dd);
  dt=SetDTFromStr(str);
  if (!UpdateRnxDTObs(list,dt,2)) return false;
 }

 return true;
}

int RinexFile::KolSutInRnx(QStringList list)
{
 int sut=0;
 QString str;
 QDate dn,dk;
 QDateTime dt;

 if (list.isEmpty()) return sut;

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 dn=dt.date();

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 dk=dt.date();

 sut=(dn.daysTo(dk))+1;

 return sut;
}

QStringList RinexFile::ReadSecDatRnx(QStringList list, QString hdrdat, int &ind)
{
 QStringList sec;
 int ind1=0;
 int ind2=0;
 int kolizm;
 QString err;

 if (list.isEmpty()) return sec;

 ind1=FindStrInList(list,hdrdat,ind,2);
 if (ind1 == -1)
 {
  ind2=FindStrInList(list,hdrdat);
  if (ind2 == -1) return sec;
  ind1=ind2;
 }

 kolizm=KolIzmInDat(hdrdat,err);

 sec=CopyList(list,ind1,kolizm+1);

 ind=ind1+kolizm+1;

 return sec;
}

int RinexFile::KolIzmInDat(QString hdrdat,QString &err)
{
 int kol=0;
 QString str;

 err="";
 str=hdrdat.simplified();

 if (str.section(" ",0,0) != ">") return -1;

 str=str.section(" ",8,8).trimmed();
 if (str == "")
 {
  err="В эпохе : "+hdrdat+" нет количества измерений cпутников.";
  return -1;
 }

 kol=str.toInt();

 if (kol > 56)
 {
  err="В эпохе : "+hdrdat+" недопустимое количество спутников.";
  return -1;
 }

 return kol;
}

QStringList RinexFile::ProvStrDatRnx(QStringList list,QString nfile)
{
 QStringList err;
 int tekind,tekind1,tekind2,inderr;
 QList<qint32> linderr;
 QString strerr;
 QString strhdr;
 int kolizm;
 qlonglong kolstr;
 QString str;
 QProgressDialog prd;

 kolstr=list.count();
 if (kolstr == 0)
 {
  strerr=QObject::tr("Файл пустой.");
  err << strerr;
  return err;
 }

 tekind=FindStrInList(list,"END OF HEADER");
 if (tekind == -1)
 {
  strerr=QObject::tr("Не найден конец заголовка Rinex-файла.");
  err << strerr;
  return err;
 }
 tekind++;

 if (tekind == kolstr)
 {
  strerr=QObject::tr("Измерений не найдено.");
  err << strerr;
  return err;
 }

 tekind1=tekind;

 if (nfile != "")
 {
  str="Проверка структуры файла : "+nfile;
  prd.setModal(true);
  prd.setCancelButton(0);
  prd.setLabelText(str);
  prd.setMinimum(tekind);
  prd.setMaximum(kolstr);
  prd.show();
 }

 while (tekind < kolstr)
 {
  strhdr=list.at(tekind);
  kolizm=KolIzmInDat(strhdr,strerr);

  if (kolizm == -1)
  {
   strerr="Строка: "+QString::number(tekind1)+". Нарушена структура Rinex-файла.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << list[tekind1-1];
   tekind=FindStrInList(list,">",tekind,2);
   if (tekind == -1)
   {
    if (nfile != "")
    {
     prd.close();
     return err;
    }
   }
   if (nfile != "")
   {
    prd.setValue(tekind);
    qApp->processEvents();
   }
   continue;
  }

  if (kolizm == 0)
  {
   strerr="Строка: "+QString::number(tekind+1)+". Нулевое количество измерений.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << list[tekind];
   tekind2=FindStrInList(list,">",tekind,2);
   if ((tekind2 == -1) || (tekind == tekind2))
   {
    if (nfile != "")
    {
     prd.close();
     return err;
    }
   }
   tekind++;
   if (nfile != "")
   {
    prd.setValue(tekind);
    qApp->processEvents();
   }
   continue;
  }

  if (!ProvRnxNSat(list,tekind,kolizm,linderr))
  {
   for (int j=0; j<linderr.count(); j++)
   {
    inderr=linderr[j];
    strerr="Строка: "+QString::number(inderr)+". Неверный номер спутника ГНСС.";
    err << strerr;
    err << list[inderr];
   }
  }

  tekind1=tekind+1;
  tekind=tekind+kolizm+1;
  if (nfile != "") prd.setValue(tekind);
  qApp->processEvents();
 }
 if (nfile != "") prd.close();

 return err;
}

QStringList RinexFile::ProvStrDatRnxN(QStringList list,QString nkg)
{
 QStringList err;
 QString strerr;
 qlonglong kolstr;
 QString str;
 int ch;
 qint32 tekind,ind;
 QDate df,di;
 QDateTime dt;

 kolstr=list.count();
 if (kolstr == 0)
 {
  strerr=QObject::tr("Файл пустой.");
  err << strerr;
  return err;
 }

 tekind=FindStrInList(list,"END OF HEADER");
 if (tekind == -1)
 {
  strerr=QObject::tr("Не найден конец заголовка Rinex-файла.");
  err << strerr;
  return err;
 }
 tekind++;

 if (tekind == kolstr)
 {
  strerr=QObject::tr("Измерений не найдено.");
  err << strerr;
  return err;
 }

 ind=FindStrInList(list,"DATE");
 if (ind != -1)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(" ",3,3);
  df=QDate::fromString(str,"yyyyMMdd");
 }


 if (nkg == "G") ch=4;
 if (nkg == "N") ch=8;

 for (int i=tekind; i<kolstr; i=i+ch)
 {
  str=list.at(i);
  if ((str.mid(0,1) != "R") && (str.mid(0,1) != "G"))
  {
   strerr="Строка: "+QString::number(i)+". Ошибка структуры.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << str;
   return err;
  }
  str=ReadDTFromN(str);
  dt=SetDTFromStr(str);
  di=dt.date();
  if ((di > df) || (!di.isValid()))
  {
   strerr="Строка: "+QString::number(i)+". Ошибка в дате эпохи.";
   err << strerr;
   err << str;
  }

  qApp->processEvents();
 }

 return err;
}

bool RinexFile::ProvRnxFile(QString nfile,bool pr_errfile,bool pr_progres)
{
 QStringList list;
 QStringList err;
 QStringList file;
 QString nfile_err;

 list=ReadFileInList(nfile);

 if (pr_progres) err=ProvStrDatRnx(list,nfile);
 else err=ProvStrDatRnx(list);
 if (err.count() == 0) return true;

 if (pr_errfile)
 {
  file=FileInfo(nfile);
  nfile_err=file.at(0)+".err";
  WriteFileFromList(err,nfile_err);
 }

 return false;
}

bool RinexFile::ProvRnxFileN(QString nfile,bool pr_errfile)
{
 QStringList list;
 QStringList err;
 QStringList file;
 QString nfile_err;
 QString nka;

 list=ReadFileInList(nfile);
 file=FileInfo(nfile);
 nka=file[4];
 nka=nka.right(1);

 err=ProvStrDatRnxN(list,nka);
 if (err.count() == 0) return true;

 if (pr_errfile)
 {
  nfile_err=file.at(0)+".err";
  WriteFileFromList(err,nfile_err);
 }

 return false;
}

QString RinexFile::NazOFile(QString pn,QStringList list,QString path)
{
 QString naz_file="";
 QString str;
 QString pref="123456789abcdefghijklmnopqastuvwxyz";//34
 QDateTime dt;
 int sut,god;
 QString ssut="000";
 QString sgod="00";
 QFile f;

 if (list.isEmpty()) return naz_file;

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);

 sut=dt.date().dayOfYear();
 str.setNum(sut);
 ssut.replace(3-str.length(),str.length(),str);

 god=dt.date().year();
 str.setNum(god);
 str=str.mid(2,2);
 sgod.replace(2-str.length(),str.length(),str);

 naz_file=pn+ssut+"0."+sgod+"O";
 naz_file=naz_file.toUpper();

 for (int i=0; i<34; i++)
 {
  f.setFileName(path+"/"+naz_file);
  if (!f.exists()) break;
  naz_file.replace(7,1,pref.mid(i,1));
 }

 return naz_file;
}

QString RinexFile::NazOFile(QString nfile)
{
 QString naz_file="";
 QString pref="123456789abcdefghijklmnopqastuvwxyz";//34
 QFile f;
 QStringList fi;
 QString path;

 fi=FileInfo(nfile);
 naz_file=fi[3];
 path=fi[1];
 if (naz_file.length() > 8) naz_file.resize(8);
 naz_file.replace(7,1,"0");
 naz_file=naz_file+"."+fi[4];
 naz_file=naz_file.toUpper();

 for (int i=0; i<34; i++)
 {
  f.setFileName(path+"/"+naz_file);
  if (!f.exists()) break;
  naz_file.replace(7,1,pref.mid(i,1));
 }

 return naz_file;
}

QStringList RinexFile::CreateOFile(QStringList list,QDate dt)
{
 QStringList ret;
 int nind;
 QString str;
 QDateTime dt_dat;
 bool przap;
 int kolsut;

 if (list.isEmpty()) return ret;

 kolsut=KolSutInRnx(list);
 if (kolsut == 1) return ret;

 nind=FindStrInList(list,">");
 dt_dat=DTForHdrDatRnx(list.at(nind));
 if (dt < dt_dat.date()) return ret;

 nind=FindStrInList(list,">",0,3);
 dt_dat=DTForHdrDatRnx(list.at(nind));
 if (dt > dt_dat.date()) return ret;


 ret=ReadRnxHdr(list);
 if (ret.count() == 0) return ret;

 nind=FindStrInList(list,">");
 for(int i=nind; i<list.count(); i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">")
  {
   dt_dat=DTForHdrDatRnx(list.at(i));
   if (dt_dat.date() != dt) przap=false;
   else przap=true;
  }
  if (przap)
  {
   ret << str;
  }
 }

 nind=FindStrInList(ret,">");
 if (nind == -1)
 {
  ret.clear();
  return ret;
 }
 dt_dat=DTForHdrDatRnx(ret.at(nind));
 UpdateRnxDTObs(ret,dt_dat,1);

 nind=FindStrInList(ret,">",0,3);
 dt_dat=DTForHdrDatRnx(ret.at(nind));
 UpdateRnxDTObs(ret,dt_dat,2);

 return ret;
}

QDateTime RinexFile::DTForHdrDatRnx(QString hdrdat)
{
 QDateTime dt;
 QString str;

 dt.date().addDays(0);
 dt.date().addMonths(0);
 dt.date().addYears(0);

 if (hdrdat.mid(0,1) != ">") return dt;

 str=hdrdat.mid(1,28);
 dt=SetDTFromStr(str);

 return dt;
}

bool RinexFile::RazbAllOFile(QString nfile,QStringList &sp_file)
{
 QStringList list;
 QStringList err;
 int kolsut;
 QString str;
 QDateTime dt;
 QDate dat;
 QStringList new_list;
 QStringList ifile;
 QString npn;

 list=ReadFileInList(nfile);
 if (list.count() == 0) return false;

 err=ProvStrDatRnx(list);
 if (err.count() > 0) return false;

 kolsut=KolSutInRnx(list);
 if (kolsut == 1) return false;

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);

 ifile=FileInfo(nfile);
 str=ifile.at(3);
 npn=str.mid(0,4);

 sp_file.clear();

 DelFile(nfile);

 for(int i=0; i<kolsut; i++)
 {
  dat=dt.date();
  dat=dat.addDays(i);

  new_list=CreateOFile(list,dat);
  if (new_list.count() == 0) continue;
  UpdatePrnOfObs(new_list);

  str=NazOFile(npn,new_list,ifile.at(1));

  if (! WriteFileFromList(new_list,ifile[1]+"/"+str)) return false;

  sp_file << ifile[1]+"/"+str;
 }


 return true;
}

QMap<QString,QString> RinexFile::ReadOFileInMap(QString nfile)
{
 QMap<QString,QString> map;
 QStringList list;

 list=ReadFileInList(nfile);
 map=ReadOFileInMap(list);

 return map;
}

QMap<QString,QString> RinexFile::ReadOFileInMap(QStringList list)
{
 QMap<QString,QString> map;
 QString str;
 QString key1,key;
 qlonglong kolstr;
 int ind;

 kolstr=list.count();
 if (kolstr == 0) return map;

 ind=FindStrInList(list,">");

 for (int i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">") //формируем первую часть ключа
  {
   str=str.replace(29,6,"......").trimmed();
   key1=str;
   continue;
  }

  if ((str.mid(0,1) == "R") || (str.mid(0,1) == "G")) //формируем вторую часть ключа
  {
   key=key1+" "+str.mid(0,3);
   map.insert(key,str);
  }
 }

 return map;
}

/*!
 * \brief RinexFile::FormOFileFromMap
 * формирование списка измерений в формате RINEX 3.02 из ключевого списка
 * количество спутников в заголовках измерений (эпох) корректируется
 * \param map[in] ключевой список с измерениями (ключ - заголовок эпохи)
 * \return возвращает список измерений или пустой список при неудачном завершении
*/
QStringList RinexFile::FormOFileFromMap(QMap<QString, QString> map)
{
 QStringList list;
 QString key1,key2;
 QMap<QString,QString>::const_iterator i;

 if (map.isEmpty()) return list;

 key1="";
 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
  key2=i.key();
  key2.resize(key2.length()-3);
  if (key1 == key2)
  {
   list << i.value();
   continue;
  }

  key1=key2;
  list << key1;
  list << i.value();
 }

 KorrKolSputn(list); ///корректировка количества спутников в заголовках измерений

 return list;
}

/*!
 * \brief RinexFile::KorrKolSputn
 * корректировка количества спутников в заголовках измерений
 * одновременно заполняются массивы mas_gps и mas_glo
 * \param list[in,out] список измерений RINEX 3.02
*/
void RinexFile::KorrKolSputn(QStringList &list)
{
 QString str,str1,sch;
 int ind= -1;
 int ch;
 qlonglong kolstr;
 int nizm;

 kolstr=list.count();
 if (kolstr == 0) return;

 ClearMasSputn();

 nizm=FindStrInList(list,">");

 if (nizm == -1) return;

 for (int i=nizm; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">")
  {
   if (ind >= 0)
   {
    str=list.at(ind);
    if (ch > 99) ch=0;
    sch.setNum(ch);
    str1=str.mid(0,29)+"  0"+QString("%1").arg(sch,3,' ');
    if (str.length() > 35) str1=str1+str.mid(35);
    list.replace(ind,str1);
   }
   ind=i; ch=0;
   continue;
  }
  ch++;
  ZapSputnInMas(str);
  continue;
 }

 if (ind >= 0)
 {
  str=list.at(ind);
  if (ch > 99) ch=0;
  sch.setNum(ch);
  str1=str.mid(0,29)+"  0"+QString("%1").arg(sch,3,' ');
  if (str.length() > 35) str1=str1+str.mid(35);
  list.replace(ind,str1);
 }

 return;
}

QStringList RinexFile::FormHdrDDS(QStringList spis_f,QStringList list,QString nka)
{
 QStringList hdr;
 QString str;
 QStringList list_obs;
 QStringList list_dd;

 if (list.isEmpty()) return hdr;

 KorrKolSputn(list);

 str=QString("%1").arg(3.00,9,'f',2).leftJustified(20,' ')+
     QString("OBSERVATION DATA    M").leftJustified(40,' ')+"RINEX VERSION / TYPE";
 hdr << str;

 str=QString("ConvertCOGI").leftJustified(20,' ')+QString("COGI").leftJustified(20,' ')+
      QDate::currentDate().toString("yyyyMMdd")+
      QTime::currentTime().toString(" hhmmss UTC").leftJustified(12,' ')+"PGM / RUN BY / DATE ";
 hdr << str;

 // имя исходного файла
 for (int i=0; i<spis_f.count(); i++)
 {
  str= QString(QFileInfo(spis_f[i]).fileName()).leftJustified(60,' ')+QString("COMMENT").leftJustified(20,' ');
  hdr << str;
 }

 str=QString("G=GPS R=GLONASS").leftJustified(60,' ')+QString("COMMENT").leftJustified(20,' ');
 hdr << str;

 str=QString("Geo-ik-2").leftJustified(60,' ')+QString("COMMENT").leftJustified(20,' ');
 hdr << str;

 str=(nka+QString("G")).leftJustified(60,' ')+"MARKER NAME         ";
 hdr << str;

 str=QString(" ").leftJustified(60,' ')+"MARKER NUMBER       ";
 hdr << str;

 str=QString("1").leftJustified(20,' ')+QString("A").leftJustified(20,' ')+str.fill(' ',20)+"ANT # / TYPE        ";
 hdr << str;

 str=(QString("%1").arg(QString("0").toFloat(),14,'f',4)
         +QString("%1").arg(QString("0").toFloat(),14,'f',4)
         +QString("%1").arg(QString("0").toFloat(),14,'f',4)).leftJustified(60,' ')+"ANTENNA: DELTA H/E/N";
 hdr << str;

 str=CreateObsTypesDDS(1);
 if (str != "") hdr << str;

 str=CreateObsTypesDDS(2);
 if (str != "") hdr << str;

 str=QString("%1").arg(QString("10").toDouble(),10,'f',3)+str.fill(' ',50)+ "INTERVAL            ";
 hdr << str;

 str=QString(" ").leftJustified(48,' ')+QString("GPS").leftJustified(12,' ')+"TIME OF FIRST OBS   ";
 hdr << str;

 str=QString(" ").leftJustified(60,' ')+"TIME OF LAST OBS    ";
 hdr << str;

 str=CreateOfSatellites();
 if (str != "") hdr << str;

 list_obs=CreatePrnOfObsDDS();
 AddList(hdr,list_obs);

 str=str.fill(' ',60)+"END OF HEADER       ";
 hdr << str;

 list_dd=SpisRnxDataDesc(list);
 IsprRnxDTObs(hdr,list_dd);

 return hdr;
}

QString RinexFile::CreateObsTypesDDS(int kod)
{
 QString ot="";
 QString ot1;
 int fl_gps[3];
 int fl_glo[6];
 int ch=0;

 if (kod == 1)
 {
  fl_gps[0]=0; fl_gps[1]=0; fl_gps[2]=0;
  for (int i=0; i<32; i++)
  {
   if (mas_gps[i][1] > 0) fl_gps[0]=1;
   if (mas_gps[i][2] > 0) fl_gps[1]=1;
   if (mas_gps[i][3] > 0) fl_gps[2]=1;
  }
  ch=fl_gps[0]+fl_gps[1]+fl_gps[2];
  if (ch == 3)
  {
   ot="G"+QString("    3")+" C1C L1C D1C ";
   ot=ot+ot1.fill(' ',60-ot.size())+"SYS / # / OBS TYPES ";
  }
 }

 if (kod == 2)
 {
  fl_glo[0]=0; fl_glo[1]=0; fl_glo[2]=0; fl_glo[3]=0; fl_glo[4]=0; fl_glo[5]=0;
  for (int i=0; i<24; i++)
  {
   if (mas_glo[i][1] > 0) fl_glo[0]=1;
   if (mas_glo[i][2] > 0) fl_glo[1]=1;
   if (mas_glo[i][3] > 0) fl_glo[2]=1;
   if (mas_glo[i][4] > 0) fl_glo[3]=1;
   if (mas_glo[i][5] > 0) fl_glo[4]=1;
   if (mas_glo[i][6] > 0) fl_glo[5]=1;
  }
  ch=fl_glo[0]+fl_glo[1]+fl_glo[2]+fl_glo[3]+fl_glo[4]+fl_glo[5];
  if (ch == 3)
  {
   ot="R"+QString("    3")+" C1P L1P D1P ";
   ot=ot+ot1.fill(' ',60-ot.size())+"SYS / # / OBS TYPES ";
  }
  if (ch == 6)
  {
   ot="R"+QString("    6")+" C1P L1P D1P C2P L2P D2P";
   ot=ot+ot1.fill(' ',60-ot.size())+"SYS / # / OBS TYPES ";
  }
 }

 return ot;
}

QString RinexFile::CreateOfSatellites()
{
 QString str="";
 int ch=0;

 for (int i=0; i<32; i++)
 {
  if (mas_gps[i][0] > 0) ch++;
 }

 for (int i=0; i<24; i++)
 {
  if (mas_glo[i][0] > 0) ch++;
 }

 str=QString::number(ch).rightJustified(6,' ')+ str.fill(' ',54)+"# OF SATELLITES     ";

 return str;
}

QStringList RinexFile::CreatePrnOfObsDDS()
{
 QStringList list;
 QString str,str1;
 int kolizm;

 for (int i=0; i<32; i++)
 {
  if (mas_gps[i][0] == 0) continue;

  str="   G"+QString::number(i+1).rightJustified(2,'0');
  for ( int j=0; j<3; ++j )
  {
   str=str+str1.setNum(mas_gps[i][j+1]).rightJustified(6,' ');
  }
  str=str+str1.fill(' ',60-str.length())+"PRN / # OF OBS      ";
  list << str;
 }

 str=CreateObsTypesDDS(2);
 if (str == "") return list;
 kolizm=str.mid(5,1).toInt();
 for (int i=0; i<24; i++)
 {
  if (mas_glo[i][0] == 0) continue;

  str="   R"+QString::number(i+1).rightJustified(2,'0');
  for ( int j=0; j<kolizm; ++j )
  {
   str=str+str1.setNum(mas_glo[i][j+1]).rightJustified(6,' ');
  }
  str=str+str1.fill(' ',60-str.length())+"PRN / # OF OBS      ";
  list << str;
 }

 return list;
}

QStringList RinexFile::RazbDDSOFile(QString nfile,QStringList spis_file,QString nka)
{
 QStringList list;
 QStringList err;
 QStringList hdr;
 int kolsut;
 QString str;
 QDateTime dt;
 QDate dat;
 QStringList new_list;
 QStringList ifile;
 QString npn;
 QStringList ret;
 QFile f;

 list=ReadFileInList(nfile);
 if (list.count() == 0) return ret;

 err=ProvStrDatRnx(list,nfile);
 if (err.count() > 0) return ret;

 kolsut=KolSutInRnx(list);
 if (kolsut == 1)
 {
  ret << nfile;
  return ret;
 }

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);

 ifile=FileInfo(nfile);
 str=ifile.at(3);
 npn=str.mid(0,4);

 f.setFileName(nfile);
 f.remove();

 for(int i=0; i<kolsut; i++)
 {
  dat=dt.date();
  dat=dat.addDays(i);

  new_list=CreateListForDate(list,dat);
  if (new_list.count() == 0) continue;

  hdr=FormHdrDDS(spis_file,new_list,nka);
  AddList(hdr,new_list);

  str=NazOFile(npn,hdr,ifile.at(1));

  WriteFileFromList(hdr,ifile[1]+"/"+str);
  ret << ifile[1]+"/"+str;
 }

 return ret;
}

QStringList RinexFile::CreateListForDate(QStringList list,QDate dt)
{
 QStringList ret;
 int nind;
 QString str;
 QDateTime dt_dat;
 bool przap;

 if (list.isEmpty()) return ret;

 nind=FindStrInList(list,">");
 dt_dat=DTForHdrDatRnx(list.at(nind));
 if (dt < dt_dat.date()) return ret;

 nind=FindStrInList(list,">",0,3);
 dt_dat=DTForHdrDatRnx(list.at(nind));
 if (dt > dt_dat.date()) return ret;

 nind=FindStrInList(list,">");
 for(int i=nind; i<list.count(); i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">")
  {
   dt_dat=DTForHdrDatRnx(list.at(i));
   if (dt_dat.date() != dt) przap=false;
   else przap=true;
  }
  if (przap)
  {
   ret << str;
  }
 }

 return ret;
}

bool RinexFile::CopyOFileInBII(QString nfile,QString path_bii,bool pr_zam,bool pr_ud)
{
 QString vfile;
 QStringList fi;
 QString vpath;
 QFile fisx, fvix;
 bool ret;
 QDir dir;

 fi=FileInfo(nfile);

 vpath=PathInBIIOFile(nfile,path_bii);
 dir.setPath(vpath);
 if (!dir.exists()) dir.mkpath(vpath);

 if (!pr_zam) vfile=NazOFile(vpath+"/"+fi[2]);
 else vfile=fi[2];

 vfile=vpath+"/"+vfile;

 fisx.setFileName(nfile);
 fvix.setFileName(vfile);

 if (pr_zam)
 {
  if (fvix.exists()) fvix.remove();
 }

 ret=QFile::copy(nfile,vfile);

 if (pr_ud) fisx.remove();

 return ret;
}

bool RinexFile::CopyOFileInBII(QStringList lfile,QString path_bii,bool pr_zam,bool pr_ud)
{
 bool ret;
 qlonglong kol_str;

 kol_str=lfile.count();
 if (kol_str == 0) return false;

 for (int i=0; i<kol_str; i++)
 {
  ret=CopyOFileInBII(lfile[i],path_bii,pr_zam,pr_ud);
  if (! ret) return ret;
 }
 return true;
}

QString RinexFile::PathInBIIOFile(QString nfile,QString path_bii)
{
 QString ret="";
 QStringList fi;
 QString god, sut;

 fi=FileInfo(nfile);

 god="20"+fi[4].mid(0,2);
 sut=fi[3].mid(4,3);

 ret=path_bii+"/"+god+"/"+sut;

 return ret;
}

QStringList RinexFile::ObOFile(QStringList sp_file)
{
 qlonglong kolstr;
 QMap<QString,QString> map;
 QMap<QString,QString> map1;
 QStringList ret;

 kolstr=sp_file.count();
 if (kolstr == 0) return ret;

 for (int i=0; i<kolstr; i++)
 {
  map1=ReadOFileInMap(sp_file[i]);
  AddMap(map,map1,false);
 }

 ret=FormOFileFromMap(map);
 return ret;
}

bool RinexFile::ReadHdrDDS(QStringList list,QString &nka,QStringList &spis_f,int &zn_sec)
{
 QStringList hdr;
 qlonglong kolstr;
 QString str,str1;
 int ind;
 QDateTime dt;
 QTime tm;
 int sec,sec1;

 if (list.isEmpty()) return false;

 if (! ReadHdrDDS(list)) return false;

 nka="";
 spis_f.clear();
 zn_sec=-1;

 hdr=ReadRnxHdr(list);
 kolstr=hdr.count();
 if (kolstr == 0) return false;

 str=hdr.at(0);
 str=str.simplified();
 str1=str.section(' ',0,0);
 if (str1 != "3.00") return false;
 str1=str.section(' ',1,1);
 if (str1 != "OBSERVATION") return false;

 str=hdr.at(1);
 str=str.simplified();
 str1=str.section(' ',0,0);
 if (str1 != "ConvertCOGI") return false;

 for (int i=2; i<kolstr; i++)
 {
  str=hdr.at(i);
  str=str.simplified();
  str1=str.section(' ',0,0);
  str=str1.right(3);
  if ((str != "bin") && (str != "BIN") && (str != "126") && (str != "127")) break;
  spis_f << str1;
 }

 ind=FindStrInList(hdr,"MARKER NAME");
 if (ind == -1) return false;

 str=hdr.at(ind);
 str=str.simplified();
 nka=str.section(' ',0,0);

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 tm=dt.time();
 sec=tm.second();
 sec1=(sec/10)*10;
 zn_sec=sec-sec1;

 return true;
}

bool RinexFile::ReadHdrDDS(QStringList list)
{
 QString str,str1;
 int ind,izm;
// QString fo,lo;
 QStringList spep;

 if (list.isEmpty()) return false;

 str=list.at(0);
 str=str.simplified();
 str1=str.section(' ',0,0);
 if (str1 != "3.00") return false;
 str1=str.section(' ',1,1);
 if (str1 != "OBSERVATION") return false;

 str=list.at(1);
 str=str.simplified();
 str1=str.section(' ',0,0);
 if (str1 != "ConvertCOGI") return false;

 ind=FindStrInList(list,"END OF HEADER");
 if (ind == -1) return false;

// fo=RnxFirstObs(list);
// lo=RnxLastObs(list);

// if (fo != lo) return true;

 spep=SpisRnxDataDesc(list);
 if (spep.count() == 0) return false;

 for (int i=0; i<spep.count(); i++)
 {
  izm=KolIzmInDat(spep[i],str);
  if (izm > 1) return true;
 }

// ind=FindStrInList(list,">");
// if (ind == -1) return false;

// izm=KolIzmInDat(list[ind]);
// if (izm == 1) return false;

 return false;
}

bool RinexFile::ObDDS(QStringList spis_file,QString path_bii,QString path_temp,bool prd_show,bool pr_soob)
{
 qlonglong kolstr,kolstr1;
 QStringList list,hdr;
 QStringList sp_file_ppsi,l_ppsi;
 QString nka,nka1;
 bool pr;
 QStringList new_spis_file;
 int sec;
 QStringList list_err;
 QProgressDialog prd;
 QString str;
 QString soob;

 kolstr=spis_file.count();
 if (kolstr == 0) return false;
 nka="";
 sec=-1;

 if (prd_show)
 {
  str="Проверка файлов......";
  prd.setModal(true);
  prd.setCancelButton(0);
  prd.setLabelText(str);
  prd.setMinimum(0);
  prd.setMaximum(kolstr);
  prd.show();
 }

  for (int i=0; i<kolstr; i++)
  {
   list=ReadFileInList(spis_file[i]);
   kolstr1=list.count();
   if (! DelEpoch(1,list,str))
   {
    if (pr_soob)
    {
     soob=QObject::tr("Ошибка структурного контроля. ")+"\n";
     soob=soob+QObject::tr("Файл : ")+spis_file[i]+QObject::tr(" исключён из обработки.");
     QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
    }
    if (prd_show) prd.setValue(i);
    list_err.clear(); list_err << str;
    str=spis_file[i]+".ERR";
    WriteFileFromList(list_err,str);
    qApp->processEvents();
    continue;
   }
   list_err=ProvStrDatRnx(list,"");
   if (list_err.count() > 0)
   {
    if (pr_soob)
    {
     soob=QObject::tr("Ошибка структурного контроля. ")+"\n";
     soob=soob+QObject::tr("Файл : ")+spis_file[i]+QObject::tr(" исключён из обработки.");
     QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
    }
    if (prd_show) prd.setValue(i);
    str=spis_file[i]+".ERR";
    WriteFileFromList(list_err,str);
    qApp->processEvents();
    continue;
   }
   pr=ReadHdrDDS(list,nka1,l_ppsi,sec);
   if (!pr)
   {
    if (pr_soob)
    {
     soob=QObject::tr("Ошибка данных. ")+"\n";
     soob=soob+QObject::tr("Файл : ")+spis_file[i]+QObject::tr(" исключён из обработки.");
     QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
    }
    if (prd_show) prd.setValue(i);
    qApp->processEvents();
    continue;
   }
   if (nka == "") nka=nka1;
   if (nka != nka1)
   {
    if (prd_show) prd.setValue(i);
    qApp->processEvents();
    continue;
   }
   if (kolstr1 != list.count()) WriteFileFromList(list,spis_file[i]);
   AddList(sp_file_ppsi,l_ppsi);
   new_spis_file << spis_file[i];
   if (prd_show) prd.setValue(i);
   qApp->processEvents();
  }

  if (prd_show) prd.close();
  qApp->processEvents();
  list=ObOFile(new_spis_file);
  if (list.count() == 0) return false;
  DelDublForList(sp_file_ppsi);
  hdr=FormHdrDDS(sp_file_ppsi,list,nka.mid(0,3));
  AddList(hdr,list);

 list=FileInfo(spis_file[0]);
 nka=list[2];
 nka.replace(7,1,"0");
 if (! WriteFileFromList(hdr,path_temp+"/"+nka)) return false;

 if (! CopyOFileInBII(path_temp+"/"+nka,path_bii,true,true)) return false;

 return true;
}

QStringList RinexFile::StatDDS(QStringList list)
{
 QStringList ret;
 QStringList list_dd;
 qlonglong kolstr;
 QDateTime ne,ke,te,te1;
 QTime tne,tte,zt,tte1,tke;
 int sec, sec1, psec;
 QString zap_ret;
 QDate dne;
 double psecd;

 if (list.isEmpty()) return ret;

 if (KolSutInRnx(list) > 1) return ret;

 list_dd=SpisRnxDataDesc(list);
 kolstr=list_dd.count();
 if (kolstr == 0) return ret;

 ne=DTForHdrDatRnx(list_dd[0]);
 dne=ne.date();
 tne=ne.time();
 sec=tne.second();
 sec1=(sec/10)*10;
 sec=sec-sec1;
 tne.setHMS(0,0,sec,0);
 ne.setTime(tne); //определили первую теоретическую эпоху суток

 ke=DTForHdrDatRnx(list_dd[kolstr-1]);
 tne=ke.time();
 tne.setHMS(23,59,50+sec,0);
 ke.setTime(tne); //определили последнюю теоретическую эпоху суток

 te=DTForHdrDatRnx(list_dd[0]);

 if (te > ne) //первая эпоха измерений больше теоретической
 {
  tte=te.time();
  tne=ne.time();
  psec=tne.secsTo(tte);
  zt.setHMS(0,0,0,0);
  zt=zt.addSecs(psec);
  psecd=(psec/10)+0.1;
  psec=psecd-1;
  if (psec < 1) psec++;
  zap_ret=dne.toString("yyyy.MM.dd")+","+tne.toString("hh:mm:ss")+","+QString::number(psec)+","+zt.toString("hh:mm:ss");
  ret << zap_ret;
 }

 for (int i=0; i<kolstr-1; i++) //цикл по эпохам измерений
 {
  te=DTForHdrDatRnx(list_dd[i]); //текущая эпоха
  te1=DTForHdrDatRnx(list_dd[i+1]);//следующая эпоха
  tte=te.time();
  tte1=te1.time();
  psec=tte.secsTo(tte1);
  if (psec > 10)
  {
   zt.setHMS(0,0,0,0);
   zt=zt.addSecs(psec);
   psecd=(psec/10)+0.1;
   psec=psecd-1;
   if (psec < 1) continue;
   zap_ret=dne.toString("yyyy.MM.dd")+","+tte.toString("hh:mm:ss")+","+QString::number(psec)+","+zt.toString("hh:mm:ss");
   ret << zap_ret;
  }
 }

 te=DTForHdrDatRnx(list_dd[kolstr-1]);

 if (te < ke)//последняя эпоха меньше теоретической
 {
  tte=te.time();
  tke=ke.time();
  psec=tte.secsTo(tke);
  zt.setHMS(0,0,0,0);
  zt=zt.addSecs(psec);
  psecd=(psec/10)+0.1;
  psec=psecd-1;
  if (psec < 1) psec++;
  zap_ret=dne.toString("yyyy.MM.dd")+","+tte.toString("hh:mm:ss")+","+QString::number(psec)+","+zt.toString("hh:mm:ss");
  ret << zap_ret;
 }

 return ret;
}

void RinexFile::ClearMasSputn()
{
 for (int j=0; j<32; j++)
 {
  for (int k=0; k<13;k++)
  mas_gps[j][k]=0;
 }
 for (int j=0; j<24; j++)
 {
  for (int k=0; k<13;k++)
  mas_glo[j][k]=0;
 }
 return;
}

void RinexFile::ZapSputnInMas(QString str)
{
 QString nkg;
 int nsp;
 int dlstr;
 int kolgr=0;
 int ind;

 nkg=str.mid(0,1);
 if ((nkg != "G") && (nkg != "R")) return;

 nsp=str.mid(1,2).toInt();
 if (nsp == 0) return;

 str=str.trimmed();
 dlstr=str.length();

 if (dlstr == 3) return;

 kolgr++;
 if (dlstr > 49) kolgr++;
 if (dlstr > 97) kolgr++;
 if (dlstr > 145) kolgr++;

 for (int i=0; i<kolgr; i++)
 {
  ind=(i*3)+1;

  if (nkg == "G")
  {
   if (nsp > 32)  continue;
   mas_gps[nsp-1][0]=1;
   mas_gps[nsp-1][ind]=mas_gps[nsp-1][ind]+1;
   mas_gps[nsp-1][ind+1]=mas_gps[nsp-1][ind+1]+1;
   mas_gps[nsp-1][ind+2]=mas_gps[nsp-1][ind+2]+1;
  }
  if (nkg == "R")
  {
   if (nsp > 24) continue;
   mas_glo[nsp-1][0]=1;
   mas_glo[nsp-1][ind]=mas_glo[nsp-1][ind]+1;
   mas_glo[nsp-1][ind+1]=mas_glo[nsp-1][ind+1]+1;
   mas_glo[nsp-1][ind+2]=mas_glo[nsp-1][ind+2]+1;
  }

 }

 return;
}

void RinexFile::ZapSputnInMas(QStringList list)
{
 int kolstr;
 int nizm;
 QString str;

 kolstr=list.count();
 if (kolstr == 0) return;

 nizm=FindStrInList(list,">");

 ClearMasSputn();

 for (int i=nizm; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">") continue;
  ZapSputnInMas(str);
 }

 return;
}

int RinexFile::ReadKolObsTypes(QStringList list,QString kod)
{
 int ind;
 QString str,str1;
 int kolizm;

 ind=FindStrInList(list,"SYS / # / OBS TYPES");
 if (ind < 0) return -1;

 str=list.at(ind);
 str=str.simplified();
 str1=str.section(" ",0,0);

 if (str1 == kod)
 {
  kolizm=str.section(" ",1,1).toInt();
  return kolizm;
 }

 ind=FindStrInList(list,"SYS / # / OBS TYPES",ind+1,2);
 if (ind < 0) return -1;

 str=list.at(ind);
// str1=str.mid(0,1);
 str=str.simplified();
 str1=str.section(" ",0,0);
 if (str1 == kod)
 {
  kolizm=str.section(" ",1,1).toInt();
  return kolizm;
 }

 return -1;
}

QStringList RinexFile::CreatePrnOfObs(QStringList list)
{
 QStringList prn;
 int kolizm;
 QString str,str1;
 qint32 dlstr;

 if (list.isEmpty()) return prn;

 ZapSputnInMas(list);

 kolizm=ReadKolObsTypes(list,"G");

 if (kolizm > 0)
 {
  for (int i=0; i<32; i++)
  {
   if (mas_gps[i][0] == 0) continue;

   str="   G"+QString::number(i+1).rightJustified(2,'0');
   for ( int j=1; j<=kolizm; j++ )
   {
    str=str+str1.setNum(mas_gps[i][j]).rightJustified(6,' ');
   }
   dlstr=60-str.length();
   if (dlstr > 0) str=str+str1.fill(' ',dlstr)+"PRN / # OF OBS      ";
   else str=str+"PRN / # OF OBS      ";
   prn << str;
  }
 }

 kolizm=ReadKolObsTypes(list,"R");

 if (kolizm > 0)
 {
  for (int i=0; i<24; i++)
  {
   if (mas_glo[i][0] == 0) continue;

   str="   R"+QString::number(i+1).rightJustified(2,'0');
   for ( int j=1; j<=kolizm; j++ )
   {
    str=str+str1.setNum(mas_glo[i][j]).rightJustified(6,' ');
   }
   dlstr=60-str.length();
   if (dlstr > 0) str=str+str1.fill(' ',dlstr)+"PRN / # OF OBS      ";
   else str=str+"PRN / # OF OBS      ";
   prn << str;
  }
 }

 return prn;
}

void RinexFile::UpdatePrnOfObs(QStringList &list)
{
 QStringList prn;
 int indn,indk,indd;
 QStringList hdr,nhdr,hdr1,hdr2;
 QStringList dat;

 prn=CreatePrnOfObs(list);
 if (prn.count() == 0) return;

 hdr=ReadRnxHdr(list);
 indn=FindStrInList(hdr,"PRN / # OF OBS");
 if (indn == -1)
 {
  indn=FindStrInList(hdr,"SYS / PHASE SHIFT",0,3);
  if (indn == -1) return;
 }
 indk=FindStrInList(hdr,"PRN / # OF OBS",0,3);
 if (indk == -1)
 {
  indk=FindStrInList(hdr,"SYS / PHASE SHIFT",0,3);
  if (indk == -1) return;
 }
 indd=FindStrInList(list,">");
 dat=CopyList(list,indd,0);

 hdr1=CopyList(hdr,0,indn);
 hdr2=CopyList(hdr,indk+1,0);

 AddList(nhdr,hdr1);
 AddList(nhdr,prn);
 AddList(nhdr,hdr2);

 list.clear();
 AddList(list,nhdr);
 AddList(list,dat);

 UpdateOfSat(list);

 return;
}

QString RinexFile::ReadDTFromN(QString str)
{
 QString ret="";

 if ((str.mid(0,1) != "R") && (str.mid(0,1) != "G")) return ret;

 ret=str.mid(4,19);

 return ret;
}

QStringList RinexFile::SetListNFile(QStringList list)
{
 QStringList ret;
 int ind;
 qlonglong kolstr;
 QString str, str1;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"END OF HEADER");
 ind++;

 kolstr=list.count();
 if (kolstr == 0) return ret;

 str1="";

 for (int i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  if ((str.mid(0,1) == "R") || (str.mid(0,1) == "G"))
  {
   if (str1 != "") ret << str1;
   str1=str+";";
   continue;
  }
  str1=str1+str+";";
 }

 if (str1 != "") ret << str1;

 return ret;
}

QStringList RinexFile::GetListNFile(QStringList list)
{
 QStringList ret;
 qlonglong kolstr;
 QString str;
 QStringList epoh;

 kolstr=list.count();
 if (kolstr == 0) return ret;

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  epoh=str.split(";",QString::SkipEmptyParts);
  AddList(ret,epoh);
 }

 return ret;
}

bool RinexFile::KolSutNFile(QStringList list,QDate &dn,QDate &dk,int &kolsut)
{
 qlonglong kolstr;
 QDateTime dt;
 QDate d;
 QString str;

 kolstr=list.count();
 if (kolstr == 0) return false;

 kolsut=0;
 str=ReadDTFromN(list[0]);
 if (str == "") return false;
 dt=SetDTFromStr(str);
 d=dt.date();
 dn=d; dk=d;
 for (int i=0; i<kolstr; i++)
 {
  str=ReadDTFromN(list[i]);
  dt=SetDTFromStr(str);
  d=dt.date();
  if (d < dn) dn=d;
  if (d > dk) dk=d;
 }

 kolsut=(dn.daysTo(dk))+1;

 return true;
}

QStringList RinexFile::RazbNFile(QString nfile)
{
 QStringList ret;
 QStringList hdr,dat,dat1,dat2,datepoh;
 QDate dn,dk;
 int kolsut;
 QString str,npn,path,ras,nf;


 dat=FileInfo(nfile);
 path=dat[1];
 str=dat[2];
 nf=dat[2];
 npn=str.mid(0,4);
 ras=str.right(1);

 dat=ReadFileInList(nfile);
 str=RnxVersion(dat);
 if (str != "3.02") return ret;
 hdr=ReadRnxHdr(dat);
 dat1=CopyList(dat,hdr.count(),0);
 datepoh=SetListNFile(dat1);
 if (! KolSutNFile(datepoh,dn,dk,kolsut)) return ret;
 if (kolsut < 2) return ret;
 DelFile(nfile);

 for (int i=0; i<kolsut; i++)
 {
  dat2.clear();
  dk=dn.addDays(i);
  dat=OtbNFileForDat(datepoh,dk);
  if (dat.isEmpty())
  {
   qApp->processEvents();
   continue;
  }
  dat1=GetListNFile(dat);
  AddList(dat2,hdr);
  AddList(dat2,dat1);
  UpdateDateNFile(dk,dat2);
  str=NazNFile(npn,dk,path,ras);
  WriteFileFromList(dat2,path+"/"+str);
  ret << path+"/"+str;
  qApp->processEvents();
 }

 return ret;
}

QStringList RinexFile::OtbNFileForDat(QStringList list,QDate depoh)
{
 QStringList ret;
 qlonglong kolstr;
 QString str,str1;
 QDateTime dt;
 QDate d;

 kolstr=list.count();
 if (kolstr == 0) return ret;

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str1=ReadDTFromN(str);
  dt=SetDTFromStr(str1);
  d=dt.date();
  if (d == depoh) ret << str;
 }

 return ret;
}

QString RinexFile::NazNFile(QString npn,QDate d,QString path,QString tipf)
{
 QString ret="";
 QString str;
 QString pref="123456789abcdefghijklmnopqastuvwxyz";//34
 int sut,god;
 QString ssut="000";
 QString sgod="00";
 QFile f;

 sut=d.dayOfYear();
 str.setNum(sut);
 ssut.replace(3-str.length(),str.length(),str);

 god=d.year();
 str.setNum(god);
 str=str.mid(2,2);
 sgod.replace(2-str.length(),str.length(),str);

 ret=npn+ssut+"0."+sgod+tipf;

 for (int i=0; i<34; i++)
 {
  f.setFileName(path+"/"+ret);
  if (!f.exists()) break;
  ret.replace(7,1,pref.mid(i,1));
 }

 return ret;
}

bool RinexFile::ObNFile(QStringList spis_f,QString &nfile)
{
 qlonglong kolstr;
 QStringList full,hdr,dat,datepoch,newdat;
 int inddat;
 QMap<QString,QString> map;
 QDate dn,dk;
 int kolsut;
 QString str,npn,path,tipf;
 int zapr;

 kolstr=spis_f.count();
 if (kolstr == 0) return false;

 for (int i=0; i<kolstr; i++)
 {
  full=ReadFileInList(spis_f[i]);
  str=RnxVersion(full);
  if (str != "3.02") continue;
  if (hdr.isEmpty()) hdr=ReadRnxHdr(full);
  inddat=FindStrInList(full,"END OF HEADER");
  if (inddat == -1) return false;
  dat.clear();
  dat=CopyList(full,inddat+1,0);
  datepoch=SetListNFile(dat);
  KolSutNFile(datepoch,dn,dk,kolsut);
  if (kolsut > 365)
  {
   return false;
//   str="Очень большое количество суток в файле : "+spis_f[i]+" ."+"\n"+" Файл исключить из обработки?";
//   zapr=QMessageBox::warning(0,QObject::tr("Предупреждение"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
//   if (zapr == QMessageBox::Yes)
//   {
//    str=spis_f[i]+".bak";
//    spis_f.replace(i,str);
//    continue;
//   }
  }
   if (dn.year() < 2000)
   {
    return false;
//    str="Год первой эпохи в файле : "+spis_f[i]+" - "+QString::number(dn.year())+" ."+"\n"+" Файл исключить из обработки?";
//    zapr=QMessageBox::warning(0,QObject::tr("Предупреждение"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
//    if (zapr == QMessageBox::Yes)
//    {
//     str=spis_f[i]+".bak";
//     spis_f.replace(i,str);
//     continue;
//    }
   }
  AddList(newdat,datepoch);
 }

 for (int j=0; j<kolstr; j++)
 {
  str=spis_f[j];
  if (str.right(3) == "bak")
  {
   str=str.mid(0,str.length()-4);
   QFile::rename(str,str+".bak");
  }
 }

 if (newdat.isEmpty()) return false;

 map=CopyListToMapNFile(newdat);
 newdat=CopyMapToListNFile(map);
 KolSutNFile(newdat,dn,dk,kolsut);

 dat=GetListNFile(newdat);
 AddList(hdr,dat);

 full=FileInfo(spis_f[0]);
 path=full[1];
 str=full[2];
 npn="OBFL";
 tipf=str.right(1);

 for (int j=0; j<kolstr; j++)
 {
  str=spis_f[j];
  if (str.right(3) == "bak") continue;
  DelFile(spis_f[j]);
 }

 str=NazNFile(npn,dn,path,tipf);
 WriteFileFromList(hdr,path+"/"+str);

 nfile=path+"/"+str;

 return true;
}

QMap<QString,QString> RinexFile::CopyListToMapNFile(QStringList list)
{
 QMap<QString,QString> map;
 qlonglong kolstr;
 QString str,key;

 kolstr=list.count();
 if (kolstr == 0) return map;

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  key=str.mid(0,23);
  map.insert(key,str);
 }

 return map;
}

QStringList RinexFile::CopyMapToListNFile(QMap<QString,QString> map)
{
 QStringList list;
 QMap<QString,QString>::const_iterator i;

 if (map.isEmpty()) return list;

 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
   list << i.value();
 }

 return list;
}

bool RinexFile::UpdateDateNFile(QDate dat,QStringList &list)
{
 QString strdat="";
 int ind;
 QString str;

 strdat=dat.toString("yyyyMMdd");

 ind=FindStrInList(list,"PGM / RUN BY / DATE");
 if (ind == -1) return false;

 str=list[ind];
 str=str.replace(40,8,strdat);

 list.replace(ind,str);

 return true;
}

bool RinexFile::ObAllNFile(QString path,QStringList &sp_g,QStringList &sp_n)
{
 QStringList spis_f;
 qlonglong kolstr;
 QStringList info;
 QStringList spis_g,spis_n;
 QString str;

 spis_f=SpisAllFile(path);

 if (spis_f.isEmpty()) return false;
 kolstr=spis_f.count();

 for (int i=0; i<kolstr; i++)
 {
  info=FileInfo(spis_f[i]);

  if ((info[4].right(1) == "N") || (info[4].right(1) == "n"))
  {
   spis_n << spis_f[i];
  }
  if ((info[4].right(1) == "G") || (info[4].right(1) == "g"))
  {
   spis_g << spis_f[i];
  }
 }

 if (!spis_n.isEmpty())
 {
  if (! ObNFile(spis_n,str)) return false;
  sp_n=RazbNFile(str);
 }

 if (!spis_g.isEmpty())
 {
  if (! ObNFile(spis_g,str)) return false;
  sp_g=RazbNFile(str);
 }

 return true;
}

QStringList RinexFile::SetListNFile2(QStringList list)
{
 QStringList ret;
 int ind;
 qlonglong kolstr;
 QString str, str1;
 int nsp;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"END OF HEADER");
 ind++;

 str1="";

 kolstr=list.count();
 if (kolstr == 0) return ret;

 for (int i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  nsp=str.mid(0,2).toInt();
  if (nsp > 0)
  {
   if (str1 != "") ret << str1;
   str1=str+";";
   continue;
  }
  str1=str1+str+";";
 }

 if (str1 != "") ret << str1;

 return ret;
}

QString RinexFile::ReadDTFromN2(QString str)
{
 QString ret="";
 qint16 ns;

 ns=str.mid(0,2).trimmed().toInt();
 if (ns == 0) return ret;

 ret=str.mid(3,19);

 return ret;
}

bool RinexFile::KolSutNFile2(QStringList list,QDateTime &dtn,QDateTime &dtk,int &kolsut)
{
 qlonglong kolstr;
 QDateTime dt;
 QString str;

 kolstr=list.count();
 if (kolstr == 0) return false;

 kolsut=0;
 str=ReadDTFromN2(list[0]);
 if (str == "") return false;
 dt=SetDTFromStr(str);
 dtn=dt; dtk=dt;
 for (int i=0; i<kolstr; i++)
 {
  str=ReadDTFromN2(list[i]);
  dt=SetDTFromStr(str);
  if (dt < dtn) dtn=dt;
  if (dt > dtk) dtk=dt;
 }

 kolsut=(dtn.date().daysTo(dtk.date()))+1;

 return true;
}

bool RinexFile::KolSutNFile(QStringList list,QDateTime &dtn,QDateTime &dtk,int &kolsut)
{
 qlonglong kolstr;
 QDateTime dt;
 QString str;

 kolstr=list.count();
 if (kolstr == 0) return false;

 kolsut=0;
 str=ReadDTFromN(list[0]);
 if (str == "") return false;
 dt=SetDTFromStr(str);
 dtn=dt; dtk=dt;
 for (int i=0; i<kolstr; i++)
 {
  str=ReadDTFromN(list[i]);
  dt=SetDTFromStr(str);
  if (dt < dtn) dtn=dt;
  if (dt > dtk) dtk=dt;
 }

 kolsut=(dtn.date().daysTo(dtk.date()))+1;

 return true;
}

qint32 RinexFile::RnxInterval(QStringList list)
{
 qint32 ret=0;
 int ind;
 QString str;

 if (list.isEmpty()) return ret;

 ind=FindStrInList(list,"INTERVAL");
 if (ind < 0) return ret;

 str=list[ind];
 str=str.simplified().section(" ",0,0);
 ret=str.toFloat();

 return ret;
}

void RinexFile::UpdateOfSat(QStringList &list)
{
 QString str;
 qlonglong ind;

 str=CreateOfSatellites();
 ind=FindStrInList(list,"# OF SATELLITES");
 if (ind == -1) return;
 list.replace(ind,str);

 return;
}

bool RinexFile::DelEpoch(qint32 kol_sat,QStringList &list,QString &err)
{
 qlonglong kolstr;
 QString str;
 qint32 ind;
 qint32 kol_izm;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,">");
 if (ind == -1) return false;

 for (int i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) != ">") continue;
  kol_izm=KolIzmInDat(str,err);
  if (kol_izm == -1) return false;
  if (kol_izm > kol_sat) continue;
  for (int j=0; j<kol_izm+1; j++)
  {
   list.removeAt(i);
  }

  i--;
  kolstr=list.count();
  if (kolstr == 0) continue;
 }

 return true;
}

QStringList RinexFile::RazdNFileToNG(QStringList list,QString kod)
{
 QStringList ret;
 QStringList hdr;
 QMap<QString,QString> map;
 qlonglong kol;
 QString str;
 QStringList list1;

 hdr=ReadRnxHdr(list);

 list=SetListNFile(list);
 map=CopyListToMapNFile(list);
 list=CopyMapToListNFile(map);

 kol=list.count();
 if (kol == 0) return ret;

 if (kod == "G") kod="R";
 if (kod == "N") kod="G";

 for (int i=0; i<kol; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == kod) list1 << str;
 }

 list1=GetListNFile(list1);
 if (list1.isEmpty()) return ret;

 AddList(hdr,list1);

 return hdr;
}

bool RinexFile::RazdNFileToNG(QString nfile,QString npn,QStringList &sp_file)
{
 QStringList list,list1,list2;
 QString newname;
 QStringList f;
 QString path;
 QDate dn,dk;
 int kolsut;

 sp_file.clear();

 f=FileInfo(nfile);
 path=f[1];

 list=ReadFileInList(nfile);
 if (list.isEmpty()) return false;

 list1=RazdNFileToNG(list,"G");
 if (! list1.isEmpty())
 {
  list2=SetListNFile(list1);
  KolSutNFile(list2,dn,dk,kolsut);
  UpdateDateNFile(dn,list1);
  newname=NazNFile(npn,dn,path,"G");
  newname=path+"/"+newname;
  if (! WriteFileFromList(list1,newname)) return false;
  sp_file << newname;
 }

 list1=RazdNFileToNG(list,"N");
 if (! list1.isEmpty())
 {
  list2=SetListNFile(list1);
  KolSutNFile(list2,dn,dk,kolsut);
  UpdateDateNFile(dn,list1);
  newname=NazNFile(npn,dn,path,"N");
  newname=path+"/"+newname;
  if (! WriteFileFromList(list1,newname)) return false;
  sp_file << newname;
 }

 DelFile(nfile);

 return true;
}

bool RinexFile::StatEpochForDDS(QStringList list,qint32 &kol1,qint32 &kol2,qint32 &kol3,qint32 &kolep)
{
 qlonglong kolstr,ind;
 qint32 kolizm, ch;
 QString str;

 kol1=0; kol2=0; kol3=0; kolep=0;
 for (int i=0; i<24; i++) kol_ep_dds[i]=0;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,">");
 if (ind == -1) return false;

 for (qint32 i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">")
  {
   kolep++;
   ch=StatGlonassForEpoch(list,i,kolizm);

   if (ch < 4) kol1++;
   if (ch == 4) kol2++;
   if (ch > 4) kol3++;
   i=i+kolizm;
   kol_ep_dds[ch]++;
  }
 }

 return true;
}

qint32 RinexFile::StatGlonassForEpoch(QStringList &list,qint32 ind,qint32 &kolizm)
{
 qint32 ch;
 QString str;
 QString err;

 ch=0;
 str=list.at(ind);
 if (str.mid(0,1) != ">") return ch;

 kolizm=KolIzmInDat(str,err);
 if (kolizm == -1) return ch;

 for (int j=1; j<=kolizm; j++)
 {
  str=list.at(ind+j);
  if (str.mid(0,1) == "G") continue;
  if (str.trimmed().length() < 50) continue;
  ch++;
 }
 return ch;
}

void RinexFile::SetToGPS(QDateTime dt,qint32 &weekGPS, qint32 &secGPS)
{
 //QDate ndt, kdt;
 //QTime ntm, ktm;
 //qint64 njd,kjd,r1jd;
 qint32 rjd,rjd1;
 qint32 ksek;

 /*
 weekGPS=0; secGPS=0;

 ndt.setDate(1980,1,6);
 ntm.setHMS(0,0,0);

 kdt=dt.date();
 ktm=dt.time();
 ksek=ntm.secsTo(ktm);

 njd=ndt.toJulianDay();
 kjd=kdt.toJulianDay();
 rjd=(kjd-njd)/7;
 r1jd=rjd*7;
 r1jd=(kjd-njd)-r1jd;
 ksek=r1jd*86400+ksek;

 while (rjd >= 1024) rjd=rjd-1024;

 */

 SetToGPS(dt,rjd,ksek,rjd1);

 weekGPS=rjd;
 secGPS=ksek;

 return;
}

void RinexFile::SetToGPS(QDateTime dt,qint32 &weekGPS, qint32 &secGPS,qint32 &weekGPS1)
{
 QDate ndt, kdt;
 QTime ntm, ktm;
 qint64 njd,kjd,rjd,r1jd;
 qint32 ksek;

 weekGPS=0; secGPS=0;

 ndt.setDate(1980,1,6);
 ntm.setHMS(0,0,0);

 kdt=dt.date();
 ktm=dt.time();
 ksek=ntm.secsTo(ktm);

 njd=ndt.toJulianDay();
 kjd=kdt.toJulianDay();
 rjd=(kjd-njd)/7;
 r1jd=rjd*7;
 r1jd=(kjd-njd)-r1jd;
 ksek=r1jd*86400+ksek;

 weekGPS1=rjd;

 while (rjd >= 1024) rjd=rjd-1024;

 weekGPS=rjd;
 secGPS=ksek;

 return;
}

void RinexFile::SetToGLO(QDateTime dt,qint32 &sutGLO, qint32 &secGLO)
{
 QDate dat,vdat;
 QTime tm,ntm;
 qint32 year,vyear;
 bool pr_vis=false;
 qint32 kolsut,kolsec;

 sutGLO=0; secGLO=0;
 dat=dt.date();
 tm=dt.time();
 year=dat.year();

 vyear=year;
 while (!pr_vis)
 {
  pr_vis=isVisYear(vyear);
  if (!pr_vis) vyear--;
 }

 vdat.setDate(vyear,1,1);
 kolsut=vdat.daysTo(dat)+1;
 ntm.setHMS(0,0,0);
 kolsec=ntm.secsTo(tm);

 sutGLO=kolsut; secGLO=kolsec;

 return;
}

bool RinexFile::ObDDS(QMap<QString,QString> &map,QStringList dob_file,QStringList &ppsi_file,QString nka)
{
 qlonglong kolstr;
 QStringList l_ppsi;
 QString nka1;
 int sec;
 QString str;
 QMap<QString,QString> map1;

 kolstr=dob_file.count();
 if (kolstr == 0) return false;
 sec=-1;

 if (! DelEpoch(1,dob_file,str)) return false;
// ind=FindStrInList(dob_file,">");
// if (ind == -1) return true;

 if (!ReadHdrDDS(dob_file,nka1,l_ppsi,sec)) return false;
 nka1=nka1.mid(0,3);
 if (nka != nka1) return false;
 AddList(ppsi_file,l_ppsi);
 map1=ReadOFileInMap(dob_file);

 AddMap(map,map1,true);
// AddMap(map,map1,true); //ОТЛАДКА

 qSort(ppsi_file);
 DelDublInList(ppsi_file);

 return true;
}

QStringList RinexFile::RazbDDSOFile(QStringList list,QString path)
{
 QStringList hdr;
 int kolsut;
 QString str;
 QDateTime dt;
 QDate dat;
 QStringList new_list;
 QStringList ret;
 QString nka,nka1;
 QStringList spis_f;
 int zn_sec;

 if (list.isEmpty()) return ret;

 kolsut=KolSutInRnx(list);
 if (kolsut == 1) return ret;

 if (! ReadHdrDDS(list,nka,spis_f,zn_sec)) return ret;

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);

 for(int i=0; i<kolsut; i++)
 {
  dat=dt.date();
  dat=dat.addDays(i);

  new_list=CreateListForDate(list,dat);
  if (new_list.count() == 0) continue;

  nka1=nka.mid(0,3);
  hdr=FormHdrDDS(spis_f,new_list,nka1);
  AddList(hdr,new_list);

  str=NazOFile(nka,hdr,path);

  WriteFileFromList(hdr,path+"/"+str);
  ret << path+"/"+str;
 }

 return ret;
}

QStringList RinexFile::ProvStrDatRnx(QStringList list)
{
 QStringList err;
 int tekind,tekind1,tekind2,inderr;
 QString strerr;
 QString strhdr;
 int kolizm;
 qlonglong kolstr;
 QList<qint32> linderr;

 kolstr=list.count();
 if (kolstr == 0)
 {
  strerr=QObject::tr("Файл пустой.");
  err << strerr;
  return err;
 }

 tekind=FindStrInList(list,"END OF HEADER");
 if (tekind == -1)
 {
  strerr=QObject::tr("Не найден конец заголовка Rinex-файла.");
  err << strerr;
  return err;
 }
 tekind++;

 if (tekind == kolstr)
 {
  strerr=QObject::tr("Измерений не найдено.");
  err << strerr;
  return err;
 }

 tekind1=tekind;

 while (tekind < kolstr)
 {
  strhdr=list.at(tekind);
  kolizm=KolIzmInDat(strhdr,strerr);

  if (kolizm == -1)
  {
   strerr="Строка: "+QString::number(tekind1-1)+". Нарушена структура Rinex-файла.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << list[tekind1-1];
   tekind=FindStrInList(list,">",tekind,2);
   if (tekind == -1) return err;
   continue;
  }

  if (kolizm == 0)
  {
   strerr="Строка: "+QString::number(tekind+1)+". Нулевое количество измерений.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << list[tekind];
   tekind2=FindStrInList(list,">",tekind,2);
   if ((tekind2 == -1) || (tekind == tekind2)) return err;
   tekind++;
   continue;
  }

  if (!ProvRnxNSat(list,tekind,kolizm,linderr))
  {
   for (int j=0; j<linderr.count(); j++)
   {
    inderr=linderr[j];
    strerr="Строка: "+QString::number(inderr)+". Неверный номер спутника ГНСС.";
    err << strerr;
    err << list[inderr];
   }
  }

  tekind1=tekind+1;
  tekind=tekind+kolizm+1;
  qApp->processEvents();
 }

 return err;
}

int RinexFile::KolSutInRnx(QString nfile,QDate &dn,QDate &dk)
{
 int sut=0;
 QString str;
 QDateTime dt;
 QStringList list;

 list=ReadFileInList(nfile);
 if (list.isEmpty()) return 0;

 if (list.isEmpty()) return sut;

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 dn=dt.date();

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 dk=dt.date();

 sut=(dn.daysTo(dk))+1;

 return sut;
}

bool RinexFile::ProvRnxInterval(QStringList rnx,QStringList &err)
{
 bool ret;
 QStringList sp_err_epoch;

 ret=ProvRnxInterval(rnx,err,sp_err_epoch);

 return ret;
}

bool RinexFile::ProvRnxInterval(QStringList rnx,QStringList &err,QStringList &sp_err_epoch)
{
 qint32 koli;
 QStringList sp_ep;
 QString ep,ep1,ep2;
 QTime tep1,tep;
 bool ret;
 qint32 zn_sec,sec,sec1,sec2;
 qint32 mas_sec[10];

 err.clear();
 sp_err_epoch.clear();
 ret=true;
 zn_sec=0;

 err=ProvStrDatRnx(rnx); //структурный контроль
 if (!err.isEmpty()) return false;

 koli=RnxInterval(rnx);
 if (koli <= 0)
 {
  err << "Не определён интервал";
  return false;
 }

 sp_ep=SpisRnxDataDesc(rnx);
 if (sp_ep.isEmpty())
 {
  err << "Не найдено эпох";
  return false;
 }

 if (sp_ep.count() < 3)
 {
  ep=sp_ep[0];
  ep1=sp_ep[1];
  sec=ZnSekDds(ep,tep);
  sec1=ZnSekDds(ep1,tep1);
  if ((qAbs(sec-sec1)) >1)
  {
   err << "Ошибка в интервале наблюдений: "+QString::number(koli);
   err << "-----------------------------: "+ep;
   err << "-----------------------------: "+ep1;
  }
  return false;
 }

 for (qint32 i=0; i < 10; i++)
 {
  mas_sec[i]=0;
 }


 for (qint32 i=0; i < sp_ep.count(); i++)
 {
  ep=sp_ep.at(i);
  sec=ZnSekDds(ep,tep);
  mas_sec[sec]++;
 }

 for (qint32 i=0; i < 10; i++)
 {
  mas_sec[i]=(mas_sec[i]*100)/sp_ep.count();
 }


 for (qint32 i=0; i < sp_ep.count()-2; i++)
 {
  ep=sp_ep.at(i);
  sec=ZnSekDds(ep,tep);
  ep1=sp_ep.at(i+1);
  sec1=ZnSekDds(ep1,tep1);
  ep2=sp_ep.at(i+2);
  sec2=ZnSekDds(ep1,tep1);

  if ((qAbs(sec-sec1) > 1) && (qAbs(sec1-sec2) > 1) && ((mas_sec[sec] == 0) || (mas_sec[sec1] == 0)))
  {
   err << "Ошибка в интервале наблюдений: "+QString::number(koli);
   err << "-----------------------------: "+ep;
   err << "-----------------------------: "+ep1;

   if (mas_sec[sec] == 0) sp_err_epoch << ep;
   if (mas_sec[sec1] == 0) sp_err_epoch << ep1;

   ret=false;
   i++;
  }

 }

 if (!sp_err_epoch.isEmpty())
 {
  err << "\n Список ошибочных эпох:";
  for (int i=0; i<sp_err_epoch.count(); i++)
  {
   err << sp_err_epoch[i];
  }
 }

 return ret;
}

bool RinexFile::ProvRnxNSat(QStringList rnx,qint32 ind_str,qint32 kolizm,QList<qint32> &ind_err)
{
 QString str;
 qint32 nsat;
 bool ret=true;

 ind_err.clear();

 if ((ind_str+kolizm+1) > rnx.count()) {ind_err << ind_str; return false;}

 str=rnx.at(ind_str);
 if (str.left(1) != ">") {ind_err << ind_str; return false;}

 for (qint32 i=1; i<=kolizm; i++)
 {
  str=rnx.at(ind_str+i);
  str=str.simplified();

  str=str.section(" ",0,0);

  if ((str.left(1) != "R") && (str.left(1) != "G"))
   {ind_err << ind_str+i; ret=false;}
  nsat=str.mid(1).toInt();
  if (nsat == 0)
  {ind_err << ind_str+i; ret=false;}
  if ((nsat > 32) && (str.left(1) == "G"))
  {ind_err << ind_str+i; ret=false;}
  if ((nsat > 28) && (str.left(1) == "R"))
  {ind_err << ind_str+i; ret=false;}
 }

 return ret;
}

bool RinexFile::ProvRnxFileN2(QString nfile,bool pr_errfile)
{
 QStringList list;
 QStringList err;
 QStringList file;
 QString nfile_err;
 QString nka;

 list=ReadFileInList(nfile);
 file=FileInfo(nfile);
 nka=file[4];
 nka=nka.right(1);

 err=ProvStrDatRnxN2(list,nka);
 if (err.count() == 0) return true;

 if (pr_errfile)
 {
  nfile_err=file.at(0)+".err";
  WriteFileFromList(err,nfile_err);
 }

 return false;
}

QStringList RinexFile::ProvStrDatRnxN2(QStringList list,QString nkg)
{
 QStringList err;
 QString strerr;
 qlonglong kolstr;
 QString str;
 int ch;
 qint32 tekind;
 QDateTime dt;
 QDate d,d_hdr;
 QTime t;
 qint64 rd;

 kolstr=list.count();
 if (kolstr == 0)
 {
  strerr=QObject::tr("Файл пустой.");
  err << strerr;
  return err;
 }

 tekind=FindStrInList(list,"END OF HEADER");
 if (tekind == -1)
 {
  strerr=QObject::tr("Не найден конец заголовка Rinex-файла.");
  err << strerr;
  return err;
 }
 tekind++;

 if (tekind == kolstr)
 {
  strerr=QObject::tr("Измерений не найдено.");
  err << strerr;
  return err;
 }

 if (nkg == "G") ch=4;
 if (nkg == "N") ch=8;

 d_hdr=DatRnxN2(list);
 if (!d_hdr.isValid()) d_hdr=QDate::currentDate();

 for (int i=tekind; i<kolstr; i=i+ch)
 {
  str=list.at(i);
  if (str.mid(0,3) == "   ")
  {
   strerr="Строка: "+QString::number(i)+". Ошибка структуры.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << str;
   return err;
//   i=i-ch-1;
//   continue;
  }

  str=ReadDTFromN2(str);
  dt=SetDTFromStr(str);
  d=dt.date();
  if (d.isValid()) rd=d.daysTo(d_hdr);
  if ((!d.isValid()) || (d > d_hdr.addDays(1)) || (qAbs(rd) > 14))
  {
   strerr="Строка: "+QString::number(i)+". Ошибка даты.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << str;
  }
  t=dt.time();
  if (!t.isValid())
  {
   strerr="Строка: "+QString::number(i)+". Ошибка времени.";
   strerr=QObject::tr(strerr.toStdString().c_str());
   err << strerr;
   err << str;
  }

  for (int j=1; j<=ch-1; j++)
  {
   if ((i+j) >= kolstr)
   {
    strerr="Строка: "+QString::number(i+j)+". Ошибка структуры.";
    strerr=QObject::tr(strerr.toStdString().c_str());
    err << strerr;
    err << str;
    return err;
   }
   str=list.at(i+j);
   if (str.mid(0,3) != "   ")
   {
    strerr="Строка: "+QString::number(i+j)+". Ошибка структуры.";
    strerr=QObject::tr(strerr.toStdString().c_str());
    err << strerr;
    err << str;
    return err;
   }
  }

  qApp->processEvents();
 }

 return err;
}

bool RinexFile::DelEpoch(QString str_ep,QStringList &list,QString &err)
{
 qlonglong kolstr;
 QString str;
 qint32 ind;
 qint32 kol_izm;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,str_ep);
 if (ind == -1) return false;

  str=list.at(ind);
  if (str.mid(0,1) != ">") return false;
  kol_izm=KolIzmInDat(str,err);
  if (kol_izm == -1) return false;
  for (int j=0; j<kol_izm+1; j++)
  {
   list.removeAt(ind);
  }

 return true;
}

/*
///*************************************************
/// Проверка структуры rinex-file Version 2
///*************************************************
bool RinexFile::test_rinex_V2(QFile *file, QStringList &_errorList){
    int countTypesOfObs;  // количество типов измерений спутников
    QString typesOfObs;   // строка типов измерений спутников

    QDateTime dateTimeFirst,dateTimeLast; // Время начала и конца измерений во всем файле
    QString err = "";                     // Строка ошибок

    QTextStream str(file);
    if(!file->open(QIODevice::ReadOnly))
        qDebug()<<"Error. File NOT Open!";

    QString nextLine = str.readLine();

    ///-----------------------
    /// РАЗБОР ШАПКИ
    ///-----------------------

    nextLine = findString(&str, "TYPES OF OBSERV");
    if(nextLine == ""){
        _errorList.push_back("Ошибка в структуре заголовка rinex:        TYPES OF OBSERV '/n'");
    }

    //виды измерений (из шапки)
    countTypesOfObs = nextLine.left(7).toInt();
    typesOfObs = nextLine;

    //Число строк для измерений одного спутника
    int stepIzm = countTypesOfObs / 5 + 1;


    nextLine = findString(&str, "TIME OF FIRST OBS");
    if(nextLine == ""){
        _errorList.push_back("Ошибка в структуре заголовка rinex 2:        TIME OF FIRST OBS '/n'");
    }

    QDate date;
    QTime time;
    QString line = nextLine;

    date = QDate(line.mid(1,5).toInt(), line.mid(10,3).toInt(), line.mid(16,3).toInt());
    time = QTime(line.mid(22,3).toInt(), line.mid(28,3).toInt(),line.mid(33,2).toInt());
    dateTimeFirst = QDateTime(date, time);

    nextLine = str.readLine();
    line = nextLine;

    if(!nextLine.contains("TIME OF LAST OBS")){
        _errorList.push_back("Ошибка в структуре заголовка rinex 2:        TIME OF LAST OBS '/n'");
    }

    date = QDate(line.mid(1,5).toInt(), line.mid(10,3).toInt(), line.mid(16,3).toInt());
    time = QTime(line.mid(22,3).toInt(), line.mid(28,3).toInt(),line.mid(33,2).toInt());
    dateTimeLast = QDateTime(date, time);

    file->close();
    file->open(QIODevice::ReadOnly);

    ///расчет номера строки конца заголовка
    QString endOFHead = "";
    int long numStringInFile = 0;              //Текущая строка
    int long numStringEnd=0;
    QTextStream stream(file);
    file->open(QIODevice::ReadOnly);
    while(!endOFHead.contains("END OF HEADER")){
        if(stream.atEnd()){
            _errorList.push_back("Ошибка в структуре заголовка rinex:        END OF HEADER '/n'");
            return false;
        }
        endOFHead = stream.readLine();
        numStringInFile++;
        numStringEnd=numStringInFile;

    }


    ///----------------------
    /// РАЗБОР ЭПОХ
    ///----------------------

    int countEpochStringInterval = 0;
    int count = 0;
    int countSatellite = 0;             //Кол-во спутников в эпохе
    QString epoch;
    QString epochCurrent;            //Для информации об ошибках
    epoch = stream.readLine();
    epochCurrent = epoch;
    numStringInFile++;
    countSatellite = epoch.mid(30, 2).toInt();
    countEpochStringInterval = countSatellite * stepIzm + int(countSatellite / 12.5);

    ///Проверка структуры измерений (эпох)
    while(!stream.atEnd()){
        QDate dateEpoch;
        QTime timeEpoch;
        QDateTime dateTimeEpoch;
        epoch = stream.readLine();
        numStringInFile++;
        //Проверка строки по структуре: (ЭПОХА?)
        if((epoch.length() <= 12) || (epoch.at(3)!=" " || epoch.at(6) !=" " || epoch.at(9) != " "
                                 || epoch.at(12) != " " || epoch.at(5) == " " || epoch.at(8) == " ")){
            count++;
        }else{

            //Проверка попадания даты и времени эпохи в интервал из header файла
            dateEpoch = QDate(epoch.mid(0,3).toInt() + 2000, epoch.mid(3,3).toInt(), epoch.mid(6,3).toInt());
            timeEpoch = QTime(epoch.mid(9,3).toInt(), epoch.mid(12,3).toInt(),epoch.mid(15,3).toInt(), epoch.mid(19,3).toInt());
            dateTimeEpoch = QDateTime(dateEpoch, timeEpoch);
            if(dateTimeEpoch < dateTimeFirst || dateTimeEpoch > dateTimeLast){
                _errorList.push_back("Стр.: " + QString::number(numStringInFile) + ". Дата не попадает в диапазон");
                _errorList.push_back(epoch);
                _errorList.push_back(dateTimeEpoch.toString() + ". " + "[ " + dateTimeFirst.toString() + ", " + dateTimeLast.toString() + "]");
            }
            countSatellite = epoch.mid(30, 2).toInt();

            //Если число строк между эпохами равно количеству измерений умноженное на шаг измерений!
            //Проще: если число строк между эпохами правильное
            if(count == countEpochStringInterval){
                epochCurrent = epoch;
                countSatellite = epoch.mid(30, 2).toInt();
                countEpochStringInterval = countSatellite * stepIzm + int(countSatellite / 12.5);
                count = 0;
            }else{
                _errorList.push_back("Стр.: " + QString::number(numStringInFile - count) + ". Неверное число строк в эпохе: "+ QString::number(count) + " - есть, " + QString::number(countEpochStringInterval) + " - должно быть");
                _errorList.push_back(epochCurrent);

                epochCurrent = epoch;
                countSatellite = epoch.mid(30, 2).toInt();
                countEpochStringInterval = countSatellite * stepIzm + int(countSatellite / 12.5);
                count = 0;
            }
        }
    }

    if (numStringEnd == numStringInFile-1)
    {
      _errorList.push_back("Нет измерений");
      return false;
    }

    if(count == countEpochStringInterval && _errorList.isEmpty()){
        return true;
    }
    if(count != countEpochStringInterval){
        _errorList.push_back("Стр.: " + QString::number(numStringInFile - count) + ". Неверное число строк в эпохе: "+
                             QString::number(count) + " - есть, " + QString::number(countEpochStringInterval) + " - должно быть");
    }

    return false;
}
*/

QStringList RinexFile::StatMixForGlonass(QStringList list)
{
 qlonglong kolstr,ind;
 qint32 kolizm, ch;
 QString str;
 QStringList mix;

 mix.clear();

 kolstr=list.count();
 if (kolstr == 0) return mix;

 ind=FindStrInList(list,">");
 if (ind == -1) return mix;

 for (qint32 i=ind; i<kolstr; i++)
 {
  str=list.at(i);
  if (str.mid(0,1) == ">")
  {
   ch=StatGlonassForEpoch(list,i,kolizm);

   str=str.mid(2,27)+";"+QString::number(ch);
   mix << str;
   i=i+kolizm;
  }
 }
 return mix;
}

QMap<QString, QString> RinexFile::DelDublInMap(QMap<QString, QString> map)
{
 QString key1,key2;
 QMap<QString,QString>::const_iterator i;
 QMap<QString, QString> map1;
 QList<QString> list1;
 QString key3;

 if (map.isEmpty()) return map1;

 //убираем одинаковые спутники в эпохах
 key1.clear();
 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
  key2=i.key(); key3=key2;
  if (key2 == key1) continue;
  list1=map.values(key2);
  if (list1.count() == 0) continue;
  DelDublInList(list1);
  if (list1.count() > 1) continue; //измерения с объединяемых файлов неодинаковые
  map1.insert(key2,list1.at(0));
  key1=key2;
 }

 return map1;
}

qint32 RinexFile::ZnSekDds(QString ep,QTime &t_ep)
{
 QDateTime dt_ep;
 qint32 sec,sec1;

 dt_ep=DTForHdrDatRnx(ep);
 t_ep=dt_ep.time();
 sec=t_ep.second();
 sec1=(sec/10)*10;
 sec=sec-sec1;

 return sec;
}

QDate RinexFile::DatRnxN2(QStringList &list)
{
 QDate dt;
 qint32 ind;
 QString str;
 qint32 g,m,d;

 dt.setDate(0,0,0);

 ind=FindStrInList(list,"DATE");
 if (ind == -1) return dt;

 str=list.at(ind);
 str=str.simplified();

 str=str.section(" ",2,2);
 d=str.section("-",0,0).toInt();
 ind=FindStrInList(list_m,str.section("-",1,1));
 m=ind+1;
 g=str.section("-",2,2).toInt()+2000;

 dt.setDate(g,m,d);

 return dt;
}

QStringList RinexFile::ProvStrDatRnx2(QStringList list,QDateTime &dn,QDateTime &dk)
{
 QStringList err;
 qint32 ind,ind1;
 qint32 kol_typeobs,kol_sat,kol_strizm,kol_strep;
 QString str,str1;
 QDateTime dtn,dtk,dte;
 bool fl_soob,pr_perep;

 err.clear();

 if (list.isEmpty())
 {
  err << "RINEX файл пустой.";
  return err;
 }

 ind=FindStrInList(list,"END OF HEADER");
 if (ind == -1)
 {
  err << "Не найден заголовок RINEX файла.";
  return err;
 }

 if (ind+1 == list.count())
 {
  err << "Не найдено измерений.";
  return err;
 }

 ind1=FindStrInList(list,"TYPES OF OBSERV");
 if (ind1 == -1)
 {
  err << "Не найдена строка типов наблюдений.";
  return err;
 }
 else
 {
  str=list.at(ind1);
  kol_typeobs=str.mid(0,6).trimmed().toInt();
  kol_strizm=qCeil(kol_typeobs/5.0);
 }

 dtn=SetDTFromStr(RnxFirstObs(list));
 dtk=SetDTFromStr(RnxLastObs(list));

 fl_soob=true;
 pr_perep=true;
 for (qint32 i=ind+1; i<list.count(); i++)
 {
  str=list.at(i);

  if (! ZagEpoch2(str))
  {
   if (fl_soob)
   {
    err << "Строка : "+QString::number(i)+". Не найден заголовок эпохи.";
    err << str;
    fl_soob=false;
   }
   continue;
  }

  fl_soob=false;

  dte=SetDTFromStr(str.mid(1,25));
  if (!dte.isValid())
  {
   err << "Строка : "+QString::number(i)+". Ошибка в дате/времени эпохи.";
   err << str;
  }
  else
  {
   if (pr_perep)
   {
    dn=dte;
    dk=dte;
    pr_perep=false;
   }
   else
   {
    if (dte < dn) dn=dte;
    if (dte > dk) dk=dte;
   }
   if ((dte < dtn) || (dte > dtk))
   {
    err << "Строка : "+QString::number(i)+". Ошибка в дате/времени эпохи. Вне диапазона измерений в шапке.";
    err << str;
   }
  }

 // fl_soob=true;
  kol_sat=str.mid(29,3).trimmed().toInt();
  kol_strep=qCeil(kol_sat/12.0)+(kol_strizm*kol_sat);

  if (i+kol_strep > list.count())
  {
   err << "Строка : "+QString::number(i)+". Файл обрезан.";
   err << str;
   return err;
  }

  if (i+kol_strep == list.count()) return err;

  str1=list.at(i+kol_strep);
  if (! ZagEpoch2(str1))
  {
   err << "Строка : "+QString::number(i)+". В эпохе неверное количество измерений.";
   err << str;
   continue;
  }

  i=i+kol_strep-1;
 }

 return err;
}

bool RinexFile::ProvRnx2File(QString nfile,bool pr_errfile)
{
 QStringList list;
 QStringList err;
 QStringList file;
 QString nfile_err;
 QDateTime dtn,dtk;

 list=ReadFileInList(nfile);

 err=ProvStrDatRnx2(list,dtn,dtk);
 if (err.isEmpty()) return true;

 if (pr_errfile)
 {
  file=FileInfo(nfile);
  nfile_err=file.at(0)+".err";
  WriteFileFromList(err,nfile_err);
 }

 return false;
}

bool RinexFile::ZagEpoch2(QString zag_epoch)
{
 QString sat;
 qint32 kol_sat;
 qint32 sm=32;

 sat=zag_epoch.mid(32,1);
 if ((sat != "R") && (sat != "G")) return false;

 kol_sat=zag_epoch.mid(29,3).trimmed().toInt();
 if (kol_sat == 0) return false;
 if (kol_sat > 12) kol_sat=12;

 for (qint32 i=1; i<=kol_sat; i++)
 {
  sat=zag_epoch.mid(sm,1);
  if ((sat != "R") && (sat != "G")) return false;
  sm=sm+3;
 }

 return true;
}

