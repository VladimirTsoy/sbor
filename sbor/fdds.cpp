#include "fdds.h"
#include "ui_fdds.h"

FDds::FDds(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FDds)
{
 ui->setupUi(this);

 connect(ui->btn_Refresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->btn_List,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
 connect(ui->btn_Stat,SIGNAL(clicked(bool)),this,SLOT(on_btnStat()));
 connect(ui->btn_Konv,SIGNAL(clicked(bool)),this,SLOT(on_btnKonv()));
 connect(ui->btn_Del,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
 connect(ui->btn_Vigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
 connect(ui->btnProt,SIGNAL(clicked(bool)),this,SLOT(on_btnProt()));
 connect(ui->cb_kvi,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->cb_kvno,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->rb_all,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->rb_err,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->rb_nobr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->rb_obr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
 connect(ui->cbNka,SIGNAL(currentIndexChanged(int)),this,SLOT(on_btnRefresh()));
 connect(ui->btn_Graf,SIGNAL(clicked(bool)),this,SLOT(on_btnGraf()));


 sql_select="SELECT dds.nz,dds.naz,dds.tip,dds.nka,dds.dtn,dds.dtk,strk.sod,kodobr.soob FROM dds,strk,kodobr ";
 sql_where="WHERE dds.strk = strk.nz AND dds.kodobr = kodobr.nz ";
 sql_where_tip="AND (dds.tip = 'KVI' OR dds.tip = 'KVNO') ";
 sql_order="ORDER BY dds.nka ASC,dds.godn ASC,dds.sutn ASC,dds.sekn ASC,dds.naz ASC,dds.tip ASC";
 sql_where_obr.clear();
 sql_where_nka.clear();

 formTxtFileView=new FTxtFileView(this);
 formTxtFileView->setWindowFlags(Qt::Window);
 formStatDDS=new FStatDDS(this);
 formStatDDS->setWindowFlags(Qt::Window);
 formDdsView=new FDdsView(this);
 formDdsView->setWindowFlags(Qt::Window);

 m_dds=new QSqlQueryModel(this);

 ui->btn_Refresh->setToolTip("Обновить содержимое таблицы");
 ui->btn_List->setToolTip("Просмотр содержимого файла");
 ui->btn_Stat->setToolTip("Просмотр статистики суточного файла");
 ui->btn_Konv->setToolTip("Выполнить получение и замену суточного файла в БД");
 ui->btn_Del->setToolTip("Удалить файл");
 ui->btn_Vigr->setToolTip("Выгрузить файл на диск");
}

FDds::~FDds()
{
    delete ui;
}

void FDds::showForm()
{
 cOI->kol_htable=8;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Тип"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Дата,время начала сеанса"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время конца сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Контроль"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Состояние обработки"); cOI->hidcol[7]=false;
 FormFiltrTip(); FormFiltrObr(); FormFiltrNka();
 cOI->setQueryModel(m_dds,ui->tableView,sql_select+sql_where+sql_where_tip+sql_where_obr+sql_where_nka+sql_order,true);
 return;
}

void FDds::showEvent(QShowEvent *event)
{
 showForm();
 ind=m_dds->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FDds::FormFiltrTip()
{
 sql_where_tip="AND (dds.tip = 'KVI' OR dds.tip = 'KVNO') ";

 if (ui->cb_kvi->isChecked() && ui->cb_kvno->isChecked()) return;

 if (ui->cb_kvi->isChecked())
 {
  sql_where_tip="AND dds.tip = 'KVI' ";
  return;
 }

 if (ui->cb_kvno->isChecked())
 {
  sql_where_tip="AND dds.tip = 'KVNO' ";
  return;
 }

 ui->cb_kvi->setChecked(true);
 ui->cb_kvno->setChecked(true);

 return;
}

void FDds::on_btnRefresh()
{
 qlonglong nz;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 showForm();
// FormFiltrTip(); FormFiltrObr();
// cOI->setQueryModel(m_dds,ui->tableView,sql_select+sql_where+sql_where_tip+sql_where_obr+sql_order,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FDds::FormFiltrObr()
{
 if (ui->rb_all->isChecked())
 {
  sql_where_obr.clear();
  return;
 }

 if (ui->rb_obr->isChecked())
 {
  sql_where_obr="AND dds.kodobr = 0 ";
  return;
 }

 if (ui->rb_err->isChecked())
 {
  sql_where_obr="AND dds.kodobr = 1 ";
  return;
 }

 if (ui->rb_nobr->isChecked())
 {
  sql_where_obr="AND dds.kodobr > 1 ";
  return;
 }

 return;
}

void FDds::on_btnList()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=nz;
 formTxtFileView->ntable="dds";
 formTxtFileView->show();

 return;
}

void FDds::on_btnStat()
{
 qlonglong nz;
 QString tip;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,2,tip);

 if (!cOI->ReadDDS(nz,false))
 {
  QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
  return;
 }
 if (cOI->zapdds.kodobr != 0) return;

 if (tip != "KVI") return;

 formStatDDS->cOI=cOI;
 formStatDDS->nz_dds=nz;
 formStatDDS->show();

 return;
}

void FDds::on_btnKonv()
{
 QString naz_file;
 QDate dat;
 qlonglong nz;
 quint64 nzn;
 QDate dn,dk;
 QList<qlonglong> list;
 qint32 kod;
 QString soob;
 qint32 kodobr;

 QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
 if (!lock.tryLock(100)) return;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 soob=QObject::tr("Выполнить создание и замену суточного файла ?");
 int k=QMessageBox::question(0,tr("Конвертирование "),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 if (!cOI->ReadDDS(nz,false))
 {
  QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
  return;
 }
 if ((cOI->zapdds.kodobr != 0) && (cOI->zapdds.kodobr != 1))
 {
  QMessageBox::warning(0,"Предупреждение","С данным кодом обработки конвертирование не выполняется.",QMessageBox::Yes);
  return;
 }
 if (cOI->zapdds.tip == "KVI") kod=1;
 if (cOI->zapdds.tip == "KVNO") kod=2;

 dat=cOI->DayToDate(cOI->zapdds.sutn,cOI->zapdds.godn);

 if (!cOI->CreateFileForPpsi(dat,kod,naz_file,true,cOI->zapdds.nka,list,kodobr)) return;

 if (kod == 1)
 {
  nzn=cOI->SetDDSFromFileKVI(naz_file,true,kodobr,true,dn,dk);
  if (kodobr == 1) cOI->UpdateRow("dds","strk",2,nzn,true);
 }
 if (kod == 2)
     nzn=cOI->SetDDSFromFileKVNO(naz_file,true,0,true,dn,dk);

 if (nzn > 0)
 {
  for (int j=0; j<list.count(); j++)
  {
   cOI->ClearSxema();
   cOI->zapsxema.nz_educh=list[j];
   cOI->zapsxema.nz_educho1=nzn;
   cOI->zapsxema.nz_educho2=0;
   cOI->InsertSxema("sxema",true);
  }
  cOI->UpdateRow("dds","kodobr",6,nz,true);
  cOI->RemoveDDS(nz,true);
 }
 cOI->DelFile(naz_file);
 on_btnRefresh();
 QMessageBox::information(0,"Сообщение","Конвертирование выполнено успешно.",QMessageBox::Yes);

}

void FDds::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,m_dds,1,naz);

 soob=QObject::tr("Удалить файл ДДС ?");
 int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 cOI->ClearSist();
 if (cOI->RemoveDdsInBasket(nz,true))
 {
  cOI->zapsist.naz_ed=naz;
  cOI->zapsist.nz_kodobr=6;
  cOI->zapsist.sost="выполнено";
  cOI->InsertSist(false);
 }
 on_btnRefresh();
}

void FDds::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("dds",nz,sdir,true,nfile))
 {
  QMessageBox::critical(0,"Ошибка","Ошибка при выгрузке файла.",QMessageBox::Yes);
  return;
 }

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}

