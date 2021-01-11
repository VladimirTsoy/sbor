#include "fust.h"
#include "ui_fust.h"

FUst::FUst(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FUst)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);

    connect(ui->btnOk,SIGNAL(clicked()),this,SLOT(on_btnOk()),Qt::UniqueConnection);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(on_btnCancel()),Qt::UniqueConnection);
    connect(ui->btnSir,SIGNAL(clicked()),this,SLOT(on_btnSir()),Qt::UniqueConnection);
    connect(ui->btnTemp,SIGNAL(clicked()),this,SLOT(on_btnTemp()),Qt::UniqueConnection);
    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(on_btnSave()),Qt::UniqueConnection);
    connect(ui->btnLoad,SIGNAL(clicked()),this,SLOT(on_btnLoad()),Qt::UniqueConnection);
}

FUst::~FUst()
{
    delete ui;
}

void FUst::showEvent(QShowEvent *event)
{  
 nz=cOI->FindRow("ust","nazkomp",cOI->komp_name_ust,true);
 if (nz == 0)
 {
  QTimer::singleShot(100, this, SLOT(close()));
  return;
 }
 event->accept();
 QTimer::singleShot(100, this, SLOT(ShowForm()));
}

void FUst::on_btnOk()
{
 UpdateUst();
 this->close();
}

void FUst::on_btnCancel()
{
 ShowForm();
 return;
}

void FUst::ShowForm()
{
 if (nz < 1) this->close();

 cOI->ReadUst(nz,true);

 ui->cb_obr2->setChecked(cOI->zapust.pr_obr2);
 ui->cb_obr3->setChecked(cOI->zapust.pr_obr3);
 ui->cb_obr4->setChecked(cOI->zapust.pr_obr4);
 ui->cb_obr7->setChecked(cOI->zapust.pr_obr7);
 ui->cb_obr8->setChecked(cOI->zapust.pr_obr8);
 ui->cb_obr9->setChecked(cOI->zapust.pr_obr9);
 ui->cb_obr10->setChecked(cOI->zapust.pr_obr10);
 ui->cb_obr11_15->setChecked(cOI->zapust.pr_obr11_15);
 ui->cb_obr12->setChecked(cOI->zapust.pr_obr12);
 ui->cb_obr13->setChecked(cOI->zapust.pr_obr13);
 ui->cb_obr14->setChecked(cOI->zapust.pr_obr14);
 ui->cb_obr16->setChecked(cOI->zapust.pr_obr16);
 ui->cb_obr17->setChecked(cOI->zapust.pr_obr17);
 ui->cb_obr18->setChecked(cOI->zapust.pr_obr18);
 ui->cb_obr19->setChecked(cOI->zapust.pr_obr19);
 ui->cb_obrsi->setChecked(cOI->zapust.pr_obrsi);
 ui->cb_pravt->setChecked(cOI->zapust.pravt);
 ui->le_km->setText(QString::number(cOI->zapust.km));
 ui->le_nazkomp->setText(cOI->zapust.nazkomp);
 ui->le_si->setText(cOI->zapust.path_si);
 ui->le_temp->setText(cOI->zapust.path_temp);

 ui->le_obrsi->setText(QString::number(cOI->zapust.kol_obrsi));
 ui->le_obr11_15->setText(QString::number(cOI->zapust.kol_obr11_15));
 ui->le_obr12->setText(QString::number(cOI->zapust.kol_obr12));
 ui->le_obr14->setText(QString::number(cOI->zapust.kol_obr14));
 ui->le_obr13->setText(QString::number(cOI->zapust.kol_obr13));
 ui->le_obr17->setText(QString::number(cOI->zapust.kol_obr17));
 ui->le_obr16->setText(QString::number(cOI->zapust.kol_obr16));
 ui->le_obr19->setText(QString::number(cOI->zapust.kol_obr19));
 ui->le_obr18->setText(QString::number(cOI->zapust.kol_obr18));
 ui->le_obr2->setText(QString::number(cOI->zapust.kol_obr2));
 ui->le_obr3->setText(QString::number(cOI->zapust.kol_obr3));
 ui->le_obr4->setText(QString::number(cOI->zapust.kol_obr4));
 ui->le_obr7->setText(QString::number(cOI->zapust.kol_obr7));
 ui->le_obr8->setText(QString::number(cOI->zapust.kol_obr8));
 ui->le_obr9->setText(QString::number(cOI->zapust.kol_obr9));
 ui->le_obr10->setText(QString::number(cOI->zapust.kol_obr10));

 ui->cb_obrzapr->setChecked(cOI->zapust.pr_obrzapr);

 return;
}

void FUst::on_btnSir()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"Директория с необработанными измерениями","");
 if (!sdir.isEmpty()) ui->le_si->setText(sdir);
 ui->le_si->setFocus();

}

void FUst::on_btnTemp()
{
 QString sdir;

 sdir=QFileDialog::getExistingDirectory(0,"Директория для обработки","");
 if (!sdir.isEmpty()) ui->le_temp->setText(sdir);
 ui->le_temp->setFocus();

}

