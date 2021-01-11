#include "frnxv.h"
#include "ui_frnxv.h"

FRnxv::FRnxv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FRnxv)
{
    ui->setupUi(this);

    strsql="SELECT rnxv.nz,rnxv.naz,rnxv.npn,rnxv.tip,rnxv.ver,rnxv.dtn,rnxv.dtk,strk.sod,kodobr.soob ";
    strsql=strsql+"FROM rnxv,strk,kodobr WHERE rnxv.strk = strk.nz AND rnxv.kodobr = kodobr.nz";
    strsort=" ORDER BY rnxv.godn ASC,rnxv.sutn ASC,rnxv.sekn ASC,rnxv.npn ASC,rnxv.naz ASC";

    model=new QSqlQueryModel(this);
    formTxtFileView=new FTxtFileView(this);
    formTxtFileView->setWindowFlags(Qt::Window);

    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnList,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
    connect(ui->btnProt,SIGNAL(clicked(bool)),this,SLOT(on_btnProt()));
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbOtbnpn,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbO,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbN,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbG,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbObrn,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbObrz,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbNoch,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbEoch,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));

    ui->dtn->setDate(QDate::currentDate());
    ui->dtk->setDate(QDate::currentDate());
}

FRnxv::~FRnxv()
{
    delete ui;
}

void FRnxv::showEvent(QShowEvent *event)
{
 QStringList list_npn;

 list_npn=cOI->SpisNPN("rnxv","",true);
 ui->cbNpn->clear();
 ui->cbNpn->setMaxCount(list_npn.count());
 ui->cbNpn->addItems(list_npn);
 ui->cbOtbnpn->setChecked(false);

 cOI->kol_htable=9;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ НПН"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Тип"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Версия"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Контроль"); cOI->hidcol[7]=false;
 cOI->htable[8]=tr("Код обработки"); cOI->hidcol[8]=false;
 on_btnRefresh();
 ind=model->index(0,0);
// cOI->setQueryModel(model,ui->tableView,strsql+strsort,true);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FRnxv::on_btnRefresh()
{
 qlonglong nz;
 QString str;

 str=strsql+FiltrFormat()+FiltrNpn()+FiltrDate()+FiltrObr()+FiltrKontr()+strsort;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,str,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FRnxv::on_btnList()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=nz;
 formTxtFileView->ntable="rnxv";
 formTxtFileView->show();

 return;
}

void FRnxv::on_btnProt()
{
 QModelIndex idx;
 qlonglong nz;
 QStringList list,p_list;

 idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadRnx("rnxv",nz,true)) return;

 if (cOI->zaprnx.strk == 0) return;

 cOI->ReadEdxrInList("rnxv",nz,list,true);

 if (cOI->zaprnx.tip == "O") p_list=cOI->ProvStrDatRnx(list);
 else p_list=cOI->ProvStrDatRnxN(list,cOI->zaprnx.tip);
 if (p_list.isEmpty()) return;
 cOI->WriteFileFromList(p_list,cOI->zapust.path_temp+"/err.err");
 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=0;
 formTxtFileView->ntable=cOI->zapust.path_temp+"/err.err";
 formTxtFileView->show();

 return;
}

QString FRnxv::FiltrFormat()
{
 QString ret;

 ret.clear();

 if (ui->cbO->isChecked())
 {
  if (ret.isEmpty()) ret=" tip = 'O'";
  else ret=ret+" OR tip = 'O'";
 }
 if (ui->cbN->isChecked())
 {
  if (ret.isEmpty()) ret=" tip = 'N'";
  else ret=ret+" OR tip = 'N'";
 }
 if (ui->cbG->isChecked())
 {
  if (ret.isEmpty()) ret=" tip = 'G'";
  else ret=ret+" OR tip = 'G'";
 }

 if (!ret.isEmpty()) ret=" AND ("+ret+")";

 return ret;
}

QString FRnxv::FiltrNpn()
{
 QString ret;

 ret.clear();
 if (!ui->cbOtbnpn->isChecked()) return ret;

 ret=" AND npn = '"+ui->cbNpn->currentText()+"'";

 return ret;
}

QString FRnxv::FiltrDate()
{
 QString ret;
 qint32 yn,sn,yk,sk;
 QString dn,dk;

 ret.clear();
 if (!ui->cbDat->isChecked()) return ret;

 sn=ui->dtn->date().dayOfYear();
 yn=ui->dtn->date().year();
 dn=QString::number((yn*1000)+sn);
 sk=ui->dtk->date().dayOfYear();
 yk=ui->dtk->date().year();
 dk=QString::number((yk*1000)+sk);
 ret=" AND ((rnxv.godn*1000)+rnxv.sutn BETWEEN "+dn+" AND "+dk+")";

 return ret;
}

QString FRnxv::FiltrObr()
{
 QString ret;

 ret.clear();

 if (ui->cbObrz->isChecked() && ui->cbObrn->isChecked()) return ret;

 if (ui->cbObrz->isChecked()) ret=" AND (kodobr = 0 OR kodobr = 5)";
 if (ui->cbObrn->isChecked()) ret=" AND (kodobr <> 0 AND kodobr <> 5)";

 return ret;
}

QString FRnxv::FiltrKontr()
{
 QString ret;

 ret.clear();

 if (ui->cbNoch->isChecked() && ui->cbEoch->isChecked()) return ret;

 if (ui->cbNoch->isChecked()) ret=" AND strk = 0";
 if (ui->cbEoch->isChecked()) ret=" AND strk <> 0";

 return ret;
}

void FRnxv::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("rnxv",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}

void FRnxv::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,model,1,naz);

 soob=QObject::tr("Удалить файл сеанса БДС?");
 int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 cOI->ClearSist();
 if (cOI->RemoveRnxInBasket("rnxv",nz,true))
 {
  cOI->zapsist.naz_ed=naz;
  cOI->zapsist.nz_kodobr=6;
  cOI->zapsist.sost="выполнено";
  cOI->InsertSist(false);
 }
 on_btnRefresh();
}
