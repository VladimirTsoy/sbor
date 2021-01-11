#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QtWidgets>
#include "obrizm.h"
#include "fpass.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    FPass *formPass;
    ObrIzm *cOI;
    QTimer t_si;
    QString name,pas,hst;

private:
    Ui::MainWindow *ui;
//    QString kname;
    void ReadUst();
    qlonglong nz_ust;
    void Run();
    bool pr_close;
//    bool pr_stop;
    void closeEvent(QCloseEvent *event);
    QIcon icon,icon_red,icon_green;

public slots:
    void Pass();
    void Start();
    void Obr();
    void Cl();
    void on_btnStop();
};

#endif // MAINWINDOW_H
