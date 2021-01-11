#include "fsba.h"
#include "ui_fsba.h"

FSba::FSba(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSba)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);
    formEditSba=new FEditSba(this);
    formEditSba->setWindowFlags(Qt::Window);

    strsql="SELECT sba.nz,sba.nka,kodba.naz,sba.dvik,sba.vvik,sba.dvkl,sba.vvkl FROM sba,kodba WHERE sba.nz_kodba = kodba.nz "
           "ORDER BY sba.nka ASC,kodba.naz ASC,sba.dvik ASC, sba.vvik ASC";

    connect(ui->pbRefresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(ui->pbAdd,SIGNAL(clicked(bool)),this,SLOT(on_pbAdd()));
    connect(formEditSba,SIGNAL(ok()),this,SLOT(on_pbRefresh()));
    connect(ui->pbEdit,SIGNAL(clicked(bool)),this,SLOT(on_pbEdit()));
    connect(ui->pbDel,SIGNAL(clicked(bool)),this,SLOT(on_pbDel()));
}

FSba::~FSba()
{
    delete ui;
}

void FSba::showForm()
{
 cOI->kol_htable=7;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Номер КА"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Название БА КА"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Дата выключения"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Время выключения"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата включения"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Время включения"); cOI->hidcol[6]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FSba::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FSba::on_pbRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FSba::on_pbAdd()
{
 formEditSba->cOI=cOI;
 formEditSba->nz=0;
 formEditSba->show();
}

void FSba::on_pbEdit()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formEditSba->cOI=cOI;
 formEditSba->nz=nz;
 formEditSba->show();
}

void FSba::on_pbDel()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->DeleteRow("sba",nz,true);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}
