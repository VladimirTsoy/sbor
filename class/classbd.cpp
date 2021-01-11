#include "classbd.h"
#include <QtWidgets>

ClassBd::ClassBd()
{
 tek_nz_nos=0;
 tek_path_arx="";
 zapobr.naz_komp="";
 zapobr.nz_edxr=0;
 ClearUst();

 os="LINUX";
#ifdef Q_OS_WIN
 os="WIN";
#endif

}

bool ClassBd::createConnectionPSQL(QString namedb,QString name, QString password,QString host,QSqlError &str)
{
   db=QSqlDatabase::addDatabase("QPSQL");
   db.setDatabaseName(namedb);
   db.setUserName(name);
   db.setPassword(password);
   db.setHostName(host);
   if (!db.open())
   {
     str=db.lastError();
     return false;
   }
   nikp=name;
   passp=password;
   hostp=host;
  return true;
}

bool ClassBd::setModel(QSqlTableModel *model,QTableView *tableView,QString naz_table)
{
 UstModel(model,naz_table);
 if (! SelectModel(model)) return false;
 UstView(model,tableView);
 return true;
}

bool ClassBd::setRelModel(QSqlRelationalTableModel *model,QTableView *tableView,QString naz_table)
{
 UstRelModel(model,naz_table);
 if (! SelectRelModel(model)) return false;
 UstRelView(model,tableView);
 return true;
}

bool ClassBd::setQueryModel(QSqlQueryModel *model,QTableView *tableView,QString query,bool prsoob)
{
 UstQueryModel(model,query,prsoob);
 UstQueryView(model,tableView);
 return true;
}

QModelIndex ClassBd::IndModel(qlonglong id,QSqlTableModel *model)
{
    QModelIndexList ind;
    ind=model->match(model->index(0,0),Qt::DisplayRole,id,1,Qt::MatchExactly);
    int idni=ind.value(0).row();
    QModelIndex idx=model->index(idni,0);
    return idx;
}

bool ClassBd::DeleteRow(QSqlTableModel *model,QTableView *tableView)
{
 QModelIndex idx;

 idx=tableView->currentIndex();
 int tekstr=idx.row();
 if (tekstr >= 0)
 if (! model->removeRows(tekstr,1)) return false;
 if (! model->submitAll()) return false;
 tableView->setCurrentIndex(model->index(tekstr-1,0));
 tableView->setFocus();
 return true;
}

qulonglong ClassBd::InsertRow(QString strsql,bool prsoob,QSqlQuery query)
{
 qulonglong id;
 QString soob;

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при добавлении строки. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return 0;
 }
 query.next();
 id=query.value(0).toLongLong();
 return id;
}

qulonglong ClassBd::InsertRow(QString strsql,bool prsoob)
{
 QSqlQuery query;
 qulonglong id;

 id=InsertRow(strsql,prsoob,query);

 return id;
}

