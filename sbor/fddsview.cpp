#include "fddsview.h"
#include "ui_fddsview.h"
#include "fddsview.h"
#include "ui_fddsview.h"

FDdsView::FDdsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FDdsView)
{
    ui->setupUi(this);
    deltaT = 11;                                                            // Промежуток измерений
    deltaTepoh = 10;                                                        // Промежуток измерений между эпохами
    addPlot();
    addPlotGrid();
    enableMagnifier();
    enableMovinOnPlot();
    enablePicker();
}

void FDdsView::addPlot()
{
    QPalette pal = ui->p_plot->palette();

    /// Параметры осей координат
    ui->p_plot->setAxisTitle( QwtPlot::yLeft, " Спутники " );
    ui->p_plot->setAxisTitle( QwtPlot::xBottom, "Время, ЧЧ:ММ:СС" );
    ui->p_plot->canvas()->setCursor(Qt::ArrowCursor);                       // Назначение курсора для канвы
    legend = new QwtLegend;
    ui->p_plot->insertLegend( legend, QwtPlot::RightLegend );               // Вставка легенды

    /// Установка цвета фона plot
    pal.setColor(QPalette::Window, QColor(196,196,220));
    ui->p_plot->setPalette(pal);
    ui->p_plot->setAutoFillBackground(true);                                // Установление фона

    ui->p_plot->setFrameStyle(QFrame::Box | QFrame::Sunken);                // Обрамление рамкой

    ui->p_plot->setContentsMargins( 20, 20, 100, 20 );                      // Расстояние до поля между меткой оси и заголовком
    ui->p_plot->setGeometry( 0, 50,this->width(), this->height()- 60 );

    QwtPlotCanvas * canvas = new QwtPlotCanvas(ui->p_plot);
    canvas->setPalette(Qt::white);
    canvas->setFrameStyle(QFrame::Box | QFrame::Plain );
    canvas->setLineWidth(1);
    ui->p_plot->setCanvas(canvas);
}

void FDdsView::addPlotGrid()
{
    grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajorPen(QPen(Qt::black,0,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,0,Qt::DotLine));
    grid->attach( ui->p_plot );
    grid->attach(ui->p_plot);
}

void FDdsView::enableMagnifier()
{
    /// Приближение и отдаление графика, qwt_plot_magnifier.h
    magnifier = new QwtPlotMagnifier( ui->p_plot->canvas() );

    /// Клавиша, активирующее приближение или отдаление
    magnifier->setMouseButton( Qt::MidButton );
}

void FDdsView::enableMovinOnPlot()
{
    /// Перемещение по графику, qwt_plot_panner.h
    p_panner = new QwtPlotPanner( ui->p_plot->canvas() );

    /// Клавиша, активирующая перемещение
    p_panner->setMouseButton( Qt::RightButton );
//    ui->p_plot->replot();
}

void FDdsView::enablePicker()
{
    /// Включить отображение координат курсора и двух перпендикулярных
    /// линий в месте его отображения, qwt_plot_picker.h
    /// Настройка функций
//    p_picker = new MyQwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,     // Ассоциация с осями
//                                  QwtPlotPicker::CrossRubberBand,       // Стиль перепендикулярных линий
//                                  QwtPicker::ActiveOnly,                // Включение/выключение
//                                  ui->p_plot->canvas() );               // Ассоциация с полем


    p_picker = new MyQwtPlotPicker( ui->p_plot->canvas() );     // Ассоциация с полем

    p_picker->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);        // Ассоциация с осями
    p_picker->setRubberBand(QwtPlotPicker::CrossRubberBand);    // Стиль перепендикулярных линий
//    p_picker->setTrackerMode(QwtPicker::AlwaysOn);            // Включение/выключение

    /// Цвет перпендикулярных линий
//    p_picker->setRubberBandPen( QColor(Qt::red) );

    /// Цвет координат положения указателя
//    p_picker->setTrackerPen( QColor(Qt::black) );

    /// непосредственное включение вышеописанных функций
    p_picker->setStateMachine( new QwtPickerDragPointMachine);
}

