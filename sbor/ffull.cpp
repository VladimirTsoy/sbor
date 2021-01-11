#include "ffull.h"
#include "ui_ffull.h"


FFull::FFull(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFull)
{
    ui->setupUi(this);
    query_select="SELECT fullppsi.nz,fullppsi.naz,fullppsi.npn,fullppsi.kan,fullppsi.nka,fullppsi.dtn,fullppsi.dtk,"
                 "kodobr.soob,strk.sod FROM fullppsi,kodobr,strk ";
    query_where="WHERE fullppsi.kodobr = kodobr.nz AND fullppsi.strk = strk.nz ";
    query_order="ORDER BY fullppsi.nka ASC,fullppsi.godn ASC,fullppsi.sutn ASC,(fullppsi.sekn/60) ASC,fullppsi.kan ASC;";
    m_fullppsi=new QSqlQueryModel(this);
    formTxtFileView=new FTxtFileView(this);
    formTxtFileView->setWindowFlags(Qt::Window);
    formStatFull=new FStatFull(this);
    formStatFull->setWindowFlags(Qt::Window);
    formEditfull=new FEditFull(this);
    formEditfull->setWindowFlags(Qt::Window);

    timer=new QTimer(this);

    ui->de2->setDate(QDate::currentDate());
    query_where1.clear();

    PPSI = new ppsi(this->parentWidget());
    ui->painGraf->installEventFilter(this);
    PPSI->installEventFilter(this);
//    this->installEventFilter(this);

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()),Qt::UniqueConnection);
    connect(ui->btnListProt,SIGNAL(clicked(bool)),this,SLOT(on_btnListProt()),Qt::UniqueConnection);
    connect(ui->btnStat,SIGNAL(clicked(bool)),this,SLOT(on_btnStat()),Qt::UniqueConnection);
    connect(ui->btnEdit,SIGNAL(clicked(bool)),this,SLOT(on_btnEdit()),Qt::UniqueConnection);
    connect(formEditfull,SIGNAL(ok()),this,SLOT(on_btnRefresh()),Qt::UniqueConnection);
    connect(ui->btnDel,SIGNAL(clicked(bool)),this,SLOT(on_btnDel()),Qt::UniqueConnection);
    connect(ui->btnKonv,SIGNAL(clicked(bool)),this,SLOT(on_btnKonv()),Qt::UniqueConnection);
    connect(ui->btnVigr,SIGNAL(clicked(bool)),this,SLOT(on_btnVigr()),Qt::UniqueConnection);
    connect(timer,SIGNAL(timeout()),SLOT(ShowPb()));
    connect(&tobr12,SIGNAL(finished()),SLOT(EndKonv()));
    connect(ui->cb1k,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cb2k,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbNeobr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbObr,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbOch,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbDat,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbOtbNpn,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->cbNka,SIGNAL(currentIndexChanged(int)),this,SLOT(on_btnRefresh()));
    connect( ui->painGraf, SIGNAL(clicked(bool)), this, SLOT(DravPPSI()), Qt::UniqueConnection );
//    connect( ui->painGraf, SIGNAL(clicked(bool)), this, SLOT( a->focusChanged(this, PPSI) ) );
//    connect(qApp, SIGNAL(focusChanged(QWidget* this, QWidget* PPSI)), this, SLOT(setWidgets(QWidget*, QWidget*)));

    ui->btnRefresh->setToolTip("Обновить содержимое таблицы");
    ui->btnStat->setToolTip("Просмотр статистки по сеансам");
    ui->btnDel->setToolTip("Удалить сеанс");
    ui->btnEdit->setToolTip("Редактировать поля записи таблицы");
    ui->btnVigr->setToolTip("Выгрузить сеанс на диск");
    ui->btnKonv->setToolTip("Выполнить конвертирование сеанса для ВРВ по одному каналу");
    ui->btnListProt->setToolTip("Просмотр протокола работы конвертера");
}


FFull::~FFull()
{
    delete ui;
}


void FFull::showEvent(QShowEvent *event)
{
    showForm();
    event->accept();
}


bool FFull::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::Close && watched == PPSI )
        this->setVisible(true);

    return QWidget::eventFilter(watched, event);
}


