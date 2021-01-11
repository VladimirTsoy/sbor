#include "fustzapr.h"
#include "ui_fustzapr.h"

FUstZapr::FUstZapr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FUstZapr)
{
    ui->setupUi(this);

    formPotr=new FPotr(this);
    formPotr->setWindowFlags(Qt::Window);

    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(Write()),Qt::UniqueConnection);
    connect(ui->tbtn_potr,SIGNAL(clicked()),this,SLOT(on_tbtnPotr()),Qt::UniqueConnection);
    connect(formPotr,SIGNAL(NzPotr(qlonglong)),this,SLOT(IzmPotr(qlonglong)));
    connect(ui->tbtn_path,SIGNAL(clicked()),this,SLOT(on_tbtnPath()),Qt::UniqueConnection);
    connect(ui->btn_Cancel,SIGNAL(clicked()),this,SLOT(Read()),Qt::UniqueConnection);
    connect(ui->btn_Exit,SIGNAL(clicked()),this,SLOT(close()),Qt::UniqueConnection);
//    connect(ui->cb_ispdtn,SIGNAL(clicked(bool)),this,SLOT(on_dtn()),Qt::UniqueConnection);
//    connect(ui->cb_ispdtk,SIGNAL(clicked(bool)),this,SLOT(on_dtn()),Qt::UniqueConnection);
    connect(ui->rb_ust,SIGNAL(clicked(bool)),this,SLOT(on_rb_ust()));
    connect(ui->rb_tek,SIGNAL(clicked(bool)),this,SLOT(on_rb_tek()));
    connect(ui->rb_teksut,SIGNAL(clicked(bool)),this,SLOT(on_rb_teksut()));

    //ui->cb_pravt->setVisible(false);

}

FUstZapr::~FUstZapr()
{
    delete ui;
}

void FUstZapr::Read()
{
 ui->le_naz->setText(cOI->zapzapr.naz);
 ui->dt_dat->setDateTime(cOI->zapzapr.dat);
 if (cOI->ReadPotr(cOI->zapzapr.nz_potr,false)) ui->le_nazpotr->setText(cOI->zappotr.naz);
 else ui->le_nazpotr->clear();
 ui->cb_pravt->setChecked(cOI->zapzapr.pr_avt);
 ui->cb_ds->setChecked(cOI->zapzapr.ds);
 ui->cb_dzs->setChecked(cOI->zapzapr.dzs);
 ui->cb_kos->setChecked(cOI->zapzapr.kos);
 ui->cb_meteo->setChecked(cOI->zapzapr.meteo);
 ui->cb_ppsi->setChecked(cOI->zapzapr.ppsi);
 ui->cb_kvi->setChecked(cOI->zapzapr.kvi);
 ui->cb_kvno->setChecked(cOI->zapzapr.kvno);
 ui->cb_alti->setChecked(cOI->zapzapr.alti);
 ui->cb_cal->setChecked(cOI->zapzapr.cal);
 ui->cb_bds_l->setChecked(cOI->zapzapr.bds_l);
 ui->cb_bds_o->setChecked(cOI->zapzapr.bds_o);
 ui->cb_bds_n->setChecked(cOI->zapzapr.bds_n);
 if (cOI->zapzapr.rnx2) ui->rb_2->setChecked(true);
 else ui->rb_3->setChecked(true);
 ui->le_path->setText(cOI->zapzapr.path);
 ui->dt_dtn->setDate(cOI->zapzapr.dtn);
 ui->dt_dtk->setDate(cOI->zapzapr.dtk);
 ui->dt_dtt->setDate(cOI->zapzapr.dtt);
 ui->le_path->setText(cOI->zapzapr.path);
 ui->cb_sp3->setChecked(cOI->zapzapr.sp3);
 ui->cb_sol->setChecked(cOI->zapzapr.sol);

 /*
 if (cOI->zapzapr.dtn.year() == 1900)
 {
  ui->cb_ispdtn->setChecked(true);
  ui->dt_dtn->setEnabled(false);
 }
 else
 {
  ui->cb_ispdtn->setChecked(false);
  ui->dt_dtn->setEnabled(true);
 }

 if (cOI->zapzapr.dtk.year() == 1900)
 {
  ui->cb_ispdtk->setChecked(true);
  ui->dt_dtk->setEnabled(false);
 }
 else
 {
  ui->cb_ispdtk->setChecked(false);
  ui->dt_dtk->setEnabled(true);
 }
*/

 ui->cb_pralti->setChecked(cOI->zapzapr.pr_alti);

 ui->le_kolsut->setText(QString::number(cOI->zapzapr.kolsut));
 if (cOI->zapzapr.pr_dat == 0) ui->rb_ust->setChecked(true);
 if (cOI->zapzapr.pr_dat == 1)
 {
  ui->rb_tek->setChecked(true);
  on_rb_tek();
 }
 if (cOI->zapzapr.pr_dat == 2)
 {
  ui->rb_teksut->setChecked(true);
  on_rb_teksut();
 }


 return;
}