void FDdsView::ReadInList(QStringList input_line, QStringList list_mix)
{
    if (input_line.isEmpty())
    {
        qDebug() << "line is empty !";
        return;
    }

    QVector <QDate> DateKA;         // Временное хранение даты
    QVector <QTime> TimeKA;         // Временное хранение времени
    QPolygonF tmpSkip;              // Временное хранение пропусков
    QVector <QPolygonF> tmpSkip1D;  //
    QVector <QPointF> AllPoints;
    QVector <int> numFreq;          // Количество частот в каждом измерении
    float DT = 0;
    int num_freq;                   // Количество частот
    QString tempStr,tmpStr;
    QDate tempdt;
    QTime tempT;
    int posA = 0;                   // Номер позиции начала искомого символа в строке
    int posB = 0;                   // Номер позиции конца искомого символа в строке
    int count = 0;                  // Счетчик для подсчета кол-ва измерений для одного КА
    int gps = 0;                    // Количество gps
    int glonass = 0;                // Количество glonass
    int numberSatel = 0;            // Порядковый номер спутника
    QString GR = "G";               // Gps или Glonass
    int GenGps = 0;
    int GenGlonas = 0;

    /// Разбор строки и заполнение соответствующих массивов
    tempStr.clear();
    QRegExp reg(";|\b");
    posB = reg.indexIn( input_line[0], posA );
    tempStr = input_line[0].mid(posA, posB-posA);
    tmpStr = tempStr;

    if( !tempStr.isEmpty() )
        gps++;

    NameSatel.push_back( tempStr );

    skip = 0;
    int i = 0;
    while ( i<input_line.size() )
    {
        tempT.setHMS(0,0,0);
        tempdt.setDate(1753,1,1);
        posA = 0;
        posB = 0;

        tempStr.clear();
        reg.setPattern(";|\b");
        posB = reg.indexIn( input_line[i], posA );
        tempStr = input_line[i].mid(posA, posB-posA);
        GR = input_line[i].at(0);

        if ( tmpStr == tempStr )
        {
            numberSatel = input_line[i].mid(1, posB-posA-1).toInt();

            tempStr.clear();
            reg.setPattern("\\s[0-9]");
            posA = posB;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA+1, posB-posA);
            tempdt = tempdt.addYears( tempStr.toInt() - tempdt.year() );    // Установка даты

            tempStr.clear();
            reg.setPattern("\\s[0-9]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempdt = tempdt.addMonths(tempStr.toInt() - tempdt.month());    // Установка месяца

            tempStr.clear();
            reg.setPattern("\\s[0-9]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempdt = tempdt.addDays(tempStr.toInt() - tempdt.day());    // Установка дня

            tempStr.clear();
            reg.setPattern("\\s[0-9]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempT = tempT.addSecs( tempStr.toInt()*3600 );              // Установка часов

            tempStr.clear();
            reg.setPattern("\\s[0-9]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempT = tempT.addSecs( tempStr.toInt()*60 );            // Установка минут

            tempStr.clear();
            reg.setPattern("[.]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempT = tempT.addSecs( tempStr.toInt());                // Установка секунд

            tempStr.clear();
            reg.setPattern("[;]| ");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA, posB-posA);
            tempT = tempT.addMSecs(tempStr.toInt());                // Установка милисекунд

            tempStr.clear();
            reg.setPattern("[0-9]");
            posA = posB+1;
            posB = reg.indexIn( input_line[i], posA );
            tempStr = input_line[i].mid(posA);
            num_freq = tempStr.toInt();
            numFreq.push_back( num_freq );                          // Установка количества частот

            DateKA.push_back( tempdt );
            TimeKA.push_back( tempT );

            if( GR == "R" )
            {
                GenGlonas = numberSatel;
                AllPoints.push_back( QPointF(tempT.hour()*3600 + tempT.minute()*60 + tempT.second(), GenGps+numberSatel) );
            }

            if( GR == "G" )
            {
                GenGps = numberSatel;
                AllPoints.push_back( QPointF(tempT.hour()*3600 + tempT.minute()*60 + tempT.second(), numberSatel) );
            }

            if ( i == input_line.size()-1 )
            {
                DT = qAbs(  TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second() - TimeKA[count-1].hour()*3600 - TimeKA[count-1].minute()*60 - TimeKA[count-1].second() );
                if( DT > deltaT )
                {
                    skip++;
                    if( GR == 'R' )
                    {
                        tmpSkip << QPointF( TimeKA[count-1].hour()*3600 + TimeKA[count-1].minute()*60 + TimeKA[count-1].second(), GenGps + numberSatel )
                                << QPointF( TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second(),       GenGps + numberSatel );
                        tmpSkip1D.push_back( tmpSkip );
                    }
                    else if( GR == 'G' )
                    {
                        tmpSkip << QPointF( TimeKA[count-1].hour()*3600 + TimeKA[count-1].minute()*60 + TimeKA[count-1].second(), numberSatel )
                                << QPointF( TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second(),       numberSatel );
                        tmpSkip1D.push_back( tmpSkip );
                    }
                    tmpSkip.clear();
                }

                NameSatel.push_back( tempStr );
                timeKA2D.push_back( TimeKA );
                dateKA2D.push_back( DateKA );
                AllPoints2D.push_back( AllPoints );
                numFreq2D.push_back( numFreq );
                Skip2D.push_back( tmpSkip1D );

                DateKA.clear();
                TimeKA.clear();
                AllPoints.clear();
                numFreq.clear();
                tmpSkip1D.clear();
            }

            if( count > 0 &&  count < TimeKA.size() )
            {
                DT = qAbs(  TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second() - TimeKA[count-1].hour()*3600 - TimeKA[count-1].minute()*60 - TimeKA[count-1].second() );

                if( DT > deltaT )
                {
                    skip++;
                    if( GR == 'R' )
                    {
                        tmpSkip << QPointF( TimeKA[count-1].hour()*3600 + TimeKA[count-1].minute()*60 + TimeKA[count-1].second(), GenGps + numberSatel )
                                << QPointF( TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second(),       GenGps + numberSatel );
                        tmpSkip1D.push_back( tmpSkip );
                    }

                    else if( GR == 'G' )
                    {
                        tmpSkip << QPointF( TimeKA[count-1].hour()*3600 + TimeKA[count-1].minute()*60 + TimeKA[count-1].second(), numberSatel )
                                << QPointF( TimeKA[count].hour()*3600 + TimeKA[count].minute()*60 + TimeKA[count].second(),       numberSatel );
                        tmpSkip1D.push_back( tmpSkip );
                    }
                    tmpSkip.clear();
                }
            }
            count++;
            i++;
        }

        else
        {
            if( tempStr.at(0) == 'R' )
            {
                glonass++;
            }

            else if( tempStr.at(0) == 'G' )
            {
                gps++;
            }

            NameSatel.push_back( tempStr );
            timeKA2D.push_back( TimeKA );
            dateKA2D.push_back( DateKA );
            AllPoints2D.push_back( AllPoints );
            numFreq2D.push_back( numFreq );
            Skip2D.push_back( tmpSkip1D );
            tmpStr = tempStr;
            count = 0;

            DateKA.clear();
            TimeKA.clear();
            AllPoints.clear();
            numFreq.clear();
            tmpSkip1D.clear();
        }
    }

    DateKA.clear();
    TimeKA.clear();
    AllPoints.clear();
    tmpSkip.clear();
    numFreq.clear();
    input_line.clear();

    /// Настройка легенды
    legendWidget = new QWidget;
    label = new QwtLegendLabel;
    legend->contentsWidget()->layout()->setSpacing(20);

    label->setSpacing(20);
    curve1 = new QwtPlotCurve();
    curve1->setTitle( "GPS" );
    curve1->setPen( Qt::blue, 5 );
    curve1->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    curve1->attach(ui->p_plot);
    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve1) );
    label = qobject_cast<QwtLegendLabel*>(legendWidget);
    label->setSpacing(20);
    label->setVisible(true);

    curve2 = new QwtPlotCurve();
    curve2->setTitle( "ГЛОНАСС, 1 частота" );
    curve2->setPen( Qt::red, 5 );
    curve2->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    curve2->attach(ui->p_plot);
    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve2) );
    label = qobject_cast<QwtLegendLabel*>(legendWidget);
    label->setSpacing(20);
    label->setVisible(true);

    curve3 = new QwtPlotCurve();
    curve3->setTitle( "ГЛОНАСС, 2 частоты" );
    curve3->setPen( Qt::magenta, 5 );
    curve3->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    curve3->attach(ui->p_plot);
    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve3) );
    label = qobject_cast<QwtLegendLabel*>(legendWidget);
    label->setSpacing(20);
    label->setVisible(true);


    curve4 = new QwtPlotCurve();
    if( cOI->FindSp3ForDds( nz_dds, false ) > 0 )
    {
        curve4->setTitle( "ЭВИ(SP3)" );
        curve4->setPen( Qt::darkBlue, 5 );
        curve4->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        curve4->attach(ui->p_plot);
        legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve4) );
        label = qobject_cast<QwtLegendLabel*>(legendWidget);
        label->setSpacing(20);
        label->setVisible(true);
    }

    curve5 = new QwtPlotCurve();
    curve6 = new QwtPlotCurve();
    curve7 = new QwtPlotCurve();

    if( !list_mix.isEmpty() )
    {
        curve5->setTitle( "Кол-во изм.ГЛОНАСС 2 част. < 4" );
        curve5->setPen( Qt::darkRed, 5 );
        curve5->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        curve5->attach(ui->p_plot);
        legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve5) );
        label = qobject_cast<QwtLegendLabel*>(legendWidget);
        label->setSpacing(20);
        label->setVisible(true);

        curve6->setTitle( "Кол-во изм.ГЛОНАСС 2 част. = 4" );
        curve6->setPen( Qt::green, 5 );
        curve6->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        curve6->attach(ui->p_plot);
        legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve6) );
        label = qobject_cast<QwtLegendLabel*>(legendWidget);
        label->setSpacing(20);
        label->setVisible(true);

        curve7->setTitle( "Кол-во изм.ГЛОНАСС 2 част. > 4" );
        curve7->setPen( Qt::darkGreen, 5 );
        curve7->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        curve7->attach(ui->p_plot);
        legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(curve7) );
        label = qobject_cast<QwtLegendLabel*>(legendWidget);
        label->setSpacing(20);
        label->setVisible(true);
    }

    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    QPalette pal = legend->palette();
    pal.setColor(QPalette::Window, QColor(Qt::white));
    legend->setPalette(pal);
    legend->setAutoFillBackground(true);

    DrawEpoh( GenGps, GenGlonas );
    DrawEpohColor( GenGps, GenGlonas, list_mix );
    DrawDash( gps );
    DrawGap( GenGps, GenGlonas );

    ui->p_plot->setAxisScale(QwtPlot::xBottom, -5, 86403);
    ui->p_plot->setAxisMaxMajor(QwtPlot::xBottom, 20);
    ui->p_plot->setAxisMaxMinor(QwtPlot::xBottom, 10);

    ui->p_plot->setAxisScale(QwtPlot::yLeft, 0, GenGlonas + GenGps + 5);
    ui->p_plot->setAxisMaxMajor(QwtPlot::yLeft, 60);
    ui->p_plot->setAxisMaxMinor(QwtPlot::yLeft, 7);
    ui->p_plot->setAxisAutoScale(QwtPlot::yLeft);

    ui->p_plot->setAxisScaleDraw( QwtPlot::yLeft, new Y_ScaleDraw( isEpoh, GenGps, GenGlonas ) );
    ui->p_plot->setAxisScaleDraw( QwtPlot::xBottom, new X_ScaleDraw() );

    ui->p_plot->replot();

    for(int i=0; i<timeKA2D.size(); ++i)
    {
        timeKA2D[i].clear();
        dateKA2D[i].clear();
        AllPoints2D[i].clear();
    }
    timeKA2D.clear();
    dateKA2D.clear();
    AllPoints2D.clear();

    for(int i=0; i<Sheted2D.size(); ++i)
    {
        Sheted2D[i].clear();
    }
    Sheted2D.clear();

    for(int i=0; i<numFreq2D.size(); ++i)
    {
        numFreq2D[i].clear();
    }
    numFreq2D.clear();
    NameSatel.clear();

    for(int i=0; i<Skip2D.size(); ++i)
    {
        Skip2D[i].clear();
    }
    Skip2D.clear();

    if( isEpoh == true )
    {
        for( int i=0; i<TimeEpoh.size(); ++i )
        {
            TimeEpoh[i].clear();
        }
        TimeEpoh.clear();
    }

    if( isEpohColor == true )
    {
        for( int c=0; c<Numb.size(); c++ )
        {
            Numb[c].clear();
        }
        Numb.clear();
    }

    if(isEpohColor == true)
    {
        for(int c=0; c<TimeEpohCr.size(); c++)
        {
            TimeEpohCr[c].clear();
        }
        TimeEpohCr.clear();
    }
}

