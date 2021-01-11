#include "fstatfull.h"
#include "ui_fstatfull.h"

FStatFull::FStatFull(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FStatFull)
{
    ui->setupUi(this);
}

FStatFull::~FStatFull()
{
    delete ui;
}

void FStatFull::showEvent(QShowEvent *event)
{
 showForm();
 event->accept();
}

void FStatFull::showForm()
{
    QDate dt;
    dt.setDate(0,0,0);
    qint64 koli,kolo;

    ui->toolBox->setEnabled(true);
    ui->frame_ppsi1->setEnabled(true);
    ui->frame_ppsi2->setEnabled(true);
    ui->lab_nfile1->clear();
    ui->lab_nfile2->clear();
    ui->le_ns1->clear();
    ui->le_ns2->clear();
    ui->le_ks1->clear();
    ui->le_ks2->clear();
    ui->le_k1->clear();
    ui->le_k2->clear();
    ui->le_pk1->clear();
    ui->le_pk2->clear();
    ui->le_kvi1->clear();
    ui->le_kvi2->clear();
    ui->le_kvno1->clear();
    ui->le_kvno2->clear();
    ui->le_vrv11->clear();
    ui->le_vrv21->clear();
    ui->le_vrv12->clear();
    ui->le_vrv22->clear();
    ui->le_rez1->clear();
    ui->le_rez2->clear();
    ui->le_crc1->clear();
    ui->le_crc2->clear();
    ui->le_opk1->clear();
    ui->le_opk2->clear();
    ui->le_okvi1->clear();
    ui->le_okvi2->clear();
    ui->le_okvno1->clear();
    ui->le_okvno2->clear();
    ui->le_okvrv11->clear();
    ui->le_okvrv21->clear();
    ui->le_okvrv12->clear();
    ui->le_okvrv22->clear();
    ui->le_orez1->clear();
    ui->le_orez2->clear();
    ui->le_izmkvi1->clear();
    ui->le_izmkvi2->clear();
    ui->le_ddskvi1->clear();
    ui->le_ddskvi2->clear();
    ui->le_crcddskvi1->clear();
    ui->le_crcddskvi2->clear();
    ui->le_ochddskvi1->clear();
    ui->le_ochddskvi2->clear();
    ui->le_zapddskvi1->clear();
    ui->le_zapddskvi2->clear();
    ui->le_ddskvno1->clear();
    ui->le_ddskvno2->clear();
    ui->le_crcddskvno1->clear();
    ui->le_crcddskvno2->clear();
    ui->le_ochddskvno1->clear();
    ui->le_ochddskvno2->clear();
    ui->le_zapddskvno1->clear();
    ui->le_zapddskvno2->clear();
    ui->le_k1vrv1->clear();
    ui->le_k1vrv2->clear();
    ui->le_k2vrv1->clear();
    ui->le_k2vrv2->clear();
    ui->le_crc1vrv1->clear();
    ui->le_crc1vrv2->clear();
    ui->le_crc2vrv1->clear();
    ui->le_crc2vrv2->clear();
    ui->le_och1vrv1->clear();
    ui->le_och1vrv2->clear();
    ui->le_och2vrv1->clear();
    ui->le_och2vrv2->clear();
    ui->le_kvrv->clear();
    ui->le_kbm1->clear();
    ui->le_kbm2->clear();
    ui->le_kcal->clear();
    ui->dn_kvi1->clear();
    ui->dn_kvi2->clear();
    ui->dn_kvno1->clear();
    ui->dn_kvno2->clear();
    ui->dn_vrv1->clear();
    ui->dn_vrv2->clear();
    ui->dk_kvi1->clear();
    ui->dk_kvi2->clear();
    ui->dk_kvno1->clear();
    ui->dk_kvno2->clear();
    ui->dk_vrv1->clear();
    ui->dk_vrv2->clear();
    ui->le_proc1->clear();
    ui->le_proc2->clear();

    if ( !cOI->ReadFullppsi(nz_full,true) )
    {
        ui->toolBox->setEnabled(false);
        return;
    }

    if ( cOI->zapfull.kan == 2 )
    {
        nz_full2 = nz_full;
        nz_full = cOI->FindRowFull(cOI->zapfull.godn,cOI->zapfull.sutn,cOI->zapfull.sekn,cOI->zapfull.nka,1,true); //поиск записи для 1 канала

        if ( !cOI->ReadFullppsi(nz_full,true) )
        {
            ui->toolBox->setEnabled(false);
            return;
        }
    }

    if ( cOI->zapfull.kan == 1 )
    {
        nz_full2 = cOI->FindRowFull(cOI->zapfull.godn,cOI->zapfull.sutn,cOI->zapfull.sekn,cOI->zapfull.nka,2,true); //поиск записи для 2 канала
    }

    if ( nz_full > 0 )
    {
        if ( !cOI->ReadStatFull(nz_full,true) )
        {
            ui->toolBox->setEnabled(false);
            return;
        }

        ui->lab_nfile1->setText(cOI->zapfull.naz);
        ui->le_ns1->setText(cOI->zapfull.dtn);
        ui->le_ks1->setText(cOI->zapfull.dtk);
        //  ui->le_k1->setText(QString::number(cOI->statfull.k));
        ui->le_k1->setText(QString::number(cOI->statfull.k+cOI->statfull.kbm));
        ui->le_pk1->setText(QString::number(cOI->statfull.pk));
        ui->le_kvi1->setText(QString::number(cOI->statfull.kvi));
        ui->le_kvno1->setText(QString::number(cOI->statfull.kvno));
        ui->le_vrv11->setText(QString::number(cOI->statfull.vrv1));
        ui->le_vrv21->setText(QString::number(cOI->statfull.vrv2));
        ui->le_rez1->setText(QString::number(cOI->statfull.rez));
        ui->le_crc1->setText(QString::number(cOI->statfull.crc));
        ui->le_opk1->setText(QString::number(cOI->statfull.opk));
        ui->le_okvi1->setText(QString::number(cOI->statfull.okvi));
        ui->le_okvno1->setText(QString::number(cOI->statfull.okvno));
        ui->le_okvrv11->setText(QString::number(cOI->statfull.ovrv1));
        ui->le_okvrv21->setText(QString::number(cOI->statfull.ovrv2));
        ui->le_orez1->setText(QString::number(cOI->statfull.orez));
        ui->le_izmkvi1->setText(QString::number(cOI->statfull.izm_kvi));

        ui->le_ddskvi1->setText(QString::number(cOI->statfull.k_kvi));
        ui->le_crcddskvi1->setText(QString::number(cOI->statfull.crc_kvi));
        ui->le_ochddskvi1->setText(QString::number(cOI->statfull.och_kvi));
        ui->le_zapddskvi1->setText(QString::number(cOI->statfull.k_kvi-(cOI->statfull.crc_kvi+cOI->statfull.och_kvi)));
        ui->le_ddskvno1->setText(QString::number(cOI->statfull.k_kvno));
        ui->le_crcddskvno1->setText(QString::number(cOI->statfull.crc_kvno));
        ui->le_ochddskvno1->setText(QString::number(cOI->statfull.och_kvno));
        ui->le_zapddskvno1->setText(QString::number(cOI->statfull.k_kvno-(cOI->statfull.crc_kvno+cOI->statfull.och_kvno)));
        ui->le_kbm1->setText(QString::number(cOI->statfull.kbm));

        ui->le_k1vrv1->setText(QString::number(cOI->statfull.k1_vrv1));
        ui->le_k2vrv1->setText(QString::number(cOI->statfull.k2_vrv1));
        ui->le_crc1vrv1->setText(QString::number(cOI->statfull.crc1_vrv1));
        ui->le_crc2vrv1->setText(QString::number(cOI->statfull.crc2_vrv1));
        ui->le_och1vrv1->setText(QString::number(cOI->statfull.och1_vrv1));
        ui->le_och2vrv1->setText(QString::number(cOI->statfull.och2_vrv1));
        ui->le_kvrv->setText(QString::number(cOI->statfull.k_vrv));
        ui->le_kcal->setText(QString::number(cOI->statfull.k_cal));

        ui->dn_kvi1->setText(cOI->statfull.dn_kvi.toString("dd-MM-yyyy"));
        ui->dk_kvi1->setText(cOI->statfull.dk_kvi.toString("dd-MM-yyyy"));
        ui->dn_kvno1->setText(cOI->statfull.dn_kvno.toString("dd-MM-yyyy"));
        ui->dk_kvno1->setText(cOI->statfull.dk_kvno.toString("dd-MM-yyyy"));
        ui->dn_vrv1->setText(cOI->statfull.dn_vrv.toString("dd-MM-yyyy"));
        ui->dk_vrv1->setText(cOI->statfull.dk_vrv.toString("dd-MM-yyyy"));

        koli=cOI->statfull.k+cOI->statfull.kbm;
        kolo=cOI->statfull.kbm+cOI->statfull.crc;
        if (koli == 0) ui->le_proc1->setText("0");
        else ui->le_proc1->setText(QString::number(100-((kolo*100)/koli)));
    }

    if ( nz_full2 > 0 )
    {
        if ( !cOI->ReadFullppsi(nz_full2,true) )
        {
            ui->frame_ppsi2->setEnabled(false);
            return;
        }

        if ( !cOI->ReadStatFull(nz_full2,true) )
        {
            ui->frame_ppsi2->setEnabled(false);
            return;
        }

        ui->lab_nfile2->setText(cOI->zapfull.naz);
        ui->le_ns2->setText(cOI->zapfull.dtn);
        ui->le_ks2->setText(cOI->zapfull.dtk);
        //  ui->le_k2->setText(QString::number(cOI->statfull.k));
        ui->le_k2->setText(QString::number(cOI->statfull.k+cOI->statfull.kbm));
        ui->le_pk2->setText(QString::number(cOI->statfull.pk));
        ui->le_kvi2->setText(QString::number(cOI->statfull.kvi));
        ui->le_kvno2->setText(QString::number(cOI->statfull.kvno));
        ui->le_vrv12->setText(QString::number(cOI->statfull.vrv1));
        ui->le_vrv22->setText(QString::number(cOI->statfull.vrv2));
        ui->le_rez2->setText(QString::number(cOI->statfull.rez));
        ui->le_crc2->setText(QString::number(cOI->statfull.crc));
        ui->le_opk2->setText(QString::number(cOI->statfull.opk));
        ui->le_okvi2->setText(QString::number(cOI->statfull.okvi));
        ui->le_okvno2->setText(QString::number(cOI->statfull.okvno));
        ui->le_okvrv12->setText(QString::number(cOI->statfull.ovrv1));
        ui->le_okvrv22->setText(QString::number(cOI->statfull.ovrv2));
        ui->le_orez2->setText(QString::number(cOI->statfull.orez));
        ui->le_izmkvi2->setText(QString::number(cOI->statfull.izm_kvi));

        ui->le_ddskvi2->setText(QString::number(cOI->statfull.k_kvi));
        ui->le_crcddskvi2->setText(QString::number(cOI->statfull.crc_kvi));
        ui->le_ochddskvi2->setText(QString::number(cOI->statfull.och_kvi));
        ui->le_zapddskvi2->setText(QString::number(cOI->statfull.k_kvi-(cOI->statfull.crc_kvi+cOI->statfull.och_kvi)));
        ui->le_ddskvno2->setText(QString::number(cOI->statfull.k_kvno));
        ui->le_crcddskvno2->setText(QString::number(cOI->statfull.crc_kvno));
        ui->le_ochddskvno2->setText(QString::number(cOI->statfull.och_kvno));
        ui->le_zapddskvno2->setText(QString::number(cOI->statfull.k_kvno-(cOI->statfull.crc_kvno+cOI->statfull.och_kvno)));
        ui->le_kbm2->setText(QString::number(cOI->statfull.kbm));

        ui->le_k1vrv2->setText(QString::number(cOI->statfull.k1_vrv2));
        ui->le_k2vrv2->setText(QString::number(cOI->statfull.k2_vrv2));
        ui->le_crc1vrv2->setText(QString::number(cOI->statfull.crc1_vrv2));
        ui->le_crc2vrv2->setText(QString::number(cOI->statfull.crc2_vrv2));
        ui->le_och1vrv2->setText(QString::number(cOI->statfull.och1_vrv2));
        ui->le_och2vrv2->setText(QString::number(cOI->statfull.och2_vrv2));
        ui->le_kvrv->setText(QString::number(cOI->statfull.k_vrv));
        ui->le_kcal->setText(QString::number(cOI->statfull.k_cal));

        ui->dn_kvi2->setText(cOI->statfull.dn_kvi.toString("dd-MM-yyyy"));
        ui->dk_kvi2->setText(cOI->statfull.dk_kvi.toString("dd-MM-yyyy"));
        ui->dn_kvno2->setText(cOI->statfull.dn_kvno.toString("dd-MM-yyyy"));
        ui->dk_kvno2->setText(cOI->statfull.dk_kvno.toString("dd-MM-yyyy"));
        ui->dn_vrv2->setText(cOI->statfull.dn_vrv.toString("dd-MM-yyyy"));
        ui->dk_vrv2->setText(cOI->statfull.dk_vrv.toString("dd-MM-yyyy"));

        koli = cOI->statfull.k + cOI->statfull.kbm;
        kolo = cOI->statfull.kbm + cOI->statfull.crc;

        if (koli == 0)
            ui->le_proc2->setText("0");
        else
            ui->le_proc2->setText(QString::number(100-((kolo*100)/koli)));
    }

    return;
}
