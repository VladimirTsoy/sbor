#ifndef DIRFILE_H
#define DIRFILE_H

#include <QtWidgets>
#include <stdio.h>
#include <unistd.h>
#include <QTextStream>

class DirFile
{
public:
    DirFile();

    const unsigned short Crc16Table[256] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
    };

    typedef union
    {
     char byte[2];
     unsigned short byte2;
    } UnionByte2;

    typedef union
    {
     char byte[4];
     unsigned int byte4;
    } UnionByte4;

    typedef union
    {
     unsigned short byte;
     struct
     {
      unsigned bit1 : 1;
      unsigned bit2 : 1;
      unsigned bit3 : 1;
      unsigned bit4 : 1;
      unsigned bit5 : 1;
      unsigned bit6 : 1;
      unsigned bit7 : 1;
      unsigned bit8 : 1;
     } bits;
    } UnionByteU1;
    typedef union
    {
     unsigned short byte;
     struct
     {
      unsigned bit1 : 1;
      unsigned bit2 : 1;
      unsigned bit3 : 1;
      unsigned bit4 : 1;
      unsigned bit5 : 1;
      unsigned bit6 : 1;
      unsigned bit7 : 1;
      unsigned bit8 : 1;
     } bits;
    } UnionByteU1_1;

    typedef union
    {
     double byte_double;
     qlonglong byte8;
    } UnionByte8;

    UnionByte2 bt2;
    UnionByte4 bt4;
    UnionByteU1 btu1;
    UnionByteU1_1 btu1_1;
    UnionByte8 bt8;

    static uchar      C1(const char *p) {uchar       u; memcpy(&u,p,1); return u;}
    static qint8      I1(const char *p) {qint8       u; memcpy(&u,p,1); return u;}
    static qint16     I2(const char *p) {qint16      i; memcpy(&i,p,2); return i;}
    static qint32     I4(const char *p) {qint32      i; memcpy(&i,p,4); return i;}
    static float      R4(const char *p) {float       r; memcpy(&r,p,4); return r;}
    static double     R8(const char *p) {double      r; memcpy(&r,p,8); return r;}
    static quint8     U1(const char *p) {quint8      u; memcpy(&u,p,1); return u;}
    static quint16    U2(const char *p) {quint16     u; memcpy(&u,p,2); return u;}
    static quint32    U4(const char *p) {quint32     u; memcpy(&u,p,4); return u;}
    static qulonglong U8(const char *p) {qulonglong  u; memcpy(&u,p,8); return u;}

    //удалить файлы из директории
    bool ClearDir(QString path);

    //получить информацию о файле по номеру индекса в списке:
    //0-абсолютный путь к файлу, включая имя файла и расширение
    //1-путь к директории, где находится файл
    //2-название файла с расширением
    //3-название файла без расширения
    //4-название расширения, без разделителя "."
    //5-размер файла в байтах
    QStringList FileInfo(QString file);

    //получить расширение файла без разделителя "."
    QString FileExt(QString file);

    //получить список файлов в директории, включая вложенные в поддиректориях
    //список с полными путями
    QStringList SpisAllFile(QString dir);

    //получить список поддиректорий в директории
    QStringList SpisDir(QString dir);

    //получить список поддиректорий в директории (списке директорий)
    //директории с полными путями
    QStringList SpisPathDir(QString dir);
    QStringList SpisPathDir(QStringList spis_dir);

    //добавить содержимое списка 2 в список 1 с добавлением префикса к каждому элементу
    //возвращает количество добавленых элементов
    int List2ToList1(QStringList &list1,QStringList &list2,QString pref);

    qint32 List2ToList1NoEmpty(QStringList &list1, QStringList list2);

    //получить список всех поддиректорий со всеми вложениями с полными путями
    QStringList SpisAllDir(QString dir);

    //получить список файлов в директории
    QStringList SpisFile(QString dir);

    //получить список всех файлов в директории (списке директорий) с путями
    QStringList SpisPathFile(QString dir);
    QStringList SpisPathFile(QStringList spis_dir);

    //чтение текстового файла в текстовый список
    QStringList ReadFileInList(QString NFile);
    QStringList ReadFileInList(QString NFile,QString str);

    //возвращает true, если в dat установлен бит nom (справа, начиная с 1)
    bool getBit(qint8 dat, qint8 nom);

    QDate DayToDate(QString day, QString year);

    QDate DayToDate(int day, int year);

    QTime SecToTime(QString sec);

    QTime SecToTime(quint64 sec,QString &stime);

    QString DayToDateS(QString day,QString year);

    QString SecToTimeS(QString sec);

    bool WriteFileFromList(QStringList list, QString NFile);

    bool AppendFileFromList(QStringList list, QString nfile);

    bool isValidNameFile(QString &nfile);

    bool isValidNameFileN(QString &nfile);

    //архивирует/разархивирует файл
    //nfile - название файла с полным путём
    //pr - код операции 0-ничего не делать; 1-архивировать; 2-разархивировать
    //возвращает длину(новую длину) файла, КС, байтовый массив с содержимым(новым содержимым) файла
    bool ArxFile(QString nfile, qint8 pr, qulonglong &idlf, qint16 &iks, qulonglong &adlf, qint16 &aks, QByteArray &buf);

    bool ArxFileInMem(qint8 pr, qulonglong &idlf, qint16 &iks, qulonglong &adlf, qint16 &aks, QByteArray &buf);

    //читает весь файл в память (байтовый массив)
    QByteArray ReadFile(QString file);
    bool ReadFile(QString file,QByteArray &buf);
    bool ReadFile(QString file,QByteArray &buf,qint64 kolb);

    qint16 KS(QByteArray buf);

    //записывает файл с именем file из области памяти buf
    //при prud=true файл перезаписывает существующий
    //при ошибке возвращает -1
    int WriteFile(QString file, QByteArray buf, bool prud);
    qint64 WriteFile(QString file, QByteArray &buf, bool prud,QString &str_err);
    bool AppendFile(QString file, QByteArray &buf);

    void CompressBuf(QByteArray &buf, int level);

    void UncompressBuf(QByteArray &buf);

    //определяет № строки в списке с данными из par
    //при неудаче -1
    int FindStrInList(QStringList list, QString par);
    int FindStrInList(QStringList list,QString par,int ind,short reg);

    QDateTime SetDTFromStr(QString str);

    QStringList CopyList(QStringList list,int ind,int col);

    int AddList(QStringList &list,QStringList list1);
    int AddList(QList<qlonglong> &list,QList<qlonglong> list1);

    int AddMap(QMap<QString,QString> &map, QMap<QString,QString> map1, bool pr_multi);

    bool DelFile(QString nfile);

    bool DelFile(QStringList spis_file);

    void DateToDay(QDate dat,quint32 &day,quint16 &year);

    void DateToDay(QDate dat,qint32 &day,qint32 &year);

    quint32 TimeToSec(QTime time);

    void DelDublForList(QStringList &list);

    bool isVisYear(qint32 year);

    QBitArray BytesToBits(QByteArray bytes);

    unsigned short Crc16(unsigned char * pcBlock, unsigned short len);

    bool Crc8(QByteArray &buf, qint64 ind, qint64 len, uchar &crc);

    bool DelDublInList(QStringList &list);

    bool DelDublInList(QList<QString> &list);

    qint32 AddArray(QByteArray &buf1, QByteArray &buf2, qint32 ind);

    qreal TimeToHD(qint32 h, qint32 m, qint32 s);

    bool RenameFile(QString isxf,QString mask,QString &vixf);

    QString NameMask(QString name,QString mask);

    void Beep(int Hz,int msec);

    QString findString(QTextStream *_stream, QString _string);

    bool VariantIsDigit(QVariant var);

    QStringList MapToList(QMap<QString, QString> &map);

    void WriteLog(QString text);

    bool CopyErr(QString path_si,QString nfile);

    bool DelFileM(QString nfile);


};

#endif // DIRFILE_H
