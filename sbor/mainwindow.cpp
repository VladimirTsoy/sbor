#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();

    cOI=new ObrIzm();
    formPass=new FPass(this);
    formPass->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint);
    formPolz=new FPolz(this);
    formPolz->setWindowFlags(Qt::Window);
    formUst=new FUst(this);
    formUst->setWindowFlags(Qt::Window);
    formDs=new FDs(this);
    formDs->setWindowFlags(Qt::Window);
    formKos=new FKos(this);
    formKos->setWindowFlags(Qt::Window);
    formRnxi=new FRnxi(this);
    formRnxi->setWindowFlags(Qt::Window);
    formRnxv=new FRnxv(this);
    formRnxv->setWindowFlags(Qt::Window);
    formFull=new FFull(this);
    formFull->setWindowFlags(Qt::Window);
    formDds=new FDds(this);
    formDds->setWindowFlags(Qt::Window);
    formVrv=new FVrv(this);
    formVrv->setWindowFlags(Qt::Window);
    formPotr=new FPotr(this);
    formPotr->setWindowFlags(Qt::Window);
    formZapr=new FZapr(this);
    formZapr->setWindowFlags(Qt::Window);
    formSeansDs=new FSeansDs(this);
    formSeansDs->setWindowFlags(Qt::Window);
    formSeansPpsi=new FSeansPpsi(this);
    formSeansPpsi->setWindowFlags(Qt::Window);
    formSeansDds=new FSeansDds(this);
    formSeansDds->setWindowFlags(Qt::Window);
    formSeansVrv=new FSeansVrv(this);
    formSeansVrv->setWindowFlags(Qt::Window);
    formKolSeansDs=new FKolSeansDs(this);
    formKolSeansDs->setWindowFlags(Qt::Window);
    formServ=new FServ(this);
    formServ->setWindowFlags(Qt::Window);
    formStatServ=new FStatServ(this);
    formStatServ->setWindowFlags(Qt::Window);
    formSba=new FSba(this);
    formSba->setWindowFlags(Qt::Window);
    formSist=new FSist(this);
    formSist->setWindowFlags(Qt::Window);
    formRpom=new FRPom(this);
    formRpom->setWindowFlags(Qt::Window);
    formNosd=new FNosd(this);
    formNosd->setWindowFlags(Qt::Window);
    formPpsi=new FPPsi(this);
    formPpsi->setWindowFlags(Qt::Window);
    formUstnf=new FUstnf(this);
    formUstnf->setWindowFlags(Qt::Window);
    formCogi=new FCogi(this);
    formCogi->setWindowFlags(Qt::Window);
    formCup=new FCup(this);
    formCup->setWindowFlags(Qt::Window);
    formAutoZapr=new FAutoZapr(this);
    formAutoZapr->setWindowFlags(Qt::Window);
    formSp3=new FSp3(this);
    formSp3->setWindowFlags(Qt::Window);
    formSol=new FSol(this);
    formSol->setWindowFlags(Qt::Window);

    connect(formPass,SIGNAL(no()),this,SLOT(close()));
    connect(formPass,SIGNAL(ok()),this,SLOT(Start()));
    connect(ui->action_CREATEBD,SIGNAL(triggered()),this,SLOT(CreateBd()),Qt::UniqueConnection);
    connect(ui->action_FNOS,SIGNAL(triggered()),this,SLOT(ShowFNos()),Qt::UniqueConnection);
    connect(ui->action_FPOLZ,SIGNAL(triggered()),this,SLOT(ShowFPolz()),Qt::UniqueConnection);
    connect(ui->action_FUST,SIGNAL(triggered()),this,SLOT(ShowFUst()),Qt::UniqueConnection);
    connect(ui->action_IZMPOLZ,SIGNAL(triggered()),this,SLOT(Pass()),Qt::UniqueConnection);
    connect(ui->action_EXIT,SIGNAL(triggered()),this,SLOT(close()),Qt::UniqueConnection);
    connect(ui->action_FDS,SIGNAL(triggered()),this,SLOT(ShowFDs()),Qt::UniqueConnection);
    connect(ui->action_FDZS,SIGNAL(triggered()),this,SLOT(ShowFDzs()),Qt::UniqueConnection);
    connect(ui->action_FKOS,SIGNAL(triggered()),this,SLOT(ShowFKos()),Qt::UniqueConnection);
    connect(ui->action_BDSI,SIGNAL(triggered()),this,SLOT(ShowBdsi()),Qt::UniqueConnection);
    connect(ui->action_BDSV,SIGNAL(triggered()),this,SLOT(ShowBdsv()),Qt::UniqueConnection);
    connect(ui->action_METEO,SIGNAL(triggered()),this,SLOT(ShowMeteo()),Qt::UniqueConnection);
    connect(ui->action_FFULL,SIGNAL(triggered()),this,SLOT(ShowFFull()),Qt::UniqueConnection);
    connect(ui->action_FDDS,SIGNAL(triggered()),this,SLOT(ShowFDds()),Qt::UniqueConnection);
    connect(ui->action_FVRV,SIGNAL(triggered()),this,SLOT(ShowFVrv()),Qt::UniqueConnection);
    connect(ui->action_FPOTR,SIGNAL(triggered()),this,SLOT(ShowFPotr()),Qt::UniqueConnection);
    connect(ui->action_FZAPR,SIGNAL(triggered()),this,SLOT(ShowFZapr()),Qt::UniqueConnection);
    connect(ui->action_FSEANSDS,SIGNAL(triggered()),this,SLOT(ShowFSeansDs()),Qt::UniqueConnection);
    connect(ui->action_FSEANSDZS,SIGNAL(triggered()),this,SLOT(ShowFSeansDzs()),Qt::UniqueConnection);
    connect(ui->action_FSEANSPPSI,SIGNAL(triggered()),this,SLOT(ShowFSeansPpsi()),Qt::UniqueConnection);
    connect(ui->action_FSEANSDDS,SIGNAL(triggered()),this,SLOT(ShowFSeansDds()),Qt::UniqueConnection);
    connect(ui->action_FSEANSVRV,SIGNAL(triggered()),this,SLOT(ShowFSeansVrv()),Qt::UniqueConnection);
    connect(ui->action_FKOLSEANSDS,SIGNAL(triggered()),this,SLOT(ShowFKolSeansDs()),Qt::UniqueConnection);
    connect(ui->action_FKOLSEANSDZS,SIGNAL(triggered()),this,SLOT(ShowFKolSeansDzs()),Qt::UniqueConnection);
    connect(ui->action_FSERV,SIGNAL(triggered()),this,SLOT(ShowFServ()),Qt::UniqueConnection);
    connect(ui->action_FSTATSERV,SIGNAL(triggered()),this,SLOT(ShowFStatServ()),Qt::UniqueConnection);
    connect(ui->action_FSBA,SIGNAL(triggered()),this,SLOT(ShowFSba()),Qt::UniqueConnection);
    connect(ui->action_SPO,SIGNAL(triggered()),this,SLOT(ShowFSpo()),Qt::UniqueConnection);
    connect(ui->action_FSIST,SIGNAL(triggered()),this,SLOT(ShowFSist()),Qt::UniqueConnection);
    connect(ui->action_RPOM,SIGNAL(triggered()),this,SLOT(ShowFRpom()),Qt::UniqueConnection);
    connect(ui->action_STARTSBOR,SIGNAL(triggered()),this,SLOT(StartSbor()),Qt::UniqueConnection);
    connect(ui->action_STARTSORT,SIGNAL(triggered()),this,SLOT(StartSort()),Qt::UniqueConnection);
    connect(ui->action_FPPSI,SIGNAL(triggered()),this,SLOT(ShowFPpsi()),Qt::UniqueConnection);
    connect(ui->action_FUSTNF,SIGNAL(triggered()),this,SLOT(ShowFUstnf()),Qt::UniqueConnection);
    connect(ui->action_STARTFORM,SIGNAL(triggered()),this,SLOT(StartForm()),Qt::UniqueConnection);
    connect(ui->action_FCOGI,SIGNAL(triggered()),this,SLOT(ShowFCogi()),Qt::UniqueConnection);
    connect(ui->action_FCUP,SIGNAL(triggered()),this,SLOT(ShowFCup()),Qt::UniqueConnection);
    connect(ui->action_FAUTOZAPR,SIGNAL(triggered()),this,SLOT(ShowFAutoZapr()),Qt::UniqueConnection);
    connect(ui->action_FSP3,SIGNAL(triggered()),this,SLOT(ShowFSp3()),Qt::UniqueConnection);
    connect(ui->action_FSOL,SIGNAL(triggered()),this,SLOT(ShowFSol()),Qt::UniqueConnection);


    QTimer::singleShot(100, this, SLOT(Pass()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cOI;
//    QApplication::quit();
}

void MainWindow::Pass()
{
 formPass->cOI=cOI;
 formPass->title="Sbor";
 formPass->show();
}

void MainWindow::Start()
{
 if (cOI->os == "WIN") QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

  cOI->komp_name_ust=cOI->komp_name;
  cOI->komp_name=cOI->komp_name+"_sbor";

  if (cOI->db.isOpen() && cOI->db.databaseName() == "geoik")
  {

   cOI->DeleteRow("obr","naz_komp",cOI->komp_name,true);

   nz_ust=cOI->FindRow("ust","nazkomp",cOI->komp_name_ust,true);
   if (nz_ust <= 0)
   {
    cOI->ClearUst();
    cOI->zapust.nazkomp=cOI->komp_name_ust;
    cOI->InsertUst(true);
   }
   else cOI->ReadUst(nz_ust,true);

   nz_ustnf=cOI->FindRow("ustnf","komp",cOI->komp_name_ust,true);
   if (nz_ustnf <= 0)
   {
    cOI->ClearUstnf();
    cOI->zapustnf.komp=cOI->komp_name_ust;
    cOI->InsertUstnf(true);
   }
   else cOI->ReadUstnf(nz_ustnf,true);

   cOI->ClearSlu();

   QString soob;
   QSqlQuery query;

   soob="SELECT usesuper FROM pg_user WHERE usename = '"+cOI->nikp+"'";
   if (cOI->ExecQuery(query,soob,true) != -1)
   {
    cOI->pr_superuser=cOI->spis_field[0].toBool();
   }


   if (cOI->tek_nz_nos == 0)
   {
    if (! cOI->GetNos(true))
    {
     int k=QMessageBox::warning(0,"Выбор носителя","Нет ни одного архивного носителя. Установить?",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
     if (k == QMessageBox::No)
     {
      QApplication::exit(1);
      return;
     }
     else
     {
      formNosd->cOI=cOI;
      formNosd->show();
     }
    }
   }

   if (cOI->zapust.pr_obrzapr == 1)
   {
    QList<qlonglong> nz_zapr;
    nz_zapr=cOI->FindRowAvtZapr(true,cOI->komp_name_ust);
    if (!nz_zapr.isEmpty())
   {
    int k=QMessageBox::warning(0,"Обработка запросов","Обработать автоматические запросы?",QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes);
    if (k == QMessageBox::Yes)
    {
     formAutoZapr->cOI=cOI;
     formAutoZapr->show();
     return;
    }
   }
   }
  }

 return;
}

void MainWindow::CreateBd()
{
 if (!cOI->CreateBD()) return;

 this->close();

}

void MainWindow::ShowFNos()
{
 formNosd->cOI=cOI;
 formNosd->show();
}

void MainWindow::ShowFPolz()
{
 formPolz->cOI=cOI;
 formPolz->show();
}

void MainWindow::ShowFUst()
{
 formUst->cOI=cOI;
 formUst->show();
}

void MainWindow::ShowFDs()
{
 formDs->cOI=cOI;
 formDs->nazis="DS";
 formDs->show();
}

void MainWindow::ShowFDzs()
{
 formDs->cOI=cOI;
 formDs->nazis="DZS";
 formDs->show();
}

void MainWindow::ShowFKos()
{
 formKos->cOI=cOI;
 formKos->show();
}

void MainWindow::ShowBdsi()
{
 formRnxi->cOI=cOI;
 formRnxi->pr_meteo=false;
 formRnxi->show();
}

void MainWindow::ShowBdsv()
{
 formRnxv->cOI=cOI;
 formRnxv->show();
}

void MainWindow::ShowMeteo()
{
 formRnxi->cOI=cOI;
 formRnxi->pr_meteo=true;
 formRnxi->show();
}

void MainWindow::ShowFFull()
{
 formFull->cOI=cOI;
 formFull->show();
}

void MainWindow::ShowFDds()
{
 formDds->cOI=cOI;
 formDds->show();
}

void MainWindow::ShowFVrv()
{
 formVrv->cOI=cOI;
 formVrv->show();
}

void MainWindow::ShowFPotr()
{
 formPotr->cOI=cOI;
 formPotr->show();
}

void MainWindow::ShowFZapr()
{
 formZapr->cOI=cOI;
 formZapr->show();
}

void MainWindow::ShowFSeansDs()
{
 formSeansDs->cOI=cOI;
 formSeansDs->tip="DS";
 formSeansDs->show();
}

void MainWindow::ShowFSeansDzs()
{
 formSeansDs->cOI=cOI;
 formSeansDs->tip="DZS";
 formSeansDs->show();
}

void MainWindow::ShowFSeansPpsi()
{
 formSeansPpsi->cOI=cOI;
 formSeansPpsi->show();
}

void MainWindow::ShowFSeansDds()
{
 formSeansDds->cOI=cOI;
 formSeansDds->show();
}

void MainWindow::ShowFSeansVrv()
{
 formSeansVrv->cOI=cOI;
 formSeansVrv->show();
}

void MainWindow::ShowFKolSeansDs()
{
 formKolSeansDs->cOI=cOI;
 formKolSeansDs->tip="DS";
 formKolSeansDs->show();
}

void MainWindow::ShowFKolSeansDzs()
{
 formKolSeansDs->cOI=cOI;
 formKolSeansDs->tip="DZS";
 formKolSeansDs->show();
}

void MainWindow::ShowFServ()
{
 formServ->cOI=cOI;
 formServ->show();
}

void MainWindow::ShowFStatServ()
{
 formStatServ->cOI=cOI;
 formStatServ->show();
}

void MainWindow::ShowFSba()
{
 formSba->cOI=cOI;
 formSba->show();
}

void MainWindow::ShowFSpo()
{
 formDs->cOI=cOI;
 formDs->nazis="SPO";
 formDs->show();
}

void MainWindow::ShowFSist()
{
 formSist->cOI=cOI;
 formSist->show();
}

void MainWindow::ShowFRpom()
{
// QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
// if (!lock.tryLock(100)) return;

 formRpom->cOI=cOI;
 formRpom->show();
}

void MainWindow::StartSbor()
{
 QProcess *proc;
 QString p;

 p=QApplication::applicationDirPath()+"/AutoSbor "+cOI->nikp+" "+cOI->passp+" "+cOI->hostp;

 proc=new QProcess;
 proc->startDetached(p);

 return;
}

void MainWindow::StartSort()
{
 QProcess *proc;
 QString p;

 p=QApplication::applicationDirPath()+"/AutoSort "+cOI->nikp+" "+cOI->passp+" "+cOI->hostp;


 proc=new QProcess;
 proc->startDetached(p);

 return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
 qApp->quit();
 event->accept();
}

void MainWindow::ShowFPpsi()
{
 formPpsi->coI=cOI;
 formPpsi->show();
}

void MainWindow::ShowFUstnf()
{
 formUstnf->cOI=cOI;
 formUstnf->show();
}

void MainWindow::StartForm()
{
 QProcess *proc;
 QString p;

 p=QApplication::applicationDirPath()+"/AutoForm "+cOI->nikp+" "+cOI->passp+" "+cOI->hostp;

 proc=new QProcess;
 proc->startDetached(p);

 return;
}

void MainWindow::ShowFCogi()
{
 formCogi->cOI=cOI;
 formCogi->show();
}

void MainWindow::ShowFCup()
{
 formCup->cOI=cOI;
 formCup->show();
}

void MainWindow::ShowFAutoZapr()
{
 formAutoZapr->cOI=cOI;
 formAutoZapr->show();
}

void MainWindow::ShowFSp3()
{
 formSp3->cOI=cOI;
 formSp3->show();
}

void MainWindow::ShowFSol()
{
 formSol->cOI=cOI;
 formSol->show();
}
