#include "feditnos.h"
#include "ui_feditnos.h"

FEditNos::FEditNos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FEditNos)
{
    ui->setupUi(this);
    connect(ui->btnZap,SIGNAL(clicked(bool)),this,SLOT(on_btnZap()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnPath,SIGNAL(clicked(bool)),this,SLOT(on_btnPath()));
}

FEditNos::~FEditNos()
{
    delete ui;
}

void FEditNos::showEvent(QShowEvent *event)
{
 Read();
 event->accept();
}

void FEditNos::Read()
{
 if (nz == 0)
 {
  ui->lePath->clear();
  ui->cbSost->setCurrentIndex(-1);
  return;
 }

 if (!cOI->ReadNos(nz,true))
 {
  ui->lePath->clear();
  ui->cbSost->setCurrentIndex(-1);
  return;
 }

 ui->lePath->setText(cOI->zapnos.naz);
 ui->cbSost->setCurrentIndex(cOI->zapnos.prblok);

 return;
}

void FEditNos::on_btnZap()
{
 Write();
 emit ok();
 this->close();
 return;
}

void FEditNos::Write()
{
 qlonglong nz_nos;

 cOI->zapnos.naz=ui->lePath->text();
 cOI->zapnos.prblok=ui->cbSost->currentIndex();
 if ((cOI->zapnos.prblok < 0) || (cOI->zapnos.prblok > 2)) return;

 if (nz == 0)
 {
  nz_nos=cOI->FindRow("nos","naz",cOI->zapnos.naz,false);
  if (nz_nos > 0)
  {
   QMessageBox::critical(0,"Ошибка","Установленный путь к носителю уже существует.",QMessageBox::Yes);
   return;
  }
  cOI->InsertNos(true);
 }
 else cOI->UpdateNos(true);

 return;
}

void FEditNos::on_btnPath()
{
 quint64 nz_edxr;

 nz_edxr=cOI->FindRow("edxr","nz_nos",nz,true);
 if (nz_edxr > 0) return;

 QString str=QFileDialog::getExistingDirectory(0,tr("Выбирайте директорию"),"");
 if (str.isEmpty()) return;

 ui->lePath->setText(str);
 return;
}
