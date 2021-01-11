#include "frpom.h"
#include "ui_frpom.h"

FRPom::FRPom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FRPom)
{
    ui->setupUi(this);

    connect(ui->pbCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pbVib,SIGNAL(clicked(bool)),this,SLOT(on_pbVib()));
    connect(ui->pbOtbr,SIGNAL(clicked(bool)),this,SLOT(on_pbOtbr()));
    connect(ui->pbRun,SIGNAL(clicked(bool)),this,SLOT(on_pbRun()));
}

FRPom::~FRPom()
{
    delete ui;
}

void FRPom::showEvent(QShowEvent *event)
{
 ui->leKol->setText("0");
 ui->leOtbr->clear();
 ui->leKolbrak->setText("0");
 ui->prbar->setMinimum(0);
 ui->prbar->setMaximum(0);
 ui->prbar->setValue(0);
 ui->prbar->setTextVisible(false);
 ui->cbErr->setChecked(false);

 sp_file.clear();

 pr_close=true;

 event->accept();
}

void FRPom::on_pbVib()
{
 sp_file=QFileDialog::getOpenFileNames(0,"Выбирайте файлы",cOI->zapust.path_si,"*.*");
 if (sp_file.isEmpty()) return;

 ui->leKol->setText(QString::number(sp_file.count()));
 ui->prbar->setMaximum(sp_file.count());
 ui->prbar->setValue(0);
 ui->prbar->setTextVisible(true);
}

void FRPom::on_pbOtbr()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"","");
 if (sdir.isEmpty()) return;

 ui->leOtbr->setText(sdir);
}

void FRPom::on_pbRun()
{
 QStringList info;
 qulonglong nz;
 QFile f;
 qint32 kolotbr;
 bool prsoob;

 if (ui->leOtbr->text().isEmpty()) return;
 ui->leKolbrak->setText("0");

 kolotbr=0;
 prsoob=false;
 ui->pbCancel->setEnabled(false);

 for (qint32 i=0; i<sp_file.count(); i++)
 {
  pr_close=false;
  info=cOI->FileInfo(sp_file[i]);
  prsoob=ui->cbErr->isChecked();

  cOI->ClearSist();
  cOI->zapsist.nz_kodobr=23;
  cOI->zapsist.naz_komp=cOI->zapust.nazkomp;
  cOI->zapsist.polz=cOI->nikp;
  cOI->zapsist.naz_ed=info[2];
  if (cOI->ZapEduch(sp_file[i],nz,false,prsoob))
  {
   cOI->DelFile(sp_file[i]);
   cOI->zapsist.sost="выполнено";
  }
  else
  {
   cOI->zapsist.sost="не выполнено";
   f.setFileName(ui->leOtbr->text()+"/"+info[2]);
   if (f.exists()) f.remove();

   if (QFile::copy(sp_file[i],ui->leOtbr->text()+"/"+info[2])) cOI->DelFile(sp_file[i]);
   kolotbr++;
   ui->leKolbrak->setText(QString::number(kolotbr));
  }
  cOI->InsertSist(false);
  ui->prbar->setValue(i+1);
  qApp->processEvents();
 }

 ui->leKol->setText("0");
 ui->leOtbr->clear();
 ui->prbar->setMinimum(0);
 ui->prbar->setMaximum(0);
 ui->prbar->setValue(0);
 ui->prbar->setTextVisible(false);

 sp_file.clear();

 ui->pbCancel->setEnabled(true);
 pr_close=true;
}

void FRPom::closeEvent(QCloseEvent *event)
{
 if (pr_close) event->accept();
 else event->ignore();
}
