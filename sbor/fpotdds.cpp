#include "fpotdds.h"
#include "ui_fpotdds.h"

FPotDds::FPotDds(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FPotDds)
{
    ui->setupUi(this);

    QDate d = QDate::currentDate();

    //Строка для формирования имени файла pdf
//    if(strList->size() > 0){
//        date = (*strList)[0].split(';')[0].split('-')[0] + (*strList)[0].split(';')[0].split('-')[1];
//        d = QDate((*strList)[0].split(';')[0].split('-')[0].toInt(),(*strList)[0].split(';')[0].split('-')[1].toInt(),(*strList)[0].split(';')[0].split('-')[2].toInt());
//    }
    //    qDebug()<<date;

    QHBoxLayout *lyt = new QHBoxLayout;
    QPushButton *btn = new QPushButton("Сохранить в PDF");
    icon.addPixmap(QPixmap(":/res/1/file_extension_pdf.png"));
    QSize size; size.setHeight(32); size.setWidth(32);
    icon.actualSize(size);
    btn->setIcon(icon);

    graph = new QwtPlot(this);

    lyt->addWidget(graph);
    lyt->addWidget(btn);



    graph->setTitle(getMonth(d.month()));
    graph->setCanvasBackground(Qt::white);

    graph->setAxisTitle(QwtPlot::yLeft, "% Процент");
    graph->setAxisTitle(QwtPlot::xBottom, " ");//"Date: " + strList[0][0].split(';')[0]);

    QwtLegend *leg = new QwtLegend();
    graph->insertLegend(leg, QwtPlot::RightLegend);

    grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::gray, 2));
    grid->attach(graph);

//    QwtPlotCurve *curve = new QwtPlotCurve(); //кривая
//    curve->setTitle("Curve");
//    curve->setPen(Qt::blue, 6);
//    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true); //сглаживание

//    //Маркеры кривой
//    QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::yellow), QPen(Qt::red, 2), QSize(8,8));
//    curve->setSymbol(symbol);

//    //Добавить точки на кривую
//    QPolygonF points;
//    points << QPointF(1.0, 1.0) << QPointF(1.5, 2.0) << QPointF(3.0, 2.0) << QPointF(3.5, 3.0)
//           << QPointF(5.0, 3.0) << QPointF(6.0, 4.0);

//    curve->setSamples(points); // ассоциировать набор точек с кривой
//    curve->attach(graph);      // отобразить кривую на графике graph

    graph->setAxisScale(QwtPlot::xBottom, 0, 30, 1);   // задать максимум оси
    graph->setAxisScale(QwtPlot::yLeft, 0, 100);

    //Возможность приближения удаления графика
    magnifier = new QwtPlotMagnifier(graph->canvas());
    magnifier->setMouseButton(Qt::MidButton);


    //Возможность перемещения по графику
    QwtPlotPanner *d_panner = new QwtPlotPanner(graph->canvas());

    d_panner->setMouseButton(Qt::RightButton);

    //Возможность определения координат курсора
    QwtPlotPicker *d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,   // ассоциация с осями
                                            QwtPlotPicker::CrossRubberBand,         // стиль перпендикулярных линий
                                            QwtPicker::ActiveOnly,                  // включение / выключение
                                            graph->canvas() );                      // ассоциация с полем

    d_picker->setRubberBandPen( QColor(Qt::red));
    d_picker->setTrackerPen(QColor(Qt::black));
    d_picker->setStateMachine( new QwtPickerDragPointMachine());

    QObject::connect(btn, SIGNAL(clicked(bool)), this, SLOT(savePdf()));

    setLayout(lyt);
}

FPotDds::~FPotDds()
{
    delete ui;

    //strList->clear();
    //strList = nullptr;

    intervalsL = nullptr;
    intervalsR = nullptr;

//    delete hystogramL;
//    delete hystogramR;
//    hystogramL = nullptr;
//    hystogramR = nullptr;

//    for(int i = 0; i < txtsQwt.size(); i++){
//        if(txtsQwt[i] != nullptr)
//            delete txtsQwt[i];
//    }
//    for(int i = 0; i < markers.size(); i++){
//        if(markers[i] != nullptr)
//            delete markers[i];
//    }
//    txtsQwt.clear();
//    markers.clear();

    delete graph;
    graph = nullptr;


}

void FPotDds::showEvent(QShowEvent *event)
{
 getDateInterval();
 graph->setTitle(getMonth(dat.month()));
 grid = new QwtPlotGrid();
 grid->setMajorPen(QPen(Qt::gray, 2));
 grid->attach(graph);
// graph->detachItems(QwtPlotItem::Rtti_PlotItem,true);
 //graph->detachItems(QwtPlotItem::Rtti_PlotCurve);
 //graph->detachItems(QwtPlotItem::Rtti_PlotMarker);
 //Возможность приближения удаления графика
// magnifier = new QwtPlotMagnifier(graph->canvas());
// magnifier->setMouseButton(Qt::MidButton);

 //graph->setAxisScale(QwtPlot::xBottom, 0, 30, 1);   // задать максимум оси
 //graph->setAxisScale(QwtPlot::yLeft, 0, 100);

 //Возможность приближения удаления графика
 //magnifier = new QwtPlotMagnifier(graph->canvas());
 //magnifier->setMouseButton(Qt::MidButton);
 //graph->attachItem(QwtPlotItem::Rtti_PlotMarker,true);

 strList = cOI->StatDDSInList(dtn, dtk, nka,true);
 setGraphParams(&strList);
 event->accept();
}