void FUstZapr::showEvent(QShowEvent *event)
{
 Read();
 event->accept();
}

void FUstZapr::Write()
{
 cOI->zapzapr.naz=ui->le_naz->text();
 cOI->zapzapr.dat=ui->dt_dat->dateTime();
 cOI->zapzapr.pr_avt=ui->cb_pravt->isChecked();
 cOI->zapzapr.ds=ui->cb_ds->isChecked();
 cOI->zapzapr.dzs=ui->cb_dzs->isChecked();
 cOI->zapzapr.kos=ui->cb_kos->isChecked();
 cOI->zapzapr.meteo=ui->cb_meteo->isChecked();
 cOI->zapzapr.ppsi=ui->cb_ppsi->isChecked();
 cOI->zapzapr.kvi=ui->cb_kvi->isChecked();
 cOI->zapzapr.kvno=ui->cb_kvno->isChecked();
 cOI->zapzapr.alti=ui->cb_alti->isChecked();
 cOI->zapzapr.cal=ui->cb_cal->isChecked();
 cOI->zapzapr.bds_l=ui->cb_bds_l->isChecked();
 cOI->zapzapr.bds_o=ui->cb_bds_o->isChecked();
 cOI->zapzapr.bds_n=ui->cb_bds_n->isChecked();
 cOI->zapzapr.rnx2=ui->rb_2->isChecked();
 cOI->zapzapr.path=ui->le_path->text();
 cOI->zapzapr.sp3=ui->cb_sp3->isChecked();
 cOI->zapzapr.sol=ui->cb_sol->isChecked();

 cOI->zapzapr.dtn=ui->dt_dtn->date();
 cOI->zapzapr.dtk=ui->dt_dtk->date();
 cOI->zapzapr.dtt=ui->dt_dtt->date();

 cOI->zapzapr.pr_alti=ui->cb_pralti->isChecked();

 cOI->zapzapr.kolsut=ui->le_kolsut->text().toInt();
 if (ui->rb_ust->isChecked()) cOI->zapzapr.pr_dat=0;
 if (ui->rb_tek->isChecked()) cOI->zapzapr.pr_dat=1;
 if (ui->rb_teksut->isChecked()) cOI->zapzapr.pr_dat=2;

 cOI->UpdateZapr(false);
 emit SaveUstZapr();
 this->close();
 return;
}

void FUstZapr::on_tbtnPotr()
{
 formPotr->show_vib=true;
 formPotr->cOI=cOI;
 formPotr->show();
 return;
}

void FUstZapr::IzmPotr(qlonglong nz)
{
 cOI->zapzapr.nz_potr=nz;
 if (cOI->ReadPotr(cOI->zapzapr.nz_potr,false)) ui->le_nazpotr->setText(cOI->zappotr.naz);
 return;
}

void FUstZapr::on_tbtnPath()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"Путь к дирректории для выдачи","");

 if (sdir.isEmpty()) return;

 cOI->zapzapr.path=sdir;
 ui->le_path->setText(cOI->zapzapr.path);
 return;
}

/*
void FUstZapr::on_dtn()
{
 QDate d;

 d.setDate(1900,1,1);

 if (ui->cb_ispdtn->isChecked())
 {
  ui->dt_dtn->setDate(d);
  ui->dt_dtn->setEnabled(false);
 }
 else ui->dt_dtn->setEnabled(true);

 if (ui->cb_ispdtk->isChecked())
 {
  ui->dt_dtk->setDate(d);
  ui->dt_dtk->setEnabled(false);
 }
 else ui->dt_dtk->setEnabled(true);
 return;
}
*/

void FUstZapr::on_rb_ust()
{
 ui->dt_dtk->setEnabled(true);
 return;
}

void FUstZapr::on_rb_tek()
{
 ui->dt_dtk->setDate(QDate::currentDate());
 ui->dt_dtk->setEnabled(false);
 return;
}

void FUstZapr::on_rb_teksut()
{
 ui->dt_dtk->setDate(QDate::currentDate().addDays(-cOI->zapzapr.kolsut));
 ui->dt_dtk->setEnabled(false);
 return;
}
