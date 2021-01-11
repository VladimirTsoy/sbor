#include "fppsi.h"
#include "ui_fppsi.h"

FPPsi::FPPsi(QWidget *parent) : QWidget(parent), ui(new Ui::FPPsi){
    queryBetween = queryBetweenConst;
    queryNka = "";
    queryNpn = "";
    ui->setupUi(this);
    model = new QSqlQueryModel();
    model->setQuery(query);
    sqlSelect = "plppsi.nka, fullppsi.nka, fullppsi.naz,plppsi.godn,plppsi.npn, plppsi.sutn,plppsi.sekn AS sek, fullppsi.sekn";
    connect(ui->btn_Refresh, SIGNAL(clicked(bool)), SLOT(setQuerySet()));
    //connect(ui->groupBox_2, SIGNAL(toggled(bool)), SLOT(setQueryInterval()));
    QList<QRadioButton *> listGroupBox = ui->groupBox_2->findChildren<QRadioButton *>();
    group.setExclusive(true);                                                                                                       //Возможность выбрать только одно значение из группы кнопок
    for(int i = 0; i < listGroupBox.size(); i++){
        group.addButton(listGroupBox[i],i);
    }
    connect(&group, SIGNAL(buttonClicked(QAbstractButton *)),SLOT(setQuerySet()));
    connect(ui->cbNka, SIGNAL(activated(QString)),SLOT(setNka(QString)));
    ui->dateBegin->setDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate());
}


void FPPsi::forDay(){
    coI->kol_htable = 5;
    coI->htable[0] = tr("Год"); coI->hidcol[0] = true;
    coI->htable[1] = tr("Дата"); coI->hidcol[1] = false;
    coI->htable[2] = tr("Месяц"); coI->hidcol[2] = true;
    coI->htable[3] = tr("Запланировано");  coI->hidcol[3] = false;
    coI->htable[4] = tr("Выполнено"); coI->hidcol[4] = false;

    queryStr = queryBegin +" "+queryDaySelect +" "+queryBetween+queryNka+queryNpn+" "+queryDayGroupBy;
}

void FPPsi::forMonth(){
    coI->kol_htable = 4;
    coI->htable[0] = tr("Год"); coI->hidcol[0] = false;
    coI->htable[1] = tr("Месяц");  coI->hidcol[1] = false;
    coI->htable[2] = tr("Запланировано");  coI->hidcol[2] = false;
    coI->htable[3] = tr("Выполнено");  coI->hidcol[3] = false;

    queryStr = queryBegin +" "+ queryMonthSelect+" "+queryBetween+queryNka+queryNpn+" "+queryMonthGroupBy;

}

void FPPsi::forYear(){
    coI->kol_htable = 3;
    coI->htable[0] = tr("Год");coI->hidcol[0] = false;
    coI->htable[1] = tr("Запланировано"); coI->hidcol[1] = false;
    coI->htable[2] = tr("Выполнено"); coI->hidcol[2] = false;

    queryStr = queryBegin +" "+ queryYearSelect+" "+queryBetween+queryNka+queryNpn+" "+queryYearGroupBy;
}
void FPPsi::forAll(){
    coI->kol_htable = 10;
    coI->hidcol[0] = true;
    coI->htable[1] = tr("№ НПН");  coI->hidcol[1] = false;
    coI->htable[2] = tr("№ КА");   coI->hidcol[2] = false;
    coI->htable[3] = tr("Year");   coI->hidcol[3] = true;
    coI->htable[4] = tr("Дата");   coI->hidcol[4] = false;
    coI->htable[5] = tr("Месяц");  coI->hidcol[5] = true;
    coI->htable[6] = tr("Время");  coI->hidcol[6] = false;
    coI->htable[7] = tr("1 канал");coI->hidcol[7] = false;
    coI->htable[8] = tr("2 канал");coI->hidcol[8] = false;
    coI->hidcol[9] = true;


    queryStr = queryBegin +" "+queryAll+" "+queryBetween+queryNpn+queryNka;
}

void FPPsi::setNka(QString nka){
    if(nka == "все"){
        queryNka = "";
        setQuerySet();
        return;
    }
    queryNka = " AND tab.nka = " + nka;
    setQuerySet();
}

void FPPsi::setNpn(QString npn){
    if(npn == "все"){
        queryNpn = "";
        setQuerySet();
        return;
    }
    queryNpn = " AND tab.npn = " + npn;
    setQuerySet();
}

void FPPsi::setQuerySet(){
    list_npn=coI->SpisNPN("fullppsi","",true);
    ui->npnCombo->clear();
    ui->npnCombo->setMaxCount(list_npn.size());
    ui->npnCombo->addItems(list_npn);
    model->clear();
    queryBetween = queryBetweenConst;    //   Временной интервал по умолчанию

    if(ui->dateOn->isChecked()){        // Если производится выбор диапазона дат
        QDate dateBegin, dateEnd;
        dateBegin = ui->dateBegin->date();
        dateEnd = ui->dateEnd->date();

        QString dateB = dateBegin.toString("yyyy-MM-dd");
        QString dateE = dateEnd.toString("yyyy-MM-dd");
        queryBetween = "WHERE tab.date_plan BETWEEN '"+dateB+ "' AND '"+ dateE+"'";
    }

    if(ui->ppsi_all->isChecked()){
          forAll();
    }else if(ui->ppsi_day->isChecked()){
        forDay();
    }else if(ui->ppsi_month->isChecked()){
        forMonth();
    }else if(ui->ppsi_year->isChecked()){
        forYear();
    }else{
        queryStr = "SELECT * FROM plppsi";
    }
    coI->setQueryModel(model, ui->tableView, queryStr, true);
}


void FPPsi::showEvent(QShowEvent *event){
    setQuerySet();
//    coI->setQueryModel(model, ui->tableView, queryStr, true);
    event->accept();
}
