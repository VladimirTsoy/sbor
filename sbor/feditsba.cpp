#include "feditsba.h"
#include "ui_feditsba.h"

FEditSba::FEditSba(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FEditSba)
{
    ui->setupUi(this);

    connect(ui->pbSave,SIGNAL(clicked(bool)),this,SLOT(on_pbSave()));
    connect(ui->pbCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
}

FEditSba::~FEditSba()
{
    delete ui;
}

void FEditSba::showEvent(QShowEvent *event)
{
 QStringList list;
 QSqlQuery query;
 QString str;

 str="SELECT kodba.nz,kodba.naz FROM kodba ORDER BY kodba.nz";
 if (!query.exec(str)) return;
 list=cOI->GetRowAll(query,1);
 ui->cbNba->clear();
 ui->cbNba->setMaxCount(list.count());
 ui->cbNba->addItems(list);
 cOI->ClearSba();
 if (nz > 0)
 {
  if (!cOI->ReadSba(nz,true)) return;
 }

 ui->cbNba->setCurrentIndex(cOI->zapsba.nz_kodba);
 ui->cbNka->setCurrentIndex(cOI->zapsba.nka-126);
 if (ui->cbNka->currentIndex() < 0) ui->cbNka->setCurrentIndex(0);
 ui->dtVik->setDate(cOI->zapsba.dvik);
 ui->dtVik->setTime(cOI->zapsba.vvik);
 ui->dtVkl->setDate(cOI->zapsba.dvkl);
 ui->dtVkl->setTime(cOI->zapsba.vvkl);

 event->accept();
}

void FEditSba::FormZap()
{
 cOI->zapsba.nz_kodba=ui->cbNba->currentIndex();
 cOI->zapsba.dvik=ui->dtVik->date();
 cOI->zapsba.vvik=ui->dtVik->time();
 cOI->zapsba.dvkl=ui->dtVkl->date();
 cOI->zapsba.vvkl=ui->dtVkl->time();
 cOI->zapsba.nka=ui->cbNka->currentIndex()+126;
 if (cOI->zapsba.nka < 126) cOI->zapsba.nka=126;
}

void FEditSba::on_pbSave()
{
 FormZap();
 if (nz == 0) cOI->InsertSba(true);
 else cOI->UpdateSba(true);
 emit ok();
 this->close();
}
