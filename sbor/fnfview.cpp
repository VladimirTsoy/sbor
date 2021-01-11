#include "fnfview.h"
#include "ui_fnfview.h"

FNfView::FNfView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FNfView)
{
    ui->setupUi(this);

    connect(ui->pb_podt,SIGNAL(clicked(bool)),this,SLOT(on_pbPodt()));
    connect(ui->pb_Otpr,SIGNAL(clicked(bool)),this,SLOT(on_pbOtpr()));
    connect(ui->pb_cod,SIGNAL(clicked(bool)),this,SLOT(on_pbCod()));
    connect(ui->pb_otpr2,SIGNAL(clicked(bool)),this,SLOT(on_pbOtpr2()));
    connect(ui->pb_apodt,SIGNAL(clicked(bool)),this,SLOT(on_pbApodt()));
    connect(ui->pb_opodt,SIGNAL(clicked(bool)),this,SLOT(on_pbOpodt()));
    connect(this,SIGNAL(Err()),this,SLOT(on_Err()));

}

FNfView::~FNfView()
{
    delete ui;
}

/*
void FNfView::Zagr()
{
 if (!cOI->ReadEdxrInList(ntable,nz,list,true)) return;
 this->setWindowTitle("Просмотр : "+nfile);
 ui->l_cod->setText("Текущая кодировка : "+cod);

 return;
}
*/

void FNfView::showEvent(QShowEvent *event)
{
 if (reg == 0)
 {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));
  cod="WINDOWS-1251";
  ui->l_cod->setText("Текущая кодировка : "+cod);
  ObrVxod();
 }

  if (reg == 1)
  {
   QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));
   cod="WINDOWS-1251";
   ui->l_cod->setText("Текущая кодировка : "+cod);
   ObrIsxod();
  }

  if (reg == 2)
  {
   QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));
   cod="WINDOWS-1251";
   ui->l_cod->setText("Текущая кодировка : "+cod);
   ObrNew();
  }

 event->accept();
}

void FNfView::closeEvent(QCloseEvent *event)
{
 QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
 emit Ret();
 event->accept();
}

void FNfView::on_pbPodt()
{
 QString str;
 QDate dat;
 QTime time;

 dat=QDate::currentDate();
 time=QTime::currentTime();

 str="Подтверждение приёма операторами ЦОГИ формы \n"+nfile+" "+dat.toString("dd-MM-yyyy")+" "+time.toString("hh:mm:ss");
 ui->textEdit1->clear();
 ui->textEdit1->append(str);
 ui->textEdit1->moveCursor(QTextCursor::Start);
 ui->textEdit1->setReadOnly(false);
 ui->textEdit1->repaint();
 ui->textEdit1->setVisible(true);
 ui->pb_Otpr->setVisible(true);
 nf_podt="nf406-"+nfile.mid(6,3)+"-"+dat.toString("ddMMyyyy")+"-"+time.toString("hhmm")+".cup";
}

void FNfView::ObrVxod()
{
 list.clear();
 cOI->ReadCogi(nz,true);
 nfile=cOI->zapcogi.naz;
 if (cOI->zapcogi.tip == 306) list << "Визуализация формы не поддерживается.";
 else if (!cOI->ReadEdxrInList("cogi",nz,list,true)) return;
 this->setWindowTitle("Просмотр : "+nfile);

 ui->textEdit->clear();
 ui->textEdit->append(list.join("\n"));
 ui->textEdit->moveCursor(QTextCursor::Start);
 ui->textEdit->setReadOnly(true);
 ui->textEdit->repaint();
 ui->pb_podt->setVisible(true);
 ui->pb_apodt->setVisible(true);
 ui->pb_opodt->setVisible(true);

 ui->textEdit1->setVisible(false);
 ui->pb_Otpr->setVisible(false);
 ui->pb_otpr2->setVisible(false);
 ui->lb_nazf->setVisible(false);
 ui->le_nazf->setVisible(false);

 if (cOI->zapcogi.sost == 2)
 {
  if (!cOI->ReadEdxrInList("cup",cOI->zapcogi.nz_cup,list1,true)) return;
  ui->textEdit1->clear();
  ui->textEdit1->append(list1.join("\n"));
  ui->textEdit1->moveCursor(QTextCursor::Start);
  ui->textEdit1->setReadOnly(true);
  ui->textEdit1->setVisible(true);
  ui->textEdit1->repaint();
  ui->pb_podt->setVisible(false);
  ui->pb_apodt->setVisible(false);
  ui->pb_opodt->setVisible(false);
  ui->pb_otpr2->setVisible(true);
 }

 if (cOI->zapcogi.sost == 0) cOI->UpdateRow("cogi","sost",1,nz,true);
 return;
}

