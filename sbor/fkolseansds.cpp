#include "fkolseansds.h"
#include "ui_fkolseansds.h"

FKolSeansDs::FKolSeansDs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FKolSeansDs)
{
    ui->setupUi(this);

    model=new QSqlQueryModel(this);
    filtr.clear();
    tip.clear();
    ui->de_ns->setDate(QDate::currentDate());
    ui->de_ks->setDate(QDate::currentDate());

    connect(ui->btnRefresh,SIGNAL(clicked(bool)),this,SLOT(on_btnRefresh()));
    connect(ui->btnExit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->cb_ns,SIGNAL(clicked(bool)),this,SLOT(Obr_cb_ns()));
    connect( ui->cb_ns, SIGNAL(clicked(bool)), this, SLOT(on_btnRefresh()) );
    connect( ui->rbSut, SIGNAL(clicked(bool)), this, SLOT(on_btnRefresh()) );
    connect( ui->rbMes,  SIGNAL(clicked(bool)), this, SLOT(on_btnRefresh()));
    connect( ui->rbGod,  SIGNAL(clicked(bool)), this, SLOT(on_btnRefresh()));
    connect( ui->cbNka,  SIGNAL(currentIndexChanged(int)), this, SLOT(on_btnRefresh()) );
    connect( ui->cbNpn,  SIGNAL(currentIndexChanged(int)), this, SLOT(on_btnRefresh()) );
//    connect(ui->btnFiltr,SIGNAL(clicked(bool)),this,SLOT(on_btnFiltr()));
}

FKolSeansDs::~FKolSeansDs()
{
    delete ui;
}

void FKolSeansDs::showEvent(QShowEvent *event)
{
    if (tip == "DS") this->setWindowTitle("Количество проведённых сеансов ДС");
    if (tip == "DZS") this->setWindowTitle("Количество проведённых сеансов ДЗС");
    filtr.clear();
    model->clear();
    Obr_cb_ns();

    list_npn=cOI->SpisNPN("pla","",true);
    list_npn.push_front("Все");
    ui->cbNpn->clear();
    ui->cbNpn->setMaxCount(list_npn.count());
    ui->cbNpn->addItems(list_npn);
    on_btnRefresh();
    event->accept();
}

void FKolSeansDs::FormSpis1()
{
    QString  tipstat;

    tipstat.clear();
    if (tip == "DS")  tipstat="stat313";
    if (tip == "DZS") tipstat="stat322";

    str_query = " WITH spis1 AS ( SELECT nz, naz, npn, nka, godn, sutn, sekn, dtn::timestamp, nazis FROM ds \
                  WHERE nazis ILIKE '" + tip + "' ";

                /// Отбор КА и НПН для выполненных измерений
    if( ui->cbNpn->currentIndex() != 0 )
    {
        str_query += " AND npn ILIKE '" + ui->cbNpn->currentText() + "%' " ;
    }

    if( ui->cbNka->currentIndex() !=0 )
    {
        str_query += " AND nka ILIKE '" + ui->cbNka->currentText() + "%' ";
    }

            /// Выбор временного интревала для выполненных измерений
    if ( ui->cb_ns->isChecked() )
    {
        str_query += " AND ( dtn::timestamp BETWEEN ' " + ui->de_ns->date().toString("dd-MM-yyyy") + " ' AND ' " + ui->de_ks->date().toString("dd-MM-yyyy") + " ' ) ), ";
    }

    else
    {
        str_query += "), ";
    }

    str_query += " spis2 AS ( SELECT naz, nka, npn, dat, vrem," + tip + " AS syst FROM pla WHERE nz IS NOT NULL ";

            /// Отбор КА и НПН для планов
    if( ui->cbNpn->currentIndex() != 0 )
    {
        str_query += " AND (npn ILIKE '" + ui->cbNpn->currentText() + "%' )" ;
    }

    if( ui->cbNka->currentIndex() !=0 )
    {
        str_query += " AND (nka ILIKE '" + ui->cbNka->currentText() + "%' )";
    }

            /// Выбор временного интревала для планов
    if ( ui->cb_ns->isChecked() )
    {
        str_query += " AND ( dat BETWEEN ' " + ui->de_ns->date().toString("yyyy-MM-dd") + " ' AND ' " + ui->de_ks->date().toString("yyyy-MM-dd") + " ' ) ), ";
    }

    else
    {
        str_query += "), ";
    }
}

