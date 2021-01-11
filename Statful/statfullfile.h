#ifndef STATFULLFILE_H
#define STATFULLFILE_H

#include "obrizm.h"
#include "dirfile.h"

using namespace std;

namespace Ui {
class StatFullFile;
}


class CadrePPSI
{
public:
    CadrePPSI();
    qint64 indSt;            // Начало заголовка кадра
    qint64 indSinxr;         // Индекс маркера синхронизации
    QTime T;                 // Время начала кадра
    qint32 NvirtKan;         // Номер виртуального канала
    bool em;                 // Наличие маркера синхронизации
    bool beaten;             // Контрольная сумма
    int count;

    unsigned short flopk;     //флаг наличия поля crc
    unsigned short ident;     //идентификатор объекта
    unsigned short nver;      //№ версии кадра передачи
    quint8 chkad;             // Счетчик кадров
    quint8 chvirtkan;         // Счетчик виртуальных каналов
    unsigned short ukzag;     // Указатель 1ого заголовка
    unsigned short iddlseg;   // Идентификатор длины сегмента
    unsigned short flocpak;   // Флаг очередности пакетов
    unsigned short flsinxr;   // Флаг синхронизации
    unsigned short vtzag;     // 2ой заголовок кадра передачи
    unsigned short identific; //идентификатор кадра передачи
    unsigned short ch;
    unsigned short status;

    qint32 ARU1, ARU2;
    int fract;                // Счетчик кадров (наземный)
    qint32 SiNoRa1, SiNoRa2;  // Отношение сигнал/шум
    int FrLen;                // Длина принятого кадра
    int RecFr;                // Счетчик записаных кадров
    int NumPort;              // Номер порта
    qint16 chek_sum;          // Контрольная сумма

    friend bool operator== ( const CadrePPSI& left, const CadrePPSI& right );
};


class Cadrs
{
public:
    Cadrs();
    int cad1, cad2;                         // 1-есть кадр, 0 - нет кадра
    qint32 NVirtKan1, NVirtKan2;            // Номера вирт. каналов
    QTime T1, T2;                           // Время начала кадров
    qint64 indSt1, indSt2;                  // Индекс начала кадра
    bool em1, em2;                          // Наличие маркера синхронизации
    bool cadr1, cadr2;                      // true - есть кадр, иначе false
    bool beaten1, beaten2;                  // Контрольная сумма
    quint8 chkad1, chkad2;                  // Счетчик кадров
    quint8 chvirtkan1, chvirtkan2;          // Счетчик виртуальных каналов
    friend bool operator== ( const Cadrs& left, const Cadrs& right );


    int delta_t;                            // Допуск по времени, мсек
};


class StatFullFile /*: public QWidget*/
{
//    Q_OBJECT

public:
//    explicit StatFullFile( QWidget *parent = 0 );
    StatFullFile();
    ~StatFullFile() {delete cOI;/* delete ui;*/}
    void ZapBuf();
    void Collation3(QVector<CadrePPSI> &cadrs1, QVector<CadrePPSI> &cadrs2 , QVector<Cadrs> &Cad);
    bool FindKadr(QVector<CadrePPSI> &cadrs, CadrePPSI &PPSI , int &indFind);
    bool FindKadr( qint64 indCur, QVector<CadrePPSI> &cadrs, int &indPrev, unsigned short &NVirtKan, QTime &T, bool &beaten );
    void build( QByteArray &buf1, QByteArray &buf2 , QVector<CadrePPSI> &PPSI1, QVector<CadrePPSI> &PPSI2 );
    void Record( QTextStream &out );
    void Record( QTextStream &out, CadrePPSI &ppsi, int c );
    void out_file(QVector<CadrePPSI> &cadrs1, QVector<CadrePPSI> &cadrs2);

    ObrIzm *cOI;
    int deltaT;                             // Допуск по времени, мсек
//private:
//    Ui::StatFullFile *ui;
};

#endif // STATFULLFILE_H
