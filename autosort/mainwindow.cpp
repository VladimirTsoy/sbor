#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->showMinimized();

    cOI=new ObrIzm();
    formPass=new FPass(this);
    formPass->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint);

    connect(formPass,SIGNAL(no()),this,SLOT(Cl()));
    connect(formPass,SIGNAL(ok()),this,SLOT(Start()));
    connect(&t_si,SIGNAL(timeout()),SLOT(Obr()),Qt::UniqueConnection);

    QTimer::singleShot(100, this, SLOT(Pass()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Pass()
{
 formPass->cOI=cOI;
 formPass->title="AutoSort";
 formPass->show();
}

void MainWindow::Start()
{
 qlonglong nz;

 if (cOI->os == "WIN") QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

 nz=cOI->FindRow("serv","nazkomp",cOI->komp_name,true);
 if (nz <= 0) {pr_close=true; this->close(); return;}
 else cOI->ReadServ(nz,true);

 pr_close=false;
 pr_stop=false;

 ui->lb_vip->setText("Ожидание");
 ui->pb_vip->setMaximum(0);
 ui->pb_vip->setMinimum(0);
 ui->pb_vip->setValue(0);
 this->repaint();

 t_si.start(10*1000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
 if (!pr_close)
 {
  pr_stop=true;
  ui->lb_vip->setText("Завершение работы. Ждите.");
  this->repaint();
  event->ignore();
 }
 else
 {
  QApplication::quit();
  event->accept();
 }
 return;
}

void MainWindow::Cl()
{
 pr_close=true;
 this->close();
}

void MainWindow::Obr()
{
 QSqlQuery query;

 t_si.stop();

 if (pr_stop) {pr_close=true;this->close(); return;}

 strsql="SELECT * FROM serv WHERE nazkomp = '"+cOI->komp_name+"' ORDER BY opis";
 list=cOI->ExecQueryList(query,strsql,true);
 if (list.isEmpty()) {t_si.start(10*1000);return;}

 for (qint32 i=0; i<list.count(); i++)
 {
  if (!cOI->ReadServ(list[i],false)) continue;
  ObrZap();
 }

 t_si.start(10*1000);
}

void MainWindow::ObrZap()
{
 QDir dir;
 QString ifile,vfile,rfile,cfile,dfile;
 QList<qlonglong> dlf;
 QStringList inf_f;
 qlonglong df;
 QString str;
 qulonglong idlf;
 qint16 iks,iks1;
 qulonglong adlf;
 qint16 aks;
 QByteArray buf,buf1;
 QFile f;
 QProcess *proc;

 proc=new QProcess;

 dlf.clear();

 if (pr_stop) {pr_close=true;this->close(); return;}

 dir.setPath(cOI->zapserv.vixdir);
 if (!dir.exists()) return;

 cOI->btu1.byte=cOI->zapserv.kobr;

 if (cOI->btu1.bits.bit1 == 1)
 {
  dir.setPath(cOI->zapserv.copdir);
  if (!dir.exists()) return;
 }
 if (cOI->btu1.bits.bit2 == 1)
 {
  dir.setPath(cOI->zapserv.copdir);
  if (!dir.exists()) return;
 }

 dir.setPath(cOI->zapserv.isdir);
 if (!dir.exists()) return;

 if (cOI->zapserv.maska.isEmpty()) return;

 QStringList s=cOI->zapserv.maska.split(" ",QString::SkipEmptyParts);
 QFileInfoList listf=dir.entryInfoList(s,QDir::Files);
 if (listf.isEmpty()) return;


 ui->lb_vip->setText(cOI->zapserv.opis);
 ui->pb_vip->setMaximum(listf.count());
 ui->pb_vip->setValue(0);
 this->repaint();

 for (qint32 i=0; i<listf.count(); i++)
 {
  //определение длины выбранных файлов
  inf_f=cOI->FileInfo(listf.at(i).absoluteFilePath());
  str=inf_f[5];
  df=str.toLongLong();
  if (df == 0) continue;
  dlf << df;
 }

 if (pr_stop) {pr_close=true;this->close(); return;}

 QThread::sleep(10);

 for (qint32 i=0; i<listf.count(); i++)
 {
  if (pr_stop) {pr_close=true;this->close(); return;}
  ifile=listf.at(i).absoluteFilePath();
  inf_f=cOI->FileInfo(ifile);
  str=inf_f[5];
  df=str.toLongLong();
  if (df == 0) continue;
  if (dlf[i] != df) continue;
  buf.clear();
  buf=cOI->ReadFile(ifile);
  if (buf.isEmpty()) continue;


  vfile=cOI->zapserv.vixdir+"/"+listf.at(i).fileName();
  if (cOI->WriteFile(vfile,buf,true) < 0) continue;

  buf1.clear();
  if (!cOI->ArxFile(ifile,0,idlf,iks,adlf,aks,buf1)) continue;
  if (!cOI->ArxFile(vfile,0,idlf,iks1,adlf,aks,buf1)) continue;
  if (iks != iks1) {DelFileM(vfile); continue;}

  if (cOI->btu1.bits.bit1 == 1)
  {
   //скопировать до обработки
   cfile=cOI->zapserv.copdir+"/"+listf.at(i).fileName();
   if (cOI->WriteFile(cfile,buf,true) < 0) {DelFileM(cfile);continue;}
  }

  cOI->btu1_1.byte=cOI->zapserv.karx;

  if (cOI->btu1_1.bits.bit1 == 1)
  {
   //разархивировать
   if (cOI->os != "WIN")
   {
    if (vfile.right(1).toUpper() != "Z") {DelFileM(vfile);DelFileM(cfile);continue;}
    proc->start("uncompress "+vfile);
    proc->waitForFinished(-1);
    vfile=cOI->NameMask(vfile,"*[0,2]");
    f.setFileName(vfile);
    if (!f.exists()) {DelFileM(cfile);continue;}
   }
  }

  if (cOI->btu1_1.bits.bit2 == 1)
  {
   //разархивирование Хатанака
   if (vfile.right(1).toUpper() != "D") {DelFileM(vfile);DelFileM(cfile);continue;}
   cOI->RenameFile(vfile,"*.??O",dfile);
   if (cOI->UnCompress(vfile))
   {
    cOI->DelFile(vfile);
    f.setFileName(dfile);
    if (!f.exists()) {DelFileM(cfile);continue;}
    vfile=dfile;
   }
   else {DelFileM(vfile);DelFileM(dfile);DelFileM(cfile);continue;}
  }

  if (cOI->btu1_1.bits.bit3 == 1)
  {
   //архивирование Хатанака
   if (vfile.right(1).toUpper() != "O") {DelFileM(vfile);DelFileM(cfile);continue;}
   cOI->RenameFile(vfile,"*.??D",dfile);
   if (cOI->Compress(vfile))
   {
    DelFileM(vfile);
    f.setFileName(dfile);
    if (!f.exists()) {DelFileM(cfile);continue;}
    vfile=dfile;
   }
   else {DelFileM(vfile);DelFileM(dfile);DelFileM(cfile);continue;}
  }

  if (cOI->btu1_1.bits.bit4 == 1)
  {
   //архивировать
   if (cOI->os != "WIN")
   {
    proc->start("gzip -9 --suffix=.Z "+vfile);
    proc->waitForFinished(-1);
    vfile=cOI->NameMask(vfile,"*.Z");
    f.setFileName(vfile);
    if (!f.exists()) {DelFileM(cfile);continue;}
   }
  }

  if (cOI->zapserv.prvd == 1)
  {
   if (!cOI->RenameFile(vfile,cOI->zapserv.maskvd,rfile))
   {
    DelFileM(vfile);
    continue;
   }
   DelFileM(rfile);
   if (!QFile::rename(vfile,rfile))
   {
    DelFileM(vfile);
    continue;
   }
   vfile=rfile;
  }

  if (cOI->btu1.bits.bit2 == 1)
  {
   //скопировать после обработки
   buf1.clear();
   buf1=cOI->ReadFile(vfile);
   if (buf1.isEmpty()) continue;
//   vfile=cOI->zapserv.copdir+"/"+listf.at(i).fileName();
   inf_f=cOI->FileInfo(vfile);
   cfile=cOI->zapserv.copdir+"/"+inf_f[2];
   if (cOI->WriteFile(cfile,buf1,true) < 0) {DelFileM(cfile);continue;}
  }

  if (cOI->zapserv.prcd == 1)
  {
   if (!cOI->RenameFile(cfile,cOI->zapserv.maskcd,rfile))
   {
    DelFileM(cfile);
    continue;
   }
   DelFileM(rfile);
   if (!QFile::rename(cfile,rfile))
   {
    DelFileM(cfile);
    continue;
   }
  }

  if (cOI->btu1.bits.bit3 == 1)
  {
   if (!DelFileM(ifile)) continue;
  }
  cOI->ClearStatServ();
  cOI->zapstatserv.ifile=listf.at(i).fileName();
  cOI->zapstatserv.nz_serv=cOI->zapserv.nz;
  cOI->InsertStatServ(false);



  ui->pb_vip->setValue(i+1);
  this->repaint();
  qApp->processEvents();
  qDebug() << vfile;
 }

 ui->lb_vip->setText("Ожидание");
 ui->pb_vip->setMaximum(0);
 ui->pb_vip->setValue(0);
 this->repaint();

 qApp->processEvents();

}

bool MainWindow::DelFileM(QString nfile)
{
 for (qint32 i=0; i<10; i++)
 {
  if (cOI->DelFile(nfile)) return true;
 }

 return false;
}
