#include "fseansds.h"
#include "ui_fseansds.h"

FSeansDs::FSeansDs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSeansDs)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);
    filtr.clear();
    tip.clear();
    ui->de_ns->setDate(QDate::currentDate());
    ui->de_ks->setDate(QDate::currentDate());

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
//    connect(ui->cb_ns,SIGNAL(clicked(bool)),this,SLOT(Obr_cb_ns()));
    connect(ui->btnFiltr,SIGNAL(clicked(bool)),this,SLOT(on_btnFiltr()));
    connect(ui->cb_ns,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbNo,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbSut,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbMes,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbGod,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
}

FSeansDs::~FSeansDs()
{
    delete ui;
}

void FSeansDs::FormSpis1()
{
 QDate no,ko;
 qint32 nd,kd;
 QString  tipstat;

 tipstat.clear();
 if (tip == "DS") tipstat="stat313";
 if (tip == "DZS") tipstat="stat322";

 str_query="WITH spis1 AS (SELECT ds.nz,ds.naz,ds.npn,ds.nka,ds.dtn,ds.dtk,";
 str_query=str_query+tipstat+".kizm,100-(("+tipstat+".kizmn*100)/"+tipstat+".kizm)";
 str_query=str_query+" AS proc_izm,ds.godn,to_number((SUBSTRING(ds.dtn from 4 for 2)),'999') AS mesn,SUBSTRING(ds.dtn from 1 for 10) AS dts,"
           "ds.sutn FROM ds LEFT OUTER JOIN "+tipstat+" ON (ds.nz = "+tipstat+".nz_educh) WHERE ds.nazis = '"+tip+"' ";

 if (!filtr.isNull()) str_query=str_query+"AND ds.npn = '"+filtr+"' ";

 if (ui->cb_ns->isChecked())
 {
  no=ui->de_ns->date();
  ko=ui->de_ks->date();
  nd=(no.year()*1000)+no.dayOfYear();
  kd=(ko.year()*1000)+ko.dayOfYear();
  str_query=str_query+"AND ((ds.godn*1000)+ds.sutn BETWEEN "+QString::number(nd)+" AND "+QString::number(kd)+"))";
 }
 else str_query=str_query+")";
}

void FSeansDs::spis_gr_no() //общий список сеансов
{
 str_query=str_query+"SELECT npn,naz,nka,dtn,dtk,kizm,proc_izm FROM spis1 ORDER BY npn,nka,dtn";

 cOI->kol_htable=7;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("Название сеанса"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ КА"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Дата,время начала сеанса"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Дата,время конца сеанса"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Общее количество измерений"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("% достоверных измерений"); cOI->hidcol[6]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansDs::showEvent(QShowEvent *event)
{
 if (tip == "DS") this->setWindowTitle("Первичная оценка сеансов ДС");
 if (tip == "DZS") this->setWindowTitle("Первичная оценка сеансов ДЗС");
 ui->btnFiltr->setChecked(false);
 filtr.clear();
 model->clear();
// Obr_cb_ns();
 on_btnRefresh();
 event->accept();
}

void FSeansDs::on_btnRefresh()
{
 QDate dn,dk;

 dn.setDate(1900,1,1);
 dk.setDate(2999,1,1);

 if (ui->cb_ns->isChecked())
 {
  dn=ui->de_ns->date();
  dk=ui->de_ks->date();
  ui->de_ns->setEnabled(true);
  ui->de_ks->setEnabled(true);
 }
 else
 {
  ui->de_ns->setEnabled(false);
  ui->de_ks->setEnabled(false);
 }

 FormSpis1();
 if (ui->rbNo->isChecked()) spis_gr_no();
 if (ui->rbSut->isChecked()) spis_gr_sut();
 if (ui->rbMes->isChecked()) spis_gr_mes();
 if (ui->rbGod->isChecked()) spis_gr_god();
}

/*
void FSeansDs::Obr_cb_ns()
{
 if (ui->cb_ns->isChecked())
 {
  ui->de_ns->setEnabled(true);
  ui->de_ks->setEnabled(true);
 }
 else
 {
  ui->de_ns->setEnabled(false);
  ui->de_ks->setEnabled(false);
 }
}
*/

void FSeansDs::spis_gr_sut()
{
 str_query=str_query+"SELECT npn,nka,dts,sum(kizm) AS sum_izm,floor(avg(proc_izm)) AS avg_proc FROM spis1 "
                     "GROUP BY npn,nka,dts ORDER BY dts,npn,nka";

 cOI->kol_htable=5;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Дата"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Сумма количества измерений"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Средний % достоверных измерений"); cOI->hidcol[4]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansDs::spis_gr_mes()
{
 str_query=str_query+"SELECT npn,nka,mes.opis,godn,sum(kizm) AS sum_izm,floor(avg(proc_izm)) AS avg_proc,mesn FROM spis1,mes "
                     "WHERE mesn = mes.nz GROUP BY npn,nka,mesn,godn,mes.opis ORDER BY npn,nka,godn,mesn";

 cOI->kol_htable=7;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Месяц"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Год"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Сумма количества измерений"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Средний % достоверных измерений"); cOI->hidcol[5]=false;
 cOI->hidcol[6]=true;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansDs::spis_gr_god()
{
 str_query=str_query+"SELECT npn,nka,godn,sum(kizm) AS sum_izm,floor(avg(proc_izm)) AS avg_proc FROM spis1 GROUP BY npn,nka,godn "
                     "ORDER BY godn,npn,nka";

 cOI->kol_htable=5;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Год"); cOI->hidcol[3]=false;
 cOI->htable[3]=tr("Сумма количества измерений"); cOI->hidcol[4]=false;
 cOI->htable[4]=tr("Средний % достоверных измерений"); cOI->hidcol[5]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansDs::on_btnFiltr()
{
 QString zn;

 if (ui->btnFiltr->isChecked())
 {
  ind=cOI->getCurrentIndex(ui->tableView,model,0,zn);
  if (zn.isEmpty())
  {
   ui->btnFiltr->setChecked(false);
   return;
  }
  filtr=zn;
 }
 else filtr.clear();

 on_btnRefresh();

}
