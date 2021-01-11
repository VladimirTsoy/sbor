#include "fcup.h"
#include "ui_fcup.h"

FCup::FCup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCup)
{
    ui->setupUi(this);

    sql="SELECT cup.nz,cup.naz,cup.dat,cup.vrem,tipnf.opis,sostcup.opis,cogi.naz FROM tipnf,sostcup,cup "
        "LEFT OUTER JOIN cogi ON (cogi.nz_cup = cup.nz)";
    where=" WHERE cup.tip = tipnf.nz AND cup.sost = sostcup.nz";
    order=" ORDER BY cup.dat, cup.vrem, cup.naz";

    model=new QSqlQueryModel(this);

    formNfView=new FNfView(this);
    formNfView->setWindowFlags(Qt::Window);

    ui->de1->setDate(QDate::currentDate());
    ui->de2->setDate(QDate::currentDate());

    connect(ui->pb_Refresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(ui->cb_notpr,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->cb_otpr,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->de1,SIGNAL(dateChanged(QDate)),this,SLOT(on_dat()));
    connect(ui->de2,SIGNAL(dateChanged(QDate)),this,SLOT(on_dat()));
    connect(ui->pbView,SIGNAL(clicked(bool)),this,SLOT(on_pbView()));
    connect(formNfView,SIGNAL(Ret()),this,SLOT(on_pbRefresh()));
    connect(ui->pbDel,SIGNAL(clicked(bool)),this,SLOT(on_pbDel()));
    connect(ui->pbAdd,SIGNAL(clicked(bool)),this,SLOT(on_pbAdd()));
}

FCup::~FCup()
{
    delete ui;
}

void FCup::showForm()
{
 QString str;

 cOI->kol_htable=7;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название формы"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Дата"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Время"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Тип формы"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Состояние обработки"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Название подтверждаемой формы"); cOI->hidcol[6]=false;
 str=where+FiltrSost()+FiltrDate();
 cOI->setQueryModel(model,ui->tableView,sql+str+order,true);
 return;
}

void FCup::showEvent(QShowEvent *event)
{
 showForm();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FCup::on_pbRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 showForm();
 ui->tableView->setCurrentIndex(ind);
 return;
}

QString FCup::FiltrDate()
{
 QString ret;

 ret.clear();
 if (!ui->cbDat->isChecked()) return ret;

 ret=" AND (cup.dat BETWEEN '"+ui->de1->date().toString("dd-MM-yyyy")+"' AND '"+ui->de2->date().toString("dd-MM-yyyy")+"')";

 return ret;
}

QString FCup::FiltrSost()
{
 QString ret;
 QString pr;

 ret=" AND (";
 pr.clear();

 if (ui->cb_notpr->isChecked()) {ret=ret+" cup.sost = 1"; pr=" OR";}
 if (ui->cb_otpr->isChecked()) ret=ret+pr+" cup.sost = 2";

 if (ret != " AND (") ret=ret+")";
 else ret.clear();

 return ret;
}

void FCup::on_cb()
{
 qint32 ch=0;

 if (ui->cb_notpr->isChecked()) ch++;
 if (ui->cb_otpr->isChecked()) ch++;

 if (ch == 0)
 {
  ui->cb_notpr->setChecked(true);
  ui->cb_otpr->setChecked(true);
 }
 on_pbRefresh();
}

void FCup::on_dat()
{
 if (!ui->cbDat->isChecked()) return;

 on_pbRefresh();
}

void FCup::on_pbView()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formNfView->nz=nz;
 formNfView->cOI=cOI;
 formNfView->reg=1;
 formNfView->show();
 return;
}

void FCup::on_pbDel()
{
 qlonglong nz,nz_cogi;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 cOI->ReadCup(nz,true);
 nz_cogi=cOI->FindRow("cogi","nz_cup",nz,true);

 if (!cOI->RemoveEdxr(cOI->zapcup.nz_edxr,true)) return;
 if (!cOI->DeleteRow("cup",cOI->zapcup.nz,true)) return;

 if (nz_cogi > 0)
 {
  cOI->UpdateRow("cogi","sost",1,nz_cogi,true);
  cOI->UpdateRow("cogi","nz_cup",0,nz_cogi,true);
 }

 on_pbRefresh();
 return;
}

void FCup::on_pbAdd()
{
 formNfView->nz=0;
 formNfView->cOI=cOI;
 formNfView->reg=2;
 formNfView->show();
 return;
}
