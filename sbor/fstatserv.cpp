#include "fstatserv.h"
#include "ui_fstatserv.h"

FStatServ::FStatServ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FStatServ)
{
    ui->setupUi(this);
    model=new QSqlQueryModel(this);

    ui->dtN->setDate(QDate::currentDate());
    ui->dtK->setDate(QDate::currentDate());

    connect(ui->pbRefresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
}

FStatServ::~FStatServ()
{
    delete ui;
}

void FStatServ::FormSpis()
{
 if (!ui->pbGrup->isChecked())
 {
  strsql="SELECT statserv.nz,serv.opis,statserv.ifile,statserv.dat,statserv.vr,kobrserv.soob AS soob_obr,"
         "karx.soob AS soob_arx FROM statserv,serv,kobrserv,karx WHERE statserv.nz_serv = serv.nz AND serv.kobr = kobrserv.nz AND "
         "serv.karx = karx.nz";
 }
 else
 {
  strsql="SELECT serv.opis,count(*) FROM statserv,serv WHERE statserv.nz_serv = serv.nz";
 }

 if (ui->cbDat->isChecked())
 {
  strsql=strsql+" AND statserv.dat BETWEEN '"+ui->dtN->date().toString("dd-MM-yyyy")+"' AND '"+ui->dtK->date().toString("dd-MM-yyyy")+"'";
 }

 if (ui->pbGrup->isChecked())
 {
  strsort=" GROUP BY serv.opis ORDER BY serv.opis ASC";
 }
 else strsort=" ORDER BY serv.opis ASC,statserv.dat ASC,statserv.vr ASC";

 strsql=strsql+strsort;
}

void FStatServ::FormShow()
{

 if (!ui->pbGrup->isChecked())
 {
  cOI->kol_htable=7;
  cOI->hidcol[0]=true;
  cOI->htable[1]=tr("Название группы"); cOI->hidcol[1]=false;
  cOI->htable[2]=tr("Название файла"); cOI->hidcol[2]=false;
  cOI->htable[3]=tr("Дата"); cOI->hidcol[3]=false;
  cOI->htable[4]=tr("Время"); cOI->hidcol[4]=false;
  cOI->htable[5]=tr("Обработка"); cOI->hidcol[5]=false;
  cOI->htable[6]=tr("Архивирование"); cOI->hidcol[6]=false;
 }
 else
 {
  cOI->kol_htable=2;
  cOI->htable[0]=tr("Название группы"); cOI->hidcol[0]=false;
  cOI->htable[1]=tr("Количество файлов"); cOI->hidcol[1]=false;
 }

}

void FStatServ::showEvent(QShowEvent *event)
{
 FormSpis();
 FormShow();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 event->accept();
}

void FStatServ::on_pbRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 FormSpis();
 FormShow();
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}
