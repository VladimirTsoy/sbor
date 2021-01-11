#include "ppsi.h"
//#include "CRC.h"

#if QT_VERSION >= 0x040000
#include <qpaintengine.h>
#endif


ppsi::ppsi(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    cOI = new ObrIzm;
    pi= 3.14159265358979323846264338327950/180;
    snMkadr = 0;
    errorCRC = 0;
    kanal = 1;
    num_empty = 0;
    cOI->ind_full = 0;

    ui.setupUi(this);

    ui.qwtPlot->setAxisTitle( QwtPlot::yLeft, "Сигнал/Шум" );
    ui.qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, 17);
    ui.qwtPlot->setAxisMaxMinor(QwtPlot::xBottom, 7);
    ui.qwtPlot->setFrameStyle(QFrame::Box | QFrame::Sunken);

    ui.qwtPlot_stat->setAxisTitle(QwtPlot::yLeft,"Вир кан");
    ui.qwtPlot_stat->setAxisMaxMajor(QwtPlot::xBottom, 17);
    ui.qwtPlot_stat->setAxisMaxMinor(QwtPlot::xBottom, 7);
    ui.qwtPlot_stat->setFrameStyle(QFrame::Box | QFrame::Sunken);

    ui.qwtPlot_dtime->setAxisTitle( QwtPlot::yLeft,"Потери" );
    ui.qwtPlot_dtime->setAxisMaxMajor(QwtPlot::xBottom, 17);
    ui.qwtPlot_dtime->setAxisMaxMinor(QwtPlot::xBottom,7);
    ui.qwtPlot_dtime->setFrameStyle(QFrame::Box | QFrame::Sunken);

    ui.qwtPlot_stat->installEventFilter( this );
    ui.qwtPlot->installEventFilter( this );
    ui.qwtPlot_dtime->installEventFilter( this );

                /// Параметры для QwtPolarPlot
    file = 0;
    ui.qwtPolarPlot->setTitle( "Прохождения" );
    ui.qwtPolarPlot->setPlotBackground( Qt::lightGray );
    ui.qwtPolarPlot->setScale( QwtPolar::Azimuth, 360, 0, -10 );
    ui.qwtPolarPlot->setAzimuthOrigin( 90*pi );
    ui.qwtPolarPlot->setScale( QwtPolar::Radius, 90, 0, -10 );

    piker = new QwtPolarPicker(QwtPolarPicker::CrossRubberBand, QwtPolarPicker::ActiveOnly, ui.qwtPolarPlot->canvas());
    piker->setRubberBandPen(QColor(Qt::yellow));
    piker->setTrackerPen(QColor(Qt::black));
    piker->setStateMachine(new QwtPickerDragPointMachine);

                /// Установка сетки
    d_grid.setPen( QPen( Qt::white ) );        // Для отрисовки прохождений
    d_grid.setAxisPen( QwtPolar::AxisAzimuth, QPen(Qt::blue) );
    d_grid.setAxisPen( QwtPolar::AxisLeft, QPen(Qt::yellow) );
    d_grid.setAxisPen( QwtPolar::AxisRight, QPen(Qt::yellow) );
    d_grid.setAxisPen( QwtPolar::AxisTop, QPen(Qt::yellow) );
    d_grid.setAxisPen( QwtPolar::AxisBottom, QPen(Qt::yellow) );
    d_grid.showAxis( QwtPolar::AxisAzimuth, true );
    d_grid.showAxis( QwtPolar::AxisLeft, true );
    d_grid.showAxis( QwtPolar::AxisRight, true );
    d_grid.showAxis( QwtPolar::AxisTop, true );
    d_grid.showAxis( QwtPolar::AxisBottom, true );
    d_grid.attach( ui.qwtPolarPlot );

    grid_stat.enableXMin( true );  // Для виртуальных каналов
    grid_stat.enableYMin( true );
    grid_stat.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid_stat.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid_stat.attach( ui.qwtPlot_stat );

    grid_SH.enableXMin( true );    // Сигнал/Шум
    grid_SH.enableYMin( true );
    grid_SH.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid_SH.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid_SH.attach( ui.qwtPlot );

    grid_les.enableXMin( true );   // Потери
    grid_les.enableYMin( true );
    grid_les.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid_les.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid_les.attach( ui.qwtPlot_dtime );

        /// Настройка легенды
    ui.qwtPolarPlot->insertLegend( &legend_polar, QwtPolarPlot::BottomLegend );

        /// Перетаскивание графиков
    pan = new QwtPolarPanner( ui.qwtPolarPlot->canvas() );
    pan->setMouseButton( Qt::RightButton );

    pan_stat = new QwtPlotPanner( ui.qwtPlot_stat->canvas() );
    pan_stat->setMouseButton( Qt::RightButton );

    pan_SH = new QwtPlotPanner( ui.qwtPlot->canvas() );
    pan_SH->setMouseButton( Qt::RightButton );

    pan_les = new QwtPlotPanner( ui.qwtPlot_dtime->canvas() );
    pan_les->setMouseButton( Qt::RightButton );

        /// Отдаление, приближение
    magni = new QwtPolarMagnifier( ui.qwtPolarPlot->canvas() );
    magni->setMouseButton( Qt::MidButton );

    magni_stat = new QwtPlotMagnifier( ui.qwtPlot_stat->canvas() );
    magni_stat->setMouseButton( Qt::MidButton );

    magni_les = new QwtPlotMagnifier( ui.qwtPlot_dtime->canvas() );
    magni_les->setMouseButton( Qt::MidButton );

    magni_SH = new QwtPlotMagnifier( ui.qwtPlot->canvas() );
    magni_SH->setMouseButton( Qt::MidButton );

    ui.qwtPolarPlot->replot();

    ui.tabWidget->setCurrentIndex(1);

    ui.progressInd->setStyleSheet( " QProgressBar { text-align: centre; color: rgb(255, 255, 0); background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(100,0,0,255), stop: 1 rgba(200,0,0,255)); border: 2px solid grey; border-radius: 5px;} \
                                    QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(0,200,255,255), stop: 1 rgba(0,0,255,255));}");

    sinxr = QByteArray::fromHex("1ACFFC1D");
    connect( ui.PuSkipBack, SIGNAL( clicked( bool ) ), this, SLOT( SkipNBack() ) );
    connect( ui.push_txt, SIGNAL( clicked(bool) ), this, SLOT( save_stat() ) );
    connect( ui.cbARU1, SIGNAL(clicked(bool)), this, SLOT(cheked_cbARU1()) );
    connect( ui.cbARU2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbARU2()) );
    connect( ui.cbSH1, SIGNAL(clicked(bool)), this, SLOT(cheked_cbSH1()) );
    connect( ui.cbSH2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbSH2()) );
    connect( ui.cbARU1_2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbARU1_2()) );
    connect( ui.cbARU2_2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbARU2_2()) );
    connect( ui.cbSH1_2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbSH1_2()) );
    connect( ui.cbSH2_2, SIGNAL(clicked(bool)), this, SLOT(cheked_cbSH2_2()) );
    connect( ui.auto_scale, SIGNAL(clicked(bool)), this, SLOT(set_auto_scale()) );

//    init_zona();
}


ppsi::~ppsi()
{
    buf1.clear();
    buf2.clear();
    VecCU.clear();

    delete magni_SH;
    delete magni_les;
    delete magni_stat;
    delete magni;
    delete pan_les;
    delete pan_SH;
    delete pan_stat;
    delete pan;
    delete legend_widget_stat;
    delete leg_label;
    delete label_polar;
    delete legend_widget_crc;
    delete legend_widget_tel;
    delete legend_widget_polar;
    delete piker;
    delete cOI;
    delete buf_consider;
    delete file;
    delete ppsi_consider;
}


void ppsi::resizeEvent( QResizeEvent *event )
{
    this->setMinimumSize( qApp->desktop()->height()/2, qApp->desktop()->height()/2 );
    event->accept();
}


bool ppsi::eventFilter( QObject *obj, QEvent *event )
{
    if( !buf1.isEmpty() || !buf2.isEmpty() )
    {
        if( event->type() == QEvent::MouseButtonDblClick && obj == ui.qwtPlot_stat )        // График виртуальных каналов
        {
            StViK.setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDrawX());
            StViK.setAxisScaleDraw(QwtPlot::yLeft, new KanScaleDrawY());
            StViK.PlotVK(ppsi1, ppsi2, Cad);
            StViK.PlotEmpty(empty_cad1, empty_cad2);
            StViK.setMinimumSize(800, 350);
            StViK.show();
        }

        if( event->type() == QEvent::MouseButtonDblClick && obj == ui.qwtPlot_dtime )       // График потерь
        {
            WidCRC.setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDrawX());
            WidCRC.setAxisScaleDraw(QwtPlot::yLeft, new KanScaleDrawY());
            WidCRC.Plot_er_CRC(&er_crc1, &er_crc2);
            WidCRC.Plot_empty_cadrs(empty_cad1, empty_cad2);
            WidCRC.setMinimumSize(800, 350);
            WidCRC.show();
        }

        if( event->type() == QEvent::MouseButtonDblClick && obj == ui.qwtPlot )     // График сигнал/шум
        {
            WidSiAr.setAxisScaleDraw( QwtPlot::xBottom, new TimeScaleDrawX() );
            WidSiAr.PlotStat(&sh1_1, &sh1_2, &aru1_1, &aru1_2);
            WidSiAr.PlotAnother(&sh2_1, &sh2_2, &aru2_1, &aru2_2);
            WidSiAr.setMinimumSize(800, 350);
            WidSiAr.show();
        }

        if( event->type() == QEvent::Close )
        {
            refresh();
            this->~ppsi();
        }
    }

    return QWidget::eventFilter(obj, event);
}


void ppsi::closeEvent(QCloseEvent *event)
{
    StViK.close();
    WidCRC.close();
    WidSiAr.close();
    event->accept();
}


void ppsi::plot_all( QString nfile, qlonglong nz, int kan, QDateTime dt_begin, QDateTime dt_end )
{
    QString label = "";
    int pos = 0;
    qlonglong nz_second = 0;
    QString zap = "";
    QSqlQuery query;
    QString nfile2 = "";
    kanal = kan;

    if( !buf1.isEmpty() || !buf2.isEmpty() )
        refresh();

                /// Считываем содержимое в буфер
    fileName = nfile;
    ui.full->setText(fileName);
    nfile2 = nfile;
    pos = nfile2.indexOf( QRegExp( "\M1B|M2G" ) );           // Определяем номер канала

    if(kanal == 1)
    {
        label = "M2G";
        nfile2.replace( pos, 3, label );
        zap = "SELECT * FROM fullppsi WHERE naz ILIKE '" + nfile2 + "'";
        nz_second = cOI->ExecQuery(query, zap, true);   // Находим номер записи файла по 2ому каналу

        cOI->ReadEdxrInBuf( "fullppsi", nz, buf1, false );
        cOI->ReadEdxrInBuf( "fullppsi", nz_second, buf2, false );
        buf_consider = &buf1;
        ppsi_consider = &ppsi1;
        init_ppsi_stat( ppsi1, buf1, QTime(0,0,0), QTime(23,59,59,999) );
        init_ppsi( ppsi2, buf2, QTime(0,0,0), QTime(23,59,59,999) );
    }

    else if(kanal == 2)
    {
        label = "M1B";
        nfile2.replace( pos, 3, label );
        zap = "SELECT * FROM fullppsi WHERE naz ILIKE '" + nfile2 + "'";
        nz_second = cOI->ExecQuery(query, zap, true);   // Находим номер записи файла по 2ому каналу

        cOI->ReadEdxrInBuf( "fullppsi", nz, buf2, false );
        cOI->ReadEdxrInBuf( "fullppsi", nz_second, buf1, false );
        buf_consider = &buf2;
        ppsi_consider = &ppsi2;
        init_ppsi_stat( ppsi2, buf2, QTime(0,0,0), QTime(23,59,59,999) );
        init_ppsi( ppsi1, buf1, QTime(0,0,0), QTime(23,59,59,999) );
    }

    else
    {
        qDebug() << "Неверный номер канала !!!";
        return;
    }

//    qDebug() << "nz = " << nz << "\t nfile = " << nfile;
//    qDebug() << "nz_second = " << nz_second << "\t nfile2 = " << nfile2 << "\t kan = " << kan;
//    qDebug() << "Size of buf1 = " << buf1.size() << "\t Size of buf2 = " << buf2.size() << "\t buf_consider = " << buf_consider->size() << endl;
//    qDebug() << "Size of ppsi1 = " << ppsi1.size() << "Size of ppsi2 = " << ppsi2.size();
//    qDebug() << "snMkadr = " << snMkadr << "\t errorCRC = " << errorCRC << "\t num_empty = " << num_empty;

    set_telemetry(ppsi1, ppsi2);
    time_start.setDate(dt_begin.date());
    time_end.setDate(dt_end.date());

//    qDebug() << "Size of aru1_1 = " << aru1_1.size() << " Size of aru1_2 = " << aru1_2.size();
//    qDebug() << "Size of aru2_1 = " << aru2_1.size() << " Size of aru2_2 = " << aru2_2.size();
//    qDebug() << "Size of sh1_1 = " << sh1_1.size() << " Size of sh1_2 = " << sh1_2.size();
//    qDebug() << "Size of sh2_1 = " << sh2_1.size() << " Size of sh2_2 = " << sh2_2.size();
//    qDebug() << "Size of stat1 = " << stat1.size() << " Size of stat2 = " << stat2.size();
//    qDebug() << "Size of er_crc1 = " << er_crc1.size() << " Size of er_crc2 = " << er_crc2.size();
//    qDebug() << "Size of empty_cad1 = " << empty_cad1.size() << " Size of empty_cad2 = " << empty_cad2.size();

    draw_stat();
    draw_telemetry();

    ui.progressInd->setMaximum( buf_consider->size() );
    ui.progressInd->setRange( 0, buf_consider->size() );
    cOI->ind_full = 0;
    snMkadr = 0;

    save_PPSI(ppsi1, 1);
    save_PPSI(ppsi2, 2);

    plot_go( nz, nfile, dt_begin.date() );            // Отрисовка прохождений
}


