#include "fpolz.h"
#include "ui_fpolz.h"

FPolz::FPolz(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPolz)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);

    formPolzEdit=new FPolzEdit(this);
    formPolzEdit->setWindowFlags(Qt::Window);

    connect(ui->btnAdd,SIGNAL(clicked(bool)),this,SLOT(on_btnAdd()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnUpd,SIGNAL(clicked(bool)),this,SLOT(on_btnUpd()));
    connect(formPolzEdit,SIGNAL(ok()),this,SLOT(Refresh()));
}

FPolz::~FPolz()
{
    delete ui;
}

void FPolz::showEvent(QShowEvent *event)
{
 strsql="SELECT usename FROM pg_user ORDER BY usename ASC";

 cOI->kol_htable=2;
 cOI->htable[0]=tr("Пользователь"); cOI->hidcol[0]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;

 event->accept();
}

void FPolz::on_btnAdd()
{
 formPolzEdit->naz_polz.clear();
 formPolzEdit->cOI=cOI;
 formPolzEdit->show();
 return;
}

void FPolz::on_btnDel()
{
 QString str;
 QString name;
 QSqlQuery query;
 QString soob;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,name);

 if (name == "postgres") return;

 str="DROP ROLE "+name;
 if (! query.exec(str))
 {
  soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
  QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  return;
 }

 Refresh();
 return;
}

void FPolz::on_btnUpd()
{
 QString name;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,name);

 formPolzEdit->naz_polz=name;
 formPolzEdit->cOI=cOI;
 formPolzEdit->show();

 return;
}

void FPolz::Refresh()
{
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
}
