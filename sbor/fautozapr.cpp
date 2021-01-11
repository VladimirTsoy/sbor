#include "fautozapr.h"
#include "ui_fautozapr.h"

FAutoZapr::FAutoZapr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FAutoZapr)
{
    ui->setupUi(this);

    connect(ui->pb_Otm,SIGNAL(clicked(bool)),this,SLOT(on_Otm()));
    connect(ui->pb_Obr,SIGNAL(clicked(bool)),this,SLOT(on_Obr()));
    connect(ui->pb_Abort,SIGNAL(clicked(bool)),this,SLOT(on_Abort()));
}

FAutoZapr::~FAutoZapr()
{
    delete ui;
}

void FAutoZapr::showEvent(QShowEvent *event)
{
 nz_zapr=cOI->FindRowAvtZapr(true,cOI->komp_name_ust);
 pr_close=false;
 pr_obr=true;
 ui->le_kolzap->setText(QString::number(nz_zapr.count()));
 ui->pb->setMaximum(nz_zapr.count()-1);
 ui->pb->setValue(0);
 ui->pb->setVisible(false);
 ui->l_inf->clear();
 ui->l_inf->setVisible(false);
 ui->pb_Abort->setVisible(false);
 ui->pb_Obr->setVisible(true);
 ui->pb_Otm->setVisible(true);
 event->accept();
}

void FAutoZapr::closeEvent(QCloseEvent *event)
{
 if (!pr_close) event->ignore();
 else event->accept();
}

void FAutoZapr::on_Otm()
{
 pr_close=true;
 this->close();
 return;
}

void FAutoZapr::on_Obr()
{
 QByteArray buf;
 QString path1,path2,file,dt,vfile;
 QDir dir;
 QFile f;
 QStringList fi;
 qint32 istr;

 ui->pb->setVisible(true);
 ui->l_inf->setVisible(true);
 ui->pb_Abort->setVisible(true);
 ui->pb_Obr->setVisible(false);
 ui->pb_Otm->setVisible(false);

 for (qint32 i=0; i<nz_zapr.count(); i++)
 {
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->ReadZapr(nz_zapr.at(i),true);
  ui->l_inf->setText("Обновление запроса - "+cOI->zapzapr.naz);

  //выполнение функции обновления запроса
  cOI->EdvidForDs(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForDds(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForPpsi(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForRnxi(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForRnxv(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForSp3(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->EdvidForSol(nz_zapr.at(i),false);
  if (!pr_obr) {pr_close=true; this->close(); return;}

  ui->pb->setValue(i+1);
 }

 ui->pb->setValue(0);
 for (qint32 i=0; i<nz_zapr.count(); i++)
 {
  if (!pr_obr) {pr_close=true; this->close(); return;}
  cOI->ReadZapr(nz_zapr.at(i),true);
  ui->l_inf->setText("Выдача запроса - "+cOI->zapzapr.naz);

  //выполнение функции выдачи запроса
  if (cOI->zapzapr.path.isEmpty())
  {
   QMessageBox::critical(0,"Выдача","Установите директорию для выдачи!",QMessageBox::Yes);
   continue;
  }

  cOI->UpdateRow("zapr","dtt",QDate::currentDate().toString("dd_MM_yyyy"),cOI->zapzapr.nz,false);

  list_nz=cOI->FindRowForEdvid(cOI->zapzapr.nz,true);
  if (list_nz.isEmpty()) continue;

  ui->pb->setMinimum(0);
  ui->pb->setMaximum(list_nz.count()-1);
  ui->pb->setValue(0);

  dt=QDateTime::currentDateTime().toString("dd_MM_yyyy hh:mm:ss");
  path1=cOI->zapzapr.path;

  for (qint32 j=0; j<list_nz.count(); j++)
  {

   if (!pr_obr) {pr_close=true; this->close(); return;}
   path2=path1;
   if (!cOI->ReadEdvid(list_nz[j],false)) continue;
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
   if (!cOI->ReadEdxrInBuf("edvid",list_nz[j],buf,false)) continue;
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
   cOI->UpdateRow("edvid","dvid",dt,list_nz[j],false);
   ui->pb->setValue(j);
   this->repaint();
   qApp->processEvents();
  }

  //  ui->pb->setValue(i+1);
 }

 ui->pb->setValue(0);
 ui->pb->setVisible(false);
 ui->l_inf->clear();
 ui->l_inf->setVisible(false);
 ui->pb_Abort->setVisible(false);
 ui->pb_Obr->setVisible(true);
 ui->pb_Otm->setVisible(true);
 return;
}

void FAutoZapr::on_Abort()
{
 ui->l_inf->setText("Завершение обработки. Ждите.");
 pr_obr=false;
 return;
}