void FDdsView::DrawDash(int numGps)
{
    QPolygonF tmpSheted;
    QVector <QPolygonF> Skip, Sheted;
    QVector < QVector <int> > FreqNum2D;
    QVector <int> FreqNum1D;
    int count = 0;
    float DT = 0;

    for( int i=0; i<AllPoints2D.size(); ++i)
    {
        for(int j=0; j<AllPoints2D[i].size(); ++j)
        {

            if ( j == 0 )
            {
                tmpSheted << AllPoints2D[i][j];
            }

            else if( j > 0 && j < AllPoints2D[i].size()-1 )
            {
                DT = qAbs( AllPoints2D[i][j].x() - AllPoints2D[i][j-1].x() );
                if( DT <= deltaT )
                {
                    tmpSheted << AllPoints2D[i][j];
                }

                else
                {
                    Sheted.push_back( tmpSheted );
                    tmpSheted.clear();
                    FreqNum1D.push_back( numFreq2D[i][j-1] );
                    tmpSheted << AllPoints2D[i][j];
                }
            }

            else
            {
                DT = qAbs( AllPoints2D[i][j].x() - AllPoints2D[i][j-1].x() );
                if( DT <= deltaT )
                {
                    tmpSheted << AllPoints2D[i][j];
                    Sheted.push_back( tmpSheted );
                    FreqNum1D.push_back( numFreq2D[i][j-1] );
                    tmpSheted.clear();
                }

                else
                {
                    Sheted.push_back( tmpSheted );
                    tmpSheted.clear();
                    FreqNum1D.push_back( numFreq2D[i][j-1] );

                    tmpSheted << AllPoints2D[i][j];
                    Sheted.push_back( tmpSheted );
                    FreqNum1D.push_back( numFreq2D[i][j] );
                }
            }
        }
        Sheted2D.push_back( Sheted );
        FreqNum2D.push_back( FreqNum1D );
        tmpSheted.clear();
        Sheted.clear();
        FreqNum1D.clear();
    }

    for( int i=0; i<Sheted2D.size(); ++i)
    {
        count += Sheted2D[i].size();            // Подсчет количества элементов для отрисовки
    }

    p_curve = new QwtPlotCurve[ count ];
    count = 0;
    for(int i=0; i<Sheted2D.size(); ++i)
    {
        for(int j=0; j<Sheted2D[i].size(); ++j)
        {
            if( i < numGps )
            {
                p_curve[count].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::blue ), QPen( Qt::blue, 3 ), QSize(3,3) ) );
                p_curve[count].setSamples( Sheted2D[i][j] );
                p_curve[count].setPen( Qt::blue, 3 );
                p_curve[count].attach( ui->p_plot );
                legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_curve[count]) );
                label = qobject_cast<QwtLegendLabel*>(legendWidget);
                label->setVisible(false);
            }

            else
            {
                if( FreqNum2D[i][j] == 2 )
                {
                    p_curve[count].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::magenta ), QPen( Qt::magenta, 3 ), QSize(3,3) ) );
                    p_curve[count].setSamples( Sheted2D[i][j] );
                    p_curve[count].setPen( Qt::magenta, 3 );
                    p_curve[count].attach( ui->p_plot );
                    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_curve[count]) );
                    label = qobject_cast<QwtLegendLabel*>(legendWidget);
                    label->setVisible(false);
                }

                else
                {
                    p_curve[count].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::red ), QPen( Qt::red, 3 ), QSize(3,3) ) );
                    p_curve[count].setSamples( Sheted2D[i][j] );
                    p_curve[count].setPen( Qt::red, 3 );
                    p_curve[count].attach( ui->p_plot );
                    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_curve[count]) );
                    label = qobject_cast<QwtLegendLabel*>(legendWidget);
                    label->setVisible(false);
                }
            }
            count ++;
        }
    }
    ui->p_plot->replot();

    tmpSheted.clear();
    Sheted.clear();
    Skip.clear();
    FreqNum1D.clear();
    for(int i=0; i<FreqNum2D.size(); ++i)
        FreqNum2D[i].clear();
    FreqNum2D.clear();
}

