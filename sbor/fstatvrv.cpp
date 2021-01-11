#include "fstatvrv.h"
#include "ui_fstatvrv.h"

FStatVRV::FStatVRV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FStatVRV)
{
    ui->setupUi(this);
    m_statrvrv=new QSqlQueryModel(this);
    m_alt=new QSqlQueryModel(this);
}

FStatVRV::~FStatVRV()
{
    delete ui;
}

void FStatVRV::showEvent(QShowEvent *event)
{
 if (nz_dds > 0)
 {
  nz_stat=cOI->FindRow("statvrv","nz_dds",nz_dds,true);

  if (nz_stat == 0)
     if (!cOI->ZapStatVRV(nz_dds,nz_stat,true)) return;
  cOI->ReadDDS(nz_dds,true);
  cOI->ReadStatVrv(nz_stat,true);
 }


 ui->le_plan->setText(QString::number(cOI->zapstatvrv.plan));
 ui->le_vip->setText(QString::number(cOI->zapstatvrv.vip));
 ui->le_vne->setText(QString::number(cOI->zapstatvrv.vne));
 ui->le_novip->setText(QString::number(cOI->zapstatvrv.nevip));
 ui->le_noplan->setText(QString::number(cOI->zapstatvrv.noplan));
 if (cOI->zapstatvrv.plan != 0) ui->le_proc->setText(QString::number((cOI->zapstatvrv.vip*100)/cOI->zapstatvrv.plan));
 else ui->le_proc->setText("0");

 query="SELECT alt.datn,to_char(alt.vrn,'HH24:MI:SS') AS vrnn,alt.prod,alt.nvit,alt.datk,to_char(alt.vrk,'HH24:MI:SS') AS vrkn,kodalt.naz FROM alt,kodalt WHERE alt.reg = kodalt.nz";
 query=query+" AND alt.nka = '"+cOI->zapdds.nka+"'";
 query=query+" AND ((alt.datn = '"+cOI->zapdds.dtn+"' AND alt.datk = '"+cOI->zapdds.dtn+"') ";
 query=query+"OR (alt.datn < '"+cOI->zapdds.dtn+"' AND alt.datk = '"+cOI->zapdds.dtn+"') ";
 query=query+"OR (alt.datn = '"+cOI->zapdds.dtn+"' AND alt.datk > '"+cOI->zapdds.dtn+"')) ";
 query=query+"ORDER BY alt.datn ASC,alt.vrn ASC;";

 m_alt->clear();
 cOI->kol_htable=7;
 cOI->htable[0]=tr("Дата начала сеанса"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("Время начала сеанса"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Продолжительность сеанса в сек."); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("№ витка"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Дата конца сеанса"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Время конца сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Режим работы"); cOI->hidcol[6]=false;
 ind=m_alt->index(0,0);
 cOI->setQueryModel(m_alt,ui->tv_plan,query,true);
 ui->tv_plan->setCurrentIndex(ind);

 query="SELECT to_char(statrvrv.tm,'HH24:MI:SS') AS tmn,statrvrv.kolpak,kodalt.naz FROM statrvrv,kodalt ";
 query=query+"WHERE statrvrv.reg = kodalt.nz AND statrvrv.nz_statvrv = "+QString::number(nz_stat)+" ORDER BY statrvrv.tm ASC;";
 m_statrvrv->clear();
 cOI->kol_htable=3;
 cOI->htable[0]=tr("Время эпохи"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("Количество полученных пакетов"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Режим работы по плану"); cOI->hidcol[2]=false;
 ind=m_statrvrv->index(0,0);
 cOI->setQueryModel(m_statrvrv,ui->tv_statvrv,query,true);
 ui->tv_statvrv->setCurrentIndex(ind);


 event->accept();
}
