#include "fsp3.h"
#include "ui_fsp3.h"

FSp3::FSp3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSp3)
{
    ui->setupUi(this);
    select_query="SELECT sp3.nz,sp3.naz,sp3.nka,sp3.dtn,sp3.dtk,(sp3.kolep*100)/8639 AS proc FROM sp3 ";
    sort_query="ORDER BY sp3.nka ASC,sp3.godn ASC,sp3.sutn ASC,sp3.sekn ASC;";
    filtr_dat.clear();

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnList,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));

    formTxtFileView=new FTxtFileView(this);
    formTxtFileView->setWindowFlags(Qt::Window);

    model=new QSqlQueryModel(this);

    ui->dtn->setDate(QDate::currentDate());
    ui->dtk->setDate(QDate::currentDate());
}

FSp3::~FSp3()
{
    delete ui;
}

void FSp3::showForm()
{
 cOI->kol_htable=6;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ КА"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Дата,время начала"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Дата,время конца"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("% полученных эпох"); cOI->hidcol[5]=false;
 on_btnRefresh();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FSp3::on_btnRefresh()
{
 qlonglong nz;
 qint32 yn,sn,yk,sk;
 QString dn,dk;

 if (ui->cbDat->isChecked())
 {
  sn=ui->dtn->date().dayOfYear();
  yn=ui->dtn->date().year();
  dn=QString::number((yn*1000)+sn);
  sk=ui->dtk->date().dayOfYear();
  yk=ui->dtk->date().year();
  dk=QString::number((yk*1000)+sk);
  filtr_dat=" WHERE ((sp3.godn*1000)+sp3.sutn BETWEEN "+dn+" AND "+dk+")";
 }
 else filtr_dat.clear();

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,select_query+filtr_dat+sort_query,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FSp3::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FSp3::on_btnList()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;
 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=nz;
 formTxtFileView->ntable="sp3";
 formTxtFileView->show();

 return;
}

void FSp3::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("sp3",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}

void FSp3::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,model,1,naz);

 soob=QObject::tr("Удалить файл эфемеридно-временной информации ?");
 int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 cOI->ClearSist();
 if (cOI->RemoveSp3InBasket(nz,true))
 {
  cOI->zapsist.naz_ed=naz;
  cOI->zapsist.nz_kodobr=6;
  cOI->zapsist.sost="выполнено";
  cOI->InsertSist(false);
 }
 on_btnRefresh();
}