void FDdsView::DrawGap(int GenGps, int GenGlonass)
{
    QVector <QwtIntervalSample> itervSamp;
    QPolygonF polygon;

        /// Добавление пропусков от крайних точек
    polygon.clear();
    for( int i=0; i<AllPoints2D.size(); ++i)
    {
        if( AllPoints2D[i][0].x() > deltaT )                                                // Левая граница
        {
            skip++;
            polygon << QPointF( 0, AllPoints2D[i][0].y() ) << AllPoints2D[i][0];
            Skip2D[i].push_front( polygon );
            polygon.clear();
        }

        if( ( 86399 - AllPoints2D[i][ AllPoints2D[i].size()-1 ].x() ) > deltaT )            // Правая граница
        {
            skip++;
            polygon << AllPoints2D[i][ AllPoints2D[i].size()-1 ] << QPointF( 86399, AllPoints2D[i][ AllPoints2D[i].size()-1 ].y() );
            Skip2D[i].push_back( polygon );
            polygon.clear();
        }
    }

        /// Отрисовка области пропусков
        /// Для поиска области пропуска достаточно проверить пропуски измерения произвольно выбранного спутника
        /// с измерениями других спутников. В нашем случае это будет 1ый спутник. Если пробел 1ого спутника не совпадает
        /// хотя бы с одним пробелом другого спутника, то мы этот интервал больше не рассматриваем.

    QwtInterval tmpInterv, tempInterv;
    QList <QwtInterval> RowList, tmpRowList;

        /// Поиск спутника с минимальным количеством пропусков
    int MinSk = 0;
    int tmpSize = Skip2D[0].size();
    for( int i=1; i<Skip2D.size(); ++i)
    {
        if( Skip2D[i].size() < tmpSize )
        {
            tmpSize = Skip2D[i].size();
            MinSk = i;
        }
    }

        /// Ставим пропуски спутника с мин количеством замеров в начало массива Skip2D для удобства
        /// и заполняем начальный набор интервалов в RowList
    for( int i=0; i<Skip2D[MinSk].size(); ++i )
    {
        tmpInterv.setInterval( Skip2D[MinSk][i].at(0).x(), Skip2D[MinSk][i].at(1).x(), QwtInterval::ExcludeBorders );
        RowList.push_back( tmpInterv );
    }

    for( int i=0; i<Skip2D.size(); i++ )
    {
        if( i != MinSk )
        {
            for( int j=0; j<Skip2D[i].size(); j++ )
            {
                tempInterv.setInterval(Skip2D[i][j].at(0).x(), Skip2D[i][j].at(1).x(), QwtInterval::IncludeBorders);

                for( int k=0; k<RowList.size(); k++ )
                {
                    if( RowList[k].intersects( tempInterv ) )
                    {
                        if( RowList[k].intersect(tempInterv).width() > deltaT )
                        {
                            tmpRowList.push_back( RowList[k].intersect(tempInterv) );
                        }
                    }
                }
            }

            RowList.clear();
            RowList = tmpRowList;
            tmpRowList.clear();
        }
    }

        /// Рисуем зоны, где отсутствуют измерения со всех спутников
    CenterZone = new QwtPlotHistogram[ RowList.size() ];
    lZ = new QwtPlotCurve;
    pZ = new QwtPlotCurve[2];

    if( !RowList.isEmpty())
    {

        for( int i=0; i<RowList.size(); ++i )
        {
            itervSamp.clear();
            itervSamp.push_back( QwtIntervalSample( GenGps+GenGlonass, QwtInterval( RowList[i].minValue(), RowList[i].maxValue() ) ) );
            CenterZone[i].setSamples( itervSamp );
            CenterZone[i].setStyle(QwtPlotHistogram::Columns);
            CenterZone[i].setBrush( QBrush(Qt::darkGray, Qt::Dense4Pattern) );
            CenterZone[i].setPen(Qt::darkGray,1);
            CenterZone[i].attach(ui->p_plot);
            legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&CenterZone[i]) );
            label = qobject_cast<QwtLegendLabel*>(legendWidget);
            label->setVisible(false);
        }
    }

    if( RowList.size() > 1 )
    {
        QVector <double> PZ;
        QVector <QPointF> PtZ;
        for( int i=1; i<RowList.size(); ++i )
        {
            if( RowList[i-1].maxValue() == RowList[i].minValue() )
            {
                PZ.push_back( RowList[i-1].maxValue() );
            }
        }

        pZ = new QwtPlotCurve[ PZ.size() ];
        for( int i=0; i<PZ.size(); ++i)
        {
            PtZ.push_back( QPointF(PZ[i], GenGps+GenGlonass) );
            PtZ.push_back( QPointF(PZ[i], 0) );
            pZ[i].setSamples(PtZ);
            pZ[i].setPen(Qt::white,2);
            pZ[i].attach(ui->p_plot);
            legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(pZ) );
            label = qobject_cast<QwtLegendLabel*>(legendWidget);
            label->setVisible(false);
            PtZ.clear();
        }
        PZ.clear();
    }
}