void FUst::on_btnSave()
{
 QString str,soob;
 QStringList list;
 QString spisp;

 soob=QFileDialog::getSaveFileName(this,"Сохранить в файл",QDir::currentPath(),"Формат CSV (*.csv)");
 qDebug() << soob;
 if (soob.isEmpty()) return;

 UpdateUst();

 spisp="nazkomp,path_temp,path_si,pravt,km,pr_obrsi,pr_obr2,pr_obr3,pr_obr4,pr_obr7,pr_obr8,pr_obr9,"
       "pr_obr10,pr_obr11_15,pr_obr12,pr_obr14,pr_obr13,pr_obr17,pr_obr16,pr_obr19,pr_obr18,kol_obrsi,"
       "kol_obr2,kol_obr3,kol_obr4,kol_obr7,kol_obr8,kol_obr9,kol_obr10,kol_obr11_15,kol_obr12,kol_obr14,"
       "kol_obr13,kol_obr17,kol_obr16,kol_obr19,kol_obr18,pr_obrzapr,path_err";

 str="SELECT "+spisp+" FROM ust WHERE nazkomp = '"+cOI->komp_name_ust+"'";
 cOI->UstQueryModel(model,str,true);
 cOI->ModelInList(model,0,list,",",false);
 cOI->WriteFileFromList(list,soob);

 return;
}

void FUst::on_btnLoad()
{
 QString str,spisp;
 QStringList list;
 //qulonglong nz;

 QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
 if (!lock.tryLock(100)) return;

 str=QFileDialog::getOpenFileName(this,"Выбрать файл",QDir::currentPath(),"Формат CSV (*.csv)");
 if (str.isEmpty()) return;

 list=cOI->ReadFileInList(str);
 if (list.isEmpty()) return;

 cOI->DeleteRow("ust","nazkomp",cOI->komp_name_ust,true);

 spisp="nazkomp,path_temp,path_si,pravt,km,pr_obrsi,pr_obr2,pr_obr3,pr_obr4,pr_obr7,pr_obr8,pr_obr9,"
       "pr_obr10,pr_obr11_15,pr_obr12,pr_obr14,pr_obr13,pr_obr17,pr_obr16,pr_obr19,pr_obr18,kol_obrsi,"
       "kol_obr2,kol_obr3,kol_obr4,kol_obr7,kol_obr8,kol_obr9,kol_obr10,kol_obr11_15,kol_obr12,kol_obr14,"
       "kol_obr13,kol_obr17,kol_obr16,kol_obr19,kol_obr18,pr_obrzapr,path_err";

  str=list.at(0);
  str="INSERT INTO ust ("+spisp+") VALUES ("+str+")";
  nz=cOI->InsertRow(str,true);
  nz=cOI->FindRow("ust","nazkomp",cOI->komp_name_ust,true);
  if (nz <= 0)
  {
   cOI->ClearUst();
   cOI->zapust.nazkomp=cOI->komp_name_ust;
   cOI->InsertUst(true);
  }

 ShowForm();
 return;
}

void FUst::UpdateUst()
{
 cOI->zapust.pr_obr2=ui->cb_obr2->isChecked();
 cOI->zapust.pr_obr3=ui->cb_obr3->isChecked();
 cOI->zapust.pr_obr4=ui->cb_obr4->isChecked();
 cOI->zapust.pr_obr7=ui->cb_obr7->isChecked();
 cOI->zapust.pr_obr8=ui->cb_obr8->isChecked();
 cOI->zapust.pr_obr9=ui->cb_obr9->isChecked();
 cOI->zapust.pr_obr10=ui->cb_obr10->isChecked();
 cOI->zapust.pr_obr11_15=ui->cb_obr11_15->isChecked();
 cOI->zapust.pr_obr12=ui->cb_obr12->isChecked();
 cOI->zapust.pr_obr13=ui->cb_obr13->isChecked();
 cOI->zapust.pr_obr14=ui->cb_obr14->isChecked();
 cOI->zapust.pr_obr16=ui->cb_obr16->isChecked();
 cOI->zapust.pr_obr17=ui->cb_obr17->isChecked();
 cOI->zapust.pr_obr18=ui->cb_obr18->isChecked();
 cOI->zapust.pr_obr19=ui->cb_obr19->isChecked();
 cOI->zapust.pr_obrsi=ui->cb_obrsi->isChecked();
 cOI->zapust.pravt=ui->cb_pravt->isChecked();
 cOI->zapust.km=ui->le_km->text().toInt();
 cOI->zapust.nazkomp=ui->le_nazkomp->text();
 cOI->zapust.path_si=ui->le_si->text();
 cOI->zapust.path_temp=ui->le_temp->text();

 cOI->zapust.kol_obrsi=ui->le_obrsi->text().toInt();
 cOI->zapust.kol_obr11_15=ui->le_obr11_15->text().toInt();
 cOI->zapust.kol_obr12=ui->le_obr12->text().toInt();
 cOI->zapust.kol_obr14=ui->le_obr14->text().toInt();
 cOI->zapust.kol_obr13=ui->le_obr13->text().toInt();
 cOI->zapust.kol_obr17=ui->le_obr17->text().toInt();
 cOI->zapust.kol_obr16=ui->le_obr16->text().toInt();
 cOI->zapust.kol_obr19=ui->le_obr19->text().toInt();
 cOI->zapust.kol_obr18=ui->le_obr18->text().toInt();
 cOI->zapust.kol_obr2=ui->le_obr2->text().toInt();
 cOI->zapust.kol_obr3=ui->le_obr3->text().toInt();
 cOI->zapust.kol_obr4=ui->le_obr4->text().toInt();
 cOI->zapust.kol_obr7=ui->le_obr7->text().toInt();
 cOI->zapust.kol_obr8=ui->le_obr8->text().toInt();
 cOI->zapust.kol_obr9=ui->le_obr9->text().toInt();
 cOI->zapust.kol_obr10=ui->le_obr10->text().toInt();

 cOI->zapust.pr_obrzapr=ui->cb_obrzapr->isChecked();

 cOI->UpdateUst(true);
}