void FFull::showForm()
{
    qlonglong nz_ust;

    list_npn=cOI->SpisNPN("fullppsi","",true);
    ui->cbNpn->clear();
    ui->cbNpn->setMaxCount(list_npn.count());
    ui->cbNpn->addItems(list_npn);
    ui->cbOtbNpn->setChecked(false);

    cOI->kol_htable=9;
    cOI->hidcol[0]=true;
    cOI->htable[1]=tr("Название файла"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("№ НПН"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("№ канала"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("№ КА"); cOI->hidcol[4]=false;
    cOI->htable[5]=tr("Дата,время начала сеанса"); cOI->hidcol[5]=false;
    cOI->htable[6]=tr("Дата,время конца сеанса"); cOI->hidcol[6]=false;
    cOI->htable[7]=tr("Дальнейшая обработка"); cOI->hidcol[7]=false;
    cOI->htable[8]=tr("Контроль"); cOI->hidcol[8]=false;
    ind=m_fullppsi->index(0,0);
    ui->tableView->setCurrentIndex(ind);
//    ui->tableView->setAlternatingRowColors( true );
//    ui->tableView->resizeRowsToContents();
//    ui->tableView->resizeColumnsToContents();

    FormFiltr();
    cOI->setQueryModel(m_fullppsi,ui->tableView,query_select+query_where+query_where1+query_order,true);
    ui->pb->setMinimum(0);
    ui->pb->setMaximum(100);
    ui->pb->setValue(0);
    ui->pb->setVisible(false);

    nz_ust=cOI->FindRow("ust","nazkomp",cOI->komp_name_ust,true);
    if (nz_ust > 0)
    {
        cOI->ReadUst(nz_ust,false);
        if (!cOI->zapust.pravt) ui->btnKonv->setEnabled(true);
    }

    return;
}


void FFull::on_btnRefresh()
{
    qlonglong nz;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    FormFiltr();
    cOI->setQueryModel(m_fullppsi,ui->tableView,query_select+query_where+query_where1+query_order,true);
    ui->tableView->setCurrentIndex(ind);

    return;
}


void FFull::FormFiltr()
{
    query_where1.clear();

    query_where1=FormFiltrKan()+FormFiltrSost()+FormFiltrDat()+FormFiltrNka()+FormFiltrNpn();

    return;
}


QString FFull::FormFiltrKan()
{
    QString ret;

    if ((ui->cb1k->isChecked()) && (ui->cb2k->isChecked())) {ret.clear(); return ret;}
    if ((!ui->cb1k->isChecked()) && (!ui->cb2k->isChecked()))
    {
        ui->cb1k->setChecked(true);
        ui->cb2k->setChecked(true);
        ret.clear(); return ret;
    }
    if (ui->cb1k->isChecked())
    {
        ret="AND kan = 1 ";
        return ret;
    }
    if (ui->cb2k->isChecked())
    {
        ret="AND kan = 2 ";
        return ret;
    }

    return ret;
}


QString FFull::FormFiltrSost()
{
    QString ret;
    qint32 sost=0;

    if (ui->cbObr->isChecked()) sost=100;
    if (ui->cbNeobr->isChecked()) sost=sost+10;
    if (ui->cbOch->isChecked()) sost=sost+1;
    ret.clear();

    if (sost == 111) {ret.clear(); return ret;}
    if (sost == 0)
    {
        ui->cbObr->setChecked(true);
        ui->cbNeobr->setChecked(true);
        ui->cbOch->setChecked(true);
        ret.clear(); return ret;
    }
    if (sost >= 100)
    {
        ret="AND (kodobr = 5";
        sost=sost-100;
    }
    if (sost >= 10)
    {
        if (ret.isEmpty()) ret="AND (kodobr = 11 OR kodobr = 15 OR kodobr = 12";
        else ret=ret+" OR kodobr = 11 OR kodobr = 15 OR kodobr = 12";
        sost=sost-10;
    }
    if (sost == 1)
    {
        if (ret.isEmpty()) ret="AND (kodobr = 1";
        else ret=ret+" OR kodobr = 1";
    }

    if (!ret.isEmpty()) ret=ret+") ";

    return ret;
}


QString FFull::FormFiltrDat()
{
    QString ret;
    QDate dtn,dtk;
    qint32 dns,dks;

    ret.clear();

    if (!ui->cbDat->isChecked()) return ret;

    dtn=ui->de1->date();
    dtk=ui->de2->date();
    dns=(dtn.year()*1000)+dtn.dayOfYear();
    dks=(dtk.year()*1000)+dtk.dayOfYear();

    ret="AND (((godn*1000)+sutn) >= "+QString::number(dns)+" AND ((godn*1000)+sutn) <= "+QString::number(dks)+") ";

    return ret;
}


void FFull::on_btnListProt()
{
    qlonglong nz;
    QString nprot;
    QString pref;
    QFile f;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;

    if (!cOI->ReadEdxr("fullppsi",nz,true)) return;
    nprot=cOI->readedxr.arxf;

    if (ui->rbKvi->isChecked()) pref="PRTKVI_";
    if (ui->rbKvno->isChecked()) pref="PRTKVNO_";
    if (ui->rbVrv->isChecked()) pref="PRTVRV_";

    nprot=pref+nprot.mid(5);
    nprot=cOI->readedxr.path_arx+"/"+nprot;

    f.setFileName(nprot);
    if (!f.exists()) return;

    formTxtFileView->nz=0;
    formTxtFileView->ntable=nprot;
    formTxtFileView->show();

    return;
}


void FFull::on_btnStat()
{
    qlonglong nz;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;

    formStatFull->cOI=cOI;
    formStatFull->nz_full=nz;
    formStatFull->show();
}


void FFull::on_btnEdit()
{
    qlonglong nz;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;

    formEditfull->cOI=cOI;
    formEditfull->nz_full=nz;
    formEditfull->show();


    return;
}


void FFull::on_btnDel()
{
    qlonglong nz;
    QString soob;
    QString naz;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;
    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,1,naz);

    soob=QObject::tr("Удалить файл ППСИ ?");
    int k=QMessageBox::question(0,tr("Удаление данных!!!"),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

    if (k == QMessageBox::No) return;

    if(cOI->RemoveFullInBasket(nz,true))
    {
        cOI->zapsist.naz_ed=naz;
        cOI->zapsist.nz_kodobr=6;
        cOI->zapsist.sost="выполнено";
        cOI->InsertSist(false);
    }
    on_btnRefresh();
}


void FFull::on_btnKonv()
{
    qlonglong nz;
    QString soob;

    QLockFile lock(QDir::temp().absoluteFilePath("AutoSbor.loc"));
    if (!lock.tryLock(100)) return;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;

    soob=QObject::tr("Выполнить конвертирование (ВРВ) по одному каналу ?");
    int k=QMessageBox::question(0,tr("Конвертирование "),soob,QMessageBox::Yes | QMessageBox::No,QMessageBox::No);

    if (k == QMessageBox::No) return;


    ui->pb->setVisible(true);
    timer->start(100);
    qApp->processEvents();
    tobr12.cOI=cOI;
    tobr12.nz=nz;
    tobr12.start(QThread::NormalPriority);
}


void FFull::ShowPb()
{
    qint32 zn;

    zn=ui->pb->value();

    zn++;
    if (zn > 100) zn=0;

    ui->pb->setValue(zn);
    qApp->processEvents();
}


void FFull::EndKonv()
{
    timer->stop();
    ui->pb->setVisible(false);
    on_btnRefresh();
}


void FFull::on_btnVigr()
{
    QString sdir;
    qlonglong nz;
    QString nfile;

    ind=cOI->getCurrentIndex(ui->tableView,m_fullppsi,0,nz);
    if (nz == 0) return;

    sdir=QFileDialog::getExistingDirectory(0,"Дирректория для выгрузки","");
    if (sdir.isEmpty()) return;


    if (!cOI->CopyEdxr("fullppsi",nz,sdir,true,nfile)) return;

    QMessageBox::information(0,"Выгрузка","Выгрузка выполнена успешно",QMessageBox::Yes);

}


QString FFull::FormFiltrNka()
{
    QString ret;

    ret.clear();
    if (ui->cbNka->currentIndex() == 0) return ret;

    if (ui->cbNka->currentIndex() == 1)
    {
        ret=" AND nka = '126' ";
        return ret;
    }

    if (ui->cbNka->currentIndex() == 2)
    {
        ret=" AND nka = '127' ";
        return ret;
    }
    return ret;
}


QString FFull::FormFiltrNpn()
{
    QString ret;

    ret.clear();
    if (!ui->cbOtbNpn->isChecked()) return ret;

    ret=" AND npn = '"+ui->cbNpn->currentText()+"' ";
    return ret;
}


void FFull::DravPPSI()
{
    PPSI->cOI = cOI;
    qlonglong nz = 0;
    QString nfile = "";
    int kan = 0;
    QString data_str = "", data_str2 = "";
    QDateTime date_time_begin;
    QDateTime date_time_end;


    cOI->getCurrentIndex( ui->tableView, m_fullppsi, 0, nz );
    nfile = m_fullppsi->data(m_fullppsi->index(ui->tableView->currentIndex().row(),1)).toString();
    kan = m_fullppsi->data(m_fullppsi->index(ui->tableView->currentIndex().row(),3)).toInt();

    data_str = m_fullppsi->data(m_fullppsi->index(ui->tableView->currentIndex().row(),5)).toString();
    date_time_begin.setDate( QDate(  data_str.mid(6,4).toInt(),data_str.mid(3,2).toInt(), data_str.mid(0,2).toInt() ) );
    date_time_begin.setTime( QTime( data_str.mid(11, 2).toInt(), data_str.mid(14, 2).toInt() ) );

    data_str2 = m_fullppsi->data(m_fullppsi->index(ui->tableView->currentIndex().row(),6)).toString();
    date_time_end.setDate( QDate(  data_str2.mid(6,4).toInt(),data_str2.mid(3,2).toInt(), data_str.mid(0,2).toInt() ) );
    date_time_end.setTime( QTime( data_str2.mid(11, 2).toInt(), data_str2.mid(14, 2).toInt() ) );

    qDebug() << "\t\t FFull::DrawPPSI";
    qDebug() << "nz = " << nz << "\t Name of file = " << nfile;
    qDebug() << "data_str = " << data_str << "\t data_str2 = " << data_str2;
    qDebug() << "date_time_begin = " << date_time_begin << "\t date_time_end = " << date_time_end << endl;
    PPSI->plot_all( nfile, nz, kan, date_time_begin, date_time_end );
    this->setVisible(false);
    PPSI->show();
}


void FFull::Report(QTableView *table, QString zag)
{
    QString strStream = "";
    QTextStream out(&strStream);
    out.setCodec("UTF-8");
    out.setLocale( QLocale("UTF-8") );

    const int rowCount = table->model()->rowCount();
    const int columnCount = table->model()->columnCount();

    out << "<html>"
                "<meta http-equiv = \"Content-Type\" Content=\"Text/html; charset=utf-8\">\n"
                "<h1> " + zag + "</h1>"
           "</html>";
    out <<  "<html>\n"
        "<head>\n"
        "<meta Content=\"Text/html; charset=UTF-8\">\n"
        <<  "</head>\n"
        "<body bgcolor=#ffffff link=#5000A0>\n"
        "<table border=1 cellspacing=0 cellpadding=2>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; ++column)
    {
        if ( !table->isColumnHidden(column) )
        {
            out << QString("<th>%1</th>").arg( table->model()->headerData(column, Qt::Horizontal).toString() );
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr>";
        for ( int column = 0; column < columnCount; ++column )
        {
            if ( !table->isColumnHidden(column) )
            {
                QString data = table->model()->data( table->model()->index(row, column) ).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg(( !data.isEmpty()) ? data : QString("&nbsp;") );
            }
        }
        out << "</tr>\n";
    }

    out <<  "</table>\n"
        "</body>\n"
        "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml( strStream );

    QPrinter printer;
    printer.setPageSize( QPrinter::A4 );
    printer.setPageMargins( QMarginsF( 15, 15, 15, 15 ) );
    printer.setOutputFileName("/home/user/new_file_ful.pdf");

    QPrintDialog *dialog = new QPrintDialog( &printer );
    if (dialog->exec() == QDialog::Accepted)
    {
        document->print( &printer );
    }

    delete document;
    delete dialog;
}