void ppsi::plot_go( qlonglong nz, QString nfile, QDate date )
{
    if( ppsi_consider->isEmpty() )
        return;

    VecCU.clear();
    polCur.detach();
    start_point.clear();
    polCur_start.detach();

    QSqlQuery query;
    QString str;
    QList<qlonglong> list_nz;           // Для хранения найденных номеров записи
    QStringList listing;                // Для считывания содержимого К-файла
    QwtPointPolar PoinPol;
    QString tmp;
    QStringList tmp_list;
    qreal x_ka = 0.0, y_ka = 0.0, z_ka = 0.0;         // Координаты КА
    qreal x_npn = 0.0, y_npn = 0.0, z_npn = 0.0;      // Координаты антенны
    qreal az = 0.0, um = 0.0, dist = 0.0;             // Азимут и угол места
    qreal az_gr = 0, um_gr = 0;
    qint32 gr = 0;
    qint32 min = 0;
    qreal sec = 0.0;
    QTime t(0,0,0,0);
    QDateTime t_k;
    QDateTime t_begin, t_end ;
    qint32 sec_retard = 0;              // Запаздывание в текущем году (сек)
    sec_retard = cOI->FindLeap(date);

                /// Ищем соотв. К-файл
    str = " WITH ful1 AS ( SELECT DISTINCT f.nz, f.naz, f.nka, f.dtn, f.dtk, f.godn, f.godk, f.sutn, f.sutk, f.sekn, f.sekk,    \
            ( make_timestamp( extract(year from f.dtn::timestamp)::integer, extract(month from f.dtn::timestamp)::integer, extract(day from f.dtn::timestamp)::integer, 0, 0, 0 ) + f.sekn::integer * interval '1 second' ) AS daten,   \
            ( make_timestamp( extract(year from f.dtk::timestamp)::integer, extract(month from f.dtk::timestamp)::integer, extract(day from f.dtk::timestamp)::integer, 0, 0, 0 ) + f.sekk::integer * interval '1 second' ) AS datek    \
            FROM fullppsi f ),  \
            \
            ful2 AS ( SELECT f.nz, f.naz, f.nka, f.daten, f.datek, f.godn AS old_godn, f.godk AS old_godk, f.sutn AS old_sutn, f.sutk AS old_sutk, f.sekn AS old_sekn, f.sekk AS old_sekk,  \
            ( SELECT f.daten - interval '3 hour' + interval '" + QString::number(sec_retard) +" second' ) AS newDtn,    \
            ( SELECT f.datek - interval '3 hour' + interval '" + QString::number(sec_retard) +" second' ) AS newDtk FROM ful1 f ),  \
            \
            ful3 AS ( SELECT f.nz, f.naz, f.nka, f.old_godn, f.old_godk, f.old_sutn, f.old_sutk, f.old_sekn, f.old_sekk,    \
            extract( year from newDtn ) AS newGodn, extract( doy from newDtn ) AS newSutn,  \
            ( extract( second from newDtn ) + extract( minute from newDtn ) * 60 + extract( hour from newDtn ) * 3600 ) AS newSekn, \
            extract( year from newDtk ) AS newGodk, extract( doy from newDtk ) AS newSutk,  \
            ( extract( second from newDtk ) + extract( minute from newDtk ) * 60 + extract( hour from newDtk ) * 3600 ) AS newSekk, \
            f.newDtn, f.newDtk FROM ful2 f )    \
            \
            SELECT d.nz, f.nz, f.naz, f.nka, f.newGodn, f.newGodk, f.newSutn, f.newSutk, f.newSekn, f.newSekk, f.newDtn, f.newDtk,    \
            f.old_godn, f.old_godk, f.old_sutn, f.old_sutk, f.old_sekn, f.old_sekk, \
            d.naz, d.tip, d.nka, d.godn, d.godk, d.sutn, d.sutk, d.sekn, d.sekk, d.dtn, d.dtk  FROM ful3 f    \
            JOIN dds d ON f.nka = d.nka AND f.newGodn = d.godn AND f.newGodk = d.godk AND f.newSutn = d.sutn AND f.newSutk = d.sutk AND d.tip ILIKE 'KVNO'  \
            AND f.old_godn = d.godn AND f.old_godk = d.godk AND f.old_sutn = d.sutn AND f.old_sutk = d.sutk \
            AND f.naz ILIKE '" + nfile + "' AND f.nz = " + QString::number(nz);

    list_nz = cOI->ExecQueryList( query, str, true );

    qDebug() << "\t\t ppsi::plot_go";
    if( list_nz.isEmpty() )
    {
        qDebug() << " list_nz is empty !!!";
        return;
    }


    t_begin = time_start;
    t_end = time_end;

            /// Перевод из Московского времени в время GPS
    t_begin = t_begin.addSecs( -3 * 3600 + sec_retard );
    t_end = t_end.addSecs( -3 * 3600 + sec_retard );

    x_npn = 2086228.73; y_npn = 2848676.79; z_npn = 5293975.21;
    for( size_t i=0; i<list_nz.size(); ++i  )
    {
        listing.clear();
        if( cOI->ReadEdxrInList( "dds", list_nz.at(i), listing, false ) )     // Читаем К-файл в список
        {
            for( size_t j=0; j<listing.size(); ++j )
            {
                tmp = listing.at(j);
                tmp_list = tmp.split( QRegExp("\\s"), QString::SkipEmptyParts );
                t = t.fromString( tmp_list.at(3) + ":" + tmp_list.at(4) + ":" + tmp_list.at(5), "hh:mm:s.zzzzzzz" );
                t_k.setDate( QDate( tmp_list.at(0).toInt(), tmp_list.at(1).toInt(), tmp_list.at(2).toInt() ) );
                t_k.setTime( t );

                if( t_k >= t_begin && t_k <= t_end )
                {
                    x_ka = tmp_list.at(8).toDouble();
                    y_ka = tmp_list.at(9).toDouble();
                    z_ka = tmp_list.at(10).toDouble();

                    cOI->OGZ( x_ka, y_ka, z_ka, x_npn, y_npn, z_npn, az, um, dist );
                    cOI->RadToGMS(az, gr, min, sec, az_gr );
                    cOI->RadToGMS( um, gr, min, sec, um_gr );

                    PoinPol.setAzimuth( az_gr );
                    PoinPol.setRadius( um_gr );
                    VecCU.push_back( PoinPol );

                    tmp.clear();
                    tmp_list.clear();
                }
            }
        }
    }

    draw_polar_go();
}

