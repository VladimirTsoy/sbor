#ifndef PPSI_H
#define PPSI_H

#include "ui_ppsi.h"
#include "statfullfile.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

#include <qwt_polar_panner.h>
#include <qwt_polar_curve.h>
#include <qwt_polar_picker.h>
#include <qwt_polar_magnifier.h>
#include <qwt_polar_grid.h>


class TimeScaleDrawX: public QwtScaleDraw
{
public:

    TimeScaleDrawX():QwtScaleDraw() {}

    virtual QwtText label(double v) const
    {
        QTime t = QTime(0,0,0,0).addMSecs(v);
        QString time=t.toString("hh:mm:ss.zzz");
        return time;
    }
};


class KanScaleDrawY: public QwtScaleDraw
{
public:
    KanScaleDrawY():QwtScaleDraw() {}

    virtual QwtText label(double v) const
    {
        QString yleft;
        int iv = v;

        if( iv == v )
            yleft = QString::number(iv);

        else
            yleft =" ";

        return yleft;
    }
};


class Data0: public QwtSeriesData<QwtPointPolar>
{
    public:
    Data0( const QVector<QwtPointPolar> &p,size_t size): d_p(p),d_size( size )
    {
    }

    virtual size_t size() const
    {
        return d_size;
    }

    protected:
    QVector<QwtPointPolar> d_p;
    size_t d_size;
};


class ShowTime: public QwtPlotPicker
{
public:
    ShowTime( QWidget *canvas );
    ShowTime(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget* canvas);
    virtual QwtText trackerTextF( const QPointF & ) const;
};


class PolarZona: public Data0
{
    public:
    PolarZona(const QVector<QwtPointPolar> &p, size_t size): Data0( p, size )
    {
       p1=p;
    }

    virtual QwtPointPolar sample( size_t i ) const
    {
        return p1.at(i);
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }
    QVector<QwtPointPolar> p1;

};


class ComparePoint
{
public:
    ComparePoint() {}
    bool operator()(QPointF &point_left, QPointF &point_right) const
    { return point_left.x() < point_right.x(); }
};


class EqualPoint
{
public:
    EqualPoint() {}
    bool operator() (QPointF &point_left, QPointF &point_right) const
    { return point_left.x() == point_right.x(); }
};


class ZoomSiAr:public QwtPlot
{
    Q_OBJECT
public:
    ZoomSiAr();
    ~ZoomSiAr();
    void PlotStat( QVector <QPointF> *SiSh1, QVector <QPointF> *SiSh2, QVector <QPointF> *ARU1, QVector <QPointF> *ARU2 );
    void PlotAnother(  QVector <QPointF> *SiSh1, QVector <QPointF> *SiSh2, QVector <QPointF> *ARU1, QVector <QPointF> *ARU2 );
private:
    QwtPlotCurve curSiSh1, curSiSh2;
    QwtPlotCurve curARU1, curARU2;
    QwtPlotCurve curSiSh1_an, curSiSh2_an;  // Для 2ого канала
    QwtPlotCurve curARU1_an, curARU2_an;

    QwtPlotPanner *paner;
    ShowTime *piker;
    QwtPlotGrid grid;
    QwtPlotMagnifier *magni;
    QwtSymbol symSiSh1_an, symSiSh2_an;
    QwtSymbol symARU1_an, symARU2_an;
    QwtLegend legend;
    QwtLegendLabel *leg_label;
    QWidget *legend_widget;
};