void FKolSeansDs::spis_gr_sut() //общий список количества сеансов по суткам
{

    str_query += " IzmSut  AS ( SELECT npn, nka, godn, sutn, count(sutn) AS Real_Amount FROM spis1 GROUP BY npn, nka, godn, sutn ORDER BY npn, nka, godn, sutn  ),     \
                                                                                                                                                                    \                                                                                                                                                                                            \
                   PlanSut AS ( SELECT npn, nka, EXTRACT(year from dat) AS god, EXTRACT( doy from dat) AS sut, sum( syst ) AS Plan_Amount FROM spis2                \
                                GROUP BY  nka, npn, god, sut, syst                                                                                                  \
                                ORDER BY nka, npn, god, sut),                                                                                                       \
                                                                                                                                                                    \
                   Result AS (SELECT DISTINCT p.nka, p.npn, p.god, p.sut , p.Plan_Amount, i.Real_amount FROM PlanSut p                                              \
                              LEFT OUTER JOIN IzmSut i ON i.nka=p.nka AND ( i.npn ILIKE ( p.npn || '%' ) ) AND p.god=i.godn AND p.sut=i.sutn )                      \
                                                                                                                                                                    \
                   SELECT npn, nka, ( SELECT make_date(god::integer,1,1) + (sut::integer-1)*integer'1' ) AS dat, Plan_Amount,                                       \
                   ( CASE WHEN ( Real_Amount IS NULL ) THEN '0'                                                                                                     \
                     ELSE Real_Amount END ) AS Real_Amount FROM Result                                                                                              \
                   ORDER BY npn, nka, dat; ";

    cOI->kol_htable=5;
    cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("Дата проведения сеансов"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("Количество запланированных сеансов"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Количество проведенных сеансов"); cOI->hidcol[4]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str_query,true);
    ui->tableView->setCurrentIndex(ind);
}

void FKolSeansDs::on_btnRefresh()
{
    QDate dn,dk;

    dn.setDate(1900,1,1);
    dk.setDate(2999,1,1);

    if (ui->cb_ns->isChecked())
    {
        dn=ui->de_ns->date();
        dk=ui->de_ks->date();
    }

    FormSpis1();
    if (ui->rbSut->isChecked()) spis_gr_sut();
    if (ui->rbMes->isChecked()) spis_gr_mes();
    if (ui->rbGod->isChecked()) spis_gr_god();
}

void FKolSeansDs::spis_gr_mes()
{
    str_query += " IzmMes AS (SELECT npn, nka, godn As god, EXTRACT(MONTH FROM dtn) AS mes, count(*) AS Real_Amount FROM spis1                         \
                   GROUP BY npn, nka, godn, mes ),                                                                                                     \
                                                                                                                                                       \
                   PlanMes AS ( SELECT npn, nka, EXTRACT(year from dat) AS god, EXTRACT( month from dat) AS mes, sum(syst) AS Plan_Amount FROM spis2   \
                                GROUP BY nka, npn, god, mes, syst ),                                                                                   \
                                                                                                                                                       \
                   Result AS (SELECT DISTINCT p.npn, p.nka, p.god, p.mes, p.Plan_Amount, i.Real_amount FROM PlanMes p                                  \
                   LEFT OUTER JOIN IzmMes i ON i.nka=p.nka AND ( i.npn ILIKE ( p.npn || '%' ) ) AND p.god=i.god AND p.mes=i.mes )                      \
                                                                                                                                                       \
                   SELECT npn, nka, god, mes, Plan_Amount,                                                                                             \
                   ( CASE WHEN ( Real_Amount IS NULL ) THEN '0'                                                                                        \
                     ELSE Real_Amount END ) AS Real_Amount FROM Result                                                                                 \
                   ORDER BY npn, nka, god, mes; ";

    cOI->kol_htable=6;
    cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("Год"); cOI->hidcol[2]=false;
    cOI->htable[3]=tr("Месяц"); cOI->hidcol[3]=false;
    cOI->htable[4]=tr("Количество запланированных сеансов"); cOI->hidcol[4]=false;
    cOI->htable[5]=tr("Количество проведенных сеансов"); cOI->hidcol[5]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str_query,true);
    ui->tableView->setCurrentIndex(ind);

}

void FKolSeansDs::spis_gr_god()
{
    str_query += " IzmGod AS ( SELECT npn, nka, godn AS god, count(godn) AS Real_Amount FROM spis1 GROUP BY npn,nka,godn ORDER BY godn,npn,nka ),                   \
                                                                                                                                                                    \
                   PlanGod AS ( SELECT npn, nka, EXTRACT(year from dat) AS god, sum(syst) AS Plan_Amount FROM spis2                                                 \
                                GROUP BY npn, nka, god, syst ),                                                                                                     \
                                                                                                                                                                    \
                   Result AS ( SELECT DISTINCT PlanGod.npn, PlanGod.nka, PlanGod.god, PlanGod.Plan_Amount, IzmGod.Real_Amount FROM PlanGod                          \
                               LEFT OUTER JOIN IzmGod ON ( IzmGod.npn ILIKE ( PlanGod.npn || '%' ) ) AND PlanGod.nka=IzmGod.nka AND PlanGod.god=IzmGod.god )        \
                                                                                                                                                                    \
                   SELECT npn, nka, god, Plan_Amount,                                                                                                               \
                   ( CASE WHEN ( Real_Amount IS NULL ) THEN '0'                                                                                                     \
                     ELSE Real_Amount END ) AS Real_Amount FROM Result                                                                                              \
                   ORDER BY npn, nka, god; ";

    cOI->kol_htable=5;
    cOI->htable[0]=tr("№ НПН"); cOI->hidcol[0]=false;
    cOI->htable[1]=tr("№ КА"); cOI->hidcol[1]=false;
    cOI->htable[2]=tr("Год"); cOI->hidcol[3]=false;
    cOI->htable[3]=tr("Количество запланированных сеансов"); cOI->hidcol[4]=false;
    cOI->htable[4]=tr("Количество проведенных сеансов"); cOI->hidcol[4]=false;
    ind=model->index(0,0);
    cOI->setQueryModel(model,ui->tableView,str_query,true);
    ui->tableView->setCurrentIndex(ind);
}

void FKolSeansDs::Obr_cb_ns()
{
    if (ui->cb_ns->isChecked())
    {
        ui->de_ns->setEnabled(true);
        ui->de_ks->setEnabled(true);
    }
    else
    {
        ui->de_ns->setEnabled(false);
        ui->de_ks->setEnabled(false);
    }
}

//void FKolSeansDs::on_btnFiltr()
//{
//    QString zn;

//    if (ui->btnFiltr->isChecked())
//    {
//        ind=cOI->getCurrentIndex(ui->tableView,model,0,zn);

//        if (zn.isEmpty())
//        {
//            ui->btnFiltr->setChecked(false);
//            return;
//        }

//        filtr=zn;
//    }

//    else
//    {
//        filtr.clear();
//    }

//    on_btnRefresh();
//}