void ppsi::draw_polar_go()
{
    if(!VecCU.isEmpty())
        start_point.push_back(VecCU.first());

    else
        return;

    QSize icon_size = QSize(55, 15);
    int icon_space = 25;
    int symb_size = 5;
    legend_polar.setFrameStyle(QFrame::Box|QFrame::Sunken);
    pal_polar = legend_polar.palette();
    pal_polar.setColor(QPalette::Window, QColor(Qt::white));
    legend_polar.setPalette(pal_polar);
    legend_polar.setAutoFillBackground(true);
    ui.qwtPolarPlot->insertLegend(&legend_polar, QwtPolarPlot::BottomLegend);

    sym_polar.setStyle(QwtSymbol::Ellipse);
    sym_polar.setPen(QColor(Qt::green));
    sym_polar.setSize(symb_size);
    polCur.setTitle("Прохождения");
    polCur.setPen(QPen(Qt::green, symb_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    polCur.setLegendAttribute(QwtPolarCurve::LegendShowLine);
    polCur.setLegendIconSize(icon_size);
    polCur.setSymbol(&sym_polar);
    polCur.attach(ui.qwtPolarPlot);
    legend_polar.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_polar = legend_polar.legendWidget( ui.qwtPolarPlot->itemToInfo(&polCur) );
    label_polar = qobject_cast<QwtLegendLabel*>(legend_widget_polar);
    label_polar->setVisible(true);
    polCur.setData(new PolarZona( VecCU, VecCU.size() ));

    symb_size += 12;
    sym_polar_start.setStyle(QwtSymbol::Star1);
    sym_polar_start.setPen(QColor(Qt::darkGreen));
    sym_polar_start.setSize(symb_size);
    polCur_start.setTitle("Начало движения");
    polCur_start.setPen(QPen(Qt::darkGreen, symb_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) );
    polCur_start.setLegendAttribute(QwtPolarCurve::LegendShowSymbol);
    polCur_start.setLegendIconSize(icon_size);
    polCur_start.setSymbol(&sym_polar_start);
    polCur_start.attach(ui.qwtPolarPlot);
    legend_polar.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_polar = legend_polar.legendWidget( ui.qwtPolarPlot->itemToInfo(&polCur_start) );
    label_polar = qobject_cast<QwtLegendLabel*>(legend_widget_polar);
    label_polar->setVisible(true);
    polCur_start.setData(new PolarZona(start_point, start_point.size()));

    ui.qwtPolarPlot->replot();
}


void ppsi::cheked_cbARU1()
{
    if ( (ui.cbARU1->checkState() == Qt::Checked) && (!aru1_1.isEmpty()) )
        cur_aru1_1.setSamples(aru1_1);

    else
        cur_aru1_1.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbARU2()
{
    if ( (ui.cbARU2->checkState() == Qt::Checked) && (!aru1_2.isEmpty()) )
        cur_aru1_2.setSamples(aru1_2);

    else
        cur_aru1_2.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbSH1()
{
    if ( (ui.cbSH1->checkState() == Qt::Checked) && (!sh1_1.isEmpty()) )
        cur_sh1_1.setSamples(sh1_1);

    else
        cur_sh1_1.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbSH2()
{
    if ( (ui.cbSH2->checkState() == Qt::Checked) && (!sh1_2.isEmpty()) )
        cur_sh1_2.setSamples(sh1_2);

    else
        cur_sh1_2.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbARU1_2()
{
    if ( (ui.cbARU1_2->checkState() == Qt::Checked) && (!aru2_1.isEmpty()) )
        cur_aru2_1.setSamples(aru2_1);

    else
        cur_aru2_1.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbARU2_2()
{
    if ( (ui.cbARU2_2->checkState() == Qt::Checked) && (!aru2_2.isEmpty()) )
        cur_aru2_2.setSamples(aru2_2);

    else
        cur_aru2_2.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbSH1_2()
{
    if ( (ui.cbSH1_2->checkState() == Qt::Checked) && (!sh2_1.isEmpty()) )
        cur_sh2_1.setSamples(sh2_1);

    else
        cur_sh2_1.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::cheked_cbSH2_2()
{
    if ( (ui.cbSH2_2->checkState() == Qt::Checked) && (!sh2_2.isEmpty()) )
        cur_sh2_2.setSamples(sh2_2);

    else
        cur_sh2_2.setSamples(vec_empty);

    ui.qwtPlot->replot();
}


void ppsi::refresh()
{
    buf1.clear(), buf2.clear(), buf_consider->clear();
    VecCU.clear();
    snMkadr = 0;
    errorCRC = 0;
    num_empty = 0;
    cOI->ind_full = 0;
    ppsi1.clear(), ppsi2.clear(), ppsi_consider->clear();
    stat1.clear(), stat2.clear();
    aru1_1.clear(), aru1_2.clear(), aru2_1.clear(), aru2_2.clear();
    sh1_1.clear(), sh1_2.clear(), sh2_1.clear(), sh2_2.clear();
    er_crc1.clear(), er_crc2.clear();
    empty_cad1.clear(), empty_cad2.clear();
    Cad.clear();
    one_chanel.clear(), two_chanel.clear(), both_chanel.clear();
    gen_izm1.clear(), gen_izm2.clear(), gen_izm_all.clear();
    one_chanel.clear(), two_chanel.clear(), both_chanel.clear();

    clearWid();
    qDebug() << "refresh is completed !";
}


void ppsi::clearWid()
{
    ui.nVer->clear();
    ui.Indf->clear();
    ui.VirtK->clear();
    ui.VirtK_2->clear();
    ui.NKadr->clear();
    ui.uZag->clear();
    ui.CRC->clear();
    ui.CRC_2->clear();
    ui.Marker->clear();
    ui.Time->clear();
    ui.ARU1->clear();
    ui.ARU2->clear();
    ui.SSH1->clear();
    ui.SSH2->clear();
    ui.AB->clear();
    ui.BK1->clear();
    ui.BK2->clear();
    ui.SizeKadr->clear();
    ui.NKadr_2->clear();
    ui.nport->clear();
    ui.Chvirtkan->clear();
    ui.zagKadr2->clear();
    ui.NZkadrov->clear();
    ui.flagOcPak->clear();
    ui.sizeSegm->clear();
    ui.progressInd->setValue( 0 );
}


void ppsi::on_findmarker_clicked()
{   
    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    qint64 ind = 512;
    QTime tm(0,0,0);
//    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    int indKan = 0, indPort = 0, indMarker = 0;

    QApplication::setOverrideCursor( Qt::WaitCursor );

    if(cOI->ind_full < ind)
    {
        cOI->ind_full = ind;
        snMkadr = 0;
    }

    while(cOI->ind_full < buf_consider->size()-len_cad)
    {
        cOI->KadrFULL(*buf_consider, cOI->ind_full, nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2, ms);

        ui.progressInd->setValue( cOI->ind_full );
        cOI->ind_full += len_cad;
        ++snMkadr;

        indKan = ifvirtK( nvk );
        indPort = ifport();
        indMarker = ifmarker(s);

        if(indMarker != -1 && indPort != -1 && indKan != -1)
            break;
    }

    if ( cOI->ind_full > buf_consider->size()-len_cad )
    {
        ui.progressInd->setValue( buf_consider->size() );
        ui.textEdit->setText( " Вы достигли конца файла !" );
        statusBar()->showMessage( " Достигнут конец файла... ", 10000 );
        clearWid();
        return;
    }

    read_file(nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2);
    ui.kadr->setText( QString::number( snMkadr ) );
    ui.pos->setText( QString::number( cOI->ind_full ) );

    QApplication::restoreOverrideCursor();
}


void ppsi::on_up_clicked()
{
    if ( cOI->ind_full <= 512 )
    {
        ui.progressInd->setValue(0);
        ui.textEdit->setText( " Вы достигли начала файла !" );
        statusBar()->showMessage( " Достигнуто начало файла ... ", 10000 );
        clearWid();
        return;
    }

    int tail = 0;
    qint64 ind = 512;

    if(cOI->ind_full > buf_consider->size()-len_cad)
    {
        tail = (buf_consider->size() - ind) % 576;

        if(tail > 0)
            cOI->ind_full = buf_consider->size() - tail;
    }

    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    QTime tm(0,0,0);
//    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    int indKan = 0, indPort = 0, indMarker = 0;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    while(cOI->ind_full > ind)
    {
        cOI->KadrFULL(*buf_consider, cOI->ind_full, nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2, ms);

        ui.progressInd->setValue( cOI->ind_full );
        cOI->ind_full -= len_cad;
        --snMkadr;

        indKan = ifvirtK( nvk );
        indPort = ifport();
        indMarker = ifmarker(s);

        if(indMarker != -1 && indPort != -1 && indKan != -1)
            break;
    }

    read_file(nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2);
    ui.kadr->setText( QString::number( snMkadr ) );
    ui.pos->setText( QString::number( cOI->ind_full ) );

    QApplication::restoreOverrideCursor();
}


void ppsi::on_dn_clicked()
{
    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    qint64 ind = 512;
    QTime tm(0,0,0);
//    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    int indKan = 0, indPort = 0, indMarker = 0;

    if(cOI->ind_full < ind)
    {
        cOI->ind_full = ind;
        snMkadr = 0;
    }

    QApplication::setOverrideCursor( Qt::WaitCursor );

    while(cOI->ind_full < buf_consider->size()-len_cad)
    {
        cOI->KadrFULL(*buf_consider, cOI->ind_full, nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2, ms);

        ui.progressInd->setValue( cOI->ind_full );
        cOI->ind_full += len_cad;
        ++snMkadr;

        indKan = ifvirtK( nvk );
        indPort = ifport();
        indMarker = ifmarker(s);

        if(indMarker != -1 && indPort != -1 && indKan != -1)
            break;
    }

    if ( cOI->ind_full > buf_consider->size()-len_cad )
    {
        ui.progressInd->setValue( buf_consider->size() );
        ui.textEdit->setText( " Вы достигли конца файла !" );
        statusBar()->showMessage( " Достигнут конец файла... ", 10000 );
        clearWid();
        return;
    }

    read_file(nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2);
    ui.kadr->setText( QString::number( snMkadr ) );
    ui.pos->setText( QString::number( cOI->ind_full ) );
}


void ppsi::SkipNBack()
{
    qint64 ind = 512;
    if ( cOI->ind_full <= ind )
    {
        ui.progressInd->setValue(0);
        ui.textEdit->setText( " Вы достигли начала файла !" );
        statusBar()->showMessage( " Достигнуто начало файла ... ", 10000 );
        clearWid();
        return;
    }

    int tail = 0;


    if(cOI->ind_full > buf_consider->size()-len_cad)
    {
        tail = (buf_consider->size() - ind) % 576;

        if(tail > 0)
            cOI->ind_full = buf_consider->size() - tail;
    }

    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    QTime tm(0,0,0);
//    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    int count = 0, num_kad = 0;
    bool ok = false;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    num_kad = ui.skipBack->text().toInt( &ok );
    while(count < num_kad)
    {
        if(cOI->ind_full <= ind || snMkadr <= 0)
            break;

        cOI->KadrFULL(*buf_consider, cOI->ind_full, nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2, ms);

        ui.progressInd->setValue( cOI->ind_full );
        cOI->ind_full -= len_cad;
        --snMkadr;
        ++count;
    }

    read_file(nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2);
    ui.kadr->setText( QString::number( snMkadr ) );
    ui.pos->setText( QString::number( cOI->ind_full ) );

    QApplication::restoreOverrideCursor();
}


void ppsi::on_nnZap_clicked()
{
    if ( cOI->ind_full > buf_consider->size() - len_cad )
    {
        ui.progressInd->setValue( buf_consider->size() );
        ui.textEdit->setText( " Вы достигли конца файла !" );
        statusBar()->showMessage( " Достигнут конец файла... ", 10000 );
        clearWid();
        return;
    }

    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    QTime tm(0,0,0);
//    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    int count = 0, num_kad = 0;
    bool ok = false;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    num_kad = ui.nn->text().toInt( &ok );
    while(count < num_kad)
    {
        if(cOI->ind_full >= buf_consider->size()-len_cad)
            break;

        cOI->KadrFULL(*buf_consider, cOI->ind_full, nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2, ms);

        ui.progressInd->setValue( cOI->ind_full );
        cOI->ind_full += len_cad;
        ++snMkadr;
        ++count;
    }

    read_file(nvk, pr_crc, tm, s, aru1, aru2, sh1, sh2);
    ui.kadr->setText( QString::number( snMkadr ) );
    ui.pos->setText( QString::number( cOI->ind_full ) );

    QApplication::restoreOverrideCursor();
}


bool ppsi::read_file(qint32 nvk, bool pr_crc, QTime tm, bool s, qint32 aru1, qint32 aru2, qint32 sh1, qint32 sh2)
{
    if( cOI->ind_full == buf_consider->size() )
        return false;

    bool ok = false;
    QByteArray bt;
    QString bi, bu;
    QByteArray data, datai;
    ui.textEdit->clear();

                    /// Запись главного заголовка кадра
    ui.nVer->setText( QString::number( cOI->glzag.fl.nver ) );
    ui.Indf->setText( QString::number( cOI->glzag.fl.ident ) );
    ui.VirtK->setText( QString::number( nvk ) );
    ui.VirtK_2->setText( QString::number( cOI->glzag.fl.flopk ) );
    ui.NKadr->setText( QString::number( cOI->glzag.fl.chkad ) );
    ui.Chvirtkan->setText( QString::number( cOI->glzag.fl.chvirtkan ) );
    ui.zagKadr2->setText( QString::number( cOI->glzag.fl.vtzag ) );
    ui.CRC_2->setText( QString::number( cOI->glzag.fl.flsinxr ) );
    ui.CRC->setText( QString::number( pr_crc ) );
    ui.flagOcPak->setText( QString::number( cOI->glzag.fl.flocpak ) );
    ui.sizeSegm->setText( QString::number( cOI->glzag.fl.iddlseg ) );
    ui.uZag->setText( QString::number( cOI->glzag.fl.ukzag ) );

    if(s == true)
        ui.Marker->setText(" Есть ");
    else
        ui.Marker->setText(" Нет ");

    data = buf_consider->mid(cOI->ind_full,576);
    QByteArray sh = buf_consider->mid( cOI->ind_full, 6 ).toHex().toUpper();
    bu = QString( sh );
    bi = QString( "%1" ).arg( sh.mid( 0,  2 ).toInt( &ok, 16 ), 8, 2, QChar('0') ) + "  " +
         QString( "%1" ).arg( sh.mid( 2,  2 ).toInt( &ok, 16 ), 8, 2, QChar('0') ) + "  " +
         QString( "%1" ).arg( sh.mid( 4,  2 ).toInt( &ok, 16 ), 8, 2, QChar('0') ) + "  " +
         QString( "%1" ).arg( sh.mid( 6,  2 ).toInt( &ok, 16 ), 8, 2, QChar('0') ) + "  " +
         QString( "%1" ).arg( sh.mid( 8,  2 ).toInt( &ok, 16 ), 8, 2, QChar('0') ) + "  " +
         QString( "%1" ).arg( sh.mid( 10, 2 ).toInt( &ok, 16 ), 8, 2, QChar('0') );
    ui.textEdit->append( bu + "  " + bi );
    ui.textEdit->append("-------------------------------------------------------------------------");

                    /// Чтение пользовательских данных
    for ( int i=0; i<16; ++i )
    {
        bu = QString( data.mid( i * 32 + 6,  8 ).toHex() + " | " +
                      data.mid( i * 32 + 14, 8 ).toHex() + " | " +
                      data.mid( i * 32 + 22, 8 ).toHex() + " | " +
                      data.mid( i * 32 + 30, 8 ).toHex() );
        ui.textEdit->append( bu );
    }
    ui.textEdit->append("-------------------------------------------------------------------------");

    ui.Time->setText( tm.toString("hh:mm:ss.zzz") );
    ui.ARU1->setText( QString::number( aru1 ) );
    ui.ARU2->setText( QString::number( aru2 ) );
    ui.SSH1->setText( QString::number( sh1 ) );
    ui.SSH2->setText( QString::number( sh2 ) );
    ui.AB->setText( QString::number( buf_consider->mid( cOI->ind_full+536, 1 ).toHex().toUpper().toInt( &ok, 16 ) ) );
    ui.BK1->setText( QString::number( buf_consider->mid( cOI->ind_full+537, 1 ).toHex().toUpper().toInt( &ok, 16 ) ) );
    ui.BK2->setText( QString::number( buf_consider->mid( cOI->ind_full+538, 1 ).toHex().toUpper().toInt( &ok, 16 ) ) );

    bt = buf_consider->mid( cOI->ind_full+541, 2 ).toHex().toUpper();
    bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
         QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
    ui.SizeKadr->setText( QString::number( bi.toUInt(&ok, 2) ) );

    ui.NKadr_2->setText( QString::number( buf_consider->mid( cOI->ind_full+543, 1 ).toHex().toUpper().toUInt( &ok, 16 ) ) );
    ui.nport->setText( QString::number( buf_consider->mid( cOI->ind_full+574, 1 ).toHex().toUpper().toUInt( &ok, 16 ) ) );
    ui.NZkadrov->setText( QString::number( buf_consider->mid( cOI->ind_full+575, 1 ).toHex().toUpper().toUInt( &ok, 16 ) ) );

    bu = "CRC  " + QString( data.mid( 518, 2 ).toHex() ) + "   marker  " + QString( data.mid( 520, 4 ).toHex() );
    ui.textEdit->append( bu );

    bu = QString( data.mid( 528, 2 ).toHex() + " " + data.mid( 530, 2 ).toHex() + " " + data.mid( 532, 2 ).toHex() + " " + data.mid( 534, 2 ).toHex() );
    ui.textEdit->append( bu );

    for ( int i=1; i<3; i++ )
    {
        bu = QString( data.mid( i * 16 + 528, 8).toHex() + " | " + data.mid( i * 16 + 528 + 8, 8).toHex() );
        ui.textEdit->append( bu );
    }

    datai = data.mid( 524, 4 );
    sh = datai.toHex().toUpper();
    bi = QString( "%1" ).arg( sh.mid( 6, 2 ).toInt( &ok, 16 ), 8, 2, QChar( '0' ) ) +
         QString( "%1" ).arg( sh.mid( 4, 2 ).toInt( &ok, 16 ), 8, 2, QChar( '0' ) ) +
         QString( "%1" ).arg( sh.mid( 2, 2 ).toInt( &ok, 16 ), 8, 2, QChar( '0' ) ) +
         QString( "%1" ).arg( sh.mid( 0, 2 ).toInt( &ok, 16 ), 8, 2, QChar( '0' ) );
    ui.textEdit->append( bi );

    return true;
}


int ppsi::ifmarker( bool s )
{
    if( ui.skipMarmer->checkState() == Qt::Checked )
    {
        if( s == true )
            return 1;

        else
            return -1;
    }

    return 0;
}


int ppsi::ifvirtK( qint32 nvk )
{

    if( ui.cbvirtk->isChecked() )
    {
        if( nvk == ui.nvirtk->currentText().toInt() )
            return 1;

        else
            return -1;
    }

    return 0;
}


int ppsi::ifport( )
{
    bool ok;

    if( ui.cbport->checkState() == Qt::Checked )
    {
        if( buf_consider->mid( cOI->ind_full+574, 1 ).toHex().toUpper().toInt( &ok, 16 ) == ui.port->text().toInt() )
            return 1;

        else
            return -1;
    }

    return 0;
}


void ppsi::on_font_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Monospac821 BT", 12), this);
    if (ok)
    {
        ui.textEdit->setFont(font);
    }
}


void ppsi::init_ppsi(QVector<CadrePPSI> &ppsi, QByteArray buf, QTime tn, QTime tk)
{
    QByteArray buf_tmp;
    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    qint64 ind = 512;
    QTime tm(0,0,0);
    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    QByteArray bt;
    QString bi;
    CadrePPSI tmpPPSI;
    bool ok;

    while(ind<buf.length()-len_cad)
    {
        cOI->KadrFULL(buf,ind,nvk,pr_crc,tm,s,aru1,aru2,sh1,sh2,ms);
        if (tm > tk) {ind += len_cad; continue;}
        if (tm < tn) {ind += len_cad; continue;}

        if( s == false )
        {
            tmpPPSI.indSt = ind;
            tmpPPSI.T = tm;
            tmpPPSI.beaten = pr_crc;
            tmpPPSI.NvirtKan = nvk;
            tmpPPSI.em = false;
            tmpPPSI.chkad = 0;
            tmpPPSI.chvirtkan = 0;
            tmpPPSI.flocpak = 0;
            tmpPPSI.flsinxr = 0;
            tmpPPSI.iddlseg = 0;
            tmpPPSI.ident = 0;
            tmpPPSI.nver = 0;
            tmpPPSI.ukzag = 0;
            tmpPPSI.vtzag = 0;
            tmpPPSI.flopk = 0;
            tmpPPSI.ch = 0;
            tmpPPSI.identific = 0;
            tmpPPSI.status = 0;
            tmpPPSI.ARU1 = aru1;
            tmpPPSI.ARU2 = aru2;
            tmpPPSI.SiNoRa1 = sh1;
            tmpPPSI.SiNoRa2 = sh2;

            bt = buf.mid( ind+541, 2 ).toHex().toUpper();
            bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
                 QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
            tmpPPSI.FrLen = bi.toInt( &ok, 2 );

            tmpPPSI.fract = buf.mid( ind+543, 1 ).toHex().toUpper().toInt( &ok, 16 );
            tmpPPSI.NumPort = buf.mid( ind+574, 1 ).toHex().toUpper().toInt( &ok, 16 );
            tmpPPSI.RecFr = buf.mid( ind+575, 1 ).toHex().toUpper().toInt( &ok, 16 );
        }

        else
        {
            tmpPPSI.indSt = ind;
            tmpPPSI.T = tm;
            tmpPPSI.beaten = pr_crc;
            tmpPPSI.NvirtKan = nvk;
            tmpPPSI.em = true;
            tmpPPSI.chkad = cOI->glzag.fl.chkad;
            tmpPPSI.chvirtkan =  cOI->glzag.fl.chvirtkan;
            tmpPPSI.flocpak = cOI->glzag.fl.flocpak;
            tmpPPSI.flsinxr = cOI->glzag.fl.flsinxr;
            tmpPPSI.iddlseg = cOI->glzag.fl.iddlseg;
            tmpPPSI.ident = cOI->glzag.fl.ident;
            tmpPPSI.nver = cOI->glzag.fl.nver;
            tmpPPSI.ukzag = cOI->glzag.fl.ukzag;
            tmpPPSI.vtzag = cOI->glzag.fl.vtzag;
            tmpPPSI.flopk = cOI->glzag.fl.flopk;
            tmpPPSI.ch = cOI->glzag.bt.ch;
            tmpPPSI.identific = cOI->glzag.bt.ident;
            tmpPPSI.status = cOI->glzag.bt.status;

            tmpPPSI.ARU1 = aru1;
            tmpPPSI.ARU2 = aru2;
            tmpPPSI.SiNoRa1 = sh1;
            tmpPPSI.SiNoRa2 = sh2;

            bt = buf.mid( ind+541, 2 ).toHex().toUpper();
            bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
                 QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
            tmpPPSI.FrLen = bi.toInt( &ok, 2 );

            tmpPPSI.fract = buf.mid( ind+543, 1 ).toHex().toUpper().toUInt( &ok, 16 );
            tmpPPSI.NumPort = buf.mid( ind+574, 1 ).toHex().toUpper().toUInt( &ok, 16 );
            tmpPPSI.RecFr = buf.mid( ind+575, 1 ).toHex().toUpper().toUInt( &ok, 16 );

            buf_tmp.clear();
            buf_tmp = buf.mid(ind+6, 512);
            ks = qChecksum(buf_tmp, 512);
            tmpPPSI.chek_sum = ks;
        }

        ppsi.push_back(tmpPPSI);
        ind += len_cad;
    }
}


void ppsi::init_ppsi_stat(QVector<CadrePPSI> &ppsi, QByteArray buf, QTime tn, QTime tk)
{
    QByteArray buf_tmp;
    qint32 nvk = 0;
    bool pr_crc = false, s = false;
    qint64 ind = 512;
    QTime tm(0,0,0);
    qint16 ks = 0;
    qint32 aru1 = 0, aru2 = 0, sh1 = 0, sh2 = 0;
    qint32 ms = 0;
    QByteArray bt;
    QString bi;
    CadrePPSI tmpPPSI;
    bool ok;

    S1ind000 = S1ind001 = S1ind010 = S1ind011 = S1ind100 = S1ind101 = S1ind110 = S1ind111 = 0;
    S2ind000 = S2ind001 = S2ind010 = S2ind011 = S2ind100 = S2ind101 = S2ind110 = S2ind111 = 0;
    S3ind000 = S3ind001 = S3ind010 = S3ind011 = S3ind100 = S3ind101 = S3ind110 = S3ind111 = 0;

    while(ind<buf.length()-len_cad)
    {
        cOI->KadrFULL(buf,ind,nvk,pr_crc,tm,s,aru1,aru2,sh1,sh2,ms);
        if (tm > tk) {ind += len_cad; continue;}
        if (tm < tn) {ind += len_cad; continue;}

        if( s == false )
        {
            tmpPPSI.indSt = ind;
            tmpPPSI.T = tm;
            tmpPPSI.beaten = pr_crc;
            tmpPPSI.NvirtKan = nvk;
            tmpPPSI.em = false;
            tmpPPSI.chkad = 0;
            tmpPPSI.chvirtkan = 0;
            tmpPPSI.flocpak = 0;
            tmpPPSI.flsinxr = 0;
            tmpPPSI.iddlseg = 0;
            tmpPPSI.ident = 0;
            tmpPPSI.nver = 0;
            tmpPPSI.ukzag = 0;
            tmpPPSI.vtzag = 0;
            tmpPPSI.flopk = 0;
            tmpPPSI.ch = 0;
            tmpPPSI.identific = 0;
            tmpPPSI.status = 0;
            tmpPPSI.ARU1 = aru1;
            tmpPPSI.ARU2 = aru2;
            tmpPPSI.SiNoRa1 = sh1;
            tmpPPSI.SiNoRa2 = sh2;

            bt = buf.mid( ind+541, 2 ).toHex().toUpper();
            bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
                    QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
            tmpPPSI.FrLen = bi.toInt( &ok, 2 );

            tmpPPSI.fract = buf.mid( ind+543, 1 ).toHex().toUpper().toInt( &ok, 16 );
            tmpPPSI.NumPort = buf.mid( ind+574, 1 ).toHex().toUpper().toInt( &ok, 16 );
            tmpPPSI.RecFr = buf.mid( ind+575, 1 ).toHex().toUpper().toInt( &ok, 16 );

            ++num_empty;
        }

        else
        {
            tmpPPSI.indSt = ind;
            tmpPPSI.T = tm;
            tmpPPSI.beaten = pr_crc;
            tmpPPSI.NvirtKan = nvk;
            tmpPPSI.em = true;
            tmpPPSI.chkad = cOI->glzag.fl.chkad;
            tmpPPSI.chvirtkan =  cOI->glzag.fl.chvirtkan;
            tmpPPSI.flocpak = cOI->glzag.fl.flocpak;
            tmpPPSI.flsinxr = cOI->glzag.fl.flsinxr;
            tmpPPSI.iddlseg = cOI->glzag.fl.iddlseg;
            tmpPPSI.ident = cOI->glzag.fl.ident;
            tmpPPSI.nver = cOI->glzag.fl.nver;
            tmpPPSI.ukzag = cOI->glzag.fl.ukzag;
            tmpPPSI.vtzag = cOI->glzag.fl.vtzag;
            tmpPPSI.flopk = cOI->glzag.fl.flopk;
            tmpPPSI.ch = cOI->glzag.bt.ch;
            tmpPPSI.identific = cOI->glzag.bt.ident;
            tmpPPSI.status = cOI->glzag.bt.status;


            tmpPPSI.ARU1 = aru1;
            tmpPPSI.ARU2 = aru2;
            tmpPPSI.SiNoRa1 = sh1;
            tmpPPSI.SiNoRa2 = sh2;

            bt = buf.mid( ind+541, 2 ).toHex().toUpper();
            bi = QString("%1").arg( bt.mid(2,2).toInt(&ok,16), 8, 2, QChar('0') ) +
                 QString("%1").arg( bt.mid(0,2).toInt(&ok,16), 8, 2, QChar('0') );
            tmpPPSI.FrLen = bi.toInt( &ok, 2 );

            tmpPPSI.fract = buf.mid( ind+543, 1 ).toHex().toUpper().toUInt( &ok, 16 );
            tmpPPSI.NumPort = buf.mid( ind+574, 1 ).toHex().toUpper().toUInt( &ok, 16 );
            tmpPPSI.RecFr = buf.mid( ind+575, 1 ).toHex().toUpper().toUInt( &ok, 16 );

            buf_tmp.clear();
            buf_tmp = buf.mid(ind+6, 512);
            ks = qChecksum(buf_tmp, 512);
            tmpPPSI.chek_sum = ks;

            if( pr_crc )
            {
                if( tmpPPSI.NumPort == 1 )
                {
                    if     ( nvk == 0 ) { ++S1ind000; }
                    else if( nvk == 1 ) { ++S1ind001; }
                    else if( nvk == 2 ) { ++S1ind010; }
                    else if( nvk == 3 ) { ++S1ind011; }
                    else if( nvk == 4 ) { ++S1ind100; }
                    else if( nvk == 5 ) { ++S1ind101; }
                    else if( nvk == 6 ) { ++S1ind110; }
                    else if( nvk == 7 ) { ++S1ind111; }
                }

                else if( tmpPPSI.NumPort == 2 )
                {
                    if     ( nvk == 0 ) { ++S2ind000; }
                    else if( nvk == 1 ) { ++S2ind001; }
                    else if( nvk == 2 ) { ++S2ind010; }
                    else if( nvk == 3 ) { ++S2ind011; }
                    else if( nvk == 4 ) { ++S2ind100; }
                    else if( nvk == 5 ) { ++S2ind101; }
                    else if( nvk == 6 ) { ++S2ind110; }
                    else if( nvk == 7 ) { ++S2ind111; }
                }

                else
                {
                    if     ( nvk == 0 ) { ++S3ind000; }
                    else if( nvk == 1 ) { ++S3ind001; }
                    else if( nvk == 2 ) { ++S3ind010; }
                    else if( nvk == 3 ) { ++S3ind011; }
                    else if( nvk == 4 ) { ++S3ind100; }
                    else if( nvk == 5 ) { ++S3ind101; }
                    else if( nvk == 6 ) { ++S3ind110; }
                    else if( nvk == 7 ) { ++S3ind111; }
                }
            }

            else
            {
                ++errorCRC;
            }

            ++snMkadr;
        }

        ppsi.push_back(tmpPPSI);
        ind += len_cad;
    }

    ui.snMkadr->setText( QString("%1").arg( snMkadr ) );
    ui.errorCRC->setText( QString("%1").arg( errorCRC ) );

    if( !ppsi.isEmpty() )
    {
        ui.proc->setText( QString::number( errorCRC * 100.0 / snMkadr, '0', 2 ) );
    }

    else
    {
        ui.proc->setText( "100" );
    }

    ui.ind000->setText( QString("%1").arg( S1ind000 ) );
    ui.ind001->setText( QString("%1").arg( S1ind001 ) );
    ui.ind010->setText( QString("%1").arg( S1ind010 ) );
    ui.ind011->setText( QString("%1").arg( S1ind011 ) );
    ui.ind100->setText( QString("%1").arg( S1ind100 ) );
    ui.ind101->setText( QString("%1").arg( S1ind101 ) );
    ui.ind110->setText( QString("%1").arg( S1ind110 ) );
    ui.ind111->setText( QString("%1").arg( S1ind111 ) );

    ui.ind000_2->setText( QString("%1").arg( S2ind000) );
    ui.ind001_2->setText( QString("%1").arg( S2ind001) );
    ui.ind010_2->setText( QString("%1").arg( S2ind010) );
    ui.ind011_2->setText( QString("%1").arg( S2ind011) );
    ui.ind100_2->setText( QString("%1").arg( S2ind100) );
    ui.ind101_2->setText( QString("%1").arg( S2ind101) );
    ui.ind110_2->setText( QString("%1").arg( S2ind110) );
    ui.ind111_2->setText( QString("%1").arg( S2ind111) );

    ui.ind000_3->setText( QString("%1").arg( S3ind000) );
    ui.ind001_3->setText( QString("%1").arg( S3ind001) );
    ui.ind010_3->setText( QString("%1").arg( S3ind010) );
    ui.ind011_3->setText( QString("%1").arg( S3ind011) );
    ui.ind100_3->setText( QString("%1").arg( S3ind100) );
    ui.ind101_3->setText( QString("%1").arg( S3ind101) );
    ui.ind110_3->setText( QString("%1").arg( S3ind110) );
    ui.ind111_3->setText( QString("%1").arg( S3ind111) );
}


void ppsi::set_telemetry(QVector<CadrePPSI> &ppsi1, QVector<CadrePPSI> &ppsi2)
{
    /// Функция заполняет соотв. веторы для построения графиков

    QTime t(0,0,0);
    double delta_y = 0.02;
    for(QVector<CadrePPSI>::const_iterator it = ppsi1.begin(); it < ppsi1.end(); ++it)
    {
        if(!it->em)  // Пустой кадр
        {
            empty_cad1.push_back(QPointF(it->T.msecsSinceStartOfDay(), -1.0));
        }

        else
        {
            if(it->beaten)
            {
                stat1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->NvirtKan));
            }

            else
            {
                er_crc1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->NvirtKan));
            }
        }

        aru1_1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->ARU1));
        aru1_2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->ARU2));
        sh1_1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->SiNoRa1));
        sh1_2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->SiNoRa2));
        gen_izm1.push_back(QPointF(it->T.msecsSinceStartOfDay(), 9));
    }

    for(QVector<CadrePPSI>::const_iterator it = ppsi2.begin(); it < ppsi2.end(); ++it)
    {
        if(!it->em)  // Пустой кадр
        {
            empty_cad2.push_back(QPointF(it->T.msecsSinceStartOfDay(), -1.0 - delta_y));
        }

        else
        {
            if(it->beaten)
            {
                stat2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->NvirtKan - delta_y));
            }

            else
            {
                er_crc2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->NvirtKan - delta_y));
            }
        }

        aru2_1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->ARU1));
        aru2_2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->ARU2));
        sh2_1.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->SiNoRa1));
        sh2_2.push_back(QPointF(it->T.msecsSinceStartOfDay(), it->SiNoRa2));
        gen_izm2.push_back(QPointF(it->T.msecsSinceStartOfDay(), 9));
    }

    if(kanal == 1)
    {
        if(!er_crc1.isEmpty() && !stat1.isEmpty())
        {
            time_start.setTime( t.addMSecs( min(stat1.first().x(), er_crc1.first().x()) ) );
            time_end.setTime( t.addMSecs( max(stat1.last().x(), er_crc1.last().x()) ) );
        }

        else if(er_crc1.isEmpty() && !stat1.isEmpty())
        {
            time_start.setTime( t.addMSecs( stat1.first().x() ) );
            time_end.setTime( t.addMSecs( stat1.last().x() ) );
        }

        else if(!er_crc1.isEmpty() && stat1.isEmpty())
        {
            time_start.setTime( t.addMSecs( er_crc1.first().x() ) );
            time_end.setTime( t.addMSecs( er_crc1.last().x() ) );
        }

        else
        {
            time_start.setTime(t);
            time_end.setTime(t);
        }
    }

    else
    {
        if(!er_crc2.isEmpty() && !stat2.isEmpty())
        {
            time_start.setTime( t.addMSecs( min(stat2.first().x(), er_crc2.first().x()) ) );
            time_end.setTime( t.addMSecs( max(stat2.last().x(), er_crc2.last().x()) ) );
        }

        else if(er_crc2.isEmpty() && !stat2.isEmpty())
        {
            time_start.setTime( t.addMSecs( stat2.first().x() ) );
            time_end.setTime( t.addMSecs( stat2.last().x() ) );
        }

        else if(!er_crc2.isEmpty() && stat2.isEmpty())
        {
            time_start.setTime( t.addMSecs( er_crc2.first().x() ) );
            time_end.setTime( t.addMSecs( er_crc2.last().x() ) );
        }

        else
        {
            time_start.setTime(t);
            time_end.setTime(t);
        }
    }

    gen_izm_all.resize(gen_izm1.size() + gen_izm2.size());
    merge(gen_izm1.begin(), gen_izm1.end(), gen_izm2.begin(), gen_izm2.end(), gen_izm_all.begin(), ComparePoint());
}


