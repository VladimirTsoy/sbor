#include "fvrv.h"
#include "ui_fvrv.h"

FVrv::FVrv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FVrv)
{
    ui->setupUi(this);

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnList,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
    connect(ui->btnStat,SIGNAL(clicked(bool)),this,SLOT(on_btnStat()));
    connect(ui->btnPlan,SIGNAL(clicked(bool)),this,SLOT(on_btnPlan()));
    connect(ui->btnKonva,SIGNAL(clicked(bool)),this,SLOT(on_btnKonva()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
    connect(ui->cb_alti,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb_cal1,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb_cal2,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb_Obr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb_Och,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb_Neobr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbNka,SIGNAL(currentIndexChanged(int)),this,SLOT(on_btnRefresh()));

    sql_select="SELECT dds.nz,dds.naz,dds.tip,dds.nka,dds.dtn,dds.dtk,strk.sod,kodobr.soob FROM dds,strk,kodobr ";
    sql_where="WHERE dds.strk = strk.nz AND dds.kodobr = kodobr.nz ";
    sql_where_tip="AND (dds.tip = 'ALTI' OR dds.tip = 'CAL1' OR dds.tip = 'CAL2') ";
    sql_order="ORDER BY dds.godn ASC,dds.sutn ASC,dds.sekn ASC,dds.naz ASC,dds.nka ASC,dds.tip ASC";
    sql_where_obr.clear();
    sql_where_nka.clear();

    m_vrv=new QSqlQueryModel(this);

    formTxtFileView=new FTxtFileView(this);
    formTxtFileView->setWindowFlags(Qt::Window);
    formStatVRV=new FStatVRV(this);
    formStatVRV->setWindowFlags(Qt::Window);

    ui->btnRefresh->setToolTip("Обновить содержимое таблицы");
    ui->btnList->setToolTip("Просмотр эпох в файле");
    ui->btnStat->setToolTip("Просмотр статистики суточного файла");
    ui->btnKonva->setToolTip("Выполнить получение и замену суточного файла в БД");
    ui->btnDel->setToolTip("Удалить файл");
    ui->btnVigr->setToolTip("Выгрузить файл на диск");
    ui->btnPlan->setToolTip("Загрузить в БД планы работ ВРВ");
}

FVrv::~FVrv()
{
    delete ui;
}

void FVrv::showForm()
{
 showTitle();
 ind=m_vrv->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 FormFiltrTip(); FormFiltrObr(); FormFiltrNka();
 cOI->setQueryModel(m_vrv,ui->tableView,sql_select+sql_where+sql_where_tip+sql_where_obr+sql_where_nka+sql_order,true);
 return;
}

void FVrv::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FVrv::on_btnRefresh()
{
 qlonglong nz;

 showTitle();

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz);
 FormFiltrTip(); FormFiltrObr(); FormFiltrNka();
 cOI->setQueryModel(m_vrv,ui->tableView,sql_select+sql_where+sql_where_tip+sql_where_obr+sql_where_nka+sql_order,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FVrv::FormFiltrTip()
{
 sql_where_tip.clear();

 if (ui->cb_alti->isChecked()) sql_where_tip="AND (tip = 'ALTI'";
 if (ui->cb_cal1->isChecked())
 {
  if (sql_where_tip.isEmpty()) sql_where_tip="AND (tip = 'CAL1'";
  else sql_where_tip=sql_where_tip+" OR tip = 'CAL1'";
 }
 if (ui->cb_cal2->isChecked())
 {
  if (sql_where_tip.isEmpty()) sql_where_tip="AND (tip = 'CAL2'";
  else sql_where_tip=sql_where_tip+" OR tip = 'CAL2'";
 }
 if (!sql_where_tip.isEmpty()) sql_where_tip=sql_where_tip+")";
 else
 {
  sql_where_tip="AND (dds.tip = 'ALTI' OR dds.tip = 'CAL1' OR dds.tip = 'CAL2') ";
  ui->cb_alti->setChecked(true);
  ui->cb_cal1->setChecked(true);
  ui->cb_cal2->setChecked(true);
 }
 return;
}

void FVrv::FormFiltrObr()
{
 sql_where_obr.clear();

 if (ui->cb_Obr->isChecked()) sql_where_obr="AND (kodobr = 0";

 if (ui->cb_Neobr->isChecked())
 {
  if (sql_where_obr.isEmpty()) sql_where_obr="AND (kodobr = 18 OR kodobr = 19";
  else sql_where_obr=sql_where_obr+" OR kodobr = 18 OR kodobr = 19";
 }

 if (ui->cb_Och->isChecked())
 {
  if (sql_where_obr.isEmpty()) sql_where_obr="AND (kodobr = 1";
  else sql_where_obr=sql_where_obr+" OR kodobr = 1";
 }

 if (!sql_where_obr.isEmpty()) sql_where_obr=sql_where_obr+")";
 else
 {
  ui->cb_Obr->setChecked(true);
  ui->cb_Neobr->setChecked(true);
  ui->cb_Och->setChecked(true);
 }

 return;
}

void FVrv::on_btnList()
{
 qlonglong nz;
 QByteArray buf;
 QString tip;
 QStringList list;

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,2,tip);

 if (!cOI->ReadEdxrInBuf("dds",nz,buf,true)) return;

 cOI->ReadAltiInList(buf,list);
 cOI->WriteFileFromList(list,path_temp+"/protvrv.txt");

 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=0;
 formTxtFileView->ntable=path_temp+"/protvrv.txt";
 formTxtFileView->show();

 return;
}

void FVrv::on_btnStat()
{
 quint64 nz,nz_stat;
 QString tip;
 qlonglong nz1;

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz1);
 if (nz1 == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,2,tip);

 cOI->ReadDDS(nz1,false);
 if (cOI->zapdds.kodobr != 0) return;

 if (tip != "ALTI") return;

 nz=nz1;

 nz_stat=cOI->FindRow("statvrv","nz_dds",nz,true);

 if (nz_stat == 0)
    if (!cOI->ZapStatVRV(nz,nz_stat,true)) return;

 cOI->ReadDDS(nz1,true);
 cOI->ReadStatVrv(nz_stat,true);

 formStatVRV->cOI=cOI;
 formStatVRV->nz_dds=0;
 formStatVRV->nz_stat=nz_stat;
 formStatVRV->show();

 return;
}

void FVrv::on_btnPlan()
{
 QStringList spisf;

 spisf=QFileDialog::getOpenFileNames(0,"Планы работ ВРВ",cOI->zapust.path_si,"*.alt");
 if (spisf.isEmpty()) return;

 for (qint32 i=0; i<spisf.count(); i++)
 {
  if (!cOI->SetAltInBD(spisf[i],false)) continue;
  cOI->DelFile(spisf[i]);
 }

 return;
}

void FVrv::on_btnKonva()
{
 QString naz_file;
 QDate dat;
 qlonglong nz;
 quint64 nzn;
 QDate dn,dk;
 QList<qlonglong> list;
 qint32 kod,kodobr;
 QString soob;


 QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
 if (!lock.tryLock(100)) return;

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz);
 if (nz == 0) return;

 soob=QObject::tr("Выполнить создание и замену суточного файла ?");
 int k=QMessageBox::question(0,tr("Конвертирование "),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;


 cOI->ReadDDS(nz,false);
 if (cOI->zapdds.kodobr != 0) return;
 if (cOI->zapdds.tip == "ALTI") kod=3;
 else return;

 dat=cOI->DayToDate(cOI->zapdds.sutn,cOI->zapdds.godn);

 if (!cOI->CreateFileForPpsi(dat,kod,naz_file,true,cOI->zapdds.nka,list,kodobr)) return;

 nzn=cOI->SetDDSFromFileAlti(naz_file,true,"ALTI",0,true);

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

void FVrv::showTitle()
{
 path_temp=cOI->zapust.path_temp;
 cOI->kol_htable=8;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Тип"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("№ КА"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Дата,время начала сеанса"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время конца сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Контроль"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Состояние обработки"); cOI->hidcol[7]=false;
 return;
}

void FVrv::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,1,naz);

 soob=QObject::tr("Удалить файл ВРВ ?");
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

void FVrv::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,m_vrv,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("dds",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}

void FVrv::FormFiltrNka()
{
 sql_where_nka.clear();

 if (ui->cbNka->currentIndex() == 0) return;

 if (ui->cbNka->currentIndex() == 1) sql_where_nka=" AND nka = '126' ";
 if (ui->cbNka->currentIndex() == 2) sql_where_nka=" AND nka = '127' ";
}
