#include "feditfull.h"
#include "ui_feditfull.h"

FEditFull::FEditFull(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FEditFull)
{
    ui->setupUi(this);

    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnVost,SIGNAL(clicked(bool)),this,SLOT(Read()));
    connect(ui->btnOk,SIGNAL(clicked(bool)),this,SLOT(Write()));

}

FEditFull::~FEditFull()
{
    delete ui;
}

void FEditFull::Read()
{
 ui->le_seans->setText("Ошибка данных");
 ui->sb_kan->clear();
 ui->sb_nka->clear();
 ui->dt_dtn->clear();
 ui->dt_dtk->clear();
 ui->cb_sbr->setChecked(false);
 ui->cb_sbr->setText("");
 ui->le_arxf->clear();
 ui->le_isxf->clear();
 ui->btnOk->setEnabled(false);
 ui->btnVost->setEnabled(false);

 if (!cOI->ReadFullppsi(nz_full,true)) return;
 if (!cOI->ReadEdxr(cOI->zapfull.nz_edxr,true)) return;

 ui->le_seans->setText(cOI->zapfull.naz);
 ui->sb_kan->setValue(cOI->zapfull.kan);
 ui->sb_nka->setValue(cOI->zapfull.nka.toInt());
 ui->dt_dtn->setDateTime(QDateTime::fromString(cOI->zapfull.dtn,"dd-MM-yyyy hh:mm"));
 ui->dt_dtk->setDateTime(QDateTime::fromString(cOI->zapfull.dtk,"dd-MM-yyyy hh:mm"));
 ui->cb_sbr->setChecked(false);
 if (cOI->zapfull.kodobr == 5) ui->cb_sbr->setText("Повторить обработку");
 else ui->cb_sbr->setText("Отменить обработку");
 ui->le_arxf->setText(cOI->zapedxr.arxf);
 ui->le_isxf->setText(cOI->zapedxr.isxf);
 ui->btnOk->setEnabled(true);
 ui->btnVost->setEnabled(true);
}

void FEditFull::showEvent(QShowEvent *event)
{
 Read();
 event->accept();
}

void FEditFull::Write()
{
 QTime t;
 t.setHMS(0,0,0);
 bool pr1;

 pr1=false;

 if (cOI->zapfull.naz != ui->le_seans->text())
 {
  cOI->zapfull.naz=ui->le_seans->text();
  pr1=true;
 }
 if (cOI->zapfull.kan != ui->sb_kan->value())
 {
  cOI->zapfull.kan=ui->sb_kan->value();
  pr1=true;
 }
 if (cOI->zapfull.nka != QString::number(ui->sb_nka->value()))
 {
  cOI->zapfull.nka=QString::number(ui->sb_nka->value());
  pr1=true;
 }
 if (cOI->zapfull.dtn != ui->dt_dtn->dateTime().toString("dd-MM-yyyy hh:mm"))
 {
  cOI->zapfull.dtn=ui->dt_dtn->dateTime().toString("dd-MM-yyyy hh:mm");
  pr1=true;
 }
 if (cOI->zapfull.godn != ui->dt_dtn->date().year())
 {
  cOI->zapfull.godn=ui->dt_dtn->date().year();
  pr1=true;
 }
 if (cOI->zapfull.sutn != ui->dt_dtn->date().dayOfYear())
 {
  cOI->zapfull.sutn=ui->dt_dtn->date().dayOfYear();
  pr1=true;
 }
 if (cOI->zapfull.sekn != cOI->TimeToSec(ui->dt_dtn->time()))
 {
  cOI->zapfull.sekn=cOI->TimeToSec(ui->dt_dtn->time());
  pr1=true;
 }
 if (cOI->zapfull.dtk != ui->dt_dtk->dateTime().toString("dd-MM-yyyy hh:mm"))
 {
  cOI->zapfull.dtk=ui->dt_dtk->dateTime().toString("dd-MM-yyyy hh:mm");
  pr1=true;
 }
 if (cOI->zapfull.godk != ui->dt_dtk->date().year())
 {
  cOI->zapfull.godk=ui->dt_dtk->date().year();
  pr1=true;
 }
 if (cOI->zapfull.sutk != ui->dt_dtk->date().dayOfYear())
 {
  cOI->zapfull.sutk=ui->dt_dtk->date().dayOfYear();
  pr1=true;
 }
 if (cOI->zapfull.sekk != cOI->TimeToSec(ui->dt_dtk->time()))
 {
  cOI->zapfull.sekk=cOI->TimeToSec(ui->dt_dtk->time());
  pr1=true;
 }
 if (ui->cb_sbr->isChecked())
 {
  if (cOI->zapfull.kodobr == 5) cOI->zapfull.kodobr=11;
  else cOI->zapfull.kodobr=5;
  pr1=true;
 }

 if (pr1)
 {
  if (!cOI->UpdateFull(true)) return;
  emit ok();
 }

 if (cOI->zapedxr.isxf != ui->le_isxf->text())
 {
  cOI->zapedxr.isxf=ui->le_isxf->text();
  cOI->UpdateRow("edxr","isxf",cOI->zapedxr.isxf,cOI->zapedxr.nz,true);
 }

 Read();
}