void FDdsView::DrawEpoh(int GenGps, int GenGlonass)
{
    QStringList list, tmpList;
    QString str;
    quint64 nz_sp3;
    QVector <QPointF> tmpTime;
    float T = 0.0, tmpT = 0.0 , delta = 0.0;
    bool prsoob = false;
    QTime CT(0,0,0,0), PT(0,0,0,0);

    nz_sp3 = cOI->FindSp3ForDds( nz_dds, prsoob );
    list = cOI->SP3InList(nz_sp3,true);

    if( list.isEmpty() )
    {
        isEpoh = false;
        return;
    }

    else
    {
        isEpoh = true;
        /// Заполняем TimeEpoh для дальнейшей отрисовки
        str = list[0];
        tmpList = str.split( QRegExp("\\s+") );
        str = tmpList.last();
        tmpList.removeLast();
        tmpList.append(str.split( QRegExp("\\W+") ));
        T = tmpList.at(3).toFloat()*3600 + tmpList.at(4).toFloat()*60 + tmpList.at(5).toFloat() + tmpList.at(6).toFloat();
        tmpTime.push_back( QPointF( T, GenGps + GenGlonass + 2 ) );                                                           // Положили начальное значение
        CT.addSecs(T);
        for( int c=1; c<list.size(); c++ )
        {
            str = list[c];
            tmpList = str.split( QRegExp("\\s+") );
            str = tmpList.last();
            tmpList.removeLast();
            tmpList.append(str.split( QRegExp("\\W+") ));
            tmpT = T;
            T = tmpList.at(3).toFloat()*3600 + tmpList.at(4).toFloat()*60 + tmpList.at(5).toFloat() + tmpList.at(6).toFloat();
            delta = qAbs(T - tmpT);

            PT = CT;
            CT.setHMS(0,0,0,0);
            CT = CT.addSecs(T);

            if( delta <= deltaTepoh )
            {
                tmpTime.push_back( QPointF( T, GenGps + GenGlonass + 2 ) );

                if( c==list.size()-1 )
                {
                    TimeEpoh.push_back(tmpTime);
                }
            }

            else
            {
                if( c==list.size()-1 )
                {
                    TimeEpoh.push_back(tmpTime);
                    tmpTime.clear();
                    tmpTime.push_back( QPointF( T,GenGps + GenGlonass + 2 ) );
                    TimeEpoh.push_back(tmpTime);
                    tmpTime.clear();
                }

                else
                {
                    TimeEpoh.push_back(tmpTime);
                    tmpTime.clear();
                    tmpTime.push_back( QPointF( T,GenGps + GenGlonass + 2 ) );
                }
            }

            str.clear();
            tmpList.clear();
        }

        p_epoh = new QwtPlotCurve[ TimeEpoh.size() ];
        for( int c=0; c<TimeEpoh.size(); c++ )
        {
            p_epoh[c].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::darkBlue ), QPen( Qt::darkBlue, 3 ), QSize(3,3) ) );
            p_epoh[c].setPen( Qt::darkBlue, 3 );
            p_epoh[c].setSamples( TimeEpoh[c] );
            p_epoh[c].attach( ui->p_plot );
            legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_epoh[c]) );
            label = qobject_cast<QwtLegendLabel*>(legendWidget);
            label->setVisible(false);
        }
    }

}

