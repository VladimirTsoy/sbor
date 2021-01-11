#include "feditserv.h"
#include "ui_feditserv.h"

FEditServ::FEditServ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FEditServ)
{
    ui->setupUi(this);

    connect(ui->pbSave,SIGNAL(clicked(bool)),this,SLOT(on_pbSave()));
    connect(ui->pb_isdir,SIGNAL(clicked(bool)),this,SLOT(on_pbIsdir()));
    connect(ui->pb_vixdir,SIGNAL(clicked(bool)),this,SLOT(on_pbVixdir()));
    connect(ui->pb_copdir,SIGNAL(clicked(bool)),this,SLOT(on_pbCopdir()));
    connect(ui->pbCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
}

FEditServ::~FEditServ()
{
    delete ui;
}

void FEditServ::showEvent(QShowEvent *event)
{
 bool fl;

 ui->le_opis->clear();
 ui->le_isdir->clear();
 ui->le_vixdir->clear();
 ui->le_copdir->clear();
 ui->le_maska->clear();
 ui->cb_copdobr->setChecked(false);
 ui->cb_rarx->setChecked(false);
 ui->cb_rhata->setChecked(false);
 ui->cb_uhata->setChecked(false);
 ui->cb_uarx->setChecked(false);
 ui->cb_coppobr->setChecked(false);
 ui->cb_delisx->setChecked(false);
 ui->cb_prvd->setChecked(false);
 ui->cb_prcd->setChecked(false);
 ui->le_maskvd->clear();
 ui->le_maskcd->clear();
 ui->rb_ns->setChecked(true);
 ui->rb_nsc->setChecked(true);

 cOI->ClearServ();

 cOI->zapserv.nazkomp=cOI->komp_name_ust;
 if (nz > 0) cOI->ReadServ(nz,true);
 ui->le_opis->setText(cOI->zapserv.opis);
 ui->le_isdir->setText(cOI->zapserv.isdir);
 ui->le_vixdir->setText(cOI->zapserv.vixdir);
 ui->le_copdir->setText(cOI->zapserv.copdir);
 ui->le_maska->setText(cOI->zapserv.maska);
 fl=cOI->zapserv.prvd;
 ui->cb_prvd->setChecked(fl);
 fl=cOI->zapserv.prcd;
 ui->cb_prcd->setChecked(fl);
 ui->le_maskvd->setText(cOI->zapserv.maskvd);
 ui->le_maskcd->setText(cOI->zapserv.maskcd);
 if (cOI->zapserv.sdir == 1) ui->rb_dat->setChecked(true);
 if (cOI->zapserv.sdir == 2) ui->rb_god->setChecked(true);
 if (cOI->zapserv.sdir == 3) ui->rb_sut->setChecked(true);
 if (cOI->zapserv.sdirc == 1) ui->rb_datc->setChecked(true);
 if (cOI->zapserv.sdirc == 2) ui->rb_godc->setChecked(true);
 if (cOI->zapserv.sdirc == 3) ui->rb_sutc->setChecked(true);

 cOI->btu1.byte=cOI->zapserv.kobr;
 fl=cOI->btu1.bits.bit1;
 ui->cb_copdobr->setChecked(fl);
 fl=cOI->btu1.bits.bit2;
 ui->cb_coppobr->setChecked(fl);
 fl=cOI->btu1.bits.bit3;
 ui->cb_delisx->setChecked(fl);

 cOI->btu1.byte=cOI->zapserv.karx;
 fl=cOI->btu1.bits.bit1;
 ui->cb_rarx->setChecked(fl);
 fl=cOI->btu1.bits.bit2;
 ui->cb_rhata->setChecked(fl);
 fl=cOI->btu1.bits.bit3;
 ui->cb_uhata->setChecked(fl);
 fl=cOI->btu1.bits.bit4;
 ui->cb_uarx->setChecked(fl);

 event->accept();
}

void FEditServ::FormZap()
{
 cOI->zapserv.opis=ui->le_opis->text();
 cOI->zapserv.isdir=ui->le_isdir->text();
 cOI->zapserv.vixdir=ui->le_vixdir->text();
 cOI->zapserv.copdir=ui->le_copdir->text();
 cOI->zapserv.maska=ui->le_maska->text();
 cOI->zapserv.maskvd=ui->le_maskvd->text();
 cOI->zapserv.maskcd=ui->le_maskcd->text();
 cOI->zapserv.prvd=ui->cb_prvd->isChecked();
 cOI->zapserv.prcd=ui->cb_prcd->isChecked();

 cOI->zapserv.sdir=0;
 if  (ui->rb_dat->isChecked()) cOI->zapserv.sdir=1;
 if  (ui->rb_god->isChecked()) cOI->zapserv.sdir=2;
 if  (ui->rb_sut->isChecked()) cOI->zapserv.sdir=3;
 cOI->zapserv.sdirc=0;
 if  (ui->rb_datc->isChecked()) cOI->zapserv.sdirc=1;
 if  (ui->rb_godc->isChecked()) cOI->zapserv.sdirc=2;
 if  (ui->rb_sutc->isChecked()) cOI->zapserv.sdirc=3;

 cOI->btu1.byte=0;
 cOI->btu1.bits.bit1=ui->cb_copdobr->isChecked();
 cOI->btu1.bits.bit2=ui->cb_coppobr->isChecked();
 cOI->btu1.bits.bit3=ui->cb_delisx->isChecked();
 cOI->zapserv.kobr=cOI->btu1.byte;

 cOI->btu1.byte=0;
 cOI->btu1.bits.bit1=ui->cb_rarx->isChecked();
 cOI->btu1.bits.bit2=ui->cb_rhata->isChecked();
 cOI->btu1.bits.bit3=ui->cb_uhata->isChecked();
 cOI->btu1.bits.bit4=ui->cb_uarx->isChecked();
 cOI->zapserv.karx=cOI->btu1.byte;
}

void FEditServ::on_pbSave()
{
 FormZap();

 if (ui->cb_copdobr->isChecked())
 {
  if (ui->le_copdir->text().isEmpty()) return;
 }
 if (ui->cb_coppobr->isChecked())
 {
  if (ui->le_copdir->text().isEmpty()) return;
 }
 if (ui->cb_prvd->isChecked())
 {
  if (ui->le_maskvd->text().isEmpty()) return;
 }
 if (ui->cb_prcd->isChecked())
 {
  if (ui->le_maskcd->text().isEmpty()) return;
 }

 if (nz == 0) cOI->InsertServ(true);
 else cOI->UpdateServ(true);
 emit ok();
 this->close();
}

void FEditServ::on_pbIsdir()
{
 QString str=QFileDialog::getExistingDirectory(0,tr("Выбирайте директорию"),"");
 if (str == "") return;

 ui->le_isdir->setText(str);
}

void FEditServ::on_pbVixdir()
{
 QString str=QFileDialog::getExistingDirectory(0,tr("Выбирайте директорию"),"");
 if (str == "") return;

 ui->le_vixdir->setText(str);
}

void FEditServ::on_pbCopdir()
{
 QString str=QFileDialog::getExistingDirectory(0,tr("Выбирайте директорию"),"");
 if (str == "") return;

 ui->le_copdir->setText(str);
}