void ppsi::draw_stat()
{
    QSize icon_size = QSize(20,10);
    int icon_space = 25;
    int symb_size = 3;
    ui.qwtPlot_stat->setAxisScaleDraw( QwtPlot::xBottom, new TimeScaleDrawX() );
    ui.qwtPlot_stat->setAxisScaleDraw( QwtPlot::yLeft, new KanScaleDrawY() );
    ui.qwtPlot_dtime->setAxisScaleDraw( QwtPlot::xBottom, new TimeScaleDrawX() );
    ui.qwtPlot_dtime->setAxisScaleDraw( QwtPlot::yLeft, new KanScaleDrawY() );

    legend_stat.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    pal_stat = legend_stat.palette();
    pal_stat.setColor(QPalette::Window, QColor(Qt::white));
    legend_stat.setPalette(pal_stat);
    legend_stat.setAutoFillBackground(true);
    ui.qwtPlot_stat->insertLegend(&legend_stat, QwtPlot::BottomLegend);

    legend_crc.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    pal_crc = legend_crc.palette();
    pal_crc.setColor(QPalette::Window, QColor(Qt::white));
    legend_crc.setPalette(pal_crc);
    legend_crc.setAutoFillBackground(true);
    ui.qwtPlot_dtime->insertLegend(&legend_crc, QwtPlot::BottomLegend);

    sym_stat1.setStyle(QwtSymbol::Star1);
    sym_stat1.setPen(QColor(Qt::darkGreen));
    sym_stat1.setSize(symb_size);
    cur_stat1.setTitle(" Вир кан 1  ");
    cur_stat1.setSymbol(&sym_stat1);
    cur_stat1.setStyle(QwtPlotCurve::NoCurve);
    cur_stat1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_stat1.setLegendIconSize(icon_size);
    cur_stat1.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_stat1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_stat1.setSamples(stat1);

    sym_stat2.setStyle( QwtSymbol::Star2 );
    sym_stat2.setPen( QColor( Qt::darkYellow ) );
    sym_stat2.setSize(symb_size);
    cur_stat2.setTitle(" Вир кан 2 ");
    cur_stat2.setSymbol(&sym_stat2);
    cur_stat2.setStyle(QwtPlotCurve::NoCurve);
    cur_stat2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_stat2.setLegendIconSize(icon_size);
    cur_stat2.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_stat2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_stat2.setSamples(stat2);

    sym_ercrc1.setStyle(QwtSymbol::XCross);
    sym_ercrc1.setPen(QColor(Qt::red));
    sym_ercrc1.setSize(symb_size);
    cur_stat_crc1.setTitle(" Ошибки CRC 1 ");
    cur_stat_crc1.setSymbol(&sym_ercrc1);
    cur_stat_crc1.setStyle(QwtPlotCurve::NoCurve);
    cur_stat_crc1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_stat_crc1.setLegendIconSize(icon_size);
    cur_stat_crc1.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_stat_crc1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_stat_crc1.setSamples(er_crc1);

    sym_ercrc2.setStyle(QwtSymbol::XCross);
    sym_ercrc2.setPen(QColor(Qt::darkCyan));
    sym_ercrc2.setSize(symb_size);
    cur_stat_crc2.setTitle(" Ошибки CRC 2 ");
    cur_stat_crc2.setSymbol(&sym_ercrc2);
    cur_stat_crc2.setStyle(QwtPlotCurve::NoCurve);
    cur_stat_crc2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_stat_crc2.setLegendIconSize(icon_size);
    cur_stat_crc2.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_stat_crc2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_stat_crc2.setSamples(er_crc2);

    sym_gen_izm.setStyle(QwtSymbol::VLine);
    sym_gen_izm.setPen(QColor(Qt::black));
    sym_gen_izm.setSize(symb_size);
    cur_gen_izm.setTitle(" Кадры ");
    cur_gen_izm.setSymbol(&sym_gen_izm);
    cur_gen_izm.setStyle(QwtPlotCurve::NoCurve);
    cur_gen_izm.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_gen_izm.setLegendIconSize(icon_size);
    cur_gen_izm.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_gen_izm) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_gen_izm.setSamples(gen_izm_all);

    draw_empty(icon_space,symb_size, icon_size);

    cur_er_crc1.setTitle( " Ошибки CRC 1 " );
    cur_er_crc1.setSymbol( &sym_ercrc1 );
    cur_er_crc1.setStyle( QwtPlotCurve::NoCurve );
    cur_er_crc1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_er_crc1.setLegendIconSize(icon_size);
    cur_er_crc1.attach( ui.qwtPlot_dtime );
    legend_crc.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_crc = legend_crc.legendWidget( ui.qwtPlot_dtime->itemToInfo(&cur_er_crc1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_crc);
    leg_label->setVisible(true);
    cur_er_crc1.setSamples(er_crc1);

    cur_er_crc2.setTitle( " Ошибки CRC 2 " );
    cur_er_crc2.setSymbol( &sym_ercrc2 );
    cur_er_crc2.setStyle( QwtPlotCurve::NoCurve );
    cur_er_crc2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_er_crc2.setLegendIconSize(icon_size);
    cur_er_crc2.attach( ui.qwtPlot_dtime );
    legend_crc.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_crc = legend_crc.legendWidget( ui.qwtPlot_dtime->itemToInfo(&cur_er_crc2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_crc);
    leg_label->setVisible(true);
    cur_er_crc2.setSamples(er_crc2);
    
    ui.qwtPlot_stat->setAxisScale(QwtPlot::yLeft, -1, 9, 1);
    ui.qwtPlot_stat->setAxisAutoScale(QwtPlot::xBottom);
    ui.qwtPlot_dtime->setAxisScale(QwtPlot::yLeft, -1, 7, 1);
    ui.qwtPlot_dtime->setAxisAutoScale(QwtPlot::xBottom);

    ui.qwtPlot_stat->replot();
    ui.qwtPlot_dtime->replot();
}


void ppsi::draw_empty(int icon_space, int symb_size, QSize icon_size)
{
    sym_empty1.setStyle(QwtSymbol::Ellipse);
    sym_empty1.setPen(QColor(Qt::green));
    sym_empty1.setColor(QColor(Qt::green));
    sym_empty1.setSize(symb_size);
    cur_empty1.setTitle(" Пустые кадры 1  ");
    cur_empty1.setSymbol(&sym_empty1);
    cur_empty1.setStyle(QwtPlotCurve::NoCurve);
    cur_empty1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty1.setLegendIconSize(icon_size);
    cur_empty1.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_empty1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_empty1.setSamples(empty_cad1);

    sym_empty2.setStyle(QwtSymbol::Ellipse);
    sym_empty2.setPen(QColor(Qt::blue));
    sym_empty2.setColor(QColor(Qt::blue));
    sym_empty2.setSize(symb_size);
    cur_empty2.setTitle(" Пустые кадры 2  ");
    cur_empty2.setSymbol(&sym_empty2);
    cur_empty2.setStyle(QwtPlotCurve::NoCurve);
    cur_empty2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty2.setLegendIconSize(icon_size);
    cur_empty2.attach(ui.qwtPlot_stat);
    legend_stat.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_stat = legend_stat.legendWidget( ui.qwtPlot_stat->itemToInfo(&cur_empty2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_stat);
    leg_label->setVisible(true);
    cur_empty2.setSamples(empty_cad2);

    cur_empty_loss1.setTitle( " Пустые кадры 1 " );
    cur_empty_loss1.setSymbol( &sym_empty1 );
    cur_empty_loss1.setStyle( QwtPlotCurve::NoCurve );
    cur_empty_loss1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty_loss1.setLegendIconSize(icon_size);
    cur_empty_loss1.attach( ui.qwtPlot_dtime );
    legend_crc.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_crc = legend_crc.legendWidget( ui.qwtPlot_dtime->itemToInfo(&cur_empty_loss1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_crc);
    leg_label->setVisible(true);
    cur_empty_loss1.setSamples(empty_cad1);

    cur_empty_loss2.setTitle( " Пустые кадры 2 " );
    cur_empty_loss2.setSymbol( &sym_empty2 );
    cur_empty_loss2.setStyle( QwtPlotCurve::NoCurve );
    cur_empty_loss2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty_loss2.setLegendIconSize(icon_size);
    cur_empty_loss2.attach( ui.qwtPlot_dtime );
    legend_crc.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_crc = legend_crc.legendWidget( ui.qwtPlot_dtime->itemToInfo(&cur_empty_loss2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_crc);
    leg_label->setVisible(true);
    cur_empty_loss2.setSamples(empty_cad2);
}


void ppsi::draw_telemetry()
{
    QSize icon_size = QSize(45,10);
    int icon_space = 25;
    legend_tel.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    pal_tel = legend_tel.palette();
    pal_tel.setColor(QPalette::Window, QColor(Qt::white));
    legend_tel.setPalette(pal_tel);
    legend_tel.setAutoFillBackground(true);
    ui.qwtPlot->insertLegend(&legend_tel, QwtPlot::BottomLegend);
    ui.qwtPlot->setAxisScaleDraw( QwtPlot::xBottom, new TimeScaleDrawX() );
    ui.qwtPlot->setAxisScaleDraw( QwtPlot::yLeft, new KanScaleDrawY() );

    cur_aru1_1.setTitle("АРУ 1, кан 1");
    cur_aru1_1.setPen(QColor(Qt::darkRed), 3, Qt::SolidLine);
    cur_aru1_1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_aru1_1.setLegendIconSize(icon_size);
    cur_aru1_1.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_aru1_1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_aru1_1.setSamples(aru1_1);

    cur_aru1_2.setTitle("АРУ 2, кан 1");
    cur_aru1_2.setPen(QColor(Qt::darkMagenta), 3, Qt::SolidLine);
    cur_aru1_2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_aru1_2.setLegendIconSize(icon_size);
    cur_aru1_2.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_aru1_2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_aru1_2.setSamples(aru1_2);

    cur_sh1_1.setTitle("С\Ш 1, кан 1");
    cur_sh1_1.setPen(QColor(Qt::darkGreen), 3, Qt::SolidLine);
    cur_sh1_1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_sh1_1.setLegendIconSize(icon_size);
    cur_sh1_1.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_sh1_1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_sh1_1.setSamples(sh1_1);

    cur_sh1_2.setTitle("С\Ш 2, кан 1");
    cur_sh1_2.setPen(QColor(Qt::darkBlue), 3, Qt::SolidLine);
    cur_sh1_2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_sh1_2.setLegendIconSize(icon_size);
    cur_sh1_2.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_sh1_2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_sh1_2.setSamples(sh1_2);

    cur_aru2_1.setTitle("АРУ 1, кан 2");
    cur_aru2_1.setPen(QColor(Qt::red), 3, Qt::DotLine);
    cur_aru2_1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_aru2_1.setLegendIconSize(icon_size);
    cur_aru2_1.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_aru2_1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_aru2_1.setSamples(aru2_1);

    cur_aru2_2.setTitle("АРУ 2, кан 2");
    cur_aru2_2.setPen(QColor(Qt::magenta), 3, Qt::DotLine);
    cur_aru2_2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_aru2_2.setLegendIconSize(icon_size);
    cur_aru2_2.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_aru2_2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_aru2_2.setSamples(aru2_2);

    cur_sh2_1.setTitle("С\Ш 1, кан 2");
    cur_sh2_1.setPen(QColor(Qt::green), 3, Qt::SolidLine);
    cur_sh2_1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_sh2_1.setLegendIconSize(icon_size);
    cur_sh2_1.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_sh2_1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_sh2_1.setSamples(sh2_1);

    cur_sh2_2.setTitle("С\Ш 2, кан 2");
    cur_sh2_2.setPen(QColor(Qt::blue), 3, Qt::SolidLine);
    cur_sh2_2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
    cur_sh2_2.setLegendIconSize(icon_size);
    cur_sh2_2.attach(ui.qwtPlot);
    legend_tel.contentsWidget()->layout()->setSpacing(icon_space);
    legend_widget_tel = legend_tel.legendWidget( ui.qwtPlot->itemToInfo(&cur_sh2_2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget_tel);
    leg_label->setVisible(true);
    cur_sh2_2.setSamples(sh2_2);

    ui.qwtPlot->replot();
}


void ppsi::save_PPSI(QVector<CadrePPSI> &cadrs_ppsi, int kan)
{    
    if( !cadrs_ppsi.isEmpty() )
    {
        QFile f0("PPSI_"+ QString::number(kan) + "_Kan0.txt");
        QFile f1("PPSI_"+ QString::number(kan) + "_Kan1.txt");
        QFile f2("PPSI_"+ QString::number(kan) + "_Kan2.txt");
        QFile f3("PPSI_"+ QString::number(kan) + "_Kan3.txt");
        QFile f4("PPSI_"+ QString::number(kan) + "_Kan4.txt");
        QFile f5("PPSI_"+ QString::number(kan) + "_Kan5.txt");
        QFile f6("PPSI_"+ QString::number(kan) + "_Kan6.txt");
        QFile f7("PPSI_"+ QString::number(kan) + "_Kan7.txt");
        QFile f8("PPSI_"+ QString::number(kan) + "_Kan_-1.txt");
        QFile f_gen("PPSI_"+ QString::number(kan) + "_Gen.txt");

        f0.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f1.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f2.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f3.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f4.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f5.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f6.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f7.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f8.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);
        f_gen.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text);

        QTextStream stream0(&f0);
        QTextStream stream1(&f1);
        QTextStream stream2(&f2);
        QTextStream stream3(&f3);
        QTextStream stream4(&f4);
        QTextStream stream5(&f5);
        QTextStream stream6(&f6);
        QTextStream stream7(&f7);
        QTextStream stream8(&f8);
        QTextStream stream_gen(&f_gen);

        QString tire = " ";
        for(int c=0; c<229; c++)
        {
            tire.push_back("-");
        }

        stream0 << tire << endl;
        Record(stream0);
        stream0 << tire << endl;

        stream1 << tire << endl;
        Record(stream1);
        stream1 << tire << endl;

        stream2 << tire << endl;
        Record(stream2);
        stream2 << tire << endl;

        stream3 << tire << endl;
        Record(stream3);
        stream3 << tire << endl;

        stream4 << tire << endl;
        Record(stream4);
        stream4 << tire << endl;

        stream5 << tire << endl;
        Record(stream5);
        stream5 << tire << endl;

        stream6 << tire << endl;
        Record(stream6);
        stream6 << tire << endl;

        stream7 << tire << endl;
        Record(stream7);
        stream7 << tire << endl;

        stream8 << tire << endl;
        Record(stream8);
        stream8 << tire << endl;

        for( int c=0; c<cadrs_ppsi.size(); c++ )
        {
            if( cadrs_ppsi[c].NvirtKan == 0 )
            {
                Record( stream0, cadrs_ppsi[c], c );
            }

            else if( cadrs_ppsi[c].NvirtKan == 1 )
            {
                Record(stream1, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == 2 )
            {
                Record(stream2, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == 3 )
            {
                Record(stream3, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == 4 )
            {
                Record(stream4, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == 5 )
            {
                Record(stream5, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == 6 )
            {
                Record(stream6, cadrs_ppsi[c], c);
            }

            else if( cadrs_ppsi[c].NvirtKan == -1 )
            {
                Record(stream8, cadrs_ppsi[c], c);
            }

            else
            {
                Record(stream7, cadrs_ppsi[c], c);
            }
        }

        f0.close();
        f1.close();
        f2.close();
        f3.close();
        f4.close();
        f5.close();
        f6.close();
        f7.close();
        f8.close();

        stream_gen << tire << endl;
        Record(stream_gen);
        stream_gen << tire << endl;

        int count = 0;
        for(QVector<CadrePPSI>::iterator it = cadrs_ppsi.begin(); it < cadrs_ppsi.end(); ++it)
        {
            Record(stream_gen, *it, count);
            ++count;
        }
    }

    else
        return;
}


void ppsi::save_stat()
{
    QString namefile0 = "stat_ppsi.txt";
    int W1 = 26, W2 = 30;                   // Ширина полей

    QFile f0(namefile0);
    f0.open( QIODevice::WriteOnly | QIODevice::Text );
//    f0.open( QIODevice::Append | QIODevice::Text );

    QTextStream PPSI(&f0);
    PPSI.setCodec("UTF8");

    if( !ppsi_consider->isEmpty() )
    {

        PPSI << " " << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left <<   "Время начала кадра" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ppsi_consider->first().T.toString( "hh:mm:ss.zzz" ) <<  qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Время окончания кадра" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ppsi_consider->last().T.toString( "hh:mm:ss.zzz" ) <<  qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left <<  "Название файла" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.full->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "С маркером" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.snMkadr->text() << qSetFieldWidth(3)  << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Ошибки CRC" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.errorCRC->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "% Ошибок CRC" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.proc->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Пустые" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind000->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 1" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind001->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 2" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 3" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind011->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 4" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind100->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 5" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind101->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Вир. кан. 6" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind110->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 1: Резерв"  << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Пустые" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind000_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 1" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind001_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 2" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 3" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind011_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 4" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind100_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 5" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind101_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Вир. кан. 6" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind110_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Порт 2: Резерв" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010_2->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Пустые" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind000_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 1" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind001_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 2" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 3" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind011_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 4" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind100_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 5" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind101_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Вир. кан. 6" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind110_3->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Другие порты: Резерв" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.ind010_3->text() << qSetFieldWidth(3) << center << "|" << endl;
    }

    else
    {
        PPSI << " " << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "Время начала кадров" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << "----------" <<  qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left <<  "Время окончания кадров" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << "----------"  <<  qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left <<  "Имя файла" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << ui.full->text() << qSetFieldWidth(3) << center << "|" << endl

             << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1)  << left << "С маркером" << qSetFieldWidth(3)  << center << "|"
             << qSetFieldWidth(W2) << left << "0" << qSetFieldWidth(3)  << center << "|" << endl;
    }

    PPSI << "\n\n";

    f0.close();
}


void ppsi::set_auto_scale()
{
    ui.qwtPlot_stat->setAxisScale(QwtPlot::yLeft, -1, 9, 1);
    ui.qwtPlot_stat->setAxisAutoScale(QwtPlot::xBottom);
    ui.qwtPlot_dtime->setAxisScale(QwtPlot::yLeft, -1, 7, 1);
    ui.qwtPlot_dtime->setAxisAutoScale(QwtPlot::xBottom);
    ui.qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
    ui.qwtPlot->setAxisAutoScale(QwtPlot::xBottom);

    ui.qwtPlot_stat->replot();
    ui.qwtPlot_dtime->replot();
    ui.qwtPlot->replot();
}


//void ppsi::init_zona()
//{
//    QVector <QwtPointPolar> Polar;
//    QwtPointPolar Plr;
//    int i;

//    double AZona[] = {0, 0, 1, 1, 4, 7, 9,12,16,18,21,23,25,28,30,32,35,38,39,42,45,51,53,55,58,60,63,65,68,80,90,100,110,120,130,130,140,140,150,160,170,170,178,178,182,182,193,193,205,205,220,230,240,248,248,251,251,260,270,280,290,290,330,330,340,340,342,342,350,350, 0};
//    double hZona[] = {0,27,27,10,15,10,16,10,15,10,15,10,14,10,14,10,12,10,13,10,13,10,12,10,12,10,12,10,12,10,11, 10, 10, 10, 10, 18, 18, 19, 19, 19, 19, 10, 10, 18, 18, 10, 10, 17, 17, 10, 10, 10, 10, 10, 20, 20, 10, 10, 10, 10, 10, 12, 12,  0,  0, 12, 12,  0,  0, 18,18};

//    for(i=0;i<71;i++)
//    {
//        Plr.setAzimuth( AZona[i] );
//        Plr.setRadius( hZona[i] );
//        Polar.append( Plr );                            // РґРѕР±Р°РІРёС‚СЊ РґР°РЅРЅС‹Рµ РІ РјР°СЃСЃРёРІ
////    }

//    curZone = new QwtPolarCurve();
//    curZone->setStyle(QwtPolarCurve::Lines);

//    curZone->setPen(QPen(Qt::red, 2));

//    curZone->setData( new PolarZona( Polar, Polar.size() ) );
//    curZone->setVisible( true );
//    curZone->attach( ui.qwtPolarPlot );
//    pcurve.append( curZone );

//    ui.qwtPolarPlot->replot();
//}


void ppsi::Record(QTextStream &out)
{
    out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(9) << center << "iter" <<  qSetFieldWidth(3) << center << "|"        // РќРѕРјРµСЂ РёС‚РµСЂР°С†РёРё
        << qSetFieldWidth(12) << center << "ind_full" <<  qSetFieldWidth(3) << center << "|"                                          // РРЅРґРµРєСЃ С„СѓР»Р»
        << qSetFieldWidth(14) << center << "Time" << qSetFieldWidth(3) << center << "|"                                               // Р’СЂРµРјСЏ РєР°РґСЂР°
        << qSetFieldWidth(6) << center << "Mark" << qSetFieldWidth(3) << center << "|"                                                  // РЎС‡РµС‚С‡РёРє РєР°РґСЂРѕРІ
        << qSetFieldWidth(5) << center << "CC" << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(5) << center << "VCC" << qSetFieldWidth(3) << center << "|"                                                 // РЎС‡РµС‚С‡РёРє РІРёСЂС‚СѓР°Р»СЊРЅС‹С… РєР°РЅР°Р»РѕРІ
        << qSetFieldWidth(6) << center << "pr_crc" << qSetFieldWidth(3) << center << "|"                                              // РџСЂРёР·РЅР°Рє CRC
        << qSetFieldWidth(7) << center << "flocpak" << qSetFieldWidth(3) << center << "|"                                             // Р¤Р»Р°Рі РѕС‡РµСЂРµРґРЅРѕСЃС‚Рё РїР°РєРµС‚РѕРІ
        << qSetFieldWidth(7) << center << "flsinxr" << qSetFieldWidth(3) << center << "|"                                             // Р¤Р»Р°Рі СЃРёРЅС…СЂРѕРЅРёР·Р°С†РёРё
        << qSetFieldWidth(7) << center << "iddlseg" << qSetFieldWidth(3) << center << "|"                                             // РРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ РґР»РёРЅС‹ СЃРµРіРјРµРЅС‚Р°
        << qSetFieldWidth(7) << center << "ukzag" << qSetFieldWidth(3) << center << "|"                                               // РЈРєР°Р·Р°С‚РµР»СЊ 1РѕРіРѕ Р·Р°РіРѕР»РѕРІРєР°
        << qSetFieldWidth(11) << center << "identific" << qSetFieldWidth(3) << center << "|"                                          // РРґРµРЅС‚РёС„РёРєР°С‚РѕСЂ РєР°РґСЂР° РїРµСЂРµРґР°С‡Рё
        << qSetFieldWidth(8) << center << "status" << qSetFieldWidth(3) << center << "|"                                              // РЎС‚Р°С‚СѓСЃ
        << qSetFieldWidth(7) << center << "bt.ch" << qSetFieldWidth(3) << center << "|"                                               // РљР°РЅР°Р» (bt.ch)
        << qSetFieldWidth(6) << center << "ARU1" << qSetFieldWidth(3) << center << "|"                                                // ARU1
        << qSetFieldWidth(6) << center << "ARU2" << qSetFieldWidth(3) << center << "|"                                                // ARU2
        << qSetFieldWidth(6) << center << "S/N1" << qSetFieldWidth(3) << center << "|"                                                // РћС‚РЅРѕС€РµРЅРёРµ СЃРёРіРЅР°Р»/С€СѓРј 1
        << qSetFieldWidth(6) << center << "S/N2" << qSetFieldWidth(3) << center << "|"                                                // РћС‚РЅРѕС€РµРЅРёРµ СЃРёРіРЅР°Р»/С€СѓРј 2
        << qSetFieldWidth(10) << center << "Cadr len" << qSetFieldWidth(3) << center << "|"                                           // Р”Р»РёРЅР° РїСЂРёРЅСЏС‚РѕРіРѕ РєР°РґСЂР°
        << qSetFieldWidth(5) << center << "fract" << qSetFieldWidth(3) << center << "|"                                               // РЎС‡РµС‚С‡РёРє РєР°РґСЂРѕРІ(РЅР°Р·РµРјРЅС‹Р№)
        << qSetFieldWidth(6) << center << "port" << qSetFieldWidth(3) << center << "|"                                                // РќРѕРјРµСЂ РїРѕС‚РѕРєР°(РїРѕСЂС‚Р°)
        << qSetFieldWidth(7) << center << "RecFr" << qSetFieldWidth(3) << center << "|"                                               // РЎС‡РµС‚С‡РёРє Р·Р°РїРёСЃР°РЅРЅС‹С… РєР°РґСЂРѕРІ
        << qSetFieldWidth(5) << center << "VKan" << qSetFieldWidth(3) << center << "|"                                                // Р’РёСЂС‚СѓР°Р»СЊРЅС‹Р№ РєР°РЅР°Р»
        << qSetFieldWidth(1) << center << endl;
}


void ppsi::Record(QTextStream &out, CadrePPSI &ppsi, int c)
{
    out << qSetFieldWidth(3)  << center << "|" << qSetFieldWidth(9) << center << c <<  qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(12) << center << ppsi.indSt <<  qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(14) << center << ppsi.T.toString("hh:mm:ss.zzz") << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(5)  << center << ppsi.em << qSetFieldWidth(3)  << center << "|"
        << qSetFieldWidth(5)  << center << ppsi.chkad << qSetFieldWidth(3)  << center << "|"
        << qSetFieldWidth(5)  << center << ppsi.chvirtkan << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.beaten << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.flocpak << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.flsinxr << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.iddlseg << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.ukzag << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(11) << center << ppsi.identific << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(8)  << center << ppsi.status << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.ch << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.ARU1 << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.ARU2 << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.SiNoRa1 << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.SiNoRa2 << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(10) << center << ppsi.FrLen << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(5)  << center << ppsi.fract << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(6)  << center << ppsi.NumPort << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(7)  << center << ppsi.RecFr << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(5)  << center << ppsi.NvirtKan << qSetFieldWidth(3) << center << "|"
        << qSetFieldWidth(1)  << center << endl;
}


void ppsi::rec_cadrs(QVector<CadrePPSI> &vec_ppsi, QStringList &lst_ppsi)
{
//    qint16 ks;
    QString str = "";
    QStringList lst_str;
    QString vec_name = "vec_ppsi.txt";
    QString lst_name = "lst_ppsi.txt";

    int W1 = 16, W2 = 7;                   // Ширина полей

    QFile vec_file(vec_name);
    QFile lst_file(lst_name);
    vec_file.open( QIODevice::WriteOnly | QIODevice::Text );
    lst_file.open( QIODevice::WriteOnly | QIODevice::Text );


    QTextStream out_vec(&vec_file);
    QTextStream out_lst(&lst_file);
    out_vec.setCodec("UTF8");
    out_lst.setCodec("UTF8");

    out_vec << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1) << center << "Time" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "Chanel" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(15) << center << "virt. chanel" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "S/Sh" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "ARU" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(7) << center << "CS" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(10) << center << "iteration" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(1) << center << endl;
    int i = 0;
    for(QVector<CadrePPSI>::const_iterator it = vec_ppsi.begin(); it<vec_ppsi.end(); ++it)
    {
        out_vec << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1) << center << it->T.toString("hh:mm:ss.zzz") << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << kanal << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(15) << center << it->NvirtKan << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << it->SiNoRa1 << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << it->ARU1 << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(7) << center << qChecksum(buf1,512) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(10) << center << i << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(1) << center << endl;
        ++i;
    }

    out_lst << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1) << center << "Time" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "Chanel" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(15) << center << "virt. chanel" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "S\Sh" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(W2) << center << "ARU" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(7) << center << "CS" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(10) << center << "iteration" << qSetFieldWidth(3) << center << "|"
            << qSetFieldWidth(1) << center << endl;
    for(size_t i=0; i<lst_ppsi.size(); ++i)
    {
        str = lst_ppsi[i];
        lst_str = str.split(';');
        out_lst << qSetFieldWidth(3) << center << "|" << qSetFieldWidth(W1) << center << lst_str.at(0) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << lst_str.at(1) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(15) << center << lst_str.at(2) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << lst_str.at(3) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(W2) << center << lst_str.at(4) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(7) << center << lst_str.at(5) << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(10) << center << i << qSetFieldWidth(3) << center << "|"
                << qSetFieldWidth(1) << center << endl;
    }

    vec_file.close();
    lst_file.close();
}


ZoomSiAr::ZoomSiAr()
{
    this->setWindowTitle("Сигнал/Шум, АРУ");
    paner = new QwtPlotPanner( this->canvas() );
    paner->setMouseButton( Qt::RightButton );

    grid.enableXMin( true );
    grid.enableYMin( true );
    grid.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid.attach( this );

    piker = new ShowTime( QwtPlot::xBottom, QwtPlot::yLeft,
                               QwtPlotPicker::CrossRubberBand,
                               QwtPicker::ActiveOnly,
                               this->canvas() );
    piker->setRubberBandPen( QColor(Qt::black) );
    piker->setTrackerPen( QColor(Qt::black) );
    piker->setStateMachine( new QwtPickerDragPointMachine );

    magni = new QwtPlotMagnifier( this->canvas() );
    magni->setMouseButton( Qt::MidButton );

    legend.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    QPalette pal = legend.palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    legend.setPalette(pal);
    legend.setAutoFillBackground(true);
    this->insertLegend(&legend,QwtPlot::TopLegend);

    this->setAxisTitle(QwtPlot::yLeft, "Сигнлал/Шум, АРУ");
    this->setAxisMaxMajor(QwtPlot::xBottom, 15);
    this->setAxisMaxMinor(QwtPlot::xBottom,8);

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setContentsMargins(25, 15, 15, 25);

    this->replot();
}


ShowTime::ShowTime(int x, int y, RubberBand rb, DisplayMode dm, QWidget* w): QwtPlotPicker(x, y, rb, dm, w)
{
    setTrackerMode( dm );
    setRubberBand( rb );
    setStateMachine( new QwtPickerTrackerMachine() );
    setTrackerPen(QColor(Qt::cyan));
    setRubberBandPen(QColor(Qt::cyan));
    setEnabled(true);
}


ShowTime::ShowTime( QWidget *canvas ): QwtPlotPicker( canvas )
{
    setTrackerMode( QwtPicker::AlwaysOn );
    setRubberBand( QwtPlotPicker::CrossRubberBand );
    setStateMachine( new QwtPickerTrackerMachine() );
    setTrackerPen(QColor(Qt::cyan));
    setRubberBandPen(QColor(Qt::cyan));
    setEnabled(true);
}


QwtText ShowTime::trackerTextF( const QPointF &pos ) const
{
    QString text;
    QTime T(0,0,0);
    T = T.addMSecs(pos.x());
    text = T.toString("hh:mm:ss.zzz");
    return QwtText(text);
}


ZoomSiAr::~ZoomSiAr()
{
    delete paner;
    delete magni;
    delete piker;
    delete leg_label;
    delete legend_widget;
}


void ZoomSiAr::PlotStat( QVector <QPointF> *SiSh1, QVector <QPointF> *SiSh2, QVector <QPointF> *ARU1, QVector <QPointF> *ARU2 )
{
    if( !SiSh1->isEmpty() )
    {
        curSiSh1.setTitle("С/Ш 1, кан 1");
        curSiSh1.setPen(QColor(Qt::darkGreen), 3, Qt::SolidLine);
        curSiSh1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curSiSh1.setLegendIconSize(QSize(35,20));
        curSiSh1.setSamples( *SiSh1 );
        curSiSh1.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curSiSh1) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !SiSh2->isEmpty() )
    {
        curSiSh2.setTitle("С/Ш 2, кан 1");
        curSiSh2.setPen(QColor(Qt::darkBlue), 3, Qt::SolidLine);
        curSiSh2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curSiSh2.setLegendIconSize(QSize(35, 20));
        curSiSh2.setSamples( *SiSh2 );
        curSiSh2.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curSiSh2) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !ARU1->isEmpty() )
    {
        curARU1.setTitle("АРУ 1, кан 1");
        curARU1.setPen(QColor(Qt::darkRed), 3, Qt::SolidLine);
        curARU1.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curARU1.setLegendIconSize(QSize(35, 20));
        curARU1.setSamples( *ARU1 );
        curARU1.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curARU1) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !ARU2->isEmpty() )
    {
        curARU2.setTitle("АРУ 2, кан 1");
        curARU2.setPen(QColor(Qt::darkMagenta), 3, Qt::SolidLine);
        curARU2.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curARU2.setLegendIconSize(QSize(35, 20));
        curARU2.setSamples( *ARU2 );
        curARU2.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curARU2) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    this->setAxisAutoScale( QwtPlot::xBottom, true );
    this->setAxisAutoScale( QwtPlot::yLeft, true );
    this->replot();
}


void ZoomSiAr::PlotAnother(QVector<QPointF> *SiSh1, QVector<QPointF> *SiSh2, QVector<QPointF> *ARU1, QVector<QPointF> *ARU2)
{
    if( !SiSh1->isEmpty() )
    {
        curSiSh1_an.setTitle("С/Ш 1, кан 2");
        curSiSh1_an.setPen(QColor(Qt::green), 4, Qt::DotLine);
        curSiSh1_an.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curSiSh1_an.setLegendIconSize(QSize(35, 20));
        curSiSh1_an.setSamples( *SiSh1 );
        curSiSh1_an.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curSiSh1_an) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !SiSh2->isEmpty() )
    {
        curSiSh2_an.setTitle("C/Ш 2, кан 2");
        curSiSh2_an.setPen(QColor(Qt::blue), 4, Qt::DotLine);
        curSiSh2_an.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curSiSh2_an.setLegendIconSize(QSize(35, 20));
        curSiSh2_an.setSamples( *SiSh2 );
        curSiSh2_an.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curSiSh2_an) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !ARU1->isEmpty() )
    {
        curARU1_an.setTitle("АРУ 1, кан 2");
        curARU1_an.setPen(QColor(Qt::red), 4, Qt::DotLine);
        curARU1_an.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curARU1_an.setLegendIconSize(QSize(35, 20));
        curARU1_an.setSamples( *ARU1 );
        curARU1_an.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend.contentsWidget()->setMinimumSize(100,150);
        legend_widget = legend.legendWidget( this->itemToInfo(&curARU1_an) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !ARU2->isEmpty() )
    {
        curARU2_an.setTitle("АРУ 2, кан 2");
        curARU2_an.setPen(QColor(Qt::magenta), 4, Qt::DotLine);
        curARU2_an.setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curARU2_an.setLegendIconSize(QSize(35, 20));
        curARU2_an.setSamples( *ARU2 );
        curARU2_an.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&curARU2_an) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    this->setAxisAutoScale( QwtPlot::xBottom, true );
    this->replot();
}