void FPotDds::closeEvent(QCloseEvent *event)
{
 graph->detachItems(QwtPlotItem::Rtti_PlotItem,true);
 //graph->detachItems(QwtPlotItem::Rtti_PlotCurve);
 //graph->detachItems(QwtPlotItem::Rtti_PlotMarker);
 markers.clear();

 event->accept();
}

///----------------------------------------------
/// Установка параметров графика из QStringList
///----------------------------------------------

void FPotDds::setGraphParams(QStringList *_strList)
{
    hystogramL = new QwtPlotHistogram();


    hystogramR = new QwtPlotHistogram();

    QVector<QwtIntervalSample> *intervals_1 = new QVector<QwtIntervalSample>;
    QVector<QwtIntervalSample> *intervals_2 = new QVector<QwtIntervalSample>;
    QStringList lst;

    hystogramL->setPen(QPen(Qt::black));
    hystogramR->setBrush(QBrush(Qt::blue));

    //Добавление координат точек гистограммы
    for(auto str:*_strList){
        lst = str.split(';');
        intervals_1->append(QwtIntervalSample( lst[2].toFloat() / lst[1].toFloat() * 100,
                                            lst[0].split('-')[2].toFloat() - 0.2, lst[0].split('-')[2].toFloat() ));
        intervals_2->append(QwtIntervalSample(lst[3].toFloat() / lst[1].toFloat() * 100,
                                            lst[0].split('-')[2].toFloat(), lst[0].split('-')[2].toFloat() + 0.2));
    }

    //Формирование надписей над точками гистограмм

    for(auto str:*_strList){
        lst = str.split(';');
        QwtText *txt = new QwtText(QString::number(lst[2].toInt() * 100 / lst[1].toInt()) + "/" + QString::number(lst[3].toInt() * 100 / lst[1].toInt()));
        txtsQwt.append(txt);
        //QFont font("Helvetica [Cronyx]", 8);
        QFont font("Times", 8);

        txt->setFont(font);
        QwtPlotMarker *marker = new QwtPlotMarker();

        markers.push_back(marker);
        marker->setLabel(*txt); // (txts[0])

        marker->setXValue(lst[0].split('-')[2].toFloat());
        marker->setYValue(lst[2].toFloat() * 100 / lst[1].toFloat() + 3);


        marker->attach(graph);
        //marker->detach();
    }

    QwtPlotMarker m;
    QwtText txt = QString("awds");


    m.setLabel(txt);
    m.attach(graph);

    hystogramL->setTitle("Всего");
    hystogramR->setTitle("2-х част/д");

    hystogramL->setSamples(*intervals_1);

    hystogramL->attach(graph);

    hystogramR->setSamples(*intervals_2);

    hystogramR->attach(graph);

    //Ручка, кисть
    hystogramL->setPen(QPen(Qt::black));
    hystogramL->setBrush(QBrush(Qt::blue));

    hystogramR->setPen(QPen(Qt::black));
    hystogramR->setBrush(QBrush(Qt::yellow));

    graph->replot();

}

QString FPotDds::getMonth(int month)
{
    QString Smonth[] = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь","Июль","Август","Сентябрь","Октябрь","Ноябрь", "Декабрь"};
    return Smonth[month - 1];
}

void FPotDds::getDateInterval()
{
    dtn.setDate(dat.year(), dat.month(), 1);
    dtk.setDate(dat.year(), dat.month(), dat.daysInMonth());
}


///----------------------------------
/// Сохранение графика в формате pdf
///----------------------------------

void FPotDds::savePdf()
{
    QPrinter printer;
    QPainter p;
    QDir dirFile;

//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::currentPath(),
//                               tr("Document files (*.doc *.rtf *.pdf);;All files (*.*)"),0, QFileDialog::DontUseNativeDialog);
//    QString dirname = QFileDialog::getExistingDirectory(
//                this,tr("Выберите папку для сохранения PDF файла: "),
//                QDir::currentPath());

//сохранить как
    QString dirname = QFileDialog::getSaveFileName(
                this,tr("Open document: "),
                QDir::currentPath() + "/dds" + date,
                tr("Document files (*.doc *.pdf)") );
    dirFile.setPath("");
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(dirname + ".pdf");

    p.begin(&printer);
    QRect rect = p.window();
//    p.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, tr("Text for test one"));

    p.save();

    QRect qwtRect = graph->rect();
    qwtRect.setY(qwtRect.y() + 30);
    qwtRect.setWidth(rect.width());
    qwtRect.setHeight(rect.height() - 80);
    QRectF qwtRectF(qwtRect);
    QwtPlotRenderer ren(0);
    ren.render(graph, &p, qwtRectF);
    p.end();
}

///--------------------------------------------
///  Показать / Скрыть маркеры над столбиками
///--------------------------------------------
void FPotDds::wheelEvent(QWheelEvent *event)
{
    if(!event)
        return;
    int delta = event->delta() / 120;
    deltaScale += delta;
    if(deltaScale > 3){
        for(int i = 0; i < markers.size(); i++){
            if(markers[i] != nullptr){
                markers[i]->hide();
            }
        }
    }else{
        for(int i = 0; i < markers.size(); i++){
            if(markers[i]!= nullptr){
                markers[i]->show();
            }
        }
    }
}


