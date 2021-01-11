#include "fnosd.h"
#include "ui_fnosd.h"

FNosd::FNosd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FNosd)
{
    ui->setupUi(this);

//    query="SELECT nos.nz,nos.naz,kodnos.opis FROM nos,kodnos WHERE nos.prblok = kodnos.nz ORDER BY nos.nz";

//    query="SELECT nos.nz,kodnos.opis,nos.naz,COUNT(edxr.arxf) AS koled,round(SUM(edxr.adlf/1073741824.0),2) AS sumed "
//          "FROM public.nos,public.edxr,public.kodnos WHERE nos.nz = edxr.nz_nos AND nos.prblok = kodnos.nz "
//          "GROUP BY nos.nz,kodnos.opis ORDER BY nos.nz";

    query="SELECT nos.nz,nos.naz,kodnos.opis, COUNT(edxr.arxf) AS koled,round(SUM(edxr.adlf/1073741824.0),2) AS sumed "
          "FROM public.nos LEFT OUTER JOIN public.edxr ON (nos.nz = edxr.nz_nos),public.kodnos WHERE nos.prblok = kodnos.nz "
          "GROUP BY nos.nz,kodnos.opis ORDER BY nos.nz";


    model=new QSqlQueryModel(this);

    formEditNos=new FEditNos(this);
    formEditNos->setWindowFlags(Qt::Window);

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnAdd,SIGNAL(clicked(bool)),this,SLOT(on_btnAdd()));
    connect(formEditNos,SIGNAL(ok()),this,SLOT(on_btnRefresh()));
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(on_btnEdit()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
}

FNosd::~FNosd()
{
    delete ui;
}

void FNosd::showEvent(QShowEvent *event)
{
 cOI->kol_htable=5;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Путь к носителю"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Состояние носителя"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Колич.единиц"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Объём в Гб"); cOI->hidcol[4]=false;
 on_btnRefresh();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FNosd::on_btnRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,query,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FNosd::on_btnAdd()
{
 formEditNos->nz=0;
 formEditNos->cOI=cOI;
 formEditNos->show();
 return;
}

void FNosd::on_btnEdit()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 formEditNos->nz=nz;
 formEditNos->cOI=cOI;
 formEditNos->show();
 return;
}

void FNosd::on_btnDel()
{
 qlonglong nz_edxr,nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 nz_edxr=cOI->FindRow("edxr","nz_nos",nz,true);
 if (nz_edxr > 0) return;

 cOI->DeleteRow("nos",nz,true);
 on_btnRefresh();

 return;
}

void FNosd::closeEvent(QCloseEvent *event)
{
 if (! cOI->GetNos(false))
 {
  QMessageBox::warning(0,"Предупреждение","Необходимо завести архивный носитель!",QMessageBox::Yes);
  event->ignore();
  return;
 }
 event->accept();
 return;
}
