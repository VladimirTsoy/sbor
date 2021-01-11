#ifndef FTXTFILEVIEW_H
#define FTXTFILEVIEW_H

#include <QWidget>
#include "obrizm.h"

namespace Ui {
class FTxtFileView;
}

class FTxtFileView : public QWidget
{
    Q_OBJECT

public:
    explicit FTxtFileView(QWidget *parent = 0);
    ~FTxtFileView();
    qint64 nz;
    QString ntable;
    ObrIzm *cOI;
    QString nfile;
    QStringList t_list;
    QStringList list;
    QStringListModel *model;

private:
    Ui::FTxtFileView *ui;
    void SetModel();
    qint32 kolstr;
    qint32 tekstr;

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void on_btnZagr();
    void on_btnNum();
    void on_btnNext();
    void on_btnBack();
    void on_btnStr();
};

#endif // FTXTFILEVIEW_H
