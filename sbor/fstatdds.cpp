#include "fstatdds.h"
#include "ui_fstatdds.h"

FStatDDS::FStatDDS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FStatDDS)
{
    ui->setupUi(this);
//    cOI=new ObrIzm;
    m_statrdds=new QSqlQueryModel(this);
}

FStatDDS::~FStatDDS()
{
    delete ui;
}

void FStatDDS::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FStatDDS::showForm()
{
 qlonglong nz_stat;
 qint32 proc;

 nz_stat=cOI->FindRow("statdds","nz_dds",nz_dds,true);

 if (nz_stat == 0)
  if (!cOI->ZapStatDDS(nz_dds,true)) return;

 cOI->ReadStatDDS(nz_dds,true);
 ui->le_eppl->setText(QString::number(cOI->zapstatdds.eppl));
 ui->le_dat->setText(cOI->zapstatdds.dat.toString("dd-MM-yyyy"));
 ui->le_eppol->setText(QString::number(cOI->zapstatdds.eppol));
 proc=(cOI->zapstatdds.eppol*100)/cOI->zapstatdds.eppl;
 ui->le_prpol->setText(QString::number(proc));
 ui->le_ep4mglo->setText(QString::number(cOI->zapstatdds.ep_4mglo));
 proc=(cOI->zapstatdds.ep_4mglo*100)/cOI->zapstatdds.eppol;
 ui->le_pr4mglo->setText(QString::number(proc));
 ui->le_ep4rglo->setText(QString::number(cOI->zapstatdds.ep_4rglo+cOI->zapstatdds.ep_4bglo));
 proc=100-proc;
 ui->le_pr4rglo->setText(QString::number(proc));

 query="SELECT to_char(statrdds.vrn,'HH24:MI:SS'),to_char(statrdds.vrr,'HH24:MI:SS'),statrdds.kolep FROM statrdds ";
 query=query+"WHERE statrdds.nz_statdds = "+QString::number(cOI->zapstatdds.nz)+" ORDER BY statrdds.vrn ASC;";

 cOI->kol_htable=3;
 cOI->htable[0]=tr("Время начала разрыва"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("Продолжительность разрыва"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Количество пропущенных эпох"); cOI->hidcol[2]=false;
 ind=m_statrdds->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 cOI->setQueryModel(m_statrdds,ui->tableView,query,true);

 return;
}
