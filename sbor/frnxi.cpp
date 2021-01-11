#include "frnxi.h"
#include "ui_frnxi.h"

FRnxi::FRnxi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FRnxi)
{
    ui->setupUi(this);

    strsql="SELECT rnxi.nz,rnxi.naz,rnxi.npn,rnxi.tip,rnxi.ver,rnxi.dtn,rnxi.dtk,strk.sod,kodobr.soob ";
    strsql=strsql+"FROM rnxi,strk,kodobr WHERE rnxi.strk = strk.nz AND rnxi.kodobr = kodobr.nz";
    strsort=" ORDER BY rnxi.godn ASC,rnxi.sutn ASC,rnxi.sekn ASC,rnxi.npn ASC,rnxi.naz ASC";

    model=new QSqlQueryModel(this);
    formTxtFileView=new FTxtFileView(this);
    formTxtFileView->setWindowFlags(Qt::Window);

    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnList,SIGNAL(clicked(bool)),this,SLOT(on_btnList()));
    connect(ui->btnProt,SIGNAL(clicked(bool)),this,SLOT(on_btnProt()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()));
    connect(ui->btnObr,SIGNAL(clicked(bool)),this,SLOT(on_btnObr()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbO,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbL,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbN,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbG,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbObrn,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbObrz,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbEoch,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbNoch,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbOtbnpn,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));

    ui->dtn->setDate(QDate::currentDate());
    ui->dtk->setDate(QDate::currentDate());
}

FRnxi::~FRnxi()
{
    delete ui;
}

void FRnxi::showEvent(QShowEvent *event)
{
 if (pr_meteo) this->setWindowTitle("Список сеансов метеоданных");
 else this->setWindowTitle("Список необработанных сеансов БДС");

 if (pr_meteo) ui->groupBox->setVisible(false);
 else ui->groupBox->setVisible(true);

 //if (pr_meteo) list_npn=cOI->SpisNPN("rnxi","M",true);
 //else list_npn=cOI->SpisNPN("rnxi","",true);
 //ui->cbNpn->clear();
 //ui->cbNpn->setMaxCount(list_npn.count());
 //ui->cbNpn->addItems(list_npn);
 //ui->cbOtbnpn->setChecked(false);

 cOI->kol_htable=9;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("№ НПН"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Тип"); cOI->hidcol[3]=false;
 if (pr_meteo)  cOI->hidcol[3]=true;
 cOI->htable[4]=tr("Версия"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Контроль"); cOI->hidcol[7]=false;
 cOI->htable[8]=tr("Код обработки"); cOI->hidcol[8]=false;
 on_btnRefresh();
 ui->cbOtbnpn->setChecked(false);
 ind=model->index(0,0);
 ui->tableView->setCurrentIndex(ind);
 event->accept();
}

void FRnxi::on_btnRefresh()
{
 qlonglong nz;
 QString str;

 str=strsql+FiltrMeteo()+FiltrFormat()+FiltrNpn()+FiltrDate()+FiltrObr()+FiltrKontr()+strsort;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,str,true);
 ui->tableView->setCurrentIndex(ind);

 if (!ui->cbOtbnpn->isChecked())
 {
  if (pr_meteo) list_npn=cOI->SpisNPN("rnxi","M",true);
  else list_npn=cOI->SpisNPN("rnxi","",true);
  ui->cbNpn->clear();
  ui->cbNpn->setMaxCount(list_npn.count());
  ui->cbNpn->addItems(list_npn);
  //ui->cbOtbnpn->setChecked(false);
 }
 return;
}

void FRnxi::on_btnList()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadRnx("rnxi",nz,true)) return;

 if (cOI->zaprnx.tip == "L") return;

 formTxtFileView->cOI=cOI;
 formTxtFileView->nz=nz;
 formTxtFileView->ntable="rnxi";
 formTxtFileView->show();

 return;
}


void FRnxi::on_btnProt()
{
    QModelIndex idx;
    qlonglong nz;
    QStringList list,p_list;
    QString nfile;
    QDateTime dten,dtek;

    idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
    cOI->zapust.path_temp = "/home/user/temp";

    qDebug() << "on_btnProt: nz = " << nz << "\t path_temp = " << cOI->zapust.path_temp;
    if (nz == 0) return;

    if (!cOI->ReadRnx("rnxi",nz,true)) return;

    if (cOI->zaprnx.strk == 0) return;

    if (cOI->zaprnx.tip == "L")
    {
        if (!cOI->CopyEdxr("rnxi",cOI->zaprnx.nz,cOI->zapust.path_temp,true,nfile)) return;
        QFile *jps_file= new QFile(cOI->zapust.path_temp+"/"+cOI->readedxr.isxf);
        QFile *log_file= new QFile(cOI->zapust.path_temp+"/err.err");
        integrityControl *Control = new integrityControl(jps_file,log_file);
    }

    else
    {
        cOI->ReadEdxrInList("rnxi",nz,list,true);

        if (cOI->zaprnx.ver.left(1) == "3")
        {
            if (cOI->zaprnx.tip == "O") p_list=cOI->ProvStrDatRnx(list);
            else p_list=cOI->ProvStrDatRnxN(list,cOI->zaprnx.tip);
        }

        if (cOI->zaprnx.ver.left(1) == "2")
        {
            if (cOI->zaprnx.tip == "O")
            {
                //    cOI->CopyEdxr("rnxi",nz,cOI->zapust.path_temp,true,nfile);
                //    QFile *file = new QFile(nfile);
                //    cOI->test_rinex_V2(file,p_list);
                p_list=cOI->ProvStrDatRnx2(list,dten,dtek);
            }

            else p_list=cOI->ProvStrDatRnxN2(list,cOI->zaprnx.tip);
        }

        if (p_list.isEmpty()) return;
        cOI->WriteFileFromList(p_list,cOI->zapust.path_temp+"/err.err");
        qDebug() << "path_temp = " << cOI->zapust.path_temp;
    }

    formTxtFileView->cOI=cOI;
    formTxtFileView->nz=0;
    formTxtFileView->ntable=cOI->zapust.path_temp+"/err.err";
    formTxtFileView->show();

    return;
}
//void FRnxi::on_btnProt()
//{
// QModelIndex idx;
// qlonglong nz;
// QStringList list,p_list;
// QString nfile;
// QDateTime dten,dtek;

// idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
// if (nz == 0) return;

// if (!cOI->ReadRnx("rnxi",nz,true)) return;

// if (cOI->zaprnx.strk == 0) return;
// if (cOI->zaprnx.tip == "L")
// {
//  if (!cOI->CopyEdxr("rnxi",cOI->zaprnx.nz,cOI->zapust.path_temp,true,nfile)) return;
//  QFile *jps_file= new QFile(cOI->zapust.path_temp+"/"+cOI->readedxr.isxf);
//  QFile *log_file= new QFile(cOI->zapust.path_temp+"/err.err");

//  integrityControl *Control = new integrityControl(jps_file,log_file);
// }
// else
// {
//  cOI->ReadEdxrInList("rnxi",nz,list,true);

//  if (cOI->zaprnx.ver.left(1) == "3")
//  {
//   if (cOI->zaprnx.tip == "O") p_list=cOI->ProvStrDatRnx(list);
//   else p_list=cOI->ProvStrDatRnxN(list,cOI->zaprnx.tip);
//  }
//  if (cOI->zaprnx.ver.left(1) == "2")
//  {
//   if (cOI->zaprnx.tip == "O")
//   {
////    cOI->CopyEdxr("rnxi",nz,cOI->zapust.path_temp,true,nfile);
////    QFile *file = new QFile(nfile);
////    cOI->test_rinex_V2(file,p_list);
//    p_list=cOI->ProvStrDatRnx2(list,dten,dtek);
//   }
//   else p_list=cOI->ProvStrDatRnxN2(list,cOI->zaprnx.tip);
//  }
//  if (p_list.isEmpty()) return;
//  cOI->WriteFileFromList(p_list,cOI->zapust.path_temp+"/err.err");
// }

// formTxtFileView->cOI=cOI;
// formTxtFileView->nz=0;
// formTxtFileView->ntable=cOI->zapust.path_temp+"/err.err";
// formTxtFileView->show();

// return;
//}

QString FRnxi::FiltrMeteo()
{
 QString ret;

 ret.clear();
 if (pr_meteo) ret=" AND rnxi.tip = 'M'";
 else ret=" AND rnxi.tip <> 'M'";

 return ret;
}

QString FRnxi::FiltrFormat()
{
 QString ret;

 ret.clear();
 if (pr_meteo) return ret;

 if (ui->cbL->isChecked()) ret=" tip = 'L'";
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

QString FRnxi::FiltrNpn()
{
 QString ret;

 ret.clear();
 if (!ui->cbOtbnpn->isChecked()) return ret;

 ret=" AND npn = '"+ui->cbNpn->currentText()+"'";

 return ret;
}

QString FRnxi::FiltrDate()
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
 ret=" AND ((rnxi.godn*1000)+rnxi.sutn BETWEEN "+dn+" AND "+dk+")";

 return ret;
}

QString FRnxi::FiltrObr()
{
 QString ret;

 ret.clear();

 if (ui->cbObrz->isChecked() && ui->cbObrn->isChecked()) return ret;

 if (ui->cbObrz->isChecked()) ret=" AND (kodobr = 0 OR kodobr = 5)";
 if (ui->cbObrn->isChecked()) ret=" AND (kodobr <> 0 AND kodobr <> 5)";

 return ret;
}

QString FRnxi::FiltrKontr()
{
 QString ret;

 ret.clear();

 if (ui->cbNoch->isChecked() && ui->cbEoch->isChecked()) return ret;

 if (ui->cbNoch->isChecked()) ret=" AND strk = 0";
 if (ui->cbEoch->isChecked()) ret=" AND strk <> 0";

 return ret;
}

void FRnxi::on_btnDel()
{
 qlonglong nz;
 QString soob;
 QString naz;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;
 ind=cOI->getCurrentIndex(ui->tableView,model,1,naz);

 if (pr_meteo) soob=QObject::tr("Удалить файл сеанса метеоданных?");
 else soob=QObject::tr("Удалить файл сеанса БДС?");
 int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

 if (k == QMessageBox::No) return;

 cOI->ClearSist();
 if (cOI->RemoveRnxInBasket("rnxi",nz,true))
 {
  cOI->zapsist.naz_ed=naz;
  cOI->zapsist.nz_kodobr=6;
  cOI->zapsist.sost="выполнено";
  cOI->InsertSist(false);
 }
 on_btnRefresh();
}

void FRnxi::on_btnVigr()
{
 QString sdir;
 qlonglong nz;
 QString nfile;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
 if (sdir.isEmpty()) return;


 if (!cOI->CopyEdxr("rnxi",nz,sdir,true,nfile)) return;

 QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}

void FRnxi::on_btnObr()
{
 QModelIndex idx;
 qlonglong nz;

 idx=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 if (nz == 0) return;

 if (!cOI->ReadRnx("rnxi",nz,true)) return;

 if (cOI->zaprnx.strk != 2) return;
 if (cOI->zaprnx.tip != "L") return;

 if (!cOI->UpdateRow("rnxi","strk",0,nz,true)) return;
 cOI->UpdateRow("rnxi","kodobr",10,nz,true);

 on_btnRefresh();

 return;
}
