#include "fedvid.h"
#include "ui_fedvid.h"

FEdvid::FEdvid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FEdvid)
{
    ui->setupUi(this);
//    model=0;
    pr_close=true;
    pr_break=false;

    model=new QSqlQueryModel(this);

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnQuit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnVid,SIGNAL(clicked(bool)),this,SLOT(on_btnVid()));
    connect(ui->btnClear,SIGNAL(clicked(bool)),this,SLOT(on_btnClear()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
}

FEdvid::~FEdvid()
{
    delete ui;
}

void FEdvid::showEvent(QShowEvent *event)
{
 this->setWindowTitle("Список единиц выдачи для запроса - "+cOI->zapzapr.naz);
 ui->pbVid->setMinimum(0);
 ui->pbVid->setValue(0);

 //if (model == 0)  model=new QSqlQueryModel(this);

 str_query="SELECT edvid.nz,edvid.naz_edxr,tipedvid.opis,to_char(edvid.dvid,'DD-MM-YYYY HH24:MI:SS') "
           "AS sdvid FROM edvid,tipedvid WHERE edvid.tip = tipedvid.tip AND edvid.nz_zapr = "+QString::number(cOI->zapzapr.nz);
 cOI->kol_htable=4;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название единицы"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Тип единицы"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Дата выдачи"); cOI->hidcol[3]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
 pr_close=true;
 pr_break=false;
 btnVis(true);
 event->accept();
 return;
}

void FEdvid::on_btnRefresh()
{
 ind=ui->tableView->currentIndex();
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FEdvid::on_btnVid()
{
 QList<qlonglong> list_nz;
 QByteArray buf;
 QString path1,path2,file,dat,dt,vfile;
 QDir dir;
 QFile f;
 QStringList fi;
 qint32 istr;

 if (cOI->zapzapr.path.isEmpty())
 {
  QMessageBox::critical(0,"Выдача","Установите директорию для выдачи!",QMessageBox::Yes);
  return;
 }

 dat=QDate::currentDate().toString("dd_MM_yyyy");
 cOI->UpdateRow("zapr","dtt",dat,cOI->zapzapr.nz,false);

 list_nz=cOI->FindRowForEdvid(cOI->zapzapr.nz,true);
 if (list_nz.isEmpty()) return;

 ui->pbVid->setMinimum(0);
 ui->pbVid->setMaximum(list_nz.count()-1);
 ui->pbVid->setValue(0);

 dt=QDateTime::currentDateTime().toString("dd_MM_yyyy hh:mm:ss");
 path1=cOI->zapzapr.path;

 pr_close=false;
 btnVis(false);
 for (int i=0; i<list_nz.count(); i++)
 {
  if (pr_break) break;

  path2=path1;
  if (!cOI->ReadEdvid(list_nz[i],false)) {pr_close=true;return;}
  if ((cOI->zapedvid.tip == "KVI") || (cOI->zapedvid.tip == "KVNO")) path2=path1+"/DDS";
  if (cOI->zapedvid.tip == "FULL") path2=path1+"/PPSI";
  if ((cOI->zapedvid.tip == "ALTI") || (cOI->zapedvid.tip == "CAL1") || (cOI->zapedvid.tip == "CAL2")) path2=path1+"/VRV";
  if ((cOI->zapedvid.tip == "KOS") || (cOI->zapedvid.tip == "FRD")) path2=path1+"/KOS";
  if ((cOI->zapedvid.tip == "DS") || (cOI->zapedvid.tip == "SPO")) path2=path1+"/DS";
  if (cOI->zapedvid.tip == "DZS") path2=path1+"/DZS";
  if (cOI->zapedvid.tip == "I_M") path2=path1+"/METEO";
  if ((cOI->zapedvid.tip == "I_O") || (cOI->zapedvid.tip == "I_N") || (cOI->zapedvid.tip == "I_G") || (cOI->zapedvid.tip == "I_L")) path2=path1+"/ISX_BDS";
  if ((cOI->zapedvid.tip == "V_O") || (cOI->zapedvid.tip == "V_N") || (cOI->zapedvid.tip == "V_G")) path2=path1+"/BDS";
  if (cOI->zapedvid.tip == "SP3") path2=path1+"/SP3";
  if (cOI->zapedvid.tip == "SOL") path2=path1+"/SOL";
  if (path2.isEmpty()) continue;
  if (!cOI->ReadEdxrInBuf("edvid",list_nz[i],buf,false)) continue;
  dir.setPath(path2);
  if (!dir.exists()) dir.mkpath(path2);
  file=path2+"/"+cOI->readedxr.isxf;
  if (cOI->WriteFile(file,buf,true) == -1) continue;
  if ((cOI->zapzapr.rnx2) && (cOI->zapedvid.tip == "V_O")) if (!cOI->RnxToRnx(file)) {cOI->DelFile(file); continue;}
  if ((cOI->zapzapr.rnx2) && (cOI->zapedvid.tip == "V_N")) if (!cOI->RnxToRnx(file)) {cOI->DelFile(file); continue;}
  if ((cOI->zapzapr.rnx2) && (cOI->zapedvid.tip == "V_G")) if (!cOI->RnxToRnx(file)) {cOI->DelFile(file); continue;}
  if ((cOI->zapzapr.pr_alti) && (cOI->zapedvid.tip == "ALTI"))
  {
   fi=cOI->FileInfo(file);
   vfile=fi[3];
   istr=vfile.lastIndexOf('_');
   vfile=vfile.mid(0,istr);
   vfile=fi[1]+"/"+vfile+"_UGN0."+fi[4];
   QFile::rename(file,vfile);
  }
  dt=QDateTime::currentDateTime().toString("dd_MM_yyyy hh:mm:ss");
  cOI->UpdateRow("edvid","dvid",dt,list_nz[i],false);
  ui->pbVid->setValue(i);
  this->repaint();
  qApp->processEvents();
 }
 pr_close=true;
 if (pr_break) {this->close(); return;}

 ui->pbVid->setValue(0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);

 QMessageBox::information(0,"Выдача","Выдача информации выполнена успешно",QMessageBox::Yes);

 return;
}

void FEdvid::on_btnClear()
{
 cOI->DelEdvid(cOI->zapzapr.nz,true);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 return;
}

void FEdvid::on_btnDel()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->DeleteRow("edvid",nz,true);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FEdvid::closeEvent(QCloseEvent *event)
{
 if (pr_close) event->accept();
 else {pr_break=true;event->ignore();}
 return;
}

void FEdvid::btnVis(bool pr)
{
 ui->btnClear->setEnabled(pr);
 ui->btnDel->setEnabled(pr);
 return;
}
