#include "fserv.h"
#include "ui_fserv.h"

FServ::FServ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FServ)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);
    model1=new QSqlQueryModel(this);

    formEditServ=new FEditServ(this);
    formEditServ->setWindowFlags(Qt::Window);

    connect(ui->pbAdd,SIGNAL(clicked(bool)),this,SLOT(on_pbAdd()));
    connect(ui->pbRefresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
    connect(formEditServ,SIGNAL(ok()),this,SLOT(on_pbRefresh()));
    connect(ui->pbEdit,SIGNAL(clicked(bool)),this,SLOT(on_pbEdit()));
    connect(ui->pbDel,SIGNAL(clicked(bool)),this,SLOT(on_pbDel()));
    connect(ui->pbSave,SIGNAL(clicked(bool)),this,SLOT(on_pbSave()));
    connect(ui->pbLoad,SIGNAL(clicked(bool)),this,SLOT(on_pbLoad()));
}

FServ::~FServ()
{
    delete ui;
}

void FServ::showForm()
{
    strsql="SELECT serv.nz,serv.opis,serv.maska,serv.isdir,serv.vixdir,serv.copdir,kobrserv.soob AS soob_obr,karx.soob AS soob_arx "
           "FROM serv,kobrserv,karx WHERE serv.kobr = kobrserv.nz AND serv.karx = karx.nz AND nazkomp ='";
    strsql=strsql+cOI->komp_name_ust+"' ORDER BY serv.opis ASC";

 cOI->kol_htable=8;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Описание файлов"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Маска файлов"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Исходная директория"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Выходная директория"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Директория для копий"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Обработка"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Дополнительная обработка"); cOI->hidcol[7]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FServ::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FServ::on_pbAdd()
{
 formEditServ->cOI=cOI;
 formEditServ->nz=0;
 formEditServ->show();
}

void FServ::on_pbRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
}

void FServ::on_pbEdit()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formEditServ->cOI=cOI;
 formEditServ->nz=nz;
 formEditServ->show();
}

void FServ::on_pbDel()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 cOI->DeleteRow("serv",nz,true);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
}

void FServ::on_pbSave()
{
 QString str,soob;
 QStringList list;

 soob=QFileDialog::getSaveFileName(this,"Сохранить в файл",QDir::currentPath(),"Формат CSV (*.csv)");
 if (soob.isEmpty()) return;

// str="SELECT * FROM serv WHERE nazkomp = '"+cOI->komp_name_ust+"'";
 str="SELECT isdir,vixdir,copdir,opis,maska,kobr,karx,nazkomp,prvd,prcd,maskvd,maskcd,sdir,sdirc FROM serv WHERE nazkomp = '"+cOI->komp_name_ust+"'";
 cOI->UstQueryModel(model1,str,true);
 cOI->ModelInList(model1,0,list,",",false);
 cOI->WriteFileFromList(list,soob);

 return;
}

void FServ::on_pbLoad()
{
 QString str;
 QStringList list;
 qulonglong nz;

 QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
 if (!lock.tryLock(100)) return;

 str=QFileDialog::getOpenFileName(this,"Выбрать файл",QDir::currentPath(),"Формат CSV (*.csv)");
 if (str.isEmpty()) return;

 list=cOI->ReadFileInList(str);
 if (list.isEmpty()) return;

 cOI->DeleteRow("serv","nazkomp",cOI->komp_name_ust,true);

 for (qint32 i=0; i<list.count(); i++)
 {
  str=list.at(i);
  str="INSERT INTO serv (isdir,vixdir,copdir,opis,maska,kobr,karx,nazkomp,prvd,prcd,maskvd,maskcd,sdir,sdirc) VALUES ("+str+")";
  qDebug() << str;
  nz=cOI->InsertRow(str,true);
 }

 showForm();
 return;
}