qlonglong ClassBd::FindRow(QString naz_table,QString naz_col,qlonglong sod,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString soob;
    QString strsql;

    soob.setNum(sod);
    strsql="SELECT * FROM "+naz_table+" WHERE "+naz_col+"="+soob;
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRow(QString naz_table,QString naz_col,qlonglong sod,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRow(naz_table,naz_col,sod,prsoob,query);

    return id;
}

qlonglong ClassBd::FindRow(QString naz_table,QString naz_col,QString sod,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString strsql;

    strsql="SELECT * FROM "+naz_table+" WHERE "+naz_col+"='"+sod+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRow(QString naz_table,QString naz_col,QString sod,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRow(naz_table,naz_col,sod,prsoob,query);

    return id;
}

qlonglong ClassBd::FindRowFull(QString dtn,QString dtk,QString nka,qint32 kan,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString strsql;

    strsql="SELECT nz FROM fullppsi WHERE dtn = '"+dtn+"' AND dtk = '"+dtk+"' AND ";
    strsql=strsql+"kan = "+QString::number(kan)+" AND nka ='"+nka+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRowFull(QString dtn,QString dtk,QString nka,qint32 kan,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRowFull(dtn,dtk,nka,kan,prsoob,query);

    return id;
}

QModelIndex ClassBd::getCurrentIndex(QTableView *table,QSqlTableModel *model,int col,qlonglong &id)
{
 QModelIndex idx;

 idx=table->currentIndex();
 id=model->data(model->index(idx.row(),col)).toLongLong();
 return idx;
}

bool ClassBd::UpdateRow(QString naz_table,QString naz_col,qlonglong zn,qlonglong nz,bool prsoob,QSqlQuery query)
{
    QString soob, soob1;
    QString strsql;

    soob.setNum(zn);
    soob1.setNum(nz);
    strsql="UPDATE "+naz_table+" SET "+naz_col+" = "+soob+" WHERE nz="+soob1;
    if (! query.exec(strsql))
    {
     soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
     WriteLog(soob);
     if (prsoob)
     {
      QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
     }
     return false;
    }

 return true;
}

bool ClassBd::UpdateRow(QString naz_table,QString naz_col,qlonglong zn,qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRow(naz_table,naz_col,zn,nz,prsoob,query);

 return ret;
}

QStringList ClassBd::SpisTable()
{
 QStringList spTable;
 spTable=db.tables();
 return spTable;
}

/*
bool ClassBd::FindFormForExt(QString ext,qlonglong &nz,QString &naz)
{
 nz=0; naz="";

 nz=FindRow("ext","naz",ext,true);
 if (nz <= 0) return false;
 nz=spis_field[3].toLongLong();

 nz=FindRow("form","nz",nz,true);
 if (nz <= 0) return false;

 return true;
}
*/

/*!
 * \brief ClassBd::GetField
 * Чтение значения поля из запроса
 * \param query[in] выполненный запрос
 * \param nom[in] индекс номера поля в записи запроса
 * \return возвращает значение поля в формате longlong
*/
qlonglong ClassBd::GetField(QSqlQuery query,int nom)
{
 return query.value(nom).toLongLong();
}

/*!
 * \brief ClassBd::GetFieldAll
 * Читает запись результата запроса и заполняет переменную класса kol_spis_field и массив spis_field
 * \param query[in] выполненный запрос
*/
void ClassBd::GetFieldAll(QSqlQuery query)
{
 QSqlRecord rec;

 rec=query.record();

 kol_spis_field=rec.count();
 if (! query.isValid()) kol_spis_field=0;

 for(int i=0; i<kol_spis_field; i++)
 {
  spis_field[i]=query.value(i);
 }

 return;
}

/*
qulonglong ClassBd::InsertEduch(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(naz,npn,godn,godk,sutn,sutk,sekn,sekk,nka,tipis,nkis,prsv,datn,vrn,datk,vrk,kodobr,probr,strk)";
 val="('"+zapeduch.naz+"','"+zapeduch.npn+"',"+QString::number(zapeduch.godn)+","+QString::number(zapeduch.godk)+","+QString::number(zapeduch.sutn)+","+QString::number(zapeduch.sutk);
 val=val+","+QString::number(zapeduch.sekn)+","+QString::number(zapeduch.sekk)+",'"+zapeduch.nka+"','"+zapeduch.tipis+"','"+zapeduch.nkis+"','"+zapeduch.prsv+"'";
 val=val+",'"+zapeduch.datn.toString("dd-MM-yyyy")+"','"+zapeduch.vrn.toString("hh:mm:ss")+"','"+zapeduch.datk.toString("dd-MM-yyyy")+"','"+zapeduch.vrk.toString("hh:mm:ss")+"',"+QString::number(zapeduch.kodobr)+","+QString::number(zapeduch.probr)+","+QString::number(zapeduch.strk)+")";


 strsql="INSERT INTO educh "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}
*/

qlonglong ClassBd::ExecQuery(QSqlQuery query,QString strsql,bool prsoob)
{
    QString soob;
    qlonglong id;

    if (! query.exec(strsql))
    {
     soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
     WriteLog(soob);
     if (prsoob)
     {
      QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
     }
     return -1;
    }
    query.next();
    id=GetField(query,0);
    GetFieldAll(query);
    if (kol_spis_field == 0) id=0;
    else id=spis_field[0].toLongLong();

    return id;
}

QList<qlonglong> ClassBd::ExecQueryList(QSqlQuery query,QString strsql,bool prsoob)
{
    QString soob;
    QList<qlonglong> list;

    if (! query.exec(strsql))
    {
     soob=QObject::tr("Ошибка при поиске записи. ")+query.lastError().text();
     WriteLog(soob);
     if (prsoob)
     {
      QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
     }
     return list;
    }
    list=GetIdRowAll(query);
    return list;
}

void ClassBd::UstModel(QSqlTableModel *model,QString naz_table)
{
 QVariant hdr;

 model->setEditStrategy(QSqlTableModel::OnManualSubmit);
 model->setTable(naz_table);
 for(int i=0; i<kol_htable; i++)
 {
   hdr=htable[i];
   model->setHeaderData(i,Qt::Horizontal,hdr);
 };
 return;
}

void ClassBd::UstQueryModel(QSqlQueryModel *model,QString query,bool prsoob)
{
 QVariant hdr;

 SelectQueryModel(model,query,prsoob);

 for(int i=0; i<kol_htable; i++)
 {
   hdr=htable[i];
   model->setHeaderData(i,Qt::Horizontal,hdr);
 };
 return;
}

void ClassBd::UstRelModel(QSqlRelationalTableModel *model,QString naz_table)
{
 QVariant hdr;

 model->setEditStrategy(QSqlTableModel::OnManualSubmit);
 model->setTable(naz_table);
 for(int i=0; i<kol_htable; i++)
 {
   hdr=htable[i];
   model->setHeaderData(i,Qt::Horizontal,hdr);
 };
 return;
}

void ClassBd::UstView(QSqlTableModel *model,QTableView *tableView)
{
 tableView->setModel(model);
 tableView->resizeColumnsToContents();
 tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
 tableView->setSelectionMode(QAbstractItemView::SingleSelection);
 tableView->horizontalHeader()->setStretchLastSection(true);
 tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
// tableView->horizontalHeader()->adjustSize();
 tableView->setCurrentIndex(model->index(0,0));
 for(int i=0; i<kol_htable; i++)
 {
  tableView->setColumnHidden(i,hidcol[i]);
 };
 return;
}

void ClassBd::UstQueryView(QSqlQueryModel *model,QTableView *tableView)
{
 tableView->setModel(model);
 tableView->resizeColumnsToContents();
 tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
 tableView->setSelectionMode(QAbstractItemView::SingleSelection);
 tableView->horizontalHeader()->setStretchLastSection(true);
 tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
// tableView->horizontalHeader()->adjustSize();
 tableView->setCurrentIndex(model->index(0,0));
 for(int i=0; i<kol_htable; i++)
 {
  tableView->setColumnHidden(i,hidcol[i]);
 };
 return;
}

void ClassBd::UstRelView(QSqlRelationalTableModel *model,QTableView *tableView)
{
 tableView->setModel(model);
 tableView->resizeColumnsToContents();
 tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
 tableView->setSelectionMode(QAbstractItemView::SingleSelection);
 tableView->horizontalHeader()->setStretchLastSection(true);
 tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
 tableView->setCurrentIndex(model->index(0,0));
 for(int i=0; i<kol_htable; i++)
 {
  tableView->setColumnHidden(i,hidcol[i]);
 };
 return;
}

bool ClassBd::SelectModel(QSqlTableModel *model)
{
 QString soob;

 model->select();
 if (model->lastError().isValid())
 {
  soob=QObject::tr("Ошибка при открытии таблицы. ")+model->lastError().text();
  WriteLog(soob);
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 return true;
}

bool ClassBd::SelectQueryModel(QSqlQueryModel *model,QString query,bool prsoob)
{
 QString soob;

 model->setQuery(query);

 if (model->lastError().isValid())
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при открытии таблицы. ")+model->lastError().text();
   WriteLog(soob);
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::SelectRelModel(QSqlRelationalTableModel *model)
{
 QString soob;

 model->select();
 if (model->lastError().isValid())
 {
  qDebug() << model->lastError();
  soob=QObject::tr("Ошибка при открытии таблицы. ")+model->lastError().text();
  WriteLog(soob);
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 return true;
}

bool ClassBd::GetNos(bool prsoob,QSqlQuery query)
{
 qlonglong nos=0;
 QString strsql;

 strsql="SELECT nz,naz FROM nos WHERE prblok = 0 ORDER BY nz ASC";

 nos=ExecQuery(query,strsql,prsoob);

 if (nos == 0) return false;

 tek_nz_nos=nos;
 tek_path_arx=spis_field[1].toString();

 return true;
}

bool ClassBd::GetNos(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=GetNos(prsoob,query);

 return ret;
}

qulonglong ClassBd::InsertEdxr(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString dat,dat1;

 dat=zapedxr.datp.toString("dd-MM-yyyy");
 dat1=zapedxr.datr.toString("dd-MM-yyyy");

 pol="(isxf,arxf,idlf,iks,adlf,aks,datp,datr,prarx,nz_nos)";
 val="('"+zapedxr.isxf+"','"+zapedxr.arxf+"',"+QString::number(zapedxr.idlf)+","+QString::number(zapedxr.iks)+","+QString::number(zapedxr.adlf)+","+QString::number(zapedxr.aks);
 val=val+",'"+dat+"','"+dat1+"',"+QString::number(zapedxr.prarx)+","+QString::number(zapedxr.nz_nos)+")";

 strsql="INSERT INTO edxr "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertEdxr(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString dat,dat1;
 QSqlQuery query;

 nz=InsertEdxr(prsoob,query);

 return nz;
}

bool ClassBd::DeleteRow(QString table,qulonglong nz,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 soob.setNum(nz);
 strsql="DELETE FROM "+table+" WHERE nz="+soob;
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при удалении записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DeleteRow(QString table,qulonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DeleteRow(table,nz,prsoob,query);

 return ret;
}

bool ClassBd::DeleteRow(QString table,QString naz_col,qlonglong zn,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 soob.setNum(zn);
 strsql="DELETE FROM "+table+" WHERE "+naz_col+"="+soob;
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при удалении записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DeleteRow(QString table,QString naz_col,qlonglong zn,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DeleteRow(table,naz_col,zn,prsoob,query);

 return ret;
}

bool ClassBd::DeleteRow(QString table,QString naz_col,QString zn,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString soob;

 strsql="DELETE FROM "+table+" WHERE "+naz_col+"='"+zn+"'";
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при удалении записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DeleteRow(QString table,QString naz_col,QString zn,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DeleteRow(table,naz_col,zn,prsoob,query);

 return ret;
}

/*
qulonglong ClassBd::InsertEdvigr(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString dt="NULL";

 if (! zapedvigr.dtvigr.isNull())
 {
  dt=zapedvigr.dtvigr.toString("dd-MM-yyyy");
  dt="'"+dt+"'";
 }

 pol="(nz_edxr,dtvigr)";
 val="("+QString::number(zapedvigr.nz_edxr)+","+dt+")";

 strsql="INSERT INTO edvigr "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}
*/

QList<qlonglong> ClassBd::GetIdRowAll(QSqlQuery query)
{
 QList<qlonglong> spis_id;

 query.first();

 do
 {
  GetFieldAll(query);
  if (kol_spis_field > 0) spis_id.append(spis_field[0].toLongLong());
 } while (query.next());

 return spis_id;
}

QList<qlonglong> ClassBd::GetKodobr(QString ntab,qint32 kodobr,qint16 probr,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM "+ntab+" WHERE probr = "+QString::number(probr)+" AND kodobr = "+QString::number(kodobr)+";";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetKodobr(QString ntab,qint32 kodobr,qint16 probr,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetKodobr(ntab,kodobr,probr,prsoob,query);

 return list;
}

bool ClassBd::ReadEdxr(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearEdxr();

 if (nz == 0) return false;

 nz1=FindRow("edxr","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapedxr.nz=spis_field[0].toLongLong();
 zapedxr.isxf=spis_field[1].toString();
 zapedxr.arxf=spis_field[2].toString();
 zapedxr.idlf=spis_field[3].toLongLong();
 zapedxr.iks=spis_field[4].toInt();
 zapedxr.adlf=spis_field[5].toLongLong();
 zapedxr.aks=spis_field[6].toInt();
 zapedxr.datp=spis_field[7].toDate();
 zapedxr.datr=spis_field[8].toDate();
 zapedxr.prarx=spis_field[9].toInt();
 zapedxr.nz_nos=spis_field[10].toLongLong();

 return true;
}

bool ClassBd::ReadEdxr(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadEdxr(nz,prsoob,query);

 return ret;
}

/*
bool ClassBd::ReadEduch(qlonglong nz,bool prsoob)
{
 qlonglong nz1;

 ClearEduch();

 nz1=FindRow("educh","nz",nz,prsoob);
 if (nz1 == -1) return false;

 if (kol_spis_field < 19) return false;

 zapeduch.nz=spis_field[0].toLongLong();
 zapeduch.naz=spis_field[1].toString();
 zapeduch.npn=spis_field[2].toString();
 zapeduch.godn=spis_field[3].toInt();
 zapeduch.godk=spis_field[4].toInt();
 zapeduch.sutn=spis_field[5].toInt();
 zapeduch.sutk=spis_field[6].toInt();
 zapeduch.sekn=spis_field[7].toLongLong();
 zapeduch.sekk=spis_field[8].toLongLong();
 zapeduch.nka=spis_field[9].toString();
 zapeduch.tipis=spis_field[10].toString();
 zapeduch.nkis=spis_field[11].toString();
 zapeduch.prsv=spis_field[12].toString();
 zapeduch.datn=spis_field[13].toDate();
 zapeduch.vrn=spis_field[14].toTime();
 zapeduch.datk=spis_field[15].toDate();
 zapeduch.vrk=spis_field[16].toTime();
 zapeduch.kodobr=spis_field[17].toInt();
 zapeduch.probr=spis_field[18].toInt();
 zapeduch.strk=spis_field[19].toInt();

 return true;
}
*/

void ClassBd::ClearEdxr()
{
 zapedxr.nz=0;
 zapedxr.isxf.clear();
 zapedxr.arxf.clear();
 zapedxr.idlf=0;
 zapedxr.iks=0;
 zapedxr.adlf=0;
 zapedxr.aks=0;
 zapedxr.datp.setDate(0,0,0);
 zapedxr.datr.setDate(0,0,0);
 zapedxr.prarx=0;
 zapedxr.nz_nos=0;
 return;
}

/*
void ClassBd::ClearEduch()
{
 zapeduch.nz=0;
 zapeduch.naz="";
 zapeduch.npn="";
 zapeduch.godn=0;
 zapeduch.godk=0;
 zapeduch.sutn=0;
 zapeduch.sutk=0;
 zapeduch.sekn=0;
 zapeduch.sekk=0;
 zapeduch.nka="";
 zapeduch.tipis="";
 zapeduch.nkis="";
 zapeduch.prsv="";
 zapeduch.datn.setDate(0,0,0);
 zapeduch.vrn.setHMS(0,0,0,0);
 zapeduch.datk.setDate(0,0,0);
 zapeduch.vrk.setHMS(0,0,0,0);
 zapeduch.kodobr=0;
 zapeduch.probr=0;
 zapeduch.strk=0;
 return;
}
*/

void ClassBd::ClearNos()
{
 zapnos.nz=0;
 zapnos.naz.clear();
 zapnos.prblok=0;
 return;
}

bool ClassBd::ReadNos(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearNos();

 nz1=FindRow("nos","nz",nz,prsoob,query);
 if (nz1 <= 0) return false;

 zapnos.nz=spis_field[0].toLongLong();
 zapnos.naz=spis_field[1].toString();
 zapnos.prblok=spis_field[2].toInt();

 return true;
}

bool ClassBd::ReadNos(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadNos(nz,prsoob,query);

 return ret;
}

/*
void ClassBd::ClearEducho()
{
 zapeducho.nz=0;
 zapeducho.naz="";
 zapeducho.npn="";
 zapeducho.godn=0;
 zapeducho.godk=0;
 zapeducho.sutn=0;
 zapeducho.sutk=0;
 zapeducho.sekn=0;
 zapeducho.sekk=0;
 zapeducho.nka="";
 zapeducho.tipis="";
 zapeducho.datn.setDate(0,0,0);
 zapeducho.vrn.setHMS(0,0,0,0);
 zapeducho.datk.setDate(0,0,0);
 zapeducho.vrk.setHMS(0,0,0,0);
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
qulonglong ClassBd::InsertEducho(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(naz,npn,godn,godk,sutn,sutk,sekn,sekk,nka,tipis,datn,vrn,datk,vrk,kodobr,probr,strk,form,prsut,inter)";
 val="('"+zapeducho.naz+"','"+zapeducho.npn+"',"+QString::number(zapeducho.godn)+","+QString::number(zapeducho.godk)+","+QString::number(zapeducho.sutn)+","+QString::number(zapeducho.sutk);
 val=val+","+QString::number(zapeducho.sekn)+","+QString::number(zapeducho.sekk)+",'"+zapeducho.nka+"','"+zapeducho.tipis;
 val=val+"','"+zapeducho.datn.toString("dd-MM-yyyy")+"','"+zapeducho.vrn.toString("hh:mm:ss")+"','"+zapeducho.datk.toString("dd-MM-yyyy")+"','"+zapeducho.vrk.toString("hh:mm:ss")+"',"+QString::number(zapeducho.kodobr)+","+QString::number(zapeducho.probr)+","+QString::number(zapeducho.strk);
 val=val+",'"+zapeducho.form+"',"+QString::number(zapeducho.prsut)+","+QString::number(zapeducho.inter)+")";

 strsql="INSERT INTO educho "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}
*/

/*
void ClassBd::ClearEdvigr()
{
 zapedvigr.nz=0;
 zapedvigr.nz_edxr=0;
 zapedvigr.dtvigr.setDate(0,0,0);
 return;
}
*/

void ClassBd::ClearSlu()
{
 QDir dir;

 zapslu.path_temp=zapust.path_temp;
 zapslu.path_obm=zapust.path_si;

 dir.setPath(zapslu.path_temp);
 if (!dir.exists())
 {
  dir.mkdir(zapslu.path_temp);
 }

 dir.setPath(zapslu.path_obm);
 if (!dir.exists())
 {
  dir.mkdir(zapslu.path_obm);
 }

 return;
}

/*
bool ClassBd::ReadEducho(qlonglong nz)
{
 qlonglong nz1;

 ClearEducho();

 nz1=FindRow("educho","nz",nz,false);
 if (nz1 == -1) return false;

 if (kol_spis_field < 21) return false;

 zapeducho.nz=spis_field[0].toLongLong();
 zapeducho.naz=spis_field[1].toString();
 zapeducho.npn=spis_field[2].toString();
 zapeducho.godn=spis_field[3].toInt();
 zapeducho.godk=spis_field[4].toInt();
 zapeducho.sutn=spis_field[5].toInt();
 zapeducho.sutk=spis_field[6].toInt();
 zapeducho.sekn=spis_field[7].toLongLong();
 zapeducho.sekk=spis_field[8].toLongLong();
 zapeducho.nka=spis_field[9].toString();
 zapeducho.tipis=spis_field[10].toString();
 zapeducho.datn=spis_field[11].toDate();
 zapeducho.vrn=spis_field[12].toTime();
 zapeducho.datk=spis_field[13].toDate();
 zapeducho.vrk=spis_field[14].toTime();
 zapeducho.kodobr=spis_field[15].toInt();
 zapeducho.probr=spis_field[16].toInt();
 zapeducho.strk=spis_field[17].toInt();
 zapeducho.form=spis_field[18].toString();
 zapeducho.prsut=spis_field[19].toInt();
 zapeducho.inter=spis_field[20].toInt();

 return true;
}
*/

void ClassBd::ClearSxema()
{
 zapsxema.nz=0;
 zapsxema.nz_educh=0;
 zapsxema.nz_educho1=0;
 zapsxema.nz_educho2=0;
 zapsxema.pr_isx=0;
 return;
}

bool ClassBd::ReadSxema(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearSxema();

 nz1=FindRow("sxema","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapsxema.nz=spis_field[0].toLongLong();
 zapsxema.nz_educh=spis_field[1].toLongLong();
 zapsxema.nz_educho1=spis_field[2].toLongLong();
 zapsxema.nz_educho2=spis_field[3].toLongLong();
 zapsxema.pr_isx=spis_field[4].toInt();

 return true;
}

bool ClassBd::ReadSxema(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadSxema(nz,prsoob,query);

 return ret;
}

qulonglong ClassBd::InsertSxema(QString ntable,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(nz_educh,nz_educho1,nz_educho2,pr_isx)";
 val="("+QString::number(zapsxema.nz_educh)+","+QString::number(zapsxema.nz_educho1);
 val=val+","+QString::number(zapsxema.nz_educho2)+","+QString::number(zapsxema.pr_isx)+")";

 strsql="INSERT INTO "+ntable+" "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertSxema(QString ntable,bool prsoob)
{
 qulonglong nz=0;
 QSqlQuery query;

 nz=InsertSxema(ntable,prsoob,query);

 return nz;
}

/*
bool ClassBd::DelEduch()
{
 QSqlQuery query;
 QString strsql,soob;

 strsql="DELETE FROM educh";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 return true;
}
*/

/*
bool ClassBd::DelEducho()
{
 QSqlQuery query;
 QString strsql,soob;

 strsql="DELETE FROM educho";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 return true;
}
*/

bool ClassBd::DelEdxr(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM edxr";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelEdxr(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelEdxr(prsoob,query);

 return ret;
}

bool ClassBd::DelSxema(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM sxema";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelSxema(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelSxema(prsoob,query);

 return ret;
}

bool ClassBd::DelSxema1(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM sxema1";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelSxema1(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelSxema1(prsoob,query);

 return ret;
}

/*
QList<qlonglong> ClassBd::GetRawForOb(QString npn, qint16 godn, qint32 sutn, quint64 nz, QString tipis, qint32 inter,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;
 QString strsql;

 strsql="SELECT nz FROM educho WHERE npn = '"+npn+"' AND godn = "+QString::number(godn)+" AND sutn = ";
 strsql=strsql+QString::number(sutn)+" AND nz != "+QString::number(nz)+" AND tipis = '"+
        tipis+"' AND kodobr != 6 AND kodobr != 1 AND inter = "+QString::number(inter)+";";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}
*/

/*
QList<qlonglong> ClassBd::GetRawForOb(qint16 godn, qint32 sutn, quint64 nz, QString tipis,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;
 QString strsql;

 strsql="SELECT nz FROM educho WHERE godn = "+QString::number(godn)+" AND sutn = ";
 strsql=strsql+QString::number(sutn)+" AND nz != "+QString::number(nz)+" AND tipis = '"+tipis+"' AND kodobr != 6 AND kodobr !=1;";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}
*/

/*
bool ClassBd::ReadEdxrForEduch(qint8 kod,qlonglong nz)
{
 QString tab="";
 qlonglong nz1;

 if (kod == 1) tab="nz_educh";
 if (kod == 2) tab="nz_educho";
 if (tab == "") return false;

 nz1=FindRow("edxr",tab,nz,false);
 if (nz1 == -1) return false;
 if (! ReadEdxr(nz1,false)) return false;

 nz1=FindRow("nos","nz",zapedxr.nz_nos,false);
 if (nz1 == -1) return false;
 if (! ReadNos(nz1,false)) return false;

 return true;
}
*/

void ClassBd::ClearStat313()
{
 zapstat313.nz=0;
 zapstat313.nz_educh=0;
 zapstat313.kizm=0;
 zapstat313.dlsek=0;
 zapstat313.kizmn=0;
 zapstat313.dlsekn=0;
 zapstat313.shf1max=0;
 zapstat313.shf1min=0;
 zapstat313.shf1med=0;
 zapstat313.shf2max=0;
 zapstat313.shf2min=0;
 zapstat313.shf2med=0;
 zapstat313.kizmnf1=0;
 zapstat313.kizmnf2=0;
 return;
}

bool ClassBd::DelStat313(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM stat313";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelStat313(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelStat313(prsoob,query);

 return ret;
}

qulonglong ClassBd::InsertStat313(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(nz_educh,kizm,dlsek,kizmn,dlsekn,shf1max,shf1min,shf1med,shf2max,shf2min,shf2med,kizmnf1,kizmnf2)";
 val="("+QString::number(zapstat313.nz_educh)+","+QString::number(zapstat313.kizm)+","+QString::number(zapstat313.dlsek)+
      ","+QString::number(zapstat313.kizmn)+","+QString::number(zapstat313.dlsekn)+","+QString::number(zapstat313.shf1max)+
      ","+QString::number(zapstat313.shf1min)+","+QString::number(zapstat313.shf1med)+","+QString::number(zapstat313.shf2max)+
      ","+QString::number(zapstat313.shf2min)+","+QString::number(zapstat313.shf2med)+
      ","+QString::number(zapstat313.kizmnf1)+","+QString::number(zapstat313.kizmnf2)+")";

 strsql="INSERT INTO stat313 "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertStat313(bool prsoob)
{
 qulonglong nz=0;
 QSqlQuery query;

 nz=InsertStat313(prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertStatDDS(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString str_mas;

 str_mas="'{";
 for (int i=0; i<24; i++)
 {
  if (i > 0) str_mas=str_mas+",";
  str_mas=str_mas+QString::number(zapstatdds.mas_ep[i]);
 }
 str_mas=str_mas+"}'";

 pol="(dat,eppl,eppol,ep_4mglo,ep_4rglo,ep_4bglo,nz_dds,mas_ep)";
 val="('"+zapstatdds.dat.toString("dd-MM-yyyy")+"',"+QString::number(zapstatdds.eppl)+","+QString::number(zapstatdds.eppol);
 val=val+","+QString::number(zapstatdds.ep_4mglo)+","+QString::number(zapstatdds.ep_4rglo)+","+QString::number(zapstatdds.ep_4bglo);
 val=val+","+QString::number(zapstatdds.nz_dds)+","+str_mas+")";

 strsql="INSERT INTO statdds "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertStatDDS(bool prsoob)
{
 qulonglong nz=0;
 QSqlQuery query;

 nz=InsertStatDDS(prsoob,query);

 return nz;
}

void ClassBd::ClearStatDDS()
{
 zapstatdds.nz=0;
 zapstatdds.dat.setDate(0,0,0);
 zapstatdds.eppl=0;
 zapstatdds.eppol=0;
 zapstatdds.ep_4mglo=0;
 zapstatdds.ep_4rglo=0;
 zapstatdds.ep_4bglo=0;
 zapstatdds.nz_dds=0;

 for(int i=0; i<24; i++) zapstatdds.mas_ep[i]=0;
 return;
}

bool ClassBd::DelStatDDS(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM statdds";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelStatDDS(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelStatDDS(prsoob,query);

 return ret;
}

void ClassBd::ClearStatrDDS()
{
 zapstatrdds.nz=0;
 zapstatrdds.nz_statdds=0;
 zapstatrdds.vrn.setHMS(0,0,0,0);
 zapstatrdds.vrr.setHMS(0,0,0,0);
 zapstatrdds.kolep=0;
 return;
}

qulonglong ClassBd::InsertStatrDDS(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(nz_statdds,vrn,vrr,kolep)";
 val="("+QString::number(zapstatrdds.nz_statdds)+",'"+zapstatrdds.vrn.toString("hh:mm:ss")+"','"+zapstatrdds.vrr.toString("hh:mm:ss")+"',";
 val=val+QString::number(zapstatrdds.kolep)+")";

 strsql="INSERT INTO statrdds "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertStatrDDS(bool prsoob)
{
 qulonglong nz=0;
 QSqlQuery query;

 nz=InsertStatrDDS(prsoob,query);

 return nz;
}

/*
void ClassBd::ClearStatPPSI()
{
 zapstatppsi.nz=0;
 zapstatppsi.naz.clear();
 zapstatppsi.dat.setDate(1900,1,1);
 zapstatppsi.vr.setHMS(0,0,0,0);
 zapstatppsi.kan=0;
 zapstatppsi.dtn_dds.setDate(zapstatppsi.dat);
 zapstatppsi.dtn_dds.setTime(zapstatppsi.vr);
 zapstatppsi.dtk_dds.setDate(zapstatppsi.dat);
 zapstatppsi.dtk_dds.setTime(zapstatppsi.vr);
 zapstatppsi.k_dds=0;
 zapstatppsi.pk_dds=0;
 zapstatppsi.oks_dds=0;
 zapstatppsi.och_dds=0;
 zapstatppsi.obr_dds=0;
 zapstatppsi.dtn_vrv.setDate(zapstatppsi.dat);
 zapstatppsi.dtn_vrv.setTime(zapstatppsi.vr);
 zapstatppsi.dtk_vrv.setDate(zapstatppsi.dat);
 zapstatppsi.dtk_vrv.setTime(zapstatppsi.vr);
 zapstatppsi.k_vrv=0;
 zapstatppsi.obr_vrv=0;
 zapstatppsi.och_vrv=0;
 return;
}
*/

/*
qulonglong ClassBd::InsertStatPPSI(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(naz,dat,vr,kan,dtn_dds,dtk_dds,k_dds,pk_dds,oks_dds,och_dds,obr_dds,dtn_vrv,dtk_vrv,k_vrv,obr_vrv,och_vrv)";
 val="('"+zapstatppsi.naz+"','"+zapstatppsi.dat.toString("dd-MM-yyyy")+"','"+zapstatppsi.vr.toString("hh:mm:ss")+"',";
 val=val+QString::number(zapstatppsi.kan)+",'"+zapstatppsi.dtn_dds.toString("dd-MM-yyyy hh:mm:ss")+"','";
 val=val+zapstatppsi.dtk_dds.toString("dd-MM-yyyy hh:mm:ss")+"',"+QString::number(zapstatppsi.k_dds)+","+QString::number(zapstatppsi.pk_dds)+","+QString::number(zapstatppsi.oks_dds)+",";
 val=val+QString::number(zapstatppsi.och_dds)+","+QString::number(zapstatppsi.obr_dds)+",'"+zapstatppsi.dtn_vrv.toString("dd-MM-yyyy hh:mm:ss")+"','";
 val=val+zapstatppsi.dtk_vrv.toString("dd-MM-yyyy hh:mm:ss")+"',"+QString::number(zapstatppsi.k_vrv)+","+QString::number(zapstatppsi.obr_vrv)+",";
 val=val+QString::number(zapstatppsi.och_vrv)+")";

 strsql="INSERT INTO statppsi "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}
*/

/*
bool ClassBd::DelStatPPSI(bool prsoob)
{
 QSqlQuery query;
 QString strsql,soob;

 strsql="DELETE FROM statppsi";

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}
*/

void ClassBd::ClearDs()
{
 zapds.nz=0;
 zapds.naz.clear();
 zapds.npn.clear();
 zapds.godn=0;
 zapds.godk=0;
 zapds.sutn=0;
 zapds.sutk=0;
 zapds.sekn=0;
 zapds.sekk=0;
 zapds.nka.clear();
 zapds.nkis.clear();
 zapds.dtn.clear();
 zapds.dtk.clear();
 zapds.sv=0;
 zapds.nazis.clear();
 zapds.nz_edxr=0;
 return;
}

bool ClassBd::DelDs(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM ds";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelDs(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelDs(prsoob,query);

 return ret;
}

quint64 ClassBd::InsertDs(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(naz,npn,godn,godk,sutn,sutk,sekn,sekk,nka,nkis,dtn,dtk,nz_edxr,sv,nazis)";
 val="('"+zapds.naz+"','"+zapds.npn+"',"+QString::number(zapds.godn)+","+QString::number(zapds.godk)+","+QString::number(zapds.sutn)+","+QString::number(zapds.sutk);
 val=val+","+QString::number(zapds.sekn)+","+QString::number(zapds.sekk)+",'"+zapds.nka+"','"+zapds.nkis+"'";
 val=val+",'"+zapds.dtn+"','"+zapds.dtk+"',"+QString::number(zapds.nz_edxr)+","+QString::number(zapds.sv)+",'"+zapds.nazis+"'"+")";


 strsql="INSERT INTO ds "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertDs(bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertDs(prsoob,query);

 return nz;
}

void ClassBd::ClearFullppsi()
{
 zapfull.nz=0;
 zapfull.naz.clear();
 zapfull.kan=0;
 zapfull.godn=0;
 zapfull.godk=0;
 zapfull.sutn=0;
 zapfull.sutk=0;
 zapfull.sekn=0;
 zapfull.sekk=0;
 zapfull.nka.clear();
 zapfull.dtn.clear();
 zapfull.dtk.clear();
 zapfull.strk=0;
 zapfull.kodobr=0;
 zapfull.nz_edxr=0;
 zapfull.npn.clear();
 return;
}

bool ClassBd::DelFull(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM fullppsi";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelFull(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelFull(prsoob,query);

 return ret;
}

quint64 ClassBd::InsertFull(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(naz,kan,godn,godk,sutn,sutk,sekn,sekk,nka,dtn,dtk,strk,kodobr,nz_edxr,npn)";
 val="('"+zapfull.naz+"',"+QString::number(zapfull.kan)+","+QString::number(zapfull.godn)+","+QString::number(zapfull.godk)+","+QString::number(zapfull.sutn)+","+QString::number(zapfull.sutk);
 val=val+","+QString::number(zapfull.sekn)+","+QString::number(zapfull.sekk)+",'"+zapfull.nka+"'";
 val=val+",'"+zapfull.dtn+"','"+zapfull.dtk+"',"+QString::number(zapfull.strk)+","+QString::number(zapfull.kodobr)+","+QString::number(zapfull.nz_edxr);
 val=val+",'"+zapfull.npn+"')";


 strsql="INSERT INTO fullppsi "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertFull(bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertFull(prsoob,query);

 return nz;
}

/*!
 * \brief ClassBd::ReadEdxr
 * Получение информации о единице хранения в таблице edxr,nos и заполнение структуры readedxr
 * \param ntab[in] название таблицы в которой есть поле(nz_edxr): ссылка на № записи в таблице edxr
 * \param nz_tab[in] № записи в таблице
 * \return при успешном выполнении возвращается true, иначе false
*/
bool ClassBd::ReadEdxr(QString ntab,quint64 nz_tab,bool prsoob,QSqlQuery query)
{
 QString str;
 qlonglong id;

 str="SELECT edxr.nz,edxr.isxf,edxr.arxf,edxr.idlf,edxr.iks,edxr.adlf,edxr.aks,edxr.prarx,nos.naz ";
 str=str+"FROM "+ntab+",edxr,nos ";
 str=str+"WHERE "+ntab+".nz_edxr = edxr.nz AND edxr.nz_nos = nos.nz AND "+ntab+".nz = "+QString::number(nz_tab);

 id=ExecQuery(query,str,prsoob);
 if (id == 0) return false;

 readedxr.nz_edxr=spis_field[0].toLongLong();
 readedxr.isxf=spis_field[1].toString();
 readedxr.arxf=spis_field[2].toString();
 readedxr.idlf=spis_field[3].toLongLong();
 readedxr.iks=spis_field[4].toInt();
 readedxr.adlf=spis_field[5].toLongLong();
 readedxr.aks=spis_field[6].toInt();
 readedxr.prarx=spis_field[7].toInt();
 readedxr.path_arx=spis_field[8].toString();

 return true;
}

bool ClassBd::ReadEdxr(QString ntab,quint64 nz_tab,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadEdxr(ntab,nz_tab,prsoob,query);

 return ret;
}

QList<qlonglong> ClassBd::GetKodobr(QString ntab,qint32 kodobr,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM "+ntab+" WHERE kodobr = "+QString::number(kodobr)+" ORDER BY godn ASC,sutn ASC;";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetKodobr(QString ntab,qint32 kodobr,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetKodobr(ntab,kodobr,prsoob,query);

 return list;
}

bool ClassBd::InsertObr(QString nkomp, quint64 nz_edxr,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;

 zapobr.naz_komp=nkomp;
 zapobr.nz_edxr=nz_edxr;

 pol="(naz_komp,nz_edxr)";
 val="('"+zapobr.naz_komp+"',"+QString::number(zapobr.nz_edxr)+")";

 strsql="INSERT INTO obr "+pol+" VALUES "+val;

 if (! query.exec(strsql)) return false;

 return true;
}

bool ClassBd::InsertObr(QString nkomp, quint64 nz_edxr)
{
 QSqlQuery query;
 bool ret;

 if (nz_edxr == 0) return false;
 ret=InsertObr(nkomp,nz_edxr,query);

 return ret;
}

/*!
 * \brief ClassBd::ReadFullppsi
 * Чтение записи из таблицы fullppsi и заполнение структуры zapfull
 * \param nz[in] ключ записи в таблице
 * \param prsoob[in] признак вывода сообщений об ошибке
 * \return если запись не найдена возвращается false; иначе true
*/
bool ClassBd::ReadFullppsi(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearFullppsi();

 //чтение записи FULL из fullppsi
 nz1=FindRow("fullppsi","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 //заполнение структуры
 zapfull.nz=spis_field[0].toLongLong();
 zapfull.naz=spis_field[1].toString();
 zapfull.kan=spis_field[2].toInt();
 zapfull.godn=spis_field[3].toInt();
 zapfull.godk=spis_field[4].toInt();
 zapfull.sutn=spis_field[5].toInt();
 zapfull.sutk=spis_field[6].toInt();
 zapfull.sekn=spis_field[7].toLongLong();
 zapfull.sekk=spis_field[8].toLongLong();
 zapfull.nka=spis_field[9].toString();
 zapfull.dtn=spis_field[10].toString();
 zapfull.dtk=spis_field[11].toString();
 zapfull.strk=spis_field[12].toInt();
 zapfull.kodobr=spis_field[13].toInt();
 zapfull.nz_edxr=spis_field[14].toLongLong();
 zapfull.npn=spis_field[15].toString();

 return true;
}

bool ClassBd::ReadFullppsi(qlonglong nz,bool prsoob)
{
 qlonglong nz1;
 QSqlQuery query;

 ClearFullppsi();

 //чтение записи FULL из fullppsi
 nz1=FindRow("fullppsi","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 //заполнение структуры
 zapfull.nz=spis_field[0].toLongLong();
 zapfull.naz=spis_field[1].toString();
 zapfull.kan=spis_field[2].toInt();
 zapfull.godn=spis_field[3].toInt();
 zapfull.godk=spis_field[4].toInt();
 zapfull.sutn=spis_field[5].toInt();
 zapfull.sutk=spis_field[6].toInt();
 zapfull.sekn=spis_field[7].toLongLong();
 zapfull.sekk=spis_field[8].toLongLong();
 zapfull.nka=spis_field[9].toString();
 zapfull.dtn=spis_field[10].toString();
 zapfull.dtk=spis_field[11].toString();
 zapfull.strk=spis_field[12].toInt();
 zapfull.kodobr=spis_field[13].toInt();
 zapfull.nz_edxr=spis_field[14].toLongLong();
 zapfull.npn=spis_field[15].toString();

 return true;
}

QList<qlonglong> ClassBd::GetRawForFull(qint16 kan,qint32 godn,qint32 sutn,quint64 sekn,QString nka,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM fullppsi WHERE kan = "+QString::number(kan)+" AND godn = "+QString::number(godn)+" AND sutn = ";
 strsql=strsql+QString::number(sutn)+" AND sekn = "+QString::number(sekn)+" AND nka = '"+nka+"'";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetRawForFull(qint16 kan,qint32 godn,qint32 sutn,quint64 sekn,QString nka,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetRawForFull(kan,godn,sutn,sekn,nka,prsoob,query);

 return list;
}

bool ClassBd::DelDDS(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM dds";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelDDS(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelDDS(prsoob,query);

 return ret;
}

void ClassBd::ClearDDS()
{
 zapdds.nz=0;
 zapdds.naz.clear();
 zapdds.tip.clear();
 zapdds.godn=0;
 zapdds.godk=0;
 zapdds.sutn=0;
 zapdds.sutk=0;
 zapdds.sekn=0;
 zapdds.sekk=0;
 zapdds.nka.clear();
 zapdds.dtn.clear();
 zapdds.dtk.clear();
 zapdds.strk=0;
 zapdds.kodobr=0;
 zapdds.nz_edxr=0;
 return;
}

/*!
 * \brief ClassBd::ReadDDS
 * чтение записи из таблицы dds и заполнение структуры ZapDDS
 * \param nz[in] ключ записи в таблице
 * \param prsoob[in] признак вывода сообщений об ошибках
 * \return возвращает false - если запись не найдена; иначе - true
*/
bool ClassBd::ReadDDS(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearDDS();

 nz1=FindRow("dds","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapdds.nz=spis_field[0].toLongLong();
 zapdds.naz=spis_field[1].toString();
 zapdds.tip=spis_field[2].toString();
 zapdds.godn=spis_field[3].toInt();
 zapdds.godk=spis_field[4].toInt();
 zapdds.sutn=spis_field[5].toInt();
 zapdds.sutk=spis_field[6].toInt();
 zapdds.sekn=spis_field[7].toLongLong();
 zapdds.sekk=spis_field[8].toLongLong();
 zapdds.nka=spis_field[9].toString();
 zapdds.dtn=spis_field[10].toString();
 zapdds.dtk=spis_field[11].toString();
 zapdds.strk=spis_field[12].toInt();
 zapdds.kodobr=spis_field[13].toInt();
 zapdds.nz_edxr=spis_field[14].toLongLong();

 return true;
}

bool ClassBd::ReadDDS(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadDDS(nz,prsoob,query);

 return ret;
}

quint64 ClassBd::InsertDDS(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(naz,tip,godn,godk,sutn,sutk,sekn,sekk,nka,dtn,dtk,strk,kodobr,nz_edxr)";
 val="('"+zapdds.naz+"','"+zapdds.tip+"',"+QString::number(zapdds.godn)+","+QString::number(zapdds.godk)+","+QString::number(zapdds.sutn)+","+QString::number(zapdds.sutk);
 val=val+","+QString::number(zapdds.sekn)+","+QString::number(zapdds.sekk)+",'"+zapdds.nka+"'";
 val=val+",'"+zapdds.dtn+"','"+zapdds.dtk+"',"+QString::number(zapdds.strk)+","+QString::number(zapdds.kodobr)+","+QString::number(zapdds.nz_edxr)+")";


 strsql="INSERT INTO dds "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertDDS(bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertDDS(prsoob,query);

 return nz;
}

QList<qlonglong> ClassBd::GetKodobrKan(QString ntab,qint32 kodobr,qint32 kan,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM "+ntab+" WHERE kan = "+QString::number(kan)+" AND kodobr = "+QString::number(kodobr)+" ORDER BY godn ASC,sutn ASC;";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetKodobrKan(QString ntab,qint32 kodobr,qint32 kan,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetKodobrKan(ntab,kodobr,kan,prsoob,query);

 return list;
}

QList<qlonglong> ClassBd::FindRowForObDDS(quint64 nz_dds,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;

 if (!ReadDDS(nz_dds,prsoob)) return list_id;
 if (zapdds.nz_edxr == 0) return list_id;
 if (zapdds.kodobr != 13) return list_id;

 strsql="SELECT nz FROM dds WHERE godn = "+QString::number(zapdds.godn)+" AND sutn = "+QString::number(zapdds.sutn);
 strsql=strsql+" AND tip = 'KVI' AND nka = '"+zapdds.nka+"' AND (kodobr = 13 OR kodobr = 0)";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForObDDS(quint64 nz_dds,bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForObDDS(nz_dds,prsoob,query);

 return list_id;
}

bool ClassBd::UpdateRows(QString naz_table,QString naz_col,qlonglong new_zn,QString otb_col,qlonglong otb_zn,bool prsoob,qint32 pr_isx,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE "+naz_table+" SET "+naz_col+" = "+QString::number(new_zn)+" WHERE pr_isx = "+QString::number(pr_isx)+" AND "+otb_col+" = "+QString::number(otb_zn);
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateRows(QString naz_table,QString naz_col,qlonglong new_zn,QString otb_col,qlonglong otb_zn,bool prsoob,qint32 pr_isx)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRows(naz_table,naz_col,new_zn,otb_col,otb_zn,prsoob,pr_isx,query);

 return ret;
}

QList<qlonglong> ClassBd::FindRowForObKVNO(quint64 nz_dds,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;

 if (!ReadDDS(nz_dds,prsoob)) return list_id;
 if (zapdds.nz_edxr == 0) return list_id;
 if (zapdds.kodobr != 16) return list_id;

 strsql="SELECT nz FROM dds WHERE godn = "+QString::number(zapdds.godn)+" AND sutn = "+QString::number(zapdds.sutn);
 strsql=strsql+" AND tip = 'KVNO' AND nka = '"+zapdds.nka+"' AND (kodobr = 16 OR kodobr = 0)";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForObKVNO(quint64 nz_dds,bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForObKVNO(nz_dds,prsoob,query);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForObAlti(quint64 nz_dds,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;

 if (!ReadDDS(nz_dds,prsoob)) return list_id;
 if (zapdds.kodobr != 18) return list_id;

 strsql="SELECT nz FROM dds WHERE godn = "+QString::number(zapdds.godn)+" AND sutn = "+QString::number(zapdds.sutn);
 strsql=strsql+" AND tip = 'ALTI' AND nka = '"+zapdds.nka+"' AND (kodobr = 18 OR kodobr = 0)";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForObAlti(quint64 nz_dds,bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForObAlti(nz_dds,prsoob,query);

 return list_id;
}

qlonglong ClassBd::FindRowFull(qint32 godn,qint32 sutn,qint64 sekn, QString nka, qint32 kan,qint32 kodobr,bool prsoob,QSqlQuery query)
{
 qlonglong id;
 QString strsql;
 qint32 sekn1,sekn2;

 sekn1=sekn-30; sekn2=sekn+30;
 if (sekn1 < 0) sekn1=0;
 if (sekn2 > 86400) sekn2=86400;

 strsql="SELECT nz FROM fullppsi WHERE (sekn > "+QString::number(sekn1)+" AND sekn < "+QString::number(sekn2)+")"+" AND ";
 strsql=strsql+"kan = "+QString::number(kan)+" AND godn = "+QString::number(godn);
 strsql=strsql+" AND sutn = "+QString::number(sutn)+" AND nka ='"+nka+"'"+" AND kodobr = "+QString::number(kodobr);
 id=ExecQuery(query,strsql,prsoob);

 return id;
}

qlonglong ClassBd::FindRowFull(qint32 godn,qint32 sutn,qint64 sekn, QString nka, qint32 kan,qint32 kodobr,bool prsoob)
{
 qlonglong id;
 QSqlQuery query;

 id=FindRowFull(godn,sutn,sekn,nka,kan,kodobr,prsoob,query);

 return id;
}

qlonglong ClassBd::FindRowFull(qint32 godn,qint32 sutn,qint64 sekn, QString nka, qint32 kan,bool prsoob,QSqlQuery query)
{
 qlonglong id;
 QString strsql;
 qint32 sekn1,sekn2;

 sekn1=sekn-30; sekn2=sekn+30;
 if (sekn1 < 0) sekn1=0;
 if (sekn2 > 86400) sekn2=86400;

 strsql="SELECT nz FROM fullppsi WHERE (sekn >= "+QString::number(sekn1)+" AND sekn <= "+QString::number(sekn2)+")"+" AND ";
 strsql=strsql+"kan = "+QString::number(kan)+" AND godn = "+QString::number(godn);
 strsql=strsql+" AND sutn = "+QString::number(sutn)+" AND nka ='"+nka+"'";
 id=ExecQuery(query,strsql,prsoob);

 return id;
}

qlonglong ClassBd::FindRowFull(qint32 godn,qint32 sutn,qint64 sekn, QString nka, qint32 kan,bool prsoob)
{
 qlonglong id;
 QSqlQuery query;

 id=FindRowFull(godn,sutn,sekn,nka,kan,prsoob,query);

 return id;
}

quint64 ClassBd::InsertStatFull(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(k,kbm,crc,pk,vrv1,vrv2,kvi,kvno,rez,opk,ovrv1,ovrv2,okvi,okvno,orez,izm_kvi,nz_fullppsi)";
 val="("+QString::number(statfull.k)+","+QString::number(statfull.kbm)+","+QString::number(statfull.crc)+","+QString::number(statfull.pk);
 val=val+","+QString::number(statfull.vrv1)+","+QString::number(statfull.vrv2)+","+QString::number(statfull.kvi)+","+QString::number(statfull.kvno);
 val=val+","+QString::number(statfull.rez)+","+QString::number(statfull.opk)+","+QString::number(statfull.ovrv1)+","+QString::number(statfull.ovrv2);
 val=val+","+QString::number(statfull.okvi)+","+QString::number(statfull.okvno)+","+QString::number(statfull.orez);
 val=val+","+QString::number(statfull.izm_kvi)+","+QString::number(statfull.nz_fullppsi)+")";


 strsql="INSERT INTO statfull "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertStatFull(bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertStatFull(prsoob,query);

 return nz;
}

bool ClassBd::UpdateRowStatFull(quint64 p1,quint64 p2,quint64 p3,QString naz_p1,QString naz_p2,QString naz_p3,quint64 nz_fullppsi,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE statfull SET "+naz_p1+" = "+QString::number(p1)+","+naz_p2+" = "+QString::number(p2);
 strsql=strsql+","+naz_p3+" = "+QString::number(p3);
 strsql=strsql+" WHERE nz_fullppsi="+QString::number(nz_fullppsi);
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateRowStatFull(quint64 p1,quint64 p2,quint64 p3,QString naz_p1,QString naz_p2,QString naz_p3,quint64 nz_fullppsi,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRowStatFull(p1,p2,p3,naz_p1,naz_p2,naz_p3,nz_fullppsi,prsoob,query);

 return ret;
}

void ClassBd::ClearStatFull()
{
 statfull.k=0;
 statfull.kbm=0;
 statfull.crc=0;
 statfull.pk=0;
 statfull.vrv1=0;
 statfull.vrv2=0;
 statfull.kvi=0;
 statfull.kvno=0;
 statfull.rez=0;
 statfull.opk=0;
 statfull.ovrv1=0;
 statfull.ovrv2=0;
 statfull.okvi=0;
 statfull.okvno=0;
 statfull.orez=0;
 statfull.izm_kvi=0;
 statfull.k_kvi=0;
 statfull.crc_kvi=0;
 statfull.och_kvi=0;
 statfull.k_kvno=0;
 statfull.crc_kvno=0;
 statfull.och_kvno=0;
 statfull.k1_vrv1=0;
 statfull.k2_vrv1=0;
 statfull.k1_vrv2=0;
 statfull.k2_vrv2=0;
 statfull.crc1_vrv1=0;
 statfull.crc2_vrv1=0;
 statfull.crc1_vrv2=0;
 statfull.crc2_vrv2=0;
 statfull.och1_vrv1=0;
 statfull.och2_vrv1=0;
 statfull.och1_vrv2=0;
 statfull.och2_vrv2=0;
 statfull.k_vrv=0;
 statfull. nz_fullppsi=0;
 statfull.k_cal=0;
 statfull.dn_kvi.setDate(0,0,0);
 statfull.dk_kvi.setDate(0,0,0);
 statfull.dn_kvno.setDate(0,0,0);
 statfull.dk_kvno.setDate(0,0,0);
 statfull.dn_vrv.setDate(0,0,0);
 statfull.dk_vrv.setDate(0,0,0);
 return;
}

bool ClassBd::UpdateRowStatFullVRV(quint64 k11,quint64 k12,quint64 k21,quint64 k22,quint64 crc11,quint64 crc12,quint64 crc21,quint64 crc22,
                                   quint64 och11,quint64 och12,quint64 och21,quint64 och22,quint64 k,quint64 nz_fullppsi,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE statfull SET k1_vrv1 = "+QString::number(k11)+",k1_vrv2 = "+QString::number(k12)+",k2_vrv1 = "+QString::number(k21);
 strsql=strsql+",k2_vrv2 = "+QString::number(k22)+",crc1_vrv1 = "+QString::number(crc11)+",crc1_vrv2 = "+QString::number(crc12);
 strsql=strsql+",crc2_vrv1 = "+QString::number(crc21)+",crc2_vrv2 = "+QString::number(crc22)+",och1_vrv1 = "+QString::number(och11);
 strsql=strsql+",och1_vrv2 = "+QString::number(och12)+",och2_vrv1 = "+QString::number(och21)+",och2_vrv2 = "+QString::number(och22);
 strsql=strsql+",k_vrv = "+QString::number(k);
 strsql=strsql+" WHERE nz_fullppsi="+QString::number(nz_fullppsi);
 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateRowStatFullVRV(quint64 k11,quint64 k12,quint64 k21,quint64 k22,quint64 crc11,quint64 crc12,quint64 crc21,quint64 crc22,
                                   quint64 och11,quint64 och12,quint64 och21,quint64 och22,quint64 k,quint64 nz_fullppsi,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRowStatFullVRV(k11,k12,k21,k22,crc11,crc12,crc21,crc22,och11,och12,och21,och22,k,nz_fullppsi,prsoob,query);

 return ret;
}

void ClassBd::ClearUst()
{
 zapust.nz=0;
 zapust.nazkomp.clear();
 zapust.path_temp.clear();
 zapust.path_si.clear();
 zapust.pravt=false;
 zapust.km=0;
 zapust.pr_obrsi=false;
 zapust.pr_obr2=false;
 zapust.pr_obr3=false;
 zapust.pr_obr4=false;
 zapust.pr_obr7=false;
 zapust.pr_obr8=false;
 zapust.pr_obr9=false;
 zapust.pr_obr10=false;
 zapust.pr_obr11_15=false;
 zapust.pr_obr12=false;
 zapust.pr_obr14=false;
 zapust.pr_obr13=false;
 zapust.pr_obr17=false;
 zapust.pr_obr16=false;
 zapust.pr_obr19=false;
 zapust.pr_obr18=false;
 zapust.kol_obrsi=0;
 zapust.kol_obr2=0;
 zapust.kol_obr3=0;
 zapust.kol_obr4=0;
 zapust.kol_obr7=0;
 zapust.kol_obr8=0;
 zapust.kol_obr9=0;
 zapust.kol_obr10=0;
 zapust.kol_obr11_15=0;
 zapust.kol_obr12=0;
 zapust.kol_obr14=0;
 zapust.kol_obr13=0;
 zapust.kol_obr17=0;
 zapust.kol_obr16=0;
 zapust.kol_obr19=0;
 zapust.kol_obr18=0;
 zapust.pr_obrzapr=0;
 zapust.path_err.clear();
 return;
}

quint64 ClassBd::InsertUst(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString spravt,spr_obrsi,spr_obr11_15,spr_obr12,spr_obr14,spr_obr13,spr_obr17,spr_obr16,spr_obr19,spr_obr18;
 QString spr_obr2,spr_obr3,spr_obr4,spr_obr7,spr_obr8,spr_obr9,spr_obr10;

 if (zapust.pravt) spravt="t";
 else spravt="f";

 if (zapust.pr_obrsi) spr_obrsi="t";
 else spr_obrsi="f";

 if (zapust.pr_obr2) spr_obr2="t";
 else spr_obr2="f";

 if (zapust.pr_obr3) spr_obr3="t";
 else spr_obr3="f";

 if (zapust.pr_obr7) spr_obr7="t";
 else spr_obr7="f";

 if (zapust.pr_obr8) spr_obr8="t";
 else spr_obr8="f";

 if (zapust.pr_obr9) spr_obr9="t";
 else spr_obr9="f";

 if (zapust.pr_obr10) spr_obr10="t";
 else spr_obr10="f";

 if (zapust.pr_obr4) spr_obr4="t";
 else spr_obr4="f";

 if (zapust.pr_obr11_15) spr_obr11_15="t";
 else spr_obr11_15="f";

 if (zapust.pr_obr12) spr_obr12="t";
 else spr_obr12="f";

 if (zapust.pr_obr14) spr_obr14="t";
 else spr_obr14="f";

 if (zapust.pr_obr13) spr_obr13="t";
 else spr_obr13="f";

 if (zapust.pr_obr17) spr_obr17="t";
 else spr_obr17="f";

 if (zapust.pr_obr16) spr_obr16="t";
 else spr_obr16="f";

 if (zapust.pr_obr19) spr_obr19="t";
 else spr_obr19="f";

 if (zapust.pr_obr18) spr_obr18="t";
 else spr_obr18="f";

 pol="(nazkomp,path_temp,path_si,pravt,km,pr_obrsi,pr_obr11_15,pr_obr12,pr_obr14,"
     "pr_obr13,pr_obr17,pr_obr16,pr_obr19,pr_obr18,pr_obr2,pr_obr3,pr_obr4,pr_obr7,"
     "pr_obr8,pr_obr9,pr_obr10,pr_obrzapr,path_err)";
 val="('"+zapust.nazkomp+"','"+zapust.path_temp+"','"+zapust.path_si+"','"+spravt+"',"+QString::number(zapust.km);
 val=val+",'"+spr_obrsi+"','"+spr_obr11_15+"','"+spr_obr12+"','"+spr_obr14+"','"+spr_obr13;
 val=val+"','"+spr_obr17+"','"+spr_obr16+"','"+spr_obr19+"','"+spr_obr18+"','"+spr_obr2;
 val=val+"','"+spr_obr3+"','"+spr_obr4+"','"+spr_obr7+"','"+spr_obr8+"','"+spr_obr9+"','"+spr_obr10+"'";
 val=val+","+QString::number(zapust.pr_obrzapr)+",'"+zapust.path_err+"')";

 strsql="INSERT INTO ust "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}

bool ClassBd::ReadUst(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearUst();

 nz1=FindRow("ust","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapust.nz=spis_field[0].toLongLong();
 zapust.nazkomp=spis_field[1].toString();
 zapust.path_temp=spis_field[2].toString();
 zapust.path_si=spis_field[3].toString();
 zapust.pravt=spis_field[4].toBool();
 zapust.km=spis_field[5].toInt();
 zapust.pr_obrsi=spis_field[6].toBool();
 zapust.pr_obr2=spis_field[7].toBool();
 zapust.pr_obr3=spis_field[8].toBool();
 zapust.pr_obr4=spis_field[9].toBool();
 zapust.pr_obr7=spis_field[10].toBool();
 zapust.pr_obr8=spis_field[11].toBool();
 zapust.pr_obr9=spis_field[12].toBool();
 zapust.pr_obr10=spis_field[13].toBool();
 zapust.pr_obr11_15=spis_field[14].toBool();
 zapust.pr_obr12=spis_field[15].toBool();
 zapust.pr_obr14=spis_field[16].toBool();
 zapust.pr_obr13=spis_field[17].toBool();
 zapust.pr_obr17=spis_field[18].toBool();
 zapust.pr_obr16=spis_field[19].toBool();
 zapust.pr_obr19=spis_field[20].toBool();
 zapust.pr_obr18=spis_field[21].toBool();
 zapust.kol_obrsi=spis_field[22].toInt();
 zapust.kol_obr2=spis_field[23].toInt();
 zapust.kol_obr3=spis_field[24].toInt();
 zapust.kol_obr4=spis_field[25].toInt();
 zapust.kol_obr7=spis_field[26].toInt();
 zapust.kol_obr8=spis_field[27].toInt();
 zapust.kol_obr9=spis_field[28].toInt();
 zapust.kol_obr10=spis_field[29].toInt();
 zapust.kol_obr11_15=spis_field[30].toInt();
 zapust.kol_obr12=spis_field[31].toInt();
 zapust.kol_obr14=spis_field[32].toInt();
 zapust.kol_obr13=spis_field[33].toInt();
 zapust.kol_obr17=spis_field[34].toInt();
 zapust.kol_obr16=spis_field[35].toInt();
 zapust.kol_obr19=spis_field[36].toInt();
 zapust.kol_obr18=spis_field[37].toInt();
 zapust.pr_obrzapr=spis_field[38].toInt();
 zapust.path_err=spis_field[39].toString();

 return true;
}

bool ClassBd::ReadUst(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadUst(nz,prsoob,query);

 return ret;
}

bool ClassBd::UpdateErr(QString naz_table,qint32 zn_strk,qint32 zn_kodobr,qlonglong nz,bool prsoob,QSqlQuery query)
{
    QString soob;
    QString strsql;

    strsql="UPDATE "+naz_table+" SET strk = "+QString::number(zn_strk)+",kodobr = "+QString::number(zn_kodobr)+" WHERE nz = "+QString::number(nz);
    if (! query.exec(strsql))
    {
     soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
     WriteLog(soob);
     if (prsoob)
     {
      QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
     }
     return false;
    }

 return true;
}

bool ClassBd::UpdateErr(QString naz_table,qint32 zn_strk,qint32 zn_kodobr,qlonglong nz,bool prsoob)
{
    QSqlQuery query;
    bool ret;

    ret=UpdateErr(naz_table,zn_strk,zn_kodobr,nz,prsoob,query);

 return ret;
}

void ClassBd::ClearRnx()
{
 zaprnx.nz=0;
 zaprnx.naz.clear();
 zaprnx.ver.clear();
 zaprnx.npn.clear();
 zaprnx.tip.clear();
 zaprnx.godn=0;
 zaprnx.godk=0;
 zaprnx.sutn=0;
 zaprnx.sutk=0;
 zaprnx.sekn=0;
 zaprnx.sekk=0;
 zaprnx.dtn.clear();
 zaprnx.dtk.clear();
 zaprnx.strk=0;
 zaprnx.kodobr=0;
 zaprnx.interv=0;
 zaprnx.nz_edxr=0;

 return;
}

quint64 ClassBd::InsertRnx(QString ntable,bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(naz,tip,ver,npn,godn,godk,sutn,sutk,sekn,sekk,dtn,dtk,strk,kodobr,nz_edxr,interv)";
 val="('"+zaprnx.naz+"','"+zaprnx.tip+"','"+zaprnx.ver+"','"+zaprnx.npn+"',"+QString::number(zaprnx.godn)+","+QString::number(zaprnx.godk)+","+QString::number(zaprnx.sutn)+","+QString::number(zaprnx.sutk);
 val=val+","+QString::number(zaprnx.sekn)+","+QString::number(zaprnx.sekk);
 val=val+",'"+zaprnx.dtn+"','"+zaprnx.dtk+"',"+QString::number(zaprnx.strk)+","+QString::number(zaprnx.kodobr)+","+QString::number(zaprnx.nz_edxr)+","+QString::number(zaprnx.interv)+")";

 strsql="INSERT INTO "+ntable+" "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertRnx(QString ntable,bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertRnx(ntable,prsoob,query);

 return nz;
}

bool ClassBd::ReadRnx(QString ntab,qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearRnx();

 nz1=FindRow(ntab,"nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zaprnx.nz=spis_field[0].toLongLong();
 zaprnx.naz=spis_field[1].toString();
 zaprnx.ver=spis_field[2].toString();
 zaprnx.npn=spis_field[3].toString();
 zaprnx.tip=spis_field[4].toString();
 zaprnx.godn=spis_field[5].toInt();
 zaprnx.godk=spis_field[6].toInt();
 zaprnx.sutn=spis_field[7].toInt();
 zaprnx.sutk=spis_field[8].toInt();
 zaprnx.sekn=spis_field[9].toLongLong();
 zaprnx.sekk=spis_field[10].toLongLong();
 zaprnx.dtn=spis_field[11].toString();
 zaprnx.dtk=spis_field[12].toString();
 zaprnx.strk=spis_field[13].toInt();
 zaprnx.kodobr=spis_field[14].toInt();
 zaprnx.interv=spis_field[15].toInt();
 zaprnx.nz_edxr=spis_field[16].toLongLong();

 return true;
}

bool ClassBd::ReadRnx(QString ntab,qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadRnx(ntab,nz,prsoob,query);

 return ret;
}

bool ClassBd::DelRnx(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM rnxi";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 strsql="DELETE FROM rnxv";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelRnx(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelRnx(prsoob,query);

 return ret;
}

QList<qlonglong> ClassBd::GetRawForO(QString npn, qint16 godn, qint32 sutn, quint64 nz, qint32 inter,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM rnxv WHERE tip = 'O' AND npn = '"+npn+"' AND godn = "+QString::number(godn)+" AND sutn = ";
 strsql=strsql+QString::number(sutn)+" AND nz != "+QString::number(nz)+
        " AND kodobr != 20 AND kodobr != 1 AND interv = "+QString::number(inter)+";";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetRawForO(QString npn, qint16 godn, qint32 sutn, quint64 nz, qint32 inter,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetRawForO(npn,godn,sutn,nz,inter,prsoob,query);

 return list;
}

QList<qlonglong> ClassBd::GetRawForN(qint16 godn, qint32 sutn, quint64 nz,QString tip,bool prsoob,QSqlQuery query)
{
 QList<qlonglong> list;
 QString strsql;

 strsql="SELECT nz FROM rnxv WHERE godn = "+QString::number(godn)+" AND sutn = ";
 strsql=strsql+QString::number(sutn)+" AND nz != "+QString::number(nz)+
        " AND kodobr != 20 AND kodobr != 1 AND tip = '"+tip+"';";

 list=ExecQueryList(query,strsql,prsoob);

 return list;
}

QList<qlonglong> ClassBd::GetRawForN(qint16 godn, qint32 sutn, quint64 nz,QString tip,bool prsoob)
{
 QList<qlonglong> list;
 QSqlQuery query;

 list=GetRawForN(godn,sutn,nz,tip,prsoob,query);

 return list;
}

void ClassBd::ClearStatVRV()
{
 zapstatvrv.nz=0;
 zapstatvrv.nz_dds=0;
 zapstatvrv.dat.setDate(0,0,0);
 zapstatvrv.plan=0;
 zapstatvrv.vip=0;
 zapstatvrv.vne=0;
 zapstatvrv.nevip=0;
 zapstatvrv.noplan=0;
 return;
}

quint64 ClassBd::InsertStatVRV(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(nz_dds,dat,plan,vip,vne,nevip,noplan)";
 val="("+QString::number(zapstatvrv.nz_dds)+",'"+zapstatvrv.dat.toString("dd-MM-yyyy")+"',"+QString::number(zapstatvrv.plan);
 val=val+","+QString::number(zapstatvrv.vip)+","+QString::number(zapstatvrv.vne)+","+QString::number(zapstatvrv.nevip);
 val=val+","+QString::number(zapstatvrv.noplan)+")";

 strsql="INSERT INTO statvrv "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertStatVRV(bool prsoob)
{
 quint64 nz=0;
 QSqlQuery query;

 nz=InsertStatVRV(prsoob,query);

 return nz;
}

void ClassBd::ClearStat322()
{
 zapstat322.nz=0;
 zapstat322.nz_educh=0;
 zapstat322.kizm=0;
 zapstat322.dlsek=0;
 zapstat322.kizmn=0;
 zapstat322.dlsekn=0;
 zapstat322.shrcmax=0;
 zapstat322.shrcmin=0;
 zapstat322.shrcmed=0;
 zapstat322.shpsmax=0;
 zapstat322.shpsmin=0;
 zapstat322.shpsmed=0;
 return;
}

qulonglong ClassBd::InsertStat322(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;

 pol="(nz_educh,kizm,dlsek,kizmn,dlsekn,shrcmax,shrcmin,shrcmed,shpsmax,shpsmin,shpsmed)";
 val="("+QString::number(zapstat322.nz_educh)+","+QString::number(zapstat322.kizm)+","+QString::number(zapstat322.dlsek)+
      ","+QString::number(zapstat322.kizmn)+","+QString::number(zapstat322.dlsekn)+","+QString::number(zapstat322.shrcmax)+
      ","+QString::number(zapstat322.shrcmin)+","+QString::number(zapstat322.shrcmed)+","+QString::number(zapstat322.shpsmax)+
      ","+QString::number(zapstat322.shpsmin)+","+QString::number(zapstat322.shpsmed)+")";

 strsql="INSERT INTO stat322 "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

qulonglong ClassBd::InsertStat322(bool prsoob)
{
 qulonglong nz=0;
 QSqlQuery query;

 nz=InsertStat322(prsoob,query);

 return nz;
}

QModelIndex ClassBd::getCurrentIndex(QTableView *table,QSqlQueryModel *model,int col,qlonglong &id)
{
 QModelIndex idx;

 idx=table->currentIndex();
 id=model->data(model->index(idx.row(),col)).toLongLong();
 return idx;
}

QModelIndex ClassBd::getCurrentIndex(QTableView *table,QSqlQueryModel *model,int col,QString &id)
{
 QModelIndex idx;

 idx=table->currentIndex();
 id=model->data(model->index(idx.row(),col)).toString();
 return idx;
}

bool ClassBd::DelSist(bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM sist";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelSist(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelSist(prsoob,query);

 return ret;
}

/*
void ClassBd::UstSist(QString komp,QString nfile,qint32 kodsist)
{
 zapsist.nz=0;
 zapsist.komp=komp;
 zapsist.dat=QDate::currentDate();
 zapsist.tm=QTime::currentTime();
 zapsist.kodsist=kodsist;
 zapsist.nfile=nfile;

 return;
}
*/

/*
quint64 ClassBd::InsertSist(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;

 pol="(komp,dat,tm,nfile,kodsist)";
 val="('"+zapsist.komp+"','"+zapsist.dat.toString("dd-MM-yyyy")+"','"+zapsist.tm.toString("hh:mm:ss");
 val=val+"','"+zapsist.nfile+"',"+QString::number(zapsist.kodsist)+")";

 strsql="INSERT INTO sist "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}
*/

bool ClassBd::ReadStatFull(qlonglong nz_full,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 ClearStatFull();

 //чтение записи FULL из fullppsi
 nz1=FindRow("statfull","nz_fullppsi",nz_full,prsoob,query);
 if (nz1 == -1) return false;

 //заполнение структуры
 statfull.k=spis_field[1].toLongLong();
 statfull.kbm=spis_field[2].toLongLong();
 statfull.crc=spis_field[3].toLongLong();
 statfull.pk=spis_field[4].toLongLong();
 statfull.vrv1=spis_field[5].toLongLong();
 statfull.vrv2=spis_field[6].toLongLong();
 statfull.kvi=spis_field[7].toLongLong();
 statfull.kvno=spis_field[8].toLongLong();
 statfull.rez=spis_field[9].toLongLong();
 statfull.opk=spis_field[10].toLongLong();
 statfull.ovrv1=spis_field[11].toLongLong();
 statfull.ovrv2=spis_field[12].toLongLong();
 statfull.okvi=spis_field[13].toLongLong();
 statfull.okvno=spis_field[14].toLongLong();
 statfull.orez=spis_field[15].toLongLong();
 statfull.izm_kvi=spis_field[16].toLongLong();
 statfull.k_kvi=spis_field[17].toLongLong();
 statfull.crc_kvi=spis_field[18].toLongLong();
 statfull.och_kvi=spis_field[19].toLongLong();
 statfull.k_kvno=spis_field[20].toLongLong();
 statfull.crc_kvno=spis_field[21].toLongLong();
 statfull.och_kvno=spis_field[22].toLongLong();
 statfull.k1_vrv1=spis_field[23].toLongLong();
 statfull.k2_vrv1=spis_field[24].toLongLong();
 statfull.k1_vrv2=spis_field[25].toLongLong();
 statfull.k2_vrv2=spis_field[26].toLongLong();
 statfull.crc1_vrv1=spis_field[27].toLongLong();
 statfull.crc2_vrv1=spis_field[28].toLongLong();
 statfull.crc1_vrv2=spis_field[29].toLongLong();
 statfull.crc2_vrv2=spis_field[30].toLongLong();
 statfull.och1_vrv1=spis_field[31].toLongLong();
 statfull.och2_vrv1=spis_field[32].toLongLong();
 statfull.och1_vrv2=spis_field[33].toLongLong();
 statfull.och2_vrv2=spis_field[34].toLongLong();
 statfull.k_vrv=spis_field[35].toLongLong();
 statfull. nz_fullppsi=spis_field[36].toLongLong();
 statfull.k_cal=spis_field[37].toLongLong();
 statfull.dn_kvi=spis_field[38].toDate();
 statfull.dk_kvi=spis_field[39].toDate();
 statfull.dn_kvno=spis_field[40].toDate();
 statfull.dk_kvno=spis_field[41].toDate();
 statfull.dn_vrv=spis_field[42].toDate();
 statfull.dk_vrv=spis_field[43].toDate();

 return true;
}

bool ClassBd::ReadStatFull(qlonglong nz_full,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadStatFull(nz_full,prsoob,query);

 return ret;
}

bool ClassBd::ReadStatDDS(qlonglong nz_dds,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;
 QString str;

 ClearStatDDS();

 nz1=FindRow("statdds","nz_dds",nz_dds,prsoob,query);
 if (nz1 == -1) return false;

 //заполнение структуры
 zapstatdds.nz=spis_field[0].toLongLong();
 zapstatdds.dat=spis_field[1].toDate();
 zapstatdds.eppl=spis_field[2].toInt();
 zapstatdds.eppol=spis_field[3].toInt();
 zapstatdds.ep_4mglo=spis_field[4].toInt();
 zapstatdds.ep_4rglo=spis_field[5].toInt();
 zapstatdds.ep_4bglo=spis_field[6].toInt();
 zapstatdds.nz_dds=spis_field[7].toLongLong();
 str=spis_field[8].toString();
 str=str.mid(1,str.length()-1);
 for (int i=0; i<24; i++)
 {
  zapstatdds.mas_ep[i]=str.section(",",i,i).toInt();
 }

 return true;
}

bool ClassBd::ReadStatDDS(qlonglong nz_dds,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadStatDDS(nz_dds,prsoob,query);

 return ret;
}

bool ClassBd::DelPotr(bool prsoob)
{
 QSqlQuery query;
 QString strsql,soob;

 strsql="DELETE FROM potr";

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

quint64 ClassBd::InsertZapr(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString spravt,spr_ds,spr_dzs,spr_kos,spr_meteo,spr_ppsi,spr_kvi,spr_kvno,spr_alti,spr_cal;
 QString spr_bds_l,spr_bds_o,spr_bds_n,spr_rnx2,spr_pralti,spr_sp3,spr_sol;

 if (zapzapr.pr_avt) spravt="t";
 else spravt="f";

 if (zapzapr.ds) spr_ds="t";
 else spr_ds="f";

 if (zapzapr.dzs) spr_dzs="t";
 else spr_dzs="f";

 if (zapzapr.kos) spr_kos="t";
 else spr_kos="f";

 if (zapzapr.meteo) spr_meteo="t";
 else spr_meteo="f";

 if (zapzapr.ppsi) spr_ppsi="t";
 else spr_ppsi="f";

 if (zapzapr.kvi) spr_kvi="t";
 else spr_kvi="f";

 if (zapzapr.kvno) spr_kvno="t";
 else spr_kvno="f";

 if (zapzapr.alti) spr_alti="t";
 else spr_alti="f";

 if (zapzapr.cal) spr_cal="t";
 else spr_cal="f";

 if (zapzapr.bds_l) spr_bds_l="t";
 else spr_bds_l="f";

 if (zapzapr.bds_o) spr_bds_o="t";
 else spr_bds_o="f";

 if (zapzapr.bds_n) spr_bds_n="t";
 else spr_bds_n="f";

 if (zapzapr.rnx2) spr_rnx2="t";
 else spr_rnx2="f";

 if (zapzapr.pr_alti) spr_pralti="t";
 else spr_pralti="f";

 if (zapzapr.sp3) spr_sp3="t";
 else spr_sp3="f";

 if (zapzapr.sol) spr_sol="t";
 else spr_sol="f";

 pol="(nz_potr,pr_avt,dat,ds,dzs,kos,meteo,ppsi,kvi,kvno,alti,cal,bds_l,bds_o,bds_n,naz,rnx2,path,dtn,dtk,dtt,pr_alti,kolsut,pr_dat,komp,sp3,sol)";
 val=" ("+QString::number(zapzapr.nz_potr)+",'"+spravt+"','"+zapzapr.dat.toString("dd-MM-yyyy hh:mm")+"','"+spr_ds+"'";
 val=val+",'"+spr_dzs+"','"+spr_kos+"','"+spr_meteo+"','"+spr_ppsi+"','"+spr_kvi;
 val=val+"','"+spr_kvno+"','"+spr_alti+"','"+spr_cal+"','"+spr_bds_l+"','"+spr_bds_o;
 val=val+"','"+spr_bds_n+"','"+zapzapr.naz+"','"+spr_rnx2+"','"+zapzapr.path+"'";
 val=val+",'"+zapzapr.dtn.toString("dd-MM-yyyy")+"','"+zapzapr.dtk.toString("dd-MM-yyyy")+"','"+zapzapr.dtt.toString("dd-MM-yyyy");
 val=val+"','"+spr_pralti+"',"+QString::number(zapzapr.kolsut)+","+QString::number(zapzapr.pr_dat);
 val=val+",'"+zapzapr.komp+"','"+spr_sp3+"','"+spr_sol+"')";

 strsql="INSERT INTO zapr "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}

void ClassBd::ClearZapr()
{
    zapzapr.nz=0;
    zapzapr.nz_potr=0;
    zapzapr.pr_avt=false;
    zapzapr.dat=QDateTime::currentDateTime();
    zapzapr.ds=false;
    zapzapr.dzs=false;
    zapzapr.kos=false;
    zapzapr.meteo=false;
    zapzapr.ppsi=false;
    zapzapr.kvi=false;
    zapzapr.kvno=false;
    zapzapr.alti=false;
    zapzapr.cal=false;
    zapzapr.bds_l=false;
    zapzapr.bds_o=false;
    zapzapr.bds_n=false;
    zapzapr.naz.clear();
    zapzapr.rnx2=false;
    zapzapr.path.clear();
    zapzapr.dtn.setDate(1900,1,1);
    zapzapr.dtk.setDate(1900,1,1);
    zapzapr.dtt.setDate(1900,1,1);
    zapzapr.pr_alti=false;
    zapzapr.kolsut=0;
    zapzapr.pr_dat=0;
    zapzapr.komp.clear();
    zapzapr.sp3=false;
    zapzapr.sol=false;
 return;
}

QModelIndex ClassBd::IndModel(qlonglong id,QSqlQueryModel *model)
{
    QModelIndexList ind;
    ind=model->match(model->index(0,0),Qt::DisplayRole,id,1,Qt::MatchExactly);
    int idni=ind.value(0).row();
    QModelIndex idx=model->index(idni,0);
    return idx;
}

quint64 ClassBd::InsertPotr(bool prsoob,quint64 nz)
{
 QString strsql;
 QString pol;
 QString val;

 pol="(nz,naz)";
 val="("+QString::number(nz)+",'"+zappotr.naz+"')";

 strsql="INSERT INTO potr "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}

bool ClassBd::ReadZapr(qlonglong nz,bool prsoob)
{
 qlonglong nz1;

 nz1=FindRow("zapr","nz",nz,prsoob);
 if (nz1 == -1) return false;

 zapzapr.nz=spis_field[0].toLongLong();
 zapzapr.nz_potr=spis_field[1].toLongLong();
 zapzapr.pr_avt=spis_field[2].toBool();
 zapzapr.dat=spis_field[3].toDateTime();
 zapzapr.ds=spis_field[4].toBool();
 zapzapr.dzs=spis_field[5].toBool();
 zapzapr.kos=spis_field[6].toBool();
 zapzapr.meteo=spis_field[7].toBool();
 zapzapr.ppsi=spis_field[8].toBool();
 zapzapr.kvi=spis_field[9].toBool();
 zapzapr.kvno=spis_field[10].toBool();
 zapzapr.alti=spis_field[11].toBool();
 zapzapr.cal=spis_field[12].toBool();
 zapzapr.bds_l=spis_field[13].toBool();
 zapzapr.bds_o=spis_field[14].toBool();
 zapzapr.bds_n=spis_field[15].toBool();
 zapzapr.naz=spis_field[16].toString();
 zapzapr.rnx2=spis_field[17].toBool();
 zapzapr.path=spis_field[18].toString();
 zapzapr.dtn=spis_field[19].toDate();
 zapzapr.dtk=spis_field[20].toDate();
 zapzapr.dtt=spis_field[21].toDate();
 zapzapr.pr_alti=spis_field[22].toBool();
 zapzapr.kolsut=spis_field[23].toInt();
 zapzapr.pr_dat=spis_field[24].toInt();
 zapzapr.komp=spis_field[25].toString();
 zapzapr.sp3=spis_field[26].toBool();
 zapzapr.sol=spis_field[27].toBool();

 if (zapzapr.pr_dat == 1) zapzapr.dtk=QDate::currentDate();
 if (zapzapr.pr_dat == 2) zapzapr.dtk=QDate::currentDate().addDays(-zapzapr.kolsut);
 return true;
}

bool ClassBd::ReadPotr(qlonglong nz,bool prsoob)
{
 qlonglong nz1;

 nz1=FindRow("potr","nz",nz,prsoob);
 if (nz1 == -1) return false;

 zappotr.nz=spis_field[0].toLongLong();
 zappotr.naz=spis_field[1].toString();

 return true;
}

bool ClassBd::UpdateZapr(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString spravt,spr_ds,spr_dzs,spr_kos,spr_meteo,spr_ppsi,spr_kvi,spr_kvno,spr_alti,spr_cal;
 QString spr_bds_l,spr_bds_o,spr_bds_n,spr_rnx2,spr_pralti,spr_sp3,spr_sol;
 QString soob;

 if (zapzapr.pr_avt) spravt="t";
 else spravt="f";

 if (zapzapr.ds) spr_ds="t";
 else spr_ds="f";

 if (zapzapr.dzs) spr_dzs="t";
 else spr_dzs="f";

 if (zapzapr.kos) spr_kos="t";
 else spr_kos="f";

 if (zapzapr.meteo) spr_meteo="t";
 else spr_meteo="f";

 if (zapzapr.ppsi) spr_ppsi="t";
 else spr_ppsi="f";

 if (zapzapr.kvi) spr_kvi="t";
 else spr_kvi="f";

 if (zapzapr.kvno) spr_kvno="t";
 else spr_kvno="f";

 if (zapzapr.alti) spr_alti="t";
 else spr_alti="f";

 if (zapzapr.cal) spr_cal="t";
 else spr_cal="f";

 if (zapzapr.bds_l) spr_bds_l="t";
 else spr_bds_l="f";

 if (zapzapr.bds_o) spr_bds_o="t";
 else spr_bds_o="f";

 if (zapzapr.bds_n) spr_bds_n="t";
 else spr_bds_n="f";

 if (zapzapr.rnx2) spr_rnx2="t";
 else spr_rnx2="f";

 if (zapzapr.pr_alti) spr_pralti="t";
 else spr_pralti="f";

 if (zapzapr.sp3) spr_sp3="t";
 else spr_sp3="f";

 if (zapzapr.sol) spr_sol="t";
 else spr_sol="f";

 strsql="UPDATE zapr SET nz_potr = "+QString::number(zapzapr.nz_potr)+",pr_avt = '"+spravt+"',dat = '"+zapzapr.dat.toString("dd-MM-yyyy hh:mm")+"'";
 strsql=strsql+",ds = '"+spr_ds+"',dzs = '"+spr_dzs+"',kos = '"+spr_kos+"',meteo = '"+spr_meteo+"',ppsi = '"+spr_ppsi+"'";
 strsql=strsql+",kvi = '"+spr_kvi+"',kvno = '"+spr_kvno+"',alti = '"+spr_alti+"',cal = '"+spr_cal+"',bds_l = '"+spr_bds_l+"'";
 strsql=strsql+",bds_o = '"+spr_bds_o+"',bds_n = '"+spr_bds_n+"',naz = '"+zapzapr.naz+"',rnx2 = '"+spr_rnx2+"',path = '"+zapzapr.path+"'";
 strsql=strsql+",dtn = '"+zapzapr.dtn.toString("dd-MM-yyyy")+"',dtk = '"+zapzapr.dtk.toString("dd-MM-yyyy")+"',dtt = '"+zapzapr.dtt.toString("dd-MM-yyyy")+"'";
 strsql=strsql+",pr_alti = '"+spr_pralti+"',kolsut = "+QString::number(zapzapr.kolsut)+",pr_dat = "+QString::number(zapzapr.pr_dat);
 strsql=strsql+",komp = '"+zapzapr.komp+"',sp3 = '"+spr_sp3+"',sol = '"+spr_sol+"'";
 strsql=strsql+" WHERE nz = "+QString::number(zapzapr.nz);

 if (! query.exec(strsql))
 {
  soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
  WriteLog(soob);
  if (prsoob)
  {
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateZapr(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateZapr(prsoob,query);

 return ret;
}

quint64 ClassBd::InsertEdvid(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(nz_zapr,nz_edxr,naz_edxr,tip)";
 val="("+QString::number(zapedvid.nz_zapr)+","+QString::number(zapedvid.nz_edxr)+",'"+zapedvid.naz_edxr+"'";
// val=val+",'"+zapedvid.tip+"',"+QString::number(zapedvid.preobr)+")";
 val=val+",'"+zapedvid.tip+"')";
 strsql="INSERT INTO edvid "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}

bool ClassBd::ReadEdvid(qlonglong nz,bool prsoob)
{
 qlonglong nz1;

 nz1=FindRow("edvid","nz",nz,prsoob);
 if (nz1 == -1) return false;

 zapedvid.nz=spis_field[0].toLongLong();
 zapedvid.nz_zapr=spis_field[1].toLongLong();
 zapedvid.nz_edxr=spis_field[2].toLongLong();
 zapedvid.naz_edxr=spis_field[3].toString();
 zapedvid.dvid=spis_field[4].toDateTime();
 zapedvid.tip=spis_field[5].toString();
// zapedvid.preobr=spis_field[6].toInt();

 return true;
}

void ClassBd::ClearEdvid()
{
 QDate dat;
 QTime tm;

 zapedvid.nz=0;
 zapedvid.nz_zapr=0;
 zapedvid.nz_edxr=0;
 zapedvid.naz_edxr.clear();
 dat.setDate(1900,1,1);
 tm.setHMS(0,0,0);
 zapedvid.dvid.setDate(dat);
 zapedvid.dvid.setTime(tm);
 zapedvid.tip.clear();
// zapedvid.preobr=0;
 return;
}

bool ClassBd::UpdateRow(QString naz_table,QString naz_col,QString zn,qlonglong nz,bool prsoob,QSqlQuery query)
{
    QString soob, soob1;
    QString strsql;

    soob1.setNum(nz);
    strsql="UPDATE "+naz_table+" SET "+naz_col+" = '"+zn+"' WHERE nz="+soob1;
    if (! query.exec(strsql))
    {
     soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
     WriteLog(soob);
     if (prsoob)
     {
      QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
     }
     return false;
    }

 return true;
}

bool ClassBd::UpdateRow(QString naz_table,QString naz_col,QString zn,qlonglong nz,bool prsoob)
{
    QSqlQuery query;
    bool ret;

    ret=UpdateRow(naz_table,naz_col,zn,nz,prsoob,query);

 return ret;
}

QList<qlonglong> ClassBd::FindRowForEdvidDs(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;
 QString str1,str2,strtip;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if ((!zapzapr.ds) && (!zapzapr.dzs) && (!zapzapr.kos)) return list_id;

 strsql="SELECT ds.nz FROM ds,edxr WHERE ds.nz_edxr = edxr.nz";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");

 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 str1=" AND ("; str2=")";

 if (zapzapr.ds) strtip="ds.nazis = 'DS' OR ds.nazis = 'SPO'";
 if (zapzapr.dzs)
 {
  if (strtip.isEmpty()) strtip="ds.nazis = 'DZS'";
  else strtip=strtip+" OR ds.nazis = 'DZS'";
 }
 if (zapzapr.kos)
 {
  if (strtip.isEmpty()) strtip="ds.nazis = 'KOS' OR ds.nazis = 'FRD'";
  else strtip=strtip+" OR ds.nazis = 'KOS' OR ds.nazis = 'FRD'";
 }

 if (strtip.isEmpty()) return list_id;

 strsql=strsql+str1+strtip+str2;

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidDs(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidDs(nz_zapr,prsoob,query);

 return list_id;
}

bool ClassBd::ReadDs(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("ds","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapds.nz=spis_field[0].toLongLong();
 zapds.naz=spis_field[1].toString();
 zapds.npn=spis_field[2].toString();
 zapds.godn=spis_field[3].toInt();
 zapds.godk=spis_field[4].toInt();
 zapds.sutn=spis_field[5].toInt();
 zapds.sutk=spis_field[6].toInt();
 zapds.sekn=spis_field[7].toLongLong();
 zapds.sekk=spis_field[8].toLongLong();
 zapds.nka=spis_field[9].toString();
 zapds.nkis=spis_field[10].toString();
 zapds.dtn=spis_field[11].toString();
 zapds.dtk=spis_field[12].toString();
 zapds.sv=spis_field[13].toInt();
 zapds.nazis=spis_field[14].toString();
 zapds.nz_edxr=spis_field[15].toLongLong();

 return true;
}

bool ClassBd::ReadDs(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadDs(nz,prsoob,query);

 return ret;
}

QList<qlonglong> ClassBd::FindRowForEdvidDds(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;
 QString str1,str2,strtip;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if ((!zapzapr.kvi) && (!zapzapr.kvno) && (!zapzapr.alti) && (!zapzapr.cal)) return list_id;

 strsql="SELECT dds.nz FROM dds,edxr WHERE dds.nz_edxr = edxr.nz AND dds.strk = 0 AND dds.kodobr = 0";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 str1=" AND ("; str2=")";

 if (zapzapr.kvi) strtip="dds.tip = 'KVI'";
 if (zapzapr.kvno)
 {
  if (strtip.isEmpty()) strtip="dds.tip = 'KVNO'";
  else strtip=strtip+" OR dds.tip = 'KVNO'";
 }
 if (zapzapr.alti)
 {
  if (strtip.isEmpty()) strtip="dds.tip = 'ALTI'";
  else strtip=strtip+" OR dds.tip = 'ALTI'";
 }
 if (zapzapr.cal)
 {
  if (strtip.isEmpty()) strtip="dds.tip = 'CAL1' OR dds.tip = 'CAL2'";
  else strtip=strtip+" OR dds.tip = 'CAL1' OR dds.tip = 'CAL2'";
 }

 if (strtip.isEmpty()) return list_id;

 strsql=strsql+str1+strtip+str2;

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidDds(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidDds(nz_zapr,prsoob,query);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidPpsi(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if (!zapzapr.ppsi) return list_id;

 strsql="SELECT fullppsi.nz FROM fullppsi,edxr WHERE fullppsi.nz_edxr = edxr.nz";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidPpsi(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidPpsi(nz_zapr,prsoob,query);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidRnxi(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;
 QString str1,str2,strtip;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if ((!zapzapr.meteo) && (!zapzapr.bds_l)) return list_id;

 strsql="SELECT rnxi.nz FROM rnxi,edxr WHERE rnxi.nz_edxr = edxr.nz";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 str1=" AND ("; str2=")";

 if (zapzapr.meteo) strtip="rnxi.tip = 'M'";
 if (zapzapr.bds_l)
 {
  if (strtip.isEmpty()) strtip="rnxi.tip = 'G' OR rnxi.tip = 'N' OR rnxi.tip = 'O' OR rnxi.tip = 'L'";
  else strtip=strtip+" OR rnxi.tip = 'G' OR rnxi.tip = 'N' OR rnxi.tip = 'O' OR rnxi.tip = 'L'";
 }

 if (strtip.isEmpty()) return list_id;

 strsql=strsql+str1+strtip+str2;

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidRnxi(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidRnxi(nz_zapr,prsoob,query);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidRnxv(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;
 QString str1,str2,strtip;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if ((!zapzapr.bds_o) && (!zapzapr.bds_n)) return list_id;

 strsql="SELECT rnxv.nz FROM rnxv,edxr WHERE rnxv.nz_edxr = edxr.nz AND rnxv.kodobr = 0 AND rnxv.strk = 0";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 if (!zapzapr.pr_avt)
 {
  strdn=zapzapr.dtn.toString("dd-MM-yyyy");
 }

 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 str1=" AND ("; str2=")";

 if (zapzapr.bds_o) strtip="rnxv.tip = 'O'";
 if (zapzapr.bds_n)
 {
  if (strtip.isEmpty()) strtip="rnxv.tip = 'G' OR rnxv.tip = 'N'";
  else strtip=strtip+" OR rnxv.tip = 'G' OR rnxv.tip = 'N'";
 }

 if (strtip.isEmpty()) return list_id;

 strsql=strsql+str1+strtip+str2;

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidRnxv(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidRnxv(nz_zapr,prsoob,query);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvid(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;

 strsql="SELECT edvid.nz FROM edvid WHERE edvid.nz_zapr = "+QString::number(nz_zapr)+" AND edvid.dvid IS NULL";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvid(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvid(nz_zapr,prsoob,query);

 return list_id;
}

bool ClassBd::DelEdvid(quint64 nz_zapr,bool prsoob)
{
 QSqlQuery query;
 QString strsql,soob;

 strsql="DELETE FROM edvid WHERE nz_zapr = "+QString::number(nz_zapr);

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelTable(QString ntable,bool prsoob,QSqlQuery query)
{
 QString strsql,soob;

 strsql="DELETE FROM "+ntable;

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::DelTable(QString ntable,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=DelTable(ntable,prsoob,query);

 return ret;
}

quint64 ClassBd::InsertAlt(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(datn,vrn,prod,reg,nvit,datk,vrk,nfile,nka)";
 val="('"+zapalt.datn.toString("dd-MM-yyyy")+"','"+zapalt.vrn.toString("hh:mm:ss")+"',"+QString::number(zapalt.prod);
 val=val+","+QString::number(zapalt.reg)+","+QString::number(zapalt.nvit)+",'"+zapalt.datk.toString("dd-MM-yyyy")+"'";
 val=val+",'"+zapalt.vrk.toString("hh:mm:ss")+"','"+zapalt.nfile+"','"+zapalt.nka+"')";
 strsql="INSERT INTO alt "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertAlt(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertAlt(prsoob,query);

 return nz;
}

bool ClassBd::ReadAlt(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("alt","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapalt.nz=spis_field[0].toLongLong();
 zapalt.datn=spis_field[1].toDate();
 zapalt.vrn=spis_field[2].toTime();
 zapalt.prod=spis_field[3].toLongLong();
 zapalt.reg=spis_field[4].toInt();
 zapalt.nvit=spis_field[5].toLongLong();
 zapalt.datk=spis_field[6].toDate();
 zapalt.vrk=spis_field[7].toTime();
 zapalt.nfile=spis_field[8].toString();
 zapalt.nka=spis_field[9].toString();

 return true;
}

bool ClassBd::ReadAlt(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadAlt(nz,prsoob,query);

 return ret;
}

void ClassBd::ClearAlt()
{
 QDate dat;
 QTime tm;
 dat.setDate(1900,1,1);
 tm.setHMS(0,0,0);

 zapalt.nz=0;
 zapalt.datn.setDate(1900,1,1);
 zapalt.vrn.setHMS(0,0,0);
 zapalt.prod=0;
 zapalt.reg=0;
 zapalt.nvit=0;
 zapalt.datk.setDate(1900,1,1);
 zapalt.vrk.setHMS(0,0,0);
 zapalt.nfile.clear();
 zapalt.nka.clear();
 return;
}

bool ClassBd::ReadStatVrv(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("statvrv","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapstatvrv.nz=spis_field[0].toLongLong();
 zapstatvrv.nz_dds=spis_field[1].toLongLong();
 zapstatvrv.dat=spis_field[2].toDate();
 zapstatvrv.plan=spis_field[3].toInt();
 zapstatvrv.vip=spis_field[4].toInt();
 zapstatvrv.vne=spis_field[5].toInt();
 zapstatvrv.nevip=spis_field[6].toInt();
 zapstatvrv.noplan=spis_field[7].toInt();

 return true;
}

bool ClassBd::ReadStatVrv(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadStatVrv(nz,prsoob,query);

 return ret;
}

quint64 ClassBd::InsertStatrvrv(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(nz_statvrv,tm,kolpak,reg)";
 val="("+QString::number(zapstatrvrv.nz_statvrv)+",'"+zapstatrvrv.tm.toString("hh:mm:ss")+"',"+QString::number(zapstatrvrv.kolpak)+","+QString::number(zapstatrvrv.reg)+")";
 strsql="INSERT INTO statrvrv "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertStatrvrv(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertStatrvrv(prsoob,query);

 return nz;
}

void ClassBd::ClearStatrvrv()
{
 QDate dat;
 QTime tm;
 dat.setDate(1900,1,1);
 tm.setHMS(0,0,0);

 zapstatrvrv.nz=0;
 zapstatrvrv.nz_statvrv=0;
 zapstatrvrv.tm.setHMS(0,0,0);
 zapstatrvrv.kolpak=0;
 zapstatrvrv.reg=0;
 return;
}

bool ClassBd::UpdateRowStatFull(quint64 zn,QString naz,quint64 nz_fullppsi,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE statfull SET "+naz+" = "+QString::number(zn);
 strsql=strsql+" WHERE nz_fullppsi="+QString::number(nz_fullppsi);
 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateRowStatFull(quint64 zn,QString naz,quint64 nz_fullppsi,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRowStatFull(zn,naz,nz_fullppsi,prsoob,query);

 return ret;
}

bool ClassBd::CreateBD()
{
 QString str,soob;
 QSqlError err;
 QSqlQuery query;

 if (os == "WIN")
 {
  str="CREATE DATABASE geoik WITH OWNER = postgres ENCODING = 'UTF8' TABLESPACE = pg_default "
      "LC_COLLATE = 'Russian_Russia.1251' LC_CTYPE = 'Russian_Russia.1251' CONNECTION LIMIT = -1";
 }
 else
 {
  str="CREATE DATABASE geoik WITH OWNER = postgres ENCODING = 'UTF8' TABLESPACE = pg_default "
      "LC_COLLATE = 'ru_RU.UTF-8' LC_CTYPE = 'ru_RU.UTF-8' CONNECTION LIMIT = -1";
 }
 if (! query.exec(str))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 str="GRANT ALL ON DATABASE geoik TO postgres";
 if (! query.exec(str))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return false;
 }

 if (!createConnectionPSQL("geoik",nikp,passp,hostp,err)) return false;

 str="CREATE TABLE alt (nz bigint NOT NULL,datn date,vrn time without time zone,prod bigint,"
     "reg integer,nvit bigint,datk date,vrk time without time zone,nfile character varying,nka character varying)";
 db.exec(str);
 str="ALTER TABLE public.alt OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE alt_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.alt_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE alt_nz_seq OWNED BY alt.nz";
 db.exec(str);
 str="ALTER TABLE ONLY alt ALTER COLUMN nz SET DEFAULT nextval('alt_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY alt ADD CONSTRAINT p_alt PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY alt ADD CONSTRAINT u_alt UNIQUE (datn, vrn)";
 db.exec(str);
 str="GRANT ALL ON alt TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON alt_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE dds (nz bigint NOT NULL,naz character varying,tip character varying,godn integer,"
     "godk integer,sutn integer,sutk integer,sekn bigint,sekk bigint,nka character varying,dtn character varying,"
     "dtk character varying,strk integer,kodobr integer,nz_edxr bigint)";
 db.exec(str);
 str="ALTER TABLE public.dds OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE dds_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.dds_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE dds_nz_seq OWNED BY dds.nz";
 db.exec(str);
 str="ALTER TABLE ONLY dds ALTER COLUMN nz SET DEFAULT nextval('dds_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY dds ADD CONSTRAINT p_dds PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON dds TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON dds_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE ds (nz bigint NOT NULL,naz character varying,npn character varying,godn integer,godk integer,"
     "sutn integer,sutk integer,sekn bigint,sekk bigint,nka character varying,nkis character varying,"
     "dtn character varying,dtk character varying,sv integer,nazis character varying,nz_edxr bigint)";
 db.exec(str);
 str="ALTER TABLE public.ds OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE ds_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.ds_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE ds_nz_seq OWNED BY ds.nz";
 db.exec(str);
 str="ALTER TABLE ONLY ds ALTER COLUMN nz SET DEFAULT nextval('ds_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY ds ADD CONSTRAINT p_ds PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY ds ADD CONSTRAINT u_ds UNIQUE (naz, npn, godn, sutn, sekn, nazis)";
 db.exec(str);
 str="GRANT ALL ON ds TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON ds_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE err (kod integer NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.err OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY err ADD CONSTRAINT p_err PRIMARY KEY (kod)";
 db.exec(str);
 str="INSERT INTO err (kod,opis) VALUES (1,'Не найдено ни одного маркера синхронизации.')";
 db.exec(str);
 str="INSERT INTO err (kod,opis) VALUES (2,'Ошибка при объединении КВИ ДДС.')";
 db.exec(str);
 str="INSERT INTO err (kod,opis) VALUES (3,'Ошибка при объединении навиг.файлов.')";
 db.exec(str);
 str="GRANT ALL ON err TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE fullppsi (nz bigint NOT NULL,naz character varying,kan integer,godn integer,godk integer,"
     "sutn integer,sutk integer,sekn bigint,sekk bigint,nka character varying,dtn character varying,"
     "dtk character varying,strk integer,kodobr integer,nz_edxr bigint,npn character varying)";
 db.exec(str);
 str="ALTER TABLE public.fullppsi OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE fullppsi_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.fullppsi_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE fullppsi_nz_seq OWNED BY fullppsi.nz";
 db.exec(str);
 str="ALTER TABLE ONLY fullppsi ALTER COLUMN nz SET DEFAULT nextval('fullppsi_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY fullppsi ADD CONSTRAINT p_fullppsi PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY fullppsi ADD CONSTRAINT u_fullppsi UNIQUE (naz, kan, godn, sutn, sekn, nka, npn)";
 db.exec(str);
 str="GRANT ALL ON fullppsi TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON fullppsi_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE kodalt (nz integer NOT NULL,naz character varying)";
 db.exec(str);
 str="ALTER TABLE public.kodalt OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY kodalt ADD CONSTRAINT p_kodalt PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (0,'нет плана')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (1,'дежурный')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (2,'слежение за эхо-сигналом')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (3,'калибровка 1')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (4,'калибровка 1-AGC')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (5,'калибровка 2-УК')";
 db.exec(str);
 str="INSERT INTO kodalt (nz,naz) VALUES (6,'калибровка 2-УД')";
 db.exec(str);
 str="GRANT ALL ON kodalt TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE kodba (nz bigint NOT NULL,naz character varying)";
 db.exec(str);
 str="ALTER TABLE public.kodba OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY kodba ADD CONSTRAINT p_kodba PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO kodba (nz,naz) VALUES (0,'не определено')";
 db.exec(str);
 str="INSERT INTO kodba (nz,naz) VALUES (1,'ДДС')";
 db.exec(str);
 str="INSERT INTO kodba (nz,naz) VALUES (2,'ВРВ')";
 db.exec(str);
 str="INSERT INTO kodba (nz,naz) VALUES (3,'АСПСИ')";
 db.exec(str);
 str="INSERT INTO kodba (nz,naz) VALUES (4,'коррекция')";
 db.exec(str);
 str="GRANT ALL ON kodba TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE kodobr (nz bigint NOT NULL,soob character varying)";
 db.exec(str);
 str="ALTER TABLE public.kodobr OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY kodobr ADD CONSTRAINT p_kodobr PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (0,'Обработка завершена.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (1,'Ошибка данных. Дальнейшая обработка прекращена.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (2,'Переименовать файл измерений БДС.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (3,'Разбить файл измерений БДС по суткам.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (4,'Объединить файл измерений БДС в суточный.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (5,'Обработка завершена.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (6,'Удалить из БД.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (7,'Разделить навигационный файл БДС по ГНСС.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (8,'Разбить навигационный файл БДС по суткам.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (9,'Объединить навигационные файлы БДС в суточный.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (10,'Выполнить конвертацию файла (*.*L) БДС.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (11,'Выполнить конвертацию FULL файла: КВИ и КВНО.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (12,'Выполнить конвертацию FULL файла: ВРВ.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (13,'Объединить файл КВИ (*.*O) в суточный.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (14,'Разбить файл КВИ (*.*O) по суткам.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (15,'Выполнить конвертацию FULL файла: КВНО.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (16,'Объединить файл КВНО (*.*K) в суточный.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (17,'Разбить файл КВНО (*.*K) по суткам.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (18,'Объединить файл ВРВ (ALTI*.*) в суточный.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (19,'Разбить файл ВРВ (ALTI*.*) по суткам.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (20,'Удалить из БД.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (21,'Выполнить конвертирование в RINEX 3.02.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (22,'Выполнить конвертирование в RINEX 3.02.')";
 db.exec(str);
 str="INSERT INTO kodobr (nz,soob) VALUES (23,'Поместить в БД.')";
 db.exec(str);
 str="GRANT ALL ON kodobr TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE mes (nz integer NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.mes OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY mes ADD CONSTRAINT p_mes PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (1,'январь')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (2,'февраль')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (3,'март')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (4,'апрель')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (5,'май')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (6,'июнь')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (7,'июль')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (8,'август')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (9,'сентябрь')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (10,'октябрь')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (11,'ноябрь')";
 db.exec(str);
 str="INSERT INTO mes (nz,opis) VALUES (12,'декабрь')";
 db.exec(str);
 str="GRANT ALL ON mes TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE nos (nz bigint NOT NULL,naz character varying,prblok smallint DEFAULT 0)";
 db.exec(str);
 str="ALTER TABLE public.nos OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE nos_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.nos_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE nos_nz_seq OWNED BY nos.nz";
 db.exec(str);
 str="ALTER TABLE ONLY nos ALTER COLUMN nz SET DEFAULT nextval('nos_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY nos ADD CONSTRAINT idx_nz_nos PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY nos ADD CONSTRAINT naz_nos UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON nos TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON nos_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE kodnos (nz integer NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.kodnos OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY kodnos ADD CONSTRAINT p_kodnos PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO kodnos (nz,opis) VALUES (0,'активный')";
 db.exec(str);
 str="INSERT INTO kodnos (nz,opis) VALUES (1,'заблокирован')";
 db.exec(str);
 str="GRANT ALL ON kodnos TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE edxr (nz bigint NOT NULL,isxf character varying,arxf character varying,idlf bigint,iks bigint,"
     "adlf bigint,aks bigint,datp date,datr date,prarx smallint DEFAULT 0,nz_nos bigint)";
 db.exec(str);
 str="ALTER TABLE public.edxr OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE edxr_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.edxr_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE edxr_nz_seq OWNED BY edxr.nz";
 db.exec(str);
 str="ALTER TABLE ONLY edxr ALTER COLUMN nz SET DEFAULT nextval('edxr_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY edxr ADD CONSTRAINT pk_edxr PRIMARY KEY (nz)";
 db.exec(str);
 str="CREATE INDEX fki_s_edxr ON edxr USING btree (nz_nos)";
 db.exec(str);
 str="ALTER TABLE ONLY edxr ADD CONSTRAINT s_edxr FOREIGN KEY (nz_nos) REFERENCES nos(nz) ON DELETE RESTRICT";
 db.exec(str);
 str="GRANT ALL ON edxr TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON edxr_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE obr (naz_komp character varying,nz_edxr bigint NOT NULL)";
 db.exec(str);
 str="ALTER TABLE public.obr OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY obr ADD CONSTRAINT p_obr PRIMARY KEY (nz_edxr)";
 db.exec(str);
 str="GRANT ALL ON obr TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE potr (nz bigint NOT NULL,naz character varying)";
 db.exec(str);
 str="ALTER TABLE public.potr OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE potr_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.potr_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE potr_nz_seq OWNED BY potr.nz";
 db.exec(str);
 str="ALTER TABLE ONLY potr ALTER COLUMN nz SET DEFAULT nextval('potr_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY potr ADD CONSTRAINT p_potr PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY potr ADD CONSTRAINT u_potr UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON potr TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON potr_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE rnxi (nz bigint NOT NULL,naz character varying,ver character varying,"
     "npn character varying,tip character varying,godn integer,godk integer,sutn integer,"
     "sutk integer,sekn bigint,sekk bigint,dtn character varying,dtk character varying,"
     "strk integer,kodobr integer,interv integer,nz_edxr bigint)";
 db.exec(str);
 str="ALTER TABLE public.rnxi OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE rnxi_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.rnxi_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE rnxi_nz_seq OWNED BY rnxi.nz";
 db.exec(str);
 str="ALTER TABLE ONLY rnxi ALTER COLUMN nz SET DEFAULT nextval('rnxi_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY rnxi ADD CONSTRAINT p_rnxi PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY rnxi ADD CONSTRAINT u_rnxi UNIQUE (naz, ver, npn, tip, godn, sutn, sekn)";
 db.exec(str);
 str="GRANT ALL ON rnxi TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON rnxi_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE rnxv (nz bigint NOT NULL,naz character varying,ver character varying,"
     "npn character varying,tip character varying,godn integer,godk integer,sutn integer,"
     "sutk integer,sekn bigint,sekk bigint,dtn character varying,dtk character varying,"
     "strk integer,kodobr integer,interv integer,nz_edxr bigint)";
 db.exec(str);
 str="ALTER TABLE public.rnxv OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE rnxv_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.rnxv_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE rnxv_nz_seq OWNED BY rnxv.nz";
 db.exec(str);
 str="ALTER TABLE ONLY rnxv ALTER COLUMN nz SET DEFAULT nextval('rnxv_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY rnxv ADD CONSTRAINT p_rnxv PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON rnxv TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON rnxv_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sba (nz bigint NOT NULL,nz_kodba bigint,dvik date,vvik time without time zone,"
     "dvkl date,vvkl time without time zone,nka integer)";
 db.exec(str);
 str="ALTER TABLE public.sba OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sba_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sba_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sba_nz_seq OWNED BY sba.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sba ALTER COLUMN nz SET DEFAULT nextval('sba_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sba ADD CONSTRAINT p_sba PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON sba TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sba_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE strk (nz smallint NOT NULL,sod character varying)";
 db.exec(str);
 str="ALTER TABLE public.strk OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY strk ADD CONSTRAINT p_strk PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (0,'без ошибок.')";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (1,'нет маркера синхронизации.')";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (2,'ошибка структурного контроля.')";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (3,'ошибка при объединении навигац.файла.')";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (4,'не сформирован файл ВРВ.')";
 db.exec(str);
 str="INSERT INTO strk (nz,sod) VALUES (5,'некорректные дата/время в записях эпох.')";
 db.exec(str);
 str="GRANT ALL ON strk TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sv (nz integer NOT NULL,opisan character varying)";
 db.exec(str);
 str="ALTER TABLE public.sv OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY sv ADD CONSTRAINT p_sv PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (0,'не определено')";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (1,'UTC(USNO)')";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (2,'UTC(SU)')";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (3,'ДМВ')";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (4,'UTC(BIH)')";
 db.exec(str);
 str="INSERT INTO sv (nz,opisan) VALUES (5,'UTC(GPS)')";
 db.exec(str);
 str="GRANT ALL ON sv TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sxema (nz bigint NOT NULL,nz_educh bigint,nz_educho1 bigint,nz_educho2 bigint,pr_isx integer)";
 db.exec(str);
 str="ALTER TABLE public.sxema OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sxema_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sxema_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sxema_nz_seq OWNED BY sxema.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sxema ALTER COLUMN nz SET DEFAULT nextval('sxema_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sxema ADD CONSTRAINT p_sxema PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON sxema TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sxema_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sxema1 (nz bigint NOT NULL,nz_educh bigint,nz_educho1 bigint,nz_educho2 bigint,pr_isx integer)";
 db.exec(str);
 str="ALTER TABLE public.sxema1 OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sxema1_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sxema1_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sxema1_nz_seq OWNED BY sxema1.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sxema1 ALTER COLUMN nz SET DEFAULT nextval('sxema1_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sxema1 ADD CONSTRAINT p_sxema1 PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON sxema1 TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sxema1_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE tipedvid (nz integer NOT NULL,tip character varying,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.tipedvid OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY tipedvid ADD CONSTRAINT p_tipedvid PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY tipedvid ADD CONSTRAINT u_tipedvid UNIQUE (tip)";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (1,'ALTI','Суточный файл измерений ВРВ')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (2,'DS','Сеанс измерительной информации ДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (3,'SPO','Файл информации кадров СПО и ТЭ ЦИ')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (4,'DZS','Сеанс измерительной информации ДЗС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (5,'KOS','Сеанс измерительной информации КОС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (6,'FRD','Сеанс измерительной информации КОС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (7,'FULL','Сеанс ППСИ')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (8,'KVI','Суточный файл измерений ДДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (9,'KVNO','Суточный файл навигационных определений ДДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (10,'CAL1','Суточный файл калибровок 1-го типа ВРВ')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (11,'CAL2','Суточный файл калибровок 2-го типа ВРВ')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (12,'I_M','Сеанс информации метеоданных')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (13,'I_G','Необработанный навигационный файл БДС измерений ГЛОНАСС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (14,'I_N','Необработанный навигационный файл БДС измерений GPS')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (15,'I_O','Необработанный файл измерений БДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (16,'I_L','Необработанный файл измерений БДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (17,'V_O','Суточный файл измерений БДС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (18,'V_G','Объединённый навигационный файл БДС измерений ГЛОНАСС')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (19,'V_N','Объединённый навигационный файл БДС измерений GPS')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (20,'SP3','Файл эфемеридно-временной информации')";
 db.exec(str);
 str="INSERT INTO tipedvid (nz,tip,opis) VALUES (21,'SOL','Протокол оценки качества измерительной информации ВРВ')";
 db.exec(str);
 str="GRANT ALL ON tipedvid TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE ust (nz bigint NOT NULL,nazkomp character varying,path_temp character varying,"
     "path_si character varying,pravt boolean,km integer,pr_obrsi boolean,pr_obr2 boolean,pr_obr3 boolean,"
     "pr_obr4 boolean,pr_obr7 boolean,pr_obr8 boolean,pr_obr9 boolean,pr_obr10 boolean,pr_obr11_15 boolean,"
     "pr_obr12 boolean,pr_obr14 boolean,pr_obr13 boolean,pr_obr17 boolean,pr_obr16 boolean,pr_obr19 boolean,"
     "pr_obr18 boolean,kol_obrsi integer,kol_obr2 integer,kol_obr3 integer,kol_obr4 integer,kol_obr7 integer,"
     "kol_obr8 integer,kol_obr9 integer,kol_obr10 integer,kol_obr11_15 integer,kol_obr12 integer,kol_obr14 integer,"
     "kol_obr13 integer,kol_obr17 integer,kol_obr16 integer,kol_obr19 integer,kol_obr18 integer,pr_obrzapr integer,path_err character varying)";
 db.exec(str);
 str="ALTER TABLE public.ust OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE ust_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.ust_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE ust_nz_seq OWNED BY ust.nz";
 db.exec(str);
 str="ALTER TABLE ONLY ust ALTER COLUMN nz SET DEFAULT nextval('ust_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY ust ADD CONSTRAINT p_ust PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON ust TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON ust_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE stat313 (nz bigint NOT NULL,nz_educh bigint,kizm integer,dlsek integer,kizmn integer,dlsekn integer,"
     "shf1max integer,shf1min integer,shf1med integer,shf2max integer,shf2min integer,shf2med integer,"
     "kizmnf1 integer,kizmnf2 integer)";
 db.exec(str);
 str="ALTER TABLE public.stat313 OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE stat313_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.stat313_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE stat313_nz_seq OWNED BY stat313.nz";
 db.exec(str);
 str="ALTER TABLE ONLY stat313 ALTER COLUMN nz SET DEFAULT nextval('stat313_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY stat313 ADD CONSTRAINT p_stat313 PRIMARY KEY (nz)";
 db.exec(str);
 str="CREATE INDEX fki_p_313 ON stat313 USING btree (nz_educh)";
 db.exec(str);
 str="ALTER TABLE ONLY stat313 ADD CONSTRAINT k_stat313 FOREIGN KEY (nz_educh) REFERENCES ds(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON stat313 TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON stat313_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE stat322 (nz bigint NOT NULL,nz_educh bigint,kizm integer,dlsek integer,kizmn integer,dlsekn integer,"
     "shrcmax integer,shrcmin integer,shrcmed integer,shpsmax integer,shpsmin integer,shpsmed integer)";
 db.exec(str);
 str="ALTER TABLE public.stat322 OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE stat322_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.stat322_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE stat322_nz_seq OWNED BY stat322.nz";
 db.exec(str);
 str="ALTER TABLE ONLY stat322 ALTER COLUMN nz SET DEFAULT nextval('stat322_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY stat322 ADD CONSTRAINT p_322 PRIMARY KEY (nz)";
 db.exec(str);
 str="CREATE INDEX fki_k_stat322 ON stat322 USING btree (nz_educh)";
 db.exec(str);
 str="ALTER TABLE ONLY stat322 ADD CONSTRAINT k_stat322 FOREIGN KEY (nz_educh) REFERENCES ds(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON stat322 TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON stat322_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statdds (nz bigint NOT NULL,dat date,eppl integer,eppol integer,ep_4mglo integer,ep_4rglo integer,"
     "ep_4bglo integer,nz_dds bigint,mas_ep integer[])";
 db.exec(str);
 str="ALTER TABLE public.statdds OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statdds_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statdds_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statdds_nz_seq OWNED BY statdds.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statdds ALTER COLUMN nz SET DEFAULT nextval('statdds_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statdds ADD CONSTRAINT p_statdds PRIMARY KEY (nz)";
 db.exec(str);
 str="CREATE INDEX fki_k_statdds ON statdds USING btree (nz_dds)";
 db.exec(str);
 str="ALTER TABLE ONLY statdds ADD CONSTRAINT k_statdds FOREIGN KEY (nz_dds) REFERENCES dds(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statdds TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statdds_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statfull (nz bigint NOT NULL,k bigint DEFAULT 0,kbm bigint DEFAULT 0,crc bigint DEFAULT 0,"
     "pk bigint DEFAULT 0,vrv1 bigint DEFAULT 0,vrv2 bigint DEFAULT 0,kvi bigint DEFAULT 0,kvno bigint DEFAULT 0,"
     "rez bigint DEFAULT 0,opk bigint DEFAULT 0,ovrv1 bigint DEFAULT 0,ovrv2 bigint DEFAULT 0,okvi bigint DEFAULT 0,"
     "okvno bigint DEFAULT 0,orez bigint DEFAULT 0,izm_kvi bigint DEFAULT 0,k_kvi bigint DEFAULT 0,crc_kvi bigint DEFAULT 0,"
     "och_kvi bigint DEFAULT 0,k_kvno bigint DEFAULT 0,crc_kvno bigint DEFAULT 0,och_kvno bigint DEFAULT 0,"
     "k1_vrv1 bigint DEFAULT 0,k2_vrv1 bigint DEFAULT 0,k1_vrv2 bigint DEFAULT 0,k2_vrv2 bigint DEFAULT 0,"
     "crc1_vrv1 bigint DEFAULT 0,crc2_vrv1 bigint DEFAULT 0,crc1_vrv2 bigint DEFAULT 0,crc2_vrv2 bigint DEFAULT 0,"
     "och1_vrv1 bigint DEFAULT 0,och2_vrv1 bigint DEFAULT 0,och1_vrv2 bigint DEFAULT 0,och2_vrv2 bigint DEFAULT 0,"
     "k_vrv bigint DEFAULT 0,nz_fullppsi bigint DEFAULT 0,k_cal bigint DEFAULT 0,dn_kvi date,dk_kvi date,dn_kvno date,dk_kvno date,dn_vrv date,dk_vrv date)";
 db.exec(str);
 str="ALTER TABLE public.statfull OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statfull_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statfull_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statfull_nz_seq OWNED BY statfull.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statfull ALTER COLUMN nz SET DEFAULT nextval('statfull_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statfull ADD CONSTRAINT p_statfull PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY statfull ADD CONSTRAINT u_statfull UNIQUE (nz_fullppsi)";
 db.exec(str);
 str="ALTER TABLE ONLY statfull ADD CONSTRAINT s_statppsi FOREIGN KEY (nz_fullppsi) REFERENCES fullppsi(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statfull TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statfull_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statvrv (nz bigint NOT NULL,nz_dds bigint,dat date,plan integer,vip integer,vne integer,"
     "nevip integer,noplan integer)";
 db.exec(str);
 str="ALTER TABLE public.statvrv OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statvrv_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statvrv_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statvrv_nz_seq OWNED BY statvrv.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statvrv ALTER COLUMN nz SET DEFAULT nextval('statvrv_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statvrv ADD CONSTRAINT p_nz PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY statvrv ADD CONSTRAINT u_statvrv UNIQUE (nz_dds)";
 db.exec(str);
 str="CREATE INDEX fki_k_statvrv ON statvrv USING btree (nz_dds)";
 db.exec(str);
 str="ALTER TABLE ONLY statvrv ADD CONSTRAINT k_statvrv FOREIGN KEY (nz_dds) REFERENCES dds(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statvrv TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statvrv_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE zapr (nz bigint NOT NULL,nz_potr bigint,pr_avt boolean,dat timestamp without time zone,ds boolean,"
     "dzs boolean,kos boolean,meteo boolean,ppsi boolean,kvi boolean,kvno boolean,alti boolean,cal boolean,bds_l boolean,"
     "bds_o boolean,bds_n boolean,naz character varying,rnx2 boolean,path character varying,dtn date,dtk date,dtt date,"
     "pr_alti boolean,kolsut integer,pr_dat integer,komp character varying,sp3 boolean,sol boolean)";
 db.exec(str);
 str="ALTER TABLE public.zapr OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE zapr_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.zapr_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE zapr_nz_seq OWNED BY zapr.nz";
 db.exec(str);
 str="ALTER TABLE ONLY zapr ALTER COLUMN nz SET DEFAULT nextval('zapr_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY zapr ADD CONSTRAINT p_zapr PRIMARY KEY (nz)";
 db.exec(str);
 str="CREATE INDEX fki_k_zapr ON zapr USING btree (nz_potr)";
 db.exec(str);
 str="ALTER TABLE ONLY zapr ADD CONSTRAINT k_zapr FOREIGN KEY (nz_potr) REFERENCES potr(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON zapr TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON zapr_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE edvid (nz bigint NOT NULL,nz_zapr bigint,nz_edxr bigint,naz_edxr character varying,"
     "dvid timestamp without time zone,tip character varying)";
 db.exec(str);
 str="ALTER TABLE public.edvid OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE edvid_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.edvid_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE edvid_nz_seq OWNED BY edvid.nz";
 db.exec(str);
 str="ALTER TABLE ONLY edvid ALTER COLUMN nz SET DEFAULT nextval('edvid_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY edvid ADD CONSTRAINT p_edvid PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY edvid ADD CONSTRAINT u_edvid UNIQUE (nz_zapr,nz_edxr)";
 db.exec(str);
 str="CREATE INDEX fki_k_edvid ON edvid USING btree (nz_zapr)";
 db.exec(str);
 str="ALTER TABLE ONLY edvid ADD CONSTRAINT k_edvid FOREIGN KEY (nz_zapr) REFERENCES zapr(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON edvid TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON edvid_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statrdds (nz bigint NOT NULL,nz_statdds bigint,vrn time without time zone,"
     "vrr time without time zone,kolep integer)";
 db.exec(str);
 str="ALTER TABLE public.statrdds OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statrdds_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statrdds_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statrdds_nz_seq OWNED BY statrdds.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statrdds ALTER COLUMN nz SET DEFAULT nextval('statrdds_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statrdds ADD CONSTRAINT p_statrdds PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY statrdds ADD CONSTRAINT u_statrdds UNIQUE (nz_statdds, vrn)";
 db.exec(str);
 str="CREATE INDEX fki_i1 ON statrdds USING btree (nz_statdds)";
 db.exec(str);
 str="ALTER TABLE ONLY statrdds ADD CONSTRAINT k_statrdds FOREIGN KEY (nz_statdds) REFERENCES statdds(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statrdds TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statrdds_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statrvrv (nz bigint NOT NULL,nz_statvrv bigint,tm time without time zone,kolpak integer,reg integer)";
 db.exec(str);
 str="ALTER TABLE public.statrvrv OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statrvrv_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statrvrv_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statrvrv_nz_seq OWNED BY statrvrv.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statrvrv ALTER COLUMN nz SET DEFAULT nextval('statrvrv_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statrvrv ADD CONSTRAINT p_statrvrv PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY statrvrv ADD CONSTRAINT s_statrvrv FOREIGN KEY (nz_statvrv) REFERENCES statvrv(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statrvrv TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statrvrv_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE serv (nz bigint NOT NULL,isdir character varying,vixdir character varying,copdir character varying,"
     "opis character varying,maska character varying,kobr integer,karx integer,nazkomp character varying,prvd smallint,"
     "prcd smallint,maskvd character varying,maskcd character varying,sdir integer,sdirc integer)";
 db.exec(str);
 str="ALTER TABLE public.serv OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE serv_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.serv_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE serv_nz_seq OWNED BY serv.nz";
 db.exec(str);
 str="ALTER TABLE ONLY serv ALTER COLUMN nz SET DEFAULT nextval('serv_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY serv ADD CONSTRAINT p_serv PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON serv TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON serv_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE kobrserv (nz integer NOT NULL,soob character varying)";
 db.exec(str);
 str="ALTER TABLE public.kobrserv OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY kobrserv ADD CONSTRAINT p_kobrserv PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (0,'не определено')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (1,'скопировать до обработки')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (2,'скопировать после обработки')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (3,'скопировать до и после обработки')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (4,'удалить исходный')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (5,'скопировать до обработки, удалить исходный')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (6,'скопировать после обработки, удалить исходный')";
 db.exec(str);
 str="INSERT INTO kobrserv (nz,soob) VALUES (7,'скопировать до и после обработки, удалить исходный')";
 db.exec(str);
 str="GRANT ALL ON kobrserv TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE karx (nz integer NOT NULL,soob character varying)";
 db.exec(str);
 str="ALTER TABLE public.karx OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY karx ADD CONSTRAINT p_karx PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (0,'не определено')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (1,'распаковать архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (2,'распаковать Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (3,'распаковать архив и Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (4,'упаковать Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (5,'распаковать архив, упаковать Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (6,'распаковать и упаковать Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (7,'распаковать архив и Хатанака, упаковать Хатанака')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (8,'упаковать архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (9,'распаковать и упаковать архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (10,'распаковать Хатанака, упаковать архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (11,'распаковать архив и Хатанака, упаковать архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (12,'упаковать Хатанака и архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (13,'распаковать архив, упаковать Хатанака и архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (14,'распаковать Хатанака, упаковать Хатанака и архив')";
 db.exec(str);
 str="INSERT INTO karx (nz,soob) VALUES (15,'распаковать архив и Хатанака, упаковать Хатанака и архив')";
 db.exec(str);
 str="GRANT ALL ON karx TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE statserv (nz bigint NOT NULL,ifile character varying,dat date,vr time without time zone,nz_serv bigint)";
 db.exec(str);
 str="ALTER TABLE public.statserv OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE statserv_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.statserv_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE statserv_nz_seq OWNED BY statserv.nz";
 db.exec(str);
 str="ALTER TABLE ONLY statserv ALTER COLUMN nz SET DEFAULT nextval('statserv_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY statserv ADD CONSTRAINT p_statserv PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY statserv ADD CONSTRAINT s_statserv FOREIGN KEY (nz_serv) REFERENCES serv(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON statserv TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON statserv_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sist (nz bigint NOT NULL,dt date,vr time without time zone,nz_kodobr bigint,sost character varying,"
     "naz_komp character varying,polz character varying,naz_ed character varying)";
 db.exec(str);
 str="ALTER TABLE public.sist OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sist_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sist_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sist_nz_seq OWNED BY sist.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sist ALTER COLUMN nz SET DEFAULT nextval('sist_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sist ADD CONSTRAINT p_sist PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY sist ADD CONSTRAINT u_sist UNIQUE (sost, naz_ed)";
 db.exec(str);
 str="GRANT ALL ON sist TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sist_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE plppsi (nz bigint NOT NULL,naz character varying,godn integer,sutn integer,sekn bigint,nka integer,npn character varying,tip character varying)";
 db.exec(str);
 str="ALTER TABLE public.plppsi OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE plppsi_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.plppsi_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE plppsi_nz_seq OWNED BY plppsi.nz";
 db.exec(str);
 str="ALTER TABLE ONLY plppsi ALTER COLUMN nz SET DEFAULT nextval('plppsi_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY plppsi ADD CONSTRAINT p_plppsi PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON plppsi TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON plppsi_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE splppsi (nz bigint NOT NULL,sek bigint,az numeric,um numeric,nz_plppsi bigint)";
 db.exec(str);
 str="ALTER TABLE public.splppsi OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE splppsi_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.splppsi_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE splppsi_nz_seq OWNED BY splppsi.nz";
 db.exec(str);
 str="ALTER TABLE ONLY splppsi ALTER COLUMN nz SET DEFAULT nextval('splppsi_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY splppsi ADD CONSTRAINT p_splppsi PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY splppsi ADD CONSTRAINT k_splppsi FOREIGN KEY (nz_plppsi) REFERENCES plppsi(nz) ON DELETE CASCADE";
 db.exec(str);
 str="GRANT ALL ON splppsi TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON splppsi_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE cogi (nz bigint NOT NULL,naz character varying,tip integer,sost integer,dat date,vrem time without time zone,"
     "nz_edxr bigint,nz_cup bigint)";
 db.exec(str);
 str="ALTER TABLE public.cogi OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE cogi_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.cogi_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE cogi_nz_seq OWNED BY cogi.nz";
 db.exec(str);
 str="ALTER TABLE ONLY cogi ALTER COLUMN nz SET DEFAULT nextval('cogi_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY cogi ADD CONSTRAINT p_cogi PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY cogi ADD CONSTRAINT u_cogi UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON cogi TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON cogi_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE cup (nz bigint NOT NULL,naz character varying,tip integer,dat date,vrem time without time zone,nz_edxr bigint,sost integer)";
 db.exec(str);
 str="ALTER TABLE public.cup OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE cup_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.cup_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE cup_nz_seq OWNED BY cup.nz";
 db.exec(str);
 str="ALTER TABLE ONLY cup ALTER COLUMN nz SET DEFAULT nextval('cup_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY cup ADD CONSTRAINT p_cup PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY cup ADD CONSTRAINT u_cup UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON cup TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON cup_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE tipnf (nz bigint NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.tipnf OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY tipnf ADD CONSTRAINT p_tipnf PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (303,'НУ движения СКА')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (304,'Текстовые сообщения')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (306,'План проведения орбитальной коррекции')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (401,'Исх.данные для обеспеч.работы БЦА')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (402,'Текущие параметры орбиты СКА')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (403,'Заявки на проведение орбит.коррекции СКА')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (404,'Массивы технологич.инф. для формир.суперкадра')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (405,'Массивы специальной.инф. для формир.суперкадра')";
 db.exec(str);
 str="INSERT INTO tipnf (nz,opis) VALUES (406,'Текстовое сообщение')";
 db.exec(str);
 str="GRANT ALL ON tipnf TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sostcogi (nz bigint NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.sostcogi OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY sostcogi ADD CONSTRAINT p_sostcogi PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO sostcogi (nz,opis) VALUES (0,'получена')";
 db.exec(str);
 str="INSERT INTO sostcogi (nz,opis) VALUES (1,'просмотрена')";
 db.exec(str);
 str="INSERT INTO sostcogi (nz,opis) VALUES (2,'отправлено подтверждение')";
 db.exec(str);
 str="INSERT INTO sostcogi (nz,opis) VALUES (3,'подтверждение отменено')";
 db.exec(str);
 str="GRANT ALL ON sostcogi TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE pla (nz bigint NOT NULL,naz character varying,nka character varying,npn character varying,dat date,vrem time without time zone,"
     "vrs integer,ds integer,dzs integer,kos integer,ppsi integer,vit integer,az numeric,um numeric)";
 db.exec(str);
 str="ALTER TABLE public.pla OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE pla_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.pla_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE pla_nz_seq OWNED BY pla.nz";
 db.exec(str);
 str="ALTER TABLE ONLY pla ALTER COLUMN nz SET DEFAULT nextval('pla_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY pla ADD CONSTRAINT p_pla PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON pla TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON pla_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE ustnf (nz bigint NOT NULL,komp character varying,isx character varying,vxod character varying,zvon integer)";
 db.exec(str);
 str="ALTER TABLE public.ustnf OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE ustnf_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.ustnf_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE ustnf_nz_seq OWNED BY ustnf.nz";
 db.exec(str);
 str="ALTER TABLE ONLY ustnf ALTER COLUMN nz SET DEFAULT nextval('ustnf_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY ustnf ADD CONSTRAINT p_ustnf PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON ustnf TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON ustnf_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sostcup (nz bigint NOT NULL,opis character varying)";
 db.exec(str);
 str="ALTER TABLE public.sostcup OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY sostcup ADD CONSTRAINT p_sostcup PRIMARY KEY (nz)";
 db.exec(str);
 str="INSERT INTO sostcup (nz,opis) VALUES (0,'не определено')";
 db.exec(str);
 str="INSERT INTO sostcup (nz,opis) VALUES (1,'не отправлено')";
 db.exec(str);
 str="INSERT INTO sostcup (nz,opis) VALUES (2,'отправлено')";
 db.exec(str);
 str="INSERT INTO sostcup (nz,opis) VALUES (3,'отправлено повторно')";
 db.exec(str);
 str="INSERT INTO sostcup (nz,opis) VALUES (4,'отправка запрещена')";
 db.exec(str);
 str="GRANT ALL ON sostcup TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sp3 (nz bigint NOT NULL,naz character varying,godn integer,"
     "godk integer,sutn integer,sutk integer,sekn bigint,sekk bigint,nka character varying,dtn character varying,"
     "dtk character varying,nz_edxr bigint,kolep integer)";
 db.exec(str);
 str="ALTER TABLE public.sp3 OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sp3_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sp3_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sp3_nz_seq OWNED BY sp3.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sp3 ALTER COLUMN nz SET DEFAULT nextval('sp3_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sp3 ADD CONSTRAINT p_sp3 PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY sp3 ADD CONSTRAINT u_sp3 UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON sp3 TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sp3_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE sol (nz bigint NOT NULL,naz character varying,godn integer,"
     "godk integer,sutn integer,sutk integer,sekn bigint,sekk bigint,nka character varying,dtn character varying,"
     "dtk character varying,nz_edxr bigint,kolizm integer,kolk integer,kolo integer,sko numeric)";
 db.exec(str);
 str="ALTER TABLE public.sol OWNER TO postgres";
 db.exec(str);
 str="CREATE SEQUENCE sol_nz_seq START WITH 1 INCREMENT BY 1 NO MAXVALUE NO MINVALUE CACHE 1";
 db.exec(str);
 str="ALTER TABLE public.sol_nz_seq OWNER TO postgres";
 db.exec(str);
 str="ALTER SEQUENCE sol_nz_seq OWNED BY sol.nz";
 db.exec(str);
 str="ALTER TABLE ONLY sol ALTER COLUMN nz SET DEFAULT nextval('sol_nz_seq'::regclass)";
 db.exec(str);
 str="ALTER TABLE ONLY sol ADD CONSTRAINT p_sol PRIMARY KEY (nz)";
 db.exec(str);
 str="ALTER TABLE ONLY sol ADD CONSTRAINT u_sol UNIQUE (naz)";
 db.exec(str);
 str="GRANT ALL ON sol TO PUBLIC";
 db.exec(str);
 str="GRANT ALL ON sol_nz_seq TO PUBLIC";
 db.exec(str);

 str="CREATE TABLE leap (nz integer NOT NULL,dat1 date,dat2 date,sec integer)";
 db.exec(str);
 str="ALTER TABLE public.leap OWNER TO postgres";
 db.exec(str);
 str="ALTER TABLE ONLY leap ADD CONSTRAINT p_leap PRIMARY KEY (nz)";
 db.exec(str);
 str="GRANT ALL ON leap TO PUBLIC";
 db.exec(str);

 soob=QObject::tr("Создание БД завершено. ");
 QMessageBox::information(0,QObject::tr("Информация"),soob,QMessageBox::Yes);

 return true;
}

bool ClassBd::UpdateFull(bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE fullppsi SET naz = '"+zapfull.naz+"', kan = "+QString::number(zapfull.kan)+", godn = "+QString::number(zapfull.godn);
 strsql=strsql+", godk = "+QString::number(zapfull.godk)+", sutn = "+QString::number(zapfull.sutn)+", sutk = "+QString::number(zapfull.sutk);
 strsql=strsql+", sekn = "+QString::number(zapfull.sekn)+", sekk = "+QString::number(zapfull.sekk)+", nka = '"+zapfull.nka+"'";
 strsql=strsql+", dtn = '"+zapfull.dtn+"', dtk = '"+zapfull.dtk+"', kodobr = "+QString::number(zapfull.kodobr)+", npn = '"+zapfull.npn+"'";
 strsql=strsql+" WHERE nz = "+QString::number(zapfull.nz);

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateFull(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateFull(prsoob,query);

 return ret;
}

bool ClassBd::UpdateRowStatFull(QDate p1,QDate p2,QString naz_p1,QString naz_p2,quint64 nz_fullppsi,bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE statfull SET "+naz_p1+" = '"+p1.toString("dd-MM-yyyy")+"',"+naz_p2+" = '"+p2.toString("dd-MM-yyyy")+"'";
 strsql=strsql+" WHERE nz_fullppsi="+QString::number(nz_fullppsi);
 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateRowStatFull(QDate p1,QDate p2,QString naz_p1,QString naz_p2,quint64 nz_fullppsi,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateRowStatFull(p1,p2,naz_p1,naz_p2,nz_fullppsi,prsoob,query);

 return ret;
}

bool ClassBd::UpdateUst(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString val;
 QString spravt,spr_obrsi,spr_obr11_15,spr_obr12,spr_obr14,spr_obr13,spr_obr17,spr_obr16,spr_obr19,spr_obr18;
 QString spr_obr2,spr_obr3,spr_obr4,spr_obr7,spr_obr8,spr_obr9,spr_obr10;
 QString soob;

 if (zapust.pravt) spravt="t";
 else spravt="f";

 if (zapust.pr_obrsi) spr_obrsi="t";
 else spr_obrsi="f";

 if (zapust.pr_obr2) spr_obr2="t";
 else spr_obr2="f";

 if (zapust.pr_obr3) spr_obr3="t";
 else spr_obr3="f";

 if (zapust.pr_obr7) spr_obr7="t";
 else spr_obr7="f";

 if (zapust.pr_obr8) spr_obr8="t";
 else spr_obr8="f";

 if (zapust.pr_obr9) spr_obr9="t";
 else spr_obr9="f";

 if (zapust.pr_obr10) spr_obr10="t";
 else spr_obr10="f";

 if (zapust.pr_obr4) spr_obr4="t";
 else spr_obr4="f";

 if (zapust.pr_obr11_15) spr_obr11_15="t";
 else spr_obr11_15="f";

 if (zapust.pr_obr12) spr_obr12="t";
 else spr_obr12="f";

 if (zapust.pr_obr14) spr_obr14="t";
 else spr_obr14="f";

 if (zapust.pr_obr13) spr_obr13="t";
 else spr_obr13="f";

 if (zapust.pr_obr17) spr_obr17="t";
 else spr_obr17="f";

 if (zapust.pr_obr16) spr_obr16="t";
 else spr_obr16="f";

 if (zapust.pr_obr19) spr_obr19="t";
 else spr_obr19="f";

 if (zapust.pr_obr18) spr_obr18="t";
 else spr_obr18="f";

 val=" SET nazkomp = '"+zapust.nazkomp+"',path_temp = '"+zapust.path_temp+"',path_si = '"+zapust.path_si+"'";
 val=val+",pravt = '"+spravt+"',km = "+QString::number(zapust.km)+",pr_obrsi = '"+spr_obrsi+"'";
 val=val+",pr_obr11_15 = '"+spr_obr11_15+"',pr_obr12 = '"+spr_obr12+"',pr_obr14 = '"+spr_obr14+"'";
 val=val+",pr_obr13 = '"+spr_obr13+"',pr_obr17 = '"+spr_obr17+"',pr_obr16 = '"+spr_obr16+"',pr_obr19 = '"+spr_obr19+"'";
 val=val+",pr_obr18 = '"+spr_obr18+"',pr_obr2 = '"+spr_obr2+"',pr_obr3 = '"+spr_obr3+"',pr_obr4 = '"+spr_obr4+"'";
 val=val+",pr_obr7 = '"+spr_obr7+"',pr_obr8 = '"+spr_obr8+"',pr_obr9 = '"+spr_obr9+"',pr_obr10 = '"+spr_obr10+"'";
 val=val+",kol_obrsi = "+QString::number(zapust.kol_obrsi)+",kol_obr11_15 = "+QString::number(zapust.kol_obr11_15);
 val=val+",kol_obr12 = "+QString::number(zapust.kol_obr12)+",kol_obr14 = "+QString::number(zapust.kol_obr14);
 val=val+",kol_obr13 = "+QString::number(zapust.kol_obr13)+",kol_obr17 = "+QString::number(zapust.kol_obr17);
 val=val+",kol_obr16 = "+QString::number(zapust.kol_obr16)+",kol_obr19 = "+QString::number(zapust.kol_obr19);
 val=val+",kol_obr18 = "+QString::number(zapust.kol_obr18)+",kol_obr2 = "+QString::number(zapust.kol_obr2);
 val=val+",kol_obr3 = "+QString::number(zapust.kol_obr3)+",kol_obr4 = "+QString::number(zapust.kol_obr4);
 val=val+",kol_obr7 = "+QString::number(zapust.kol_obr7)+",kol_obr8 = "+QString::number(zapust.kol_obr8);
 val=val+",kol_obr9 = "+QString::number(zapust.kol_obr9)+",kol_obr10 = "+QString::number(zapust.kol_obr10);
 val=val+",pr_obrzapr = "+QString::number(zapust.pr_obrzapr)+",path_err = '"+zapust.path_err+"'";

 strsql="UPDATE ust "+val+" WHERE nz = "+QString::number(zapust.nz);
 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateUst(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateUst(prsoob,query);

 return ret;
}

quint64 ClassBd::InsertServ(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(isdir,vixdir,copdir,opis,maska,kobr,karx,nazkomp,prvd,prcd,maskvd,maskcd,sdir,sdirc)";
 val="('"+zapserv.isdir+"','"+zapserv.vixdir+"','"+zapserv.copdir+"','"+zapserv.opis+"','"+zapserv.maska+"'";
 val=val+","+QString::number(zapserv.kobr)+","+QString::number(zapserv.karx)+",'"+zapserv.nazkomp+"'";
 val=val+","+QString::number(zapserv.prvd)+","+QString::number(zapserv.prcd)+",'"+zapserv.maskvd+"','"+zapserv.maskcd;
 val=val+"',"+QString::number(zapserv.sdir)+","+QString::number(zapserv.sdirc)+")";
 strsql="INSERT INTO serv "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertServ(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertServ(prsoob,query);

 return nz;
}

void ClassBd::ClearServ()
{
 zapserv.nz=0;
 zapserv.isdir.clear();
 zapserv.vixdir.clear();
 zapserv.copdir.clear();
 zapserv.opis.clear();
 zapserv.maska.clear();
 zapserv.kobr=0;
 zapserv.karx=0;
 zapserv.nazkomp.clear();
 zapserv.prvd=0;
 zapserv.prcd=0;
 zapserv.maskcd.clear();
 zapserv.maskvd.clear();
 zapserv.sdir=0;
 zapserv.sdirc=0;
 return;
}

bool ClassBd::ReadServ(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("serv","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapserv.nz=spis_field[0].toLongLong();
 zapserv.isdir=spis_field[1].toString();
 zapserv.vixdir=spis_field[2].toString();
 zapserv.copdir=spis_field[3].toString();
 zapserv.opis=spis_field[4].toString();
 zapserv.maska=spis_field[5].toString();
 zapserv.kobr=spis_field[6].toInt();
 zapserv.karx=spis_field[7].toInt();
 zapserv.nazkomp=spis_field[8].toString();
 zapserv.prvd=spis_field[9].toInt();
 zapserv.prcd=spis_field[10].toInt();
 zapserv.maskvd=spis_field[11].toString();
 zapserv.maskcd=spis_field[12].toString();
 zapserv.sdir=spis_field[13].toInt();
 zapserv.sdirc=spis_field[14].toInt();

 return true;
}

bool ClassBd::ReadServ(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadServ(nz,prsoob,query);

 return ret;
}

bool ClassBd::UpdateServ(bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE serv SET isdir = '"+zapserv.isdir+"', vixdir = '"+zapserv.vixdir+"', copdir = '"+zapserv.copdir+"', opis = '"+zapserv.opis;
 strsql=strsql+"', maska = '"+zapserv.maska+"', kobr = "+QString::number(zapserv.kobr)+", karx = "+QString::number(zapserv.karx);
 strsql=strsql+", nazkomp = '"+zapserv.nazkomp+"', prvd = "+QString::number(zapserv.prvd)+", prcd = "+QString::number(zapserv.prcd);
 strsql=strsql+", maskvd = '"+zapserv.maskvd+"', maskcd = '"+zapserv.maskcd+"', sdir = "+QString::number(zapserv.sdir);
 strsql=strsql+", sdirc = "+QString::number(zapserv.sdirc);
 strsql=strsql+" WHERE nz = "+QString::number(zapserv.nz);

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateServ(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateServ(prsoob,query);

 return ret;
}

quint64 ClassBd::InsertStatServ(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(ifile,dat,vr,nz_serv)";
 val="('"+zapstatserv.ifile+"','"+zapstatserv.dat.toString("dd-MM-yyyy")+"','"+zapstatserv.vr.toString("hh:mm:ss")+"',"+QString::number(zapstatserv.nz_serv)+")";
 strsql="INSERT INTO statserv "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertStatServ(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertStatServ(prsoob,query);

 return nz;
}

void ClassBd::ClearStatServ()
{
 zapstatserv.nz=0;
 zapstatserv.ifile.clear();
 zapstatserv.dat=QDate::currentDate();
 zapstatserv.vr=QTime::currentTime();
 zapstatserv.nz_serv=0;
 return;
}

QStringList ClassBd::GetRowAll(QSqlQuery query,qint32 nkol)
{
 QStringList spis;

 query.first();

 do
 {
  GetFieldAll(query);
  if (kol_spis_field > 0) spis.append(spis_field[nkol].toString());
 } while (query.next());

 return spis;
}

void ClassBd::ClearSba()
{
 zapsba.nz=0;
 zapsba.nz_kodba=0;
 zapsba.dvik=QDate::currentDate();
 zapsba.vvik=QTime::currentTime();
 zapsba.dvkl=QDate::currentDate();
 zapsba.vvkl=QTime::currentTime();
 zapsba.nka=0;
 return;
}

bool ClassBd::ReadSba(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("sba","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 ClearSba();

 zapsba.nz=spis_field[0].toLongLong();
 zapsba.nz_kodba=spis_field[1].toLongLong();
 zapsba.dvik=spis_field[2].toDate();
 zapsba.vvik=spis_field[3].toTime();
 zapsba.dvkl=spis_field[4].toDate();
 zapsba.vvkl=spis_field[5].toTime();
 zapsba.nka=spis_field[6].toInt();

 return true;
}

bool ClassBd::ReadSba(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadSba(nz,prsoob,query);

 return ret;
}

quint64 ClassBd::InsertSba(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(nz_kodba,dvik,vvik,dvkl,vvkl,nka)";
 val="("+QString::number(zapsba.nz_kodba)+",'"+zapsba.dvik.toString("dd-MM-yyyy")+"','"+zapsba.vvik.toString("hh:mm:ss")+"'";
 val=val+",'"+zapsba.dvkl.toString("dd-MM-yyyy")+"','"+zapsba.vvkl.toString("hh:mm:ss")+"',"+QString::number(zapsba.nka)+")";
 strsql="INSERT INTO sba "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertSba(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertSba(prsoob,query);

 return nz;
}

bool ClassBd::UpdateSba(bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE sba SET nz_kodba = "+QString::number(zapsba.nz_kodba)+", dvik = '"+zapsba.dvik.toString("dd-MM-yyyy")+"', vvik = '"+zapsba.vvik.toString("hh:mm:ss")+"'";
 strsql=strsql+", dvkl = '"+zapsba.dvkl.toString("dd-MM-yyyy")+"', vvkl = '"+zapsba.vvkl.toString("hh:mm:ss")+"', nka = "+QString::number(zapsba.nka);
 strsql=strsql+" WHERE nz = "+QString::number(zapsba.nz);

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateSba(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateSba(prsoob,query);

 return ret;
}

void ClassBd::ClearSist()
{
 zapsist.nz=0;
 zapsist.dt=QDate::currentDate();
 zapsist.vr=QTime::currentTime();
 zapsist.nz_kodobr=0;
 zapsist.sost.clear();
 zapsist.naz_komp.clear();
// zapsist.polz.clear();
//zapsist.naz_ed.clear();
 zapsist.naz_komp=zapust.nazkomp;
 zapsist.polz=nikp;

 return;
}

bool ClassBd::ReadSist(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("sist","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 ClearSist();

 zapsist.nz=spis_field[0].toLongLong();
 zapsist.dt=spis_field[1].toDate();
 zapsist.vr=spis_field[2].toTime();
 zapsist.nz_kodobr=spis_field[3].toLongLong();
 zapsist.sost=spis_field[4].toString();
 zapsist.naz_komp=spis_field[5].toString();
 zapsist.polz=spis_field[6].toString();
 zapsist.naz_ed=spis_field[7].toString();

 return true;
}

bool ClassBd::ReadSist(qlonglong nz,bool prsoob)
{
 qlonglong nz1;
 QSqlQuery query;
 bool ret;

 ret=ReadSist(nz,prsoob,query);

 return ret;
}

quint64 ClassBd::InsertSist(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz;

 pol="(dt,vr,nz_kodobr,sost,naz_komp,polz,naz_ed)";
 val="('"+zapsist.dt.toString("dd-MM-yyyy")+"','"+zapsist.vr.toString("hh:mm:ss")+"',"+QString::number(zapsist.nz_kodobr);
 val=val+",'"+zapsist.sost+"','"+zapsist.naz_komp+"','"+zapsist.polz+"','"+zapsist.naz_ed+"')";
 strsql="INSERT INTO sist "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

quint64 ClassBd::InsertSist(bool prsoob)
{
 quint64 nz;
 QSqlQuery query;

 nz=InsertSist(prsoob,query);

 return nz;
}

QList<qlonglong> ClassBd::FindRowPpsiForDat(QDate dat,qint32 kod, bool prsoob, QSqlQuery query,QString nka)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString str_dat;

 str_dat="'"+dat.toString("dd-MM-yyyy")+"'";

 strsql="SELECT statfull.nz_fullppsi,fullppsi.naz FROM statfull,fullppsi WHERE statfull.nz_fullppsi = fullppsi.nz AND "
        "fullppsi.strk = 0 AND fullppsi.nka = '"+nka+"' AND ";
 if (kod == 1)
  strsql=strsql+"((statfull.dn_kvi BETWEEN "+str_dat+" AND "+str_dat+") OR (statfull.dk_kvi BETWEEN "+str_dat+" AND "+str_dat+"))";
 if (kod == 2)
  strsql=strsql+"((statfull.dn_kvno BETWEEN "+str_dat+" AND "+str_dat+") OR (statfull.dk_kvno BETWEEN "+str_dat+" AND "+str_dat+"))";
 if (kod == 3)
  strsql=strsql+"((statfull.dn_vrv BETWEEN "+str_dat+" AND "+str_dat+") OR (statfull.dk_vrv BETWEEN "+str_dat+" AND "+str_dat+"))";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowPpsiForDat(QDate dat,qint32 kod, bool prsoob,QString nka)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowPpsiForDat(dat,kod,prsoob,query,nka);

 return list_id;
}

QStringList ClassBd::SpisNPN(QString table, QString nazis, bool prsoob)
{
 QStringList list;
 QString str;
 QSqlQuery query;

 str="SELECT DISTINCT npn FROM "+table+" ORDER BY npn";
 if (!nazis.isEmpty())
 {
  if (nazis == "M")
  {
   str="SELECT DISTINCT npn FROM "+table+" WHERE tip = '"+nazis+"' ";
   str=str+" ORDER BY npn";
  }
  else
  {
   str="SELECT DISTINCT npn FROM "+table+" WHERE nazis = '"+nazis+"' ";
   if (nazis == "KOS") str="SELECT DISTINCT npn FROM "+table+" WHERE nazis = '"+nazis+"' OR nazis = 'FRD'";
   str=str+" ORDER BY npn";
  }
 }
 if (! query.exec(str))
 {
  if (prsoob)
  {
   QString soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return list;
 }

 list=GetRowAll(query,0);
 return list;
}

quint64 ClassBd::InsertNos(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 qulonglong nz=0;
 QString blk;

 pol="(naz,prblok)";
 val="('"+zapnos.naz+"',"+QString::number(zapnos.prblok)+")";

 strsql="INSERT INTO nos "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob);

 return nz;
}

quint64 ClassBd::InsertPPSI(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(naz,godn,sutn,sekn,nka,npn,tip)";
    val="('"+zapplppsi.naz +"'," + QString::number(zapplppsi.godn) + "," + QString::number(zapplppsi.sutn);
    val=val+"," + QString::number(zapplppsi.sekn) + ","+QString::number(zapplppsi.nka) +",'" + zapplppsi.npn +"','"+zapplppsi.tip+"')";
    strsql="INSERT INTO plppsi "+pol+" VALUES "+val+" RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

quint64 ClassBd::InsertPPSI(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertPPSI(prsoob, query);
    return nz;
}

quint64 ClassBd::InsertSPLPPSI(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(sek, az, um, nz_plppsi)";
    val="("+ QString::number(zapsplppsi.sek) +","+ QString().setNum(zapsplppsi.az,'f',6) + "," + QString().setNum(zapsplppsi.um,'f',6) + "," +
               QString::number(zapsplppsi.nz_plppsi) + ")";
    strsql="INSERT INTO splppsi " + pol + " VALUES " + val + " RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

quint64 ClassBd::InsertSPLPPSI(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertSPLPPSI(prsoob, query);
    return nz;
}

void ClassBd::ClearPPSI()
{
    zapplppsi.naz.clear();
    zapplppsi.godn = 0;
    zapplppsi.nka = 0;
    zapplppsi.npn.clear();
    zapplppsi.nz = 0;
    zapplppsi.sekn = 0;
    zapplppsi.sutn = 0;
    zapplppsi.tip.clear();
}

void ClassBd::ClearSPLPPSI()
{
    zapsplppsi.az = 0;
    zapsplppsi.nz = 0;
    zapsplppsi.nz_plppsi = 0;
    zapsplppsi.um = 0;
}

bool ClassBd::UpdateNos(bool prsoob)
{
 QString strsql;
 QString val;
 QString soob;
 QSqlQuery query;

 val=" SET naz = '"+zapnos.naz+"',prblok = "+QString::number(zapnos.prblok);

 strsql="UPDATE nos "+val+" WHERE nz = "+QString::number(zapnos.nz);
 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

QStringList ClassBd::StatDDSInList(QDate dtn,QDate dtk,QString nka,bool prsoob)
{
 QStringList list;
 QString str;
 QSqlQuery query;

 list.clear();

 str="SELECT statdds.dat,statdds.eppl,statdds.eppol,(statdds.ep_4rglo + statdds.ep_4bglo) AS ep2pol FROM statdds,dds"
     " WHERE statdds.nz_dds = dds.nz AND dds.nka = '"+nka+"' AND (statdds.dat BETWEEN '"+dtn.toString("yyyy-MM-dd")+"' AND '"+dtk.toString("yyyy-MM-dd")+"') ORDER BY statdds.dat";

 if (! query.exec(str))
 {
  if (prsoob)
  {
   QString soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return list;
 }

 query.first();

 do
 {
  GetFieldAll(query);
  if (kol_spis_field > 0)
  {
   str=spis_field[0].toDate().toString("yyyy-MM-dd")+";";
   str=str+QString::number(spis_field[1].toInt())+";";
   str=str+QString::number(spis_field[2].toInt())+";";
   str=str+QString::number(spis_field[3].toInt());
   list.append(str);
  }
 } while (query.next());

 return list;
}

quint64 ClassBd::InsertPla(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(naz,nka,npn,dat,vrem,vrs,ds,dzs,kos,ppsi,vit,az,um)";
    val="('"+zappla.naz+"','"+zappla.nka+"','"+zappla.npn+"','"+zappla.dat.toString("dd-MM-yyyy")+"','"+zappla.vrem.toString("hh:mm:ss")+"'";
    val=val+","+QString::number(zappla.vrs)+","+QString::number(zappla.ds)+","+QString::number(zappla.dzs)+","+QString::number(zappla.kos);
    val=val+","+QString::number(zappla.ppsi)+","+QString::number(zappla.vit)+","+QString().setNum(zappla.az,'f',1);
    val=val+","+QString().setNum(zappla.um,'f',2)+")";
    strsql="INSERT INTO pla " + pol + " VALUES " + val + " RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

void ClassBd::ClearPla()
{
 zappla.nz=0;
 zappla.naz.clear();
 zappla.nka.clear();
 zappla.npn.clear();
 zappla.dat.setDate(1900,1,1);
 zappla.vrem.setHMS(0,0,0);
 zappla.vrs=0;
 zappla.ds=0;
 zappla.dzs=0;
 zappla.kos=0;
 zappla.ppsi=0;
 zappla.vit=0;
 zappla.az=0;
 zappla.um=0;
 return;
}

quint64 ClassBd::InsertPla(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertPla(prsoob, query);
    return nz;
}

quint64 ClassBd::InsertCogi(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(naz,tip,sost,dat,vrem,nz_edxr,nz_cup)";
    val="('"+zapcogi.naz+"',"+QString::number(zapcogi.tip)+","+QString::number(zapcogi.sost);
    val=val+",'"+zapcogi.dat.toString("dd-MM-yyyy")+"','"+zapcogi.vrem.toString("hh:mm:ss")+"'";
    val=val+","+QString::number(zapcogi.nz_edxr)+","+QString::number(zapcogi.nz_cup)+")";
    strsql="INSERT INTO cogi " + pol + " VALUES " + val + " RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

quint64 ClassBd::InsertCogi(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertCogi(prsoob, query);
    return nz;
}

void ClassBd::ClearCogi()
{
 zapcogi.nz=0;
 zapcogi.naz.clear();
 zapcogi.tip=0;
 zapcogi.sost=0;
 zapcogi.dat.setDate(1900,1,1);
 zapcogi.vrem.setHMS(0,0,0);
 zapcogi.nz_edxr=0;
 zapcogi.nz_cup=0;
 return;
}

quint64 ClassBd::InsertCup(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(naz,tip,dat,vrem,nz_edxr,sost)";
    val="('"+zapcup.naz+"',"+QString::number(zapcup.tip);
    val=val+",'"+zapcup.dat.toString("dd-MM-yyyy")+"','"+zapcup.vrem.toString("hh:mm:ss")+"'";
    val=val+","+QString::number(zapcup.nz_edxr)+","+QString::number(zapcup.sost)+")";
    strsql="INSERT INTO cup " + pol + " VALUES " + val + " RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

quint64 ClassBd::InsertCup(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertCup(prsoob, query);
    return nz;
}

void ClassBd::ClearCup()
{
 zapcup.nz=0;
 zapcup.naz.clear();
 zapcup.tip=0;
 zapcup.dat.setDate(1900,1,1);
 zapcup.vrem.setHMS(0,0,0);
 zapcup.nz_edxr=0;
 zapcup.sost=0;
 return;
}

quint64 ClassBd::InsertUstnf(bool prsoob, QSqlQuery query)
{
    QString strsql;
    QString pol;
    QString val;
    quint64 nz;

    pol="(komp,isx,vxod,zvon)";
    val="('"+zapustnf.komp+"','"+zapustnf.isx+"','"+zapustnf.vxod+"',"+QString::number(zapustnf.zvon)+")";
    strsql="INSERT INTO ustnf " + pol + " VALUES " + val + " RETURNING nz";

    nz=InsertRow(strsql,prsoob,query);

    return nz;
}

quint64 ClassBd::InsertUstnf(bool prsoob)
{
    quint64 nz;
    QSqlQuery query;
    nz = InsertUstnf(prsoob, query);
    return nz;
}

void ClassBd::ClearUstnf()
{
 zapustnf.nz=0;
 zapustnf.komp.clear();
 zapustnf.isx.clear();
 zapustnf.vxod.clear();
 zapustnf.zvon=0;
 return;
}

bool ClassBd::ReadUstnf(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("ustnf","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 ClearUstnf();

 zapustnf.nz=spis_field[0].toLongLong();
 zapustnf.komp=spis_field[1].toString();
 zapustnf.isx=spis_field[2].toString();
 zapustnf.vxod=spis_field[3].toString();
 zapustnf.zvon=spis_field[4].toInt();

 return true;
}

bool ClassBd::ReadUstnf(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadUstnf(nz,prsoob,query);

 return ret;
}

bool ClassBd::UpdateUstnf(bool prsoob,QSqlQuery query)
{
 QString soob;
 QString strsql;

 strsql="UPDATE ustnf SET isx = '"+zapustnf.isx+"', vxod = '"+zapustnf.vxod+"', zvon = "+QString::number(zapustnf.zvon)+" ";
 strsql=strsql+" WHERE nz = "+QString::number(zapustnf.nz);

 if (! query.exec(strsql))
 {
  if (prsoob)
  {
   soob=QObject::tr("Ошибка при модифицировании записи. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  return false;
 }

 return true;
}

bool ClassBd::UpdateUstnf(bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=UpdateUstnf(prsoob,query);

 return ret;
}

bool ClassBd::ReadCogi(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("cogi","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 ClearCogi();

 zapcogi.nz=spis_field[0].toLongLong();
 zapcogi.naz=spis_field[1].toString();
 zapcogi.tip=spis_field[2].toInt();
 zapcogi.sost=spis_field[3].toInt();
 zapcogi.dat=spis_field[4].toDate();
 zapcogi.vrem=spis_field[5].toTime();
 zapcogi.nz_edxr=spis_field[6].toLongLong();
 zapcogi.nz_cup=spis_field[7].toLongLong();

 return true;
}

bool ClassBd::ReadCogi(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadCogi(nz,prsoob,query);

 return ret;
}

bool ClassBd::ReadCup(qlonglong nz,bool prsoob,QSqlQuery query)
{
 qlonglong nz1;

 nz1=FindRow("cup","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 ClearCup();

 zapcup.nz=spis_field[0].toLongLong();
 zapcup.naz=spis_field[1].toString();
 zapcup.tip=spis_field[2].toInt();
 zapcup.dat=spis_field[3].toDate();
 zapcup.vrem=spis_field[4].toTime();
 zapcup.nz_edxr=spis_field[5].toLongLong();
 zapcup.sost=spis_field[6].toInt();

 return true;
}

bool ClassBd::ReadCup(qlonglong nz,bool prsoob)
{
 QSqlQuery query;
 bool ret;

 ret=ReadCup(nz,prsoob,query);

 return ret;
}

QList<qlonglong> ClassBd::FindRowForOtprNf(bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;

 strsql="SELECT cup.nz FROM cup WHERE cup.sost = 1";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForOtprNf(bool prsoob)
{
 QSqlQuery query;
 QList<qlonglong> list;

 list=FindRowForOtprNf(prsoob,query);

 return list;
}

QList<qlonglong> ClassBd::FindRowAvtZapr(bool prsoob,QSqlQuery query,QString komp)
{
 QString strsql;
 QList<qlonglong> list_id;

 strsql="SELECT zapr.nz FROM zapr WHERE zapr.pr_avt = 't' AND zapr.komp = '"+komp+"'";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowAvtZapr(bool prsoob,QString komp)
{
 QSqlQuery query;
 QList<qlonglong> list;

 list=FindRowAvtZapr(prsoob,query,komp);

 return list;
}

quint64 ClassBd::InsertSP3(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;
 QSqlQuery query;

 pol="(naz,godn,godk,sutn,sutk,sekn,sekk,nka,dtn,dtk,nz_edxr,kolep)";
 val="('"+zapsp3.naz+"',"+QString::number(zapsp3.godn)+","+QString::number(zapsp3.godk)+","+QString::number(zapsp3.sutn)+","+QString::number(zapsp3.sutk);
 val=val+","+QString::number(zapsp3.sekn)+","+QString::number(zapsp3.sekk)+",'"+zapsp3.nka+"'";
 val=val+",'"+zapsp3.dtn+"','"+zapsp3.dtk+"',"+QString::number(zapsp3.nz_edxr)+","+QString::number(zapsp3.kolep)+")";


 strsql="INSERT INTO sp3 "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

bool ClassBd::ReadSP3(qlonglong nz,bool prsoob)
{
 qlonglong nz1;
 QSqlQuery query;

 ClearSP3();

 nz1=FindRow("sp3","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapsp3.nz=spis_field[0].toLongLong();
 zapsp3.naz=spis_field[1].toString();
 zapsp3.godn=spis_field[2].toInt();
 zapsp3.godk=spis_field[3].toInt();
 zapsp3.sutn=spis_field[4].toInt();
 zapsp3.sutk=spis_field[5].toInt();
 zapsp3.sekn=spis_field[6].toLongLong();
 zapsp3.sekk=spis_field[7].toLongLong();
 zapsp3.nka=spis_field[8].toString();
 zapsp3.dtn=spis_field[9].toString();
 zapsp3.dtk=spis_field[10].toString();
 zapsp3.nz_edxr=spis_field[11].toLongLong();
 zapsp3.kolep=spis_field[12].toInt();

 return true;
}

void ClassBd::ClearSP3()
{
 zapsp3.nz=0;
 zapsp3.naz.clear();
 zapsp3.godn=0;
 zapsp3.godk=0;
 zapsp3.sutn=0;
 zapsp3.sutk=0;
 zapsp3.sekn=0;
 zapsp3.sekk=0;
 zapsp3.nka.clear();
 zapsp3.dtn.clear();
 zapsp3.dtk.clear();
 zapsp3.nz_edxr=0;
 zapsp3.kolep=0;
 return;
}

QList<qlonglong> ClassBd::FindRowForEdvidSp3(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if (!zapzapr.sp3) return list_id;

 strsql="SELECT sp3.nz FROM sp3,edxr WHERE sp3.nz_edxr = edxr.nz";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidSp3(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidSp3(nz_zapr,prsoob,query);

 return list_id;
}

quint64 ClassBd::FindSp3ForDds(quint64 nz_dds,bool prsoob)
{
    quint64 id=0;
    QString strsql;
    QSqlQuery query;

    if (!ReadDDS(nz_dds,prsoob)) return id;

    strsql="SELECT * FROM sp3 WHERE  godn = "+QString::number(zapdds.godn)+" AND sutn = "+QString::number(zapdds.sutn)+" AND nka = '"+zapdds.nka+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

void ClassBd::ClearSol()
{
 zapsol.nz=0;
 zapsol.naz.clear();
 zapsol.godn=0;
 zapsol.godk=0;
 zapsol.sutn=0;
 zapsol.sutk=0;
 zapsol.sekn=0;
 zapsol.sekk=0;
 zapsol.nka.clear();
 zapsol.dtn.clear();
 zapsol.dtk.clear();
 zapsol.nz_edxr=0;
 zapsol.kolizm=0;
 zapsol.kolk=0;
 zapsol.kolo=0;
 zapsol.sko=0;
 return;
}

quint64 ClassBd::InsertSol(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;
 QSqlQuery query;

 pol="(naz,godn,godk,sutn,sutk,sekn,sekk,nka,dtn,dtk,nz_edxr,kolizm,kolk,kolo,sko)";
 val="('"+zapsol.naz+"',"+QString::number(zapsol.godn)+","+QString::number(zapsol.godk)+","+QString::number(zapsol.sutn)+","+QString::number(zapsol.sutk);
 val=val+","+QString::number(zapsol.sekn)+","+QString::number(zapsol.sekk)+",'"+zapsol.nka+"'";
 val=val+",'"+zapsol.dtn+"','"+zapsol.dtk+"',"+QString::number(zapsol.nz_edxr)+","+QString::number(zapsol.kolizm)+","+QString::number(zapsol.kolk);
 val=val+","+QString::number(zapsol.kolo)+","+QString().setNum(zapsol.sko,'f',3)+")";


 strsql="INSERT INTO sol "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

bool ClassBd::ReadSol(qlonglong nz,bool prsoob)
{
 qlonglong nz1;
 QSqlQuery query;

 ClearSol();

 nz1=FindRow("sol","nz",nz,prsoob,query);
 if (nz1 == -1) return false;

 zapsol.nz=spis_field[0].toLongLong();
 zapsol.naz=spis_field[1].toString();
 zapsol.godn=spis_field[2].toInt();
 zapsol.godk=spis_field[3].toInt();
 zapsol.sutn=spis_field[4].toInt();
 zapsol.sutk=spis_field[5].toInt();
 zapsol.sekn=spis_field[6].toLongLong();
 zapsol.sekk=spis_field[7].toLongLong();
 zapsol.nka=spis_field[8].toString();
 zapsol.dtn=spis_field[9].toString();
 zapsol.dtk=spis_field[10].toString();
 zapsol.nz_edxr=spis_field[11].toLongLong();
 zapsol.kolizm=spis_field[12].toInt();
 zapsol.kolk=spis_field[13].toInt();
 zapsol.kolo=spis_field[14].toInt();
 zapsol.sko=spis_field[15].toDouble();

 return true;
}

QList<qlonglong> ClassBd::FindRowForEdvidSol(quint64 nz_zapr, bool prsoob,QSqlQuery query)
{
 QString strsql;
 QList<qlonglong> list_id;
 QString strdn,strdk;

 if (!ReadZapr(nz_zapr,prsoob)) return list_id;
 if (!zapzapr.sol) return list_id;

 strsql="SELECT sol.nz FROM sol,edxr WHERE sol.nz_edxr = edxr.nz";

 if (zapzapr.dtt > zapzapr.dtn) strdn=zapzapr.dtt.toString("dd-MM-yyyy");
 else strdn=zapzapr.dtn.toString("dd-MM-yyyy");

// if (zapzapr.dtk.year() == 1900) strdk="01-01-2999";
// else strdk=zapzapr.dtk.toString("dd-MM-yyyy");

 strdk=QDate::currentDate().toString("dd-MM-yyyy");


 strsql=strsql+" AND edxr.datp BETWEEN '"+strdn+"' AND '"+strdk+"'";

 list_id=ExecQueryList(query,strsql,prsoob);

 return list_id;
}

QList<qlonglong> ClassBd::FindRowForEdvidSol(quint64 nz_zapr, bool prsoob)
{
 QList<qlonglong> list_id;
 QSqlQuery query;

 list_id=FindRowForEdvidSol(nz_zapr,prsoob,query);

 return list_id;
}

void ClassBd::Report(QTableView *table, QString zag, QString &path_prot)
{
    QString strStream = "";
    QTextStream out(&strStream);
    out.setCodec("UTF-8");
    out.setLocale( QLocale("UTF-8") );
    QVariant vdata;
    QString data;
    QString str;

    const int rowCount = table->model()->rowCount();
    const int columnCount = table->model()->columnCount();

    out << "<html>"
                "<meta http-equiv = \"Content-Type\" Content=\"Text/html; charset=utf-8\">\n"
                "<h1> " + zag + "</h1>"
           "</html>";
    out <<  "<html>\n"
        "<head>\n"
        "<meta Content=\"Text/html; charset=UTF-8\">\n"
        <<  "</head>\n"
        "<body bgcolor=#ffffff link=#5000A0>\n"
        "<table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; ++column)
    {
        if ( !table->isColumnHidden(column) )
        {
          str=table->model()->headerData(column, Qt::Horizontal).toString();
//          out << QString("<th>%1</th>").arg( table->model()->headerData(column, Qt::Horizontal).toString() );
          str=str.replace(QString("\n"),QString("<br/> "));
          out << "<th>"+str+"</th>";
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr>";
        for ( int column = 0; column < columnCount; ++column )
        {
            if ( !table->isColumnHidden(column) )
            {
                vdata=table->model()->data( table->model()->index(row, column) );
                qDebug() << vdata << vdata.typeName() << vdata.type();
                if (vdata.type() == QMetaType::QDateTime)
                    data = vdata.toDateTime().toString("dd-MM-yyyy hh:mm");
                else
                    data = vdata.toString().trimmed();
                qDebug() << data;
                out << QString("<td bkcolor=0>%1</td>").arg(( !data.isEmpty()) ? data : QString("&nbsp;") );
            }
        }
        out << "</tr>\n";
    }

    out <<  "</table>\n"
        "</body>\n"
        "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml( strStream );

    QPrinter printer;
    printer.setPageSize( QPrinter::A4 );
    printer.setPageMargins( QMarginsF( 15, 15, 15, 15 ) );
    printer.setOutputFileName(path_prot);

    QPrintDialog *dialog = new QPrintDialog( &printer );
    if (dialog->exec() == QDialog::Accepted)
    {
        path_prot=printer.outputFileName();
        qDebug() << path_prot;
        document->print( &printer );
    }

    delete document;
    delete dialog;
}

qlonglong ClassBd::FindRowRnxi(QString naz,QString ver,QString npn,QString tip,qint32 godn,qint32 sutn,qint32 sekn,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString strsql;

    strsql="SELECT * FROM rnxi WHERE naz = '"+naz+"' AND ver = '"+ver+"' AND npn = '"+npn+"' AND tip = '"+tip+"' AND godn = "+QString::number(godn);
    strsql=strsql+" AND sutn = "+QString::number(sutn)+" AND sekn = "+QString::number(sekn);
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRowRnxi(QString naz,QString ver,QString npn,QString tip,qint32 godn,qint32 sutn,qint32 sekn,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRowRnxi(naz,ver,npn,tip,godn,sutn,sekn,prsoob,query);

    return id;
}

qlonglong ClassBd::FindRowDs(QString naz,QString npn,qint32 godn,qint32 sutn,qint32 sekn,QString nazis,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString strsql;

    strsql="SELECT * FROM ds WHERE naz = '"+naz+"' AND npn = '"+npn+"' AND godn = "+QString::number(godn);
    strsql=strsql+" AND sutn = "+QString::number(sutn)+" AND sekn = "+QString::number(sekn)+" AND nazis = '"+nazis+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRowDs(QString naz,QString npn,qint32 godn,qint32 sutn,qint32 sekn,QString nazis,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRowDs(naz,npn,godn,sutn,sekn,nazis,prsoob,query);

    return id;
}

qlonglong ClassBd::FindRowFullppsi(QString naz,qint32 kan,qint32 godn,qint32 sutn,qint32 sekn,QString nka,QString npn,bool prsoob,QSqlQuery query)
{
    qlonglong id;
    QString strsql;

    strsql="SELECT * FROM fullppsi WHERE naz = '"+naz+"' AND npn = '"+npn+"' AND godn = "+QString::number(godn);
    strsql=strsql+" AND sutn = "+QString::number(sutn)+" AND sekn = "+QString::number(sekn)+" AND nka = '"+nka+"' ";
    strsql=strsql+" AND kan = "+QString::number(kan);
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

qlonglong ClassBd::FindRowFullppsi(QString naz,qint32 kan,qint32 godn,qint32 sutn,qint32 sekn,QString nka,QString npn,bool prsoob)
{
    qlonglong id;
    QSqlQuery query;

    id=FindRowFullppsi(naz,kan,godn,sutn,sekn,nka,npn,prsoob,query);

    return id;
}

quint64 ClassBd::FindRowSp3(QString naz,bool prsoob)
{
    quint64 id=0;
    QString strsql;
    QSqlQuery query;

    strsql="SELECT * FROM sp3 WHERE  naz = '"+naz+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

quint64 ClassBd::FindRowSol(QString naz,bool prsoob)
{
    quint64 id=0;
    QString strsql;
    QSqlQuery query;

    strsql="SELECT * FROM sol WHERE  naz = '"+naz+"'";
    id=ExecQuery(query,strsql,prsoob);
    return id;
}

quint64 ClassBd::InsertLeap(bool prsoob)
{
 QString strsql;
 QString pol;
 QString val;
 quint64 nz=0;
 QSqlQuery query;

 pol="(nz,dat1,dat2,sec)";
 val="("+QString::number(zapleap.nz)+",'"+zapleap.dat1.toString("dd-MM-yyyy")+"','"+zapleap.dat2.toString("dd-MM-yyyy")+"',"+QString::number(zapleap.sec)+")";


 strsql="INSERT INTO leap "+pol+" VALUES "+val+" RETURNING nz";

 nz=InsertRow(strsql,prsoob,query);

 return nz;
}

void ClassBd::ClearLeap()
{
 zapleap.nz=0;
 zapleap.dat1.setDate(2000,1,1);
 zapleap.dat2.setDate(2000,1,1);
 zapleap.sec = 0;
}

qint32 ClassBd::FindLeap(QDate dat)
{
 QString sdat,str;
 qint32 sec;
 QSqlQuery query;

 sdat=dat.toString("dd-MM-yyyy");

 str="SELECT leap.sec FROM leap WHERE '"+sdat+"' BETWEEN dat1 AND dat2";

 sec=ExecQuery(query,str,true);

 return sec;
}

//поиск и чтение записи плана АСПСИ (таблица plppsi)
//при неудачном поиске возвращает false
//при удачном - true и в sn,sk секунды начала и конца сеанса по плану,tn,tk время начала и конца сеанса по плану
bool ClassBd::FindPlanForFull(quint64 nz_full,qint32 &sn,qint32 &sk,QTime &tn,QTime &tk,bool prsoob)
{
 if (!ReadFullppsi(nz_full,prsoob)) return false;

 if (!FindPlanForFull(zapfull.godn,zapfull.sutn,zapfull.sekn,zapfull.nka,sn,sk,tn,tk,prsoob)) return false;

 return true;
}

//поиск и чтение записи плана АСПСИ (таблица plppsi)
//при неудачном поиске возвращает false
//при удачном - true и в sn,sk секунды начала и конца сеанса по плану,tn,tk время начала и конца сеанса по плану
bool ClassBd::FindPlanForFull(qint32 godn,qint32 sutn,qint32 sekn,QString nka,qint32 &sn,qint32 &sk,QTime &tn,QTime &tk,bool prsoob)
{
 qlonglong id;
 QString strsql;
 QSqlQuery query;

 strsql="WITH pl AS (SELECT plppsi.nz,plppsi.naz,plppsi.godn,plppsi.sutn,plppsi.sekn FROM plppsi,ppsi "
        "WHERE "+QString::number(sutn)+" = plppsi.sutn AND "+QString::number(godn)+" = plppsi.godn AND "
        "(plppsi.sekn BETWEEN "+QString::number(sekn)+"-600 AND "+QString::number(sekn)+"+600) "
        "AND '"+nka+"' = CAST(plppsi.nka AS varchar)) "
        "SELECT pl.nz,pl.sekn,(pl.sekn+splppsi.sek) AS sekk, "
        "make_time((pl.sekn / 3600)::int, (pl.sekn / 60 % 60)::int, (pl.sekn - ((pl.sekn / 3600)::int * 3600) - (pl.sekn / 60 % 60)::int *60)) AS tn,"
        "make_time(((pl.sekn+splppsi.sek) / 3600)::int, ((pl.sekn+splppsi.sek) / 60 % 60)::int, ((pl.sekn+splppsi.sek) - "
        "(((pl.sekn+splppsi.sek) / 3600)::int * 3600) - ((pl.sekn+splppsi.sek) / 60 % 60)::int *60)) AS tk "
        "FROM splppsi,pl WHERE splppsi.nz_plppsi=pl.nz";

 id=ExecQuery(query,strsql,prsoob);
 if (id == 0) return false;

 sn=spis_field[1].toInt();
 sk=spis_field[2].toInt();
 tn=spis_field[3].toTime();
 tk=spis_field[4].toTime();

 return true;
}