StatVirKan::StatVirKan()
{
    this->setWindowTitle("Виртуальные каналы");
    paner = new QwtPlotPanner( this->canvas() );
    paner->setMouseButton( Qt::RightButton );

    grid.enableXMin( true );
    grid.enableYMin( true );
    grid.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid.attach( this );

    piker = new ShowTime( QwtPlot::xBottom, QwtPlot::yLeft,
                          QwtPlotPicker::CrossRubberBand,
                          QwtPicker::ActiveOnly,
                          this->canvas() );
    piker->setRubberBandPen( QColor(Qt::darkGray) );
    piker->setTrackerPen( QColor(Qt::darkGray) );
    piker->setStateMachine( new QwtPickerDragPointMachine );

    magni = new QwtPlotMagnifier( this->canvas() );
    magni->setMouseButton( Qt::MidButton );

    legend.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    QPalette pal = legend.palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    legend.setPalette(pal);
    legend.setAutoFillBackground(true);
    this->insertLegend(&legend,QwtPlot::TopLegend);

    this->setAxisTitle(QwtPlot::yLeft, "Виртуальные каналы");
    this->setAxisMaxMajor(QwtPlot::xBottom, 17);
    this->setAxisMaxMinor(QwtPlot::xBottom,8);
    this->setAxisMaxMajor(QwtPlot::yLeft, 12);
    this->setAxisMaxMinor(QwtPlot::yLeft, 10);

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setContentsMargins(25, 15, 15, 25);

    this->replot();
}


