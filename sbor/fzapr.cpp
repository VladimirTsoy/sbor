#include "fzapr.h"
#include "ui_fzapr.h"

FZapr::FZapr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FZapr)
{
    ui->setupUi(this);
 //   select_query="SELECT zapr.nz,zapr.naz,zapr.dat,potr.naz AS naz_potr FROM zapr,potr ";
 //   select_where="WHERE zapr.nz_potr = potr.nz AND zapr.komp = '"+cOI->komp_name_ust+"'";
 //   select_order="ORDER BY zapr.dat ASC,potr.naz ASC,zapr.naz ASC";
//    model=0;
    model=new QSqlQueryModel(this);

    formUstZapr=new FUstZapr(this);
    formUstZapr->setWindowFlags(Qt::Window);
    formEdvid=new FEdvid(this);
    formEdvid->setWindowFlags(Qt::Window);

    connect(ui->btnAdd,SIGNAL(clicked(bool)),this,SLOT(on_btnAdd()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(on_btnEdit()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(formUstZapr,SIGNAL(SaveUstZapr()),this,SLOT(on_btnRefresh()));
    connect(ui->btnForm,SIGNAL(clicked(bool)),this,SLOT(on_btnForm()));
    connect(ui->btnSpis,SIGNAL(clicked(bool)),this,SLOT(on_btnSpis()));
}

FZapr::~FZapr()
{
    delete ui;
}

void FZapr::ViewQuery()
{
 select_query="SELECT zapr.nz,zapr.naz,zapr.dat,potr.naz AS naz_potr FROM zapr,potr ";
 select_where="WHERE zapr.nz_potr = potr.nz AND zapr.komp = '"+cOI->komp_name_ust+"' ";
 select_order="ORDER BY zapr.dat ASC,potr.naz ASC,zapr.naz ASC";
 cOI->setQueryModel(model,ui->tableView,select_query+select_where+select_order,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FZapr::showEvent(QShowEvent *event)
{
 ShowForm();
 event->accept();
}

void FZapr::ShowForm()
{
// if (model == 0)
// {
//  model=new QSqlQueryModel;
// }
 cOI->kol_htable=4;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название запроса"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Дата запроса"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Название потребителя"); cOI->hidcol[3]=false;
 ViewQuery();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FZapr::on_btnAdd()
{
 quint64 nz;

 cOI->ClearZapr();
 cOI->zappotr.naz="не определён";
 cOI->InsertPotr(false,0);
 cOI->zapzapr.komp=cOI->komp_name_ust;
 nz=cOI->InsertZapr(true);
 ViewQuery();
 ind=cOI->IndModel(nz,model);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FZapr::on_btnDel()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 cOI->DeleteRow("zapr",nz,true);
 ViewQuery();
 return;
}

void FZapr::on_btnEdit()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadZapr(nz,false)) return;

 formUstZapr->cOI=cOI;
 formUstZapr->show();

 return;
}

void FZapr::on_btnRefresh()
{
 ind=ui->tableView->currentIndex();
 cOI->kol_htable=4;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название запроса"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Дата запроса"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Название потребителя"); cOI->hidcol[3]=false;
 ViewQuery();
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FZapr::on_btnForm()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 cOI->EdvidForDs(nz,false);
 cOI->EdvidForDds(nz,false);
 cOI->EdvidForPpsi(nz,false);
 cOI->EdvidForRnxi(nz,false);
 cOI->EdvidForRnxv(nz,false);
 cOI->EdvidForSp3(nz,false);
 cOI->EdvidForSol(nz,false);

 QMessageBox::information(0,"Формирование запроса","Формирование запроса выполнено успешно",QMessageBox::Yes);

 return;
}

void FZapr::on_btnSpis()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadZapr(nz,false)) return;

 formEdvid->cOI=cOI;
 formEdvid->show();

 return;
}