void FNfView::on_pbOtpr()
{
 QString str;
 QFile file;
 QDate dat;
 QTime time;

 dat=QDate::currentDate();
 time=QTime::currentTime();

 str=ui->textEdit1->toPlainText();

 list1.clear();
 list1=str.split("\n");
 if (ui->le_nazf->isVisible()) nf_podt=ui->le_nazf->text().trimmed();
 else nf_podt="nf406-"+nfile.mid(6,3)+"-"+dat.toString("ddMMyyyy")+"-"+time.toString("hhmm")+".cup";
 nf_podt=cOI->zapustnf.vxod+"/"+nf_podt;
 file.setFileName(nf_podt);
 if (file.exists()) {emit Err();return;}
 if (!cOI->WriteFileFromList(list1,nf_podt)) {emit Err();return;}
 if (cOI->ZapNfCup(nf_podt,nz1,false,true,true))
 {
  cOI->DelFile(nf_podt);
  cOI->UpdateRow("cogi","nz_cup",nz1,nz,true);
  cOI->UpdateRow("cogi","sost",2,nz,true);
  this->close();
  return;
 }
 cOI->DelFile(nf_podt);
 emit Err();
 return;
}

void FNfView::on_pbCod()
{
 QString str;

 if (reg == 2) str=ui->textEdit1->toPlainText();

 if (cod == "WINDOWS-1251")
 {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  cod="UTF-8";
  ui->l_cod->setText("Текущая кодировка : "+cod);
  if (reg == 0) ObrVxod();
  if (reg == 1) ObrIsxod();
  if (reg == 2)
  {
   ui->textEdit1->clear();
   ui->textEdit1->setText(str);
   ui->textEdit1->repaint();
  }
  return;
 }
 if (cod == "UTF-8")
 {
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));
  cod="WINDOWS-1251";
  ui->l_cod->setText("Текущая кодировка : "+cod);
  if (reg == 0) ObrVxod();
  if (reg == 1) ObrIsxod();
  if (reg == 2)
  {
   ui->textEdit1->clear();
   ui->textEdit1->setText(str);
   ui->textEdit1->repaint();
  }
  return;
 }
}

void FNfView::on_pbOtpr2()
{
 cOI->UpdateRow("cup","sost",1,cOI->zapcogi.nz_cup,true);
 this->close();
}

void FNfView::on_pbApodt()
{
 QString str;
 QDate dat;
 QTime time;
 QFile file;

 dat=QDate::currentDate();
 time=QTime::currentTime();

 str="Подтверждение приёма операторами ЦОГИ формы \n"+nfile+" "+dat.toString("dd-MM-yyyy")+" "+time.toString("hh:mm:ss");
 nf_podt="nf406-"+nfile.mid(6,3)+"-"+dat.toString("ddMMyyyy")+"-"+time.toString("hhmm")+".cup";
 list1.clear();
 list1=str.split("\n");
 nf_podt=cOI->zapustnf.vxod+"/"+nf_podt;
 file.setFileName(nf_podt);
 if (file.exists()) {emit Err();return;}
 if (!cOI->WriteFileFromList(list1,nf_podt)) {emit Err();return;}
 if (cOI->ZapNfCup(nf_podt,nz1,false,true,true))
 {
  cOI->DelFile(nf_podt);
  cOI->UpdateRow("cogi","nz_cup",nz1,nz,true);
  cOI->UpdateRow("cogi","sost",2,nz,true);
  this->close();
  return;
 }
 cOI->DelFile(nf_podt);
 emit Err();
 return;
}

void FNfView::on_pbOpodt()
{
 cOI->UpdateRow("cogi","sost",3,nz,true);
 this->close();
}

void FNfView::on_Err()
{
 QMessageBox::critical(0,"Ошибка","Ошибка при создании подтверждения. Повторите операцию позднее.",QMessageBox::Yes);
 return;
}

void FNfView::ObrIsxod()
{
 list.clear();
 cOI->ReadCup(nz,true);
 nfile=cOI->zapcup.naz;
 if ((cOI->zapcup.tip == 401) || (cOI->zapcup.tip == 403) || (cOI->zapcup.tip == 404)) list << "Визуализация формы не поддерживается.";
 else if (!cOI->ReadEdxrInList("cup",nz,list,true)) return;
 this->setWindowTitle("Просмотр : "+nfile);

 ui->textEdit->setVisible(true);
 ui->textEdit->clear();
 ui->textEdit->append(list.join("\n"));
 ui->textEdit->moveCursor(QTextCursor::Start);
 ui->textEdit->setReadOnly(true);
 ui->textEdit->repaint();
 ui->pb_podt->setVisible(false);
 ui->pb_apodt->setVisible(false);
 ui->pb_opodt->setVisible(false);

 ui->textEdit1->setVisible(false);
 ui->pb_Otpr->setVisible(false);
 ui->pb_otpr2->setVisible(false);
 ui->lb_nazf->setVisible(false);
 ui->le_nazf->setVisible(false);

 return;
}

void FNfView::ObrNew()
{
 this->setWindowTitle("Создание новой формы");

 ui->textEdit->setVisible(false);
 ui->pb_podt->setVisible(false);
 ui->pb_apodt->setVisible(false);
 ui->pb_opodt->setVisible(false);

 ui->textEdit1->setVisible(true);
 ui->textEdit1->clear();
 ui->textEdit1->repaint();
 ui->pb_Otpr->setVisible(true);
 ui->pb_otpr2->setVisible(false);
 ui->lb_nazf->setVisible(true);
 ui->le_nazf->setVisible(true);

 QDate dat;
 QTime time;

 dat=QDate::currentDate();
 time=QTime::currentTime();
 nf_podt="nf406-126-"+dat.toString("ddMMyyyy")+"-"+time.toString("hhmm")+".cup";
 ui->le_nazf->setText(nf_podt);

 return;
}