void FDds::FormFiltrNka()
{
 if (ui->cbNka->currentIndex() == 0)
 {
  sql_where_nka.clear();
  return;
 }
 if (ui->cbNka->currentIndex() == 1)
 {
  sql_where_nka=" AND nka = '126' ";
  return;
 }
 if (ui->cbNka->currentIndex() == 2)
 {
  sql_where_nka=" AND nka = '127' ";
  return;
 }
}

void FDds::on_btnProt()
{
 QModelIndex idx;
 qlonglong nz;
 QStringList list,p_list;

 idx=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadDDS(nz,true))
 {
  QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
  return;
 }

 if (cOI->zapdds.strk == 0) return;
 if (cOI->zapdds.tip == "KVI")
 {
  if (!cOI->ReadEdxrInList("dds",nz,list,true)) return;
  if (cOI->ProvRnxInterval(list,p_list)) return;
  cOI->WriteFileFromList(p_list,cOI->zapust.path_temp+"/err.err");
 }

 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=0;
 formTxtFileView->ntable=cOI->zapust.path_temp+"/err.err";
 formTxtFileView->show();

 return;
}

void FDds::on_btnGraf()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,m_dds,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadDDS(nz,true))
 {
  QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
  return;
 }

 if (cOI->zapdds.kodobr != 0) return;
 if (cOI->zapdds.tip != "KVI") return;

 formDdsView->cOI=cOI;
 formDdsView->nz_dds=nz;
 formDdsView->show();


 return;
}