class StatVirKan:public QwtPlot
{
    Q_OBJECT
public:
    StatVirKan();
    ~StatVirKan();
    void PlotVK(QVector<QPointF> *Stat, QVector<QPointF> *Stat2, QVector <QPointF> *ErCRC , QVector<QPointF> *ErCRC2);
    void PlotVK(QVector<CadrePPSI> &ppsi1, QVector<CadrePPSI> &ppsi2, QVector<Cadrs> &cad);
    void PlotEmpty(QVector<QPointF> &empty_cad1, QVector<QPointF> &empty_cad2);
    void init_chanel(QVector<Cadrs> &cad);
private:
    QwtPlotCurve cur, crc, cur2, crc2;
    QwtPlotPanner *paner;
    ShowTime *piker;
    QwtPlotGrid grid;
    QwtPlotMagnifier *magni;
    QwtSymbol sym, sym2, sym3, sym4;
    QVector<QPointF> chanel1, chanel2, chanel_1and2, chanel_er;
    QVector<QPointF> chanel_some_gen, chanel_both_gen, empty_gen;
    QwtPlotCurve cur_ch1, cur_ch2, cur_ch_1and2, cur_ch_er;
    QwtPlotCurve cur_some_gen, cur_both_gen, cur_empty_gen;
    QwtPlotCurve cur_empty1, cur_empty2;
    QwtLegend legend;
    QwtLegendLabel *leg_label;
    QWidget *legend_widget;
    QwtSymbol sm_ch1, sm_ch2, sm_ch_1and2, sm_ch_er;
    QwtSymbol sm_both_gen, sm_some_gen, sm_empty_gen;
    QwtSymbol sm_empty_cad1, sm_empty_cad2;
    StatFullFile st_ful;
};


class WidErCRC: public QwtPlot
{
    Q_OBJECT
public:
    WidErCRC();
    ~WidErCRC();
    void Plot_er_CRC(QVector <QPointF> *erorCRC1 , QVector<QPointF> *erorCRC2);
    void Plot_empty_cadrs(QVector <QPointF> &empty_cad1, QVector<QPointF> &empty_cad2);
private:
    QwtPlotCurve crc, crc2;
    QwtPlotCurve cur_empty1, cur_empty2;
    QwtPlotPanner *paner;
    QwtPlotGrid grid;
    ShowTime *piker;
    QwtPlotMagnifier *magni;
    QwtSymbol sym, sym2;
    QwtSymbol sym_empty1, sym_empty2;
    QwtLegend legend;
    QWidget *legend_widget;
    QwtLegendLabel *leg_label;
};


