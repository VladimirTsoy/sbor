#include "fseansdds.h"
#include "ui_fseansdds.h"

FSeansDds::FSeansDds(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSeansDds)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);

    formPotDds=new FPotDds(this);
    formPotDds->setWindowFlags(Qt::Window);

    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnGraf,SIGNAL(clicked(bool)),this,SLOT(on_btnGraf()));
}

FSeansDds::~FSeansDds()
{
    delete ui;
}

void FSeansDds::on_btnRefresh()
{
 QDate dn,dk;
 qint32 sutn,sutk;

 dn.setDate(1900,1,1);
 dk.setDate(2999,12,31);
 sutn=(dn.year()*1000)+dn.dayOfYear();
 sutk=(dk.year()*1000)+dk.dayOfYear();
 str_nka.clear();

 if (ui->cb_otb->isChecked())
 {
  dn.setDate(ui->cb_god->currentText().toInt(),ui->cb_mes->currentIndex()+1,1);
  dk=dn.addMonths(1);
  dk=dk.addDays(-1);
  sutn=(dn.year()*1000)+dn.dayOfYear();
  sutk=(dk.year()*1000)+dk.dayOfYear();
  str_nka=" AND nka = '"+ui->cb_nka->currentText()+"' ";
 }
 if (ui->cb_grgod->isChecked())
 {
  dn.setDate(ui->cb_god->currentText().toInt(),1,1);
  dk=dn.addYears(1);
  dk=dk.addDays(-1);
  sutn=(dn.year()*1000)+dn.dayOfYear();
  sutk=(dk.year()*1000)+dk.dayOfYear();
 }

 if (!cOI->RasStatDds(dn,dk,true)) return;

 if (!ui->cb_grgod->isChecked()) ViewSpis(sutn,sutk);
 else ViewSpisMes(dn.year());

 return;
}

void FSeansDds::ViewSpis(qint32 n,qint32 k)
{
 QString str;

 str="SELECT dds.nz,naz,dat,nka,eppol,round((eppol*100)/eppl) AS proc_ep,round(((ep_4rglo+ep_4bglo)*100)/eppl) AS proc_epg,"
     "godn,sutn FROM dds LEFT OUTER JOIN statdds ON (dds.nz = statdds.nz_dds) WHERE tip = 'KVI' AND kodobr = 0 "+str_nka;
 str=str+" AND ((godn*1000)+sutn BETWEEN "+QString::number(n)+" AND "+QString::number(k)+") ORDER BY godn ASC,sutn ASC";

    cOI->kol_htable=9;
    cOI->hidcol[0]=true;
    cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("Дата"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Получ.эпохи"); cOI->hidcol[4]=false;
    cOI->htable[5]=tr("% получ.эпох"); cOI->hidcol[5]=false;
    cOI->htable[6]=tr("% эпох ГЛОНАСС"); cOI->hidcol[6]=false;
    cOI->hidcol[7]=true; cOI->hidcol[7]=true;
    cOI->hidcol[8]=true; cOI->hidcol[8]=true;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str,true);
    ui->tableView->setCurrentIndex(ind);
}

void FSeansDds::ViewSpisMes(qint32 god)
{
 QString str;

 str="SELECT opis,nka,round(avg((eppol*100)/eppl)) AS proc_ep,round(avg(((ep_4rglo+ep_4bglo)*100)/eppl)) AS proc_epg,"
     "count(*) AS kol,godn,round((sum(eppol)*100)/(8639*date_part('days',"
     "date_trunc('month',to_timestamp(to_char(((godn*10000)+(mes.nz*100)+1),'99999999'),'YYYYMMDD'))"
     "+'1 MONTH'::interval-'1 DAY'::interval))) AS proc_ep3,round((sum(ep_4rglo+ep_4bglo)*100)/"
     "(8639*date_part('days',date_trunc('month',to_timestamp(to_char(((godn*10000)+(mes.nz*100)+1),"
     "'99999999'),'YYYYMMDD'))+'1 MONTH'::interval-'1 DAY'::interval))) AS proc_ep4,date_part('days',"
     "date_trunc('month',to_timestamp(to_char(((godn*10000)+(mes.nz*100)+1),'99999999'),"
     "'YYYYMMDD'))+'1 MONTH'::interval-'1 DAY'::interval) AS kold1 "
     "FROM dds LEFT OUTER JOIN statdds ON (dds.nz = statdds.nz_dds),mes "
     "WHERE date_part('month',dat) = mes.nz AND tip = 'KVI' AND kodobr = 0 AND godn = "+QString::number(god)+" GROUP BY mes.opis,"
     "nka,godn,mes.nz ORDER BY godn ASC,mes.nz";

    cOI->kol_htable=9;
    cOI->htable[0]=tr("Месяц"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("% получ.эпох"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("% ГЛОНАСС"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Кол.суток"); cOI->hidcol[4]=false;
    cOI->hidcol[5]=true;
    cOI->htable[6]=tr("% получ.эпох в мес."); cOI->hidcol[6]=false;
    cOI->htable[7]=tr("% ГЛОНАСС в мес."); cOI->hidcol[7]=false;
    cOI->htable[8]=tr("Кол.суток в мес."); cOI->hidcol[8]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str,true);
    ui->tableView->setCurrentIndex(ind);
}

void FSeansDds::on_btnGraf()
{
 formPotDds->cOI=cOI;
 formPotDds->dat.setDate(ui->cb_god->currentText().toInt(),ui->cb_mes->currentIndex()+1,1);
 formPotDds->nka=ui->cb_nka->currentText();
 formPotDds->show();
}
