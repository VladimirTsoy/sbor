#ifndef FPOTDDS_H
#define FPOTDDS_H

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_renderer.h>
#include <qdir.h>
#include <QPainter>
#include <QMouseEvent>
#include <QPrinter>
#include <QDate>
#include <QFileDialog>
#include <QBoxLayout>
#include <QPushButton>
#include <qwt_painter.h>
#include <qwt_plot_marker.h>
#include "obrizm.h"


namespace Ui {
class FPotDds;
}


class FPotDds : public QWidget
{
    Q_OBJECT
public:
    explicit FPotDds(QWidget *parent = 0);
    ~FPotDds();
    void     setGraphParams(QStringList *_strList);

    QString     getMonth(int month);
    QDate dat;
    void        getDateInterval();
    ObrIzm   *cOI;
    QString nka;

public slots:
    void     savePdf();
protected:
    void wheelEvent(QWheelEvent *event);
//    void showEvent();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
private:
    Ui::FPotDds *ui;
    QStringList strList;
    QwtPlot *graph;
    QwtPlotHistogram *hystogramL;
    QwtPlotHistogram *hystogramR;
    QVector <QwtIntervalSample>  *intervalsL;
    QVector <QwtIntervalSample>  *intervalsR;
    QwtMagnifier *magnifier;
    QwtPlotGrid *grid;

    QVector <QwtPlotMarker*>  markers;
    QVector <QwtText*>  txtsQwt;
    QString    date = "";
    int     deltaScale = 1;
    QDate   dtn;
    QDate   dtk;
    QIcon icon;
};

#endif // FPOTDDS_H