void FDdsView::DrawEpohColor(int GenGps, int GenGlonass, QStringList list_mix)
{
    QTime CT(0,0,0,0), PT(0,0,0,0);

    if( list_mix.isEmpty() )
    {
        isEpohColor = false;
        return;
    }

    else
    {
//        cOI->WriteFileFromList(list_mix,"/home/user/TimeEpohCr.txt");
        isEpohColor = true;
        int number = 0;
        QString str = "";
        QStringList tmpList;
        QVector <QPointF> tmpTime;
        QVector <int> tmpNumb;
        float T = 0.0, tmpT = 0.0 , delta = 0.0;

        str = list_mix[0];
        tmpList = str.split( QRegExp("\\s+") );
        str = tmpList.last();
        tmpList.removeLast();
        tmpList.append( str.split( QRegExp("\\W+") ) );
        T = tmpList.at(3).toFloat()*3600 + tmpList.at(4).toFloat()*60 + tmpList.at(5).toFloat() + tmpList.at(6).toFloat();
        number = tmpList.at(7).toInt();
        tmpTime.push_back( QPointF( T, GenGps + GenGlonass + 1 ) );
        tmpNumb.push_back( number );
        CT.addSecs(T);

        int count = 0;
        for(int c=1; c<list_mix.size(); c++)
        {
            str = list_mix[c];
            tmpList = str.split( QRegExp("\\s+") );
            str = tmpList.last();
            tmpList.removeLast();
            tmpList.append(str.split( QRegExp("\\W+") ));
            tmpT = T;
            T = tmpList.at(3).toFloat()*3600 + tmpList.at(4).toFloat()*60 + tmpList.at(5).toFloat() + tmpList.at(6).toFloat();
            number = tmpList.at(7).toInt();
            delta = qAbs(T - tmpT);

            PT = CT;
            CT.setHMS(0,0,0,0);
            CT = CT.addSecs(T);

            if( delta <= deltaT )
            {
                tmpTime.push_back( QPointF( T, GenGps + GenGlonass + 1 ) );
                tmpNumb.push_back( number );

                if( c == list_mix.size()-1 )
                {
                    TimeEpohCr.push_back(tmpTime);
                    Numb.push_back( tmpNumb );
                }
            }

            else
            {
                if( c == list_mix.size()-1 )
                {
                    TimeEpohCr.push_back( tmpTime );
                    Numb.push_back( tmpNumb );
                    tmpTime.clear();
                    tmpNumb.clear();
                    tmpTime.push_back( QPointF( T,GenGps + GenGlonass + 1 ) );
                    tmpNumb.push_back( number );
                    TimeEpohCr.push_back(tmpTime);
                    Numb.push_back( tmpNumb );
                    tmpTime.clear();
                    tmpNumb.clear();
                }

                else
                {
                    TimeEpohCr.push_back( tmpTime );
                    Numb.push_back( tmpNumb );
                    tmpTime.clear();
                    tmpNumb.clear();
                    tmpTime.push_back( QPointF( T, GenGps + GenGlonass + 1 ) );
                    tmpNumb.push_back( number );
                }
                count++;
            }

            str.clear();
            tmpList.clear();
        }

        p_ClEpoh = new QwtPlotCurve[list_mix.size()];
        QVector <QPointF> tmpP;
        int score = 0;
        for( int i=0; i<Numb.size(); ++i )
        {
            for( int j=1; j<Numb[i].size(); ++j )
            {
                if( Numb[i][j] < 4 )
                {
                    tmpP.push_back( TimeEpohCr[i][j-1] );
                    tmpP.push_back( TimeEpohCr[i][j] );

                    p_ClEpoh[score].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::darkRed ), QPen( Qt::darkRed, 3 ), QSize(3,3) ) );
                    p_ClEpoh[score].setPen( Qt::darkRed, 3 );
                    p_ClEpoh[score].setSamples( tmpP );
                    p_ClEpoh[score].attach( ui->p_plot );
                    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_ClEpoh[score]) );
                    label = qobject_cast<QwtLegendLabel*>(legendWidget);
                    label->setVisible(false);

                    tmpP.clear();
                    score ++;
                }

                else if( Numb[i][j] ==4 )
                {
                    tmpP.push_back( TimeEpohCr[i][j-1] );
                    tmpP.push_back( TimeEpohCr[i][j] );

                    p_ClEpoh[score].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::green ), QPen( Qt::green, 3 ), QSize(3,3) ) );
                    p_ClEpoh[score].setPen( Qt::green, 3 );
                    p_ClEpoh[score].setSamples( tmpP );
                    p_ClEpoh[score].attach( ui->p_plot );
                    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_ClEpoh[score]) );
                    label = qobject_cast<QwtLegendLabel*>(legendWidget);
                    label->setVisible(false);

                    tmpP.clear();
                    score ++;
                }

                else
                {
                    tmpP.push_back( TimeEpohCr[i][j-1] );
                    tmpP.push_back( TimeEpohCr[i][j] );

                    p_ClEpoh[score].setSymbol( new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::darkGreen ), QPen( Qt::darkGreen, 3 ), QSize(3,3) ) );
                    p_ClEpoh[score].setPen( Qt::darkGreen, 3 );
                    p_ClEpoh[score].setSamples( tmpP );
                    p_ClEpoh[score].attach( ui->p_plot );
                    legendWidget = legend->legendWidget( ui->p_plot->itemToInfo(&p_ClEpoh[score]) );
                    label = qobject_cast<QwtLegendLabel*>(legendWidget);
                    label->setVisible(false);

                    tmpP.clear();
                    score ++;
                }

            }
            score++;
        }
    }
}

