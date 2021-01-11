#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->showMinimized();

    cOI=new ObrIzm();
    formPass=new FPass(this);
    formPass->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint);
    icon.addPixmap(QPixmap(":/res/1/database.png"));
    icon_red.addPixmap(QPixmap(":/res/1/database_red.png"));
    icon_green.addPixmap(QPixmap(":/res/1/database_green.png"));

    connect(formPass,SIGNAL(no()),this,SLOT(Cl()));
    connect(formPass,SIGNAL(ok()),this,SLOT(Start()));
    connect(&t_si,SIGNAL(timeout()),SLOT(Obr()),Qt::UniqueConnection);
    connect(ui->btnStop,SIGNAL(clicked(bool)),this,SLOT(on_btnStop()),Qt::UniqueConnection);

    QTimer::singleShot(100, this, SLOT(Pass()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Pass()
{
    QSqlError strerr;
    QString soob;
    formPass;
    if (name.isEmpty() || pas.isEmpty() || hst.isEmpty())
    {
        formPass->cOI=cOI;
        formPass->title="AutoSbor";
        formPass->show();
    }
    else
    {
        if (!cOI->createConnectionPSQL("geoik",name,pas,hst,strerr))
        {
            soob=QObject::tr("Ошибка подключения к БД. ")+strerr.text().toStdString().c_str();
            QMessageBox::critical(0,tr("Ошибка"),soob,QMessageBox::Yes);
            cOI->WriteLog(soob);
            Cl();
        }
        else Start();
    }
}

void MainWindow::Start()
{
    QStringList vol;

    if (cOI->os == "WIN") QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    cOI->komp_name_ust=cOI->komp_name;
    cOI->komp_name=cOI->komp_name+"auto";

    if (cOI->db.isOpen() && cOI->db.databaseName() == "geoik")
    {
        if (cOI->tek_nz_nos == 0)
        {
            if (! cOI->GetNos(true))
            {
                QMessageBox::warning(0,"Предупреждение","Нет зарегистрированных носителей.",QMessageBox::Yes);
                cOI->WriteLog("Нет зарегистрированных носителей.");
                QApplication::exit(1);
                return;
            }

            else
            {
                vol.clear();
                vol << "vol";
                if (!cOI->WriteFileFromList(vol,cOI->tek_path_arx+"/vol.vol"))
                {
                    QMessageBox::warning(0,"Предупреждение","Нет доступа к архивному носителю.",QMessageBox::Yes);
                    cOI->WriteLog("Нет доступа к архивному носителю.");
                    QApplication::exit(1);
                    return;
                }
            }
        }

        ui->lb_vip->clear();

        cOI->DeleteRow("obr","naz_komp",cOI->komp_name,true);

        nz_ust=cOI->FindRow("ust","nazkomp",cOI->komp_name_ust,true);
        if (nz_ust <= 0) {pr_close=true; this->close(); return;}
        else cOI->ReadUst(nz_ust,true);

        cOI->ClearSlu();
    }

    pr_close=false;
    cOI->pr_stop=false;

    if (!cOI->zapust.pravt)
    {
        ui->label->setPixmap(QPixmap(":/res/1/database_red.png"));
        this->setWindowIcon(icon_red);
        ui->lb_vip->setText("Обработка не производится.");
    }
    else
    {
        ui->label->setPixmap(QPixmap(":/res/1/database.png"));
        this->setWindowIcon(icon);
    }

    qApp->processEvents();

    QLockFile lock(QDir::temp().absoluteFilePath("Sbor.loc"));
    if (lock.tryLock(100))
        cOI->ClearDir(cOI->zapust.path_temp);

    if (cOI->zapust.km <= 0) {pr_close=true; this->close(); return;}

    cOI->WriteLog("autosbor : Запуск программы.");

    t_si.start(cOI->zapust.km*1000);
}

void MainWindow::ReadUst()
{
    cOI->ReadUst(nz_ust,false);

    if (!cOI->zapust.pravt)
    {
        ui->label->setPixmap(QPixmap(":/res/1/database_red.png"));
        this->setWindowIcon(icon_red);
        ui->lb_vip->setText("Обработка не производится.");
    }

    qApp->processEvents();

    if (cOI->zapust.km <= 0) {pr_close=true; this->close(); return;}

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
}

void MainWindow::Obr()
{
    t_si.stop();
    ReadUst();
    if (!cOI->zapust.pravt)
    {
        t_si.start(cOI->zapust.km*1000);
        return;
    }
    ui->label->setPixmap(QPixmap(":/res/1/database_green.png"));
    this->setWindowIcon(icon_green);
    qApp->processEvents();
    Run();
    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ui->label->setPixmap(QPixmap(":/res/1/database.png"));
    ui->lb_vip->setText("Обработка не производится.");
    t_si.start(cOI->zapust.km*1000);
    this->setWindowIcon(icon);
    qApp->processEvents();
}

void MainWindow::Run()
{
    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obrsi)
    {
        ui->lb_vip->setText("Выполняется сбор необработанной ИИ.");
        this->repaint();
        cOI->ObrSi(cOI->zapust.path_si,cOI->zapust.path_temp,cOI->zapust.kol_obrsi,false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr11_15)
    {
        ui->lb_vip->setText("Выполняется конвертирование файла ППСИ (КВИ,КВНО).");
        this->repaint();
        cOI->ObrKod11_15(cOI->zapust.kol_obr11_15,cOI->zapust.path_temp,false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    if (!cOI->zapust.pravt) return;
    ReadUst();
    if (cOI->zapust.pr_obr12)
    {
        ui->lb_vip->setText("Выполняется конвертирование файла ППСИ (ВРВ).");
        this->repaint();
        cOI->ObrKod12(cOI->zapust.kol_obr12,cOI->zapust.path_temp,false,0);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr14)
    {
        ui->lb_vip->setText("Файл ДДС КВИ разбивается по суткам.");
        this->repaint();
        cOI->ObrKod14(cOI->zapust.kol_obr14,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr13)
    {
        ui->lb_vip->setText("Файл ДДС КВИ объединяется в суточный.");
        this->repaint();
        cOI->ObrKod13(cOI->zapust.kol_obr13,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr17)
    {
        ui->lb_vip->setText("Файл ДДС КВНО разбивается по суткам.");
        this->repaint();
        cOI->ObrKod17(cOI->zapust.kol_obr17,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr16)
    {
        ui->lb_vip->setText("Файл ДДС КВНО объединяется в суточный.");
        this->repaint();
        cOI->ObrKod16(cOI->zapust.kol_obr16,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr19)
    {
        ui->lb_vip->setText("Файл ВРВ разбивается по суткам.");
        this->repaint();
        cOI->ObrKod19(cOI->zapust.kol_obr19,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr18)
    {
        ui->lb_vip->setText("Файл ВРВ объединяется в суточный.");
        this->repaint();
        cOI->ObrKod18(cOI->zapust.kol_obr18,cOI->zapust.path_temp,false);
        cOI->ObrKod6(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr10)
    {
        ui->lb_vip->setText("Выполняется конвертирование файла БДС (*.*L).");
        this->repaint();
        cOI->ObrKod10(cOI->zapust.kol_obr10,cOI->zapust.path_temp,false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr2)
    {
        ui->lb_vip->setText("Выполняется переименование файла измерений БДС.");
        this->repaint();
        cOI->ObrKod2(cOI->zapust.kol_obr2,cOI->zapust.path_temp,false);
        ui->lb_vip->setText("Выполняется преобразование файла измерений БДС.");
        this->repaint();
        cOI->ObrKod21(cOI->zapust.kol_obr2,cOI->zapust.path_temp,false);
        ui->lb_vip->setText("Выполняется преобразование навигационного файла БДС.");
        this->repaint();
        cOI->ObrKod22(cOI->zapust.kol_obr2,cOI->zapust.path_temp,false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr7)
    {
        ui->lb_vip->setText("Выполняется разделение навигационного файла БДС по ГНСС.");
        this->repaint();
        cOI->ObrKod7(cOI->zapust.kol_obr7,cOI->zapust.path_temp,false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr3)
    {
        ui->lb_vip->setText("Выполняется разделение файла измерений БДС по суткам.");
        this->repaint();
        cOI->ObrKod3(cOI->zapust.kol_obr3,cOI->zapust.path_temp,false);
        cOI->ObrKod20(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr4)
    {
        ui->lb_vip->setText("Выполняется объединение файлов измерений БДС в суточный.");
        this->repaint();
        cOI->ObrKod4(cOI->zapust.kol_obr4,cOI->zapust.path_temp,false);
        cOI->ObrKod20(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr8)
    {
        ui->lb_vip->setText("Выполняется разделение навигационного файла БДС по суткам.");
        this->repaint();
        cOI->ObrKod8(cOI->zapust.kol_obr8,cOI->zapust.path_temp,false);
        cOI->ObrKod20(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    ReadUst();
    if (!cOI->zapust.pravt) return;
    if (cOI->zapust.pr_obr9)
    {
        ui->lb_vip->setText("Выполняется объединение навигационных файлов БДС в суточный.");
        this->repaint();
        cOI->ObrKod9(cOI->zapust.kol_obr9,cOI->zapust.path_temp,false);
        cOI->ObrKod20(false);
    }
    qApp->processEvents();

    if (cOI->pr_stop) {pr_close=true; this->close(); return;}
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!pr_close) event->ignore();
    else
    {
        cOI->WriteLog("autosbor : Завершение работы программы.");
        QApplication::quit();
        event->accept();
    }

    return;
}

void MainWindow::Cl()
{
    pr_close=true;
    this->close();
}

void MainWindow::on_btnStop()
{
    cOI->pr_stop=true;
    ui->lb_vip->setText("Выполняется завершение работы. Ждите.");
    this->repaint();
    qApp->processEvents();
    return;
}