StatVirKan::~StatVirKan()
{
    delete paner;
    delete magni;
    delete piker;
    delete legend_widget;
    delete leg_label;
}


void StatVirKan::PlotVK(QVector <QPointF> *Stat, QVector <QPointF> *Stat2 , QVector<QPointF> *ErCRC, QVector<QPointF> *ErCRC2)
{
    if( !Stat->isEmpty() )
    {
        this->setAxisScale( QwtPlot::xBottom, Stat->first().x(), Stat->last().x() );
        sym.setStyle( QwtSymbol::Star1 );
        sym.setPen( QColor(Qt::darkGreen ) );
        sym.setSize( 5 );
        cur.setSymbol( &sym );
        cur.setPen( QColor( Qt::blue ) );
        cur.setStyle( QwtPlotCurve::NoCurve );
        cur.setSamples( *Stat );
        cur.attach( this );

        sym2.setStyle( QwtSymbol::XCross );
        sym2.setPen( QColor(Qt::darkYellow) );
        sym2.setSize(7);
        cur2.setSymbol( &sym2 );
        cur2.setStyle( QwtPlotCurve::NoCurve );
        cur2.setSamples( *Stat2 );
        cur2.attach( this );
    }

    else
    {
        this->setAxisScale( QwtPlot::xBottom, ErCRC->first().x(), ErCRC->last().x() );
    }

    if( !ErCRC->isEmpty() )
    {
        sym3.setStyle( QwtSymbol::Cross );
        sym3.setPen( QColor(Qt::red) );
        sym3.setSize( 5 );
        crc.setSymbol( &sym3 );
        crc.setPen( QColor(Qt::red) );
        crc.setStyle( QwtPlotCurve::NoCurve );
        crc.setSamples( *ErCRC );
        crc.attach( this );
    }

    if( !ErCRC2->isEmpty() )
    {
        sym4.setStyle( QwtSymbol::Cross );
        sym4.setPen( QColor(Qt::darkCyan) );
        sym4.setSize( 5 );
        crc2.setSymbol( &sym4 );
        crc2.setStyle( QwtPlotCurve::NoCurve );
        crc2.setSamples( *ErCRC2 );
        crc2.attach( this );
    }

    this->setAxisAutoScale( QwtPlot::yLeft, true );
    this->replot();
}


