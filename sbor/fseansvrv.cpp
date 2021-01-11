#include "fseansvrv.h"
#include "ui_fseansvrv.h"

FSeansVrv::FSeansVrv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSeansVrv)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);

    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
}

FSeansVrv::~FSeansVrv()
{
    delete ui;
}

void FSeansVrv::ViewSpis(qint32 n,qint32 k)
{
 QString str;

 str="SELECT naz,dat,nka,plan,noplan,round((vip*100)/plan) AS prvip,"
     "vne,round((vne*100)/(vip+vne)) AS prvne FROM  dds LEFT OUTER JOIN statvrv ON (dds.nz = statvrv.nz_dds) WHERE "
     "tip = 'ALTI' AND kodobr = 0 AND plan > 0 "+str_nka+" AND "
     "((godn*1000)+sutn BETWEEN "+QString::number(n)+" AND "+QString::number(k)+") "
     "ORDER BY godn ASC,sutn ASC";

    cOI->kol_htable=8;
    cOI->htable[0]=tr("Название файла"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("Дата"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("№ КА"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("Запланировано пакетов"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Нет плана для пакетов"); cOI->hidcol[4]=false;
    cOI->htable[5]=tr("% выполнения плана"); cOI->hidcol[5]=false;
    cOI->htable[6]=tr("Получено вне плана"); cOI->hidcol[6]=false;
    cOI->htable[7]=tr("% полученных вне плана"); cOI->hidcol[7]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str,true);
    ui->tableView->setCurrentIndex(ind);
}

void FSeansVrv::on_btnRefresh()
{
 QDate dn,dk;
 qint32 sutn,sutk;
 QString nka;

 dn.setDate(1900,1,1);
 dk.setDate(2999,12,31);
 sutn=(dn.year()*1000)+dn.dayOfYear();
 sutk=(dk.year()*1000)+dk.dayOfYear();
 str_nka.clear();
 nka.clear();

 if (ui->cb_otb->isChecked())
 {
  dn.setDate(ui->cb_god->currentText().toInt(),ui->cb_mes->currentIndex()+1,1);
  dk=dn.addMonths(1);
  dk=dk.addDays(-1);
  sutn=(dn.year()*1000)+dn.dayOfYear();
  sutk=(dk.year()*1000)+dk.dayOfYear();
  str_nka=" AND nka ='"+ui->cb_nka->currentText()+"' ";
  nka=ui->cb_nka->currentText();
 }
 if (ui->cb_grgod->isChecked())
 {
  dn.setDate(ui->cb_god->currentText().toInt(),1,1);
  dk=dn.addYears(1);
  dk=dk.addDays(-1);
  sutn=(dn.year()*1000)+dn.dayOfYear();
  sutk=(dk.year()*1000)+dk.dayOfYear();
 }

 if (!cOI->RasStatVrv(dn,dk,nka,true)) return;

 if (!ui->cb_grgod->isChecked()) ViewSpis(sutn,sutk);
 else ViewSpisMes(dn.year());

 return;
}

void FSeansVrv::ViewSpisMes(qint32 god)
{
 QString str;

 str="SELECT opis,nka,round(avg((vip*100)/plan)) AS prvip,round(avg((vne*100)/(vip+vne))) AS prvne,count(*) kol FROM dds "
     "LEFT OUTER JOIN statvrv ON (dds.nz = statvrv.nz_dds),mes WHERE date_part('month',dat) = mes.nz "
     "AND tip = 'ALTI' AND kodobr = 0 AND plan > 0 "+str_nka+" AND godn = "+QString::number(god)+" "
     "GROUP BY opis,nka,godn,mes.nz ORDER BY godn ASC,mes.nz";

    cOI->kol_htable=5;
    cOI->htable[0]=tr("Месяц"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("Средний % выполн.плана"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("Средний % вне плана"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Количество суток"); cOI->hidcol[4]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str,true);
    ui->tableView->setCurrentIndex(ind);
}