void FDdsView::closeEvent(QCloseEvent *event)
{
    ui->p_plot->detachItems(QwtPlotItem::Rtti_PlotCurve,false);
    ui->p_plot->detachItems(QwtPlotItem::Rtti_PlotZone,false);
    ui->p_plot->detachItems(QwtPlotItem::Rtti_PlotMultiBarChart, false);
    ui->p_plot->detachItems(QwtPlotItem::Rtti_PlotHistogram, false);

    delete legendWidget;
    delete label;
    delete curve1;
    delete curve2;
    delete curve3;
    delete curve4;
    delete curve5;
    delete curve6;
    delete curve7;
    delete [] p_curve;
    delete LeftZone;
    delete RightZone;
    delete [] CenterZone;
    delete lZ;
    delete [] pZ;

    if( isEpoh == true )
    {
        delete [] p_epoh;
    }

    if( isEpohColor == true )
    {
        delete [] p_ClEpoh;
    }

    event->accept();
}

void FDdsView::showEvent(QShowEvent *event)
{
    QStringList list_mix;
    ReadInList( cOI->SpisEpochForSatDDS( nz_dds, list_mix ), list_mix );
    ui->p_plot->setTitle("График суточного файла ДДС");

    event->accept();
    return;
}

void FDdsView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent( event );
    ui->p_plot->setGeometry (0, 50, event->size().width(), event->size().height()-60 );