void StatVirKan::PlotVK(QVector<CadrePPSI> &ppsi1, QVector<CadrePPSI> &ppsi2, QVector<Cadrs> &cad)
{
    chanel1.clear(), chanel2.clear(), chanel_1and2.clear(), chanel_er.clear();
    chanel_some_gen.clear(), chanel_both_gen.clear(), empty_gen.clear();
    st_ful.Collation3(ppsi1, ppsi2, cad);

    if(cad.isEmpty())
        return;

    QSize sz = QSize(35, 20);
    int space = 30;
    int sz_symb = 6;

    init_chanel(cad);

    cur_ch1.setTitle(" По 1ому каналу ");
    sm_ch1.setStyle(QwtSymbol::XCross);
    sm_ch1.setPen(QColor(Qt::black));
    sm_ch1.setSize(sz_symb);
    cur_ch1.setSymbol(&sm_ch1);
    cur_ch1.setStyle(QwtPlotCurve::NoCurve);
    cur_ch1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_ch1.setLegendIconSize(sz);
    cur_ch1.setSamples(chanel1);
    cur_ch1.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_ch1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);

    cur_ch2.setTitle(" По 2ому каналу ");
    sm_ch2.setStyle(QwtSymbol::XCross);
    sm_ch2.setPen(QColor(Qt::blue));
    sm_ch2.setSize(sz_symb);
    cur_ch2.setSymbol(&sm_ch2);
    cur_ch2.setStyle(QwtPlotCurve::NoCurve);
    cur_ch2.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    cur_ch2.setLegendIconSize(sz);
    cur_ch2.setSamples(chanel2);
    cur_ch2.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_ch2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);

    cur_ch_1and2.setTitle(" По обоим каналам ");
    sm_ch_1and2.setStyle(QwtSymbol::Diamond);
    sm_ch_1and2.setPen(QColor(Qt::green));
    sm_ch_1and2.setSize(sz_symb);
    cur_ch_1and2.setSymbol(&sm_ch_1and2);
    cur_ch_1and2.setStyle(QwtPlotCurve::NoCurve);
    cur_ch_1and2.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    cur_ch_1and2.setLegendIconSize(sz);
    cur_ch_1and2.setSamples(chanel_1and2);
    cur_ch_1and2.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_ch_1and2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);

    cur_ch_er.setTitle(" Битые каналы ");
    sm_ch_er.setStyle(QwtSymbol::Ellipse);
    sm_ch_er.setPen(QColor(Qt::red));
    sm_ch_er.setSize(sz_symb);
    cur_ch_er.setSymbol(&sm_ch_er);
    cur_ch_er.setStyle(QwtPlotCurve::NoCurve);
    cur_ch_er.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    cur_ch_er.setLegendIconSize(sz);
    cur_ch_er.setSamples(chanel_er);
    cur_ch_er.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_ch_er) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);

    cur_both_gen.setTitle(" Оба канала ");
    sm_both_gen.setStyle(QwtSymbol::VLine);
    sm_both_gen.setPen(QColor(Qt::magenta));
    sm_both_gen.setSize(sz_symb);
    cur_both_gen.setSymbol(&sm_both_gen);
    cur_both_gen.setStyle(QwtPlotCurve::NoCurve);
    cur_both_gen.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    cur_both_gen.setLegendIconSize(sz);
    cur_both_gen.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_both_gen) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_both_gen.setSamples(chanel_both_gen);

    cur_some_gen.setTitle(" По одному каналу ");
    sm_some_gen.setStyle(QwtSymbol::HLine);
    sm_some_gen.setPen(QColor(Qt::darkMagenta));
    sm_some_gen.setSize(sz_symb);
    cur_some_gen.setSymbol(&sm_some_gen);
    cur_some_gen.setStyle(QwtPlotCurve::NoCurve);
    cur_some_gen.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
    cur_some_gen.setLegendIconSize(sz);
    cur_some_gen.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_some_gen) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_some_gen.setSamples(chanel_some_gen);

    this->setAxisAutoScale( QwtPlot::yLeft, true );
    this->setAxisAutoScale( QwtPlot::xBottom, true );
    this->replot();
}


