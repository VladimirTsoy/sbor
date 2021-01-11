#include "fustnf.h"
#include "ui_fustnf.h"

FUstnf::FUstnf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FUstnf)
{
    ui->setupUi(this);

    connect(ui->pb_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pb_ok,SIGNAL(clicked(bool)),this,SLOT(on_pbok()));
    connect(ui->pb_isx,SIGNAL(clicked(bool)),this,SLOT(on_pbisx()));
    connect(ui->pb_vxod,SIGNAL(clicked(bool)),this,SLOT(on_pbvxod()));
}

FUstnf::~FUstnf()
{
    delete ui;
}

void FUstnf::showEvent(QShowEvent *event)
{
 nz=cOI->FindRow("ustnf","komp",cOI->komp_name_ust,true);
 if (nz == 0)
 {
  QTimer::singleShot(100, this, SLOT(close()));
  return;
 }
 event->accept();
 QTimer::singleShot(100, this, SLOT(ShowForm()));
}

void FUstnf::ShowForm()
{
 if (nz < 1) this->close();

 cOI->ReadUstnf(nz,true);

 ui->le_komp->setText(cOI->zapustnf.komp);
 ui->le_isx->setText(cOI->zapustnf.isx);
 ui->le_vxod->setText(cOI->zapustnf.vxod);
 ui->cb_zvon->setChecked(cOI->zapustnf.zvon);
 return;
}

void FUstnf::on_pbok()
{
 cOI->zapustnf.isx=ui->le_isx->text();
 cOI->zapustnf.vxod=ui->le_vxod->text();
 if (ui->cb_zvon->isChecked()) cOI->zapustnf.zvon=1;
 else cOI->zapustnf.zvon=0;
 cOI->UpdateUstnf(true);
 this->close();
 return;
}

void FUstnf::on_pbisx()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"Директория с исходящими формами","");
 if (!sdir.isEmpty()) ui->le_isx->setText(sdir);
 ui->le_isx->setFocus();
 return;
}

void FUstnf::on_pbvxod()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"Директория с входящими формами","");
 if (!sdir.isEmpty()) ui->le_vxod->setText(sdir);
 ui->le_vxod->setFocus();
 return;
}