//    ui->p_plot->replot();
    event->accept();
}

void FDdsView::mouseMoveEvent(QMouseEvent *event)
{
//    ui->p_plot->replot();
    event->accept();
}

void FDdsView::mousePressEvent(QMouseEvent *event)
{
    QTime T(0,0,0);
    double x = ui->p_plot->invTransform( QwtPlot::xBottom, p_picker->GlPoint.x() );
    T = T.addSecs( x );

    if(event->button() == Qt::LeftButton)
    {
        if( x >= 0 && x <= 86399)
        {
            QToolTip::showText( QCursor::pos(), T.toString(), ui->p_plot->canvas() );
        }

        else
        {
            QToolTip::showText( QCursor::pos(), QString::number(x), ui->p_plot->canvas() );
        }
    }
//    ui->p_plot->replot();
    event->accept();
}

void FDdsView::mouseReleaseEvent(QMouseEvent *event)
{
    QToolTip::hideText();
    ui->p_plot->replot();
    event->accept();
}

FDdsView::~FDdsView()
{
    delete magnifier;
    delete p_panner;
    delete p_picker;
    delete grid;
    delete legend;
    delete cOI;
    delete ui;
}

QwtText MyQwtPlotPicker::trackerTextF(const QPointF &pos)
{
    QString text;

    switch(rubberBand()) {
    case HLineRubberBand:
        text.sprintf("%.2f", pos.y());
        break;
    case VLineRubberBand:
        text.sprintf("%.2f", pos.x());
        break;
    default:
        text.sprintf("%.2f, %.2f", pos.x(), pos.y());
    }
    return QwtText(text);
}

QwtText MyQwtPlotPicker::trackerText(const QPointF & pos)
{
    QString text;

    switch(rubberBand()) {
    case HLineRubberBand:
        text.sprintf("%.2f", pos.y());
        break;
    case VLineRubberBand:
        text.sprintf("%.2f", pos.x());
        break;
    default:
        text.sprintf("%.2f, %.2f", pos.x(), pos.y());
    }
    return QwtText(text);
}

void MyQwtPlotPicker::append(const QPoint &pos)
{
    GlPoint = pos;
}