class ppsi : public QMainWindow
{
    Q_OBJECT

public:
    ppsi(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~ppsi();

    double pi;
    QString fileName;
    QFile* file;
    ObrIzm *cOI;
    QByteArray buf1, buf2;
    QByteArray *buf_consider;
    QByteArray sinxr;

    int S1ind000,S1ind001,S1ind010,S1ind011,S1ind100,S1ind101,S1ind110,S1ind111;
    int S2ind000,S2ind001,S2ind010,S2ind011,S2ind100,S2ind101,S2ind110,S2ind111;
    int S3ind000,S3ind001,S3ind010,S3ind011,S3ind100,S3ind101,S3ind110,S3ind111;
    int errorCRC;
    int snMkadr;
    int kanal;                          // Номер рассматриваемого канала
    QDateTime time_start, time_end;     // Время начала и окончания кадров (для кадров с маркером)

    QwtLegend legend_stat, legend_crc, legend_tel;
    QWidget *legend_widget_stat, *legend_widget_crc, *legend_widget_tel;
    QwtLegendLabel *leg_label;
    QwtPlotMagnifier *magni_stat, *magni_SH, *magni_les;
    QwtPlotPanner *pan_stat, *pan_SH, *pan_les;
    QVector <QwtPointPolar> VecCU, start_point;
    QwtPlotGrid grid_stat, grid_SH, grid_les;

    QwtPolarCurve polCur, polCur_start;
    QwtSymbol sym_polar, sym_polar_start;
    QwtPolarGrid d_grid;
    QWidget *legend_widget_polar;
    QwtLegendLabel *label_polar;
    QwtLegend legend_polar;
    QPalette pal_polar;
    QwtPolarPanner *pan;
    QwtPolarMagnifier *magni;
    QwtPolarPicker *piker;

    QVector <CadrePPSI> ppsi1, ppsi2;
    QVector <CadrePPSI> *ppsi_consider;
    QVector <QPointF> stat1, stat2;
    QVector <QPointF> aru1_1, aru1_2, aru2_1, aru2_2;
    QVector <QPointF> sh1_1, sh1_2, sh2_1, sh2_2;
    QVector <QPointF> er_crc1, er_crc2;
    QVector <QPointF> empty_cad1, empty_cad2;        // Кадры без маркера
    QVector <QPointF> vec_empty;
    QVector <QPointF> gen_izm1, gen_izm2, gen_izm_all;
    unsigned int num_empty;
    QPalette pal_stat, pal_crc, pal_tel;

    QVector <QPointF> one_chanel, two_chanel, both_chanel;        // по 1ому, по 2ому, по обоим каналам.
    QVector <Cadrs> Cad;
    
    QwtPlotCurve cur_stat1, cur_stat2, cur_gen_izm;
    QwtPlotCurve cur_empty1, cur_empty2, cur_empty_loss1, cur_empty_loss2;
    QwtPlotCurve cur_stat_crc1, cur_stat_crc2;
    QwtPlotCurve cur_er_crc1, cur_er_crc2;          // Для отрисовки потерь
    QwtPlotCurve cur_aru1_1, cur_aru1_2, cur_aru2_1, cur_aru2_2;
    QwtPlotCurve cur_sh1_1, cur_sh1_2, cur_sh2_1, cur_sh2_2;
    
    QwtPlotCurve cur_ercrc2, cur_ercrc2_2;
    QwtSymbol sym_stat1, sym_stat2, sym_gen_izm;
    QwtSymbol sym_ercrc1, sym_ercrc2;
    QwtSymbol sym_empty1, sym_empty2;

    int ifvirtK(qint32 nvk);    /// Проверка виртуального канала
    int ifmarker (bool s);      /// Проверка наличия маркера. s == false - нет маркера.
    int ifport();               /// Проверка порта
    void LoadFileCU( QString nameFile, QVector <QwtPointPolar> &vecCU );
//    void init_zona();
    void Record( QTextStream &out );
    void Record( QTextStream &out, CadrePPSI &ppsi, int c );
    void plot_all(QString nfile, qlonglong nz , int kan, QDateTime dt_begin, QDateTime dt_end);
    void init_ppsi(QVector<CadrePPSI> &ppsi, QByteArray buf, QTime tn, QTime tk);
    void init_ppsi_stat(QVector<CadrePPSI> &ppsi, QByteArray buf, QTime tn, QTime tk);
    void set_telemetry(QVector<CadrePPSI> &ppsi1, QVector<CadrePPSI> &ppsi2);
    void draw_stat();
    void draw_empty(int icon_space, int symb_size, QSize icon_size);
    void draw_telemetry();
    void rec_cadrs(QVector<CadrePPSI> &vec_ppsi, QStringList &lst_ppsi);

protected:
    virtual void resizeEvent( QResizeEvent * );
    virtual bool eventFilter(QObject *obj, QEvent *event );
    virtual void closeEvent( QCloseEvent *event);

private:
    Ui::ppsiClass ui;
    ZoomSiAr WidSiAr;
    StatVirKan StViK;
    WidErCRC WidCRC;
    StatFullFile st_ful;
    const qint64 len_cad = 576;     // Длина кадра (байт)

    void refresh();
    void clearWid();
    void plot_go(qlonglong nz , QString nfile, QDate date);
    void draw_polar_go();

private slots:
    void cheked_cbARU1();
    void cheked_cbARU2();
    void cheked_cbSH1();
    void cheked_cbSH2();
    void cheked_cbARU1_2();
    void cheked_cbARU2_2();
    void cheked_cbSH1_2();
    void cheked_cbSH2_2();
    void save_PPSI(QVector<CadrePPSI> &cadrs_ppsi, int kan);    /// Запись информации по отдельным каналам в файл
    void on_up_clicked();           /// Предыдущий кадр
    void on_dn_clicked();           /// Следующий кадр
    void on_nnZap_clicked();        /// Пропустить N кадров вперед
    void SkipNBack();               /// Пропустить N кадров назад
    bool read_file(qint32 nvk, bool pr_crc, QTime tm, bool s, qint32 aru1, qint32 aru2, qint32 sh1, qint32 sh2);    /// Запись данных по кадру в виджет
    void on_findmarker_clicked();   /// Поиск маркера
    void on_font_clicked();         /// Выбор шрифта
    void set_auto_scale();          /// Кнопка восстановления масштаба
    void save_stat();               /// Запись статистики в файл
};

#endif // PPSI_H
