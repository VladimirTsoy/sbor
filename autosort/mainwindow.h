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

private:
    Ui::MainWindow *ui;
    bool pr_close;
    bool pr_stop;
    void closeEvent(QCloseEvent *event);
    void ObrZap();
    QTimer t_si;
    QList<qlonglong> list;
    QString strsql;
    bool DelFileM(QString nfile);

public slots:
    void Pass();
    void Start();
    void Cl();
    void Obr();
 };

#endif // MAINWINDOW_H
