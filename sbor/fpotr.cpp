#include "fpotr.h"
#include "ui_fpotr.h"

FPotr::FPotr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPotr)
{
    ui->setupUi(this);
    model=0;
    show_vib=false;

    connect(ui->btnInsert,SIGNAL(clicked(bool)),this,SLOT(on_btnInsert()));
    connect(ui->btnSave,SIGNAL(clicked(bool)),this,SLOT(on_btnSave()));
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(on_btnCancel()));
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()));
    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnVib,SIGNAL(clicked(bool)),this,SLOT(on_btnVib()));

}

FPotr::~FPotr()
{
    delete ui;
}

void FPotr::showEvent(QShowEvent *event)
{
 if (model == 0)
 {
  model=new QSqlTableModel(this,cOI->db);
 }

 cOI->kol_htable=2;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Название потребителя"); cOI->hidcol[1]=false;
 cOI->setModel(model,ui->tableView,"potr");
 ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
 model->setSort(1,Qt::AscendingOrder);
 model->select();

 ui->btnVib->setVisible(show_vib);

 event->accept();
}

void FPotr::on_btnInsert()
{
 model->insertRow(model->rowCount());
 return;
}

void FPotr::on_btnSave()
{
 model->submitAll();
 return;
}

void FPotr::on_btnCancel()
{
 model->revertAll();
 return;
}

void FPotr::on_btnDel()
{
 cOI->DeleteRow(model,ui->tableView);
 return;
}

void FPotr::on_btnRefresh()
{
 QModelIndex ind;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 model->select();
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FPotr::closeEvent(QCloseEvent *event)
{
 show_vib=false;
 event->accept();
}

void FPotr::on_btnVib()
{
 QModelIndex ind;

 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 emit NzPotr(nz);
 this->close();
}
