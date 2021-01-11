#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
//#include <algorithm>
#include "statfullfile.h"

using namespace testing;
TEST(ident_kadrs, ident_equal1)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t(1, 11, 11, 111);

    cadr1.T = t;            cadr2.T = t;
    cadr1.beaten = true;    cadr2.beaten = true;
    cadr1.em = true;        cadr2.em = true;
    cadr1.NvirtKan = 0;     cadr2.NvirtKan = 0;
    cadr1.chkad = 0;        cadr2.chkad = 0;
    cadr1.chvirtkan = 0;    cadr2.chvirtkan = 0;

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 1;                       cad.cad2 = 1;
    cad.cadr1 = true;                   cad.cadr2 = true;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;

    ppsi_vec1.push_back(cadr1);
    ppsi_vec2.push_back(cadr2);
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    ASSERT_THAT(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, ident_equal2)
{
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(1, 11, 11, 111), t2(1, 11, 11, 113);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 1;             cadr2.NvirtKan = 1;
    cadr1.chkad = 1;                cadr2.chkad = 1;
    cadr1.chvirtkan = 1;            cadr2.chvirtkan = 1;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 1;                       cad.cad2 = 1;
    cad.cadr1 = true;                   cad.cadr2 = true;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    ASSERT_THAT(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, ident_equal3)
{
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(2, 22, 22, 222), t2(2, 22, 22, 225);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 2;             cadr2.NvirtKan = 2;
    cadr1.chkad = 10;               cadr2.chkad = 10;
    cadr1.chvirtkan = 255;          cadr2.chvirtkan = 255;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 1;                       cad.cad2 = 1;
    cad.cadr1 = true;                   cad.cadr2 = true;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    ASSERT_THAT(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal1)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t(1, 11, 11, 111);

    cadr1.T = t;                    cadr2.T = t;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 1;             cadr2.NvirtKan = 0;
    cadr1.chkad = 0;                cadr2.chkad = 0;
    cadr1.chvirtkan = 0;            cadr2.chvirtkan = 0;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal2)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t(1, 11, 11, 111);

    cadr1.T = t;                    cadr2.T = t;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 1;             cadr2.NvirtKan = 1;
    cadr1.chkad = 0;                cadr2.chkad = 1;
    cadr1.chvirtkan = 0;            cadr2.chvirtkan = 0;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr1.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr1.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal3)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(1, 11, 11, 111), t2(1, 11, 11, 112);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 1;             cadr2.NvirtKan = 1;
    cadr1.chkad = 0;                cadr2.chkad = 1;
    cadr1.chvirtkan = 0;            cadr2.chvirtkan = 0;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal4)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(1, 11, 11, 111), t2(1, 11, 11, 113);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = false;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal5)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(1, 11, 11, 111), t2(1, 11, 11, 113);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = false;               cadr2.em = true;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal6)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(1, 11, 11, 111), t2(1, 11, 11, 113);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = false;           cadr2.beaten = false;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}


TEST(ident_kadrs, not_equal7)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(15, 23, 55, 0), t2(15, 23, 55, 3);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = false;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal8)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(15, 23, 55, 0), t2(15, 23, 55, 3);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = false;               cadr2.em = true;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal9)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(15, 23, 55, 0), t2(15, 23, 55, 3);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = false;               cadr2.em = false;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 21;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}

TEST(ident_kadrs, not_equal10)
{
//    qDebug() << "\x1b[5;42;31m Test \x1b[0m" << endl;
    /// Arrange
    CadrePPSI cadr1, cadr2;
    Cadrs cad;
    QVector<CadrePPSI> ppsi_vec1, ppsi_vec2;
    QVector<Cadrs> cadrs_receiv, cadrs_exec;
    StatFullFile *st = new StatFullFile;
    QTime t1(15, 23, 55, 0), t2(15, 23, 55, 3);

    cadr1.T = t1;                   cadr2.T = t2;
    cadr1.beaten = true;            cadr2.beaten = true;
    cadr1.em = true;                cadr2.em = true;
    cadr1.NvirtKan = 5;             cadr2.NvirtKan = 5;
    cadr1.chkad = 20;               cadr2.chkad = 20;
    cadr1.chvirtkan = 21;           cadr2.chvirtkan = 22;
    ppsi_vec1.push_back(cadr1);     ppsi_vec2.push_back(cadr2);

    cad.T1 = cadr1.T;                   cad.T2 = cadr1.T;
    cad.cad1 = 1;                       cad.cad2 = 0;
    cad.cadr1 = true;                   cad.cadr2 = false;
    cad.em1 = cadr1.em;                 cad.em2 = cadr1.em;
    cad.beaten1 = cadr1.beaten;         cad.beaten2 = cadr1.beaten;
    cad.chkad1 = cadr1.chkad;           cad.chkad2 = cadr1.chkad;
    cad.chvirtkan1 = cadr1.chvirtkan;   cad.chvirtkan2 = cadr1.chvirtkan;
    cad.NVirtKan1 = cadr1.NvirtKan;     cad.NVirtKan2 = cadr1.NvirtKan;
    cadrs_exec.push_back(cad);

    cad.T1 = cadr2.T;                   cad.T2 = cadr2.T;
    cad.cad1 = 0;                       cad.cad2 = 1;
    cad.cadr1 = false;                  cad.cadr2 = true;
    cad.em1 = cadr2.em;                 cad.em2 = cadr2.em;
    cad.beaten1 = cadr2.beaten;         cad.beaten2 = cadr2.beaten;
    cad.chkad1 = cadr2.chkad;           cad.chkad2 = cadr2.chkad;
    cad.chvirtkan1 = cadr2.chvirtkan;   cad.chvirtkan2 = cadr2.chvirtkan;
    cad.NVirtKan1 = cadr2.NvirtKan;     cad.NVirtKan2 = cadr2.NvirtKan;
    cadrs_exec.push_back(cad);

    /// Act
    st->Collation3(ppsi_vec1, ppsi_vec2, cadrs_receiv);

    /// Assert
    EXPECT_NE(cadrs_exec, cadrs_receiv);
}
