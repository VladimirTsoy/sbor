#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "obrizm.h"
#include "fpass.h"
#include "fpolz.h"
#include "fust.h"
#include "fds.h"
#include "fkos.h"
#include "frnxi.h"
#include "frnxv.h"
#include "ffull.h"
#include "fdds.h"
#include "fvrv.h"
#include "fpotr.h"
#include "fzapr.h"
#include "fseansds.h"
#include "fseansppsi.h"
#include "fseansdds.h"
#include "fseansvrv.h"
#include "fkolseansds.h"
#include "fserv.h"
#include "fstatserv.h"
#include "fsba.h"
#include "fsist.h"
#include "frpom.h"
#include "fnosd.h"
#include "fppsi.h"
#include "fustnf.h"
#include "fcogi.h"
#include "fcup.h"
#include "fautozapr.h"
#include "fsp3.h"
#include "fsol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ObrIzm *cOI;
    FPass *formPass;
    FPolz *formPolz;
    FUst *formUst;
    FDs *formDs;
    FKos *formKos;
    FRnxi *formRnxi;
    FRnxv *formRnxv;
    FFull *formFull;
    FDds *formDds;
    FVrv *formVrv;
    FPotr *formPotr;
    FZapr *formZapr;
    FSeansDs *formSeansDs;
    FSeansPpsi *formSeansPpsi;
    FSeansDds *formSeansDds;
    FSeansVrv *formSeansVrv;
    FKolSeansDs *formKolSeansDs;
    FServ *formServ;
    FStatServ *formStatServ;
    FSba *formSba;
    FSist *formSist;
    FRPom *formRpom;
    FNosd *formNosd;
    FPPsi *formPpsi;
    FUstnf *formUstnf;
    FCogi *formCogi;
    FCup *formCup;
    FAutoZapr *formAutoZapr;
    FSp3 *formSp3;
    FSol *formSol;
private:
    Ui::MainWindow *ui;
    qlonglong nz_ust;
    qlonglong nz_serv;
    qlonglong nz_ustnf;
    void closeEvent(QCloseEvent *event);

public slots:
    void Pass();
    void Start();
    void CreateBd();
    void ShowFNos();
    void ShowFPolz();
    void ShowFUst();
    void ShowFDs();
    void ShowFDzs();
    void ShowFKos();
    void ShowBdsi();
    void ShowBdsv();
    void ShowMeteo();
    void ShowFFull();
    void ShowFDds();
    void ShowFVrv();
    void ShowFPotr();
    void ShowFZapr();
    void ShowFSeansDs();
    void ShowFSeansDzs();
    void ShowFSeansPpsi();
    void ShowFSeansDds();
    void ShowFSeansVrv();
    void ShowFKolSeansDs();
    void ShowFKolSeansDzs();
    void ShowFServ();
    void ShowFStatServ();
    void ShowFSba();
    void ShowFSpo();
    void ShowFSist();
    void ShowFRpom();
    void StartSbor();
    void StartSort();
    void ShowFPpsi();
    void ShowFUstnf();
    void StartForm();
    void ShowFCogi();
    void ShowFCup();
    void ShowFAutoZapr();
    void ShowFSp3();
    void ShowFSol();

};

#endif // MAINWINDOW_H
