#include "fpass.h"
#include "ui_fpass.h"

FPass::FPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPass)
{
    ui->setupUi(this);
    connect(ui->btnConnect,SIGNAL(clicked()),this,SLOT(on_btnConnect()),Qt::UniqueConnection);
    connect(ui->btnExit,SIGNAL(clicked()),this,SLOT(on_btnExit()),Qt::UniqueConnection);
}

FPass::~FPass()
{
    delete ui;
}

void FPass::on_btnConnect()
{
  QSqlError strerr;
  QString soob;
  QString cur_path;
  QVariant var;


  if (!cOI->createConnectionPSQL(ui->le_bd->text(),ui->le_polz->text(),ui->le_pass->text(),ui->le_host->text(),strerr))
  {
   soob=QObject::tr("Ошибка подключения к БД. ")+strerr.text().toStdString().c_str()+QObject::tr(" Закончить работу?") ;
   int k=QMessageBox::critical(0,tr("Ошибка"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

   if (k == QMessageBox::Yes) on_btnExit();

   return;
  }

//  cur_path=QDir::currentPath();
//  cur_path=cur_path+"/host.ini";
  cur_path=QApplication::applicationDirPath()+"/host.ini";
  var=cOI->hostp;
  QSettings sett(cur_path,QSettings::IniFormat);
  sett.setValue("CONNECT/HOST",var);

  emit ok();
  this->close();
}

void FPass::on_btnExit()
{
 emit no();
 this->close();
}

void FPass::showEvent(QShowEvent *event)
{
 QString cur_path;
 QFile ifile;

 //cur_path=QDir::currentPath();
 //cur_path=cur_path+"/host.ini";
 cur_path=QApplication::applicationDirPath()+"/host.ini";
 ifile.setFileName(cur_path);
 ui->le_host->setText("localhost");
 if (ifile.exists())
 {
  QSettings sett(cur_path,QSettings::IniFormat);
  ui->le_host->setText(sett.value("CONNECT/HOST","").toString());
 }

 this->setWindowTitle("Подключение к БД ("+title+")");
 ui->le_polz->clear();
 ui->le_pass->clear();
 ui->le_polz->setText("postgres");
 ui->le_pass->setText("postgres");
 ui->le_bd->setText("geoik");

 event->accept();
}
