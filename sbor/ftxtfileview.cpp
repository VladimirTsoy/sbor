#include "ftxtfileview.h"
#include "ui_ftxtfileview.h"

FTxtFileView::FTxtFileView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FTxtFileView)
{
 ui->setupUi(this);
 model=new QStringListModel(this);

 connect(ui->btnExit,SIGNAL(clicked()),this,SLOT(close()),Qt::UniqueConnection);
 connect(ui->btnZagr,SIGNAL(clicked()),this,SLOT(on_btnZagr()),Qt::UniqueConnection);
 connect(ui->btnNum,SIGNAL(clicked()),this,SLOT(on_btnNum()),Qt::UniqueConnection);
 connect(ui->btnNext,SIGNAL(clicked()),this,SLOT(on_btnNext()),Qt::UniqueConnection);
 connect(ui->btnBack,SIGNAL(clicked()),this,SLOT(on_btnBack()),Qt::UniqueConnection);
 connect(ui->btnStr,SIGNAL(clicked()),this,SLOT(on_btnStr()),Qt::UniqueConnection);
}

FTxtFileView::~FTxtFileView()
{
 delete ui;
}

void FTxtFileView::showEvent(QShowEvent *event)
{
 if (ntable == "sol") QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));

 on_btnZagr();
 if (kolstr <= 1)
 {
  ui->btnBack->setEnabled(false);
  ui->btnNext->setEnabled(false);
 }
 else
 {
  ui->btnBack->setEnabled(false);
  ui->btnNext->setEnabled(true);
 }
 ui->nstr->setText(QString::number(tekstr));
 event->accept();
}

void FTxtFileView::on_btnZagr()
{
    QStringList finfo;

    list.clear();
    this->setWindowTitle("Просмотр файла : ");
    tekstr=0;
    kolstr=0;
    t_list.clear();

//    ntable = "err";
//    nz = 43;
//    qDebug() << "ntable = " << ntable;
    if (nz > 0) //чтение файла по записи nz из таблицы ntable
    {
        if (!cOI->ReadEdxrInList(ntable,nz,list,true))
        {
            QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
            model->setStringList(t_list);
            ui->listView->setModel(model);
            return;
        }
        nfile=cOI->readedxr.isxf;
        cOI->WriteFileFromList(list,cOI->zapust.path_temp+"/aaaaaaaaaaa.aaa");
        qApp->processEvents();
    }

//    qDebug() << "nz = " << nz << "\t name_table = " << ntable << "\t size of list = " << list.size();
//    qDebug() << "nfile = " << nfile;
//    qDebug() << "path_temp = " << cOI->zapust.path_temp << "\t size t_list = " << t_list;

    if (nz == 0) //чтение файла ntable
    {
        list=cOI->ReadFileInList(ntable);
        if (list.isEmpty())
        {
            QMessageBox::critical(0,"Ошибка","Ошибка чтения файла.",QMessageBox::Yes);
            model->setStringList(t_list);
            ui->listView->setModel(model);
            return;
        }
        finfo=cOI->FileInfo(ntable);
        nfile=finfo[2];
        qApp->processEvents();
    }

    kolstr=(list.count()/5000)+1;
    tekstr=1;
    SetModel();

    return;
}

void FTxtFileView::on_btnNum()
{
 QString str;
 QStringList nlist;

 nlist.clear();

 for (qint32 i=0; i<list.count(); i++)
 {
  str=list.at(i);
  str=QString::number(i)+" : "+str;
  nlist << str;
 }

 list.clear();
 list=cOI->CopyList(nlist,0,0);
 SetModel();
 this->setWindowTitle("Просмотр файла (с нумерацией): "+nfile);
 return;
}

void FTxtFileView::SetModel()
{
 qint32 nstr;

 t_list.clear();

 nstr=((tekstr-1)*5000);
 ui->nstr->setText(QString::number(tekstr));

 t_list=cOI->CopyList(list,nstr,5000);
 model->setStringList(t_list);
 ui->listView->setModel(model);
 ui->listView->scrollToTop();
 this->setWindowTitle("Просмотр файла : "+nfile);
 return;
}

void FTxtFileView::on_btnNext()
{
 tekstr++;
 if (tekstr == kolstr) ui->btnNext->setEnabled(false);
 ui->btnBack->setEnabled(true);
 SetModel();
}

void FTxtFileView::on_btnBack()
{
 tekstr--;
 if (tekstr == 1) ui->btnBack->setEnabled(false);
 ui->btnNext->setEnabled(true);
 SetModel();
}

void FTxtFileView::on_btnStr()
{
 tekstr=ui->nstr->text().toInt();
 if (tekstr > kolstr) tekstr=kolstr;
 if (tekstr < 1) tekstr=1;
 if (tekstr == kolstr)
 {
  ui->btnNext->setEnabled(false);
  ui->btnBack->setEnabled(true);
 }
 if (tekstr == 1)
 {
  ui->btnNext->setEnabled(true);
  ui->btnBack->setEnabled(false);
 }
 SetModel();
}

//QTextCodec::setCodecForLocale(QTextCodec::codecForName("WINDOWS-1251"));
//cod="WINDOWS-1251";

void FTxtFileView::closeEvent(QCloseEvent *event)
{
 QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
 ntable.clear();
 event->accept();
}
