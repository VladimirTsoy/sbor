#include "obrizm.h"

ObrIzm::ObrIzm()
{
 QHostInfo host;

 list_err.clear();
 list_err << "Коды ошибок."; //0
 list_err << "Ошибка контрольной суммы измерений ДДС КВИ."; //1
 list_err << "Пустое сообщение измерений ДДС КВИ."; //2
 list_err << "Нет измерений ДДС КВИ."; //3
 list_err << "Ошибка в поле признака получения измерений ДДС КВИ."; //4
 list_err << "Время не получено для измерений ДДС КВИ."; //5
 list_err << "Ошибка в поле признака получения времени приёмника ДДС КВИ."; //6
 list_err << "Ошибка в поле модифицированного номера КА ГНСС для измерений ДДС КВИ."; //7
 list_err << "Ошибка даты получения измерений ДДС КВИ."; //8
 list_err << "Ошибка в поле времени получения измерений ДДС КВИ."; //9
 list_err << "Ошибка в поле года измерений ДДС КВИ."; //10
 list_err << "Ошибка в поле календарного номера суток НКА ГЛОНАСС для измерений ДДС КВИ."; //11
 list_err << "Ошибка в поле времени суток НКА ГЛОНАСС для измерений ДДС КВИ."; //12
 list_err << "Ошибка в поле номера недели НКА GPS для измерений ДДС КВИ."; //13
 list_err << "Ошибка в поле времени недели НКА GPS для измерений ДДС КВИ."; //14
 list_err << "Ошибка в поле номера литеры НКА ГЛОНАСС для измерений ДДС КВИ."; //15
 list_err << "Ошибка в поле модифицированного номера КА НКА ГЛОНАСС для измерений ДДС КВИ."; //16
 list_err << "Ошибка в поле модифицированного номера КА НКА GPS для измерений ДДС КВИ."; //17
 list_err << "Неделя GPS в измерении не соответствует дате приёмника для ДДС КВИ."; //18
 list_err << "Сутки ГЛОНАСС в измерении не соответствует дате приёмника для ДДС КВИ."; //19
 list_err << "Ошибка в поле фазы C/A для измерений ДДС КВИ."; //20
 list_err << "Ошибка в поле псевдодальности C/A для измерений ДДС КВИ."; //21
 list_err << "Ошибка контрольной суммы измерений ДДС КВНО."; //22
 list_err << "Навигационное определение не получено в ДДС КВНО."; //23
 list_err << "Ошибка в поле признака получения навигационного определения для ДДС КВНО."; //24
 list_err << "Время не получено для измерений ДДС КВНО."; //25
 list_err << "Ошибка в поле признака получения времени приёмника ДДС КВНО."; //26
 list_err << "Ошибка даты получения измерений ДДС КВНО."; //27
 list_err << "Ошибка времени получения измерений ДДС КВНО."; //28
 list_err << "Ошибка в поле года измерений ДДС КВНО."; //29
 list_err << "Дата в кадре ДДС КВИ больше даты приёма ниформации на ППСИ."; //30
 list_err << "Разница дат в кадре ДДС КВИ и ППСИ больше 14 суток."; //31
 list_err << "Разница дат в кадре ДДС КВНО и ППСИ больше 14 суток."; //32
 list_err << "Дата в кадре ДДС КВНО больше даты приёма ниформации на ППСИ."; //33

// QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1251"));
 komp_name=host.localHostName();
// QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

 pr_superuser=false;
 pr_stop=false;
 pr_newver=true;

 compressor = new Compressor;
 convJps = new converterJps;

}

/*!
 * \brief ObrIzm::AltiDatInList
 * Чтение содержимого ALTI файлов в список.
 * Считываются ALTI файлы на определённую дату(выбираются из имени файла)
 * и формируется объединённый неотсортированный список.
 * Содержимое списка формируется с помощью ReadAltiInList в виде:
 * dd-MM-yy hh:mm:ss №пакета_хрон №пакета_сквозн
 * \param dat[in] дата для которой формируется список
 * \param path[in] путь к директории, содержащей файлы
 * \param list[in,out] сформированный список (добавляет данные)
 * \return
*/
QStringList ObrIzm::AltiDatInList(QDate dat,QString path,QStringList &list)
{
 QStringList spis_f,info_f;
 QString str;
 QDate dat_f;
 QByteArray buf;
 QStringList ret;


 spis_f=SpisAllFile(path);
 if (spis_f.isEmpty()) return ret;

 for (int i=0; i<spis_f.count(); i++)
 {
  info_f=FileInfo(spis_f[i]);
  if (info_f[4] != "126" && info_f[4] != "127") continue;
  str=info_f[3];
  if (str.mid(0,4) != "ALTI") continue;
  dat_f=QDate::fromString("20"+str.mid(8,8),"yyyy.MM.dd");
  if (dat_f != dat) continue;
  buf=ReadFile(spis_f[i]);
  ReadAltiInList(buf,list);
  ret << spis_f[i];
 }

 return ret;
}

/*!
 * \brief ObrIzm::AltiInMap
 * Формирование(добавление) ключевого списка по содержимому ALTI файла(без заголовка файла)
 * в качестве ключа используется дата, время и интервал(мкс)
 * данными являются смещения в области данных buf
 * в процессе работы используется структура zagvrv
 * \param buf[in] область памяти с содержимым ALTI файла без заголовка
 * \param map[in,out] сформированный ключевой список
 * \param dlpak[in] длина пакета в ALTI файле
 * \param kolpak[in] количество пакетов в ALTI файле
 * \return всегда возвращает true ??????????????????????
*/
bool ObrIzm::AltiInMap(QByteArray &buf,QMap<QString,QString> &map,qint32 dlpak,qint64 kolpak)
{
 qint64 ind=0;
 QString str,str1;
 QString spak,schpak;
 qint32 inter;
 QString sinter;

 for (int i=0; i<kolpak; i++)
 {
  ReadZagVRV(buf,ind-6);
  spak=QString::number(zagvrv.npak);
  schpak=QString::number(zagvrv.chpak);
  inter=zagvrv.interv*100000;
  sinter=QString::number(inter);
  sinter=QString("%1").arg(sinter,6,'0');
  str=zagvrv.datn.toString("dd-MM-yy")+" "+zagvrv.timen.toString("hh:mm:ss")+" "+sinter;
  str=str+" "+QString("%1").arg(spak,15,'0')+" "+QString("%1").arg(schpak,10,'0');
  str1=QString::number(ind);
  map.insert(str,str1);
  ind=ind+(dlpak+7);
 }
 return true;
}

bool ObrIzm::ComparePPSI(QString fppsi,QString path_k,QStringList &prot,qreal &max_raz_az,qreal &max_raz_um)
{
 QStringList list_ppsi,list_k;
 QString fk;
 qint32 kolstr;
 QString str,str1,strp;
 QDateTime epoch;
 QString str_k;
 qreal xka,yka,zka;
 qreal xant,yant,zant;
 qreal az,um,ras;
 qreal az_ppsi,um_ppsi;
 qreal raz_az,raz_um;
 qint32 gr,min,ssec;
 qreal sec,dgrad;
 QTime time;
 qint32 ind=0;

 max_raz_az=0; max_raz_um=0;

 xant=2086228.7300;
 yant=2848676.7900;
 zant=5293975.2100;

 prot.clear();

 if (!FilePPSI(fppsi,list_ppsi,fk)) return false;
 kolstr=list_ppsi.count();

 if (path_k == "")
 {
  fk=QFileDialog::getOpenFileName(0,"Open file "+fk,"",fk);
  if (fk == "") return false;
 }
 else fk=path_k+"/"+fk;

 list_k=ReadFileInList(fk);
 if (list_k.isEmpty())
 {
  str=fk+" не найден.";
  prot << str;
  return false;
 }

 str="Обрабатываемый файл КВНО : "+fk;
 prot << str;

 for (int i=0; i<kolstr; i++)
 {
  str=list_ppsi.at(i);
  str1=str.mid(0,19);
  epoch=SetDTFromStr(str1);
  str=str.simplified();
  az_ppsi=str.section(" ",6,6).toDouble();
  um_ppsi=str.section(" ",7,7).toDouble();
  strp=epoch.toString("dd-MM-yyyy hh:mm:ss");
  str=str.section(" ",8,8);
  if (str.mid(0,1) == "=") str=QObject::tr(" планируемое");
  else str=QObject::tr("интерполяция");
  strp=strp+" "+str;
  str=QString(" %1 %2").arg(az_ppsi,10,'L',6).arg(um_ppsi,10,'L',6);
  strp=strp+str;
  time=epoch.time();
  ssec=TimeToSec(time);
  ssec=ssec-10800+18;
  time.setHMS(0,0,0,0);
  time=time.addSecs(ssec);
  epoch.setTime(time);
  if (!FindEpochInKFile(list_k,epoch,str_k,ind)) {prot << strp; continue;}


  str_k=str_k.simplified();
  xka=str_k.section(" ",8,8).toDouble();
  yka=str_k.section(" ",9,9).toDouble();
  zka=str_k.section(" ",10,10).toDouble();
  OGZ(xka,yka,zka,xant,yant,zant,az,um,ras);
  RadToGMS(az,gr,min,sec,dgrad);
  az=dgrad;
  RadToGMS(um,gr,min,sec,dgrad);
  um=dgrad;
  str=epoch.toString("     dd-MM-yyyy hh:mm:ss");
  strp=strp+str;
  str=QString(" %1 %2 %3").arg(az,10,'L',6).arg(um,10,'L',6).arg(ras,10,'L',2);
  strp=strp+str;
  raz_az=qAbs(az_ppsi-az);
  if (raz_az > 270) raz_az=360-raz_az;
  if (raz_az > max_raz_az) max_raz_az=raz_az;
  raz_um=qAbs(um_ppsi-um);
  if (raz_um > max_raz_um) max_raz_um=raz_um;

  DgradToGMS(raz_az,gr,min,sec);
  str=QString(" %1 Г %2 М %3 С").arg(gr,3).arg(min,2).arg(sec,5,'L',2);
  strp=strp+str;
  DgradToGMS(raz_um,gr,min,sec);
  str=QString(" %1 Г %2 М %3 С").arg(gr,3).arg(min,2).arg(sec,5,'L',2);
  strp=strp+str;
  prot << strp;
 }

 DgradToGMS(max_raz_az,gr,min,sec);
 str=QString(" %1 Г %2 М %3 С").arg(gr,3).arg(min,2).arg(sec,5,'L',2);
 strp=str;
 DgradToGMS(max_raz_um,gr,min,sec);
 str=QString(" %1 Г %2 М %3 С").arg(gr,3).arg(min,2).arg(sec,5,'L',2);
 strp=strp+str;
 prot << strp;

 return true;
}

/*
bool ObrIzm::SetEduchFile322(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 int pr;
 QString prsv[3];

 ClearEduch();

 prsv[0]="UTC(USNO)"; prsv[1]="UTC(SU)"; prsv[2]=QObject::tr("ДМВ");

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 str=list[0];
 str=str.simplified();

 zapeduch.npn=str.section(" ",1,1);
 zapeduch.godn=str.section(" ",4,4).toInt();
 zapeduch.sutn=str.section(" ",5,5).toInt();
 zapeduch.sekn=str.section(" ",6,6).toInt();
 zapeduch.nka=str.section(" ",0,0);
 zapeduch.tipis="ДЗС";
 zapeduch.nkis=str.section(" ",2,2);
 pr=str.section(" ",3,3).toInt();
 if (pr > 0) zapeduch.prsv=prsv[pr-1];
 zapeduch.kodobr=0;
 zapeduch.probr=2;

 str=list[kolstr-1];
 str=str.simplified();

 zapeduch.godk=str.section(" ",4,4).toInt();
 zapeduch.sutk=str.section(" ",5,5).toInt();
 zapeduch.sekk=str.section(" ",6,6).toInt();

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);
 zapeduch.strk=0;

 return true;
}
*/

/*
bool ObrIzm::ZapInEduch(QString nfile,qulonglong &nz,bool prud,bool prsoob)
{
 QStringList lf;
 qulonglong nz1;
 QString ext;
 qint32 g;
 QString nazf;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[3];
 ext=ext.toUpper();
 nazf=nazf.toUpper();

 if (ext == "322")
 {
  if (! SetEduchFile322(nfile)) return false;
  nz=InsertEduch(true);
  if (nz == 0) return false;
  nz1=ZapInEdxr(nfile,nz,0,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("educh",nz,prsoob);
   return false;
  }

  UpdateRow("educh","probr",0,nz,prsoob);
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "313")
 {
  if (! SetEduchFile313(nfile)) return false;
  nz=InsertEduch(true);
  if (nz == 0) return false;
  nz1=ZapInEdxr(nfile,nz,0,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("educh",nz,prsoob);
   return false;
  }

  zapstat313.nz_educh=nz;
  InsertStat313(true);

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "SPO")
 {
  if (! SetEduchFileSPO(nfile)) return false;
  nz=InsertEduch(true);
  if (nz == 0) return false;
  nz1=ZapInEdxr(nfile,nz,0,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("educh",nz,prsoob);
   return false;
  }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext.right(1) == "M")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   if (! SetEduchFileM(nfile)) return false;
   nz=InsertEduch(true);
   if (nz == 0) return false;
   nz1=ZapInEdxr(nfile,nz,0,0,true);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("educh",nz,prsoob);
    return false;
   }
   if (prud) DelFile(nfile);
   return true;
  }
 }

 if (ext.right(1) == "O")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   if (! SetEduchFileO(nfile,"БДС O")) return false;
   if (RnxVersion(nfile) == "3.02")
   {
    zapeduch.strk=1;
    zapeduch.kodobr=2;
    if (! ProvRnxFile(nfile,true,true))
    {
     zapeduch.strk=2;
     zapeduch.kodobr=1;
     zapeduch.probr=1;
    }
   }
   nz=InsertEduch(true);
   if (nz == 0) return false;
   nz1=ZapInEdxr(nfile,nz,0,0,true);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("educh",nz,prsoob);
    return false;
   }
   if (prud) DelFile(nfile);
   return true;
  }
 }

 if ((ext.right(1) == "G") || (ext.right(1) == "N"))
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   if (! SetEduchFileNG(nfile,"БДС "+ext.right(1))) return false;
   zapeduch.kodobr=7;
   nz=InsertEduch(true);
   if (nz == 0) return false;
   nz1=ZapInEdxr(nfile,nz,0,0,true);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("educh",nz,prsoob);
    return false;
   }
   if (prud) DelFile(nfile);
   return true;
  }
 }

 if (ext.right(3) == "RNX")
 {
  if (nazf.right(2) == "MO")
  {
   if (! SetEduchFileO(nfile,"БДС MO")) return false;
   if (RnxVersion(nfile) == "3.02")
   {
    zapeduch.strk=1;
    zapeduch.kodobr=2;
    if (! ProvRnxFile(nfile,true,true))
    {
     zapeduch.strk=2;
     zapeduch.kodobr=1;
     zapeduch.probr=1;
    }
   }
   nz=InsertEduch(true);
   if (nz == 0) return false;
   nz1=ZapInEdxr(nfile,nz,0,0,true);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("educh",nz,prsoob);
    return false;
   }
   if (prud) DelFile(nfile);
   return true;
  }
  if (nazf.right(2) == "MN")
  {
   if (! SetEduchFileNG(nfile,"БДС MN")) return false;
   if (RnxVersion(nfile) == "3.02")
   {
    zapeduch.kodobr=7;
   }
   nz=InsertEduch(true);
   if (nz == 0) return false;
   nz1=ZapInEdxr(nfile,nz,0,0,true);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("educh",nz,prsoob);
    return false;
   }
   if (prud) DelFile(nfile);
   return true;
  }
 }

 if (ext.right(1) == "L")
 {
  if (! SetEduchFileL(nfile)) return false;
  nz=InsertEduch(true);
  if (nz == 0) return false;
  nz1=ZapInEdxr(nfile,nz,0,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("educh",nz,prsoob);
   return false;
  }

  if (prud) DelFile(nfile);
  return true;
 }

 if ((ext.right(3) == "BIN") || (ext.right(3) == "126") || (ext.right(3) == "127"))
 {

 }

 return true;
}
*/

/*
qulonglong ObrIzm::ZapInEdxr(QString nfile,qulonglong nz_educh,qulonglong nz_educho,qint16 prarx,bool prsoob)
{
 QByteArray buf;
 int ret;
 qulonglong nz;

 if (! SetEdxr(nfile,nz_educh,nz_educho,prarx,buf,prsoob)) return 0;

 ret=WriteFile(tek_path_arx+"/"+zapedxr.arxf,buf,true);
 if (ret == -1) return 0;

 nz=InsertEdxr(prsoob);

 return nz;
}
*/

/*
qulonglong ObrIzm::ZapInEdxrFromMem(QString nfile, qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, bool prsoob,QByteArray buf)
{
 int ret;
 qulonglong nz;

 if (! SetEdxrFromMem(nfile,nz_educh,nz_educho,prarx,buf,prsoob)) return 0;

 ret=WriteFile(tek_path_arx+"/"+zapedxr.arxf,buf,true);
 if (ret == -1) return 0;

 nz=InsertEdxr(prsoob);

 return nz;
}
*/

bool ObrIzm::SetEdxr(QString nfile,qulonglong nz_educh,qulonglong nz_educho,qint16 prarx,QByteArray &buf,bool prsoob)
{
 qulonglong idlf,adlf;
 qint16 iks,aks;
 QStringList lf;

 if (tek_nz_nos == 0)
 {
  if (! GetNos(prsoob)) return false;
 }

 ClearEdxr();

 if (! ArxFile(nfile,prarx,idlf,iks,adlf,aks,buf)) return false;

 lf=FileInfo(nfile);

 zapedxr.isxf=lf[2];
 zapedxr.arxf=QString::number(nz_educh)+QString::number(nz_educho)+"_"+lf[2];
 zapedxr.arxf=zapedxr.arxf.toUpper();
 zapedxr.idlf=idlf;
 zapedxr.iks=iks;
 zapedxr.adlf=adlf;
 zapedxr.aks=aks;
 zapedxr.datp=QDate::currentDate();
 zapedxr.datr=QDate::currentDate();
 zapedxr.prarx=prarx;
 zapedxr.nz_nos=tek_nz_nos;

 return true;
}

bool ObrIzm::SetEdxrFromMem(QString nfile,qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, QByteArray &buf,bool prsoob)
{
 qulonglong idlf,adlf;
 qint16 iks,aks;
 QStringList lf;

 if (tek_nz_nos == 0)
 {
  if (! GetNos(prsoob)) return false;
 }

 ClearEdxr();

 if (! ArxFileInMem(prarx,idlf,iks,adlf,aks,buf)) return false;

 lf=FileInfo(nfile);

 zapedxr.isxf=lf[2];
 zapedxr.arxf=QString::number(nz_educh)+QString::number(nz_educho)+"_"+lf[2];
 zapedxr.idlf=idlf;
 zapedxr.iks=iks;
 zapedxr.adlf=adlf;
 zapedxr.aks=aks;
 zapedxr.datp=QDate::currentDate();
 zapedxr.datr=QDate::currentDate();
 zapedxr.prarx=prarx;
 zapedxr.nz_nos=tek_nz_nos;

 return true;
}

/*
bool ObrIzm::SetEduchFile313(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 int pr;
 QString prsv[3];

 ClearEduch();

 prsv[0]="UTC(USNO)"; prsv[1]="UTC(SU)"; prsv[2]=QObject::tr("ДМВ");

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 str=list[0];
 str=str.simplified();

 zapeduch.npn=str.section(" ",1,1);
 zapeduch.godn=str.section(" ",4,4).toInt();
 zapeduch.sutn=str.section(" ",5,5).toInt();
 zapeduch.sekn=str.section(" ",6,6).toInt();
 zapeduch.nka=str.section(" ",0,0);
 zapeduch.tipis="ДС";
 zapeduch.nkis=str.section(" ",2,2);
 pr=str.section(" ",3,3).toInt();
 if (pr > 0) zapeduch.prsv=prsv[pr-1];
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 str=list[kolstr-1];
 str=str.simplified();

 zapeduch.godk=str.section(" ",4,4).toInt();
 zapeduch.sutk=str.section(" ",5,5).toInt();
 zapeduch.sekk=str.section(" ",6,6).toInt();

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 Stat313(list);

 return true;
}
*/

/*
bool ObrIzm::SetEduchFileSPO(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDate dat;
 QTime time;
 bool prok=true;

 ClearEduch();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;


 do
 {
  prok=true;
  for (int i=0; i<kolstr; i++)
  {
   str=list[i];
   str=str.simplified();
   if (((str.section(" ",0,0) != "126") && (str.section(" ",0,0) != "127")) || (str.section(" ",2,2) == "0"))
   {
    list.removeAt(i);
    kolstr=list.count();
    prok=false;
    break;
   }
  }
 } while (!prok);

 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 str=list[0];
 str=str.simplified();

 dat=dat.fromString(str.section(" ",3,3),"yyyy-MM-dd");
 time=time.fromString(str.section(" ",4,4).left(8),"hh:mm:ss");

 zapeduch.npn=zapeduch.naz.left(4);

 DateToDay(dat,zapeduch.sutn,zapeduch.godn);

 zapeduch.sekn=TimeToSec(time);
 zapeduch.nka=str.section(" ",0,0);
 zapeduch.tipis="СПО";
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 str=list[kolstr-1];
 str=str.simplified();

 dat=dat.fromString(str.section(" ",3,3),"yyyy-MM-dd");
 time=time.fromString(str.section(" ",4,4).left(8),"hh:mm:ss");

 zapeduch.npn=zapeduch.naz.left(4);

 DateToDay(dat,zapeduch.sutk,zapeduch.godk);

 zapeduch.sekk=TimeToSec(time);

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

/*
bool ObrIzm::SetEduchFileM(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 int ind;
 QDateTime dt;

 ClearEduch();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 ind=FindStrInList(list,"MARKER NAME");
 if (ind > 0)
 {
  str=list[ind];
  zapeduch.npn=str.left(4);
 }
 else zapeduch.npn=zapeduch.naz.mid(0,4);

 ind=FindStrInList(list,"END OF HEADER");
 if (ind == -1) return false;
 ind++;
 if (ind > kolstr) return false;

 str=list[ind];
 str=str.left(18);
 str=str.simplified();
 str="20"+str+".0";
 dt=SetDTFromStr(str);
 DateToDay(dt.date(),zapeduch.sutn,zapeduch.godn);
 zapeduch.sekn=TimeToSec(dt.time());
 zapeduch.tipis="МЕТЕО";
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 str=list[kolstr-1];
 str=str.left(18);
 str=str.simplified();
 str="20"+str+".0";
 dt=SetDTFromStr(str);
 DateToDay(dt.date(),zapeduch.sutk,zapeduch.godk);
 zapeduch.sekk=TimeToSec(dt.time());

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

/*
bool ObrIzm::SetEduchFileO(QString nfile,QString tipis)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDateTime dt;

 ClearEduch();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];
 zapeduch.naz=zapeduch.naz.toUpper();

 zapeduch.npn=zapeduch.naz.left(4);

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 DateToDay(dt.date(),zapeduch.sutn,zapeduch.godn);
 zapeduch.sekn=TimeToSec(dt.time());
 zapeduch.tipis=tipis;
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 DateToDay(dt.date(),zapeduch.sutk,zapeduch.godk);
 zapeduch.sekk=TimeToSec(dt.time());

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

/*
bool ObrIzm::SetEduchFileNG(QString nfile, QString tipis)
{
 QStringList list,list1;
 qlonglong kolstr;
 QStringList lf;
 QDateTime dtn,dtk;
 int kolsut;
 QString ver;

 ClearEduch();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 ver=RnxVersion(list);

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];
 zapeduch.naz=zapeduch.naz.toUpper();

 zapeduch.npn=zapeduch.naz.left(4);

 if (ver == "2.10")
 {
  list1=SetListNFile2(list);
 }
 if (ver == "3.02")
 {
  list1=SetListNFile(list);
 }
 kolstr=list1.count();
 if (kolstr == 0) return false;

 if (ver == "2.10")
 {
  if (! KolSutNFile2(list1,dtn,dtk,kolsut)) return false;
 }
 if (ver == "3.02")
 {
  if (! KolSutNFile(list1,dtn,dtk,kolsut)) return false;
 }

 DateToDay(dtn.date(),zapeduch.sutn,zapeduch.godn);
 zapeduch.sekn=TimeToSec(dtn.time());
 DateToDay(dtk.date(),zapeduch.sutk,zapeduch.godk);
 zapeduch.sekk=TimeToSec(dtk.time());

 zapeduch.tipis=tipis;
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

bool ObrIzm::ObrKod2(qint32 kolobr,QString path_temp,bool prsoob)
{
 qlonglong nz,nz2;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString newname;
 QStringList r_file;

 ClearSlu();

 list=GetKodobr("rnxi",2,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i); nz2=nz;

  if (! ReadRnx("rnxi",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;

  if (!ReadEdxrInList("rnxi",zaprnx.nz,r_file,prsoob)) continue;

  newname=NazOFile(zaprnx.npn,r_file,path_temp);
  if (! WriteFileFromList(r_file,path_temp+"/"+newname)) continue;
  nz=SetRnxFileO(path_temp+"/"+newname,false,prsoob);
  if (nz == 0)
  {
   DelFile(path_temp+"/"+newname);
   continue;
  }
  else
  {
   zapsxema.nz_educh=nz2;
   zapsxema.nz_educho1=nz;
   zapsxema.nz_educho2=0;
   zapsxema.pr_isx=1;
   InsertSxema("sxema1",true);
  }

  UpdateRow("rnxi","kodobr",5,nz2,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

/*
void ObrIzm::SetEducho()
{
 zapeducho.nz=0;
 zapeducho.naz=zapeduch.naz;
 zapeducho.npn=zapeduch.npn;
 zapeducho.godn=zapeduch.godn;
 zapeducho.godk=zapeduch.godk;
 zapeducho.sutn=zapeduch.sutn;
 zapeducho.sutk=zapeduch.sutk;
 zapeducho.sekn=zapeduch.sekn;
 zapeducho.sekk=zapeduch.sekk;
 zapeducho.nka=zapeduch.nka;
 zapeducho.tipis=zapeduch.tipis;
 zapeducho.datn=zapeduch.datn;
 zapeducho.vrn=zapeduch.vrn;
 zapeducho.datk=zapeduch.datk;
 zapeducho.vrk=zapeduch.vrk;
 zapeducho.kodobr=0;
 zapeducho.probr=0;
 zapeducho.strk=0;
 zapeducho.form="";
 zapeducho.prsut=0;
 zapeducho.inter=0;
 return;
}
*/

/*
bool ObrIzm::SetEducho(QString nfile,QStringList &hdr)
{
 qint32 interval;
 QString ver;

 SetEducho();

 hdr=ReadRnxHdr(nfile);
 if (hdr.count() == 0) return false;
 interval=RnxInterval(hdr);
 ver=RnxVersion(hdr);

 zapeducho.form=ver;
 zapeducho.inter=interval;

 return true;
}
*/

/*
void ObrIzm::PrSutFile()
{
 qint32 raz;

 zapeducho.prsut=0;

 raz=zapeducho.godk-zapeducho.godn;
 if (raz > 0) {zapeducho.prsut=2; zapeducho.kodobr=3; zapeducho.probr=0; return;}

 raz=zapeducho.sutk-zapeducho.sutn;
 if (raz > 0) {zapeducho.prsut=2; zapeducho.kodobr=3; zapeducho.probr=0; return;}

 zapeducho.prsut=3;

 raz=(zapeducho.sekk-zapeducho.sekn)+zapeducho.inter;
 if (raz == 86400) zapeducho.prsut=1;

 if (zapeducho.prsut == 0) {zapeducho.kodobr=1; zapeducho.probr=1;}
 if (zapeducho.prsut == 1) {zapeducho.kodobr=4; zapeducho.probr=0;}
 if (zapeducho.prsut == 3) {zapeducho.kodobr=4; zapeducho.probr=0;}

 return;
}
*/

bool ObrIzm::CopyEdxr(QString nfile_arx,QString nfile_vix,qint8 pr,qint16 ks_arx,qint16 ks_vix)
{
 QByteArray buf;

 if (! CopyEdxrInMem(nfile_arx,pr,ks_arx,ks_vix,buf)) return false;

 if (WriteFile(nfile_vix,buf,true) == -1) return false;

 return true;
}

bool ObrIzm::ObrKod3(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString path_arx,path_ftemp;
 qint16 prarx;
 QStringList sp_file;
 qulonglong nzz;
 bool ret;

 ClearSlu();

 list=GetKodobr("rnxv",3,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i);

  if (! ReadRnx("rnxv",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) continue;

  path_arx=readedxr.path_arx+"/"+readedxr.arxf;
  path_ftemp=path_temp+"/"+readedxr.isxf;

  prarx=readedxr.prarx;
  if (prarx == 1) prarx=2;

  ClearDir(path_temp);

  if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) continue;

  if (! RazbAllOFile(path_ftemp,sp_file))
  {
   ClearDir(path_temp);
   continue;
  }
  if (sp_file.count() == 0)
  {
   UpdateRow("rnxv","probr",0,nz,prsoob);
   continue;
  }

  ret=true;
  for (int j=0; j<sp_file.count(); j++)
  {
   nzz=SetRnxFileO(sp_file[j],false,prsoob);
   if (nzz == 0) {ret=false; continue;}

   ClearSxema();
   zapsxema.nz_educh=nz;
   zapsxema.nz_educho1=nzz;
   zapsxema.nz_educho2=0;
   zapsxema.pr_isx=0;
   InsertSxema("sxema1",true);
  }

  if (ret) UpdateRow("rnxv","kodobr",20,nz,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

/*
bool ObrIzm::ZapInEducho(QString nfile,qulonglong &nz,bool prud,QString tipis)
{
 qulonglong nz1;
 QStringList hdr;

 if (RnxVersion(nfile) != "3.02") return false;

 if (! SetEduchFileO(nfile,tipis)) return false;
 if (! SetEducho(nfile,hdr)) return false;
 PrSutFile();

 zapeducho.strk=1;
 if (! ProvRnxFile(nfile,true,true))
 {
  zapeducho.strk=2;
  zapeducho.kodobr=1;
  zapeducho.probr=1;
 }

 nz=InsertEducho(true);
 if (nz == 0) return false;
 nz1=ZapInEdxr(nfile,0,nz,0,true);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("educho",nz,true);
  return false;
 }
 if (prud) DelFile(nfile);

 return true;
}
*/

bool ObrIzm::ObrKod4(qint32 kolobr, QString path_temp,bool prsoob)
{
 qulonglong nz;
 QList<qlonglong> list;
 QList<qlonglong> listo;
 qlonglong kolzap;
 QString path_arx;
 qint16 prarx;
 QStringList sp_file;
 QString str;
 qulonglong nzz;
 int i,j;
 QStringList ob;
 QStringList hdr;
 QString isxf;
 qint32 inter;
 QString path_ftemp;
 bool probr;

 ClearSlu();

 //получаем список единиц хранения для обработки
 list=GetKodobr("rnxv",4,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 //цикл по единицам хранения для обработки
 for (i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i);
  sp_file.clear();
  listo.clear();
  if (! ReadRnx("rnxv",nz,prsoob)) continue;
  if (zaprnx.kodobr != 4) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) continue;

  //получение списка ключей таблицы educho для объединения
  inter=zaprnx.interv;
  listo=GetRawForO(zaprnx.npn,zaprnx.godn,zaprnx.sutn,nz,inter,prsoob);
  if (listo.count() == 0) //если нет единиц хранения для обработки
  {
   UpdateRow("rnxv","kodobr",0,nz,prsoob);
   continue;
  }

  ClearDir(path_temp);

  //подготовка и копирование единицы хранения в директорию для обработки
  path_arx=readedxr.path_arx+"/"+readedxr.arxf;
  path_ftemp=path_temp+"/"+readedxr.isxf;
  path_ftemp=path_temp+"/"+NazOFile(path_ftemp);

  prarx=readedxr.prarx;
  if (prarx == 1) prarx=2;

  if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) continue;

  sp_file << path_ftemp;
  hdr=ReadRnxHdr(path_ftemp);
  isxf=path_ftemp;

  //цикл по ключам для обработки
  probr=true;
  for (j=0; j<listo.count(); j++)
  {
   nzz=listo.at(j);
   if (! ReadRnx("rnxv",nzz,prsoob)) {probr=false; continue;}
   if (!InsertObr(komp_name,zaprnx.nz_edxr)) {probr=false; continue;}
   if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) {probr=false; continue;}

   path_arx=readedxr.path_arx+"/"+readedxr.arxf;
   path_ftemp=path_temp+"/"+readedxr.isxf;
   path_ftemp=path_temp+"/"+NazOFile(path_ftemp);

   prarx=readedxr.prarx;
   if (prarx == 1) prarx=2;

   if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) {probr=false; continue;}

   sp_file << path_ftemp;
  }

  if (!probr)
  {
   ClearDir(path_temp);
   continue;
  }

  ob=ObOFile(sp_file);
  if (ob.count() == 0)
  {
   ClearDir(path_temp);
   continue;
  }

  for (int j=0; j<sp_file.count(); j++)
  {
   str=sp_file.at(j);
   DelFile(str);
  }

  AddList(hdr,ob);
  ob=SpisRnxDataDesc(hdr);
  IsprRnxDTObs(hdr,ob);
  path_ftemp=path_temp+"/"+NazOFile(path_ftemp);
  ZapSputnInMas(hdr);
  UpdatePrnOfObs(hdr);

  if (! WriteFileFromList(hdr,path_ftemp))
  {
   ClearDir(path_temp);
   continue;
  }

  nzz=SetRnxFileO(path_ftemp,true,prsoob);
  if (nzz == 0) {DelFile(path_ftemp); continue;}

  ClearSxema();
  zapsxema.nz_educh=nz;
  zapsxema.nz_educho1=nzz;
  zapsxema.nz_educho2=0;
  InsertSxema("sxema1",true);
  UpdateRow("rnxv","kodobr",20,nz,prsoob);

  for (j=0; j<listo.count(); j++)
  {
   nzz=listo.at(j);
   ClearSxema();
   zapsxema.nz_educh=nz;
   zapsxema.nz_educho1=nzz;
   zapsxema.nz_educho2=0;
   InsertSxema("sxema1",true);
   UpdateRow("rnxv","kodobr",20,nzz,prsoob);
  }

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

/*
bool ObrIzm::ObrKod2M(bool prsoob)
{
 qlonglong nz,nz1,nz2;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString path_arx,path_temp;
 QStringList hdr;
 QString newname;
 qint16 prarx;
 QByteArray buf;

 list=GetKodobr("educh",2,0);
 kolzap=list.count();
 if (kolzap == 0) return true;

 for (int i=0; i < kolzap; i++)
 {
  nz=list.at(i);

  if (! ReadEduch(nz,prsoob)) return false;
  if (zapeduch.strk == 2) continue;
  if (zapeduch.probr != 0) continue;
  if (zapeduch.kodobr != 2) continue;
  UpdateRow("educh","probr",2,nz,prsoob);

  if (! ReadEdxrForEduch(1,nz))
  {
   UpdateRow("educh","probr",0,nz,prsoob);
   return false;
  }

  path_arx=zapnos.naz+"/"+zapedxr.arxf;
  SetEducho(path_arx,hdr);
  PrSutFile();
  newname=NazOFile(zapeduch.npn,hdr,zapnos.naz);
  path_temp=zapnos.naz+"/"+newname;

  prarx=zapedxr.prarx;
  if (prarx == 1) prarx=2;

  if (! CopyEdxrInMem(path_arx,prarx,zapedxr.aks,zapedxr.iks,buf))
  {
   UpdateRow("educh","probr",0,nz,prsoob);
   continue;
  }

  zapeducho.naz=newname.section(".",0,0);
  zapeducho.npn=newname.mid(0,4);
  zapeducho.tipis="БДС O";

  nz1=InsertEducho(true);
  if (nz1 == 0)
  {
   DelFile(path_temp);
   UpdateRow("educh","probr",0,nz,prsoob);
   continue;
  }

  nz2=ZapInEdxrFromMem(path_temp,0,nz1,0,true,buf);
  if (nz2 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DelFile(path_temp);
   DeleteRow("educho",nz1,prsoob);
   UpdateRow("educh","probr",0,nz,prsoob);
   continue;
  }

  UpdateRow("educh","probr",1,nz,prsoob);
  UpdateRow("educh","kodobr",5,nz,prsoob);

  ClearSxema();
  zapsxema.nz_educh=nz;
  zapsxema.nz_educho1=nz1;
  InsertSxema("sxema",true);

  DelFile(path_temp);
  qApp->processEvents();
 }

 return true;
}
*/

bool ObrIzm::CopyEdxrInMem(QString nfile_arx,qint8 pr,qint16 ks_arx,qint16 ks_vix,QByteArray &buf)
{
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;

 if (! ArxFile(nfile_arx,pr,idlf,iks,adlf,aks,buf)) return false;

 if (ks_arx != iks) return false;
 if (ks_vix != aks) return false;

 return true;
}

bool ObrIzm::ObrKod7(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz,nz2;
 QList<qlonglong> list;
 qlonglong kolzap,kolzap1;
 QString path_arx,path_ftemp;
 QStringList sp_file;
 qint16 prarx;
 QString str;
 bool pr_err;

 ClearSlu();

 list=GetKodobr("rnxi",7,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i);
  pr_err=false;

  if (! ReadRnx("rnxi",nz,prsoob)) continue;
  if (zaprnx.kodobr != 7) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxi",zaprnx.nz,prsoob)) continue;

  path_arx=readedxr.path_arx+"/"+readedxr.arxf;
  path_ftemp=path_temp+"/"+readedxr.isxf;

  prarx=readedxr.prarx;
  if (prarx == 1) prarx=2;

  ClearDir(path_temp);

  if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) continue;

  if (! RazdNFileToNG(path_ftemp,zaprnx.npn,sp_file)) continue;

  kolzap1=sp_file.count();
  if (kolzap1 == 0) continue;

  pr_err=false;
  for (int j=0; j<kolzap1; j++)
  {
   str=sp_file.at(j);
   nz2=SetRnxFileNG(str,str.right(1),false,prsoob);
   if (nz2 == 0) {pr_err=true; continue;}
   ClearSxema();
   zapsxema.nz_educh=nz;
   zapsxema.nz_educho1=nz2;
   zapsxema.pr_isx=1;
   InsertSxema("sxema1",false);
   DelFile(str);
  }

  if (!pr_err) UpdateRow("rnxi","kodobr",5,nz,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

/*
bool ObrIzm::SetEduchoNFile(QString nfile)
{
 QStringList list,list1;
 QDateTime dtn,dtk;
 int kolsut;
 quint32 day;
 quint16 year;
 QStringList fi;
 QString str;

 SetEducho();

 list=ReadFileInList(nfile);
 if (list.isEmpty()) return false;

 list1=SetListNFile(list);
 if (list1.isEmpty()) return false;

 if (! KolSutNFile(list1,dtn,dtk,kolsut)) return false;

 DateToDay(dtn.date(),day,year);
 zapeducho.godn=year;
 zapeducho.sutn=day;
 zapeducho.sekn=TimeToSec(dtn.time());
 zapeducho.datn=dtn.date();
 zapeducho.vrn=dtn.time();

 DateToDay(dtk.date(),day,year);
 zapeducho.godk=year;
 zapeducho.sutk=day;
 zapeducho.sekk=TimeToSec(dtk.time());
 zapeducho.datk=dtk.date();
 zapeducho.vrk=dtk.time();

 zapeducho.form=RnxVersion(list);

 zapeducho.kodobr=9;
 if (kolsut > 1) zapeducho.kodobr=8;

 fi=FileInfo(nfile);
 zapeducho.naz=fi[3];

 str=fi[4];
 zapeducho.tipis="БДС "+str.right(1);

 return true;
}
*/

bool ObrIzm::ObrKod8(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz,nzz;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString path_arx,path_ftemp;
 qint16 prarx;
 QStringList sp_file;
 bool ret;

 ClearSlu();

 list=GetKodobr("rnxv",8,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i);

  if (! ReadRnx("rnxv",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) continue;

  path_arx=readedxr.path_arx+"/"+readedxr.arxf;
  path_ftemp=path_temp+"/"+readedxr.isxf;

  prarx=readedxr.prarx;
  if (prarx == 1) prarx=2;

  ClearDir(path_temp);

  if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) continue;

  sp_file=RazbNFile(path_ftemp);
  if (sp_file.count() == 0)
  {
   UpdateRow("rnxv","probr",0,nz,prsoob);
   continue;
  }

  ret=true;
  for (int j=0; j<sp_file.count(); j++)
  {
   nzz=SetRnxFileNG(sp_file[j],zaprnx.tip,false,prsoob);
   if (nzz == 0) {ret=false; continue;}

   ClearSxema();
   zapsxema.nz_educh=nz;
   zapsxema.nz_educho1=nzz;
   InsertSxema("sxema1",true);
  }

  if (ret) UpdateRow("rnxv","kodobr",20,nz,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

bool ObrIzm::RemoveEdxr(qlonglong nz,bool prsoob)
{
 QString nfile;

 if (!ReadEdxr(nz,prsoob)) return false;
 if (!ReadNos(zapedxr.nz_nos,prsoob)) return false;

 nfile=zapnos.naz+"/"+zapedxr.arxf;
 DelFile(nfile);

 if (!DeleteRow("edxr",nz,prsoob)) return false;

 return true;
}

/*
bool ObrIzm::RemoveEducho(qlonglong nz)
{
 QString qstr;
 QList<qlonglong> list_nz;
 QSqlQuery query;
 qlonglong nz_edxr;

 qstr="SELECT nz FROM edxr WHERE nz_educho = "+QString::number(nz)+";";

 list_nz=ExecQueryList(query,qstr,true);

 for(int i=0; i<list_nz.count(); i++)
 {
  nz_edxr=list_nz.at(i);
  if (!RemoveEdxr(nz_edxr,false)) return false;
 }

 if (!DeleteRow("educho",nz,true)) return false;

 return true;
}
*/

bool ObrIzm::ObrKod6(bool prsoob)
{
 qlonglong nz;
 QList<qlonglong> list;
 qlonglong kolzap;

 ClearSlu();

 list=GetKodobr("dds",6,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 for (int i=0; i < kolzap; i++)
 {
  nz=list.at(i);
  RemoveDDS(nz,prsoob);
 }

 return true;
}

bool ObrIzm::ObrKod9(qint32 kolobr, QString path_temp,bool prsoob)
{
 qulonglong nz;
 QList<qlonglong> list;
 QList<qlonglong> listo;
 qlonglong kolzap;
 QString path_arx,path_ftemp;
 qint16 prarx;
 QStringList sp_file;
 qulonglong nzz;
 int i,j;
 QString nfile;
 bool probr;

 ClearSlu();

 //получаем список единиц хранения для обработки
 list=GetKodobr("rnxv",9,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 //цикл по единицам хранения для обработки
 for (i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i);
  sp_file.clear();
  listo.clear();
  if (! ReadRnx("rnxv",nz,prsoob)) continue;
  if (zaprnx.kodobr != 9) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) continue;

  //получение списка ключей таблицы educho для объединения
//  inter=zaprnx.interv;
  listo=GetRawForN(zaprnx.godn,zaprnx.sutn,nz,zaprnx.tip,prsoob);
//  if (listo.count() == 0) //если нет единиц хранения для обработки
//  {
//   UpdateRow("rnxv","kodobr",0,nz);
//   continue;
//  }

  ClearDir(path_temp);

  //подготовка и копирование единицы хранения в директорию для обработки
  path_arx=readedxr.path_arx+"/"+readedxr.arxf;
  path_ftemp=path_temp+"/"+readedxr.isxf;
  path_ftemp=path_temp+"/"+NazOFile(path_ftemp);

  prarx=readedxr.prarx;
  if (prarx == 1) prarx=2;

  if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) continue;

  sp_file << path_ftemp;
//  hdr=ReadRnxHdr(path_ftemp);
//  isxf=path_ftemp;

  //цикл по ключам для обработки
  probr=true;
  for (j=0; j<listo.count(); j++)
  {
   nzz=listo.at(j);
   if (! ReadRnx("rnxv",nzz,prsoob)) {probr=false; continue;}
   if (!InsertObr(komp_name,zaprnx.nz_edxr)) {probr=false; continue;}
   if (!ReadEdxr("rnxv",zaprnx.nz,prsoob)) {probr=false; continue;}

   path_arx=readedxr.path_arx+"/"+readedxr.arxf;
   path_ftemp=path_temp+"/"+readedxr.isxf;
   path_ftemp=path_temp+"/"+NazOFile(path_ftemp);

   prarx=readedxr.prarx;
   if (prarx == 1) prarx=2;

   if (! CopyEdxr(path_arx,path_ftemp,prarx,readedxr.aks,readedxr.iks)) {probr=false; continue;}

   sp_file << path_ftemp;
  }

  if (!probr)
  {
   ClearDir(path_temp);
   continue;
  }

  if (!ObNFile(sp_file,nfile))
  {
   UpdateRow("rnxv","kodobr",1,nz,prsoob);
   UpdateRow("rnxv","strk",3,nz,prsoob);
   ClearDir(path_temp);
   continue;
  }

  nzz=SetRnxFileNG(nfile,zaprnx.tip,true,prsoob);
  if (nzz == 0) {DelFile(nfile); continue;}

  ClearSxema();
  zapsxema.nz_educh=nz;
  zapsxema.nz_educho1=nzz;
  InsertSxema("sxema1",true);
  UpdateRow("rnxv","kodobr",20,nz,prsoob);

  for (j=0; j<listo.count(); j++)
  {
   nzz=listo.at(j);
   ClearSxema();
   zapsxema.nz_educh=nz;
   zapsxema.nz_educho1=nzz;
   InsertSxema("sxema1",true);
   UpdateRow("rnxv","kodobr",20,nzz,prsoob);
  }

 }

   qApp->processEvents();

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

/*
bool ObrIzm::SetEduchFileL(QString nfile)
{
 QStringList lf;

 ClearEduch();

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 zapeduch.npn=zapeduch.naz.mid(0,4);
 zapeduch.godn=18;
 zapeduch.sutn=zapeduch.naz.mid(4,3).toInt();
 zapeduch.sekn=0;
 zapeduch.nka="";
 zapeduch.tipis="БДС L";
 zapeduch.nkis="";
 zapeduch.prsv="";
 zapeduch.kodobr=10;
 zapeduch.probr=0;
 zapeduch.godk=18;
 zapeduch.sutk=zapeduch.naz.mid(4,3).toInt();
 zapeduch.sekk=0;

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

bool ObrIzm::ObrKod10(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz,nz2;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString newname,nfile;
 QFile file;
 QStringList rnx;
 QStringList lf;
 QByteArray buf;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;
 QStringList listd;

 ClearSlu();

 list=GetKodobr("rnxi",10,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i); nz2=nz;

  if (! ReadRnx("rnxi",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxi",zaprnx.nz,prsoob)) continue;

  nfile=readedxr.path_arx+"/"+readedxr.arxf;
  if (readedxr.prarx == 1) readedxr.prarx=2;
  if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) continue;
  if (readedxr.aks != aks) continue;
  nfile=path_temp+"/"+readedxr.isxf;
  if (WriteFile(nfile,buf,true) == -1)continue;
  buf.clear();

  lf=FileInfo(nfile);
  newname=nfile;
  newname=newname.replace(newname.length()-1,1,"O");
  newname=NazOFile(newname);
  newname=lf[1]+"/"+newname;

  QFile *JPS = new QFile(nfile);
  QFile *RINEX = new QFile(newname);
  convJps->getDopplerStatus(true);
  convJps->getInf("CGTR 1.05", "RKS", "001", "002", "SN0001", "RKS T1", "T1.001", "SN0001", "RKS A1");
  newname=convJps->convertJpsToRinex (JPS,RINEX);
//  converterJps::convertJpsToRinex (JPS,RINEX);

  DelFile(nfile);

  file.setFileName(newname);
  if (!file.exists()) return false;

  rnx=ReadFileInList(newname);
  listd=SpisRnxDataDesc(rnx);
  IsprRnxDTObs(rnx,listd);
  UpdatePrnOfObs(rnx);
  WriteFileFromList(rnx,newname);

  ClearSxema();

  nz=SetRnxFileO(newname,false,prsoob);
  if (nz == 0) return false;
  ClearSxema();
  zapsxema.nz_educh=nz2;
  zapsxema.nz_educho1=nz;
  zapsxema.nz_educho2=0;
  zapsxema.pr_isx=1;
  InsertSxema("sxema1",prsoob);

  newname=newname.replace(newname.length()-1,1,"G");
  file.setFileName(newname);
  if (file.exists())
  {
   nz=SetRnxFileNG(newname,"G",false,prsoob);
   if (nz != 0)
   {
    ClearSxema();
    zapsxema.nz_educh=nz2;
    zapsxema.nz_educho1=nz;
    zapsxema.nz_educho2=0;
    zapsxema.pr_isx=1;
    InsertSxema("sxema1",prsoob);
   }
  }

  newname=newname.replace(newname.length()-1,1,"N");
  file.setFileName(newname);
  if (file.exists())
  {
   nz=SetRnxFileNG(newname,"N",false,prsoob);
   if (nz != 0)
   {
    zapsxema.nz_educh=nz2;
    zapsxema.nz_educho1=nz;
    zapsxema.nz_educho2=0;
    zapsxema.pr_isx=1;
    InsertSxema("sxema1",prsoob);
   }
  }


  UpdateRow("rnxi","kodobr",5,nz2,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

/*
bool ObrIzm::SetEduchFile313(QStringList list,QString nfile)
{
 qlonglong kolstr;
 QString str;
 QStringList lf;
 int pr;
 QString prsv[3];

 ClearEduch();

 prsv[0]="UTC(USNO)"; prsv[1]="UTC(SU)"; prsv[2]=QObject::tr("ДМВ");

 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapeduch.naz=lf[3];

 str=list[0];
 str=str.simplified();

 zapeduch.npn=str.section(" ",1,1);
 zapeduch.godn=str.section(" ",4,4).toInt();
 zapeduch.sutn=str.section(" ",5,5).toInt();
 zapeduch.sekn=str.section(" ",6,6).toInt();
 zapeduch.nka=str.section(" ",0,0);
 zapeduch.tipis="ДС";
 zapeduch.nkis=str.section(" ",2,2);
 pr=str.section(" ",3,3).toInt();
 if (pr > 0) zapeduch.prsv=prsv[pr-1];
 zapeduch.kodobr=0;
 zapeduch.probr=0;

 str=list[kolstr-1];
 str=str.simplified();

 zapeduch.godk=str.section(" ",4,4).toInt();
 zapeduch.sutk=str.section(" ",5,5).toInt();
 zapeduch.sekk=str.section(" ",6,6).toInt();

 zapeduch.datn=DayToDate(zapeduch.sutn,zapeduch.godn);
 zapeduch.datk=DayToDate(zapeduch.sutk,zapeduch.godk);

 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.vrn=zapeduch.vrn.addSecs(zapeduch.sekn);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.vrk=zapeduch.vrk.addSecs(zapeduch.sekk);

 zapeduch.strk=0;

 return true;
}
*/

bool ObrIzm::Stat313(QStringList list)
{
 qlonglong kolstr;
 QString str;
 QList<int> shf1;
 QList<int> shf2;
 qint32 ind_n,ind_k;
 bool df1,df2;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ClearStat313();
 zapstat313.kizm=kolstr;
 zapstat313.dlsek=kolstr;
 ind_n=0; ind_k=kolstr-1;

 if (!Seans313(list,ind_n,ind_k))
 {
  zapstat313.kizm=0;
  return true;
 }

 zapstat313.kizm=ind_k-ind_n+1;

 for (int i=ind_n; i<ind_k; i++)
 {
  str=list.at(i);

  if (!DostIzm313(str,0,df1,df2))
  {
   zapstat313.kizmn++;
   if (!df1) zapstat313.kizmnf1++;
   if (!df2) zapstat313.kizmnf2++;
   continue;
  }

  str=str.simplified();

  shf1 << str.section(" ",12,12).toInt();
  shf2 << str.section(" ",13,13).toInt();
 }

 zapstat313.dlsekn=zapstat313.kizmn;

 if (shf1.isEmpty()) return true;
 if (shf2.isEmpty()) return true;

 qSort(shf1);
 qSort(shf2);

 kolstr=shf1.count();
 zapstat313.shf1min=shf1[0];
 zapstat313.shf1max=shf1[kolstr-1];
 zapstat313.shf1med=shf1[kolstr/2];

 kolstr=shf2.count();
 zapstat313.shf2min=shf2[0];
 zapstat313.shf2max=shf2[kolstr-1];
 zapstat313.shf2med=shf2[kolstr/2];

 return true;
}

bool ObrIzm::StatDDSInBD(QString nfile,bool prsoob)
{
 QStringList list,stat;
 qlonglong kolstr;
 qint32 kol1;
 qint32 kol2;
 qint32 kol3;
 qint32 kolep;
 QDateTime dt;
 QString str,str1;
 int ind;
 quint64 nz,nz1;

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,">");
 if (ind == -1) return false;
 str=list.at(ind);
 dt=DTForHdrDatRnx(str);
 if (!dt.isValid()) return false;

 if (!StatEpochForDDS(list,kol1,kol2,kol3,kolep)) return false;

 ClearStatDDS();

 zapstatdds.dat=dt.date();
 zapstatdds.eppl=8639;
 zapstatdds.eppol=kolep;
 zapstatdds.ep_4mglo=kol1;
 zapstatdds.ep_4rglo=kol2;
 zapstatdds.ep_4bglo=kol3;

 nz=InsertStatDDS(true);
 if (nz == 0) return false;

 stat=StatDDS(list);
 kolstr=stat.count();

 for (int i=0; i<kolstr; i++)
 {
  ClearStatrDDS();
  str=stat.at(i);
  str=str.simplified();

  zapstatrdds.nz_statdds=nz;
  str1=str.section(",",1,1);
  zapstatrdds.vrn=QTime::fromString(str1);
  str1=str.section(",",3,3);
  zapstatrdds.vrr=QTime::fromString(str1);
  str1=str.section(",",2,2);
  zapstatrdds.kolep=str1.toInt();

  nz1=InsertStatrDDS(true);
  if (nz1 == 0)
  {
   DeleteRow("statdds",nz,prsoob);
   return false;
  }
 }



 return true;
}

/*
bool ObrIzm::StatPPSIInBD(QString nfile_dds,QString nfile_vrv)
{
 QStringList list;
 qlonglong kolstr;
 QDateTime dtn,dtk,dt;
 QString str;
 int ind;
 QTextCodec* codec;
 QByteArray arr;
 qulonglong nz;

 codec=QTextCodec::codecForName("CP1251");

 arr=ReadFile(nfile_dds);
 str=codec->toUnicode(arr);
 list=str.split("\r\n");
 arr.clear();
 kolstr=list.count();
 if (kolstr == 0) return false;

 ClearStatPPSI();
 ind=FindStrInList(list,"Файл исключён из обработки.");
 if (ind >= 0)
 {
  list=FileInfo(nfile_dds);
  str=list[2];
  ind=str.lastIndexOf(".");
  str=str.mid(0,ind);
  ind=str.lastIndexOf(".");
  str=str.mid(0,ind);
  zapstatppsi.naz=str;
  str=zapstatppsi.naz.mid(18,1);
  zapstatppsi.kan=str.toInt();
  zapstatppsi.k_dds=1;
  zapstatppsi.k_vrv=1;
  nz=InsertStatPPSI(true);
  if (nz == 0)
      nz=0;
  return true;
 }
 dtn=zapstatppsi.dtn_dds;
 dtk=zapstatppsi.dtk_dds;

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str=QObject::tr(str.toStdString().c_str());
  ind=str.indexOf(">");
  if (ind == -1) continue;
  str=str.mid(ind);
  dt=DTForHdrDatRnx(str);
  if (dtn.isNull()) {dtn=dt; dtk=dt;}
  if (dtn.date().year() == 1900) {dtn=dt; dtk=dt;}
  if (dt < dtn) dtn=dt;
  if (dt > dtk) dtk=dt;
 }


 zapstatppsi.dtn_dds=dtn;
 zapstatppsi.dtk_dds=dtk;


 ind=FindStrInList(list,"Всего обработано кадров");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.k_dds=str.toInt();
 }

 ind=FindStrInList(list,"Пустых кадров");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.pk_dds=str.toInt();
 }

 ind=FindStrInList(list,"Отбраковано по КС");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.oks_dds=str.toInt();
 }

 ind=FindStrInList(list,"Отбраковано из-за ошибок данных");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.och_dds=str.toInt();
 }

 ind=FindStrInList(list,"Успешно обработано");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.obr_dds=str.toInt();
 }

 list=FileInfo(nfile_dds);
 str=list[2];
 ind=str.lastIndexOf(".");
 str=str.mid(0,ind);
 ind=str.lastIndexOf(".");
 str=str.mid(0,ind);
 zapstatppsi.naz=str;
 str=zapstatppsi.naz.mid(8,8);
 if(QDate::fromString(str,"dd.MM.yy").isValid()) zapstatppsi.dat=QDate::fromString(str,"dd.MM.yy");
 str=zapstatppsi.naz.mid(21,5);
 if(QTime::fromString(str,"hh.mm").isValid()) zapstatppsi.vr=QTime::fromString(str,"hh.mm");
 str=zapstatppsi.naz.mid(18,1);
 zapstatppsi.kan=str.toInt();


 arr=ReadFile(nfile_vrv);
 str=codec->toUnicode(arr);
 list=str.split("\r\n");
 arr.clear();
 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,"Всего кадров ВРВ");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  str=str.simplified();
  str=str.section(" ",0,0);
  zapstatppsi.k_vrv=str.toInt();
 }

 ind=FindStrInList(list,"Успешно обработано кадров ALTI");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.obr_vrv=str.toInt();
 }

 ind=FindStrInList(list,"Успешно обработано кадров CALL_1");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.obr_vrv=zapstatppsi.obr_vrv+str.toInt();
 }

 ind=FindStrInList(list,"Успешно обработано кадров CALL_2");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.obr_vrv=zapstatppsi.obr_vrv+str.toInt();
 }
 zapstatppsi.obr_vrv=zapstatppsi.obr_vrv*2;

 ind=FindStrInList(list,"Отбраковано по КС");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.och_vrv=str.toInt();
 }

 ind=FindStrInList(list,"Отбраковано из-за ошибок");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  str=str.section(":",1,1);
  zapstatppsi.och_vrv=zapstatppsi.och_vrv+str.toInt();
 }
 zapstatppsi.och_vrv=zapstatppsi.och_vrv*2;

 ind=FindStrInList(list,"Дата и время начала измерений ALTI");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  ind=str.indexOf(":");
  str=str.mid(ind+1);
  str=str.simplified();
  if (QDateTime::fromString(str,"dd.MM.yy hh:mm:ss").isValid()) zapstatppsi.dtn_vrv=QDateTime::fromString(str,"dd.MM.yy hh:mm:ss");
 }

 ind=FindStrInList(list,"Дата и время конца измерений ALTI");
 if (ind >= 0)
 {
  str=list.at(ind);
  str=str.simplified();
  ind=str.indexOf(":");
  str=str.mid(ind+1);
  str=str.simplified();
  if (QDateTime::fromString(str,"dd.MM.yy hh:mm:ss").isValid()) zapstatppsi.dtk_vrv=QDateTime::fromString(str,"dd.MM.yy hh:mm:ss");
 }

 nz=InsertStatPPSI(true);
 if (nz == 0)
     nz=0;

 return true;
}
*/

void ObrIzm::XYZtoBLH(qreal x,qreal y,qreal z,qreal &b,qreal &l,qreal &h)
{
 qreal a,e2,d,b1,b2,delb,delh,n,h1,h2,sinb,e,f;
 bool prtr;

 //ПЗ-90
 a=6378136;
 f=298.257839303;
 e2=(2/f)-(1/qPow(f,2));

 d=qSqrt(qPow(x,2)+qPow(y,2));

 l=qAtan(y/x);
 if (l < 0) l=l+(2*PI);

 b1=qAtan(z/d); h1=0; prtr=true;

 while(prtr)
 {
  sinb=qSin(b1);
  n=a/(qSqrt(1-e2*qPow(sinb,2)));
  e=e2/(1+h1/n);
  b2=qAtan(z/(d*(1-e)));
  h2=d/qCos(b2)-n;

  delb=qAbs(b2-b1); delh=qAbs(h2-h1);

  if (delh < 0.01) prtr=false;

  b1=b2; h1=h2;
 }

 b=b1;
 h=h1;

 return;
}

void ObrIzm::RadToGMS(qreal rad,qint32 &gr,qint32 &min,qreal &sec,qreal &dgrad)
{
 dgrad=qRadiansToDegrees(rad);
 DgradToGMS(dgrad,gr,min,sec);
 return;
}

void ObrIzm::OGZ(qreal xka,qreal yka,qreal zka,qreal xnpn,qreal ynpn,qreal znpn,qreal &az,qreal &um,qreal &ras)
{
 qreal dx,dy,dz;
 qreal x2,y2,z2;
 qreal b,l,h;
 qreal r;

 az=0; um=0; ras=0;

 XYZtoBLH(xnpn,ynpn,znpn,b,l,h);

 dx=xka-xnpn;
 dy=yka-ynpn;
 dz=zka-znpn;

 x2=-dx*qSin(b)*qCos(l) - dy*qSin(b)*qSin(l) + dz*qCos(b);
 y2=-dx*qSin(l) + dy*qCos(l);
 z2=dx*qCos(b)*qCos(l) + dy*qCos(b)*qSin(l) + dz*qSin(b);

 if ((x2 == 0) && (y2 > 0)) az=PI/2;
 if ((x2 == 0) && (y2 < 0)) az=PI*3/2;
 if (x2 != 0) az=qAtan(y2/x2);

 if ((x2 < 0) && (y2 > 0)) az=PI+az;
 if ((x2 < 0) && (y2 <0)) az=PI+az;
 if ((x2 > 0) && (y2 < 0)) az=2*PI+az;

 r=qSqrt(qPow(y2,2)+qPow(x2,2));
 um=qAtan(z2/r);

 ras=qSqrt(qPow(dx,2)+qPow(dy,2)+qPow(dz,2));

 return;
}

bool ObrIzm::FilePPSI(QString nfile,QStringList &list,QString &nkfile)
{
 QStringList isxl;
 qint32 kolstr;
 QString str,str1,str2,str3,str4;
 QDate dat;
 QTime t1,t2;
 qint32 ksec;
 qreal az1,az2,ugm1,ugm2,delaz,delugm;

 list.clear();

 isxl=ReadFileInList(nfile);
 if (isxl.isEmpty()) return false;

 kolstr=isxl.count();
 if (kolstr < 3) return false;

 str=isxl.at(1);
 str=str.simplified();
 str=str.section(" ",0,0);
 dat=dat.fromString(str,"dd.MM.yyyy");

 for (int i=2; i<kolstr-1; i++)
 {
  str1=isxl.at(i);
  str2=isxl.at(i+1);

  str1=str1.simplified();
  str2=str2.simplified();

  str=str1.section(" ",0,0);
  t1=t1.fromString(str,"hh:mm:ss");
  str=str2.section(" ",0,0);
  t2=t2.fromString(str,"hh:mm:ss");
  ksec=t1.secsTo(t2);


  str=dat.toString("yyyy MM dd")+" "+t1.toString("hh mm ss")+" "+str1.section(" ",1,1)+" "+str1.section(" ",2,2)+" ==========";
  list << str;

  str=str1.section(" ",1,1);
  az1=str.toDouble();
  str=str1.section(" ",2,2);
  ugm1=str.toDouble();
  str=str2.section(" ",1,1);
  az2=str.toDouble();
  str=str2.section(" ",2,2);
  ugm2=str.toDouble();

  delaz=az2-az1;
  if (delaz > 270)
      delaz=az2-(az1+360);
  if (delaz < -270)
      delaz=(az2+360)-az1;
  delugm=ugm2-ugm1;

  delaz=delaz/ksec;
  delugm=delugm/ksec;

  for (int j=0; j<ksec-1; j++)
  {
   t1=t1.addSecs(1);
   az1=az1+delaz;
   if (az1 < 0) az1=360+az1;
   if (az1 > 360) az1=az1-360;
   str3=QString(" %1").arg(az1,10,'L',6);
   ugm1=ugm1+delugm;
   str4=QString(" %1").arg(ugm1,10,'L',6);
   str=dat.toString("yyyy MM dd")+" "+t1.toString("hh mm ss")+str3+str4+" ----------";
   list << str;
  }
 }

 str=dat.toString("yyyy MM dd")+" "+t1.toString("hh mm ss")+" "+str2.section(" ",1,1)+" "+str2.section(" ",2,2)+" ==========";
 list << str;

 nkfile="126";
 str=QString("%1").arg(dat.dayOfYear(),3);
 nkfile=nkfile+str+"0."+dat.toString("yy")+"K";

 return true;
}

bool ObrIzm::FindEpochInKFile(QStringList list,QDateTime epoch,QString &str_epoch,qint32 &ind)
{
 qint32 kolstr;
 QString str;
 QDateTime kepoch;

 kolstr=list.count();
 if (kolstr == 0) return false;

 if (ind < 0) ind=0;
 if (ind >= kolstr) ind=0;

 for (int i=ind; i<kolstr; i++)
 {
  str_epoch=list.at(i);
  str=str_epoch.mid(0,19);
  kepoch=SetDTFromStr(str);
  if (kepoch == epoch) {ind=i; return true;}
  if (kepoch > epoch) return false;
 }

 return false;
}

void ObrIzm::DgradToGMS(qreal dgrad, qint32 &gr, qint32 &min, qreal &sec)
{
 gr=dgrad;
 dgrad=(dgrad-gr)*60;
 min=dgrad;
 sec=(dgrad-min)*60;
 return;
}

/*!
 * \brief ObrIzm::CrcKadrFULL
 * Расчёт и проверка КС кадра FULL файла
 * \param buf[in] область памяти с содержимым FULL файла
 * \param ind[in] смещение в области памяти на начало кадра
 * \return для правильной КС возвращает - true, иначе - false.
*/
bool ObrIzm::CrcKadrFULL(QByteArray &buf,qint64 ind)
{
 QByteArray kadr,zn_crc;
 char* ptr;
 unsigned short crc;

 kadr=buf.mid(ind,576);
 ptr=kadr.data();
 crc=Crc16((unsigned char*)ptr,518);
 zn_crc=buf.mid(ind+518,2);
 ptr=zn_crc.data();
 memcpy(bt2.byte,ptr,2);
 bt2.byte2=qFromBigEndian(bt2.byte2);
 if (bt2.byte2 != crc) return false;
 return true;
}

/*!
 * \brief ObrIzm::ReadGlZagKadrFULL
 * Чтение главного заголовка кадра FULL файла и заполнение структуры glzag
 * \param buf[in] область памяти с содержимым FULL файла
 * \param ind[in] смещение в области памяти на начало кадра
*/
void ObrIzm::ReadGlZagKadrFULL(QByteArray &buf,qint64 ind)
{
 QByteArray zag;
 char* ptr;

 zag=buf.mid(ind,6);
 ptr=zag.data();
 memcpy(glzag.buf,ptr,6);
 glzag.bt.ident=qFromBigEndian(glzag.bt.ident);
 glzag.bt.status=qFromBigEndian(glzag.bt.status);
 return;
}

bool ObrIzm::StatFull(QByteArray &buf, bool prprt, QString nfile_prt)
{
 QByteArray sinxr;
 qint64 ind,indp,indsinxr;
 bool fl_crc;
 qint64 koldds;
 QStringList prot;
 QString str;

 ClearStatFull(); //очистка структуры StrStatFull

 sinxr=QByteArray::fromHex("1ACFFC1D");
 ind=buf.indexOf(sinxr); //поиск синхросигнала
 if (ind == -1) return false; //синхросигнал не найден (пустой сеанс)

 indsinxr=0;
 ind=0;
 indp=0;
 while (indsinxr > -1)
 {
  indsinxr=buf.indexOf(sinxr,indsinxr+1);
  if (indsinxr == -1) break;
  ind=indsinxr-520;
  if (indp > 0) statfull.kbm=statfull.kbm+((ind-indp)/576)-1;
  indp=ind;
  statfull.k++;

  fl_crc=CrcKadrFULL(buf,ind);
  if (!fl_crc) statfull.crc++;

  ReadGlZagKadrFULL(buf,ind);

  if (glzag.fl.nvirtkan == 0) //пустой кадр
  {
   statfull.pk++;
   if (!fl_crc) statfull.opk++;
   continue;
  }
  if (glzag.fl.nvirtkan == 1) //1 часть ВРВ
  {
   statfull.vrv1++;
   if (!fl_crc) statfull.ovrv1++;
   continue;
  }
  if (glzag.fl.nvirtkan == 2) //КВНО
  {
   statfull.kvno++;
   if (!fl_crc) statfull.okvno++;
   continue;
  }
  if (glzag.fl.nvirtkan == 3) //2 часть ВРВ
  {
   statfull.vrv2++;
   if (!fl_crc) statfull.ovrv2++;
   continue;
  }
  if ((glzag.fl.nvirtkan == 4) || (glzag.fl.nvirtkan == 5) || (glzag.fl.nvirtkan == 6)) //КВИ
  {
   statfull.kvi++;
   if (!fl_crc) statfull.okvi++;
   koldds=ObrKadrKVI(buf,ind);
   statfull.izm_kvi=statfull.izm_kvi+koldds;
   continue;
  }
  if (glzag.fl.nvirtkan == 7) //резерв
  {
   statfull.rez++;
   if (!fl_crc) statfull.orez++;
   continue;
  }

 }

 if (!prprt) return true;

 prot << nfile_prt;

 str="Количество прочитанных кадров : "+QString::number(statfull.k);
 prot << str;
 str="Количество кадров без маркера синхронизации : "+QString::number(statfull.kbm);
 prot << str;
 str="Количество кадров с ошибками crc : "+QString::number(statfull.crc);
 prot << str;
 str="Количество пустых кадров : "+QString::number(statfull.pk);
 prot << str;
 str="Количество кадров перв.ВРВ : "+QString::number(statfull.vrv1);
 prot << str;
 str="Количество кадров втор.ВРВ : "+QString::number(statfull.vrv2);
 prot << str;
 str="Количество кадров ДДС КВИ : "+QString::number(statfull.kvi);
 prot << str;
 str="Количество кадров ДДС КВНО : "+QString::number(statfull.kvno);
 prot << str;
 str="Количество резервных кадров : "+QString::number(statfull.rez);
 prot << str;
 str="Количество пустых кадров с ошибками crc : "+QString::number(statfull.opk);
 prot << str;
 str="Количество кадров перв.ВРВ с ошибками crc : "+QString::number(statfull.ovrv1);
 prot << str;
 str="Количество кадров втор.ВРВ с ошибками crc : "+QString::number(statfull.ovrv2);
 prot << str;
 str="Количество кадров ДДС КВИ с ошибками crc : "+QString::number(statfull.okvi);
 prot << str;
 str="Количество кадров ДДС КВНО с ошибками crc : "+QString::number(statfull.okvno);
 prot << str;
 str="Количество резервных кадров с ошибками crc : "+QString::number(statfull.orez);
 prot << str;
 str="Общее количество измерений КВИ : "+QString::number(statfull.kvi*8);
 prot << str;
 str="Полученное количество измерений КВИ : "+QString::number(statfull.izm_kvi);
 prot << str;

 if (nfile_prt != "") WriteFileFromList(prot,nfile_prt);

 return true;
}

QTime ObrIzm::TimeKadrFULL(QByteArray &buf,qint64 ind)
{
 QTime t;
 QTime time0;
 quint32 cek,msek;
 QString bi;
 bool ok;

 time0.setHMS(0,0,0,0);

 QByteArray tk=buf.mid(ind+524,4).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(6,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(4,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));

 cek = bi.mid(16,17).toUInt(&ok,2);
 msek = bi.mid(5,10).toUInt(&ok,2);
 t=time0.addSecs(cek);
 t=t.addMSecs(msek);

 return t;
}

/*!
 * \brief ObrIzm::ReadFULL
 * Определение смещения в области памяти FULL файла для первого(очередного) кадра
 * В зависимости от параметра kod, определяется смещение для первого или последующего кадра FULL файла
 * заполняется структура glzag (главного заголовка кадра FULL файла)
 * в переменной класса ind_full запоминается смещение начала кадра в области buf
 * \param buf[in] область памяти с содержимым FULL файла
 * \param kod[in] код работы: 0 - чтение первого найденного кадра; 1 - чтение следующего кадра;
 * \param nvk[out] № виртуального канала из заголовка кадра FULL файла
 * \param pr_crc[out] признак КС: true - КС хорошая; false - КС плохая.
 * \return при отсутствии кадра в файле возвращается - false, иначе - true.
 * возврат false при параметре kod=0 говорит об отсутствии кадров в файле
*/
bool ObrIzm::ReadFULL(QByteArray &buf,qint32 kod,qint32 &nvk,bool &pr_crc)
{
 QByteArray sinxr;
 qint64 ind;

 pr_crc=true;

 if (kod == 0) ind_full=0;
 else ind_full=ind_full+576;

 sinxr=QByteArray::fromHex("1ACFFC1D");
 ind=buf.indexOf(sinxr,ind_full);
 if (ind == -1)
 {
  //не найдено маркеров синхронизации
  return false;
 }

 ind_full=ind-520;

 if (!CrcKadrFULL(buf,ind_full)) pr_crc=false;

 ReadGlZagKadrFULL(buf,ind_full);

 nvk=glzag.fl.nvirtkan;

 return true;
}

bool ObrIzm::ReadBackFULL(QByteArray &buf, qint32 kod, qint32 &nvk, bool &pr_crc)
{
    if ( ind_full < 520 )
    {
        return false;
    }

    QByteArray sinxr;
    qint64 ind;

    pr_crc = true;

    if( kod == 0 )
    {
        ind_full = buf.size();
    }

    sinxr = QByteArray::fromHex( "1ACFFC1D" );
    ind = buf.lastIndexOf( sinxr, ind_full );

    if( ind == -1 )     //не найдено маркеров синхронизации
    {
        return false;
    }

    ind_full = ind - 520;

    if( !CrcKadrFULL( buf, ind_full ) ) pr_crc = false;

    ReadGlZagKadrFULL( buf, ind_full );

    nvk = glzag.fl.nvirtkan;

    return true;
}


/*!
 * \brief ObrIzm::ObrKadrKVI
 * определение количества кадров КВИ с хорошей КС в кадре ППСИ (0-8)
 * \param buf[in] область памяти с FULL файлом
 * \param ind[in] смещение на кадр ППСИ
 * \return возвращает количество кадров с хорошей КС
*/
qint32 ObrIzm::ObrKadrKVI(QByteArray &buf,qint64 ind)
{
 QByteArray buf_dds;
 qint32 ret=8;
 uchar crc;

 ReadGlZagKadrFULL(buf,ind);

 if ((glzag.fl.nvirtkan < 4) && (glzag.fl.nvirtkan > 6)) return 0;

 for (int i=0; i<8; i++)
 {
  buf_dds=buf.mid(ind+6+(i*64),64);
  if (!Crc8(buf_dds,0,64,crc)) ret--;
 }

 return ret;
}

/*!
 * \brief ObrIzm::KonvDDSfromKadrKVI
 * Формирование ключевого списка измерений КВИ для кадра ППСИ (8 кадров КВИ).
 * Изменяет содержание структуры StrStatFull.
 * \param buf[in] область памяти FULL файла
 * \param ind[in] смещение на начало кадра
 * \param map[in,out] заполняемый ключевой список
 * \param prot[in,out] заполняемый список ошибок
 * \param dat_ppsi[in] дата сеанса из FULL файла
 * \return возвращает false если кадр ППСИ не КВИ; иначе true
*/
bool ObrIzm::KonvDDSfromKadrKVI(QByteArray &buf,qint64 ind,QMap<QString,QString> &map,QStringList &prot,QDate dat_ppsi)
{
 QByteArray buf_dds;
 qint32 ret;
 QDate d_pr;
 QTime t_pr;
 QString stime,str;

 ReadGlZagKadrFULL(buf,ind); //Чтение главного заголовка кадра FULL файла и заполнение структуры glzag
 if ((glzag.fl.nvirtkan < 4) || (glzag.fl.nvirtkan > 6)) return false;

 for (int i=0; i<8; i++)
 {
  statfull.k_kvi++;
  buf_dds=buf.mid(ind+6+(i*64),64);
  ret=KonvDDS_KVI(buf_dds,map,dat_ppsi); //запись измерения в ключевой список

  //ОТЛАДКА
 // KadrKviInList(prt_kvi,ret);

  if (ret == 2) continue; //пустой кадр КВИ - сообщение не записывается
  if (ret > 0) //заполнение списка ошибок
  {
   if (ret == 1) statfull.crc_kvi++;
   else
   {
    d_pr.setDate(kadrkvi.year,kadrkvi.month,kadrkvi.day);
    t_pr=SecToTime(kadrkvi.time,stime);
    str=d_pr.toString("dd-MM-yyyy")+" "+t_pr.toString("hh:mm:ss")+" ; prTime= "+QString::number(kadrkvi.prtime);
    str=str+" ; nka= "+QString::number(kadrkvi.nka);
    str=str+" - "+list_err[ret];
    prot << str;
    statfull.och_kvi++;
   }
  }
 }

 return true;
}

/*!
 * \brief ObrIzm::KonvDDS_KVI
 * добавление эпохи измерений КВИ в формате RINEX 3.02 для одного кадра КВИ в ключевой список (Multi)
 * в заголовках измерений не записывается количество спутников
 * \param buf[in] область памяти с кадром КВИ
 * \param map[in,out] ключевой список с измерениями (ключ - заголовок эпохи)
 * \param dat_ppsi[in] дата сеанса из FULL файла для отбраковки
 * \return при успешном выполнении возвращает 0; иначе код ошибки по списку list_err
*/
qint32 ObrIzm::KonvDDS_KVI(QByteArray buf,QMap<QString, QString> &map,QDate dat_ppsi)
{
 uchar crc;
 QDate dt;
 qint64 raz;
 qint32 ret=0;

 if (!Crc8(buf,0,64,crc)) return 1; ///Расчёт КС CRC8

 ReadDDS_KVI(buf); ///чтение содержимого кадра КВИ в структуру KadrKVI

 //устранение ошибки неправильной даты
 dt=QDate(QDate(kadrkvi.year,kadrkvi.month,kadrkvi.day));
 raz=QDate(1998,10,20).daysTo(QDate(2018,6,5));
 if (kadrkvi.year>=1988 && kadrkvi.year<=2000)
 {
  dt=dt.addDays(raz);
  kadrkvi.year=dt.year();
  kadrkvi.month=dt.month();
  kadrkvi.day=dt.day();
 }

 ret=OtbrDDS_KVI(dat_ppsi); ///отбраковка кадра КВИ по данным из структуры KadrKVI
 if (ret != 0) return ret;



 FormDDS_KVI(map); ///формирование заголовка эпохи и строки измерения

 return 0;
}

QStringList ObrIzm::KonvFULL(QString nfile,QString path,qint32 nka)
{
 QByteArray buf;
 bool pr_r=true;
 qint32 nvk;
 bool pr_crc;
 qint32 kod=0;
 QMap<QString,QString> map_kvi;
 QMap<QDateTime,QString> map_kvno;
 QStringList prot_kvi,prot_kvno;
 QString nfile_kvi,nfile_kvno;
 QStringList spis_f;
 QDateTime dtn;
 QDateTime dtk;
 QDate dt;

 buf=ReadFile(nfile);
 if (buf.isEmpty()) return spis_f;
 PrefFull(buf,dtn,dtk); ///определение даты и времени начала и конца сеанса ППСИ
 dt=dtk.date();

 while(pr_r)
 {
  pr_r=ReadFULL(buf,kod,nvk,pr_crc); ///Определение смещения в области памяти FULL файла для первого(очередного) кадра
  if (kod == 0)
  {
   if (!pr_r) return spis_f;
   kod=1;
  }

  if (!pr_r) break;

  ///формирование ключевого списка измерений КВИ для кадра ППСИ (8 кадров КВИ)
  if ((nvk >= 4) && (nvk <= 6)) KonvDDSfromKadrKVI(buf,ind_full,map_kvi,prot_kvi,dt);

  if (nvk == 2) KonvDDSfromKadrKVNO(buf,ind_full,map_kvno,prot_kvno,dt);
 }

 nfile_kvi=FormFileDDS_KVI(map_kvi,path,nfile,nka);
 spis_f << nfile_kvi;
 nfile_kvno=FormFileDDS_KVNO(map_kvno,path,nka);
 spis_f << nfile_kvno;

 return spis_f;
}

/*!
 * \brief ObrIzm::ReadDDS_KVI
 * чтение содержимого кадра КВИ в структуру KadrKVI
 * содержимое памяти переворачивается под INTEL
 * \param buf[in] область памяти с содержимым кадра КВИ (64 байта)
*/
void ObrIzm::ReadDDS_KVI(QByteArray buf)
{
 char* ptr;
 QByteArray ba;
 double C=299792458;

 for (int i=0;i<32;i++) // 64 перевернуть байты
 {
  ba=buf.mid(i*2,1);
  buf.replace(i*2,1,buf.mid(i*2+1,1));
  buf.replace(i*2+1,1,ba);
 }

 ptr=buf.data();

 kadrkvi.crc=C1(ptr);			// 1
 kadrkvi.ident=C1(ptr+1);	// 1
 kadrkvi.year=U2(ptr+2);		// 2
 kadrkvi.month=U1(ptr+4);	// 1
 kadrkvi.day=U1(ptr+5);	  // 1
 kadrkvi.time=U4(ptr+6);	  // 4
 kadrkvi.nka=C1(ptr+10);		// 1
 kadrkvi.nsutki=U2(ptr+11);// 2
 kadrkvi.tsutki=U4(ptr+13);// 4
 kadrkvi.status=C1(ptr+17);// 1
 kadrkvi.ca=C1(ptr+18);		// 1
 kadrkvi.l2=C1(ptr+19);		// 1
 kadrkvi.pdca=R8(ptr+20);	// 8
 kadrkvi.pdl2=R8(ptr+28);	// 8
 kadrkvi.fl1=R8(ptr+36);		// 8
 kadrkvi.fl2=R8(ptr+44);		// 8

 kadrkvi.pdca=kadrkvi.pdca*C;  // в метры
 kadrkvi.pdl2=kadrkvi.pdl2*C;  // в метры

 kadrkvi.dl1=I4(ptr+52);      // 4
 kadrkvi.dl2=I4(ptr+56);      // 4
 kadrkvi.nl=I1(ptr+60);			 // 1
 kadrkvi.prtime=I1(ptr+61);	 // 1
 kadrkvi.prizmer=I1(ptr+62);	 // 1
 kadrkvi.reserv=C1(ptr+63);	 // 1

 return;
}

/*!
 * \brief ObrIzm::OtbrDDS_KVI
 * отбраковка кадра КВИ по данным из структуры KadrKVI
 * \param dat_ppsi[in] дата сеанса из FULL файла
 * \return возвращает 0 при успешной проверке или код ошибки
*/
qint32 ObrIzm::OtbrDDS_KVI(QDate dat_ppsi)
{
 qint32 ret=0;
 QDateTime dt;
 QDate d;
 QTime t;
 qint32 weekGPS,sutGLO;
 qint32 secGPS,secGLO;
 QString str_pdca;
 qint8 prgr;
 qint32 kolsut;

 //анализ модифицированного номера (номер КА)
 if (kadrkvi.nka == 0) return 2;
 if ((kadrkvi.nka < 0) || (kadrkvi.nka > 56)) return 7;

 if (kadrkvi.nka > 0 && kadrkvi.nka < 33) prgr=0;
 if (kadrkvi.nka > 32 && kadrkvi.nka < 65) prgr=1;

 //анализ признака получения измерений
 if (kadrkvi.prizmer == 0) return 3;
 if ((kadrkvi.prizmer < 0) || (kadrkvi.prizmer > 2)) return 4;

 //анализ признака получения времени
 if (kadrkvi.prtime == 2) return 5;
 if (kadrkvi.prtime != 0) return 6;

// if ((kadrkvi.prtime < 0) || (kadrkvi.prtime > 2)) return 6; //ОТЛАДКА

 //анализ корректности получения даты приёмника
 if (!QDate(kadrkvi.year,kadrkvi.month,kadrkvi.day).isValid()) return 8;

 //анализ корректности получения времени приёмника
 if ((kadrkvi.time < 0) || (kadrkvi.time > 86400)) return 9;

 //анализ корректности года в дате приёмника (не должен быть больше текущего)
 if (kadrkvi.year > QDate::currentDate().year()) return 10;

 //анализ корректности данных для ГЛОНАСС
 if (prgr == 1)
 {
  if ((kadrkvi.nka < 33) || (kadrkvi.nka > 56)) return 16;
  if ((kadrkvi.nsutki < 1) || (kadrkvi.nsutki > 1461)) return 11;
  if ((kadrkvi.tsutki < 0) || (kadrkvi.tsutki > 86400)) return 12;
  if ((kadrkvi.nl < -7) || (kadrkvi.nl > 12)) return 15;
 }

 //анализ корректности данных для GPS
 if (prgr == 0)
 {
  if ((kadrkvi.nka < 1) || (kadrkvi.nka > 32)) return 17;
  if ((kadrkvi.nsutki < 0) || (kadrkvi.nsutki > 1023)) return 13;
  if ((kadrkvi.tsutki < 0) || (kadrkvi.tsutki > 604800)) return 14;
 }

 d.setDate(kadrkvi.year,kadrkvi.month,kadrkvi.day);
 t.setHMS(0,0,0);
 t=t.addSecs(kadrkvi.time);
 dt.setDate(d);
 dt.setTime(t);

 //сравнение времени приёмника и времени GPS из измерений
  if (prgr == 0)
  {
   SetToGPS(dt,weekGPS,secGPS);
   if ((weekGPS != kadrkvi.nsutki) || (secGPS != kadrkvi.tsutki)) return 18;
  }

  //сравнение времени приёмника и времени ГЛОНАСС из измерений
  if (prgr == 1)
  {
   SetToGLO(dt,sutGLO,secGLO);
   kolsut=abs(kadrkvi.nsutki-sutGLO);
   if (kolsut > 1) return 19;
  }

  //сравнение времени приёмника и времени сеанса ППСИ
  if (d > dat_ppsi) return 30;
  kolsut=d.daysTo(dat_ppsi);
  if (kolsut > 14) return 31;

 if (QDate(kadrkvi.year,kadrkvi.month,kadrkvi.day) < QDate(2016,06,04)) return 8;

 if ( kadrkvi.fl1 > 1e+10 ) return 20;

 str_pdca=QString("%1").arg(kadrkvi.pdca,14,'f',3);
 str_pdca=str_pdca.trimmed();
 if (str_pdca == "inf") return 21;

 return ret;
}

/*!
 * \brief ObrIzm::FormDDS_KVI
 * формирование заголовка эпохи и строки измерения со спутника в формате RINEX 3.02
 * сформированные данные добавляются в ключевой список как Multi
 * \param map[in,out] заполняет(добавляет) строку ключевого списка (ключ-заголовок эпохи измерения)
*/
void ObrIzm::FormDDS_KVI(QMap<QString, QString> &map)
{
 QString ka;
 QString zag_izm,izm;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString str;
 qint32 kss;

 dat.setDate(kadrkvi.year,kadrkvi.month,kadrkvi.day);
 time=SecToTime(QString::number(kadrkvi.time));
 dt.setDate(dat);
 dt.setTime(time);

// if (dt.toString("dd-MM-yyyy hh:mm:ss") == "05-11-2019 22:43:31")
//     zag_izm="";

 zag_izm="> "+DtToStrHdrRnx(dt,2)+"  0  0";

 if (kadrkvi.nka > 0 && kadrkvi.nka < 33) ka="G"+QString::number(kadrkvi.nka).rightJustified(2,'0');
 if (kadrkvi.nka > 32 && kadrkvi.nka < 65) ka="R"+QString::number(kadrkvi.nka-32).rightJustified(2,'0');

 if (kadrkvi.prizmer == 1)
 {
  izm=ka;
  if (kadrkvi.pdca < 1) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.pdca,14,'f',3));
   izm.append(CodSH(kadrkvi.ca));
  }

  if (kadrkvi.fl1 < 1) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.fl1,14,'f',3));
   izm.append(str.fill(' ',2));
  }

  if (kadrkvi.dl1 != 0)
  {
   izm.append(QString("%1").arg(kadrkvi.dl1*0.0001,14,'f',3));
   izm.append(str.fill(' ',2));
  }
  else izm.append(str.fill(' ',16));
 }

 if (kadrkvi.prizmer == 2)
 {
  izm=ka;
  if (kadrkvi.pdca < 1) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.pdca,14,'f',3));
   izm.append(CodSH(kadrkvi.ca));
  }

  if (kadrkvi.fl1 < 1) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.fl1,14,'f',3));
   izm.append(str.fill(' ',2));
  }

  if (kadrkvi.dl1 != 0)
  {
   izm.append(QString("%1").arg(kadrkvi.dl1*0.0001,14,'f',3));
   izm.append(str.fill(' ',2));
  }
  else izm.append(str.fill(' ',16));

  if (kadrkvi.pdl2 < 1) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.pdl2,14,'f',3));
   izm.append(str.fill(' ',2));
  }

  if (kadrkvi.fl2 < 1 || kadrkvi.fl2 > 10e+10) izm.append(str.fill(' ',16));
  else
  {
   izm.append(QString("%1").arg(kadrkvi.fl2,14,'f',3));
   izm.append(CodSH(kadrkvi.l2));
  }

  if (kadrkvi.dl2 != 0)
  {
   izm.append(QString("%1").arg(kadrkvi.dl2*0.0001,14,'f',3));
   izm.append(str.fill(' ',2));
  }
  else izm.append(str.fill(' ',16));

  if (izm.indexOf("inf") > 0)
  {
   kss=izm.indexOf("inf")-13;
   izm.replace(kss,16,str.fill(' ',16));
  }
 }

 map.insertMulti(zag_izm,izm);

 return;
}

QString ObrIzm::CodSH(uchar cod)
{
 QString CodStr;

 if ( cod < 12)       CodStr=" 1";
 else if ( cod <= 17) CodStr=" 2";
 else if ( cod <= 23) CodStr=" 3";
 else if ( cod <= 29) CodStr=" 4";
 else if ( cod <= 35) CodStr=" 5";
 else if ( cod <= 41) CodStr=" 6";
 else if ( cod <= 47) CodStr=" 7";
 else if ( cod <= 53) CodStr=" 8";
 else if ( cod >= 54) CodStr=" 9";

 return (CodStr);
}

/*!
 * \brief ObrIzm::FormFileDDS_KVI
 * формирование и запись файла измерений КВИ в формате RINEX 3.02 на диск
 * \param map[in] ключевой список с измерениями (ключ - заголовок (эпоха) измерений)
 * \param path[in] путь для записи файла
 * \param naz_full[in] имя исходного FULL файла для записи в заголовок (включая путь или без него)
 * \param nka[in] № КА для формирования имени файла
 * \return при успешном завершении возвращает имя записанного файла (без пути), иначе пустую строку
*/
QString ObrIzm::FormFileDDS_KVI(QMap<QString, QString> &map,QString path,QString naz_full,qint32 nka)
{
 QStringList list,list_hdr,list_f;
 QString str,nfile;

 if (map.isEmpty()) return nfile;

 list_f=FileInfo(naz_full);
 str=list_f[2];
 list_f.clear();
 list_f << str;

 //ОТЛАДКА
 //list=MapToList(map);
 //WriteFileFromList(list,path+"/map.txt");

 //формирование отсортированного списка измерений в формате RINEX 3.02
 list=FormOFileFromMap(map);

 //ОТЛАДКА
 //WriteFileFromList(list,path+"/list.txt");

 map=ReadOFileInMap(list);
 list=FormOFileFromMap(map);

 //удаление эпох с одним измерением
 DelEpoch(1,list,str);
 if (list.isEmpty()) return nfile;

 //формирование заголовка файла и формирование окончательного списка (заголовок+данные)
 list_hdr=FormHdrDDS(list_f,list,QString::number(nka));
 AddList(list_hdr,list);

 //формирование имени выходного файла и запись его на диск
 nfile=NazOFile(QString::number(nka)+"G",list_hdr,path+"/");
 if (!WriteFileFromList(list_hdr,path+"/"+nfile)) return "";

 return nfile;
}

bool ObrIzm::KonvDDSfromKadrKVNO(QByteArray &buf, qint64 ind, QMap<QDateTime, QString> &map, QStringList &prot,QDate dat_ppsi)
{
 QByteArray buf_dds;
 qint32 ret;
 QDate d_pr;
 QTime t_pr;
 QString stime,str;

 ReadGlZagKadrFULL(buf,ind);
 if (glzag.fl.nvirtkan != 2) return false;

 statfull.k_kvno++;

 buf_dds=buf.mid(ind+6,64);
 ret=KonvDDS_KVNO(buf_dds,map,dat_ppsi);
 if (ret > 0)
 {
  if (ret == 1) statfull.crc_kvno++;
  else
  {
   d_pr.setDate(kadrkvno.year,kadrkvno.month,kadrkvno.day);
   t_pr=SecToTime(kadrkvno.time,stime);
   str=d_pr.toString("dd-MM-yyyy")+" "+t_pr.toString("hh:mm:ss")+" ; prTime= "+QString::number(kadrkvno.prtime);
   str=str+" - "+list_err[ret];
   prot << str;
   statfull.och_kvno++;
  }
 }

 return true;
}

qint32 ObrIzm::KonvDDS_KVNO(QByteArray buf, QMap<QDateTime, QString> &map,QDate dat_ppsi)
{
 uchar crc;
 QDate dt;
 qint64 raz;
 qint32 ret=0;

 if (!Crc8(buf,0,64,crc)) return 22;

 ReadDDS_KVNO(buf);

 dt=QDate(QDate(kadrkvno.year,kadrkvno.month,kadrkvno.day));
 raz=QDate(1998,10,20).daysTo(QDate(2018,6,5));
 if (kadrkvno.year>=1988 && kadrkvno.year<=2000)
 {
  dt=dt.addDays(raz);
  kadrkvno.year=dt.year();
  kadrkvno.month=dt.month();
  kadrkvno.day=dt.day();
 }

 ret=OtbrDDS_KVNO(dat_ppsi);
 if (ret != 0) return ret;

 FormDDS_KVNO(map);

 return 0;
}

void ObrIzm::ReadDDS_KVNO(QByteArray buf)
{
 char* p;
 QByteArray ba;
 double C=299792458;

 for (int i=0;i<32;i++) // 64 перевернуть байты
 {
  ba=buf.mid(i*2,1);
  buf.replace(i*2,1,buf.mid(i*2+1,1));
  buf.replace(i*2+1,1,ba);
 }

 p=buf.data();

 kadrkvno.crc=C1(p);      // 1
 kadrkvno.ident=C1(p+1);  // 1
 kadrkvno.time=U4(p+2);   // 4
 kadrkvno.year=U2(p+6);   // 2
 kadrkvno.month=U1(p+8);   // 1
 kadrkvno.day=U1(p+9);   // 1
 kadrkvno.prtime=C1(p+10);  // 1
 kadrkvno.prnav=C1(p+11);  // 1
 kadrkvno.x=R8(p+12);  // 8
 kadrkvno.y=R8(p+20);  // 8
 kadrkvno.z=R8(p+28);  // 8
 kadrkvno.vx=R4(p+36);   // 4
 kadrkvno.vy=R4(p+40);   // 4
 kadrkvno.vz=R4(p+44);   // 4
 kadrkvno.delt_glo=R4(p+48);   // 4
 kadrkvno.delt_gps=R4(p+52);   // 4
 kadrkvno.skok=R4(p+56);   // 4
 kadrkvno.skov=R4(p+60);   // 4

 return;
}

qint32 ObrIzm::OtbrDDS_KVNO(QDate dat_ppsi)
{
 qint32 ret=0;
 qint32 kolsut;
 QDateTime dt;
 QDate d;
 QTime t;

 if (kadrkvno.prnav == 0) return 23;
 if ((kadrkvno.prnav < 0) || (kadrkvno.prnav > 1)) return 24;

 if (kadrkvno.prtime == 2) return 25;
// if ((kadrkvno.prtime < 0) || (kadrkvno.prtime > 2)) return 26;
 if (kadrkvno.prtime != 0) return 26;

 if (!QDate(kadrkvno.year,kadrkvno.month,kadrkvno.day).isValid()) return 27;

 if ((kadrkvno.time < 0) || (kadrkvno.time > 86400)) return 28;

// if (kadrkvno.year > QDate::currentDate().year()) return 29;

 if (QDate(kadrkvno.year,kadrkvno.month,kadrkvno.day) < QDate(2016,06,04)) return 27;

 d.setDate(kadrkvno.year,kadrkvno.month,kadrkvno.day);
 t.setHMS(0,0,0);
 t=t.addSecs(kadrkvno.time);
 dt.setDate(d);
 dt.setTime(t);

 if (d > dat_ppsi) return 33;
 kolsut=d.daysTo(dat_ppsi);
 if (kolsut > 14) return 32;

 return ret;
}

void ObrIzm::FormDDS_KVNO(QMap<QDateTime, QString> &map)
{
 QDate dat;
 QTime time;
 QDateTime dt;
 QString str;

 dat.setDate(kadrkvno.year,kadrkvno.month,kadrkvno.day);
 time=SecToTime(QString::number(kadrkvno.time));
 dt.setDate(dat);
 dt.setTime(time);

 str=QString::number(kadrkvno.year);
 str=str+QString::number(kadrkvno.month).rightJustified(3,' ');
 str=str+QString::number(kadrkvno.day).rightJustified(3,' ');
 str=str+QString("%1").arg(time.toString(" hh mm ss.zzzzzzz"));

 str=str+QString::number(kadrkvno.prtime).rightJustified(2,' ');

 str=str+QString::number(kadrkvno.prnav).rightJustified(2,' ');

 str=str+QString("%1").arg(kadrkvno.x,14,'f',3);
 str=str+QString("%1").arg(kadrkvno.y,14,'f',3);
 str=str+QString("%1").arg(kadrkvno.z,14,'f',3);

 str=str+QString("%1").arg(kadrkvno.vx,14,'f',6);
 str=str+QString("%1").arg(kadrkvno.vy,14,'f',6);
 str=str+QString("%1").arg(kadrkvno.vz,14,'f',6);

 str=str+QString("%1").arg(kadrkvno.delt_glo,14,'f',9);
 str=str+QString("%1").arg(kadrkvno.delt_gps,14,'f',9);

 str=str+QString("%1").arg(kadrkvno.skok,10,'f',3);
 str=str+QString("%1").arg(kadrkvno.skov,11,'f',6);

 map.insert(dt,str);

 return;
}

QString ObrIzm::FormFileDDS_KVNO(QMap<QDateTime, QString> &map, QString path, qint32 nka)
{
 QStringList list;
 QString nfile;
 QDateTime dt;

 nfile.clear();
 if (map.isEmpty()) return nfile;

 list=FormKFileFromMap(map,dt);

 nfile=NazKFile(QString::number(nka),dt.date(),path+"/");
 WriteFileFromList(list,path+"/"+nfile);

 return nfile;
}

QString ObrIzm::NazKFile(QString nka,QDate dt,QString path)
{
 QString naz_file="";
 QString str;
 QString pref="123456789abcdefghijklmnopqastuvwxyz";//34
 int sut,god;
 QString ssut="000";
 QString sgod="00";
 QFile f;

 sut=dt.dayOfYear();
 str.setNum(sut);
 ssut.replace(3-str.length(),str.length(),str);

 god=dt.year();
 str.setNum(god);
 str=str.mid(2,2);
 sgod.replace(2-str.length(),str.length(),str);

 naz_file=nka+ssut+"0."+sgod+"K";
 naz_file=naz_file.toUpper();

 for (int i=0; i<34; i++)
 {
  f.setFileName(path+"/"+naz_file);
  if (!f.exists()) break;
  naz_file.replace(6,1,pref.mid(i,1));
 }

 return naz_file;
}

QStringList ObrIzm::FormKFileFromMap(QMap<QDateTime,QString> map,QDateTime &nep)
{
 QStringList list;
 QMap<QDateTime,QString>::const_iterator i;

 if (map.isEmpty()) return list;

 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
   list << i.value();
   if (i == map.constBegin()) nep=i.key();
 }

 return list;
}

int ObrIzm::KolSutInK(QStringList list)
{
 int sut=0;
 QString str;
 QDate dn,dk;
 QDateTime dt;

 if (list.isEmpty()) return sut;

 str=list.at(0);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dn=dt.date();

 str=list.at(list.count()-1);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dk=dt.date();

 sut=(dn.daysTo(dk))+1;

 return sut;
}

qint32 ObrIzm::KolSutInK(QStringList list,QDate &dn,QDate &dk)
{
 qint32 sut=0;
 QString str;
 QDateTime dt;

 if (list.isEmpty()) return sut;

 str=list.at(0);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dn=dt.date();

 str=list.at(list.count()-1);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dk=dt.date();

 sut=(dn.daysTo(dk))+1;

 return sut;
}

qint32 ObrIzm::KolSutInK(QString nfile,QDate &dn,QDate &dk)
{
 qint32 sut;
 QStringList list;

 list=ReadFileInList(nfile);
 sut=KolSutInK(list,dn,dk);

 return sut;
}

QStringList ObrIzm::RazbKFile(QString nfile)
{
 QStringList spis_f;
 QStringList list,new_list;
 qint32 kolsut;
 qlonglong kolstr;
 QString str,str1;
 QDateTime dt;
 QDate dn,dk;
 QStringList inf_f;
 QString path;
 QString nka;
 QString kfile;

 list=ReadFileInList(nfile);
 if (list.isEmpty()) return spis_f;

 kolsut=KolSutInK(list);

 if (kolsut == 0) return spis_f;
 if (kolsut < 2) {spis_f << nfile; return spis_f;}

 inf_f=FileInfo(nfile);
 path=inf_f[1];
 str=inf_f[2];
 nka=str.mid(0,3);

 DelFile(nfile);

 kolstr=list.count();
 str=list.at(0);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dn=dt.date();

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str1=str.mid(0,27);
  dt=SetDTFromStr(str1);
  dk=dt.date();
  if (dn == dk) {new_list << str; continue;}
  kfile=NazKFile(nka,dn,path);
  WriteFileFromList(new_list,path+"/"+kfile);
  spis_f << kfile;
  new_list.clear();
  dn=dk;
  new_list << str;
 }

 if (!new_list.isEmpty())
 {
  kfile=NazKFile(nka,dn,path);
  WriteFileFromList(new_list,path+"/"+kfile);
  spis_f << kfile;
 }

 return spis_f;
}

QMap<QDateTime,QString> ObrIzm::KFileInMap(QString nfile)
{
 QMap<QDateTime,QString> map;
 QStringList list;
 qlonglong kolstr;

 list=ReadFileInList(nfile);
 if (list.isEmpty()) return map;
 kolstr=list.count();

 map=KFileInMap(list);

 return map;
}

QString ObrIzm::ObKFile(QStringList spisf,QString nka,QString path)
{
 QString nfile="";
 qint32 kolstr;
 QStringList list,olist;
 QString str;
 QMap<QDateTime,QString> map;
 QDateTime dt;
 QStringList inff;

 if (spisf.isEmpty()) return nfile;
 kolstr=spisf.count();
 if (kolstr == 1) {nfile=spisf.at(0); return nfile;}

 inff=FileInfo(spisf.at(0));

 for (int i=0; i<kolstr; i++)
 {
  str=spisf.at(i);
  list=ReadFileInList(str);
  AddList(olist,list);
  DelFile(str);
 }

 str=path;
 if (path == "") str=inff[1];

 map=KFileInMap(olist);
 olist=FormKFileFromMap(map,dt);
 nfile=NazKFile(nka,dt.date(),str);
 nfile=str+"/"+nfile;
 if (!WriteFileFromList(olist,nfile)) nfile="";
 return nfile;
}

QMap<QDateTime,QString> ObrIzm::KFileInMap(QStringList list)
{
 QMap<QDateTime,QString> map;
 qlonglong kolstr;
 QString str,str1;
 QDateTime dt;

 if (list.isEmpty()) return map;
 kolstr=list.count();

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str1=str.mid(0,27);
  dt=SetDTFromStr(str1);
  map.insert(dt,str);
 }

 return map;
}

/*!
 * \brief ObrIzm::ReadZagVRV
 * Чтение информации заголовка кадра ВРВ и заполнение структуры zagvrv
 * \param buf[in] область данных
 * \param ind[in] смещение начала кадра в области данных FULL, для ALTI смещение уменьшить на 6
*/
void ObrIzm::ReadZagVRV(QByteArray &buf,qint64 ind)
{
 QByteArray sh2;
 QString bi,bb;
 bool ok;
 qint32 ncek;
 qlonglong imcek;
 qint32 ned;

 sh2=buf.mid(ind+6,24).toHex().toUpper();           //заголовок ВРВ	48 символа
 bi=QString("%1").arg(sh2.mid(0,2).toInt(&ok,16),8,2,QChar('0'))+	 // 0-7
    QString("%1").arg(sh2.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+	 // 8-15
    QString("%1").arg(sh2.mid(4,2).toInt(&ok,16),8,2,QChar('0'))+	 // 16-23
    QString("%1").arg(sh2.mid(6,2).toInt(&ok,16),8,2,QChar('0'))+	 // 24-31
    QString("%1").arg(sh2.mid(8,2).toInt(&ok,16),8,2,QChar('0'))+	 // 32-39
    QString("%1").arg(sh2.mid(10,2).toInt(&ok,16),8,2,QChar('0'));	 // 40-47

 zagvrv.nver=bi.mid(0,3).toInt(0,2);
 zagvrv.tippak=bi.mid(3,1).toInt(0,2);
 zagvrv.fl2zag=bi.mid(4,1).toInt(0,2);
 zagvrv.idist=bi.mid(5,11).toInt(0,2);
 zagvrv.flgr=bi.mid(16,2).toInt(0,2);
 zagvrv.chpak=bi.mid(18,14).toInt(0,2);
 zagvrv.dlpak=bi.mid(32,16).toInt(0,2);

 bb=QString("%1").arg(sh2.mid(12,4).toInt(&ok,16),16,2,QChar('0'));
 ned=bb.mid(4,12).toUInt(&ok,2);         // НЕДЕЛЯ от 06.01.1980

 bb=QString(sh2.mid(16,8));
 ncek=bb.toUInt(&ok,16);                 // СЕКУНДЫ
 bb = QString(sh2.mid(24,8));
 imcek = bb.toUInt(&ok,16);      // миллисек

 qint64 dt = dateJuli + ned*7.0 + ncek/86400.0;
 zagvrv.datn=QDate::fromJulianDay(dt);

 zagvrv.timen=QTime(0,0,0).addSecs(ncek);

 if ((zagvrv.idist == 1031) || (zagvrv.idist == 1032))
 {
  zagvrv.interv=sh2.mid(32,8).toInt(&ok,16)*137.5e-9;	// интервал
  zagvrv.npak=sh2.mid(40,8).toInt(&ok,16);							// номер пакета
 }

 if ((zagvrv.idist == 1033) || (zagvrv.idist == 1034))
 {
  zagvrv.interv=0;
  zagvrv.npak=sh2.mid(38,2).toInt(&ok,16);							// номер пакета
 }
 return;
}

/*!
 * \brief ObrIzm::ReadAltiInList
 * Формирование списка с информацией по кадрам ВРВ из FULL файла.
 * Строка списка : дата,время,мсек,№ канала,№ пакета,№ вирт.канала,смещение в FULL файле,длина пакета.
 * Заполняет переменные по ВРВ в структуре ZapStatFull.
 * \param buf[in] содержимое FULL файла
 * \param list[in,out] не отсортированный список с информацией
 * \param kan[in] № канала FULL файла
 * \param d_full[in] дата сеанса ППСИ
 * \param prot[out] протокол работы
*/
void ObrIzm::ReadAltiInList(QByteArray &buf,QStringList &list,QString kan,QDate d_full,QStringList &prot)
{
 bool pr;
 qint32 kod=0;
 qint32 nvk;
 bool pr_crc;
 QString str,key;
 qint32 inter;
 QString sinter;
 qint32 kolsut;
 bool prcet;
 QString kadr1,kadr2;

 pr=true;
 kod=0;
 key="";
 prcet=false;

 while(pr)
 {
  pr=ReadFULL(buf,kod,nvk,pr_crc); ///Определение смещения в области памяти FULL файла
  if (!pr) break;
  if (kod == 0) kod=1;
  if ((nvk != 1) && (nvk != 3)) {prcet=false;continue;}

  /*
  if ((nvk == 3) && (!prcet))
  {
   if (kan == "1") statfull.och2_vrv1++;
   if (kan == "2") statfull.och2_vrv2++;
   str="Не найден первый кадр. Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   continue;
  }
*/
  if ((nvk == 1) && (kan == "1")) statfull.k1_vrv1++;
  if ((nvk == 3) && (kan == "1")) statfull.k2_vrv1++;
  if ((nvk == 1) && (kan == "2")) statfull.k1_vrv2++;
  if ((nvk == 3) && (kan == "2")) statfull.k2_vrv2++;

  if (!pr_crc)
  {
   if ((nvk == 1) && (kan == "1")) statfull.crc1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.crc2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.crc1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.crc2_vrv2++;
   str="Ошибка контрольной суммы. Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   prcet=false;
   continue;
  }

  if ((nvk == 3) && (!prcet))
  {
   if (kan == "1") statfull.och2_vrv1++;
   if (kan == "2") statfull.och2_vrv2++;
   str="Не найден первый кадр. Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   continue;
  }
  if ((nvk == 1) && (prcet))
  {
   if (kan == "1") statfull.och2_vrv1++;
   if (kan == "2") statfull.och2_vrv2++;
   str="Не найден второй кадр. Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
//   continue;
  }


  if (nvk == 1) ReadZagVRV(buf,ind_full); ///Чтение информации заголовка кадра ВРВ и заполнение структуры zagvrv
//  ReadZagVRV(buf,ind_full); ///Чтение информации заголовка кадра ВРВ и заполнение структуры zagvrv
  if ((zagvrv.idist == 1033) || (zagvrv.idist == 1034)) {prcet=false;continue;}
  if ((zagvrv.idist != 1031) && (zagvrv.idist != 1032))
  {
   if ((nvk == 1) && (kan == "1")) statfull.och1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.och2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.och1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.och2_vrv2++;
   str="Ошибка идентификатора источника : "+QString::number(zagvrv.idist)+".Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   prcet=false;
   continue;
  }
  if (zagvrv.datn > d_full)
  {
   if ((nvk == 1) && (kan == "1")) statfull.och1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.och2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.och1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.och2_vrv2++;
   str="Дата измерения : "+zagvrv.datn.toString("yyyy-MM-dd")+" больше даты сеанса : "+d_full.toString("yyyy-MM-dd")+". Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   prcet=false;
   continue;
  }
  kolsut=zagvrv.datn.daysTo(d_full);
  kolsut=abs(kolsut);
  if (kolsut > 14) ///пропуск измерения, если дата измерения больше 14 суток от даты сеанса ППСИ
  {
   if ((nvk == 1) && (kan == "1")) statfull.och1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.och2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.och1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.och2_vrv2++;
   str="Дата измерения : "+zagvrv.datn.toString("yyyy-MM-dd")+" больше отличается от даты сеанса : "+d_full.toString("yyyy-MM-dd")+" более чем 14 суток. Виртуальный канал : "+QString::number(nvk);
   str=str+". Канал : "+kan+". Смещение : "+QString::number(ind_full);
   prot << str;
   prcet=false;
   continue;
  }

  ///формирование строки - дата,время,мсек,№ канала,№ пакета,№ вирт.канала,смещение в FULL файле,длина пакета
  key=QString::number(zagvrv.npak);
  key=QString("%1").arg(key,15,'0');
  inter=zagvrv.interv*100000;
  sinter=QString::number(inter);
  sinter=QString("%1").arg(sinter,6,'0');
  str=zagvrv.datn.toString("yyyy-MM-dd ")+zagvrv.timen.toString("hh:mm:ss ")+sinter+" "+kan+" "+key+" ";
  str=str+QString::number(nvk)+" "+QString::number(ind_full)+" "+QString::number(zagvrv.dlpak);

  ///запись строки в список
  if (nvk == 1) {kadr1=str; prcet=true;}
  if (nvk== 3) {kadr2=str; list << kadr1; list << kadr2; prcet=false;}
 }

 return;
}

/*!
 * \brief ObrIzm::FormZagTelVRV
 * Формирование заголовка ALTI, CALL файлов
 * \param buf[out] область памяти со сформированным заголовком
 * \param idist[in] идентификатор типа файла: 1031,1032 - ALTI; 1033,1034 - CALL.
 * \param isx_file[in] имя исходного файла FULL, заносимого в заголовок
 * \param vix_file[in] имя выходного файла ALTI,CALL, заносимого в заголовок
 * \param kolpak[in] количество пакетов в файле
*/
void ObrIzm::FormZagTelVRV(QByteArray &buf,qint32 idist,QString isx_file,QString vix_file,qint64 kolpak)
{
 QByteArray buf1,buf2;
 QDataStream s(&buf1,QIODevice::WriteOnly);
 qint32 id=20;
 qint32 rez=0;
 qint32 dlzag;

 buf.clear();

 if ((idist == 1031) || (idist == 1032)) buf.append("ALTI_LGB");
 if ((idist == 1033) || (idist == 1034)) buf.append("CALL_12B");

 buf2.append("[Header]\nAPID= 20").append("\nAuteur= ConvertCOGI")
    .append("\nDate= ").append(QDate::currentDate().toString("dd/MM/yyyy"))
    .append("\nHeure= ").append(QTime::currentTime().toString("hh:mm:ss.zzz"))
    .append("\nSource file= ").append(isx_file)
    .append("\nOutput file= ").append(vix_file)
    .append("\nContexte= acquisition TM1553\n");

 dlzag=buf2.size()+32;

 s << id;
 s << rez;
 s << kolpak;
 s << dlzag;
 s << rez;

 buf.append(buf1);
 buf.append(buf2);

 return;
}

/*!
 * \brief ObrIzm::FormTelPackVRV
 * Формирование пакета ВРВ из FULL файла(файлов)
 * \param pack[out] содержимое пакета ВРВ
 * \param full1[in] содержимое FULL файла для первой половины пакета
 * \param ind_full1[in] смещение в FULL файле для первой половины пакета
 * \param full2[in] содержимое FULL файла для второй половины пакета
 * \param ind_full2[in] смещение в FULL файле для второй половины пакета
 * \param dlpack[in] длина пакета
*/
void ObrIzm::FormTelPackVRV(QByteArray &pack,QByteArray &full1,quint64 ind_full1,QByteArray &full2,quint64 ind_full2,qint32 dlpack)
{
 QByteArray buf1,buf2;

 pack.clear();

 buf1=full1.mid(ind_full1+6,512);
 buf2=full2.mid(ind_full2+6,dlpack+7-512);

 pack.append(buf1);
 pack.append(buf2);

 return;
}

bool ObrIzm::CreateAltiVRV(QByteArray &buf,QByteArray &buf1,QByteArray &buf2,qint32 &ind,QStringList &list,QDate &dn,QTime &tn,qint64 &kolpak,qint32 inter,bool prdat)
{
 qint32 kolzap;
 QDate dk;
 QTime tk;
 quint64 obr[2][2];
 qint32 pakn;
 QByteArray bufp;
 qint32 hn,hk;
 qint64 sm1,sm2,sm11,sm12;
 qint32 dlpak;

 if (list.isEmpty()) return false;
 buf.clear();
 kolzap=list.count();
 if (kolzap-1 <= ind) return false;

 ReadListAlti(list.at(ind));
 dn=listalti.dat;
 tn=listalti.tm;
 dk=dn; tk=tn;
 pakn=listalti.npak;
 dlpak=listalti.dlpack;
 kolpak=0;
 obr[0][0]=0;obr[0][1]=0;obr[1][0]=0;obr[1][1]=0;
 hn=tn.hour(); hk=hn;

 for (int i=ind; i<kolzap; i++)
 {
  if (i>ind) ReadListAlti(list.at(i));

  if (listalti.dlpack != 929) continue;

  tk=listalti.tm; hk=tk.hour();
  if (prdat)
  {
   if (dn != listalti.dat)
   {
    sm1=0;sm2=0;sm11=0;sm12=0;
    sm1=obr[0][0];
    sm2=obr[0][1];
    if (sm1 == 0) sm11=obr[1][0];
    if (sm2 == 0) sm12=obr[1][1];
    ObrPackVRV(bufp,buf1,buf2,listalti.dlpack,sm1,sm2,sm11,sm12,kolpak);
    if (!bufp.isEmpty())
    {
     buf.append(bufp);
     statfull.k_vrv++;
    }
    ind=i;
    return true;
   }
  }
  if (inter > 0)
  {
   if (hk-hn > inter-1)
   {
    sm1=0;sm2=0;sm11=0;sm12=0;
    sm1=obr[0][0];
    sm2=obr[0][1];
    if (sm1 == 0) sm11=obr[1][0];
    if (sm2 == 0) sm12=obr[1][1];
    ObrPackVRV(bufp,buf1,buf2,listalti.dlpack,sm1,sm2,sm11,sm12,kolpak);
    if (!bufp.isEmpty())
    {
     buf.append(bufp);
     statfull.k_vrv++;
    }
    ind=i;
    return true;
   }
  }
  if (pakn != listalti.npak)
  {
   sm1=0;sm2=0;sm11=0;sm12=0;
   sm1=obr[0][0];
   sm2=obr[0][1];
   if (sm1 == 0) sm11=obr[1][0];
   if (sm2 == 0) sm12=obr[1][1];
   ObrPackVRV(bufp,buf1,buf2,dlpak,sm1,sm2,sm11,sm12,kolpak);
   if (!bufp.isEmpty())
   {
    buf.append(bufp);
    statfull.k_vrv++;
   }
   pakn=listalti.npak;
   dlpak=listalti.dlpack;
   obr[0][0]=0;obr[0][1]=0;obr[1][0]=0;obr[1][1]=0;
  }
  if (obr[listalti.kan-1][listalti.idist-1] == 0) obr[listalti.kan-1][listalti.idist-1]=listalti.sm;
 }

 sm1=0;sm2=0;sm11=0;sm12=0;
 sm1=obr[0][0];
 sm2=obr[0][1];
 if (sm1 == 0) sm11=obr[1][0];
 if (sm2 == 0) sm12=obr[1][1];
 ObrPackVRV(bufp,buf1,buf2,listalti.dlpack,sm1,sm2,sm11,sm12,kolpak);
 if (!bufp.isEmpty())
 {
  buf.append(bufp);
  statfull.k_vrv++;
 }

 ind=kolzap-1;

 return true;
}

void ObrIzm::ReadListAlti(QString str)
{
 QString str1,str2;

 str1=str.simplified();

 str2=str1.section(" ",0,0);
 listalti.dat=QDate::fromString(str2,"yyyy-MM-dd");

 str2=str1.section(" ",1,1);
 listalti.tm=QTime::fromString(str2,"hh:mm:ss");

 str2=str1.section(" ",2,2);
 listalti.msec=str2.toInt();

 str2=str1.section(" ",3,3);
 listalti.kan=str2.toInt();

 str2=str1.section(" ",4,4);
 listalti.npak=str2.toInt();

 str2=str1.section(" ",5,5);
 listalti.idist=str2.toInt();
 if (listalti.idist == 3) listalti.idist=2;

 str2=str1.section(" ",6,6);
 listalti.sm=str2.toLong();

 str2=str1.section(" ",7,7);
 listalti.dlpack=str2.toInt();

 return;
}

QStringList ObrIzm::CreateAllAltiVRV(QString full_file1,QString full_file2,QString path,QString nka,qint32 inter,bool prdat,QStringList &prot)
{
 QStringList nfile;
 QByteArray buf1,buf2,buf0;
 QDateTime dtn,dtk;

 buf1=ReadFile(full_file1);
 buf2=ReadFile(full_file2);

 PrefFull(buf1,dtn,dtk);

 nfile=CreateAllAltiVRV(buf1,buf2,path,nka,inter,full_file1,buf0,prdat,dtn.date(),prot,0);

 return nfile;
}

QStringList ObrIzm::CreateAllAltiVRV(QByteArray &buf1, QByteArray &buf2, QString path, QString nka, qint32 inter,QString nfile1,QByteArray &bufz,bool prdat,QDate d_full,QStringList &prot,qint32 nkan)
{
 QStringList nfile;
 QByteArray buf;
 QStringList list;
 QDate dn;
 QTime tn;
 qint32 ind=0;
 bool ret=true;
 QString str;
 qint64 kolpak;
 qint32 idist=1031;
 QStringList inff;
 QString isx_file;

 ClearStatFull();
 prot.clear();

 ReadAltiInList(buf1,list,"1",d_full,prot);
 ReadAltiInList(buf2,list,"2",d_full,prot);
 if (list.isEmpty()) return nfile;
 qSort(list);

 inff=FileInfo(nfile1);
 isx_file=inff[2];

 bufz.clear();

 while (ret)
 {
  ret=CreateAltiVRV(buf,buf1,buf2,ind,list,dn,tn,kolpak,inter,prdat);
  if (!ret) break;
  if (buf.isEmpty()) continue;
  str="ALTI_LG_"+dn.toString("yy.MM.dd")+"_"+tn.toString("hh.mm.ss")+"."+nka;
  if (nkan > 0) str="ALTI_LG_"+dn.toString("yy.MM.dd")+"_"+tn.toString("hh.mm.ss")+"_"+QString::number(nkan)+"."+nka;
  FormZagTelVRV(bufz,idist,isx_file,str,kolpak);
  bufz.append(buf);
  WriteFile(path+"/"+str,bufz,true);
  nfile << path+"/"+str;
 }

 return nfile;
}

void ObrIzm::ReadCallInList(QByteArray &buf, QStringList &list, QString kan,qint32 idist,QDate d_full)
{
 bool pr;
 qint32 kod=0;
 qint32 nvk;
 bool pr_crc;
 QString str,key;
 bool prcet;
 qint64 kolsut;
 QString kadr1,kadr2;

 if ((idist != 1033) && (idist != 1034)) return;

 pr=true;
 kod=0;
 key="";
 prcet=false;

 while(pr)
 {
  pr=ReadFULL(buf,kod,nvk,pr_crc);
  if (!pr) break;
  if (kod == 0)
  {
   if (!pr) return;
   kod=1;
  }
  if ((nvk != 1) && (nvk != 3)) {prcet=false;continue;}
  if (!pr_crc) {prcet=false;continue;}
  if ((nvk == 3) && (!prcet)) continue;
  if (nvk == 1) ReadZagVRV(buf,ind_full);

  if (zagvrv.datn > d_full)
  {
   if ((nvk == 1) && (kan == "1")) statfull.och1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.och2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.och1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.och2_vrv2++;
   prcet=false;
   continue;
  }
  kolsut=zagvrv.datn.daysTo(d_full);
  kolsut=abs(kolsut);
  if (kolsut > 14) ///пропуск измерения, если дата измерения больше 14 суток от даты сеанса ППСИ
  {
   if ((nvk == 1) && (kan == "1")) statfull.och1_vrv1++;
   if ((nvk == 3) && (kan == "1")) statfull.och2_vrv1++;
   if ((nvk == 1) && (kan == "2")) statfull.och1_vrv2++;
   if ((nvk == 3) && (kan == "2")) statfull.och2_vrv2++;
   prcet=false;
   continue;
  }

  if (zagvrv.idist != idist) {prcet=false;continue;}

  key=QString::number(zagvrv.chpak);
  key=QString("%1").arg(key,15,'0');
  str=zagvrv.datn.toString("dd-MM-yyyy ")+zagvrv.timen.toString("hh:mm:ss ")+" "+key+" "+kan+" ";
  str=str+QString::number(nvk)+" "+QString::number(ind_full)+" "+QString::number(zagvrv.dlpak);
  if (nvk == 1) {kadr1=str; prcet=true;}
  if (nvk == 3) {kadr2=str; list << kadr1; list << kadr2; prcet=false;}
 }

 return;
}

bool ObrIzm::CreateCallVRV(QByteArray &buf, QByteArray &buf1, QByteArray &buf2, qint32 &ind, QStringList &list, QDate &dn, QTime &tn, qint64 &kolpak)
{
 qint32 kolzap;
 QDate dk;
 QTime tk;
 quint64 obr[2][2];
 qint32 pakn;
 QByteArray bufp;
 qint32 hn,hk;
 qint64 sm1,sm2,sm11,sm12;

 if (list.isEmpty()) return false;
 buf.clear();
 kolzap=list.count();
 if (kolzap-1 <= ind) return false;

 ReadListCall(list.at(ind));
 dn=listcall.dat;
 tn=listcall.tm;
 dk=dn; tk=tn;
 pakn=listcall.pak;
 kolpak=0;
 obr[0][0]=0;obr[0][1]=0;obr[1][0]=0;obr[1][1]=0;
 hn=tn.hour(); hk=hn;

 for (int i=ind; i<kolzap; i++)
 {
  if (i>ind) ReadListCall(list.at(i));
  tk=listcall.tm; hk=tk.hour();
  if ((dn != listcall.dat) || (hk-hn > 1))
//  if (dn != listcall.dat)
  {
   sm1=0;sm2=0;sm11=0;sm12=0;
   sm1=obr[0][0];
   sm2=obr[0][1];
   if (sm1 == 0) sm11=obr[1][0];
   if (sm2 == 0) sm12=obr[1][1];
   ObrPackVRV(bufp,buf1,buf2,listcall.dlpack,sm1,sm2,sm11,sm12,kolpak);
   if (!bufp.isEmpty()) buf.append(bufp);
   ind=i;
   return true;
  }
  if (pakn != listcall.pak)
  {
   sm1=0;sm2=0;sm11=0;sm12=0;
   sm1=obr[0][0];
   sm2=obr[0][1];
   if (sm1 == 0) sm11=obr[1][0];
   if (sm2 == 0) sm12=obr[1][1];
   ObrPackVRV(bufp,buf1,buf2,listcall.dlpack,sm1,sm2,sm11,sm12,kolpak);
   if (!bufp.isEmpty())
   {
    statfull.k_cal++;
    buf.append(bufp);
   }
   pakn=listcall.pak;
   obr[0][0]=0;obr[0][1]=0;obr[1][0]=0;obr[1][1]=0;
  }
  if (obr[listcall.kan-1][listcall.vk-1] == 0) obr[listcall.kan-1][listcall.vk-1]=listcall.sm;
 }

 sm1=0;sm2=0;sm11=0;sm12=0;
 sm1=obr[0][0];
 sm2=obr[0][1];
 if (sm1 == 0) sm11=obr[1][0];
 if (sm2 == 0) sm12=obr[1][1];
 ObrPackVRV(bufp,buf1,buf2,listcall.dlpack,sm1,sm2,sm11,sm12,kolpak);
 if (!bufp.isEmpty())
 {
  statfull.k_cal++;
  buf.append(bufp);
 }

 ind=kolzap-1;

 return true;
}

void ObrIzm::ReadListCall(QString str)
{
 QString str1,str2;

 str1=str.simplified();

 str2=str1.section(" ",0,0);
 listcall.dat=QDate::fromString(str2,"dd-MM-yyyy");

 str2=str1.section(" ",1,1);
 listcall.tm=QTime::fromString(str2,"hh:mm:ss");

 str2=str1.section(" ",2,2);
 listcall.pak=str2.toInt();

 str2=str1.section(" ",3,3);
 listcall.kan=str2.toInt();

 str2=str1.section(" ",4,4);
 listcall.vk=str2.toInt();
 if (listcall.vk == 3) listcall.vk=2;

 str2=str1.section(" ",5,5);
 listcall.sm=str2.toLong();

 str2=str1.section(" ",6,6);
 listcall.dlpack=str2.toInt();

 return;
}

QStringList ObrIzm::CreateAllCallVRV(QString full_file1, QString full_file2, QString path, QString nka,qint32 idist,QByteArray &buf,QDate d_full)
{
 QStringList nfile;
 QByteArray buf1,buf2;

 buf1=ReadFile(full_file1);
 buf2=ReadFile(full_file2);

 nfile=CreateAllCallVRV(buf1,buf2,path,nka,idist,full_file1,buf,0,d_full);

 return nfile;
}

QStringList ObrIzm::CreateAllCallVRV(QString full_file, QString path, qint32 idist,QDate d_full)
{
 QStringList nfile;
 QByteArray buf,buf1,buf2;
 QStringList fi;
 QString str,nka;
 qint32 kan,ind;

 buf1=ReadFile(full_file);
 buf2=ReadFile(full_file);

 fi=FileInfo(full_file);
 str=fi[3];
 kan=str.mid(18,1).toInt();
 nka=fi[4];
 if (nka == "bin" || nka == "BIN") nka="126";

 ind=str.indexOf("(");
 if (ind != -1) nka=str.mid(ind+1,3);

 nfile=CreateAllCallVRV(buf1,buf2,path,nka,idist,full_file,buf,kan,d_full);

 return nfile;
}

QStringList ObrIzm::CreateAllCallVRV(QByteArray &buf1,QByteArray &buf2, QString path, QString nka,qint32 idist,QString file1,QByteArray &bufz,qint32 kan,QDate d_full)
{
 QStringList nfile;
 QByteArray buf;
 QStringList list;
 QDate dn;
 QTime tn;
 qint32 ind=0;
 bool ret=true;
 QString str;
 qint64 kolpak;
 QStringList inff;
 QString isx_file;
 QString zagf,kanf;

 ReadCallInList(buf1,list,"1",idist,d_full);
 ReadCallInList(buf2,list,"2",idist,d_full);
 qSort(list);

 inff=FileInfo(file1);
 isx_file=inff[2];

 if (idist == 1033) zagf="TM_CALL_1_";
 if (idist == 1034) zagf="TM_CALL_2_";
 kanf.clear();
 if (kan == 1) kanf="_m1B";
 if (kan == 2) kanf="_m2G";

 bufz.clear();

 while (ret)
 {
  ret=CreateCallVRV(buf,buf1,buf2,ind,list,dn,tn,kolpak);
  if (!ret) break;
  str=zagf+dn.toString("yy.MM.dd")+"_"+tn.toString("hh.mm.ss")+kanf+"."+nka;
  FormZagTelVRV(bufz,idist,isx_file,str,kolpak);
  bufz.append(buf);
  WriteFile(path+"/"+str,bufz,true);
  nfile  << path+"/"+str;
 }

 return nfile;
}

void ObrIzm::ObrPackVRV(QByteArray &buf,QByteArray &buf1, QByteArray &buf2,qint32 dlpack,quint64 sm1,quint64 sm2,quint64 sm11,quint64 sm12,qint64 &kolpak)
{
 buf.clear();

 if ((sm1>0) && (sm2>0))
 {
  FormTelPackVRV(buf,buf1,sm1,buf1,sm2,dlpack);
  kolpak++;
  return;
 }
 if ((sm11>0) && (sm12>0))
 {
  FormTelPackVRV(buf,buf2,sm11,buf2,sm12,dlpack);
  kolpak++;
  return;
 }
 if ((sm1>0) && (sm12>0))
 {
  FormTelPackVRV(buf,buf1,sm1,buf2,sm12,dlpack);
  kolpak++;
  return;
 }
 if ((sm11>0) && (sm2>0))
 {
  FormTelPackVRV(buf,buf2,sm11,buf1,sm2,dlpack);
  kolpak++;
  return;
 }

 return;
}

bool ObrIzm::ReadAltiInList(QByteArray &buf, QStringList &list)
{
 qint64 ind=0;
 qint32 dls,id,rez;
 qint64 kolpak;
 QByteArray shap;
 QDataStream s(&shap,QIODevice::ReadOnly);
 qint64 dlf;
 QString str;
 QString spak,schpak;

 if (buf.isEmpty()) return false;

 dlf=buf.size();

 shap=buf.mid(8,20);
 s >> id >> rez >> kolpak >> dls;

 if (dls == 0) return false;

 ind=ind+dls;

 for (int i=0; i<kolpak; i++)
 {
  ReadZagVRV(buf,ind-6);
  spak=QString::number(zagvrv.npak);
  schpak=QString::number(zagvrv.chpak);
  str=zagvrv.datn.toString("dd-MM-yy")+" "+zagvrv.timen.toString("hh:mm:ss");
  str=str+" "+QString("%1").arg(spak,15,'0')+" "+QString("%1").arg(schpak,10,'0');
  list << str;
  ind=ind+zagvrv.dlpak+7;
 };

 return true;
}

/*!
 * \brief ObrIzm::NameVRVFile
 * Формирование имени ALTI,CALL файла
 * \param pref[in] тип формируемого имени: "ALTI","CALL"
 * \param dat[in] дата первой эпохи в файле
 * \param time[in] время первой эпохи в файле
 * \param ncall[in] номер типа калибровки для CALL файла
 * \param kan[in] номер канала 1 или 2. При других значениях параметр игнорируется, номер канала в имя файла не записывается.
 * \param nka[in] номер КА. Трехзначное цифровое значение. Используется для расширения файла
 * \return возвращает имя файла без расширения
*/
QString ObrIzm::NameVRVFile(QString pref,QDate dat,QTime time,qint16 ncall,qint16 kan,qint32 nka)
{
 QString name;
 QString str;

 if (pref == "ALTI") name="ALTI_LG_";
 if (pref == "CALL") name="TM_CALL_"+QString::number(ncall)+"_";

 str=dat.toString("yy.MM.dd");
 name=name+str+"_";
 str=time.toString("hh.mm.ss");
 name=name+str;

 if (kan == 1) name=name+"_m1B";
 if (kan == 2) name=name+"_m2G";

 name=name+"."+QString::number(nka);

 return name;
}

/*!
 * \brief ObrIzm::PrefFull
 * определение даты и времени начала и конца сеанса ППСИ
 * из заголовка FULL файла
 * \param buf[in] содержимое FULL файла
 * \param dtn[out] дата и время начала сеанса
 * \param dtk[out] дата и время конца сеанса
*/
void ObrIzm::PrefFull(QByteArray &buf,QDateTime &dtn,QDateTime &dtk)
{
 QString str,str1;
 int pos=0;
 qint32 ind;

 ind=buf.indexOf("FULL_SS_");
 if ((ind >= 0) && (ind < 136)) pos=136;

 str=QString(buf.mid(pos,16));
 str=str.mid(0,6)+"20"+str.mid(6);
 dtn.setDate(QDate::fromString(str.mid(0,10),"dd.MM.yyyy"));
 dtn.setTime(QTime::fromString(str.mid(10),"hh:mm:ss"));
 str1=QString(buf.mid(pos+20,16));
 str1=str1.mid(0,6)+"20"+str1.mid(6);
 dtk.setDate(QDate::fromString(str1.mid(0,10),"dd.MM.yyyy"));
 dtk.setTime(QTime::fromString(str1.mid(10),"hh:mm:ss"));

 return;
}

bool ObrIzm::ZapEduch(QString nfile,qulonglong &nz,bool prud,bool prsoob)
{
 QStringList lf;
 qulonglong nz1;
 QString ext;
 qint32 g;
 QString nazf;
 QByteArray buf;
 qint32 kolsut;
 QStringList err;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[3];
 ext=ext.toUpper();
 nazf=nazf.toUpper();

 if (ext == "322") //сделано в нов.табл
 {
  if (! SetDzs(nfile)) return false;
  nz=InsertDs(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("DZS",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   return false;
  }

  if (!UpdateRow("ds","nz_edxr",nz1,nz,prsoob))
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   DeleteRow("edxr",nz1,prsoob);
   return false;
  }

  zapstat322.nz_educh=nz;
  InsertStat322(prsoob);

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "313") //сделано в нов.табл
 {
  if (! SetDs(nfile)) return false;
  nz=InsertDs(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("DS",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   return false;
  }

 if (!UpdateRow("ds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("ds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  zapstat313.nz_educh=nz;
  InsertStat313(prsoob);

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "SPO") //сделано в нов.табл
 {
  if (! SetSpo(nfile)) return false;
  nz=InsertDs(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("SPO",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   return false;
  }

 if (!UpdateRow("ds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("ds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext.right(3) == "RNX")
 {
  if (nazf.right(2) == "MO")
  {
   SetRnxO(nfile);
   zaprnx.kodobr=2;
   if (zaprnx.ver == "3.02")
   {
    if (! ProvRnxFile(nfile,false,false))
    {
     zaprnx.strk=2;
     zaprnx.kodobr=1;
    }
   }
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("IMO",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

  if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("rnxi",nz,prsoob);
   DeleteRow("edxr",nz1,prsoob);
   return false;
  }

   if (prud) DelFile(nfile);
   return true;
  }

  if (nazf.right(2) == "MN")
  {
   SetRnxNG(nfile,"N",kolsut);
   if (zaprnx.ver == "3.02")
   {
    zaprnx.kodobr=7;
   }
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("IMN",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

  if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("rnxi",nz,prsoob);
   DeleteRow("edxr",nz1,prsoob);
   return false;
  }

   if (prud) DelFile(nfile);
   return true;
  }
 }

 if ((ext.right(3) == "BIN") || (ext.right(3) == "126") || (ext.right(3) == "127"))
 {
  if (! SetFull(nfile,buf)) return false;
  nz=InsertFull(prsoob);
  if (nz == 0) return false;
  lf=FileInfo(nfile);
  nz1=ZapEdxr("FULL",nz,buf,0,prsoob,lf);
  buf.clear();
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("fullppsi",nz,prsoob);
   return false;
  }
  if (!UpdateRow("fullppsi","nz_edxr",nz1,nz,prsoob))
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("fullppsi",nz,prsoob);
   DeleteRow("edxr",nz1,prsoob);
   return false;
  }

  statfull.nz_fullppsi=nz;
  InsertStatFull(prsoob);
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "KOS") //сделано в нов.табл
 {
  if (! SetKos(nfile)) return false;
  nz=InsertDs(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("KOS",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   return false;
  }

 if (!UpdateRow("ds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("ds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "FRD") //сделано в нов.табл
 {
  if (! SetFrd(nfile)) return false;
  nz=InsertDs(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("FRD",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("ds",nz,prsoob);
   return false;
  }

 if (!UpdateRow("ds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("ds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "ALT")
 {
  if (!SetAltInBD(nfile,prsoob)) return false;
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "PPSI")
 {
  if (!SetPpsi(nfile,prsoob)) return false;
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "PLA")
 {
  if (!SetPla(nfile,prsoob)) return false;
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "SP3") //сделано в нов.табл
 {
  if (! SetSP3(nfile)) return false;
  nz=InsertSP3(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("SP3",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("sp3",nz,prsoob);
   return false;
  }

 if (!UpdateRow("sp3","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("sp3",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "SOL") //сделано в нов.табл
 {
  if (! SetSol(nfile)) return false;
  nz=InsertSol(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("SOL",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("sol",nz,prsoob);
   return false;
  }

  if (!UpdateRow("sol","nz_edxr",nz1,nz,prsoob))
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("sol",nz,prsoob);
   DeleteRow("edxr",nz1,prsoob);
   return false;
  }

  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "ASPSI")
 {
  if (!SetAspsi(nfile,prsoob)) return false;
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext == "SEC")
 {
  if (!SetLeap(nfile,prsoob)) return false;
  if (prud) DelFile(nfile);
  return true;
 }

 if (ext.right(1) == "M")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   if (! SetM(nfile)) return false;
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("M",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

   if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    DeleteRow("edxr",nz1,prsoob);
    return false;
   }

   if (prud) DelFile(nfile);
   return true;
  }
 }

 if (ext.right(1) == "O")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   SetRnxO(nfile);
   if (zaprnx.strk == 0)
   {
    if (zaprnx.ver.left(1) == "2")
    {
     zaprnx.kodobr=21;

     /*
     QFile *file = new QFile(nfile);
     if (!test_rinex_V2(file,err))
     {
      zaprnx.strk=2;
      zaprnx.kodobr=1;
     }
     */
     if (! ProvRnx2File(nfile,false))
     {
      zaprnx.strk=2;
      zaprnx.kodobr=1;
     }

    }
    if (zaprnx.ver.left(1) == "3")
    {
     zaprnx.kodobr=2;
     if (! ProvRnxFile(nfile,false,false))
     {
      zaprnx.strk=2;
      zaprnx.kodobr=1;
     }
    }
   }
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("IO",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

   if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    DeleteRow("edxr",nz1,prsoob);
    return false;
   }

   if (prud) DelFile(nfile);
   return true;
  }
 }

 if ((ext.right(1) == "G") || (ext.right(1) == "N"))
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   SetRnxNG(nfile,ext.right(1),kolsut);
   if (zaprnx.kodobr == 0)
   {
       if (zaprnx.ver.left(1) == "2")
       {
        zaprnx.kodobr=22;
        if (!ProvRnxFileN2(nfile,false))
        {
         zaprnx.strk=2;
         zaprnx.kodobr=1;
        }
       }
       if (zaprnx.ver.left(1) == "3")
       {
        zaprnx.kodobr=7;
        if (!ProvRnxFileN(nfile,false))
        {
         zaprnx.strk=2;
         zaprnx.kodobr=1;
        }
       }
   }
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("IN",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

   if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    DeleteRow("edxr",nz1,prsoob);
    return false;
   }

   if (prud) DelFile(nfile);
   return true;
  }
 }

 if (ext.right(1) == "L")
 {
  g=ext.left(2).toInt();
  if (g >= 0)
  {
   SetL(nfile);
   nz=InsertRnx("rnxi",prsoob);
   if (nz == 0) return false;
   nz1=ZapEdxr("L",nz,nfile,0,prsoob);
   if (nz1 == 0)
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    return false;
   }

   if (!UpdateRow("rnxi","nz_edxr",nz1,nz,prsoob))
   {
    DelFile(tek_path_arx+"/"+zapedxr.arxf);
    DeleteRow("rnxi",nz,prsoob);
    DeleteRow("edxr",nz1,prsoob);
    return false;
   }

   if (prud) DelFile(nfile);
   return true;
  }
 }

 return false;
}

bool ObrIzm::SetDs(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString stime;

 ClearDs();

 lf=FileInfo(nfile);
 zapds.naz=lf[3];
 zapds.naz=zapds.naz.toUpper();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 str=list[0];
 str=str.simplified();

 zapds.npn=str.section(" ",1,1);
 zapds.godn=str.section(" ",4,4).toInt()+2000;
 zapds.sutn=str.section(" ",5,5).toInt();
 zapds.sekn=str.section(" ",6,6).toInt();
 zapds.nka=str.section(" ",0,0);
 zapds.nkis=str.section(" ",2,2);
 zapds.sv=str.section(" ",3,3).toInt();

 str=list[kolstr-1];
 str=str.simplified();

 zapds.godk=str.section(" ",4,4).toInt()+2000;
 zapds.sutk=str.section(" ",5,5).toInt();
 zapds.sekk=str.section(" ",6,6).toInt();

 dat=DayToDate(zapds.sutn,zapds.godn);
 time=SecToTime(zapds.sekn,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 dat=DayToDate(zapds.sutk,zapds.godk);
 time=SecToTime(zapds.sekk,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapds.nazis="DS";

 zapds.nz_edxr=0;

 Stat313(list);

 return true;
}

quint64 ObrIzm::ZapEdxr(QString pref,quint64 nz_tab,QString nfile,qint16 prarx,bool prsoob,QSqlQuery query)
{
 QByteArray buf;
 int ret;
 quint64 nz=0;

 if (! SetEdxrN(pref,nz_tab,nfile,prarx,buf,prsoob,query)) return 0;

 ret=WriteFile(tek_path_arx+"/"+zapedxr.arxf,buf,true);
 if (ret == -1) return 0;

 nz=InsertEdxr(prsoob,query);

 return nz;
}

quint64 ObrIzm::ZapEdxr(QString pref,quint64 nz_tab,QString nfile,qint16 prarx,bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=ZapEdxr(pref,nz_tab,nfile,prarx,prsoob,query);

 return nz;
}

quint64 ObrIzm::ZapEdxr(QString pref,quint64 nz_tab,QByteArray &buf ,qint16 prarx,bool prsoob,QStringList lf)
{
 qint64 ret;
 quint64 nz=0;
 QString err;

 if (! SetEdxrN(pref,nz_tab,prarx,buf,lf,prsoob)) return 0;

 ret=WriteFile(tek_path_arx+"/"+zapedxr.arxf,buf,true,err);
 if (ret == -1)
 {
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),err,QMessageBox::Yes);
  }
  return 0;
 }

 nz=InsertEdxr(prsoob);

 return nz;
}

bool ObrIzm::SetEdxrN(QString pref,qint64 nz_tab,QString nfile,qint16 prarx,QByteArray &buf,bool prsoob,QSqlQuery query)
{
 qulonglong idlf,adlf;
 qint16 iks,aks;
 QStringList lf;

// if (tek_nz_nos == 0)
// {
//  if (! GetNos(prsoob,query)) return false;
// }

 if (! GetNos(prsoob,query)) return false;

 ClearEdxr();

 if (! ArxFile(nfile,prarx,idlf,iks,adlf,aks,buf)) return false;

 lf=FileInfo(nfile);

 zapedxr.isxf=lf[2];
 zapedxr.arxf=pref+"_"+QString::number(nz_tab)+"_"+lf[2];
 zapedxr.arxf=zapedxr.arxf.toUpper();
 zapedxr.idlf=idlf;
 zapedxr.iks=iks;
 zapedxr.adlf=adlf;
 zapedxr.aks=aks;
 zapedxr.datp=QDate::currentDate();
 zapedxr.datr=QDate::currentDate();
 zapedxr.prarx=prarx;
 zapedxr.nz_nos=tek_nz_nos;

 return true;
}

bool ObrIzm::SetEdxrN(QString pref,qint64 nz_tab,QString nfile,qint16 prarx,QByteArray &buf,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=SetEdxrN(pref,nz_tab,nfile,prarx,buf,prsoob,query);

 return ret;
}

bool ObrIzm::SetEdxrN(QString pref,qint64 nz_tab,qint16 prarx,QByteArray &buf,QStringList lf,bool prsoob)
{
 qulonglong idlf,adlf;
 qint16 iks,aks;
 QSqlQuery query;

 // if (tek_nz_nos == 0)
 // {
 //  if (! GetNos(prsoob,query)) return false;
 // }

  if (! GetNos(prsoob,query)) return false;

 ClearEdxr();

 if (! ArxFileInMem(prarx,idlf,iks,adlf,aks,buf)) return false;

 zapedxr.isxf=lf[2];
 zapedxr.arxf=pref+"_"+QString::number(nz_tab)+"_"+lf[2];
 zapedxr.arxf=zapedxr.arxf.toUpper();
 zapedxr.idlf=idlf;
 zapedxr.iks=iks;
 zapedxr.adlf=adlf;
 zapedxr.aks=aks;
 zapedxr.datp=QDate::currentDate();
 zapedxr.datr=QDate::currentDate();
 zapedxr.prarx=prarx;
 zapedxr.nz_nos=tek_nz_nos;

 return true;
}

bool ObrIzm::SetFull(QString nfile,QByteArray &buf)
{
 QStringList lf;
 QDate dat;
 QTime time,time1;
 QDateTime dtn,dtk;
 qint32 nvk;
 bool pr_crc;
 qint32 indstr;
 qint32 h,m;

 ClearFullppsi();

 if (!ReadFile(nfile,buf)) return false;

 PrefFull(buf,dtn,dtk);

 lf=FileInfo(nfile);
 zapfull.naz=lf[3];
 zapfull.naz=zapfull.naz.toUpper();

 if (zapfull.naz.mid(0,4) == "FULL")
 {
//  zapfull.kan=zapfull.naz.mid(18,1).toInt();
  zapfull.npn="624P";
 }
 else
 {
//  zapfull.kan=zapfull.naz.mid(23,1).toInt();
  zapfull.npn=zapfull.naz.mid(0,4);
 }

 indstr=zapfull.naz.indexOf("M",0);
 if (indstr == -1) zapfull.kan=0;
 else zapfull.kan=zapfull.naz.mid(indstr+1,1).toInt();

 zapfull.nka=lf[4];
 zapfull.nka=zapfull.nka.toUpper();
 if (zapfull.nka == "BIN") zapfull.nka="126";
 indstr=zapfull.naz.indexOf("(");
 if (indstr >= 0)
 {
  zapfull.nka=zapfull.naz.mid(indstr+1,3);
 }

 m=zapfull.naz.section(".",-1,-1).mid(0,2).toInt();
 h=zapfull.naz.section(".",-2,-2).toInt();
 time1.setHMS(h,m,0);

 time=dtn.time();
 zapfull.sekn=TimeToSec(time);
 time=dtk.time();
 zapfull.sekk=TimeToSec(time);
 if ((zapfull.sekn == 0) && (zapfull.sekk == 0))
 {
  dtn.setTime(time1);
  dtk.setTime(time1);
 }

 zapfull.dtn=dtn.toString("dd-MM-yyyy hh:mm");
 zapfull.dtk=dtk.toString("dd-MM-yyyy hh:mm");
 dat=dtn.date();
 zapfull.godn=dat.year();
 zapfull.sutn=dat.dayOfYear();
 time=dtn.time();
 zapfull.sekn=TimeToSec(time);
 dat=dtk.date();
 zapfull.godk=dat.year();
 zapfull.sutk=dat.dayOfYear();
 time=dtk.time();
 zapfull.sekk=TimeToSec(time);
 zapfull.nz_edxr=0;
 zapfull.strk=0;
 zapfull.kodobr=11;
 if (!ReadFULL(buf,0,nvk,pr_crc))
 {
  zapfull.strk=1;
  zapfull.kodobr=1;
 }
 StatFull(buf,false,"");
 return true;
}

/*!
 * \brief ObrIzm::ObrKod11_15
 * Обработка кодов 11 (конвертирование КВИ) и 15 (конвертирование КВНО)
 * \return
*/
bool ObrIzm::ObrKod11_15(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list,list_kvi,list_kvno;
 bool ret;

 //заполнение списка ключей из таблицы fullppsi для обработки
 list_kvi=GetKodobr("fullppsi",11,prsoob);
 list_kvno=GetKodobr("fullppsi",15,prsoob);
 list.clear();

 AddList(list,list_kvi);
 AddList(list,list_kvno);

 ret=ObrKod11_15(list,kolobr,path_temp,prsoob);

 return ret;
}

bool ObrIzm::ObrKod11_15(QList<qlonglong> list,qint32 kolobr,QString path_temp,bool prsoob)
{
 qlonglong kolzap;
 QString nfile,nfilek,nfileprt;
 QByteArray buf;
 quint64 nz_kvi,nz_kvno;
 QDate dat_ppsi;
 QStringList prot;
 QDate dn,dk;
 qlonglong nz;
 qint32 kan;

 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolobr > 0) && (kolzap > kolobr)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  if (!ReadFullppsi(list[i],prsoob)) continue; ///чтение данных в zapfull для записи fullppsi
  if (zapfull.nz_edxr == 0) continue; //для записи нет единицы хранения

  //проверка наличия второго канала
  if (zapfull.kan == 1) kan=2;
  else kan=1;
  nz=FindRowFull(zapfull.godn,zapfull.sutn,zapfull.sekn,zapfull.nka,kan,false);
  if (nz == 0)
  {
   if (kolzap < list.count()-1) kolzap++;
   continue;
  }

  dat_ppsi=DayToDate(zapfull.sutk,zapfull.godk);

  //чтение и проверка корректности КС единицы хранения
  ReadEdxr("fullppsi",list[i],prsoob);
  if (!InsertObr(komp_name,readedxr.nz_edxr)) continue;
  if (!ReadEdxrInBuf("fullppsi",list[i],buf,prsoob)) continue;

  if (zapfull.kodobr == 11)
  {
   ClearStatFull();

   //ОТЛАДКА
  // prt_kvi.clear();

   nfile=KonvFullDDS(buf,path_temp,zapfull.nka,readedxr.isxf,dat_ppsi,prot);
   if (nfile != "")
   {

    //ОТЛАДКА
//       WriteFileFromList(prt_kvi,path_temp+"/"+nfile+"_prtkvi.txt");

    nz_kvi=SetDDSFromFileKVI(path_temp+"/"+nfile,true,13,prsoob,dn,dk);
    if (nz_kvi > 0)
    {
     zapfull.kodobr=15;
     zapsxema.nz_educh=list[i];
     zapsxema.nz_educho1=nz_kvi;
     zapsxema.nz_educho2=0;
     zapsxema.pr_isx=1;
     InsertSxema("sxema",true);
     if (dn.isValid() && dk.isValid()) UpdateRowStatFull(dn,dk,"dn_kvi","dk_kvi",zapfull.nz,prsoob);
    }
   }
   UpdateRowStatFull(statfull.k_kvi,statfull.crc_kvi,statfull.och_kvi,"k_kvi","crc_kvi","och_kvi",zapfull.nz,prsoob);
   nfileprt=readedxr.path_arx+"/"+"PRTKVI_"+readedxr.arxf.mid(5);
   if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
   DelFile(path_temp+"/"+nfile);
   UpdateRow("fullppsi","kodobr",15,list[i],prsoob);
   zapfull.kodobr=15;
  }
  qApp->processEvents();

  if (zapfull.kodobr == 15)
  {
   nfilek=KonvFullKVNO(buf,path_temp,zapfull.nka,dat_ppsi,prot);
   if (nfilek != "")
   {
    nz_kvno=SetDDSFromFileKVNO(path_temp+"/"+nfilek,true,16,prsoob,dn,dk);
    if (nz_kvno > 0)
    {
     zapsxema.nz_educh=list[i];
     zapsxema.nz_educho1=nz_kvno;
     zapsxema.nz_educho2=0;
     zapsxema.pr_isx=1;
     InsertSxema("sxema",true);
     if (dn.isValid() && dk.isValid()) UpdateRowStatFull(dn,dk,"dn_kvno","dk_kvno",zapfull.nz,prsoob);
    }
   }
   UpdateRowStatFull(statfull.k_kvno,statfull.crc_kvno,statfull.och_kvno,"k_kvno","crc_kvno","och_kvno",zapfull.nz,true);
   nfileprt=readedxr.path_arx+"/"+"PRTKVNO_"+readedxr.arxf.mid(5);
   if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
   DelFile(path_temp+"/"+nfilek);
   UpdateRow("fullppsi","kodobr",12,list[i],prsoob);
  }
  qApp->processEvents();

 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

/*!
 * \brief ObrIzm::KonvFullDDS
 * Конвертирование информации КВИ из FULL файла в формат RINEX 3.02 и запись RINEX файла на диск.
 * В процессе работы использует(изменяет) содержимое структуры ZapStatFull.
 * \param buf[in] содержимое FULL файла
 * \param path[in] путь для записываемого файла
 * \param nka[in] № КА (3 символа)
 * \param nfile[in] имя исходного FULL файла (включая путь или без него)
 * \param dat_ppsi[in] дата сеанса ППСИ
 * \return при успешном завершении возвращает имя записанного файла (без пути), иначе пустую строку
*/
QString ObrIzm::KonvFullDDS(QByteArray &buf,QString path,QString nka,QString nfile,QDate dat_ppsi,QStringList &prot)
{
 bool pr_r=true;
 qint32 nvk;
 bool pr_crc;
 qint32 kod=0;
 QMap<QString,QString> map_kvi;
 QString nfile_kvi;
 qint32 nkad;

 if (buf.isEmpty()) return nfile_kvi;
 prot.clear();

 while(pr_r)
 {
  pr_r=ReadFULL(buf,kod,nvk,pr_crc); //Определение смещения в области памяти FULL файла для первого(очередного) кадра
  if (kod == 0)
  {
   if (!pr_r) //пустой сеанс ППСИ
   {
    return nfile_kvi;
   }
   kod=1;
  }

  if (!pr_r) break;

  if ((nvk < 4) || (nvk > 6)) continue; //кадр не КВИ

  KonvDDSfromKadrKVI(buf,ind_full,map_kvi,prot,dat_ppsi); //Формирование ключевого списка измерений КВИ для кадра ППСИ (8 кадров КВИ)
 }

 nkad=nka.toInt();

 nfile_kvi=FormFileDDS_KVI(map_kvi,path,nfile,nkad);

 if (nfile_kvi.isEmpty()) prot << "Файл ДДС КВИ не сформирован";

 return nfile_kvi;
}

QString ObrIzm::KonvFullKVNO(QByteArray &buf,QString path,QString nka,QDate dat_ppsi,QStringList &prot)
{
 bool pr_r=true;
 qint32 nvk;
 bool pr_crc;
 qint32 kod=0;
 QMap<QDateTime,QString> map_kvno;
 QString nfile_kvno;
 qint32 nkad;

 if (buf.isEmpty()) return nfile_kvno;
 prot.clear();

 while(pr_r)
 {
  pr_r=ReadFULL(buf,kod,nvk,pr_crc);
  if (kod == 0)
  {
   if (!pr_r) return nfile_kvno;
   kod=1;
  }

  if (!pr_r) break;

  if (nvk != 2) continue;

  KonvDDSfromKadrKVNO(buf,ind_full,map_kvno,prot,dat_ppsi);

 }

 nkad=nka.toInt();

 nfile_kvno=FormFileDDS_KVNO(map_kvno,path,nkad);

 return nfile_kvno;
}

quint64 ObrIzm::SetDDSFromFileKVI(QString nfile,bool pr,qint32 kodobr,bool prsoob,QDate &dn,QDate &dk)
{
 QStringList list,list1;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDateTime dt;
 quint64 nz,nz1;

 ClearDDS();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return 0;

 lf=FileInfo(nfile);
 zapdds.naz=lf[3];
 zapdds.naz=zapdds.naz.toUpper();
 zapdds.naz=zapdds.naz.mid(0,7)+"0";
 zapdds.nka=zapdds.naz.mid(0,3);

 zapdds.tip="KVI";

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 zapdds.sutn=dt.date().dayOfYear();
 zapdds.godn=dt.date().year();
 zapdds.sekn=TimeToSec(dt.time());
 zapdds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");
 dn=dt.date();

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 zapdds.sutk=dt.date().dayOfYear();
 zapdds.godk=dt.date().year();
 zapdds.sekk=TimeToSec(dt.time());
 zapdds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");
 dk=dt.date();

 zapdds.strk=0;
 if (zapdds.strk == 0)
 {
  zapdds.kodobr=kodobr;
  if (KolSutInRnx(list) > 1) zapdds.kodobr=14;
 }

 if (zapdds.kodobr != 14)
   if (!ProvRnxInterval(list,list1)) {zapdds.strk=2;zapdds.kodobr=1;}

 if (!pr) return 0;

 nz=InsertDDS(true);
 if (nz == 0) return 0;
 nz1=ZapEdxr("KVI",nz,nfile,0,true);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("dds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 return nz;
}

quint64 ObrIzm::SetDDSFromFileKVNO(QString nfile,bool pr,qint32 kodobr,bool prsoob,QDate &dn,QDate &dk)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDateTime dt;
 quint64 nz,nz1;

 ClearDDS();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return 0;

 lf=FileInfo(nfile);
 zapdds.naz=lf[3];
 zapdds.naz=zapdds.naz.toUpper();
 zapdds.naz=zapdds.naz.mid(0,6)+"0";
 zapdds.nka=zapdds.naz.mid(0,3);

 zapdds.tip="KVNO";

 str=list[0];
 dt=SetDTFromStr(str);
 zapdds.sutn=dt.date().dayOfYear();
 zapdds.godn=dt.date().year();
 zapdds.sekn=TimeToSec(dt.time());
 zapdds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");
 dn=dt.date();

 str=list[kolstr-1];
 dt=SetDTFromStr(str);
 zapdds.sutk=dt.date().dayOfYear();
 zapdds.godk=dt.date().year();
 zapdds.sekk=TimeToSec(dt.time());
 zapdds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");
 dk=dt.date();

 zapdds.strk=0;
 zapdds.kodobr=kodobr;
 if (KolSutInK(list) > 1) zapdds.kodobr=17;

 if (!pr) return 0;

 nz=InsertDDS(true);
 if (nz == 0) return 0;
 nz1=ZapEdxr("KVNO",nz,nfile,0,true);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("dds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 return nz;
}

bool ObrIzm::ObrKod12(qint32 kolobr,QString path_temp,bool prsoob,quint64 nz_full,QSqlQuery query)
{
 QList<qlonglong> list;
 qlonglong kolzap,kolzap1;
 QString nfile,nfile1,infile,nfileprt;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;
 QByteArray buf,buf1,buf0;
 quint64 nz1,nz_alti;
 QStringList sp_f,prot;
 qint32 kolsut;
 QDateTime dtn;
 QDateTime dtk;
 QDate df;
 QDate dn,dk;

 list.clear();

 if (nz_full == 0) list=GetKodobrKan("fullppsi",12,1,prsoob,query);
 else list << nz_full;
 kolzap1=list.count();
 if (kolzap1 == 0) return true;

 if ((kolzap1 > kolobr) && (kolobr > 0)) kolzap=kolobr;
 else kolzap=kolzap1;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  if (!ReadFullppsi(list[i],prsoob,query)) {if (kolzap < kolzap1) kolzap++;continue;} //запись в fullppsi не найдена
  if (zapfull.nz_edxr == 0) {if (kolzap < kolzap1) kolzap++;continue;} //для записи нет единицы хранения
  if (zapfull.kodobr != 12) {if (kolzap < kolzap1) kolzap++;continue;} //не тот код обработки
  if (nz_full == 0) nz1=FindRowFull(zapfull.godn,zapfull.sutn,zapfull.sekn,zapfull.nka,2,12,query); //поиск записи для 2 канала
  else nz1=nz_full;
  if (nz1 == 0) {if (kolzap < kolzap1) kolzap++;continue;}
  if (!ReadFullppsi(nz1,prsoob,query)) {if (kolzap < kolzap1) kolzap++;continue;}
  if (zapfull.nz_edxr == 0) {if (kolzap < kolzap1) kolzap++;continue;}
  if (zapfull.kodobr != 12) {if (kolzap < kolzap1) kolzap++;continue;}
  df=DayToDate(zapfull.sutn,zapfull.godn);

  ReadEdxr("fullppsi",list[i],prsoob,query);
  if (!InsertObr(komp_name,readedxr.nz_edxr,query)) {if (kolzap < kolzap1) kolzap++;continue;}
  nfile=readedxr.path_arx+"/"+readedxr.arxf;
  infile=readedxr.path_arx+"/"+readedxr.isxf;
  if (readedxr.prarx == 1) readedxr.prarx=2;
  if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) {if (kolzap < kolzap1) kolzap++;continue;}
  if (readedxr.aks != aks) {if (kolzap < kolzap1) kolzap++;continue;}
  nfileprt=readedxr.path_arx+"/"+"PRTVRV_"+readedxr.arxf.mid(5);

  ReadEdxr("fullppsi",nz1,prsoob,query);
  if (nz_full == 0) if (!InsertObr(komp_name,readedxr.nz_edxr,query)) {if (kolzap < kolzap1) kolzap++;continue;}
  nfile1=readedxr.path_arx+"/"+readedxr.arxf;
  if (readedxr.prarx == 1) readedxr.prarx=2;
  if (!ArxFile(nfile1,readedxr.prarx,idlf,iks,adlf,aks,buf1)) {if (kolzap < kolzap1) kolzap++;continue;}
  if (readedxr.aks != aks) continue;

  sp_f=CreateAllAltiVRV(buf,buf1,path_temp,zapfull.nka,0,infile,buf0,false,df,prot,0);
  qApp->processEvents();

  UpdateRowStatFullVRV(statfull.k1_vrv1,statfull.k1_vrv2,statfull.k2_vrv1,statfull.k2_vrv2,statfull.crc1_vrv1,statfull.crc1_vrv2,
                       statfull.crc2_vrv1,statfull.crc2_vrv2,statfull.och1_vrv1,statfull.och1_vrv2,statfull.och2_vrv1,
                       statfull.och2_vrv2,statfull.k_vrv,list[i],prsoob,query);
  if (nz_full == 0)
   UpdateRowStatFullVRV(statfull.k1_vrv1,statfull.k1_vrv2,statfull.k2_vrv1,statfull.k2_vrv2,statfull.crc1_vrv1,statfull.crc1_vrv2,
                       statfull.crc2_vrv1,statfull.crc2_vrv2,statfull.och1_vrv1,statfull.och1_vrv2,statfull.och2_vrv1,
                       statfull.och2_vrv2,statfull.k_vrv,nz1,prsoob,query);

  if (sp_f.count() == 0)
  {
   UpdateRow("fullppsi","kodobr",5,list[i],prsoob,query);
   if (nz_full == 0) UpdateRow("fullppsi","kodobr",5,nz1,prsoob,query);
   UpdateRow("fullppsi","strk",4,list[i],prsoob,query);
   if (nz_full == 0) UpdateRow("fullppsi","strk",4,nz1,prsoob,query);
   continue;
  }

  kolsut=KolSutInAlti(buf0,dtn,dtk);
  dn=dtn.date(); dk=dtk.date();
  if (dn.isValid() && dk.isValid())
  {
   UpdateRowStatFull(dn,dk,"dn_vrv","dk_vrv",list[i],prsoob,query);
   if (nz_full == 0) UpdateRowStatFull(dn,dk,"dn_vrv","dk_vrv",nz1,prsoob,query);
  }
  nz_alti=SetDDSFromFileAlti(sp_f[0],true,dtn,dtk,kolsut,"ALTI",18,prsoob,query);
  if (nz_alti > 0)
  {
   zapsxema.nz_educh=list[i];
   zapsxema.nz_educho1=nz_alti;
   zapsxema.nz_educho2=0;
   zapsxema.pr_isx=1;
   InsertSxema("sxema",true,query);
   if (nz_full == 0)
   {
    zapsxema.nz_educh=nz1;
    zapsxema.nz_educho1=nz_alti;
    zapsxema.nz_educho2=0;
    zapsxema.pr_isx=1;
    InsertSxema("sxema",true,query);
   }
   if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
  }

  DelFile(sp_f[0]);

  sp_f=CreateAllCallVRV(buf,buf1,path_temp,zapfull.nka,1033,infile,buf0,0,df);
  qApp->processEvents();
  for (int j=0; j<sp_f.count(); j++)
  {
   buf0.clear();
   buf0=ReadFile(sp_f[j]);
   kolsut=KolSutInAlti(buf0,dtn,dtk);
   kolsut=0;
   nz_alti=SetDDSFromFileAlti(sp_f[j],true,dtn,dtk,kolsut,"CAL1",0,prsoob,query);
   if (nz_alti > 0)
   {
    zapsxema.nz_educh=list[i];
    zapsxema.nz_educho1=nz_alti;
    zapsxema.nz_educho2=0;
    zapsxema.pr_isx=1;
    InsertSxema("sxema",true,query);
    if (nz_full == 0)
    {
     zapsxema.nz_educh=nz1;
     zapsxema.nz_educho1=nz_alti;
     zapsxema.nz_educho2=0;
     zapsxema.pr_isx=1;
     InsertSxema("sxema",true,query);
    }
   }
   DelFile(sp_f[j]);
  }

  sp_f=CreateAllCallVRV(buf,buf1,path_temp,zapfull.nka,1034,infile,buf0,0,df);
  qApp->processEvents();
  for (int j=0; j<sp_f.count(); j++)
  {
   buf0.clear();
   buf0=ReadFile(sp_f[j]);
   kolsut=KolSutInAlti(buf0,dtn,dtk);
   kolsut=0;
   nz_alti=SetDDSFromFileAlti(sp_f[j],true,dtn,dtk,kolsut,"CAL2",0,prsoob,query);
   if (nz_alti > 0)
   {
    zapsxema.nz_educh=list[i];
    zapsxema.nz_educho1=nz_alti;
    zapsxema.nz_educho2=0;
    zapsxema.pr_isx=1;
    InsertSxema("sxema",true,query);
    if (nz_full == 0)
    {
     zapsxema.nz_educh=nz1;
     zapsxema.nz_educho1=nz_alti;
     zapsxema.nz_educho2=0;
     zapsxema.pr_isx=1;
     InsertSxema("sxema",true,query);
    }
   }
   DelFile(sp_f[j]);
  }

  UpdateRowStatFull(statfull.k_cal,"k_cal",list[i],prsoob,query);
  if (nz_full == 0) UpdateRowStatFull(statfull.k_cal,"k_cal",nz1,prsoob,query);

  UpdateRow("fullppsi","kodobr",5,list[i],prsoob,query);
  if (nz_full == 0) UpdateRow("fullppsi","kodobr",5,nz1,prsoob,query);
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob,query);
 return true;
}

bool ObrIzm::ObrKod12(qint32 kolobr,QString path_temp,bool prsoob,quint64 nz_full)
{
 QSqlQuery query;
 bool ret;

 ret=ObrKod12(kolobr,path_temp,prsoob,nz_full,query);

 return ret;
}

qint32 ObrIzm::KolSutInAlti(QByteArray &buf,QDateTime &dtn,QDateTime &dtk)
{
 qint32 kolsut=0;
 QStringList list;
 QDate datn,datk;
 QString str;

 if (!ReadAltiInList(buf,list)) return kolsut;
 if (list.isEmpty()) return kolsut;

 str=list[0];
 str=str.mid(0,6)+"20"+str.mid(6,11);
 dtn=QDateTime::fromString(str,"dd-MM-yyyy hh:mm:ss");
 str=list[list.count()-1];
 str=str.mid(0,6)+"20"+str.mid(6,11);
 dtk=QDateTime::fromString(str,"dd-MM-yyyy hh:mm:ss");
 datn=dtn.date();
 datk=dtk.date();

 kolsut=datn.daysTo(datk)+1;

 return kolsut;
}

quint64 ObrIzm::SetDDSFromFileAlti(QString nfile, bool pr,QDateTime dtn,QDateTime dtk,qint32 kolsut,QString pref,qint32 kodobr,bool prsoob,QSqlQuery query)
{
 QStringList lf;
 quint64 nz,nz1;

 ClearDDS();

 lf=FileInfo(nfile);
 zapdds.naz=lf[3];
 zapdds.naz=zapdds.naz.toUpper();
 zapdds.nka=lf[4];

 zapdds.tip=pref;

 zapdds.sutn=dtn.date().dayOfYear();
 zapdds.godn=dtn.date().year();
 zapdds.sekn=TimeToSec(dtn.time());
 zapdds.dtn=dtn.toString("dd-MM-yyyy hh:mm:ss");

 zapdds.sutk=dtk.date().dayOfYear();
 zapdds.godk=dtk.date().year();
 zapdds.sekk=TimeToSec(dtk.time());
 zapdds.dtk=dtk.toString("dd-MM-yyyy hh:mm:ss");

 zapdds.strk=0;
 zapdds.kodobr=kodobr;
 if (kolsut > 1) zapdds.kodobr=19;

 if (!pr) return 0;

 nz=InsertDDS(true,query);
 if (nz == 0) return 0;
 nz1=ZapEdxr(pref,nz,nfile,0,true,query);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob,query);
  return 0;
 }
 if (!UpdateRow("dds","nz_edxr",nz1,nz,prsoob,query))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob,query);
  DeleteRow("edxr",nz1,prsoob,query);
  return 0;
 }

 return nz;
}

quint64 ObrIzm::SetDDSFromFileAlti(QString nfile, bool pr,QDateTime dtn,QDateTime dtk,qint32 kolsut,QString pref,qint32 kodobr,bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=SetDDSFromFileAlti(nfile,pr,dtn,dtk,kolsut,pref,kodobr,prsoob,query);

 return nz;
}

bool ObrIzm::ObrKod13(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list,list1,list_obr;
 qlonglong kolzap;
 QString nfile;
 QByteArray buf;
 bool pr_o;
 QString sbuf;
 QStringList rez_file;
 QStringList dob_file;
 QStringList ppsi_file;
 QMap<QString,QString> map,map1;
 quint64 nzn;
 QDate dn,dk;
 qint32 kobr;
// QStringList err;

 list=GetKodobr("dds",13,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  list1=FindRowForObDDS(list[i],prsoob);
  if (list1.isEmpty()) continue;
  if (list1.count() == 1)
  {
   UpdateRow("dds","kodobr",0,list1[i],prsoob);
   continue;
  }

  pr_o=false;
  for (int j=0; j<list1.count(); j++)
  {
   if (!ReadDDS(list1[j],prsoob)) {pr_o=true; break;}
   if (!InsertObr(komp_name,zapdds.nz_edxr)) pr_o=true;
  }

  if (pr_o)
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  pr_o=false;
  map.clear();
  ppsi_file.clear();
  list_obr.clear();
  for (int j=0; j<list1.count(); j++)
  {
   buf.clear();
   sbuf.clear();
   if (!ReadDDS(list1[j],prsoob)) {pr_o=true; list_obr << list1[j]; continue;}

   if (!ReadEdxrInList("dds",zapdds.nz,dob_file,prsoob)) {pr_o=true; list_obr << list1[j]; continue;}

   if (!ObDDS(map,dob_file,ppsi_file,zapdds.nka)) {pr_o=true; list_obr << list1[j]; continue;}
   list_obr << 0;
   qApp->processEvents();
  }

  buf.clear();
  sbuf.clear();

  if (pr_o)
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   for (int j=0; j<list1.count(); j++)
   {
    if (list_obr[j] == 0) continue;
    UpdateErr("dds",2,1,list1[j],false);
   }
   return false;
  }

  if (map.isEmpty()) continue;

  //ОТЛАДКА
//  QStringList kvi;
//  kvi=MapToList(map);
//  WriteFileFromList(kvi,path_temp+"/map.txt");

  map1=DelDublInMap(map);
  dob_file=FormOFileFromMap(map1);

 // WriteFileFromList(dob_file,path_temp+"/dob.txt"); //ОТЛАДКА


  rez_file=FormHdrDDS(ppsi_file,dob_file,zapdds.nka);
  AddList(rez_file,dob_file);
  nfile=NazOFile(zapdds.nka+"G",rez_file,path_temp);
  WriteFileFromList(rez_file,path_temp+"/"+nfile);
  kobr=0;
  //if (!ProvRnxInterval(rez_file,err)) kobr=1;
  nzn=SetDDSFromFileKVI(path_temp+"/"+nfile,true,kobr,prsoob,dn,dk);
  if (nzn > 0)
  {
   DelFile(path_temp+"/"+nfile);
   for (int j=0; j<list1.count(); j++)
   {
    UpdateRow("dds","kodobr",6,list1[j],prsoob);
    ClearSxema();
    zapsxema.nz_educh=list1[j];
    zapsxema.nz_educho1=nzn;
    zapsxema.nz_educho2=0;
    InsertSxema("sxema",true);
   }
  }
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

bool ObrIzm::ObrKod14(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list;
 qlonglong kolzap;
 QStringList rez_file;
 quint64 nzn;
 QStringList r_file;
 QDate dn,dk;

 list=GetKodobr("dds",14,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++) //цикл по количеству отобранных записей
 {
  if (pr_stop) break;
  if (!ReadDDS(list[i],prsoob))
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }
  if (!InsertObr(komp_name,zapdds.nz_edxr)) return false;

  if (!ReadEdxrInList("dds",zapdds.nz,r_file,prsoob)) return false;

  rez_file=RazbDDSOFile(r_file,path_temp);
  if (rez_file.isEmpty()) continue;

  for (int j=0; j<rez_file.count(); j++)
  {
   nzn=SetDDSFromFileKVI(rez_file[j],true,13,prsoob,dn,dk);
   if (nzn > 0)
   {
    DelFile(rez_file[j]);
    ClearSxema();
    zapsxema.nz_educh=list[i];
    zapsxema.nz_educho1=nzn;
    zapsxema.nz_educho2=0;
    InsertSxema("sxema",true);
   }
  }

  UpdateRow("dds","kodobr",6,list[i],prsoob);
  qApp->processEvents();

 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

bool ObrIzm::RemoveDDS(quint64 nz,bool prsoob)
{
 if (!ReadDDS(nz,prsoob)) return false;

 if (zapdds.kodobr != 6) return false;

 if (!RemoveEdxr(zapdds.nz_edxr,prsoob)) return false;

 if (!DeleteRow("dds",nz,prsoob)) return false;

 UpdateRows("sxema","nz_educho2",1,"nz_educh",nz,prsoob,0);
 UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz,prsoob,0);
 UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz,prsoob,1);

 return true;
}

bool ObrIzm::ObrKod16(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list,list1;
 qlonglong kolzap;
 QString nfile;
 quint64 nzn;
 QDate dn,dk;

 list=GetKodobr("dds",16,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  list1=FindRowForObKVNO(list[i],prsoob);
  if (list1.isEmpty()) continue;
  if (list1.count() == 1)
  {
   UpdateRow("dds","kodobr",0,list1[0],prsoob);
   continue;
  }

  for (int j=0; j<list1.count(); j++)
  {
   if (!ReadDDS(list1[j],prsoob))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
   if (!InsertObr(komp_name,zapdds.nz_edxr))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
   qApp->processEvents();
  }

  nfile=ObKFile(list1,path_temp,prsoob);
  if (nfile == "")
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  nzn=SetDDSFromFileKVNO(nfile,true,0,prsoob,dn,dk);
  if (nzn == 0)
 {
  DeleteRow("obr","naz_komp",komp_name,prsoob);
  return false;
 }
  DelFile(nfile);

  for (int j=0; j<list1.count(); j++)
  {
   ClearSxema();
   zapsxema.nz_educh=list1[j];
   zapsxema.nz_educho1=nzn;
   zapsxema.nz_educho2=0;
   InsertSxema("sxema",true);
   UpdateRow("dds","kodobr",6,list1[j],prsoob);
  }
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

QString ObrIzm::ObKFile(QList<qlonglong> sp_nz,QString path,bool prsoob)
{
 QString nfile;
 qint32 kolstr;
 QStringList list,olist;
 QMap<QDateTime,QString> map;
 QDateTime dt;
 quint64 nz;

 nfile.clear();

 if (sp_nz.isEmpty()) return nfile;
 kolstr=sp_nz.count();
 if (kolstr == 1)
 {
  UpdateRow("dds","kodobr",0,sp_nz[0],prsoob);
  return nfile;
 }

 for (int i=0; i<kolstr; i++)
 {
  nz=sp_nz.at(i);
  if (!ReadDDS(nz,prsoob)) return nfile;

  if (!ReadEdxrInList("dds",zapdds.nz,list,prsoob)) continue;

  AddList(olist,list);
 }

 map=KFileInMap(olist);
 olist=FormKFileFromMap(map,dt);
 nfile=NazKFile(zapdds.nka,dt.date(),path);
 nfile=path+"/"+nfile;
 if (!WriteFileFromList(olist,nfile)) nfile="";
 return nfile;
}

QStringList ObrIzm::RazbKFile(quint64 nz,QString path,bool prsoob)
{
 QStringList spis_f;
 QStringList list,new_list;
 qint32 kolsut;
 qlonglong kolstr;
 QString str,str1;
 QDateTime dt;
 QDate dn,dk;
 QString kfile;


 if (!ReadDDS(nz,prsoob)) return spis_f;

 if (!ReadEdxrInList("dds",zapdds.nz,list,prsoob)) return spis_f;

 kolsut=KolSutInK(list);

 if (kolsut == 0) return spis_f;
 if (kolsut < 2)
 {
  UpdateRow("dds","kodobr",0,nz,prsoob);
  return spis_f;
 }

 kolstr=list.count();
 str=list.at(0);
 str=str.mid(0,27);
 dt=SetDTFromStr(str);
 dn=dt.date();

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str1=str.mid(0,27);
  dt=SetDTFromStr(str1);
  dk=dt.date();
  if (dn == dk) {new_list << str; continue;}
  kfile=NazKFile(zapdds.nka,dn,path);
  WriteFileFromList(new_list,path+"/"+kfile);
  spis_f << kfile;
  new_list.clear();
  dn=dk;
  new_list << str;
 }

 if (!new_list.isEmpty())
 {
  kfile=NazKFile(zapdds.nka,dn,path);
  WriteFileFromList(new_list,path+"/"+kfile);
  spis_f << kfile;
 }

 return spis_f;
}

bool ObrIzm::ObrKod17(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list;
 qlonglong kolzap;
 quint64 nzn;
 QStringList sp_file;
 QDate dn,dk;

 list=GetKodobr("dds",17,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;
 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  if (!ReadDDS(list[i],prsoob)) continue;
  if (zapdds.nz_edxr == 0) continue;
  if (!InsertObr(komp_name,zapdds.nz_edxr)) continue;

  sp_file=RazbKFile(list[i],path_temp,prsoob);
  if (sp_file.isEmpty()) continue;
  if (sp_file.count() == 1)
  {
   UpdateRow("dds","kodobr",0,list[0],prsoob);
   continue;
  }

  for (int j=0; j<sp_file.count(); j++)
  {
   nzn=SetDDSFromFileKVNO(path_temp+"/"+sp_file[j],true,16,prsoob,dn,dk);
   if (nzn == 0)
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
   DelFile(path_temp+"/"+sp_file[j]);
   ClearSxema();
   zapsxema.nz_educh=list[i];
   zapsxema.nz_educho1=nzn;
   zapsxema.nz_educho2=0;
   InsertSxema("sxema",true);
   qApp->processEvents();
  }

  UpdateRow("dds","kodobr",6,list[i],prsoob);
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

QStringList ObrIzm::RazbAlti(quint64 nz,QString path,bool prsoob)
{
 QStringList sp_f;
 QString nfile;
 QByteArray buf;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;

 if (!ReadDDS(nz,prsoob)) return sp_f;
 if (!ReadEdxr("dds",zapdds.nz,prsoob)) return sp_f;
 nfile=readedxr.path_arx+"/"+readedxr.arxf;
 if (readedxr.prarx == 1) readedxr.prarx=2;
 if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) return sp_f;
 if (readedxr.aks != aks) return sp_f;

 sp_f=RazbAlti(buf,path,prsoob,nz,0);

 return sp_f;
}

QStringList ObrIzm::RazbAlti(QByteArray &buf,QString path,bool prsoob,quint64 nz,qint32 nkan)
{
 QStringList sp_f;
 qint32 kolsut;
 QString str;
 QDateTime dtn,dtk;
 QDate dn;
 QTime tm;
 qint64 ind=0;
 qint64 indn=0;
 qint32 dls;
 qint64 kolpak;
 qint32 chpak;
 QString isx_f="",vix_f="";
 qint32 dlpak;

 if (!InfFromAlti(buf,dtn,dtk,kolpak,dlpak,dls,isx_f,vix_f,kolsut)) return sp_f;

 if (kolsut == 0) return sp_f;
 if (nz > 0)
 {
  if (kolsut < 2)
  {
   UpdateRow("dds","kodobr",18,nz,prsoob);
   return sp_f;
  }
 }

 ind=ind+dls;
 indn=ind;

 chpak=0;
 ReadZagVRV(buf,ind-6);
 dn=zagvrv.datn;
 tm=zagvrv.timen;
 for (int i=0; i<kolpak; i++)
 {
  ReadZagVRV(buf,ind-6);
  if (dn == zagvrv.datn)
  {
   ind=ind+(zagvrv.dlpak+7);
   chpak++;
   continue;
  }

  str=WriteFileAlti(buf,zapdds.nka,dn,tm,chpak,isx_f,indn,dlpak,path,nkan);
  if (str == "")
  {
   sp_f.clear();
   return sp_f;
  }
  sp_f << str;

  chpak=1;
  dn=zagvrv.datn;
  tm=zagvrv.timen;
  indn=ind;
  ind=ind+(zagvrv.dlpak+7);
 }

 if (chpak > 0)
 {
  str=WriteFileAlti(buf,zapdds.nka,dn,tm,chpak,isx_f,indn,dlpak,path,nkan);
  if (str == "")
  {
   sp_f.clear();
   return sp_f;
  }
  sp_f << str;
 }

 return sp_f;
}

bool ObrIzm::ObrKod19(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list;
 qlonglong kolzap;
 quint64 nzn;
 QStringList sp_file;

 list=GetKodobr("dds",19,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  if (!ReadDDS(list[i],prsoob)) continue;
  if (!InsertObr(komp_name,zapdds.nz_edxr)) continue;

  sp_file=RazbAlti(list[i],path_temp,prsoob);
  if (sp_file.isEmpty()) continue;
  if (sp_file.count() == 1)
  {
   UpdateRow("dds","kodobr",0,list[i],prsoob);
   continue;
  }

  for (int j=0; j<sp_file.count(); j++)
  {
   nzn=SetDDSFromFileAlti(sp_file[j],true,"ALTI",18,prsoob);
   if (nzn == 0)
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
   DelFile(sp_file[j]);
   ClearSxema();
   zapsxema.nz_educh=list[i];
   zapsxema.nz_educho1=nzn;
   zapsxema.nz_educho2=0;
   InsertSxema("sxema",true);
  }

  UpdateRow("dds","kodobr",6,list[i],prsoob);
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);
 return true;
}

bool ObrIzm::InfFromAlti(QByteArray &buf, QDateTime &dtn, QDateTime &dtk,qint64 &kolpak,qint32 &dlpak,qint32 &dlsh,QString &i_file,QString &v_file,qint32 &kolsut)
{
 QString str;
 QDate dn,dk,dt;
 QTime tn,tk,tt;
 qint64 ind=0;
 qint32 dls,id,rez;
 QByteArray shap;
 QDataStream s(&shap,QIODevice::ReadOnly);
 qint64 dlf;
 qint32 chpak;
 QString textsh;
 QStringList listsh;
 int nz_list;
 QDateTime dtt;

 i_file=""; v_file=""; kolsut=0; dlpak=0;

 dlf=buf.size();
 if (dlf < 32) return false;

 shap=buf.mid(8,20);
 s >> id >> rez >> kolpak >> dls;
 if (dlf <= dls) return false;
 if (kolpak == 0) return false;

 textsh=buf.mid(32,dls-32);
 listsh=textsh.split("\n",QString::SkipEmptyParts);
 nz_list=FindStrInList(listsh,"Source");
 if (nz_list != -1)
 {
  str=listsh[nz_list];
  i_file=str.mid(13);
 }
 nz_list=FindStrInList(listsh,"Output");
 if (nz_list != -1)
 {
  str=listsh[nz_list];
  v_file=str.mid(13);
 }

 ind=ind+dls;
 chpak=0;
 ReadZagVRV(buf,ind-6);
 dlpak=zagvrv.dlpak;
 if (dlf != (kolpak*(dlpak+7))+dls) return false;
 dn=zagvrv.datn;
 tn=zagvrv.timen;
 dk=zagvrv.datn;
 tk=zagvrv.timen;
 dlsh=dls;
 dtn.setDate(dn);
 dtn.setTime(tn);
 dtk.setDate(dn);
 dtk.setTime(tn);
 for (int i=0; i<kolpak; i++)
 {
  ReadZagVRV(buf,ind-6);
  dt=zagvrv.datn;
  tt=zagvrv.timen;
  dtt.setDate(dt);
  dtt.setTime(tt);
  if (dtt < dtn) dtn=dtt;
  if (dtt > dtk) dtk=dtt;
  ind=ind+(zagvrv.dlpak+7);
 }

 dn=dtn.date();
 dk=dtk.date();

 kolsut=dn.daysTo(dk)+1;
 return true;
}

QString ObrIzm::WriteFileAlti(QByteArray &buf,QString nka,QDate dn,QTime tn,qint64 kolpak,QString i_file,qint64 ind,qint32 dlpak,QString path,qint32 nkan)
{
 QString str;
 qint32 nka1;
 QByteArray bufz,buf1;
 int dlbuf,dlbufz;
 int dlzap;

 nka1=nka.toInt();

 str=NameVRVFile("ALTI",dn,tn,0,nkan,nka1);
 FormZagTelVRV(bufz,1031,i_file,str,kolpak);
 dlbufz=bufz.size();
 dlbuf=buf.size();
 dlzap=kolpak*(dlpak+7);

 if (ind+dlzap > dlbuf) return "";

 dlbufz=dlbufz+dlzap;
 buf1.reserve(dlbufz);
 buf1.append(bufz);
 buf1.append(buf.mid(ind,dlzap));

 str=path+"/"+str;
 if (WriteFile(str,buf1,true) == -1) return "";

 return str;
}

quint64 ObrIzm::SetDDSFromFileAlti(QString nfile, bool pr,QString pref,qint32 kodobr,bool prsoob)
{
 QStringList lf;
 quint64 nz,nz1;
 QByteArray buf;
 QDateTime dtn,dtk;
 qint64 kolpak;
 qint32 dlpak;
 qint32 dlsh;
 QString i_file,v_file;
 qint32 kolsut;

 ClearDDS();

 buf=ReadFile(nfile);
 if (!InfFromAlti(buf,dtn,dtk,kolpak,dlpak,dlsh,i_file,v_file,kolsut)) return 0;

 lf=FileInfo(nfile);
 zapdds.naz=lf[3];
 zapdds.naz=zapdds.naz.toUpper();
 zapdds.nka=lf[4];

 zapdds.tip=pref;

 zapdds.sutn=dtn.date().dayOfYear();
 zapdds.godn=dtn.date().year();
 zapdds.sekn=TimeToSec(dtn.time());
 zapdds.dtn=dtn.toString("dd-MM-yyyy hh:mm:ss");

 zapdds.sutk=dtk.date().dayOfYear();
 zapdds.godk=dtk.date().year();
 zapdds.sekk=TimeToSec(dtk.time());
 zapdds.dtk=dtk.toString("dd-MM-yyyy hh:mm:ss");

 zapdds.strk=0;
 zapdds.kodobr=kodobr;
 if (kolsut > 1) zapdds.kodobr=19;

 if (!pr) return 0;

 nz=InsertDDS(true);
 if (nz == 0) return 0;
 nz1=ZapEdxr(pref,nz,nfile,0,true);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("dds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 return nz;
}

/*!
 * \brief ObrIzm::ObAltiInMap
 * Формирование объединённого ключевого списка и области данных из файлов ALTI
 * в качестве ключа используется дата, время и интервал(мкс)
 * данными являются смещения в области данных obuf
 * в результате работы формируется отсортированный по дате и времени список смещений на кадры в области памяти(неотсортированные)
 * \param nz[in] список ключей файлов ALTI из таблицы dds для объединения
 * \param map[out] сформированный ключевой список
 * \param okolpak[out] количество пакетов в объединённом ключевом списке
 * \param i_file[out] - целесообразно ????????????????????
 * \param obuf[out] область данных объединённых файлов ALTI
 * \param dlpak[out] - целесообразно ????????????????????
 * \return при неудачном формировании ключевого списка возвращает false, инче true
*/
bool ObrIzm::ObAltiInMap(QList<qlonglong> nz,QMap<QString,QString> &map,qint64 &okolpak,QString &i_file,QByteArray &obuf,qint32 &dlpak,bool prsoob)
{
 QByteArray buf;
 QDateTime dtn,dtk;
 qint64 kolpak;
 qint32 dlsh;
 QString v_file;
 qint32 kolsut;
 qint32 dlbuf;

 map.clear();
 obuf.clear();
 okolpak=0;
 dlbuf=0;

 for (int i=0; i<nz.count(); i++)
 {
  if (!ReadDDS(nz[i],prsoob)) return false;
  if (!ReadEdxrInBuf("dds",zapdds.nz,buf,prsoob)) return false; //=== замена
  if (!InfFromAlti(buf,dtn,dtk,kolpak,dlpak,dlsh,i_file,v_file,kolsut)) return false;
  okolpak=okolpak+kolpak;
  dlbuf=AddArray(obuf,buf,dlsh);
  buf.clear();
 }

 AltiInMap(obuf,map,dlpak,okolpak);

 return true;
}

bool ObrIzm::ObrKod18(qint32 kolobr,QString path_temp,bool prsoob)
{
 QList<qlonglong> list,list1;
 qlonglong kolzap;
 quint64 nzn;
 QString naz_ifile;
 QByteArray obuf,buf_vrv;
 qint32 dlpak;
 QDateTime dtn,dtk;
 QMap<QString, QString> map;
 qint64 kolpak;
 qint32 kolsut;
 QString nfile;

 list=GetKodobr("dds",18,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  if (!ReadDDS(list[i],prsoob))
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }
  if (zapdds.kodobr != 18) continue;
  list1=FindRowForObAlti(list[i],prsoob);
  if (list1.isEmpty()) continue;
  if (list1.count() == 1)
  {
   if (!InsertObr(komp_name,zapdds.nz_edxr)) continue;
   UpdateRow("dds","kodobr",0,list1[0],prsoob);
   continue;
  }

  for (int j=0; j<list1.count(); j++)
  {
   if (!ReadDDS(list1[j],prsoob))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
   if (!InsertObr(komp_name,zapdds.nz_edxr))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false;
   }
  }

  map.clear(); obuf.clear();
  if (!ObAltiInMapF(list1,map,kolpak,naz_ifile,obuf,dlpak,prsoob))
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  AltiMapInArray(map,obuf,buf_vrv,dlpak,kolpak);
  map.clear(); obuf.clear();

  if (!InfFromAlti(buf_vrv,dtn,dtk,kolpak,kolsut))
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  nfile=WriteFileAlti(buf_vrv,zapdds.nka,dtn.date(),dtn.time(),kolpak,naz_ifile,path_temp);
  buf_vrv.clear();
  if (nfile == "")
  {
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  nzn=SetDDSFromFileAlti(nfile,true,dtn,dtk,kolsut,"ALTI",0,prsoob);
  if (nzn == 0)
  {
   DelFile(nfile);
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   return false;
  }

  DelFile(nfile);

  for (int j=0; j<list1.count(); j++)
  {
   ClearSxema();
   zapsxema.nz_educh=list1[j];
   zapsxema.nz_educho1=nzn;
   zapsxema.nz_educho2=0;
   InsertSxema("sxema",true);
   UpdateRow("dds","kodobr",6,list1[j],prsoob);
  }
  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

bool ObrIzm::AltiMapInArray(QMap<QString,QString> &map,QByteArray &buf_isx,QByteArray &buf_alti,qint32 dlpak, qint64 &kolpak)
{
 QMap<QString,QString>::const_iterator i;
 quint64 ind;
 QString str;
 QStringList list_ind;

 if (map.isEmpty()) return false;

 buf_alti.clear();
 kolpak=0;

 for (i=map.constBegin(); i!=map.constEnd(); ++i)
 {
  list_ind << i.value();
 }

 if (list_ind.isEmpty()) return false;

 buf_alti.reserve(list_ind.count()*(dlpak+7));

 for (int j=0; j<list_ind.count(); j++)
 {
  str=list_ind[j];
  ind=str.toLongLong();
  buf_alti.append(buf_isx.mid(ind,dlpak+7));
  kolpak++;
 }

 return true;
}

/*!
 * \brief ObrIzm::InfFromAlti
 * Получение информации о содержимом ALTI,CALL файла
 * \param buf[in] область памяти с содержимым файла без заголовка
 * \param dtn[out] первая эпоха в файле
 * \param dtk[out] последняя эпоха в файле
 * \param kolpak[in] количество пакетов в файле
 * \param kolsut[out] количество суток между эпохами
 * \return
*/
bool ObrIzm::InfFromAlti(QByteArray &buf, QDateTime &dtn, QDateTime &dtk, qint64 kolpak,qint32 &kolsut)
{
 QDate dn,dk,dt;
 QTime tn,tk,tt;
 qint64 ind=0;
 QDateTime dtt;

 qDebug() << "InfFromAlti";

 if (buf.isEmpty()) return false;

 ReadZagVRV(buf,ind-6);
 dn=zagvrv.datn;
 tn=zagvrv.timen;
 dk=zagvrv.datn;
 tk=zagvrv.timen;
 dtn.setDate(dn);
 dtn.setTime(tn);
 dtk.setDate(dn);
 dtk.setTime(tn);
 if (buf.size() != kolpak*(zagvrv.dlpak+7)) return false;
 for (int i=0; i<kolpak; i++)
 {
  ReadZagVRV(buf,ind-6);
  dt=zagvrv.datn;
  tt=zagvrv.timen;
  dtt.setDate(dt);
  dtt.setTime(tt);
  if (dtt < dtn) dtn=dtt;
  if (dtt > dtk) dtk=dtt;
  ind=ind+(zagvrv.dlpak+7);
 }

 dn=dtn.date();
 dk=dtk.date();

 kolsut=dn.daysTo(dk)+1;

 return true;
}

/*!
 * \brief ObrIzm::WriteFileAlti
 * Запись файла ALTI на диск. Формирует заголовок и имя файла и записывает его на диск.
 * \param buf[in] содержимое ALTI файла без заголовка
 * \param nka[in] номер КА для имени файла
 * \param dn[in] дата первого измерения для имени файла
 * \param tn[in] время первого измерения для имени файла
 * \param kolpak[in] количество пакетов ВРВ для заголовка файла
 * \param i_file[in] имя исходного FULL файла для заголовка файла
 * \param path[in] путь к записываемому файлу
 * \return
*/
QString ObrIzm::WriteFileAlti(QByteArray &buf,QString nka,QDate dn,QTime tn,qint64 kolpak,QString i_file,QString path)
{
 QString str;
 qint32 nka1;
 QByteArray bufz;

 qDebug() << "WriteFileAlti";

 nka1=nka.toInt();

 str=NameVRVFile("ALTI",dn,tn,0,0,nka1);
 FormZagTelVRV(bufz,1031,i_file,str,kolpak);
 str=path+"/"+str;
 if (WriteFile(str,bufz,true) == -1) return "";
 if (!AppendFile(str,buf)) return "";

 return str;
}

bool ObrIzm::SetDzs(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString stime;

 ClearDs();

 lf=FileInfo(nfile);
 zapds.naz=lf[3];
 zapds.naz=zapds.naz.toUpper();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 str=list[0];
 str=str.simplified();

 zapds.npn=str.section(" ",1,1);
 zapds.godn=str.section(" ",4,4).toInt()+2000;
 zapds.sutn=str.section(" ",5,5).toInt();
 zapds.sekn=str.section(" ",6,6).toInt();
 zapds.nka=str.section(" ",0,0);
 zapds.nkis=str.section(" ",2,2);
 zapds.sv=str.section(" ",3,3).toInt();

 str=list[kolstr-1];
 str=str.simplified();

 zapds.godk=str.section(" ",4,4).toInt()+2000;
 zapds.sutk=str.section(" ",5,5).toInt();
 zapds.sekk=str.section(" ",6,6).toInt();

 dat=DayToDate(zapds.sutn,zapds.godn);
 time=SecToTime(zapds.sekn,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 dat=DayToDate(zapds.sutk,zapds.godk);
 time=SecToTime(zapds.sekk,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapds.nazis="DZS";

 zapds.nz_edxr=0;
 Stat322(list);

 return true;
}

bool ObrIzm::SetSpo(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString stime;
 bool prok;

 ClearDs();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;


 do
 {
  prok=true;
  for (int i=0; i<kolstr; i++)
  {
   str=list[i];
   str=str.simplified();
   if (((str.section(" ",0,0) != "126") && (str.section(" ",0,0) != "127")) || (str.section(" ",2,2) == "0"))
   {
    list.removeAt(i);
    kolstr=list.count();
    prok=false;
    break;
   }
  }
 } while (!prok);

 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zapds.naz=lf[3];
 zapds.naz=zapds.naz.toUpper();
 zapds.sv=3;

 str=list[0];
 str=str.simplified();

 zapds.npn=zapds.naz.left(4);

 dat=dat.fromString(str.section(" ",3,3),"yyyy-MM-dd");
 time=time.fromString(str.section(" ",4,4).left(8),"hh:mm:ss");

 zapds.sutn=dat.dayOfYear();
 zapds.godn=dat.year();

 zapds.sekn=TimeToSec(time);

 zapds.sekn=TimeToSec(time);
 zapds.nka=str.section(" ",0,0);

 str=list[kolstr-1];
 str=str.simplified();

 dat=dat.fromString(str.section(" ",3,3),"yyyy-MM-dd");
 time=time.fromString(str.section(" ",4,4).left(8),"hh:mm:ss");

 zapds.sutk=dat.dayOfYear();
 zapds.godk=dat.year();

 zapds.sekk=TimeToSec(time);

 dat=DayToDate(zapds.sutn,zapds.godn);
 time=SecToTime(zapds.sekn,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 dat=DayToDate(zapds.sutk,zapds.godk);
 time=SecToTime(zapds.sekk,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapds.nazis="SPO";

 zapds.nz_edxr=0;

 return true;
}

bool ObrIzm::SetRnxO(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QStringList lf;

 ClearRnx();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 SetRnxO(list,lf[3]);

 return true;
}

bool ObrIzm::SetRnxO(QStringList list,QString naz_file)
{
 qlonglong kolstr;
 QString str;
 QDateTime dt;

 ClearRnx();

 kolstr=list.count();
 if (kolstr == 0) return false;

 zaprnx.naz=naz_file;
 zaprnx.naz=zaprnx.naz.toUpper();

 zaprnx.npn=zaprnx.naz.left(4);

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 zaprnx.sutn=dt.date().dayOfYear();
 zaprnx.godn=dt.date().year();
 zaprnx.sekn=TimeToSec(dt.time());
 zaprnx.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 zaprnx.sutk=dt.date().dayOfYear();
 zaprnx.godk=dt.date().year();
 zaprnx.sekk=TimeToSec(dt.time());
 zaprnx.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zaprnx.kodobr=0;
 zaprnx.strk=0;
 zaprnx.tip="O";
 zaprnx.ver=RnxVersion(list);
 zaprnx.nz_edxr=0;
 zaprnx.interv=RnxInterval(list);

 if (zaprnx.dtn.isEmpty()) {zaprnx.kodobr=1; zaprnx.strk=5;}

 return true;
}

quint64 ObrIzm::SetRnxFileO(QString naz_file,bool prob,bool prsoob)
{
 qint32 kolsut;
 QString ver;
 quint64 nz,nz1;
 QDate dn,dk;

 ver=RnxVersion(naz_file);

// if(ver != "3.02") return 0;
 if(ver.left(1) != "3") return 0;
 SetRnxO(naz_file);
 kolsut=KolSutInRnx(naz_file,dn,dk);
 if (!prob) zaprnx.kodobr=4;
 if (kolsut > 1) zaprnx.kodobr=3;
 if (!ProvRnxFile(naz_file,false,false)) {zaprnx.strk=2;zaprnx.kodobr=1;}

 nz=InsertRnx("rnxv",false);
 if (nz == 0)
 {
  DelFile(naz_file);
  return 0;
 }
 nz1=ZapEdxr("VO",nz,naz_file,0,true);
 if (nz1 == 0)
 {
  DelFile(naz_file);
  DeleteRow("rnxv",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("rnxv","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(naz_file);
  DeleteRow("rnxv",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 DelFile(naz_file);
 return nz;
}

bool ObrIzm::SetRnxNG(QStringList list,QString naz_file,QString tip,qint32 &kolsut)
{
 qlonglong kolstr;
 QDateTime dtn,dtk;
 QStringList list1;
 QString ver;

 ClearRnx();
 kolsut=0;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ver=RnxVersion(list);

 zaprnx.naz=naz_file;
 zaprnx.naz=zaprnx.naz.toUpper();

 zaprnx.npn=zaprnx.naz.left(4);

 if (ver.left(1) == "2")
 {
  list1=SetListNFile2(list);
 }
 if (ver.left(1) == "3")
 {
  list1=SetListNFile(list);
 }
 kolstr=list1.count();
 if (kolstr == 0) return false;

 if (ver.left(1) == "2")
 {
  if (! KolSutNFile2(list1,dtn,dtk,kolsut)) return false;
 }
 if (ver.left(1) == "3")
 {
  if (! KolSutNFile(list1,dtn,dtk,kolsut)) return false;
 }

 zaprnx.sutn=dtn.date().dayOfYear();
 zaprnx.godn=dtn.date().year();
 zaprnx.sekn=TimeToSec(dtn.time());
 zaprnx.dtn=dtn.toString("dd-MM-yyyy hh:mm:ss");

 zaprnx.sutk=dtk.date().dayOfYear();
 zaprnx.godk=dtk.date().year();
 zaprnx.sekk=TimeToSec(dtk.time());
 zaprnx.dtk=dtk.toString("dd-MM-yyyy hh:mm:ss");

 zaprnx.kodobr=0;
 zaprnx.strk=0;
 zaprnx.tip=tip;
 zaprnx.ver=ver;
 zaprnx.nz_edxr=0;

 if (zaprnx.dtn.isEmpty()) {zaprnx.kodobr=1; zaprnx.strk=5;}
 if (zaprnx.dtk.isEmpty()) {zaprnx.kodobr=1; zaprnx.strk=5;}
 if (dtn.date() > QDate::currentDate()) {zaprnx.kodobr=1; zaprnx.strk=5;}
 if (dtk.date() > QDate::currentDate()) {zaprnx.kodobr=1; zaprnx.strk=5;}

 return true;
}

bool ObrIzm::SetRnxNG(QString nfile,QString tip,qint32 &kolsut)
{
 QStringList list;
 qlonglong kolstr;
 QStringList lf;

 ClearRnx();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 SetRnxNG(list,lf[3],tip,kolsut);

 return true;
}

quint64 ObrIzm::SetRnxFileNG(QString naz_file,QString tip,bool prob,bool prsoob)
{
 qint32 kolsut;
 QString ver;
 quint64 nz,nz1;

 ver=RnxVersion(naz_file);

 if(ver.left(1) != "3") return 0;
 if (!SetRnxNG(naz_file,tip,kolsut)) return 0;
 if (zaprnx.strk == 0)
 {
  zaprnx.kodobr=9;
  if (prob) {zaprnx.npn="OBFL"; zaprnx.kodobr=0;}
  if (kolsut > 1) zaprnx.kodobr=8;
  if (!ProvRnxFileN(naz_file,false))
  {
   zaprnx.kodobr=1;
   zaprnx.strk=2;
  }
 }

 nz=InsertRnx("rnxv",false);
 if (nz == 0)
 {
  DelFile(naz_file);
  return 0;
 }
 nz1=ZapEdxr("VNG",nz,naz_file,0,true);
 if (nz1 == 0)
 {
  DelFile(naz_file);
  DeleteRow("rnxv",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("rnxv","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(naz_file);
  DeleteRow("rnxv",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 DelFile(naz_file);
 return nz;
}

bool ObrIzm::RemoveRnxv(quint64 nz,bool prsoob)
{
 if (!ReadRnx("rnxv",nz,prsoob)) return false;

 if (zaprnx.kodobr != 20) return false;

 if (!RemoveEdxr(zaprnx.nz_edxr,prsoob)) return false;

 if (!DeleteRow("rnxv",nz,prsoob)) return false;

 UpdateRows("sxema1","nz_educho2",1,"nz_educh",nz,prsoob,0);
 UpdateRows("sxema1","nz_educho2",2,"nz_educho1",nz,prsoob,0);
 UpdateRows("sxema1","nz_educho2",2,"nz_educho1",nz,prsoob,1);

 return true;
}

bool ObrIzm::ObrKod20(bool prsoob)
{
 qlonglong nz;
 QList<qlonglong> list;
 qlonglong kolzap;

 ClearSlu();

 list=GetKodobr("rnxv",20,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 for (int i=0; i < kolzap; i++)
 {
  nz=list.at(i);
  RemoveRnxv(nz,prsoob);
 }

 return true;
}

bool ObrIzm::ZapStatDDS(QStringList list,quint64 nz_dds,bool prsoob)
{
 QStringList stat;
 qlonglong kolstr;
 qint32 kol1;
 qint32 kol2;
 qint32 kol3;
 qint32 kolep;
 QDateTime dt;
 QString str,str1;
 int ind;
 quint64 nz,nz1;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,">");
 if (ind == -1) return false;
 str=list.at(ind);
 dt=DTForHdrDatRnx(str);
 if (!dt.isValid()) return false;

 if (!StatEpochForDDS(list,kol1,kol2,kol3,kolep)) return false;

 ClearStatDDS();

 zapstatdds.dat=dt.date();
 zapstatdds.eppl=8640;
 zapstatdds.eppol=kolep;
 zapstatdds.ep_4mglo=kol1;
 zapstatdds.ep_4rglo=kol2;
 zapstatdds.ep_4bglo=kol3;
 zapstatdds.nz_dds=nz_dds;

 for (int i=0; i<24; i++) zapstatdds.mas_ep[i]=kol_ep_dds[i];

 nz=InsertStatDDS(true);
 if (nz == 0) return false;

 stat=StatDDS(list);
 kolstr=stat.count();

 for (int i=0; i<kolstr; i++)
 {
  ClearStatrDDS();
  str=stat.at(i);
  str=str.simplified();

  zapstatrdds.nz_statdds=nz;
  str1=str.section(",",1,1);
  zapstatrdds.vrn=QTime::fromString(str1);
  str1=str.section(",",3,3);
  zapstatrdds.vrr=QTime::fromString(str1);
  str1=str.section(",",2,2);
  zapstatrdds.kolep=str1.toInt();

  nz1=InsertStatrDDS(true);
  if (nz1 == 0)
  {
   DeleteRow("statdds",nz,prsoob);
   return false;
  }
 }

 return true;
}

bool ObrIzm::ZapStatVRV(quint64 nz_dds, quint64 &nz,bool prsoob)
{
 QByteArray buf;
 QDateTime dtn;
 QDate dn;
 qint32 ch_vne=0;
 qint32 ch_vip=0;
 qint32 ch_nevip=0;
 qint32 ch_noplan=0;
 qint32 ch_zapl=0;
 QTime t;

 QProgressDialog prd;
 prd.setModal(true);
 prd.setCancelButton(0);
 prd.setLabelText("Расчёт статистики");
 prd.setWindowTitle("Статистика ВРВ");
 prd.setMinimum(0);
 prd.setMaximum(86400);

 if (!ReadEdxrInBuf("dds",nz_dds,buf,prsoob)) return false;
 if (!ReadDDS(nz_dds,prsoob)) return false;
 dtn=QDateTime::fromString(zapdds.dtn,"dd-MM-yyyy hh:mm:ss");
 dn=dtn.date();
 t.setHMS(0,0,0);

 if (!ZapEpvrv(buf)) return false;
 if (!ZapEpvrvplan(dn,zapdds.nka)) return false;

 prd.show();

 for (qint32 i=0; i<86400; i++)
 {
  prd.setValue(i);
  qApp->processEvents();
  if (epvrvplan[i] == 2) ch_zapl++;
  if (epvrvplan[i] == 0) {ch_noplan++; continue;}

  if (epvrv[i] == 0)
  {
   if (epvrvplan[i] == 2) {ch_nevip++; continue;}
  }

  if (epvrv[i] > 0)
  {
   if (epvrvplan[i] == 2) {ch_vip++; continue;}
   else {ch_vne++; continue;}
  }
 }

 ClearStatVRV();
 zapstatvrv.nz_dds=nz_dds;
 zapstatvrv.dat=dn;
 zapstatvrv.plan=ch_zapl;
 zapstatvrv.vip=ch_vip;
 zapstatvrv.vne=ch_vne;
 zapstatvrv.nevip=ch_nevip;
 zapstatvrv.noplan=ch_noplan;

 nz=InsertStatVRV(prsoob);
 if (nz == 0) {prd.close();return false;}

 if (ch_zapl == 0) {prd.close();return true;}

 ClearStatrvrv();
 zapstatrvrv.nz_statvrv=nz;
 prd.setLabelText("Анализ плана");
 for (qint32 i=0;  i<86400; i++)
 {
  prd.setValue(i);
  qApp->processEvents();
  if (epvrvplan[i] == 0) {t=t.addSecs(1);continue;}
  if ((epvrv[i] > 0) && (epvrvplan[i] > 1)) {t=t.addSecs(1);continue;}
  if ((epvrvplan[i] != 2) && (epvrv[i] == 0)) {t=t.addSecs(1);continue;}
  zapstatrvrv.tm=t;
  zapstatrvrv.kolpak=epvrv[i];
  zapstatrvrv.reg=epvrvplan[i];
  InsertStatrvrv(prsoob);
  t=t.addSecs(1);
 }

 prd.close();
 return true;
}

/*
bool ObrIzm::ZapStatVRV(QString nfile_alti, quint64 nz_dds)
{
 QByteArray buf;

// if (!ReadFile(nfile_alti,buf)) return false;
 if (!ZapStatVRV(nz_dds)) return false;
 buf.clear();

 return true;
}
*/

bool ObrIzm::Stat322(QStringList list)
{
 qlonglong kolstr;
 QString str;
 QList<int> shrc;
 QList<int> shps;
 qint32 dost;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ClearStat322();
 zapstat322.kizm=kolstr;
 zapstat322.dlsek=kolstr;

 for (int i=0; i<kolstr; i++)
 {
  str=list.at(i);
  str=str.simplified();
  dost=str.section(" ",14,14).toInt();
  dost=dost+str.section(" ",15,15).toInt();
  dost=dost+str.section(" ",16,16).toInt();
  if (dost != 3) {zapstat322.kizmn++; continue;}
  shrc << str.section(" ",10,10).toInt();
  shps << str.section(" ",11,11).toInt();
 }

 zapstat322.dlsekn=zapstat322.kizmn;

 if (shrc.isEmpty()) return true;
 if (shps.isEmpty()) return true;

 qSort(shrc);
 qSort(shps);

 kolstr=shrc.count();
 zapstat322.shrcmin=shrc[0];
 zapstat322.shrcmax=shrc[kolstr-1];
 zapstat322.shrcmed=shrc[kolstr/2];

 kolstr=shps.count();
 zapstat322.shpsmin=shps[0];
 zapstat322.shpsmax=shps[kolstr-1];
 zapstat322.shpsmed=shps[kolstr/2];

 return true;
}

bool ObrIzm::SetM(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 int ind;
 QDateTime dt;

 ClearRnx();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(nfile);
 zaprnx.naz=lf[3];

 ind=FindStrInList(list,"MARKER NAME");
 if (ind > 0)
 {
  str=list[ind];
  zaprnx.npn=str.left(4);
 }
 else zaprnx.npn=zaprnx.naz.mid(0,4);

 ind=FindStrInList(list,"END OF HEADER");
 if (ind == -1) return false;
 ind++;
 if (ind > kolstr) return false;

 zaprnx.ver=RnxVersion(list);
 zaprnx.tip="M";
 zaprnx.strk=0;
 zaprnx.kodobr=0;
 zaprnx.interv=600;

 str=list[ind];
 str=str.left(18);
 str=str.simplified();
 str="20"+str+".0";
 dt=SetDTFromStr(str);

 zaprnx.sutn=dt.date().dayOfYear();
 zaprnx.godn=dt.date().year();
 zaprnx.sekn=TimeToSec(dt.time());
 zaprnx.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");


 str=list[kolstr-1];
 str=str.left(18);
 str=str.simplified();
 str="20"+str+".0";
 dt=SetDTFromStr(str);

 zaprnx.sutk=dt.date().dayOfYear();
 zaprnx.godk=dt.date().year();
 zaprnx.sekk=TimeToSec(dt.time());
 zaprnx.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 return true;
}

bool ObrIzm::SetKos(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList lf;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString stime;
 qint32 sv;

 ClearDs();

 lf=FileInfo(nfile);
 zapds.naz=lf[3];
 zapds.naz=zapds.naz.toUpper();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 str=list[0];

 zapds.npn=str.mid(24,4);
 zapds.godn=str.mid(7,2).toInt()+2000;
 zapds.sutn=str.mid(9,3).trimmed().toInt();
 zapds.sekn=str.mid(12,5).trimmed().toInt();
 zapds.nka=str.mid(0,7);
 if (zapds.nka == "1603401") zapds.nka="126";
 if (zapds.nka == "1905701") zapds.nka="127";
 sv=str.mid(120,1).toInt();
 if (sv == 3) zapds.sv=1;
 if (sv == 7) zapds.sv=4;

 str=list[kolstr-1];

 zapds.godk=str.mid(7,2).toInt()+2000;
 zapds.sutk=str.mid(9,3).trimmed().toInt();
 zapds.sekk=str.mid(12,5).trimmed().toInt();

 dat=DayToDate(zapds.sutn,zapds.godn);
 time=SecToTime(zapds.sekn,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 dat=DayToDate(zapds.sutk,zapds.godk);
 time=SecToTime(zapds.sekk,stime);
 dt.setDate(dat); dt.setTime(time);
 zapds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapds.nazis="KOS";

 zapds.nz_edxr=0;

 return true;
}

bool ObrIzm::SetFrd(QString nfile)
{
 QStringList list;
 qlonglong kolstr;
 QString str,str1,nka;
 QStringList lf;
 QDateTime dt;
 qint32 sv,ind;

 ClearDs();

 lf=FileInfo(nfile);
 zapds.naz=lf[3];
 zapds.naz=zapds.naz.toUpper();

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 sv=FindStrInList(list,"H2");
 if (sv == -1) sv=FindStrInList(list,"h2");
 if (sv == -1) return false;
 str=list[sv];

 str=str.simplified();
 zapds.npn=str.section(" ",2,2);
 sv=str.section(" ",-1,-1).toInt();
 if (sv == 3) zapds.sv=1;
 if (sv == 7) zapds.sv=4;
 if (sv == 4) zapds.sv=5;

 sv=FindStrInList(list,"H3");
 if (sv == -1) sv=FindStrInList(list,"h3");
 if (sv == -1) return false;
 str=list[sv];

 str=str.simplified();
 nka=str.section(" ",2,2);
 zapds.nka="000";
 if (nka == "1603401") zapds.nka="126";
 if (nka == "1905701") zapds.nka="127";
 if (nka == "7603901") zapds.nka="lageos1";
 if (nka == "1205502") zapds.nka="galileo104";
 if (nka == "1403201") zapds.nka="glonass133";
 if (nka == "9207002") zapds.nka="lageos2";
 if (zapds.nka == "000")
 {
  ind=str.section(" ",1,1).indexOf("1603401");
  if (ind > -1) zapds.nka="126";
  ind=str.section(" ",1,1).indexOf("1905701");
  if (ind > -1) zapds.nka="127";
  ind=str.section(" ",1,1).indexOf("7603901");
  if (ind > -1) zapds.nka="lageos1";
  ind=str.section(" ",1,1).indexOf("1205502");
  if (ind > -1) zapds.nka="galileo104";
  ind=str.section(" ",1,1).indexOf("1403201");
  if (ind > -1) zapds.nka="glonass133";
  ind=str.section(" ",1,1).indexOf("9207002");
  if (ind > -1) zapds.nka="lageos2";
 }

 sv=FindStrInList(list,"H4");
 if (sv == -1) sv=FindStrInList(list,"h4");
 if (sv == -1) return false;
 str=list[sv];

 str=str.simplified();
 str1=str.section(" ",2,2)+" "+str.section(" ",3,3)+" "+str.section(" ",4,4)+" "+str.section(" ",5,5)+" "+str.section(" ",6,6)+" "+str.section(" ",7,7);
 dt=SetDTFromStr(str1);
 zapds.godn=dt.date().year();
 zapds.sutn=dt.date().dayOfYear();
 zapds.sekn=TimeToSec(dt.time());
 zapds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 str1=str.section(" ",8,8)+" "+str.section(" ",9,9)+" "+str.section(" ",10,10)+" "+str.section(" ",11,11)+" "+str.section(" ",12,12)+" "+str.section(" ",13,13);
 dt=SetDTFromStr(str1);
 zapds.godk=dt.date().year();
 zapds.sutk=dt.date().dayOfYear();
 zapds.sekk=TimeToSec(dt.time());
 zapds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapds.nazis="FRD";

 zapds.nz_edxr=0;

 return true;
}

/*!
 * \brief ObrIzm::ReadEdxrInBuf
 * чтение единицы хранения в область памяти
 * для единицы хранения заполняется структура readedxr,
 * определяется архивный файл и читается в область памяти (при необходимости разархивируется)
 * корректность чтения проверяется по КС
 * \param ntab[in] - название таблицы с единицей хранения
 * \param nz_tab[in] - № записи в таблице для единицы хранения
 * \param buf[out] - область памяти с содержимым файла
 * \param prsoob[in] - признак вывода сообщений
 * \return
 */
bool ObrIzm::ReadEdxrInBuf(QString ntab,quint64 nz_tab,QByteArray &buf,bool prsoob)
{
    QString nfile;
    qulonglong idlf;
    qint16 iks;
    qulonglong adlf;
    qint16 aks;

    if (!ReadEdxr(ntab,nz_tab,prsoob)) return false;

    nfile=readedxr.path_arx+"/"+readedxr.arxf;
    if (readedxr.prarx == 1) readedxr.prarx=2;
    if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) return false;
    if (readedxr.aks != aks) return false;
    return true;
}

bool ObrIzm::ReadEdxrInList(QString ntab,quint64 nz_tab,QStringList &list,bool prsoob)
{
    QByteArray buf;

    if (!ReadEdxrInBuf(ntab, nz_tab, buf, prsoob)) return false;
    list.clear();

//    WriteFile(zapust.path_temp+"/tmp_"+readedxr.isxf, buf, true);
//    list=ReadFileInList(zapust.path_temp+"/tmp_"+readedxr.isxf);
    WriteFile(zapust.path_temp+"/tmp/"+readedxr.isxf, buf, true);
    list=ReadFileInList(zapust.path_temp+"/tmp/"+readedxr.isxf);
    qDebug() << "\t Name file = " << zapust.path_temp+"/tmp_"+readedxr.isxf;
    qDebug() << "\t list.sz = " << list.size() << " buf.sz = " << buf.size();
//    DelFile(zapust.path_temp+"/tmp_"+readedxr.isxf);
    DelFile(zapust.path_temp+"/tmp/"+readedxr.isxf);

    return true;
}

bool ObrIzm::ZapStatDDS(quint64 nz_dds,bool prsoob)
{
 QStringList stat;
 qlonglong kolstr;
 qint32 kol1;
 qint32 kol2;
 qint32 kol3;
 qint32 kolep;
 QDateTime dt;
 QString str,str1;
 int ind;
 quint64 nz,nz1;
 QStringList list;

 if (!ReadEdxrInList("dds",nz_dds,list,prsoob)) return false;

 kolstr=list.count();
 if (kolstr == 0) return false;

 ind=FindStrInList(list,">");
 if (ind == -1) return false;
 str=list.at(ind);
 dt=DTForHdrDatRnx(str);
 if (!dt.isValid()) return false;

 if (!StatEpochForDDS(list,kol1,kol2,kol3,kolep)) return false;

 ClearStatDDS();

 zapstatdds.dat=dt.date();
 zapstatdds.eppl=8640;
 zapstatdds.eppol=kolep;
 zapstatdds.ep_4mglo=kol1;
 zapstatdds.ep_4rglo=kol2;
 zapstatdds.ep_4bglo=kol3;
 zapstatdds.nz_dds=nz_dds;
 for (int i=0; i<24; i++) zapstatdds.mas_ep[i]=kol_ep_dds[i];

 nz=InsertStatDDS(true);
 if (nz == 0) return false;

 stat=StatDDS(list);
 kolstr=stat.count();

 for (int i=0; i<kolstr; i++)
 {
  ClearStatrDDS();
  str=stat.at(i);
  str=str.simplified();

  zapstatrdds.nz_statdds=nz;
  str1=str.section(",",1,1);
  zapstatrdds.vrn=QTime::fromString(str1);
  str1=str.section(",",3,3);
  zapstatrdds.vrr=QTime::fromString(str1);
  str1=str.section(",",2,2);
  zapstatrdds.kolep=str1.toInt();

  nz1=InsertStatrDDS(true);
  if (nz1 == 0)
  {
   DeleteRow("statdds",nz,prsoob);
   return false;
  }
 }

 return true;
}
/*
bool ObrIzm::ZapStatVRV(quint64 nz_dds,bool prsoob)
{
 QByteArray buf;

 if (!ReadEdxrInBuf("dds",nz_dds,buf,prsoob)) return false;

 if (!ZapStatVRV(buf,nz_dds)) return false;

 return true;
}
*/
bool ObrIzm::EdvidForDs(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidDs(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadDs(list[i],prsoob)) return false;
   dt=DayToDate(zapds.sutn,zapds.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zapds.nz_edxr;
   zapedvid.naz_edxr=zapds.naz;
   zapedvid.tip=zapds.nazis;
   InsertEdvid(prsoob);
  }
 }

 return true;
}

bool ObrIzm::EdvidForDds(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidDds(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadDDS(list[i],prsoob)) return false;
   dt=DayToDate(zapdds.sutn,zapdds.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zapdds.nz_edxr;
   zapedvid.naz_edxr=zapdds.naz;
   zapedvid.tip=zapdds.tip;
   InsertEdvid(prsoob);
  }
 }

 return true;
}

bool ObrIzm::EdvidForPpsi(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidPpsi(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadFullppsi(list[i],prsoob)) return false;
   dt=DayToDate(zapfull.sutn,zapfull.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zapfull.nz_edxr;
   zapedvid.naz_edxr=zapfull.naz;
   zapedvid.tip="FULL";
   InsertEdvid(prsoob);
  }
 }

 return true;
}

bool ObrIzm::EdvidForRnxi(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidRnxi(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadRnx("rnxi",list[i],prsoob)) return false;
   dt=DayToDate(zaprnx.sutn,zaprnx.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zaprnx.nz_edxr;
   zapedvid.naz_edxr=zaprnx.naz;
   zapedvid.tip="I_"+zaprnx.tip;
   InsertEdvid(prsoob);
  }
 }

 return true;
}

bool ObrIzm::EdvidForRnxv(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidRnxv(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadRnx("rnxv",list[i],prsoob)) return false;
   dt=DayToDate(zaprnx.sutn,zaprnx.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zaprnx.nz_edxr;
   zapedvid.naz_edxr=zaprnx.naz;
   zapedvid.tip="V_"+zaprnx.tip;
//   if (zaprnx.tip == "O")
//   {
//    if (zapzapr.rnx2) zapedvid.preobr=1;
//    else zapedvid.preobr=0;
//   }
   InsertEdvid(prsoob);
  }
 }

 return true;
}

/*!
 * \brief ObrIzm::ObAltiInMapF
 * Формирование объединённого ключевого списка и области данных из файлов ALTI
 * в качестве ключа используется дата, время и интервал(мкс)
 * данными являются смещения в области данных obuf
 * в результате работы формируется отсортированный по дате и времени список смещений на кадры в области памяти(неотсортированные)
 * \param nz[in] список ключей файлов ALTI из таблицы dds для объединения
 * \param map[out] сформированный ключевой список
 * \param okolpak[out] количество пакетов в объединённом ключевом списке
 * \param i_file[out] - целесообразно ????????????????????
 * \param obuf[out] область данных объединённых файлов ALTI
 * \param dlpak[out] - целесообразно ????????????????????
 * \return при неудачном формировании ключевого списка возвращает false, инче true
*/
bool ObrIzm::ObAltiInMapF(QList<qlonglong> nz,QMap<QString,QString> &map,qint64 &okolpak,QString &i_file,QByteArray &obuf,qint32 &dlpak,bool prsoob)
{
 QByteArray buf;
 QDateTime dtn,dtk;
 qint64 kolpak;
 qint32 dlsh;
 QString v_file;
 qint32 kolsut;
 qint32 dlbuf;
 QFile f;
 QString nfile;
 const char* uk;
 qint64 dl;

 map.clear();
 obuf.clear();
 okolpak=0;
 dlbuf=0;
 nfile=zapust.path_temp+"/obalti.tmp";
 f.setFileName(nfile);
 f.remove();
 if (! f.open(QIODevice::WriteOnly)) return false;

 for (int i=0; i<nz.count(); i++)
 {
  if (!ReadDDS(nz[i],prsoob)) return false;
  if (!ReadEdxrInBuf("dds",zapdds.nz,buf,prsoob)) return false; //=== замена
  if (!InfFromAlti(buf,dtn,dtk,kolpak,dlpak,dlsh,i_file,v_file,kolsut)) return false;
  okolpak=okolpak+kolpak;
  dlbuf=buf.length();
  uk=buf.constData();
  uk=uk+dlsh;
  dl=f.write(uk,dlbuf-dlsh);
  buf.clear();
 }

 f.close();
 ReadFile(nfile,obuf);
 AltiInMap(obuf,map,dlpak,okolpak);

 return true;
}

bool ObrIzm::ObAltiInMapF(QStringList naz_file,QMap<QString,QString> &map,qint64 &okolpak,QString &i_file,QByteArray &obuf,qint32 &dlpak,bool prsoob)
{
 QByteArray buf;
 QDateTime dtn,dtk;
 qint64 kolpak;
 qint32 dlsh;
 QString v_file;
 qint32 kolsut;
 qint32 dlbuf;
 QFile f;
 QString nfile;
 const char* uk;
 qint64 dl;

 map.clear();
 obuf.clear();
 okolpak=0;
 dlbuf=0;
 nfile=zapust.path_temp+"/obalti.tmp";
 f.setFileName(nfile);
 f.remove();
 if (! f.open(QIODevice::WriteOnly)) return false;

 for (int i=0; i<naz_file.count(); i++)
 {
  if (!ReadFile(naz_file[i],buf)) return false;
  if (!InfFromAlti(buf,dtn,dtk,kolpak,dlpak,dlsh,i_file,v_file,kolsut)) return false;
  okolpak=okolpak+kolpak;
  dlbuf=buf.length();
  uk=buf.constData();
  uk=uk+dlsh;
  dl=f.write(uk,dlbuf-dlsh);
  buf.clear();
 }

 f.close();
 ReadFile(nfile,obuf);
 AltiInMap(obuf,map,dlpak,okolpak);

 return true;
}

bool ObrIzm::SetAltInBD(QString nfile,bool prsoob)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 QStringList dat_list;
 QStringList f;
 QDateTime dt;

 ClearAlt();

 f=FileInfo(nfile);
 zapalt.nfile=f[2];
 zapalt.nka=zapalt.nfile.mid(0,3);

 str="DELETE FROM alt WHERE substr(nfile,1,14) = '"+zapalt.nfile.mid(0,14)+"'";
 db.exec(str);

 list=ReadFileInList(nfile);
 kolstr=list.count();
 if (kolstr == 0) return false;

 for (int i=0; i<kolstr; i++)
 {
  str=list[i];
  str=str.simplified();

  zapalt.datn=QDate::fromString(str.section(" ",0,0),"yyyy-MM-dd");
  if (!zapalt.datn.isValid()) return false;
  zapalt.vrn=QTime::fromString(str.section(" ",1,1),"hh:mm:ss");
  zapalt.prod=str.section(" ",2,2).toLongLong();
  zapalt.reg=str.section(" ",3,3).toInt();
  zapalt.nvit=str.section(" ",4,4).toLongLong();
  zapalt.datk=QDate::fromString(str.section(" ",5,5),"yyyy-MM-dd");
  zapalt.vrk=QTime::fromString(str.section(" ",6,6),"hh:mm:ss");
  if (!zapalt.datk.isValid())
  {
   dt.setDate(zapalt.datn); dt.setTime(zapalt.vrn);
   dt=dt.addSecs(zapalt.prod);
   zapalt.datk=dt.date();
   zapalt.vrk=dt.time();
  }

 // dat_list << zapalt.datn.toString("dd-MM-yyyy");
 // dat_list << zapalt.datk.toString("dd-MM-yyyy");


  dat_list << zapalt.datn.toString("yyyy-MM-dd");
  dat_list << zapalt.datk.toString("yyyy-MM-dd");

  InsertAlt(prsoob);
 }

 DelDublInList(dat_list);

 for (qint32 i=0; i<dat_list.count(); i++)
 {
  DeleteRow("statvrv","dat",dat_list[i],prsoob);
 }

 return true;
}

bool ObrIzm::ZapEpvrv(QByteArray &buf)
{
    qint64 ind=0;
    qint32 dls,id,rez;
    qint64 kolpak;
    QByteArray shap;
    QDataStream s(&shap,QIODevice::ReadOnly);
    qint64 indm;


 if (buf.isEmpty()) return false;

 shap=buf.mid(8,20);
 s >> id >> rez >> kolpak >> dls;

 if (dls == 0) return false;

 ind=ind+dls;

 for (qint32 i=0; i<86400; i++) epvrv[i]=0;

 for (qint32 j=0; j<kolpak; j++)
 {
  ReadZagVRV(buf,ind-6); //чтение заголовка кадра ВРВ

  indm=(zagvrv.timen.hour()*3600)+(zagvrv.timen.minute()*60)+zagvrv.timen.second();
  epvrv[indm]++; //определение индекса массива №секунды и отметка о наличии измерений

  ind=ind+zagvrv.dlpak+7;
  qApp->processEvents();
 };

 return true;
}

bool ObrIzm::ZapEpvrvplan(QDate dat,QString nka)
{
 QString query;
 QList<qlonglong> list_nz;
 QSqlQuery sql;
 qint32 indm,indm1;

 query="SELECT alt.nz FROM alt WHERE alt.nka = '"+nka+"' AND (alt.datn = '"+dat.toString("dd-MM-yyyy")+"' OR alt.datk = '"+dat.toString("dd-MM-yyyy")+"') ORDER BY alt.datn ASC,alt.vrn ASC";

 for (qint32 i=0; i<86400; i++) epvrvplan[i]=0;

 list_nz=ExecQueryList(sql,query,true);

 for (qint32 i=0; i<list_nz.count(); i++)
 {
  ReadAlt(list_nz[i],true);
  indm=(zapalt.vrn.hour()*3600)+(zapalt.vrn.minute()*60)+zapalt.vrn.second();
  indm1=(zapalt.vrk.hour()*3600)+(zapalt.vrk.minute()*60)+zapalt.vrk.second();
  if (zapalt.datn < dat) indm=0;
  if (zapalt.datk > dat) indm1=86399;
  for (qint32 j=indm; j<=indm1; j++) epvrvplan[j]=zapalt.reg;
  qApp->processEvents();
 }

 return true;
}

bool ObrIzm::ZapStatVRVInList(quint64 nz_dds, QStringList &list)
{
 QByteArray buf;
 QDateTime dtn;
 QDate dn;
 QString str;
 qint32 ch_vne=0;
 qint32 ch_vip=0;
 qint32 ch_nevip=0;
 qint32 ch_noplan=0;
 qint32 ch_zapl=0;
 QTime t;

 list.clear();

 if (!ReadEdxrInBuf("dds",nz_dds,buf,true)) return false;
 if (!ReadDDS(nz_dds,true)) return false;
 dtn=QDateTime::fromString(zapdds.dtn,"dd-MM-yyyy hh:mm:ss");
 dn=dtn.date();
 t.setHMS(0,0,0);

 if (!ZapEpvrv(buf)) return false;
 if (!ZapEpvrvplan(dn,zapdds.nka)) return false;

 for (qint32 i=0;  i<86400; i++)
 {
  str=t.toString("hh:mm:ss")+" : ";
  str=str+"Получено пакетов : "+QString::number(epvrv[i])+" ; Режим работы : "+QString::number(epvrvplan[i]);
  t=t.addSecs(1);
  if (epvrv[i] > 0) continue;
  if ((epvrvplan[i] != 2) && (epvrv[i] == 0)) continue;
  list << str;
 }

 for (qint32 i=0; i<86400; i++)
 {
  if (epvrvplan[i] == 2) ch_zapl++;
  if (epvrvplan[i] == 0) {ch_noplan++; continue;}

  if (epvrv[i] == 0)
  {
   if (epvrvplan[i] == 2) {ch_nevip++; continue;}
  }

  if (epvrv[i] > 0)
  {
   if (epvrvplan[i] == 2) {ch_vip++; continue;}
   else {ch_vne++; continue;}
  }
 }

 str="Запланировано эпох : "+QString::number(ch_zapl);
 list << str;
 str="Выполнено : "+QString::number(ch_vip);
 list << str;
 str="Не выполнено : "+QString::number(ch_nevip);
 list << str;
 str="Эпох вне плана : "+QString::number(ch_vne);
 list << str;
 str="Нет плана : "+QString::number(ch_noplan);
 list << str;
 if (ch_zapl > 0)
 {
  str="Выполнено запланированных эпох % : "+QString::number((ch_vip*100)/ch_zapl);
  list << str;
 }

 return true;
}

bool ObrIzm::SetL(QString nfile)
{
 QStringList lf;
 QString newname;
 QDateTime dn,dk;
 bool fl;

 lf=FileInfo(nfile);
 newname=lf[3];

 QFile *JPS = new QFile(nfile);
 QFile *LOG = new QFile(nfile+".log");

 converterJps::getDopplerStatus(true);
 converterJps::getInf("CGTR 1.05", "RKS", "001", "002", "SN0001", "RKS T1", "T1.001", "SN0001", "RKS A1");
 //Контроль целостности файла
 integrityControl *Control = new integrityControl(JPS,LOG);
 dn.setDate(Control->timeOfFirst->date());
 dn.setTime(Control->timeOfFirst->time());
 dk.setDate(Control->timeOfLast->date());
 dk.setTime(Control->timeOfLast->time());
 fl=Control->fileQualityFlag;

 LOG->remove();

 SetRnxL(newname,dn,dk,fl);

 return true;
}

bool ObrIzm::RasStatDs(QDate dn, QDate dk,bool prsoob)
{
 QString sdn,sdk;
 QString str;
 QList<qlonglong> list;
 QSqlQuery query;
 qint32 zapr;
 QProgressDialog prd;

 sdn=QString::number((dn.year()*1000)+dn.dayOfYear());
 sdk=QString::number((dk.year()*1000)+dk.dayOfYear());

 str="SELECT ds.nz,stat313.nz_educh FROM ds LEFT OUTER JOIN stat313 ON (ds.nz = stat313.nz_educh) WHERE "
     "ds.nazis = 'DS' AND ((ds.godn*1000)+ds.sutn BETWEEN "+sdn+" AND "+sdk+") AND stat313.nz_educh IS NULL";

 list=ExecQueryList(query,str,prsoob);
 if (list.isEmpty()) return true;

 str="Имеются сеансы без статистики. Расчитать статистику ?";
 zapr=QMessageBox::question(0,QObject::tr("Расчёт статистики"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
 if (zapr == QMessageBox::No) return true;

 prd.setModal(true);
 prd.setCancelButton(0);
 prd.setWindowTitle("Статистика ДС");
 prd.setLabelText("Расчёт статистики");
 prd.setMinimum(0);
 prd.setMaximum(list.count()-1);
 prd.show();


 for (qint32 i=0; i<list.count(); i++)
 {
  Stat313(list[i],prsoob);
  prd.setValue(i);
  qApp->processEvents();
 }

 prd.close();

 return true;
}

bool ObrIzm::Stat313(quint64 nz_ds,bool prsoob)
{
 QStringList list;

 if (!ReadEdxrInList("ds",nz_ds,list,prsoob)) return false;

 if (!Stat313(list)) return false;

 zapstat313.nz_educh=nz_ds;
 InsertStat313(prsoob);

 return true;
}

bool ObrIzm::RasStatDzs(QDate dn, QDate dk,bool prsoob)
{
 QString sdn,sdk;
 QString str;
 QList<qlonglong> list;
 QSqlQuery query;
 qint32 zapr;
 QProgressDialog prd;

 sdn=QString::number((dn.year()*1000)+dn.dayOfYear());
 sdk=QString::number((dk.year()*1000)+dk.dayOfYear());

 str="SELECT ds.nz,stat322.nz_educh FROM ds LEFT OUTER JOIN stat322 ON (ds.nz = stat322.nz_educh) WHERE "
     "ds.nazis = 'DZS' AND ((ds.godn*1000)+ds.sutn BETWEEN "+sdn+" AND "+sdk+") AND stat322.nz_educh IS NULL";

 list=ExecQueryList(query,str,prsoob);
 if (list.isEmpty()) return true;

 str="Имеются сеансы без статистики. Расчитать статистику ?";
 zapr=QMessageBox::question(0,QObject::tr("Расчёт статистики"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
 if (zapr == QMessageBox::No) return true;

 prd.setModal(true);
 prd.setCancelButton(0);
 prd.setLabelText("Расчёт статистики");
 prd.setWindowTitle("Статистика ДЗС");
 prd.setMinimum(0);
 prd.setMaximum(list.count()-1);
 prd.show();


 for (qint32 i=0; i<list.count(); i++)
 {
  Stat322(list[i],prsoob);
  prd.setValue(i);
  qApp->processEvents();
 }

 prd.close();

 return true;
}

bool ObrIzm::Stat322(quint64 nz_ds,bool prsoob)
{
 QStringList list;

 if (!ReadEdxrInList("ds",nz_ds,list,prsoob)) return false;

 if (!Stat322(list)) return false;

 zapstat322.nz_educh=nz_ds;
 InsertStat322(prsoob);

 return true;
}

bool ObrIzm::RasStatDds(QDate dn, QDate dk,bool prsoob)
{
 QString sdn,sdk;
 QString str;
 QList<qlonglong> list;
 QSqlQuery query;
 qint32 zapr;
 QProgressDialog prd;

 sdn=QString::number((dn.year()*1000)+dn.dayOfYear());
 sdk=QString::number((dk.year()*1000)+dk.dayOfYear());

 str="SELECT dds.nz FROM dds LEFT OUTER JOIN statdds ON (dds.nz = statdds.nz_dds) WHERE tip = 'KVI' AND kodobr = 0 "
     " AND ((godn*1000)+sutn BETWEEN "+sdn+" AND "+sdk+") AND statdds.nz IS NULL ";

 list=ExecQueryList(query,str,prsoob);
 if (list.isEmpty()) return true;

 str="Имеются сеансы без статистики. Расчитать статистику ?";
 zapr=QMessageBox::question(0,QObject::tr("Расчёт статистики"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
 if (zapr == QMessageBox::No) return true;

 prd.setModal(true);
 prd.setCancelButton(0);
 prd.setLabelText("Расчёт статистики");
 prd.setWindowTitle("Статистика ДДС");
 prd.setMinimum(0);
 prd.setMaximum(list.count()-1);
 prd.show();


 for (qint32 i=0; i<list.count(); i++)
 {
  ZapStatDDS(list[i],prsoob);
  prd.setValue(i);
  qApp->processEvents();
 }

 prd.close();

 return true;
}

bool ObrIzm::RasStatVrv(QDate dn, QDate dk,QString nka,bool prsoob)
{
 QString sdn,sdk;
 QString str;
 QList<qlonglong> list;
 QSqlQuery query;
 qint32 zapr;
 quint64 nz;
 QString str_nka;

 if (nka.isNull()) str_nka.clear();
 else str_nka=" AND nka='"+nka+"' ";

 sdn=QString::number((dn.year()*1000)+dn.dayOfYear());
 sdk=QString::number((dk.year()*1000)+dk.dayOfYear());

 str="SELECT dds.nz FROM dds LEFT OUTER JOIN statvrv ON (dds.nz = statvrv.nz_dds) WHERE tip = 'ALTI' AND kodobr = 0 "+str_nka;
 str=str+" AND ((godn*1000)+sutn BETWEEN "+sdn+" AND "+sdk+") AND statvrv.nz IS NULL ";

 list=ExecQueryList(query,str,prsoob);
 if (list.isEmpty()) return true;

 str="Имеются сеансы без статистики. Расчитать статистику ?";
 zapr=QMessageBox::question(0,QObject::tr("Расчёт статистики"),QObject::tr(str.toStdString().c_str()),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
 if (zapr == QMessageBox::No) return true;

 for (qint32 i=0; i<list.count(); i++)
 {
  ZapStatVRV(list[i],nz,prsoob);
  qApp->processEvents();
 }

 return true;
}

bool ObrIzm::SetRnxL(QString naz_file,QDateTime dn,QDateTime dk,bool kontr)
{
 ClearRnx();

 zaprnx.naz=naz_file;
 zaprnx.naz=zaprnx.naz.toUpper();

 zaprnx.npn=zaprnx.naz.left(4);

 zaprnx.sutn=dn.date().dayOfYear();
 zaprnx.godn=dn.date().year();
 zaprnx.sekn=TimeToSec(dn.time());
 zaprnx.dtn=dn.toString("dd-MM-yyyy hh:mm:ss");

 zaprnx.sutk=dk.date().dayOfYear();
 zaprnx.godk=dk.date().year();
 zaprnx.sekk=TimeToSec(dk.time());
 zaprnx.dtk=dk.toString("dd-MM-yyyy hh:mm:ss");

 zaprnx.kodobr=10;
 zaprnx.strk=0;
 zaprnx.tip="L";
 zaprnx.ver.clear();
 zaprnx.nz_edxr=0;
 zaprnx.interv=0;

 if (!kontr) {zaprnx.kodobr=1; zaprnx.strk=2;}

 return true;
}

bool ObrIzm::SetPpsi(QString nfile,bool prsoob)
{

     QStringList list;
     qlonglong kolstr;
     QString str;
     QStringList f;
     QDate date;
     QTime tm;
     quint64 nz_plppsi,nz_splppsi;

     ClearPPSI();

     f=FileInfo(nfile);
     zapplppsi.naz = f[3];
     zapplppsi.nka = zapplppsi.naz.left(3).toInt();
     date=QDate::fromString(zapplppsi.naz.mid(4,10),"yyyy_MM_dd");
     tm=QTime::fromString(zapplppsi.naz.mid(15,8),"hh_mm_ss");
     DateToDay(date, zapplppsi.sutn, zapplppsi.godn);
     if (zapplppsi.godn < 99) zapplppsi.godn=zapplppsi.godn+2000;
     zapplppsi.sekn = TimeToSec(tm);
     zapplppsi.npn = "624P";
     zapplppsi.tip="PPSI";

     str="DELETE FROM plppsi WHERE naz = '" + zapplppsi.naz + "'";
     db.exec(str);

     nz_plppsi=InsertPPSI(prsoob);
     if (nz_plppsi == 0) return false;


     list=ReadFileInList(nfile);
     kolstr=list.count();
     if (kolstr == 0) return false;

     for (int i=2; i<kolstr; i++){
        str=list[i];
        str=str.simplified();
        tm=QTime::fromString(str.section(" ",0,0),"hh:mm:ss");
        zapsplppsi.sek=TimeToSec(tm);
        zapsplppsi.az = str.section(" ",1,1).toDouble();
        zapsplppsi.um = str.section(" ",2,2).toDouble();
        zapsplppsi.nz_plppsi = nz_plppsi;

      nz_splppsi = InsertSPLPPSI(prsoob);
      if (nz_splppsi == 0)
      {
       DeleteRow("plppsi","nz",nz_plppsi,true);
       return false;
      }
     }
     return true;

}

bool ObrIzm::ObrKod21(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz,nz2;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString newname,nfile,vfile;
 QFile file;
 QStringList rnx;
 QStringList lf;
 QByteArray buf;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;

 ClearSlu();

 list=GetKodobr("rnxi",21,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i); nz2=nz;

  if (! ReadRnx("rnxi",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxi",zaprnx.nz,prsoob)) continue;

  nfile=readedxr.path_arx+"/"+readedxr.arxf;
  if (readedxr.prarx == 1) readedxr.prarx=2;
  if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) continue;
  if (readedxr.aks != aks) continue;
  nfile=path_temp+"/"+readedxr.isxf;
  if (WriteFile(nfile,buf,true) == -1)continue;
  buf.clear();

  if (! RnxToRnx(nfile)) return false;

  vfile=nfile;

  lf=FileInfo(vfile);
  newname=NazOFile(vfile);
  newname=lf[1]+"/"+newname;


  file.setFileName(vfile);
  if (!file.exists()) return false;

  rnx=ReadFileInList(vfile);
  if (rnx.isEmpty()) return false;
  UpdatePrnOfObs(rnx);
  if (!WriteFileFromList(rnx,newname)) return false;
  DelFile(vfile);

  ClearSxema();

  nz=SetRnxFileO(newname,false,prsoob);
  if (nz == 0) return false;

  ClearSxema();
  zapsxema.nz_educh=nz2;
  zapsxema.nz_educho1=nz;
  zapsxema.nz_educho2=0;
  zapsxema.pr_isx=0;
  InsertSxema("sxema1",prsoob);

  UpdateRow("rnxi","kodobr",5,nz2,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

bool ObrIzm::RemoveFullInBasket(quint64 nz_full, bool prsoob)
{
 QString nfile,prot;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf("fullppsi",nz_full,buf,prsoob))
 {
  ReadFullppsi(nz_full,prsoob);
  if (zapfull.nz_edxr == 0) DeleteRow("fullppsi",nz_full,prsoob);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow("fullppsi",nz_full,prsoob)) return false;

 prot=readedxr.path_arx+"/PRTKVI_"+readedxr.arxf.mid(5);
 DelFile(prot);
 prot=readedxr.path_arx+"/PRTKVNO_"+readedxr.arxf.mid(5);
 DelFile(prot);
 prot=readedxr.path_arx+"/PRTKVRV_"+readedxr.arxf.mid(5);
 DelFile(prot);

 UpdateRows("sxema","nz_educho2",1,"nz_educh",nz_full,prsoob,1);

 return true;
}

bool ObrIzm::CopyEdxr(QString ntab,quint64 nz_tab,QString path_vix,bool prsoob,QString &nfile)
{
 QByteArray buf;
// QString nfile;

 if (!ReadEdxrInBuf(ntab,nz_tab,buf,prsoob)) return false;

 nfile=path_vix+"/"+readedxr.isxf;

 if (WriteFile(nfile,buf,true) == -1) return false;

 return true;
}

bool ObrIzm::CreateFileForPpsi(QDate dat,qint32 kod,QString &naz_file,bool prsoob,QString nka,QList<qlonglong> &list,qint32 &kodobr)
{
 QDate dat_ppsi;
 QString nfile,nfileprt,str;
 QStringList spis_f,spis_fr,spis_fo;
 QByteArray buf,buf1,buf0;
 QDate dn,dk;
 QStringList sod,hdr;
 QMap<QString,QString> map,map1;
 QStringList ppsi_file;
 QProgressDialog prd;
 qint32 kolsut;
 QMap<QDateTime,QString> mapk;
 QDateTime nep;
 QStringList sp_f,prot;
 QString infile;
 QDateTime dtn,dtk;
 qint32 nkan;
 qint64 okolpak,kolpak;
 QString i_file;
 QByteArray obuf;
 qint32 dlpak;
 QStringList err;

 naz_file.clear();
 spis_f.clear();
 spis_fo.clear();
 sod.clear();
 map.clear();
 ppsi_file.clear();
 list.clear();
 kodobr=0;

 ClearDir(zapust.path_temp);

 //поиск записей ППСИ для создания суточного файла (kod: 1-КВИ, 2-КВНО, 3-ВРВ(ALTI))
  list=FindRowPpsiForDat(dat,kod,prsoob,nka);
  if (list.isEmpty()) return false;

  for (qint32 i=0; i<list.count(); i++)
  {
   if (!ReadFullppsi(list[i],prsoob))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false; //чтение данных в zapfull для записи fullppsi
   }
   if (zapfull.nz_edxr == 0) //для записи нет единицы хранения
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false; //чтение данных в zapfull для записи fullppsi
   }
   if (!InsertObr(komp_name,zapfull.nz_edxr))
   {
    DeleteRow("obr","naz_komp",komp_name,prsoob);
    return false; //чтение данных в zapfull для записи fullppsi
   }
  }

  str="Конвертирование файлов ППСИ";
  prd.setModal(true);
  prd.setCancelButton(0);
  prd.setLabelText(str);
  prd.setMinimum(0);
  prd.setMaximum(list.count()-1);
  prd.setWindowTitle("Обработка");
  prd.show();
  qApp->processEvents();

   for (qint32 i=0; i<list.count(); i++)
   {
    if (!ReadFullppsi(list[i],prsoob)) return false; //чтение данных в zapfull для записи fullppsi
    if (zapfull.nz_edxr == 0) return false; //для записи нет единицы хранения

    dat_ppsi=DayToDate(zapfull.sutk,zapfull.godk);

    //чтение и проверка корректности КС единицы хранения
    ReadEdxr("fullppsi",list[i],prsoob);
    if (!ReadEdxrInBuf("fullppsi",list[i],buf,prsoob))
    {
     DeleteRow("obr","naz_komp",komp_name,prsoob);
     return false; //чтение данных в zapfull для записи fullppsi
    }
    infile=readedxr.path_arx+"/"+readedxr.isxf;

    if (kod == 3)
    {
     if (!ReadEdxrInBuf("fullppsi",list[i],buf1,prsoob))
     {
      DeleteRow("obr","naz_komp",komp_name,prsoob);
      return false; //чтение данных в zapfull для записи fullppsi
     }
    }

    if (kod == 1)
    {
     ClearStatFull();
     nfile=KonvFullDDS(buf,zapust.path_temp,zapfull.nka,readedxr.isxf,dat_ppsi,prot);
     buf.clear();
     if (nfile.isEmpty()) continue;
     spis_f << nfile;
     UpdateRowStatFull(statfull.k_kvi,statfull.crc_kvi,statfull.och_kvi,"k_kvi","crc_kvi","och_kvi",zapfull.nz,prsoob);
     nfileprt=readedxr.path_arx+"/"+"PRTKVI_"+readedxr.arxf.mid(5);
     if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
    }

    if (kod == 2)
    {
     ClearStatFull();
     nfile=KonvFullKVNO(buf,zapust.path_temp,zapfull.nka,dat_ppsi,prot);
     buf.clear();
     if (nfile.isEmpty()) continue;
     spis_f << nfile;
     UpdateRowStatFull(statfull.k_kvno,statfull.crc_kvno,statfull.och_kvno,"k_kvno","crc_kvno","och_kvno",zapfull.nz,true);
     nfileprt=readedxr.path_arx+"/"+"PRTKVNO_"+readedxr.arxf.mid(5);
     if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
    }

    if (kod == 3)
    {
     ClearStatFull();
     sp_f=CreateAllAltiVRV(buf,buf1,zapust.path_temp,zapfull.nka,0,infile,buf0,false,dat_ppsi,prot,zapfull.kan);
     if (sp_f.isEmpty()) continue;
     AddList(spis_f,sp_f);
     qApp->processEvents();
     buf0.clear();buf.clear();buf1.clear();

     UpdateRowStatFullVRV(statfull.k1_vrv1,statfull.k1_vrv2,statfull.k2_vrv1,statfull.k2_vrv2,statfull.crc1_vrv1,statfull.crc1_vrv2,
                          statfull.crc2_vrv1,statfull.crc2_vrv2,statfull.och1_vrv1,statfull.och1_vrv2,statfull.och2_vrv1,
                          statfull.och2_vrv2,statfull.k_vrv,list[i],prsoob);
     nfileprt=readedxr.path_arx+"/"+"PRTVRV_"+readedxr.arxf.mid(5);
     if (!prot.isEmpty()) WriteFileFromList(prot,nfileprt);
    }

    prd.setValue(i+1);
    qApp->processEvents();
   }
   DeleteRow("obr","naz_komp",komp_name,prsoob);
   prd.close();

   if (spis_f.isEmpty()) return false;

   str="Разбор файлов по суткам";
   prd.setModal(true);
   prd.setCancelButton(0);
   prd.setLabelText(str);
   prd.setMinimum(0);
   prd.setMaximum(spis_f.count()-1);
   prd.show();

   if (kod == 1)
   {
    for (qint32 i=0; i<spis_f.count(); i++)
    {
     nfile=zapust.path_temp+"/"+spis_f[i];
     if (!RazbAllOFile(nfile,spis_fr))
        {prd.setValue(i);qApp->processEvents();spis_fo << nfile;continue;}
     for (qint32 j=0; j<spis_fr.count(); j++)
     {
      kolsut=KolSutInRnx(spis_fr[j],dn,dk);
      if (kolsut == 0) {DelFile(spis_fr[j]); continue;}
      if (dn == dat) {spis_fo << spis_fr[j];continue;}
      else DelFile(spis_fr[j]);
     }
     prd.setValue(i);
     qApp->processEvents();
    }
   }

   if (kod == 2)
   {
    for (qint32 i=0; i<spis_f.count(); i++)
    {
     nfile=zapust.path_temp+"/"+spis_f[i];
     spis_fr=RazbKFile(nfile);
     if (spis_fr.count() == 1) {prd.setValue(i);qApp->processEvents();spis_fo << nfile;continue;}
     for (qint32 j=0; j<spis_fr.count(); j++)
     {
      kolsut=KolSutInK(zapust.path_temp+"/"+spis_fr[j],dn,dk);
      if (kolsut == 0) {DelFile(zapust.path_temp+"/"+spis_fr[j]); continue;}
      if (dn == dat) {spis_fo << zapust.path_temp+"/"+spis_fr[j];continue;}
      else DelFile(zapust.path_temp+"/"+spis_fr[j]);
     }
     prd.setValue(i);
     qApp->processEvents();
    }
   }

   if (kod == 3)
   {
    for (qint32 i=0; i<spis_f.count(); i++)
    {
     if (!ReadFile(spis_f[i],buf))
     {
      ClearDir(zapust.path_temp);
      DeleteRow("obr","naz_komp",komp_name,prsoob);
      prd.close();
      return false;
     }
     sp_f=FileInfo(spis_f[i]);
     str=sp_f[3]; nkan=str.right(1).toInt();
     kolsut=KolSutInAlti(buf,dtn,dtk);
     if (kolsut < 2) {prd.setValue(i);qApp->processEvents();spis_fo << spis_f[i];continue;}

     spis_fr=RazbAlti(buf,zapust.path_temp,prsoob,0,nkan);
     if (!spis_fr.isEmpty()) DelFile(spis_f[i]);
     for (qint32 j=0; j<spis_fr.count(); j++)
     {
      sp_f=FileInfo(spis_fr[j]);
      str=sp_f[2]; str="20"+str.mid(8,8);
      dn=QDate::fromString(str,"yyyy.MM.dd");
      if (dn == dat) {spis_fo << spis_fr[j];continue;}
      else DelFile(spis_fr[j]);
     }
     prd.setValue(i);
     qApp->processEvents();
    }
   }

   if (spis_fo.isEmpty()) return false;

   if (kod == 1)
   {
    for (qint32 i=0; i<spis_fo.count(); i++)
    {
     sod=ReadFileInList(spis_fo[i]);
     if (sod.isEmpty()) continue;
     if (!ObDDS(map,sod,ppsi_file,nka)) continue;
    }
    for (qint32 i=0; i<spis_fo.count(); i++)
    {
     DelFile(spis_fo[i]);
    }
    map1=DelDublInMap(map);
    sod.clear();
    sod=FormOFileFromMap(map1);
    if (sod.isEmpty()) return false;
    hdr=FormHdrDDS(ppsi_file,sod,nka);
    if (hdr.isEmpty()) return false;
    AddList(hdr,sod);
    nfile=NazOFile(nka+"G",hdr,zapslu.path_temp+"/");
    if (!WriteFileFromList(hdr,zapslu.path_temp+"/"+nfile)) return false;
    naz_file=zapslu.path_temp+"/"+nfile;
    if (!ProvRnxInterval(hdr,err)) kodobr=1;
    return true;
   }

   if (kod == 2)
   {
    hdr.clear(); map.clear();
    for (qint32 i=0; i<spis_fo.count(); i++)
    {
     sod=ReadFileInList(spis_fo[i]);
     if (sod.isEmpty()) continue;
     AddList(hdr,sod);
    }
    for (qint32 i=0; i<spis_fo.count(); i++)
    {
     DelFile(spis_fo[i]);
    }

    mapk=KFileInMap(hdr);
    sod=FormKFileFromMap(mapk,nep);
    nfile=NazKFile(nka,dat,zapslu.path_temp);
    nfile=zapslu.path_temp+"/"+nfile;
    if (!WriteFileFromList(sod,nfile)) return false;
    naz_file=nfile;
    return true;
   }

   if (kod == 3)
   {
    map.clear();obuf.clear();
    if (!ObAltiInMapF(spis_fo,map,okolpak,i_file,obuf,dlpak,prsoob))
    {
     ClearDir(zapust.path_temp);
     DeleteRow("obr","naz_komp",komp_name,prsoob);
     prd.close();
     return false;
    }
    AltiMapInArray(map,obuf,buf,dlpak,kolpak);
    map.clear(); obuf.clear();

    if (!InfFromAlti(buf,dtn,dtk,kolpak,kolsut))
    {
     ClearDir(zapust.path_temp);
     DeleteRow("obr","naz_komp",komp_name,prsoob);
     prd.close();
     return false;
    }

     nfile=WriteFileAlti(buf,nka,dtn.date(),dtn.time(),kolpak,i_file,zapust.path_temp);
     buf.clear();
     if (nfile == "")
     {
      ClearDir(zapust.path_temp);
      DeleteRow("obr","naz_komp",komp_name,prsoob);
      prd.close();
      return false;
     }
     naz_file=nfile;
     for (qint32 i=0; i<spis_fo.count(); i++)
     {
      DelFile(spis_fo[i]);
     }
     return true;
    }

 return false;
}

bool ObrIzm::RemoveDdsInBasket(quint64 nz_dds, bool prsoob)
{
 QString nfile;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf("dds",nz_dds,buf,prsoob))
 {
  DeleteRow("dds",nz_dds,prsoob);
  UpdateRows("sxema","nz_educho2",1,"nz_educh",nz_dds,prsoob,0);
  UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz_dds,prsoob,0);
  UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz_dds,prsoob,1);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow("dds",nz_dds,prsoob)) return false;

 UpdateRows("sxema","nz_educho2",1,"nz_educh",nz_dds,prsoob,0);
 UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz_dds,prsoob,0);
 UpdateRows("sxema","nz_educho2",2,"nz_educho1",nz_dds,prsoob,1);

 return true;
}

bool ObrIzm::RemoveDsInBasket(quint64 nz_ds, bool prsoob)
{
 QString nfile;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf("ds",nz_ds,buf,prsoob))
 {
  ReadDs(nz_ds,prsoob);
  if (zapds.nz_edxr == 0) DeleteRow("ds",nz_ds,prsoob);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow("ds",nz_ds,prsoob)) return false;

 return true;
}

bool ObrIzm::RemoveRnxInBasket(QString naz_table,quint64 nz_rnxi, bool prsoob)
{
 QString nfile;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf(naz_table,nz_rnxi,buf,prsoob))
 {
  ReadRnx(naz_table,nz_rnxi,prsoob);
  if (zaprnx.nz_edxr == 0) DeleteRow(naz_table,nz_rnxi,prsoob);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow(naz_table,nz_rnxi,prsoob)) return false;

 return true;
}

bool ObrIzm::ObrKod22(qint32 kolobr, QString path_temp,bool prsoob)
{
 qlonglong nz,nz2;
 QList<qlonglong> list;
 qlonglong kolzap;
 QString nfile,vfile;
 QByteArray buf;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;

 ClearSlu();

 list=GetKodobr("rnxi",22,prsoob);
 kolzap=list.count();
 if (kolzap == 0) return true;

 if ((kolzap > kolobr) && (kolobr > 0)) kolzap=kolobr;

 for (int i=0; i < kolzap; i++)
 {
  if (pr_stop) break;
  nz=list.at(i); nz2=nz;

  if (! ReadRnx("rnxi",nz,prsoob)) continue;
  if (!InsertObr(komp_name,zaprnx.nz_edxr)) continue;
  if (!ReadEdxr("rnxi",zaprnx.nz,prsoob)) continue;

  nfile=readedxr.path_arx+"/"+readedxr.arxf;
  if (readedxr.prarx == 1) readedxr.prarx=2;
  if (!ArxFile(nfile,readedxr.prarx,idlf,iks,adlf,aks,buf)) continue;
  if (readedxr.aks != aks) continue;
  nfile=path_temp+"/"+readedxr.isxf;
  if (WriteFile(nfile,buf,true) == -1)continue;
  buf.clear();


  RnxToRnx(nfile);

  vfile=nfile.mid(0,nfile.length()-5)+"0"+nfile.right(4);
  QFile::rename(nfile,vfile);

  ClearSxema();

  nz=SetRnxFileNG(vfile,zaprnx.tip,false,prsoob);
  if (nz == 0) return false;

  ClearSxema();
  zapsxema.nz_educh=nz2;
  zapsxema.nz_educho1=nz;
  zapsxema.nz_educho2=0;
  zapsxema.pr_isx=0;
  InsertSxema("sxema1",prsoob);

  UpdateRow("rnxi","kodobr",5,nz2,prsoob);

  qApp->processEvents();
 }

 DeleteRow("obr","naz_komp",komp_name,prsoob);

 return true;
}

QStringList ObrIzm::SpisEpochForSatDDS(QStringList list)
{
 QMap<QString,QString> map;
 QStringList list_sat;
 QMap<QString,QString>::const_iterator i;
 QString val,key;
 QString str;
 QString kolc;

 map=ReadOFileInMap(list);
 if (map.isEmpty()) return list_sat;

 for (i=map.constBegin(); i!=map.constEnd();++i)
 {
  val=i.value(); val=val.trimmed();
  kolc="1";
  if (val.length() > 50) kolc="2";
  key=i.key();
  str=val.left(3)+";"+key.mid(2,27)+";"+kolc;
  list_sat << str;
 }
 qSort(list_sat);

 return list_sat;
}

QStringList ObrIzm::SpisEpochForSatDDS(qlonglong nz_dds,QStringList &list_mix)
{
 QStringList list,list_sat;

 list_mix.clear();

 if (!ReadEdxrInList("dds",nz_dds,list,true)) return list_sat;

 list_sat=SpisEpochForSatDDS(list);
 list_mix=StatMixForGlonass(list);

 return list_sat;
}

bool ObrIzm::Compress(QString _str)
{
 return compressor->compress(_str.toStdString());
}

bool ObrIzm::UnCompress(QString _str)
{
 return compressor->unCompress(_str.toStdString());
}

bool ObrIzm::RnxToRnx(QString nfile)
{
 QString ver;
 QString in_o,in_n,in_g;
 QString out_o,out_n,out_g;
 bool pr_konv=false;
 QFile f;
 QStringList fi;

 ver=RnxVersion(nfile);

 in_o.clear(); in_n.clear(); in_g.clear();
 out_o.clear(); out_n.clear(); out_g.clear();

 if (nfile.right(1).toUpper() == "O")
 {
  in_o=nfile;
//  if (!RenameFile(in_o,"K_*.*",out_o)) return false; //старый блок ========
  pr_konv=true;
 }

 if (nfile.right(1).toUpper() == "N")
 {
  in_n=nfile;
//  if (!RenameFile(in_n,"K_*.*",out_n)) return false; //старый блок ========
  pr_konv=true;
 }

 if (nfile.right(1).toUpper() == "G")
 {
  in_g=nfile;
 // if (!RenameFile(in_g,"K_*.*",out_g)) return false; //старый блок ========
  pr_konv=true;
 }

 if (!pr_konv) return false;




/*
//старый блок =========================================================
 QFile *inputObs = new QFile(in_o);
 QFile *inputNav = new QFile(in_n);
 QFile *inputNavG = new QFile(in_g);
 QFile *outputObs = new QFile(out_o);
 QFile *outputNav = new QFile(out_n);
 QFile *outputNavG = new QFile(out_g);

 qDebug() << "============================================================================================";

 convJps->getDopplerStatus(true);
 convJps->getUseTextInfFromFileStatus(true);
 //данное значение может быть или пропуском, или 0.000
// convJps->setSpaceDefault("0.000  ");
 convJps->setSpaceDefault("       ");

 convJps->getInf("CGTR 2.4.0", "RKS", "001", "002", "SN0001", "RKS T1", "T1.001", "SN0001", "RKS A1");
 //converterJps::convert211To302Full (inputObs, inputNav, inputNavG, outputObs, outputNav, outputNavG);
 //Функция принудительной установки вида записи отсутствия измерений. По спецификации RINEX
 if (ver.left(1) == "3") convJps->convert302To211Full (inputObs, inputNav, inputNavG, outputObs, outputNav, outputNavG);
 if (ver.left(1) == "2") convJps->convert211To302Full (inputObs, inputNav, inputNavG, outputObs, outputNav, outputNavG);
// if (ver.left(1) == "2") convJps->convert211To302Full_N (in_o,in_n,in_g,out_o,out_n,out_g);
// QMessageBox::critical(0,QObject::tr("Ошибка"),"продолжить",QMessageBox::Yes);
//return false;
//==============================================================================================
*/



//новый блок =========================================================
 RenexConverter *rnxConv=new RenexConverter();

 if (!in_o.isEmpty())
 {
  fi=FileInfo(in_o);
  rnxConv->setFilePath(fi[1]);
  if (ver.left(1) == "2") rnxConv->convert_rinex_O_to_302(in_o);
  if (ver.left(1) == "3") rnxConv->convert_rinex_O_to_211(in_o);
 }

 if (!in_n.isEmpty())
 {
  fi=FileInfo(in_n);
  rnxConv->setFilePath(fi[1]);
  if (ver.left(1) == "2") rnxConv->convert_rinex_N_to_302(in_n);
  if (ver.left(1) == "3") rnxConv->convert_rinex_N_to_211(in_n);
 }

 if (!in_g.isEmpty())
 {
  fi=FileInfo(in_g);
  rnxConv->setFilePath(fi[1]);
  if (ver.left(1) == "2") rnxConv->convert_rinex_G_to_302(in_g);
  if (ver.left(1) == "3") rnxConv->convert_rinex_G_to_211(in_g);
 }
//========================================================================



/*
//старый блок =========================================================
 if (!in_o.isEmpty())
 {
  f.setFileName(out_o);
  if (!f.exists()) return false;
  DelFile(in_o);
  if (!QFile::rename(out_o,in_o)) {DelFile(out_o); return false;}
 }

 if (!in_n.isEmpty())
 {
  f.setFileName(out_n);
  if (!f.exists()) return false;
  DelFile(in_n);
  if (!QFile::rename(out_n,in_n)) {DelFile(out_n); return false;}
 }

 if (!in_g.isEmpty())
 {
  f.setFileName(out_g);
  if (!f.exists()) return false;
  DelFile(in_g);
  if (!QFile::rename(out_g,in_g)) {DelFile(out_g); return false;}
 }
//=========================================================
*/

 return true;
}

bool ObrIzm::SetPla(QString nfile,bool prsoob)
{

     QStringList list;
     qlonglong kolstr;
     QString str;
     QStringList f;
     quint64 nz_pla;

     ClearPla();

     f=FileInfo(nfile);
     zappla.naz = f[3];

     str="DELETE FROM pla WHERE naz = '" + zappla.naz + "'";
     db.exec(str);

     list=ReadFileInList(nfile);
     kolstr=list.count();
     if (kolstr == 0) return false;

     for (int i=0; i<kolstr; i++)
     {
      str=list[i];
      str=str.simplified();
      zappla.nka=str.section(" ",0,0);
      zappla.npn=str.section(" ",1,1);
      zappla.dat=QDate::fromString(str.section(" ",2,2),"yyyy-MM-dd");
      zappla.vrem=QTime::fromString(str.section(" ",3,3),"hh:mm:ss");
      zappla.vrs=str.section(" ",4,4).toInt();
      zappla.ds=str.section(" ",5,5).left(1).toInt();
      zappla.dzs=str.section(" ",6,6).left(1).toInt();
      zappla.kos=str.section(" ",7,7).toInt();
      zappla.ppsi=str.section(" ",8,8).toInt();
      zappla.vit=str.section(" ",9,9).toInt();
      zappla.az=str.section(" ",10,10).toDouble();
      zappla.um=str.section(" ",11,11).toDouble();

      nz_pla=InsertPla(prsoob);
      if (nz_pla == 0)
      {
       str="DELETE FROM pla WHERE naz = '" + zappla.naz + "'";
       db.exec(str);
       return false;
      }
     }
     return true;

}

bool ObrIzm::SetCogi(QString nfile)
{
 QStringList lf;
 QString str;

 ClearCogi();

 lf=FileInfo(nfile);
 str=lf[3];

 zapcogi.naz=str;
 zapcogi.tip=str.section("-",0,0).mid(2,3).toInt();
 zapcogi.dat=QDate::fromString(str.section("-",2,2),"ddMMyyyy");
 zapcogi.vrem=QTime::fromString(str.section("-",3,3),"hhmm");

 return true;
}

bool ObrIzm::InfDatForIzmInf(QString nfile,qint32 &year,qint32 &sut,QDate &date)
{
 QStringList lf;
 QString ext;
 qint32 g;
 QString nazf,str;
 QByteArray buf;
 QDate dat;
 QDateTime dtn,dtk;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[3];
 ext=ext.toUpper();
 nazf=nazf.toUpper();
 year=0; sut=0; date=DayToDate(1,1900);

 if (ext == "313")
 {
  str=nazf;
  year=str.section("_",1,1).toInt()+2000;
  sut=str.section("_",2,2).toInt();
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "322")
 {
  str=nazf;
  year=str.section("_",1,1).toInt()+2000;
  sut=str.section("_",2,2).toInt();
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "SPO")
 {
  str=nazf;
  year=str.section("_",1,1).toInt()+2000;
  sut=str.section("_",2,2).toInt();
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "RNX")
 {
  str=nazf;
  year=str.section("_",2,2).mid(0,4).toInt();
  sut=str.section("_",2,2).mid(4,3).toInt();
  date=DayToDate(sut,year);
  return true;
  }

 if ((ext.right(3) == "BIN") || (ext.right(3) == "126") || (ext.right(3) == "127"))
 {
  if (!ReadFile(nfile,buf,200)) return false;
  PrefFull(buf,dtn,dtk);
  DateToDay(dtn.date(),sut,year);
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "KOS") //сделано в нов.табл
 {
  str=nazf;
  dat=QDate::fromString(str.section("_",1,1),"yyyyMMdd");
  DateToDay(dat,sut,year);
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "FRD") //сделано в нов.табл
 {
  SetFrd(nfile);
  sut=zapds.sutn; year=zapds.godn;
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "SP3") //сделано в нов.табл
 {
  SetSP3(nfile);
  sut=zapsp3.sutn; year=zapsp3.godn;
  date=DayToDate(sut,year);
  return true;
 }

 if ((ext == "PPSI") || (ext == "ALT") || (ext == "PLA"))//сделано в нов.табл
 {
  str=nazf;
  dat=QDate::fromString(str.mid(4,10),"yyyy_MM_dd");
  DateToDay(dat,sut,year);
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "KOS") //сделано в нов.табл
 {
  str=nazf;
  dat=QDate::fromString(str.section("_",1,1),"yyyyMMdd");
  DateToDay(dat,sut,year);
  date=DayToDate(sut,year);
  return true;
 }

 if (ext == "SOL") //сделано в нов.табл
 {
  SetSol(nfile);
  sut=zapsol.sutk; year=zapsol.godk;
  date=DayToDate(sut,year);
  return true;
 }

 if (ext.right(1) == "M")
 {
  str=nazf;
  sut=str.mid(4,3).toInt();
  year=ext.left(2).toInt()+2000;
  date=DayToDate(sut,year);
  return true;
 }

 if (ext.right(1) == "O")
 {
  str=nazf;
  sut=str.mid(4,3).toInt();
  year=ext.left(2).toInt()+2000;
  date=DayToDate(sut,year);
  return true;
 }

 if ((ext.right(1) == "G") || (ext.right(1) == "N"))
 {
  str=nazf;
  sut=str.mid(4,3).toInt();
  year=ext.left(2).toInt()+2000;
  date=DayToDate(sut,year);
  return true;
 }

 if (ext.right(1) == "L")
 {
  str=nazf;
  sut=str.mid(4,3).toInt();
  year=ext.left(2).toInt()+2000;
  date=DayToDate(sut,year);
  return true;
 }

 return false;
}

bool ObrIzm::SetCup(QString nfile)
{
 QStringList lf;
 QString str;

 ClearCup();

 lf=FileInfo(nfile);
 str=lf[3];

 zapcup.naz=str;
 zapcup.tip=str.section("-",0,0).mid(2,3).toInt();
 zapcup.dat=QDate::fromString(str.section("-",2,2),"ddMMyyyy");
 zapcup.vrem=QTime::fromString(str.section("-",3,3),"hhmm");

 return true;
}

bool ObrIzm::ZapNfCogi(QString nfile,qulonglong &nz,bool prud,bool prsoob,bool prpovt)
{
 QStringList lf;
 qulonglong nz1;
 QString ext;
 qint32 g;
 QString nazf;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[3];
 ext=ext.toUpper();
 nazf=nazf.toUpper();

  if (ext == "COGI")
 {
  if (! SetCogi(nfile)) return false;
  nz=FindRow("cogi","naz",zapcogi.naz,prsoob);
  if ((!prpovt) && (nz > 0))
  {
   if (prud) DelFile(nfile);
   return true;
  }

  nz=InsertCogi(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("NFIN",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("cogi",nz,prsoob);
   return false;
  }

 if (!UpdateRow("cogi","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("cogi",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 return false;
}

void ObrIzm::ObrNf(QString path_si,QString path_temp,bool prsoob,bool prpovt)
{
 QStringList spis_f;
 QStringList info;
 qulonglong dlf;
 qint32 kolzap;
 QFile f;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks1,aks2;
 QByteArray buf;

 //получить список файлов в директории
 spis_f=SpisAllFile(path_si);
 kolzap=spis_f.count();
 if (kolzap == 0) return;

 //цикл по списку файлов
 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  info=FileInfo(spis_f[i]);

  f.setFileName(path_temp+"/"+info[2]);
  if (f.exists()) f.remove();

  if (!QFile::copy(spis_f[i],path_temp+"/"+info[2])) continue;

  ArxFile(spis_f[i],0,idlf,iks,adlf,aks1,buf);
  ArxFile(path_temp+"/"+info[2],0,idlf,iks,adlf,aks2,buf);
  if (aks1 != aks2)
  {
   DelFile(path_temp+"/"+info[2]);
   continue;
  }

  if (ZapNfCogi(path_temp+"/"+info[2],dlf,false,prsoob,prpovt))
  {
   DelFile(path_temp+"/"+info[2]);
   DelFile(spis_f[i]);
   continue;
  }

  if (ZapNfCup(path_temp+"/"+info[2],dlf,false,prsoob,false))
  {
   DelFile(path_temp+"/"+info[2]);
   DelFile(spis_f[i]);
  }

  qApp->processEvents();
 }

 return;
}

bool ObrIzm::ZapNfCup(QString nfile,qulonglong &nz,bool prud,bool prsoob,bool pr406)
{
 QStringList lf;
 qulonglong nz1;
 QString ext;
 qint32 g;
 QString nazf;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[3];
 ext=ext.toUpper();
 nazf=nazf.toUpper();
 nz=0;

 if (ext == "CUP")
 {
  if (! SetCup(nfile)) return false;
  if ((zapcup.tip == 406) && (!pr406)) return false;
  zapcup.sost=1;
  nz=InsertCup(prsoob);
  if (nz == 0) return false;
  nz1=ZapEdxr("NFOUT",nz,nfile,0,true);
  if (nz1 == 0)
  {
   DelFile(tek_path_arx+"/"+zapedxr.arxf);
   DeleteRow("cup",nz,prsoob);
   return false;
  }

 if (!UpdateRow("cup","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("cup",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return false;
 }

  if (prud) DelFile(nfile);
  return true;
 }

 return false;
}

void ObrIzm::OtprNf(QString path_vix,bool prsoob)
{
 QList<qlonglong> list;
 QString nfile;

 list=FindRowForOtprNf(prsoob);
 if (list.isEmpty()) return;

 for (qint32 i=0; i<list.count(); i++)
 {
  ReadCup(list.at(i),prsoob);
  if (!CopyEdxr("cup",zapcup.nz,path_vix,prsoob,nfile)) continue;
  UpdateRow("cup","sost",2,zapcup.nz,prsoob);
 }

}

bool ObrIzm::SetSP3(QString naz_file)
{
 qlonglong kolstr;
 QDateTime dtn,dtk;
 QStringList list,lf,list_ep;
 QString nka;

 list=ReadFileInList(naz_file);
 if (list.isEmpty()) return false;

 ClearSP3();

 kolstr=list.count();
 if (kolstr == 0) return false;

 lf=FileInfo(naz_file);

 zapsp3.naz=lf[3];
 zapsp3.naz=zapsp3.naz.toUpper();

 if (!DtSP3(list,dtn,dtk,nka)) return false;
 zapsp3.sutn=dtn.date().dayOfYear();
 zapsp3.godn=dtn.date().year();
 zapsp3.sekn=TimeToSec(dtn.time());
 zapsp3.dtn=dtn.toString("dd-MM-yyyy hh:mm:ss");

 zapsp3.sutk=dtk.date().dayOfYear();
 zapsp3.godk=dtk.date().year();
 zapsp3.sekk=TimeToSec(dtk.time());
 zapsp3.dtk=dtk.toString("dd-MM-yyyy hh:mm:ss");

 zapsp3.nka=nka;

 zapsp3.nz_edxr=0;

 list_ep=SP3InList(list);
 zapsp3.kolep=list_ep.count();

 return true;
}

bool ObrIzm::DtSP3(QStringList &list,QDateTime &dtn,QDateTime &dtk,QString &nka)
{
 int nstr1,nstr2;
 QString sdat;

 nstr1=FindStrInList(list,"+",0,1);
 if (nstr1 == -1) return false;
 sdat=list.at(nstr1);
 nka.clear();
 if (sdat.mid(9,3) == "L01") nka="126";
 if (sdat.mid(9,3) == "L02") nka="127";

 nstr1=FindStrInList(list,"*  ",0,1);
 if (nstr1 == -1) return false;
 nstr2=FindStrInList(list,"*  ",0,3);
 if (nstr2 == -1) return false;

 sdat=list.at(nstr1);
 sdat=sdat.mid(3);
 dtn=SetDTFromStr(sdat);

 sdat=list.at(nstr2);
 sdat=sdat.mid(3);
 dtk=SetDTFromStr(sdat);

 return true;
}

QStringList ObrIzm::SP3InList(QString naz_file)
{
 QStringList list,list_ep;

 list=ReadFileInList(naz_file);
 if (list.isEmpty()) return list_ep;

 list_ep=SP3InList(list);

 return list_ep;
}

QStringList ObrIzm::SP3InList(QStringList &list)
{
 QStringList list_ep;
 int nstr;
 QString str;

 if (list.isEmpty()) return list_ep;

 nstr=FindStrInList(list,"*  ",0,1);
 if (nstr == -1) return list_ep;
 //nstr=nstr+2;

 while (nstr > 0)
 {
  str=list.at(nstr);
  str=str.mid(3);
  list_ep << str;
  nstr=FindStrInList(list,"*  ",nstr+1,2);
 }

 return list_ep;
}

bool ObrIzm::RemoveSp3InBasket(quint64 nz_sp3, bool prsoob)
{
 QString nfile;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf("sp3",nz_sp3,buf,prsoob))
 {
  ReadSP3(nz_sp3,prsoob);
  if (zapsp3.nz_edxr == 0) DeleteRow("sp3",nz_sp3,prsoob);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow("sp3",nz_sp3,prsoob)) return false;

 return true;
}

bool ObrIzm::EdvidForSp3(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidSp3(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadSP3(list[i],prsoob)) return false;
   dt=DayToDate(zapsp3.sutn,zapsp3.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zapsp3.nz_edxr;
   zapedvid.naz_edxr=zapsp3.naz;
   zapedvid.tip="SP3";
   InsertEdvid(prsoob);
  }
 }

 return true;
}

QStringList ObrIzm::SP3InList(quint64 nz_sp3,bool prsoob)
{
 QStringList list,list_ep;

 if (!ReadEdxrInList("sp3",nz_sp3,list,prsoob)) return list_ep;

 list_ep=SP3InList(list);

 return list_ep;
}

bool ObrIzm::ModelInList(QSqlQueryModel *model,int rcol,QStringList &list,QString delemiter,bool pr_fname)
{
 int kolrow,kolcol;
 QSqlField field;
 QVariant var;
 QString str,str1;
 qint32 len;

 kolrow=model->rowCount();
 kolcol=model->columnCount();
 qDebug() << kolrow << kolcol;

 if (kolrow == 0) return false;

 str="";str1="";

 if (pr_fname)
 {
  for (int i=0; i<kolcol; i++)
  {
   str1=model->record(0).fieldName(i);
   len=rcol;
   if (len == 0) len=str1.length();
   if (i == kolcol-1) str=str+QString("%1").arg(str1,rcol);
   else str=str+QString("%1"+delemiter).arg(str1,rcol);
  }
  list << str;
 }

 for (int i=0; i<kolrow; i++)
 {
  str.clear();
  for (int j=0; j<kolcol; j++)
  {
   field=model->record(i).field(j);
   var=field.value();
   qDebug() << j << var;
   str1=var.toString();
   len=rcol;
   if (len == 0) len=str1.length();
   str1=str1.mid(0,len);

   if (!VariantIsDigit(var)) str1="'"+str1+"'";

   len=rcol;
   if (len == 0) len=str1.length();
   if (j == kolcol-1) str=str+QString("%1").arg(str1,rcol);
   else str=str+QString("%1"+delemiter).arg(str1,rcol);
   qDebug() << str << j;
  }
  list << str;
 }

 return true;
}

bool ObrIzm::SetSol(QString naz_file)
{
 QDateTime dtn,dtk;
 QStringList list,lf;
 QString nka;
 qint32 kolizm,kolk,kolo;
 qreal sko;

QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));

 list=ReadFileInList(naz_file);
 if (list.isEmpty())
 {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  return false;
 }

 ClearSol();

 lf=FileInfo(naz_file);

 zapsol.naz=lf[3];
 zapsol.naz=zapsol.naz.toUpper();

 if (!DtSol(list,dtn,dtk,nka,kolizm,kolk,kolo,sko))
 {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  return false;
 }

 QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

 zapsol.sutn=dtn.date().dayOfYear();
 zapsol.godn=dtn.date().year();
 zapsol.sekn=TimeToSec(dtn.time());
 zapsol.dtn=dtn.toString("dd-MM-yyyy hh:mm:ss");

 zapsol.sutk=dtk.date().dayOfYear();
 zapsol.godk=dtk.date().year();
 zapsol.sekk=TimeToSec(dtk.time());
 zapsol.dtk=dtk.toString("dd-MM-yyyy hh:mm:ss");

 zapsol.nka=nka;
 zapsol.kolizm=kolizm;
 zapsol.kolk=kolk;
 zapsol.kolo=kolo;
 zapsol.sko=sko;

 zaprnx.nz_edxr=0;

 return true;
}

bool ObrIzm::DtSol(QStringList &list,QDateTime &dtn,QDateTime &dtk,QString &nka,qint32 &kolizm,qint32 &kolk,qint32 &kolo,qreal &sko)
{
 qint32 nstr;
 qint32 kolstr;
 QString str;
 qreal rsec;
 QDateTime dt;
 QDate d;
 QTime t;
 qint64 sec;

 d.setDate(1980,1,6);
 t.setHMS(0,0,0,0);
 dt.setDate(d); dt.setTime(t);

 kolstr=list.count();

 nstr=FindStrInList(list,"№");
 if (nstr == -1) return false;

 if (nstr+1 >= kolstr) return false;

 str=list.at(nstr+1);
 str=str.simplified();
 rsec=str.section(" ",1,1).toDouble();
 sec=rsec;
 dtn=dt.addSecs(sec);

 str=list.at(kolstr-3);
 str=str.simplified();
 rsec=str.section(" ",1,1).toDouble();
 sec=rsec;
 dtk=dt.addSecs(sec);

 nka.clear();
 nstr=FindStrInList(list,"Имя файла:");
 if (nstr > -1)
 {
  str=list.at(nstr);
  str=str.simplified();
  nka=str.section(" ",2,2).mid(0,3);
 }

 kolizm=0;
 nstr=FindStrInList(list,"Общее количество");
 if (nstr > -1)
 {
  str=list.at(nstr);
  str=str.simplified();
  kolizm=str.section(" ",5,5).toInt();
 }

 kolk=0;
 nstr=FindStrInList(list,"Количество качественных");
 if (nstr > -1)
 {
  str=list.at(nstr);
  str=str.simplified();
  kolk=str.section(" ",3,3).toInt();
 }

 kolo=0;
 nstr=FindStrInList(list,"Количество отбракованных");
 if (nstr > -1)
 {
  str=list.at(nstr);
  str=str.simplified();
  kolo=str.section(" ",3,3).toInt();
 }

 sko=0;
 nstr=FindStrInList(list,"СКО");
 if (nstr > -1)
 {
  str=list.at(nstr);
  str=str.simplified();
  sko=str.section(" ",6,6).toDouble();
 }

 return true;
}

bool ObrIzm::RemoveSolInBasket(quint64 nz_sp3, bool prsoob)
{
 QString nfile;
 QByteArray buf;
 QDir dir;

 if (!ReadEdxrInBuf("sol",nz_sp3,buf,prsoob))
 {
  ReadSol(nz_sp3,prsoob);
  if (zapsp3.nz_edxr == 0) DeleteRow("sol",nz_sp3,prsoob);
  return false;
 }

 nfile=readedxr.path_arx+"/basket";

 dir.setPath(nfile);
 if (!dir.exists())
     if (!dir.mkdir(nfile)) return false;

 nfile=nfile+"/"+QString::number(readedxr.nz_edxr)+"_"+readedxr.isxf;

 if (WriteFile(nfile,buf,false) == -1) return false;

 if (!RemoveEdxr(readedxr.nz_edxr,prsoob)) return false;

 if (!DeleteRow("sol",nz_sp3,prsoob)) return false;

 return true;
}

bool ObrIzm::EdvidForSol(quint64 nz_zapr,bool prsoob)
{
 QList<qlonglong> list;
 QDate dt;

 list=FindRowForEdvidSol(nz_zapr,prsoob);
 if (! list.isEmpty())
 {
  for(int i=0; i<list.count(); i++)
  {
   if (!ReadSol(list[i],prsoob)) return false;
   dt=DayToDate(zapsol.sutn,zapsol.godn);
   if (dt.isValid())
   {
    if (dt < zapzapr.dtn) continue;
    if (dt > zapzapr.dtk) continue;
   }
   zapedvid.nz_zapr=nz_zapr;
   zapedvid.nz_edxr=zapsol.nz_edxr;
   zapedvid.naz_edxr=zapsol.naz;
   zapedvid.tip="SOL";
   InsertEdvid(prsoob);
  }
 }

 return true;
}

void ObrIzm::KadrKviInList(QStringList &list,qint32 kod_otbr)
{
 QString ka;
 QString zag_izm,izm;
 QDate dat;
 QTime time;
 QDateTime dt;
 QString str;
 qint32 kss;

 if ((kod_otbr == 1) || (kod_otbr == 2)) return;

 dat.setDate(kadrkvi.year,kadrkvi.month,kadrkvi.day);
 time=SecToTime(QString::number(kadrkvi.time));
 dt.setDate(dat);
 dt.setTime(time);
 if (kadrkvi.nka > 0 && kadrkvi.nka < 33) ka="G"+QString::number(kadrkvi.nka).rightJustified(2,'0');
 if (kadrkvi.nka > 32 && kadrkvi.nka < 65) ka="R"+QString::number(kadrkvi.nka-32).rightJustified(2,'0');

// if (dt.toString("dd-MM-yyyy hh:mm:ss") == "05-11-2019 22:43:31")
//     zag_izm="";

 zag_izm="> "+DtToStrHdrRnx(dt,2)+" "+ka;
 list << zag_izm;

 str="  Контрольная сумма: "+QString::number(kadrkvi.crc)+" : "+QString::number(kadrkvi.crc,16).toUpper();
 list << str;
 str="  Идентификатор: "+QString::number(kadrkvi.ident)+" : "+QString::number(kadrkvi.ident,16).toUpper();
 list << str;
 str="  Год: "+QString::number(kadrkvi.year)+" : "+QString::number(kadrkvi.year,16).toUpper();
 list << str;
 str="  Месяц: "+QString::number(kadrkvi.month)+" : "+QString::number(kadrkvi.month,16).toUpper();
 list << str;
 str="  День: "+QString::number(kadrkvi.day)+" : "+QString::number(kadrkvi.day,16).toUpper();
 list << str;
 str="  Время с: "+QString::number(kadrkvi.time)+" : "+QString::number(kadrkvi.time,16).toUpper();
 list << str;
 str="  Модифицированный № КА: "+QString::number(kadrkvi.nka)+" : "+QString::number(kadrkvi.nka,16).toUpper();
 list << str;
 str="  № суток для ГЛОНАСС, № недели для GPS: "+QString::number(kadrkvi.nsutki)+" : "+QString::number(kadrkvi.nsutki,16).toUpper();
 list << str;
 str="  Время суток для ГЛОНАСС, время недели для GPS: "+QString::number(kadrkvi.tsutki)+" : "+QString::number(kadrkvi.tsutki,16).toUpper();
 list << str;
 str="  Статус: "+QString::number(kadrkvi.status)+" : "+QString::number(kadrkvi.status,16).toUpper();
 list << str;
 str="  Отношение сигнал/шум C/A: "+QString::number(kadrkvi.ca)+" : "+QString::number(kadrkvi.ca,16).toUpper();
 list << str;
 str="  Отношение сигнал/шум L2: "+QString::number(kadrkvi.l2)+" : "+QString::number(kadrkvi.l2,16).toUpper();
 list << str;
 bt8.byte_double=kadrkvi.pdca;
 str="  Псевдодальность C/A: "+QString::number(kadrkvi.pdca,'f',10)+" : "+QString::number(bt8.byte8,16).toUpper();
 list << str;
 bt8.byte_double=kadrkvi.pdl2;
 str="  Псевдодальность L2: "+QString::number(kadrkvi.pdl2,'f',10)+" : "+QString::number(bt8.byte8,16).toUpper();
 list << str;
 bt8.byte_double=kadrkvi.fl1;
 str="  Фаза C/A: "+QString::number(kadrkvi.fl1,'f',10)+" : "+QString::number(bt8.byte8,16).toUpper();
 list << str;
 bt8.byte_double=kadrkvi.fl2;
 str="  Фаза L2: "+QString::number(kadrkvi.fl2,'f',10)+" : "+QString::number(bt8.byte8,16).toUpper();
 list << str;
 str="  Доплер C/A: "+QString::number(kadrkvi.dl1)+" : "+QString::number(kadrkvi.dl1,16).toUpper();
 list << str;
 str="  Доплер L2: "+QString::number(kadrkvi.dl2)+" : "+QString::number(kadrkvi.dl2,16).toUpper();
 list << str;
 str="  № литеры ГЛОНАСС: "+QString::number(kadrkvi.nl)+" : "+QString::number(kadrkvi.nl,16).toUpper();
 list << str;
 str="  Признак получения времени: "+QString::number(kadrkvi.prtime)+" : "+QString::number(kadrkvi.prtime,16).toUpper();
 list << str;
 str="  Признак получения измерений: "+QString::number(kadrkvi.prizmer)+" : "+QString::number(kadrkvi.prizmer,16).toUpper();
 list << str;
 str="  Резерв: "+QString::number(kadrkvi.reserv)+" : "+QString::number(kadrkvi.reserv,16).toUpper();
 list << str;
 list << "  "+list_err[kod_otbr];
 list << "------------------------------------------------------------------------------";

 return;
}

void ObrIzm::ObrSi(QString path_si,QString path_temp,qint32 kolobr,bool prsoob)
{
 QStringList spis_f;
 QStringList info;
 qulonglong dlf;
 qint32 kolzap,kolzap1;
 QFile f;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks1,aks2;
 QByteArray buf;
 QString str;
 QDir dir;
 QStringList filtr;

 ClearSlu();

 //получить список файлов в директории
 dir.setPath(path_si);
 filtr << "*.*";
 spis_f=dir.entryList(filtr,QDir::Files);
 kolzap1=spis_f.count();
 if (kolzap1 == 0) return;

 if ((kolzap1 > kolobr) && (kolobr > 0)) kolzap=kolobr;
 else kolzap=kolzap1;

 //цикл по списку файлов
 for (int i=0; i<kolzap; i++)
 {
  if (pr_stop) break;
  info=FileInfo(path_si+"/"+spis_f[i]);
  str=info[5];
  idlf=str.toLongLong();
  if (idlf == 0) continue;

  f.setFileName(path_temp+"/"+info[2]);
  if (f.exists()) DelFileM(path_temp+"/"+info[2]);

  WriteLog("Помещение файла: "+path_si+"/"+spis_f[i]);

  ArxFile(path_si+"/"+spis_f[i],0,idlf,iks,adlf,aks1,buf);

  if (WriteFile(path_temp+"/"+info[2],buf,true) < 0) continue;

  ArxFile(path_temp+"/"+info[2],0,idlf,iks,adlf,aks2,buf);
  if (aks1 != aks2)
  {
   DelFileM(path_temp+"/"+info[2]);
   continue;
  }
  else
  {
   ClearSist();
   zapsist.nz_kodobr=23;
   zapsist.naz_ed=info[2];
   if (ZapEduch(path_temp+"/"+info[2],dlf,false,prsoob))
   {
    DelFileM(path_temp+"/"+info[2]);
    DelFileM(path_si+"/"+spis_f[i]);
    zapsist.sost="выполнено";
    WriteLog("Файл "+path_si+"/"+spis_f[i]+" помещён в БД");
   }
   else
   {
    zapsist.sost="не выполнено";
    WriteLog("Файл "+path_si+"/"+spis_f[i]+" в БД не помещён");
    if (CopyErr(path_si,spis_f[i])) DelFile(path_si+"/"+spis_f[i]);
    DelFileM(path_temp+"/"+info[2]);
    if (kolzap < kolzap1) kolzap++;
   }
   InsertSist(false);
  }

  qApp->processEvents();
 }

 ClearDir(path_temp);

 return;
}

void ObrIzm::ObrNf306(quint64 nz,QStringList &list)
{
 QByteArray buf;
 qint16 dnk,dkk,kol;
 qint32 vnk,vkk;
 QDate isxd;
 QDate dt;
 QTime isxtm,tm;

 isxd.setDate(2000,01,01);
 isxtm.setHMS(0,0,0);

 list.clear();

 if (!ReadEdxrInBuf("cogi",nz,buf,true))
 {
  list << "Визуализация формы не поддерживается.";
  return;
 }

 QDataStream stream(buf);

 stream >> dnk >> vnk >> dkk >> vkk >> kol;

 dt=isxd.addDays(dnk);
 tm=isxtm.addSecs(vnk);
 list << "Дата и время начала коррекции : "+dt.toString("dd-MM-yyyy ")+tm.toString("hh:mm:ss");
 dt=isxd.addDays(dkk);
 tm=isxtm.addSecs(vkk);
 list << "Дата и время конца коррекции : "+dt.toString("dd-MM-yyyy ")+tm.toString("hh:mm:ss");
 list << "Число циклов коррекции : "+QString::number(kol);

 for (qint32 i=0; i<kol; i++)
 {
  stream >> dnk >> vnk >> dkk >> vkk >> kol;

  dt=isxd.addDays(dnk);
  tm=isxtm.addSecs(vnk);
  list << "Дата и время начала "+QString::number(i+1)+" цикла коррекции : "+dt.toString("dd-MM-yyyy ")+tm.toString("hh:mm:ss");
  dt=isxd.addDays(dkk);
  tm=isxtm.addSecs(vkk);
  list << "Дата и время конца "+QString::number(i+1)+" цикла коррекции : "+dt.toString("dd-MM-yyyy ")+tm.toString("hh:mm:ss");
 }

 return;
}

bool ObrIzm::SetAspsi(QString nfile,bool prsoob)
{

     QStringList list;
     qlonglong kolstr;
     QString str,str1;
     QStringList f;
     QDate date;
     QTime tm;
     quint64 nz_plppsi,nz_splppsi;

     ClearPPSI();

     f=FileInfo(nfile);
     zapplppsi.naz = f[3];
     zapplppsi.nka = zapplppsi.naz.left(3).toInt();
     zapplppsi.npn = "624P";
     zapplppsi.tip="ASPSI";

     str="DELETE FROM plppsi WHERE naz = '" + zapplppsi.naz + "'";
     db.exec(str);

     list=ReadFileInList(nfile);
     kolstr=list.count();
     if (kolstr == 0) return false;

     for (int i=0; i<kolstr; i++)
     {
      str=list[i];
      str=str.simplified();
      date=QDate::fromString(str.section(" ",0,0),"yyyy-MM-dd");
      tm=QTime::fromString(str.section(" ",1,1),"hh:mm:ss");
      DateToDay(date, zapplppsi.sutn, zapplppsi.godn);
      if (zapplppsi.godn < 99) zapplppsi.godn=zapplppsi.godn+2000;
      zapplppsi.sekn = TimeToSec(tm);
      str1="DELETE FROM plppsi WHERE godn = "+QString::number(zapplppsi.godn)+" AND sutn = "+QString::number(zapplppsi.sutn)+" AND nka = "+QString::number(zapplppsi.nka);
      str1=str+" AND tip = 'ASPSI' AND @("+QString::number(zapplppsi.sekn)+"-plppsi.sekn) < 60";
      db.exec(str1);
      nz_plppsi=InsertPPSI(prsoob);
      if (nz_plppsi == 0) continue;

//      zapsplppsi.sek=TimeToSec(tm);
      zapsplppsi.sek=str.section(" ",2,2).toInt();
      zapsplppsi.az = str.section(" ",4,4).toDouble();
      zapsplppsi.um = str.section(" ",5,5).toDouble();
      zapsplppsi.nz_plppsi = nz_plppsi;

      nz_splppsi = InsertSPLPPSI(prsoob);
      if (nz_splppsi == 0)
      {
       DeleteRow("plppsi","nz",nz_plppsi,true);
       return false;
      }
     }
     return true;

}

bool ObrIzm::DostIzm313(QString izmer,qint32 sh,bool &df1,bool &df2)
{
 qint32 shf1,shf2;
 qint32 dost,dost1,dost2;
 qreal koef;

 df1=true; df2=true;

  izmer=izmer.simplified();
  dost1=izmer.section(" ",14,14).toInt();
  dost2=izmer.section(" ",15,15).toInt();
  dost1=dost1+izmer.section(" ",16,16).toInt();
  dost2=dost2+izmer.section(" ",17,17).toInt();
  dost=dost1+dost2;
  if (dost != 4)
  {
   if (dost1 < 2) df1=false;
   if (dost2 < 2) df2=false;
   return false;
  }
  dost=izmer.section(" ",7,7).toInt();
  koef=(dost/30000000.0)-0.01;
  if (qAbs(koef) >= 0.1)
  {
   df1=false; df2=false;
   return false;
  }
  shf1=izmer.section(" ",12,12).toInt();
  shf2=izmer.section(" ",13,13).toInt();
  if (sh > 0)
  {
   if (shf1 < sh)
   {
    df1=false;
    return false;
   }
   if (shf2 < sh) return false;
   {
    df2=false;
    return false;
   }
  }

  return true;
}

bool ObrIzm::Seans313(QStringList &list,qint32 &ind_n,qint32 &ind_k)
{
 qlonglong kolstr;
 QString str,str1;
 bool df1,df2;

 ind_n=-1; ind_k=-1;

 kolstr=list.count();
 if (kolstr < 4) return false;

 for (int i=0; i<kolstr-1; i++)
 {
  str=list.at(i);
  str1=list.at(i+1);

  if (!DostIzm313(str,0,df1,df2)) continue;
  if (!DostIzm313(str1,0,df1,df2)) continue;
  ind_n=i;
  break;
 }

 if (ind_n == -1) return false;

 for (int i=kolstr-1; i>1; i--)
 {
  str=list.at(i);
  str1=list.at(i-1);

  if (!DostIzm313(str,0,df1,df2)) continue;
  if (!DostIzm313(str1,0,df1,df2)) continue;
  ind_k=i;
  break;
 }

 if (ind_k == -1) return false;
 if (ind_n+1 == ind_k) return false;

 return true;
}

quint64 ObrIzm::PomDdsKvi(QStringList list,QString nfile,bool prsoob)
{
 QString str;
 QStringList lf;
 QDateTime dt;
 quint64 nz,nz1;
 QStringList list1;

 ClearDDS();

 lf=FileInfo(nfile);
 zapdds.naz=lf[3];
 zapdds.naz=zapdds.naz.toUpper();
 zapdds.naz=zapdds.naz.mid(0,7)+"0";
 zapdds.nka=zapdds.naz.mid(0,3);

 zapdds.tip="KVI";

 str=RnxFirstObs(list);
 dt=SetDTFromStr(str);
 zapdds.sutn=dt.date().dayOfYear();
 zapdds.godn=dt.date().year();
 zapdds.sekn=TimeToSec(dt.time());
 zapdds.dtn=dt.toString("dd-MM-yyyy hh:mm:ss");

 str=RnxLastObs(list);
 dt=SetDTFromStr(str);
 zapdds.sutk=dt.date().dayOfYear();
 zapdds.godk=dt.date().year();
 zapdds.sekk=TimeToSec(dt.time());
 zapdds.dtk=dt.toString("dd-MM-yyyy hh:mm:ss");

 zapdds.strk=0;
 zapdds.kodobr=13;
 if (KolSutInRnx(list) > 1)
 {
//  QMessageBox::critical(0,"Ошибка","Помещён может быть только суточный файл ДДС.",QMessageBox::Yes);
//  return 0;
  zapdds.kodobr=14;
 }

 if (!ProvRnxInterval(list,list1)) {zapdds.strk=2;zapdds.kodobr=1;}

 nz=InsertDDS(true);
 if (nz == 0) return 0;
 nz1=ZapEdxr("KVI",nz,nfile,0,true);
 if (nz1 == 0)
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  return 0;
 }
 if (!UpdateRow("dds","nz_edxr",nz1,nz,prsoob))
 {
  DelFile(tek_path_arx+"/"+zapedxr.arxf);
  DeleteRow("dds",nz,prsoob);
  DeleteRow("edxr",nz1,prsoob);
  return 0;
 }

 return nz;
}

bool ObrIzm::PerepomEduch(QString nfile,qint32 &obr)
{
 QStringList lf;
 qulonglong nz;
 QString ext;
 qint32 g;
 QString nazf;
 QByteArray buf;
 qint32 kolsut;
 qulonglong idlf;
 qint16 iks;
 qulonglong adlf;
 qint16 aks;
 QByteArray buf1;
 QString soob;
 qint32 k;

 lf=FileInfo(nfile);
 ext=lf[4];
 nazf=lf[2];
 ext=ext.toUpper();
 nazf=nazf.toUpper();
 obr=0;

 if (ext == "322")
 {
  if (! SetDzs(nfile)) {obr=3;return false;}
  nz=FindRowDs(zapds.naz,zapds.npn,zapds.godn,zapds.sutn,zapds.sekn,zapds.nazis,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("ds",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveDsInBasket(nz,true);
 }


 if (ext == "313")
 {
  if (! SetDs(nfile)) {obr=3;return false;}
  nz=FindRowDs(zapds.naz,zapds.npn,zapds.godn,zapds.sutn,zapds.sekn,zapds.nazis,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("ds",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveDsInBasket(nz,true);
 }

 if (ext == "SPO") //сделано в нов.табл
 {
  if (! SetSpo(nfile)) {obr=3;return false;}
  nz=FindRowDs(zapds.naz,zapds.npn,zapds.godn,zapds.sutn,zapds.sekn,zapds.nazis,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("ds",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveDsInBasket(nz,true);
 }


 if (ext.right(3) == "RNX")
 {
  if (nazf.right(2) == "MO")
  {
   SetRnxO(nfile);
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }

  if (nazf.right(2) == "MN")
  {
   SetRnxNG(nfile,"N",kolsut);
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }


 }

 if ((ext.right(3) == "BIN") || (ext.right(3) == "126") || (ext.right(3) == "127"))
 {
  if (! SetFull(nfile,buf)) {obr=3;return false;}
  nz=FindRowFullppsi(zapfull.naz,zapfull.kan,zapfull.godn,zapfull.sutn,zapfull.sekn,zapfull.nka,zapfull.npn,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("fullppsi",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveFullInBasket(nz,true);
 }

 if (ext == "KOS") //сделано в нов.табл
 {
  if (! SetKos(nfile)) {obr=3;return false;}
  nz=FindRowDs(zapds.naz,zapds.npn,zapds.godn,zapds.sutn,zapds.sekn,zapds.nazis,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("ds",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveDsInBasket(nz,true);
 }

 if (ext == "FRD") //сделано в нов.табл
 {
  if (! SetFrd(nfile)) {obr=3;return false;}
  nz=FindRowDs(zapds.naz,zapds.npn,zapds.godn,zapds.sutn,zapds.sekn,zapds.nazis,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("ds",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveDsInBasket(nz,true);
 }

 if (ext == "SP3") //сделано в нов.табл
 {
  if (! SetSP3(nfile)) {obr=3;return false;}
  nz=FindRowSp3(zapsp3.naz,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("sp3",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveSp3InBasket(nz,true);
 }

 if (ext == "SOL") //сделано в нов.табл
 {
  if (! SetSol(nfile)) {obr=3;return false;}
  nz=FindRowSol(zapsp3.naz,true);
  if (nz == 0) return true;

  if (!ReadEdxrInBuf("sol",nz,buf,true)) {obr=3;return false;}
  if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

  soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
  soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
  k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
  if (k == QMessageBox::No) return false;
  if (k == QMessageBox::Cancel) {obr=1;return false;}
  if (k == QMessageBox::Abort) {obr=2;return false;}

  return RemoveSolInBasket(nz,true);
 }

 if (ext.right(1) == "M")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   if (! SetM(nfile)) {obr=3;return false;}
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }
 }

 if (ext.right(1) == "O")
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   SetRnxO(nfile);
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }
 }

 if ((ext.right(1) == "G") || (ext.right(1) == "N"))
 {
  g=ext.left(2).toInt();
  if (g > 0)
  {
   SetRnxNG(nfile,ext.right(1),kolsut);
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }
 }

 if (ext.right(1) == "L")
 {
  g=ext.left(2).toInt();
  if (g >= 0)
  {
   SetL(nfile);
   nz=FindRowRnxi(zaprnx.naz,zaprnx.ver,zaprnx.npn,zaprnx.tip,zaprnx.godn,zaprnx.sutn,zaprnx.sekn,true);
   if (nz == 0) return true;

   if (!ReadEdxrInBuf("rnxi",nz,buf,true)) {obr=3;return false;}
   if (!ArxFile(nfile,0,idlf,iks,adlf,aks,buf1)) {obr=3;return false;}

   soob="Файл уже помещён в БД. \n "+readedxr.isxf+" КС - "+QString::number(readedxr.aks,16).toUpper()+" ДЛ - "+QString::number(readedxr.idlf)+" \n";
   soob=soob+"заменить файлом \n"+nazf+" КС - "+QString::number(aks,16).toUpper()+" ДЛ - "+QString::number(idlf);
   k=QMessageBox::question(0,"Перепомещение файла измерений",soob,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Abort,QMessageBox::No);
   if (k == QMessageBox::No) return false;
   if (k == QMessageBox::Cancel) {obr=1;return false;}
   if (k == QMessageBox::Abort) {obr=2;return false;}

   return RemoveRnxInBasket("rnxi",nz,true);
  }
 }

 return false;

}

bool ObrIzm::SetLeap(QString nfile,bool prsoob)
{
 QStringList list;
 qlonglong kolstr;
 QString str;
 quint64 nz;
 qint32 d,m,g;

 ClearLeap();

  list=ReadFileInList(nfile);
  kolstr=list.count();
  if (kolstr == 0) return false;

  for (int i=2; i<kolstr-1; i++)
  {
   str=list[i];
   str=str.simplified();

   d=str.section(" ",2,2).toInt();
   if (d == 0) return false;
   if (str.section(" ",3,3) == "JUN") m=6;
   if (str.section(" ",3,3) == "DEC") m=12;
   g=str.section(" ",4,4).toInt();

   if (i == 2)
   {
    zapleap.dat1.setDate(1980,1,1);
    zapleap.dat2.setDate(g,m,d);
    zapleap.dat2=zapleap.dat2.addDays(-1);

    zapleap.nz=i-1;
    zapleap.sec=i-1;

    str="DELETE FROM leap WHERE nz = " +QString::number(zapleap.nz);
    db.exec(str);

    nz = InsertLeap(prsoob);
    if (nz == 0) return false;
   }

   zapleap.dat1.setDate(g,m,d);

   if (i == kolstr-2) zapleap.dat2.setDate(2999,1,1);
   else
   {
    str=list[i+1];
    str=str.simplified();

    d=str.section(" ",2,2).toInt();
    if (d == 0) return false;
    if (str.section(" ",3,3) == "JUN") m=6;
    if (str.section(" ",3,3) == "DEC") m=12;
    g=str.section(" ",4,4).toInt();

    zapleap.dat2.setDate(g,m,d);
    zapleap.dat2=zapleap.dat2.addDays(-1);
   }

   zapleap.nz=i;
   zapleap.sec=i;

   str="DELETE FROM leap WHERE nz = " +QString::number(zapleap.nz);
   db.exec(str);

   nz = InsertLeap(prsoob);
   if (nz == 0) return false;
  }
     return true;
}

bool ObrIzm::VidEdvid(quint64 nz_edvid,QString path_vid,bool pr_dir)
{
 QString path2,file,vfile;
 QByteArray buf;
 QDir dir;
 QStringList fi;
 qint32 istr;
 QString dt;

 path2=path_vid;

 if (!ReadEdvid(nz_edvid,false)) return false;
 if (((zapedvid.tip == "KVI") || (zapedvid.tip == "KVNO")) && (pr_dir)) path2=path_vid+"/DDS";
 if ((zapedvid.tip == "FULL") && (pr_dir)) path2=path_vid+"/PPSI";
 if (((zapedvid.tip == "ALTI") || (zapedvid.tip == "CAL1") || (zapedvid.tip == "CAL2")) && (pr_dir)) path2=path_vid+"/VRV";
 if (((zapedvid.tip == "KOS") || (zapedvid.tip == "FRD")) && (pr_dir)) path2=path_vid+"/KOS";
 if (((zapedvid.tip == "DS") || (zapedvid.tip == "SPO")) && (pr_dir)) path2=path_vid+"/DS";
 if ((zapedvid.tip == "DZS") && (pr_dir)) path2=path_vid+"/DZS";
 if ((zapedvid.tip == "I_M") && (pr_dir)) path2=path_vid+"/METEO";
 if (((zapedvid.tip == "I_O") || (zapedvid.tip == "I_N") || (zapedvid.tip == "I_G") || (zapedvid.tip == "I_L")) && (pr_dir)) path2=path_vid+"/ISX_BDS";
 if (((zapedvid.tip == "V_O") || (zapedvid.tip == "V_N") || (zapedvid.tip == "V_G")) && (pr_dir)) path2=path_vid+"/BDS";
 if ((zapedvid.tip == "SP3") && (pr_dir)) path2=path_vid+"/SP3";
 if ((zapedvid.tip == "SOL") && (pr_dir)) path2=path_vid+"/SOL";
 if (path2.isEmpty()) return false;
 if (!ReadEdxrInBuf("edvid",nz_edvid,buf,false)) return false;
 dir.setPath(path2);
 if (!dir.exists()) dir.mkpath(path2);
 file=path2+"/"+readedxr.isxf;
 if (WriteFile(file,buf,true) == -1) return false;
 if ((zapzapr.rnx2) && (zapedvid.tip == "V_O")) if (!RnxToRnx(file)) {DelFileM(file); return false;}
 if ((zapzapr.rnx2) && (zapedvid.tip == "V_N")) if (!RnxToRnx(file)) {DelFileM(file); return false;}
 if ((zapzapr.rnx2) && (zapedvid.tip == "V_G")) if (!RnxToRnx(file)) {DelFileM(file); return false;}
 if ((zapzapr.pr_alti) && (zapedvid.tip == "ALTI"))
 {
  fi=FileInfo(file);
  vfile=fi[3];
  istr=vfile.lastIndexOf('_');
  vfile=vfile.mid(0,istr);
  vfile=fi[1]+"/"+vfile+"_UGN0."+fi[4];
  if (!QFile::rename(file,vfile)) {DelFileM(file); return false;}
 }

 dt=QDateTime::currentDateTime().toString("dd_MM_yyyy hh:mm:ss");

 UpdateRow("edvid","dvid",dt,nz_edvid,false);

 return true;
}

QDateTime ObrIzm::DmtToGps(QDateTime dt_dmt)
{
 QDateTime dt;
 qint32 sec;

 dt=dt_dmt.addSecs(-10800);

 sec=FindLeap(dt.date());
 dt=dt.addSecs(sec);

 return dt;
}

//получение времени из кадра ППСИ
//buf - область памяти с сеансом ППСИ
//ind - смещение в памяти на начало кадра
//ms - милисекунды из времени получения кадра
QTime ObrIzm::TimeKadrFULL(QByteArray &buf,qint64 ind,qint32 &ms)
{
 QTime t;
 QTime time0;
 quint32 cek,msek;
 QString bi;
 bool ok;

 time0.setHMS(0,0,0,0);

 QByteArray tk=buf.mid(ind+524,4).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(6,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(4,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));

 cek = bi.mid(16,17).toUInt(&ok,2);
 msek = bi.mid(5,10).toUInt(&ok,2);
 t=time0.addSecs(cek);
 t=t.addMSecs(msek);
 ms=msek;

 return t;
}

//получение информации из кадра ППСИ
//buf - область памяти с сеансом ППСИ
//sm - смещение в памяти на начало кадра
//nvk - № виртуального канала (для кадра без синхросигнала: -1)
//pr_crc - признак КС (false - плохая)
//tm - возвращает время получения кадра hh:mm:ss.zzz
//s - признак наличия синхросигнала (false - нет синхросигнала)
//aru1,aru2 - АРУ станции
//sh1,sh2 - уровень сигнал/шум
//ms - милисекунды из времени получения кадра
void ObrIzm::KadrFULL(QByteArray &buf,qint64 sm,qint32 &nvk,bool &pr_crc,QTime &tm,bool &s,qint32 &aru1,qint32 &aru2,qint32 &sh1,qint32 &sh2,qint32 &ms)
{
    QByteArray sinxr,buf1;
    qint64 ind;

    pr_crc=true;

    buf1=buf.mid(sm,524);
    sinxr=QByteArray::fromHex("1ACFFC1D");
    ind=buf1.indexOf(sinxr,0);
    if (ind == -1) //не найдено маркеров синхронизации
    {
        s=false; pr_crc=false; nvk=-1;
    }
    else
    {
        s=true;
        if (!CrcKadrFULL(buf,sm)) pr_crc=false;
        ReadGlZagKadrFULL(buf,sm);
        nvk=glzag.fl.nvirtkan;
    }

    TelKadrFULL(buf,sm,tm,aru1,aru2,sh1,sh2,ms);

    return;
}

//получение телеметрической информации из кадра ППСИ
//buf - область памяти с сеансом ППСИ
//ind - смещение в памяти на начало кадра
//t - возвращает время получения кадра hh:mm:ss.zzz
//aru1,aru2 - АРУ станции
//sh1,sh2 - уровень сигнал/шум
//ms - милисекунды из времени получения кадра
void ObrIzm::TelKadrFULL(QByteArray &buf,qint64 ind,QTime &t,qint32 &aru1,qint32 &aru2,qint32 &sh1,qint32 &sh2,qint32 &ms)
{
 QString bi;
 bool ok;
 QByteArray tk;

 t=TimeKadrFULL(buf,ind,ms);

 tk.clear();
 tk=buf.mid(ind+528,2).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));
 aru1 = bi.mid(0,8).toInt(&ok,2);

 tk.clear();
 tk=buf.mid(ind+530,2).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));
 aru2 = bi.mid(0,8).toInt(&ok,2);

 tk.clear();
 tk=buf.mid(ind+532,2).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));
 sh1 = bi.mid(0,8).toInt(&ok,2);

 tk.clear();
 tk=buf.mid(ind+534,2).toHex().toUpper();
 bi=QString("%1").arg(tk.mid(2,2).toInt(&ok,16),8,2,QChar('0'))+
      QString("%1").arg(tk.mid(0,2).toInt(&ok,16),8,2,QChar('0'));
 sh2 = bi.mid(0,8).toInt(&ok,2);

 return;
}

//расчёт статистики сеанса ППСИ
//заполняет структуру statfull
//buf - область памяти с сеансом ППСИ
//tn,tk - время начала/конца интервала для расчёта (обычно - план АСПСИ)
//prprt,nfile_prt - признак печати и путь к файлу протокола
bool ObrIzm::StatFull(QByteArray &buf, bool prprt, QString nfile_prt,QTime tn,QTime tk)
{
 qint64 ind;
 bool fl_crc,s;
 qint64 koldds;
 QStringList prot;
 QString str;
 QTime tm;
 qint32 aru1,aru2,sh1,sh2,nvk;
 qint32 ms;

 ClearStatFull(); //очистка структуры StrStatFull

 ind=512;

 while(ind<buf.length()-576)
 {
  KadrFULL(buf,ind,nvk,fl_crc,tm,s,aru1,aru2,sh1,sh2,ms);

  if (tm > tk) {ind=ind+576; continue;}
  if (tm < tn) {ind=ind+576; continue;}

  statfull.k++;
  if (!fl_crc) statfull.crc++;

  if (nvk == -1) statfull.kbm++;

  if (nvk == 0) //пустой кадр
  {
   statfull.pk++;
   if (!fl_crc) statfull.opk++;
  }

 if (nvk == 1) //1 часть ВРВ
 {
  statfull.vrv1++;
  if (!fl_crc) statfull.ovrv1++;
 }

 if (nvk == 2) //КВНО
 {
  statfull.kvno++;
  if (!fl_crc) statfull.okvno++;
 }

 if (nvk == 3) //2 часть ВРВ
 {
  statfull.vrv2++;
  if (!fl_crc) statfull.ovrv2++;
 }

 if ((nvk == 4) || (nvk == 5) || (nvk == 6)) //КВИ
 {
  statfull.kvi++;
  if (!fl_crc) statfull.okvi++;
  koldds=ObrKadrKVI(buf,ind);
  statfull.izm_kvi=statfull.izm_kvi+koldds;
 }

 if (nvk == 7) //резерв
 {
  statfull.rez++;
  if (!fl_crc) statfull.orez++;
 }

 ind=ind+576;
}


 if (!prprt) return true;

 prot << nfile_prt;

 str="Количество прочитанных кадров : "+QString::number(statfull.k);
 prot << str;
 str="Количество кадров без маркера синхронизации : "+QString::number(statfull.kbm);
 prot << str;
 str="Количество кадров с ошибками crc : "+QString::number(statfull.crc);
 prot << str;
 str="Количество пустых кадров : "+QString::number(statfull.pk);
 prot << str;
 str="Количество кадров перв.ВРВ : "+QString::number(statfull.vrv1);
 prot << str;
 str="Количество кадров втор.ВРВ : "+QString::number(statfull.vrv2);
 prot << str;
 str="Количество кадров ДДС КВИ : "+QString::number(statfull.kvi);
 prot << str;
 str="Количество кадров ДДС КВНО : "+QString::number(statfull.kvno);
 prot << str;
 str="Количество резервных кадров : "+QString::number(statfull.rez);
 prot << str;
 str="Количество пустых кадров с ошибками crc : "+QString::number(statfull.opk);
 prot << str;
 str="Количество кадров перв.ВРВ с ошибками crc : "+QString::number(statfull.ovrv1);
 prot << str;
 str="Количество кадров втор.ВРВ с ошибками crc : "+QString::number(statfull.ovrv2);
 prot << str;
 str="Количество кадров ДДС КВИ с ошибками crc : "+QString::number(statfull.okvi);
 prot << str;
 str="Количество кадров ДДС КВНО с ошибками crc : "+QString::number(statfull.okvno);
 prot << str;
 str="Количество резервных кадров с ошибками crc : "+QString::number(statfull.orez);
 prot << str;
 str="Общее количество измерений КВИ : "+QString::number(statfull.kvi*8);
 prot << str;
 str="Полученное количество измерений КВИ : "+QString::number(statfull.izm_kvi);
 prot << str;

 if (nfile_prt != "") WriteFileFromList(prot,nfile_prt);

 return true;
}

//получение содержимого двух сеансов ППСИ в виде сгруппированного по времени текстового списка
//list - содержимое двух сеансов ППСИ в виде отсортированного текстового списка в формате:
//время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
//формат строки:
//время кадра(hh:mm:ss.zzz) для двух каналов #
//время кадра(hh:mm:ss.zzz);1 (канал);№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров) # (если нет информации по каналу: пробел #)
//время кадра(hh:mm:ss.zzz);2 (канал);№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров) (если нет информации по каналу: ничего не выводится)
QStringList ObrIzm::ObKadrSeansFull(QStringList &list)
{
    QStringList ret;
    QString str1,str2;
    QString ostr,ostr1,ostr2;
    qint32 chk=0;

    ret.clear();
    if (list.isEmpty()) return ret;

    ZapKadr zapkadr1;
    ZapKadr zapkadr2;

    for (qint32 i=0; i<list.count()-2; i++)
    {
        str1=list.at(i);
        str2=list.at(i+1);
        chk=i;

        zapkadr1.tm=QTime::fromString(str1.section(";",0,0),"hh:mm:ss.zzz");
        zapkadr1.kan=str1.section(";",1,1).toInt();
        zapkadr1.vir=str1.section(";",2,2).toInt();
        zapkadr1.sh=str1.section(";",3,3).toInt();
        zapkadr1.aru=str1.section(";",4,4).toInt();
        zapkadr1.ks=str1.section(";",5,5).toInt();
        zapkadr1.ms=str1.section(";",6,6).toInt();
        if (zapkadr1.ms == 999) zapkadr1.ms=1000;

        zapkadr2.tm=QTime::fromString(str2.section(";",0,0),"hh:mm:ss.zzz");
        zapkadr2.kan=str2.section(";",1,1).toInt();
        zapkadr2.vir=str2.section(";",2,2).toInt();
        zapkadr2.sh=str2.section(";",3,3).toInt();
        zapkadr2.aru=str2.section(";",4,4).toInt();
        zapkadr2.ks=str2.section(";",5,5).toInt();
        zapkadr2.ms=str2.section(";",6,6).toInt();
        if (zapkadr2.ms == 999) zapkadr1.ms=1000;

        if (zapkadr1.kan == zapkadr2.kan) //нет данных по одному из каналов
        {
            ostr1.clear();ostr2.clear();
            ostr=zapkadr1.tm.toString("hh:mm:ss.zzz # ");
            if (zapkadr1.kan == 1)
            {
                ostr1=zapkadr1.tm.toString("hh:mm:ss.zzz")+";1;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru)+" # ";
            }
            else
            {
                ostr1=" # ";
                ostr2=zapkadr1.tm.toString("hh:mm:ss.zzz")+";2;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru);
            }
            ostr=ostr+ostr1+ostr2;
            ret << ostr;
            continue;
        }

        if ((qAbs(zapkadr1.ms-zapkadr2.ms) <= 1) && ((zapkadr1.ks == zapkadr2.ks)||(zapkadr1.ks == 0)||(zapkadr2.ks == 0))) //совпадение
        {
            ostr1.clear();ostr2.clear();
            if (zapkadr1.kan == 1)
            {
                ostr=zapkadr1.tm.toString("hh:mm:ss.zzz # ");
                ostr1=zapkadr1.tm.toString("hh:mm:ss.zzz")+";1;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru)+" # ";
                ostr2=zapkadr2.tm.toString("hh:mm:ss.zzz")+";2;"+QString::number(zapkadr2.vir)+";"+QString::number(zapkadr2.ks)+";"+QString::number(zapkadr2.sh)+";"+QString::number(zapkadr2.aru);
            }
            else
            {
                ostr=zapkadr2.tm.toString("hh:mm:ss.zzz # ");
                ostr1=zapkadr2.tm.toString("hh:mm:ss.zzz")+";1;"+QString::number(zapkadr2.vir)+";"+QString::number(zapkadr2.ks)+";"+QString::number(zapkadr2.sh)+";"+QString::number(zapkadr2.aru)+" # ";
                ostr2=zapkadr1.tm.toString("hh:mm:ss.zzz")+";2;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru);
            }
            ostr=ostr+ostr1+ostr2;
            ret << ostr;
            i++;
            continue;
        }

        ostr1.clear();ostr2.clear();
        ostr=zapkadr1.tm.toString("hh:mm:ss.zzz # ");
        if (zapkadr1.kan == 1)
        {
            ostr1=zapkadr1.tm.toString("hh:mm:ss.zzz")+";1;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru)+" # ";
        }
        else
        {
            ostr1=" # ";
            ostr2=zapkadr1.tm.toString("hh:mm:ss.zzz")+";2;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru);
        }
        ostr=ostr+ostr1+ostr2;
        ret << ostr;
        continue;
    }

    if (chk < list.count()-2)
    {
        str1=list.at(chk+1);

        zapkadr1.tm=QTime::fromString(str1.section(";",0,0),"hh:mm:ss.zzz");
        zapkadr1.kan=str1.section(";",1,1).toInt();
        zapkadr1.vir=str1.section(";",2,2).toInt();
        zapkadr1.ks=str1.section(";",5,5).toInt();
        zapkadr1.ms=str1.section(";",6,6).toInt();
        if (zapkadr1.ms == 999) zapkadr1.ms=1000;

        ostr1.clear();ostr2.clear();
        ostr=zapkadr1.tm.toString("hh:mm:ss.zzz # ");
        if (zapkadr1.kan == 1)
        {
            ostr1=zapkadr1.tm.toString("hh:mm:ss.zzz")+";1;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru)+" # ";
        }
        else
        {
            ostr1=" # ";
            ostr2=zapkadr1.tm.toString("hh:mm:ss.zzz")+";2;"+QString::number(zapkadr1.vir)+";"+QString::number(zapkadr1.ks)+";"+QString::number(zapkadr1.sh)+";"+QString::number(zapkadr1.aru);
        }
        ostr=ostr+ostr1+ostr2;
        ret << ostr;
    }

    return ret;
}

//получение содержимого сеанса ППСИ в текстовом виде
//buf - область памяти с сеансом ППСИ
//tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
//kan - канал сеанса
//формат строки:
//время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
QStringList ObrIzm::SeansFullInList(QByteArray &buf,QTime tn,QTime tk,QString kan)
{
    QByteArray buf1;
    qint32 nvk;
    bool pr_crc,s;
    qint64 ind;
    QTime tm;
    QStringList list;
    qint16 ks;
    qint32 aru1,aru2,sh1,sh2;
    QString str;
    qint32 ms;

    list.clear();

    ind=512;

    while(ind<buf.length()-576)
    {
     KadrFULL(buf,ind,nvk,pr_crc,tm,s,aru1,aru2,sh1,sh2,ms);
     if (tm > tk) {ind=ind+576; continue;}
     if (tm < tn) {ind=ind+576; continue;}

     if ((!pr_crc) || (!s))
     {
      str=tm.toString("hh:mm:ss.zzz")+";"+kan+";"+QString::number(nvk)+";"+QString::number(sh1)+";"+QString::number(aru1)+";0;"+QString::number(ms);
      list << str;
      ind=ind+576;
      continue;
     }

     buf1.clear();
     buf1=buf.mid(ind+6,512);
     ks=qChecksum(buf1,512);
     str=tm.toString("hh:mm:ss.zzz")+";"+kan+";"+QString::number(nvk)+";"+QString::number(sh1)+";"+QString::number(aru1)+";"+QString::number(ks).toUpper()+";"+QString::number(ms);
     list << str;
     ind=ind+576;
    }
 return list;
}

//получение содержимого двух сеансов ППСИ в виде отсортированного текстового списка
//buf1,buf2 - области памяти с сеансами ППСИ (1 и 2)
//tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
//формат строки:
//время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
QStringList ObrIzm::SeansFullInList(QByteArray &buf1,QByteArray &buf2,QTime tn,QTime tk)
{
 QStringList list1,list2;

 list1=SeansFullInList(buf1,tn,tk,"1");

 list2=SeansFullInList(buf2,tn,tk,"2");

 AddList(list1,list2);

 qSort(list1);

 return list1;
}

//получение статистики сеанса ППСИ по двум каналам
//buf1,buf2 - области памяти с сеансами ППСИ (1 и 2)
//tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
//kolk - количество кадров в сеансах (соответствует масимальному количеству из двух каналов)
//kolbr - количество отбракованных кадров (по маркеру синхр. и КС)
bool ObrIzm::StatFull(QByteArray &buf1,QByteArray &buf2,QTime tn,QTime tk,qint32 &kolk,qint32 &kolbr)
{
 QStringList list,list1;
 QString str;
 QString str1,str2;
 qint32 ks1,ks2;

 list=SeansFullInList(buf1,buf2,tn,tk);
 if (list.isEmpty()) return false;
 list1=ObKadrSeansFull(list);
 if (list1.isEmpty()) return false;

 kolk=list1.count();
 kolbr=0;

 for (qint32 i=0; i<list1.count(); i++)
 {
  str=list.at(i); str=str.simplified();
  str1=str.section("#",1,1); str1=str1.simplified();
  str2=str.section("#",2,2); str2=str2.simplified();
  ks1=str1.section(";",5,5).toInt();
  ks2=str2.section(";",5,5).toInt();
  if ((ks1 == 0) && (ks2 == 0)) kolbr++;
 }

 return true;
}
