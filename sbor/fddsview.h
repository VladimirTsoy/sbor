#ifndef FDDSVIEW_H
#define FDDSVIEW_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QTextCodec>
#include <QToolBar>
#include <QVector>
#include "obrizm.h"
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_legend_data.h>
#include <qwt_legend_label.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_legenditem.h>
#include <qwt_picker_machine.h>
#include <qwt_picker.h>
#include <qwt_counter.h>
#include <qwt_graphic.h>
#include <qwt_painter.h>
#include <qwt_interval.h>
#include <qwt_plot_directpainter.h>
#include <qwt_series_data.h>
#include <qwt_plot_seriesitem.h>
#include <qwt_curve_fitter.h>
#include <qwt_point_data.h>
#include <qwt.h>
#include <qwt_samples.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_zoomer.h>
#include <qwt_global.h>
#include <qwt_column_symbol.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_item.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_plot_barchart.h>
#include <qwt_plot_abstract_barchart.h>
#include <qwt_plot_histogram.h>
#include <fstream>
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QTime>
#include <QRegExp>
#include <QTimer>
#include <QDateTime>
#include <QLineF>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QTextCodec>
#include <QtAlgorithms>

namespace Ui
{
    class FDdsView;
}

class QWT_EXPORT MyQwtPlotPicker: public QwtPlotPicker
{
    Q_OBJECT
public:
    QPoint GlPoint;

    MyQwtPlotPicker( int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget*):
                    QwtPlotPicker( xAxis, yAxis, rubberBand, trackerMode, new QWidget)

    { }
    MyQwtPlotPicker(QWidget* canvas): QwtPlotPicker( canvas )
    { }
    QwtText trackerTextF(const QPointF &pos);
    QwtText trackerText(const QPointF & pos) ;
    void append(const QPoint &pos);
};

class FDdsView : public QWidget
{
    Q_OBJECT

public:
    explicit FDdsView(QWidget *parent = 0);
    ~FDdsView();
    ObrIzm *cOI;
    qlonglong nz_dds;
    Ui::FDdsView *ui;
//    int GLN = 0, GPS = 0;
private:

    QwtPlotGrid * grid;                         /// Сетка
    QwtLegend * legend;                         /// Легенда
    QWidget* legendWidget;                      /// Для вставки легенды
    QwtLegendLabel* label;                      /// Для отображения легенды
    QwtPlotCurve *curve1;                       ///
    QwtPlotCurve *curve2;                       /// Для высвечивания легенды
    QwtPlotCurve *curve3;                       ///
    QwtPlotCurve *curve4, *curve5, *curve6, *curve7;
    QwtPlotCurve *p_curve;                      /// Для отображения измерений
    QwtPlotCurve *p_epoh, *p_ClEpoh;             /// Для отображения эпох
    QwtPlotHistogram *LeftZone;                 /// Построение зоны пробела от начала суток
    QwtPlotHistogram *RightZone;                /// Построение зоны пробела до конца суток
    QwtPlotHistogram *CenterZone;               /// Построение зоны пробела внутри области
    QwtPlotCurve *lZ;                           /// Отрисовка левой полоски на начало измерений
    QwtPlotCurve *pZ;                           /// Для отрисовки белой линии

    QVector <QString> NameSatel;                /// Назавания спутников
    QVector < QVector <QTime> > timeKA2D;       /// Для хранения временных отметок каждого спутника
    QVector < QVector <QDate> > dateKA2D;       /// Для хранения даты замера каждого спутника
    QVector < QVector <QPointF> > AllPoints2D;  /// Для хранения набора точек каждому спутнику
    QVector < QVector <QPolygonF> > Skip2D;     /// Пропуски измерений 2D
    QVector < QVector <QPolygonF> > Sheted2D;   /// Разбитие массива для отрисовки
    QVector < QVector <int> > numFreq2D;        /// Количество частот по каждому спутнику  
    QVector < QVector <QPointF> > TimeEpoh;     /// Эпохи
    QVector < QVector <QPointF> > TimeEpohCr;   /// Эпохи (цветные)
    QVector < QVector <int> > Numb;             /// Количество измерений по 2м каналам

    QWidget *wid;

    int deltaT;
    int deltaTepoh;
    int skip;
    bool isEpoh = true;
    bool isEpohColor = true;

    void addPlot(); /// Создание поля для графика со шкалами
    void addPlotGrid(); /// Отрисовка сетки

    QwtPlotMagnifier *magnifier;
    void enableMagnifier(); /// Приближение/отдаление

    QwtPlotPanner *p_panner;
    void enableMovinOnPlot(); /// Перемещение графика

    MyQwtPlotPicker *p_picker;
    void enablePicker(); /// Отображение координаты курсора

    void ReadInList(QStringList input_line , QStringList list_mix) ;  /// Чтение эпох в массив AllPoints2D
    void DrawDash(int numGps);                  /// Отрисовка эпох
    void DrawGap(int GenGps, int GenGlonass);   /// Отросовка пропусков измерений
    void DrawEpoh(int GenGps, int GenGlonass);  /// Отрисовка эпохи
    void DrawEpohColor(int GenGps, int GenGlonass, QStringList list_mix);

protected:
    virtual void closeEvent( QCloseEvent *event );
    virtual void resizeEvent( QResizeEvent *event );
    virtual void showEvent(QShowEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

class Y_ScaleDraw: public QwtScaleDraw
{
    int Ngps = 0, Ngln = 0;
    bool isEp = false;
    int boundY = 1;
public:
    Y_ScaleDraw(bool isEpoh, int gps = 1 , int glonass = 0):QwtScaleDraw()
    {
        Ngps = gps;
        Ngln = glonass;
        isEp = isEpoh;
    }

    virtual QwtText label(double v) const
    {
        QString yleft;
        int iv = v;

        if( iv == v )
        {
            if ( v > 0 && v < Ngps + boundY )
            {
                yleft = "G"+QString::number(iv).rightJustified(2,'0');
            }

            else if ( v > Ngps && v < Ngps + Ngln + boundY)
            {
                yleft = "R"+QString::number(iv-Ngps).rightJustified(2,'0');
            }

            else if( v == Ngps + Ngln + 1 )
            {
                yleft = "Изм. ГЛОНАСС";
            }

            else if( v == Ngps + Ngln + 2 && isEp == true )
            {
                yleft = "ЭВИ";
            }

            else
            {
                yleft =" ";
            }
        }

        else
        {
            yleft =" ";
        }



//        if ( v > 0 && v < Ngps + boundY )
//        {
//            yleft = "G"+QString::number(iv).rightJustified(2,'0');
//        }

//        else if ( v > Ngps && v < Ngps + Ngln + boundY)
//        {
//            yleft = "R"+QString::number(iv-Ngps).rightJustified(2,'0');
//        }

//        else if( v == Ngps + Ngln + 1 )
//        {
//            yleft = "Эпохи.Глонасс";
//        }

//        else if( v == Ngps + Ngln + 2  )
//        {
//            yleft = "ЭВИ";
//        }

//        else
//        {
//            yleft =" ";
//        }

        return yleft;
    }
};

class X_ScaleDraw: public QwtScaleDraw
{
public:
    X_ScaleDraw():QwtScaleDraw()
    {
    }

    virtual QwtText label(double v) const
    {
        int iv = v;
        QTime t = QTime(0,0,0,0).addSecs(iv);
        QString time=t.toString("hh:mm:ss");
        return time;
    }
};

#endif // FDDSVIEW_H
