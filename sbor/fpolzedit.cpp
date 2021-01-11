#include "fpolzedit.h"
#include "ui_fpolzedit.h"

FPolzEdit::FPolzEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPolzEdit)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnOk,SIGNAL(clicked(bool)),this,SLOT(on_btnOk()));
}

FPolzEdit::~FPolzEdit()
{
    delete ui;
}

void FPolzEdit::showEvent(QShowEvent *event)
{
 QString str;
 qlonglong nz;
 QSqlQuery query;
 bool prsuper;

 this->setWindowTitle("Пользователь : "+naz_polz);

 ui->le_polz->clear();
 ui->le_pas1->clear();
 ui->le_pas2->clear();
 ui->cb_super->setChecked(false);
 ui->le_polz->setReadOnly(false);

 if (!naz_polz.isEmpty())
 {
  str="SELECT usename,usesuper FROM pg_user WHERE usename = '"+naz_polz+"'";
  nz=cOI->ExecQuery(query,str,true);
  ui->le_polz->setText(cOI->spis_field[0].toString());
  ui->le_polz->setReadOnly(true);
  prsuper=cOI->spis_field[1].toBool();
  ui->cb_super->setChecked(prsuper);
 }

 event->accept();
}

void FPolzEdit::on_btnOk()
{
    QString str,soob;
    QSqlError err;
    QSqlQuery query;

 if (naz_polz == "postgres")
 {
  if (ui->le_pas1->text().isEmpty()) return;
  str="ALTER ROLE "+naz_polz+" WITH PASSWORD '"+ui->le_pas1->text()+"'";
  if (! query.exec(str))
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
  }
  emit ok();
  close();
  return;
 }

 if (!naz_polz.isEmpty())
 {
  str="DROP ROLE "+naz_polz;
  if (! query.exec(str))
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
   return;
  }
 }
  if (ui->le_pas1->text() != ui->le_pas2->text()) return;
  str="CREATE ROLE "+ui->le_polz->text()+" WITH LOGIN PASSWORD '"+ui->le_pas1->text()+"'";
  if (ui->cb_super->isChecked()) str=str+" SUPERUSER";
  if (! query.exec(str))
  {
   soob=QObject::tr("Ошибка выполнения запроса. ")+query.lastError().text();
   QMessageBox::critical(0,QObject::tr("Ошибка"),soob,QMessageBox::Yes);
   return;
  }
  emit ok();
  close();

 return;
}