void StatVirKan::PlotEmpty(QVector<QPointF> &empty_cad1, QVector<QPointF> &empty_cad2)
{
    QSize sz = QSize(35, 20);
    int space = 30;
    int sz_symb = 6;
    QVector <QPointF> vec_merge(empty_cad1.size() + empty_cad2.size());
    QVector <QPointF>::iterator start_uniqe;

    merge(empty_cad1.begin(), empty_cad1.end(), empty_cad2.begin(), empty_cad2.end(), vec_merge.begin(), ComparePoint());

    start_uniqe = unique(vec_merge.begin(), vec_merge.end(), EqualPoint());

    QPointF point(0.0, 0.0);
    for(QVector<QPointF>::const_iterator it=vec_merge.begin(); it<start_uniqe; ++it)
    {
        point.setX(it->x());
        point.setY(9);
        empty_gen.push_back(point);
    }

    cur_empty1.setTitle(" Пустые каналы 1 ");
    sm_empty_cad1.setStyle(QwtSymbol::Ellipse);
    sm_empty_cad1.setPen(QColor(Qt::yellow));
    sm_empty_cad1.setSize(sz_symb);
    cur_empty1.setSymbol(&sm_empty_cad1);
    cur_empty1.setStyle(QwtPlotCurve::NoCurve);
    cur_empty1.setPen(QColor(Qt::white));
    cur_empty1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty1.setLegendIconSize(sz);
    cur_empty1.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_empty1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_empty1.setSamples(empty_cad1);

    cur_empty2.setTitle(" Пустые каналы 2 ");
    sm_empty_cad2.setStyle(QwtSymbol::Ellipse);
    sm_empty_cad2.setPen(QColor(Qt::darkYellow));
    sm_empty_cad2.setSize(sz_symb);
    cur_empty2.setSymbol(&sm_empty_cad2);
    cur_empty2.setStyle(QwtPlotCurve::NoCurve);
    cur_empty2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty2.setLegendIconSize(sz);
    cur_empty2.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_empty2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_empty2.setSamples(empty_cad2);

    cur_empty_gen.setTitle(" Пустые ");
    sm_empty_gen.setStyle(QwtSymbol::Ellipse);
    sm_empty_gen.setPen(QColor(Qt::darkGreen));
    sm_empty_gen.setSize(sz_symb);
    cur_empty_gen.setSymbol(&sm_empty_gen);
    cur_empty_gen.setStyle(QwtPlotCurve::NoCurve);
    cur_empty_gen.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty_gen.setLegendIconSize(sz);
    cur_empty_gen.attach(this);
    legend.contentsWidget()->layout()->setSpacing(space);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_empty_gen) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_empty_gen.setSamples(empty_gen);

    this->setAxisAutoScale( QwtPlot::yLeft, true );
    this->setAxisAutoScale( QwtPlot::xBottom, true );
    this->replot();
}


void StatVirKan::init_chanel(QVector<Cadrs> &cad)
{
    QPointF point;
    double delta_y = 0.02;
    for( QVector<Cadrs>::const_iterator iter = cad.begin(); iter<cad.end(); ++iter )
    {
        if(iter->cad1 == 1 && iter->cad2 == 1)
        {
            point.setX( (iter->T1.msecsSinceStartOfDay() + iter->T2.msecsSinceStartOfDay())/2.0 );
            point.setY( iter->NVirtKan1 );
            chanel_1and2.push_back( point );
            chanel_both_gen.push_back(QPointF(point.x(), 9));
        }

        else if(iter->cad1 ==1 && iter->cad2 == 0)
        {
            point.setX(iter->T1.msecsSinceStartOfDay());
            point.setY(iter->NVirtKan1 + delta_y);
            chanel1.push_back(point);
            chanel_some_gen.push_back(QPointF(point.x(), 9));
        }

        else if(iter->cad1 == 0 && iter->cad2 == 1)
        {
            point.setX(iter->T2.msecsSinceStartOfDay());
            point.setY(iter->NVirtKan2 - delta_y);
            chanel2.push_back(point);
            chanel_some_gen.push_back(QPointF(point.x(), 9));
        }

        else
        {
            point.setX((iter->T1.msecsSinceStartOfDay() + iter->T2.msecsSinceStartOfDay())/2.0);
            point.setY(iter->NVirtKan1);
            chanel_er.push_back(point);
            chanel_some_gen.push_back(QPointF(point.x(), 9));
        }
    }
}


WidErCRC::WidErCRC()
{
    this->setWindowTitle("Потери");
    paner = new QwtPlotPanner( this->canvas() );
    paner->setMouseButton( Qt::RightButton );

    grid.enableXMin( true );
    grid.enableYMin( true );
    grid.setMajorPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid.setMinorPen( QPen( Qt::gray,  0, Qt::DotLine ) );
    grid.attach( this );

    magni = new QwtPlotMagnifier( this->canvas() );
    magni->setMouseButton( Qt::MidButton );

    piker = new ShowTime( QwtPlot::xBottom, QwtPlot::yLeft,
                          QwtPlotPicker::CrossRubberBand,
                          QwtPicker::ActiveOnly,
                          this->canvas() );
    piker->setRubberBandPen( QColor(Qt::black) );
    piker->setTrackerPen( QColor(Qt::black) );
    piker->setStateMachine( new QwtPickerDragPointMachine );

    legend.setFrameStyle(QFrame::Box|QFrame::Sunken);   // Настройка легенды
    QPalette pal = legend.palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    legend.setPalette(pal);
    legend.setAutoFillBackground(true);
    this->insertLegend(&legend,QwtPlot::TopLegend);

    this->setAxisTitle(QwtPlot::yLeft, "Потери");
    this->setAxisMaxMajor(QwtPlot::xBottom, 17);
    this->setAxisMaxMinor(QwtPlot::xBottom, 8);
    this->setAxisMaxMajor(QwtPlot::yLeft, 11);
    this->setAxisMaxMinor(QwtPlot::yLeft, 8);

    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setContentsMargins(25, 15, 15, 25);

    this->replot();
}


WidErCRC::~WidErCRC()
{
    delete paner;
    delete magni;
}


void WidErCRC::Plot_er_CRC(QVector<QPointF> *erorCRC1, QVector<QPointF> *erorCRC2)
{
    if( !erorCRC1->isEmpty() )
    {
        sym.setStyle( QwtSymbol::Star1 );
        sym.setPen( QColor(Qt::red ) );
        sym.setSize( 5 );
        crc.setTitle("По 1ому каналу");
        crc.setSymbol( &sym );
        crc.setStyle( QwtPlotCurve::NoCurve );
        crc.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
        crc.setLegendIconSize(QSize(35, 20));
        crc.setSamples( *erorCRC1 );
        crc.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&crc) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }

    if( !erorCRC2->isEmpty() )
    {
        sym2.setStyle( QwtSymbol::Star2 );
        sym2.setPen( QColor(Qt::darkCyan ) );
        sym2.setSize( 5 );
        crc2.setTitle("По 2ому каналу");
        crc2.setSymbol( &sym2 );
        crc2.setStyle( QwtPlotCurve::NoCurve );
        crc2.setLegendAttribute(QwtPlotCurve::LegendShowSymbol);
        crc2.setLegendIconSize(QSize(35, 20));
        crc2.setSamples( *erorCRC2 );
        crc2.attach( this );
        legend.contentsWidget()->layout()->setSpacing(30);
        legend_widget = legend.legendWidget( this->itemToInfo(&crc2) );
        leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
        leg_label->setVisible(true);
    }
}


void WidErCRC::Plot_empty_cadrs(QVector<QPointF> &empty_cad1, QVector<QPointF> &empty_cad2)
{
    sym_empty1.setStyle(QwtSymbol::Ellipse);
    sym_empty1.setPen(QColor(Qt::green));
    sym_empty1.setSize(5);
    cur_empty1.setTitle( " Пустые кадры 1 " );
    cur_empty1.setSymbol( &sym_empty1 );
    cur_empty1.setStyle( QwtPlotCurve::NoCurve );
    cur_empty1.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty1.setLegendIconSize(QSize(35, 20));
    cur_empty1.attach( this );
    legend.contentsWidget()->layout()->setSpacing(30);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_empty1) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_empty1.setSamples(empty_cad1);

    sym_empty2.setStyle(QwtSymbol::Ellipse);
    sym_empty2.setPen(QColor(Qt::blue));
    sym_empty2.setSize(5);
    cur_empty2.setTitle( " Пустые кадры 2 " );
    cur_empty2.setSymbol( &sym_empty2 );
    cur_empty2.setStyle( QwtPlotCurve::NoCurve );
    cur_empty2.setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
    cur_empty2.setLegendIconSize(QSize(35, 20));
    cur_empty2.attach(this);
    legend.contentsWidget()->layout()->setSpacing(30);
    legend_widget = legend.legendWidget( this->itemToInfo(&cur_empty2) );
    leg_label = qobject_cast<QwtLegendLabel*>(legend_widget);
    leg_label->setVisible(true);
    cur_empty2.setSamples(empty_cad2);

    this->setAxisAutoScale( QwtPlot::yLeft, true );
    this->setAxisAutoScale( QwtPlot::xBottom, true );
    this->replot();
}
