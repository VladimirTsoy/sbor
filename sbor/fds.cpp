#include "fds.h"
#include "ui_fds.h"

FDs::FDs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FDs)
{
 ui->setupUi(this);
 select_query="SELECT ds.naz,ds.npn,ds.nkis,ds.nka,sv.opisan,ds.dtn,ds.dtk,ds.nz FROM ds,sv ";
 sort_query="ORDER BY ds.npn ASC,ds.nka ASC,ds.godn ASC,ds.sutn ASC,ds.sekn ASC;";
 filtr_npn.clear();
 filtr_dat.clear();

 connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->btnList,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
 connect(ui->btnFiltr,SIGNAL(clicked(bool)),this,SLOT(on_btnFiltr()));
 connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
 connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
 connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));

 formTxtFileView=new FTxtFileView(this);
 formTxtFileView->setWindowFlags(Qt::Window);

 m_ds=new QSqlQueryModel(this);

 ui->dtn->setDate(QDate::currentDate());
 ui->dtk->setDate(QDate::currentDate());

}

FDs::~FDs()
{
    delete ui;
}

void FDs::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FDs::showForm()
{
 list_npn=cOI->SpisNPN("ds",nazis,true);
 ui->cbNpn->clear();
 ui->cbNpn->setMaxCount(list_npn.count());
 ui->cbNpn->addItems(list_npn);
 where_query="WHERE ds.sv = sv.nz AND ds.nazis = '"+nazis+"' ";
 if (nazis == "DS") this->setWindowTitle("Сеансы ДС");
 if (nazis == "DZS") this->setWindowTitle("Сеансы ДЗС");
 if (nazis == "SPO") this->setWindowTitle("Кадры СПО");
 cOI->kol_htable=8;
 cOI->htable[0]=tr("Название файла"); cOI->hidcol[0]=false;
 cOI->htable[1]=tr("№ НПН"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ комплекта ИС");
 cOI->hidcol[2]=false; if (nazis == "SPO") cOI->hidcol[2]=true;
 cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Шкала времени"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
 cOI->hidcol[7]=true;
 on_btnRefresh();
// cOI->setQueryModel(m_ds,ui->tableView,select_query+where_query+sort_query,true);
 ind=m_ds->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FDs::on_btnRefresh()
{
 QString where;
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

 where=where_query+filtr_npn+filtr_dat;

 ind=cOI->getCurrentIndex(ui->tableView,m_ds,7,nz);
 cOI->setQueryModel(m_ds,ui->tableView,select_query+where+sort_query,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FDs::on_btnList()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,m_ds,7,nz);
 if (nz == 0) return;
 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=nz;
 formTxtFileView->ntable="ds";
 formTxtFileView->show();

 return;
}

void FDs::on_btnFiltr()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,m_ds,7,nz);
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

void FDs::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,m_ds,7,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,m_ds,0,naz);

 if (nazis == "DS") soob=QObject::tr("Удалить файл сеанса ДС?");
 if (nazis == "DZS") soob=QObject::tr("Удалить файл сеанса ДЗС?");
 if (nazis == "SPO") soob=QObject::tr("Удалить файл кадра СПО?");
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

void FDs::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,m_ds,7,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("ds",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}
