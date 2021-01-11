#include "fsist.h"
#include "ui_fsist.h"

FSist::FSist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FSist)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);

    ui->dtn->setDate(QDate::currentDate());
    ui->dtk->setDate(QDate::currentDate());

    connect(ui->pbRefresh,SIGNAL(clicked(bool)),this,SLOT(on_pbRefresh()));
}

FSist::~FSist()
{
    delete ui;
}

void FSist::showForm()
{
 FormSql();
 cOI->kol_htable=8;
 cOI->hidcol[0]=true;
 cOI->htable[1]=tr("Дата"); cOI->hidcol[1]=false;
 cOI->htable[2]=tr("Время"); cOI->hidcol[2]=false;
 cOI->htable[3]=tr("Название файла"); cOI->hidcol[3]=false;
 cOI->htable[4]=tr("Описание обработки"); cOI->hidcol[4]=false;
 cOI->htable[5]=tr("Результат обработки"); cOI->hidcol[5]=false;
 cOI->htable[6]=tr("Название АРМ"); cOI->hidcol[6]=false;
 cOI->htable[7]=tr("Пользователь"); cOI->hidcol[7]=false;
 ind=model->index(0,0);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
 return;
}

void FSist::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FSist::on_pbRefresh()
{
 qlonglong nz;

 FormSql();
 ind=cOI->getCurrentIndex(ui->tableView,model,0,nz);
 cOI->setQueryModel(model,ui->tableView,strsql,true);
 ui->tableView->setCurrentIndex(ind);
}

void FSist::FormSql()
{
 QString str_sel,str_ord,str_where;

 str_sel="SELECT sist.nz,sist.dt,to_char(sist.vr,'HH24:MI:SS') AS svr,sist.naz_ed,kodobr.soob,sist.sost,sist.naz_komp,sist.polz FROM sist,kodobr";
 str_ord=" ORDER BY sist.dt ASC,sist.vr ASC,sist.naz_ed ASC";
 str_where=" WHERE sist.nz_kodobr = kodobr.nz";
 str_where=str_where+" AND sist.dt BETWEEN '"+ui->dtn->date().toString("dd-MM-yyyy")+"' AND '"+ui->dtk->date().toString("dd-MM-yyyy")+"'";

 if (ui->cbRez->currentIndex() == 1) str_where=str_where+" AND sist.sost = 'выполнено'";
 if (ui->cbRez->currentIndex() == 2) str_where=str_where+" AND sist.sost = 'не выполнено'";

 strsql=str_sel+str_where+str_ord;
}
