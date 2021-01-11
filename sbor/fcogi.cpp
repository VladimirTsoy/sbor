#include "fcogi.h"
#include "ui_fcogi.h"

FCogi::FCogi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCogi)
{
    ui->setupUi(this);

    sql="SELECT cogi.nz,cogi.naz,cogi.dat,cogi.vrem,tipnf.opis,sostcogi.opis,cup.naz FROM tipnf,sostcogi,cogi "
        "LEFT OUTER JOIN cup ON (cup.nz = cogi.nz_cup)";
    where=" WHERE cogi.tip = tipnf.nz AND cogi.sost = sostcogi.nz";
    order=" ORDER BY cogi.dat, cogi.vrem, cogi.naz";

    model=new QSqlQueryModel(this);

    formNfView=new FNfView(this);
    formNfView->setWindowFlags(Qt::Window);

    ui->de1->setDate(QDate::currentDate());
    ui->de2->setDate(QDate::currentDate());

    connect(ui->pb_Refresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(ui->pbView,SIGNAL(clicked(bool)),this,SLOT(on_pbView()));
    connect(formNfView,SIGNAL(Ret()),this,SLOT(on_pbRefresh()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(ui->cb_pol,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->cb_prosm,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->cb_podt,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->cb_opodt,SIGNAL(clicked(bool)),this,SLOT(on_cb()));
    connect(ui->de1,SIGNAL(dateChanged(QDate)),this,SLOT(on_dat()));
    connect(ui->de2,SIGNAL(dateChanged(QDate)),this,SLOT(on_dat()));
    connect(ui->pbDel,SIGNAL(clicked(bool)),this,SLOT(on_pbDel()));
}

FCogi::~FCogi()
{
    delete ui;
}

void FCogi::showForm()
{
 QString str;

 cOI->kol_htable=7;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название формы"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Дата"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Время"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Тип формы"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Состояние обработки"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Название формы-подтверждения"); cOI->hidcol[6]=false;
 str=where+FiltrSost()+FiltrDate();
 cOI->setQueryModel(model,ui->tableView,sql+str+order,true);
 return;
}

void FCogi::showEvent(QShowEvent *event)
{
 showForm();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FCogi::on_pbRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 showForm();
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FCogi::on_pbView()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formNfView->nz=nz;
 formNfView->cOI=cOI;
 formNfView->reg=0;
 formNfView->show();
 return;
}

QString FCogi::FiltrSost()
{
 QString ret;
 QString pr;

 ret=" AND (";
 pr.clear();

 if (ui->cb_pol->isChecked()) {ret=ret+" cogi.sost = 0"; pr=" OR";}
 if (ui->cb_prosm->isChecked()) {ret=ret+pr+" cogi.sost = 1"; pr=" OR";}
 if (ui->cb_podt->isChecked()) {ret=ret+pr+" cogi.sost = 2";pr=" OR";}
 if (ui->cb_opodt->isChecked()) ret=ret+pr+" cogi.sost = 3";

 if (ret != " AND (") ret=ret+")";
 else ret.clear();

 return ret;
}

QString FCogi::FiltrDate()
{
 QString ret;

 ret.clear();
 if (!ui->cbDat->isChecked()) return ret;

 ret=" AND (cogi.dat BETWEEN '"+ui->de1->date().toString("dd-MM-yyyy")+"' AND '"+ui->de2->date().toString("dd-MM-yyyy")+"')";

 return ret;
}

void FCogi::on_cb()
{
 qint32 ch=0;

 if (ui->cb_pol->isChecked()) ch++;
 if (ui->cb_prosm->isChecked()) ch++;
 if (ui->cb_podt->isChecked()) ch++;
 if (ui->cb_opodt->isChecked()) ch++;

 if (ch == 0)
 {
  ui->cb_pol->setChecked(true);
  ui->cb_prosm->setChecked(true);
  ui->cb_podt->setChecked(true);
  ui->cb_opodt->setChecked(true);
 }
 on_pbRefresh();
}

void FCogi::on_dat()
{
 if (!ui->cbDat->isChecked()) return;

 on_pbRefresh();
}

void FCogi::on_pbDel()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 cOI->ReadCogi(nz,true);
 if (cOI->zapcogi.sost == 2)
 {
  cOI->ReadCup(cOI->zapcogi.nz_cup,true);
  if (!cOI->RemoveEdxr(cOI->zapcup.nz_edxr,true)) return;
  if (!cOI->DeleteRow("cup",cOI->zapcup.nz,true)) return;
 }

 if (!cOI->RemoveEdxr(cOI->zapcogi.nz_edxr,true)) return;
 if (!cOI->DeleteRow("cogi",cOI->zapcogi.nz,true)) return;

 on_pbRefresh();
 return;
}
