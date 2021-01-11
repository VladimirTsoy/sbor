#include "fseansppsi.h"
#include "ui_fseansppsi.h"

FSeansPpsi::FSeansPpsi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSeansPpsi)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);
    ui->de_ns->setDate(QDate::currentDate());
    ui->de_ks->setDate(QDate::currentDate());

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnStat,SIGNAL(clicked(bool)),this,SLOT(on_btnStat()));
    connect(ui->cb_ns,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbNo,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbSut,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbMes,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->rbGod,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));

    formStatFull=new FStatFull(this);
    formStatFull->setWindowFlags(Qt::Window);
}

FSeansPpsi::~FSeansPpsi()
{
    delete ui;
}

void FSeansPpsi::FormSpis1()
{
 QDate no,ko;
 qint32 nd,kd;

// str_query="WITH spis AS(SELECT fullppsi.nz AS nz_full,npn,naz,kan,nka,godn,sutn,sekn,dtn,dtk,(k+kbm+crc+pk) AS kadr_o,k "
 str_query="WITH spis AS(SELECT fullppsi.nz AS nz_full,npn,naz,kan,nka,godn,sutn,sekn,dtn,dtk,(kbm+crc) AS kadr_o,(k+kbm) AS kadr_i,k,kbm "
           "FROM fullppsi LEFT OUTER JOIN statfull ON (fullppsi.nz = statfull.nz_fullppsi) ";
 if (ui->cb_ns->isChecked())
 {
  no=ui->de_ns->date();
  ko=ui->de_ks->date();
  nd=(no.year()*1000)+no.dayOfYear();
  kd=(ko.year()*1000)+ko.dayOfYear();
  str_query=str_query+"WHERE ((fullppsi.godn*1000)+fullppsi.sutn BETWEEN "+QString::number(nd)+" AND "+QString::number(kd)+")), ";
 }
 else str_query=str_query+"), ";
// str_query=str_query+"proc AS(SELECT nz_full,npn,naz,kan,nka,godn,sutn,sekn,dtn,dtk,(CASE WHEN kadr_o <> 0 "
//                     "THEN (k*100)/kadr_o ELSE 0 END) AS pr FROM spis) ";
 str_query=str_query+"proc AS(SELECT nz_full,npn,naz,kan,nka,godn,sutn,sekn,dtn,dtk,(CASE WHEN kadr_i <> 0 "
                     "THEN (100-((kadr_o*100)/kadr_i)) ELSE 0 END) AS pr FROM spis) ";
}

void FSeansPpsi::spis_gr_no() //общий список сеансов
{
 str_query=str_query+"SELECT nz_full,naz,npn,nka,kan,dtn,dtk,pr FROM proc ORDER BY godn,sutn,floor(sekn/60),npn,nka,kan";

 cOI->kol_htable=8;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название сеанса"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ НПН"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("№ канала"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("% качественных кадров ППСИ"); cOI->hidcol[7]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansPpsi::on_btnRefresh()
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

void FSeansPpsi::showEvent(QShowEvent *event)
{
 model->clear();
 on_btnRefresh();
 event->accept();
}

void FSeansPpsi::spis_gr_sut()
{

 str_query=str_query+"SELECT npn,nka,kan,SUBSTRING(dtn from 1 for 10) AS dts,floor(avg(pr)) AS avg_pr "
                     "FROM proc GROUP BY kan,nka,npn,dts,godn,sutn ORDER BY godn,sutn,dts,npn,nka,kan";

 cOI->kol_htable=5;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ канала"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Дата"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("средний % качественных кадров ППСИ"); cOI->hidcol[4]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansPpsi::spis_gr_mes()
{

 str_query=str_query+"SELECT npn,nka,kan,opis,godn,floor(avg(pr)) AS avg_pr FROM proc,mes "
                     "WHERE to_number((SUBSTRING(dtn from 4 for 2)),'999') = mes.nz "
                     "GROUP BY godn,mes.nz,kan,nka,npn,opis ORDER BY godn,mes.nz,npn,nka,kan,opis";
 cOI->kol_htable=6;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ канала"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Месяц"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Год"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("средний % качественных кадров ППСИ"); cOI->hidcol[5]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansPpsi::spis_gr_god()
{

 str_query=str_query+"SELECT npn,nka,kan,godn,floor(avg(pr)) AS avg_pr FROM proc "
                     "GROUP BY godn,kan,nka,npn ORDER BY godn,npn,nka,kan";
 cOI->kol_htable=5;
 cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ канала"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Год"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("средний % качественных кадров ППСИ"); cOI->hidcol[4]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,str_query,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSeansPpsi::on_btnStat()
{
 qlonglong nz;

 if (!ui->rbNo->isChecked()) return;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formStatFull->cOI=cOI;
 formStatFull->nz_full=nz;
 formStatFull->show();
}
