#include "fkos.h"
#include "ui_fkos.h"

FKos::FKos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FKos)
{
    ui->setupUi(this);

    query="SELECT ds.nz,ds.naz,ds.npn,ds.nka,sv.opisan,ds.dtn,ds.dtk FROM ds,sv WHERE ds.sv = sv.nz AND (ds.nazis = 'KOS' OR ds.nazis = 'FRD')";
    order=" ORDER BY ds.godn ASC,ds.sutn ASC,ds.sekn ASC,ds.npn ASC,ds.naz ASC;";
    filtr_npn.clear();
    filtr_dat.clear();

    model=new QSqlQueryModel(this);

    connect(ui->btnQuit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnFiltr,SIGNAL(clicked(bool)),this,SLOT(on_btnFiltr()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));


    ui->dtn->setDate(QDate::currentDate());
    ui->dtk->setDate(QDate::currentDate());
}

FKos::~FKos()
{
    delete ui;
}

void FKos::showEvent(QShowEvent *event)
{
 list_npn=cOI->SpisNPN("ds","KOS",true);
 ui->cbNpn->clear();
 ui->cbNpn->setMaxCount(list_npn.count());
 ui->cbNpn->addItems(list_npn);
 cOI->kol_htable=7;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ НПН"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Шкала времени"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
 on_btnRefresh();
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FKos::on_btnRefresh()
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
  filtr_dat=" AND((ds.godn*1000)+ds.sutn BETWEEN "+dn+" AND "+dk+")";
 }
 else filtr_dat.clear();

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,query+filtr_npn+filtr_dat+order,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FKos::on_btnFiltr()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0)
 {
  ui->btnFiltr->setChecked(false);
  return;
 }

 if (ui->btnFiltr->isChecked())
 {
  filtr_npn=" AND ds.npn = '"+ui->cbNpn->currentText()+"'";
 }
 else filtr_npn.clear();

 on_btnRefresh();

 return;
}

void FKos::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,model,1,naz);

 soob=QObject::tr("Удалить файл сеанса КОС?");
 int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 cOI->ClearSist();
 if (cOI->RemoveDsInBasket(nz,true))
 {
  cOI->zapsist.naz_ed=naz;
  cOI->zapsist.nz_kodobr=6;
  cOI->zapsist.sost="выполнено";
  cOI->InsertSist(false);
 }
 on_btnRefresh();
}

void FKos::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("ds",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}
