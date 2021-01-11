#ifndef OBRIZM_H
#define OBRIZM_H

#include <QtWidgets>
#include <QHostInfo>
//#include "rinexfile.h"
#include "classbd.h"
//#include "epoch.h"
#include "converterjps.h"
#include "integritycontrol.h"
#include "renexwriter.h"
#include "compressor.h"
#include "converter.h"

#define PI 3.14159265358979323846264338327950
#define date1980             QDate(1980, 1, 6)
#define dateJuli             date1980.toJulianDay()
//#define time0			     QTime(0, 0, 0)

class ObrIzm : public ClassBd
{

public:
    ObrIzm();

    typedef union
    {
     char buf[6];
     struct
     {
      unsigned short ident; //идентификатор кадра передачи
      unsigned short ch;
      unsigned short status;
     }bt;
     struct
     {
      unsigned short flopk:1;//флаг наличия поля crc
      unsigned short nvirtkan:3;//№ виртуального канала
      unsigned short ident:10;//идентификатор объекта
      unsigned short nver:2; //№ версии кадра передачи
      quint8 chkad;
      quint8 chvirtkan;
      unsigned short ukzag:11;
      unsigned short iddlseg:2;
      unsigned short flocpak:1;
      unsigned short flsinxr:1;
      unsigned short vtzag:1;
     }fl;
    } UnionGlzag;

     typedef struct
     {
      uchar crc; //1
      uchar ident; //1
      quint16 year; //2
      quint8 month; //1
      quint8 day; //1
      quint32 time; //4
      uchar nka; //1
      quint16 nsutki; //2
      quint32 tsutki; //4
      uchar status; //1
      uchar ca; //1
      uchar l2; //1
      double pdca; //8
      double pdl2; //8
      double fl1; //8
      double fl2; //8
      qint32 dl1;
      qint32 dl2;
      qint8 nl;
      qint8 prtime;
      qint8 prizmer;
      uchar reserv;
     } KadrKVI;

     typedef struct
    {
     uchar   crc;               // 1
     uchar   ident;             // 1
     quint32 time;              // 4
     quint16 year;              // 2
     quint8  month;             // 1
     quint8  day;               // 1
     uchar   prtime;            // 1
     uchar   prnav;             // 1
     double x;                  // 8
     double y;                  // 8
     double z;                  // 8
     float vx;                  // 4
     float vy;                  // 4
     float vz;                  // 4
     float delt_glo;            // 4
     float delt_gps;            // 4
     float skok;                // 4
     float skov;                // 4
    } KadrKVNO;

    typedef struct
    {
     qint32 nver;
     qint32 tippak;
     qint32 fl2zag;
     qint32 idist;
     qint32 flgr;
     qint32 chpak;
     qint32 dlpak;
     qreal interv;
     qint64 npak;
     QDate datn;
     QTime timen;
    } ZagVRV;

    typedef struct
    {
     QDate dat;
     QTime tm;
     qint32 msec;
     qint8 kan;
     quint32 npak;
     qint32 idist;
     quint64 sm;
     qint32 dlpack;
    } ListAlti;

    typedef struct
    {
     QDate dat;
     QTime tm;
     qint32 pak;
     qint8 kan;
     quint32 vk;
     quint64 sm;
     qint32 dlpack;
    } ListCall;

    typedef struct
    {
     QTime tm;
     qint32 kan;
     qint32 vir;
     qint32 ks;
     qint32 ms;
     qint32 sh;
     qint32 aru;
    } ZapKadr;

    UnionGlzag glzag;
    KadrKVI kadrkvi;
    KadrKVNO kadrkvno;
    ZagVRV zagvrv;
    ListAlti listalti;
    ListCall listcall;

    qint64 ind_full;
    QStringList list_err;
    QString komp_name,komp_name_ust;
    qint16 epvrv[86400];
    qint16 epvrvplan[86400];
    bool pr_superuser;
    bool pr_stop;
    bool pr_newver;

   // QStringList prt_kvi; //ОТЛАДКА

    QStringList AltiDatInList(QDate dat,QString path,QStringList &list);

    bool AltiInMap(QByteArray &buf, QMap<QString, QString> &map, qint32 dlpak, qint64 kolpak);

    //заполнение структуры ZapEduch (ClassBd) из файлов сырых измерений
//    bool SetEduchFile322(QString nfile);
//    bool SetEduchFile313(QString nfile);
//    bool SetEduchFile313(QStringList list, QString nfile);
//    bool SetEduchFileSPO(QString nfile);
//    bool SetEduchFileM(QString nfile);
//    bool SetEduchFileO(QString nfile, QString tipis);
//    bool SetEduchFileNG(QString nfile,QString tipis);
//    bool SetEduchFileL(QString nfile);
    /*!
     * заполнение структуры ZapDs данными из сеанса наблюдения и расчёт статистики сеанса ДС
     * \brief SetDs
     * \param nfile[in] - файл сеанса наблюдения с путём к файлу
     * \return true - при успешном завершении; false - ошибка при заполнении структуры
     */
    bool SetDs(QString nfile);
    /*!
     * заполнение структуры ZapFull данными из сеанса ППСИ и расчёт статистики сеанса ППСИ
     * \brief SetFull
     * \param nfile[in] - файл сеанса ППСИ с путём к файлу
     * \param buf[out] - буфер с содержимым сеанса
     * \return true - при успешном завершении; false - ошибка при заполнении структуры
     */
    bool SetFull(QString nfile, QByteArray &buf);
    /*!
     * заполнение структуры ZapDs данными из сеанса наблюдения и расчёт статистики сеанса ДЗС
     * \brief SetDzs
     * \param nfile[in] - файл сеанса наблюдения с путём к файлу
     * \return true - при успешном завершении; false - ошибка при заполнении структуры
     */
    bool SetDzs(QString nfile);
    bool SetSpo(QString nfile);
    bool SetRnxO(QString nfile);
    bool SetRnxO(QStringList list,QString naz_file);
    quint64 SetRnxFileO(QString naz_file, bool prob, bool prsoob);
    bool SetRnxNG(QStringList list, QString naz_file, QString tip, qint32 &kolsut);
    bool SetRnxNG(QString nfile, QString tip, qint32 &kolsut);
    quint64 SetRnxFileNG(QString naz_file, QString tip, bool prob, bool prsoob);
    bool SetM(QString nfile);
    bool SetKos(QString nfile);
    bool SetFrd(QString nfile);
    bool SetAltInBD(QString nfile, bool prsoob);
    bool SetL(QString nfile);
    bool SetRnxL(QString naz_file, QDateTime dn, QDateTime dk, bool kontr);
    bool SetPpsi(QString nfile,bool prsoob);
    bool SetPla(QString nfile,bool prsoob);
    bool SetCogi(QString nfile);
    bool SetCup(QString nfile);
    bool SetSP3(QString naz_file);
    bool SetSol(QString naz_file);
    bool SetAspsi(QString nfile,bool prsoob);
    bool SetLeap(QString nfile,bool prsoob);


    //добавление записи в таблицу educh
    //производится анализ типа измерительных средств по расширению файла,
    //заполняется структура ZapEduch (ClassBd),
    //в таблицу educh добавляется запись с этими данными
    //записывает файл на архивный носитель и добавляет запись в таблицу edxr
//    bool ZapInEduch(QString nfile, qulonglong &nz, bool prud, bool prsoob);

    bool ZapEduch(QString nfile, qulonglong &nz, bool prud, bool prsoob);

    //вызывает SetEdxr(.....)
    //записывает файл на архивный носитель
//    qulonglong ZapInEdxr(QString nfile, qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, bool prsoob);
//    qulonglong ZapInEdxrFromMem(QString nfile, qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, bool prsoob,QByteArray buf);

    quint64 ZapEdxr(QString pref, quint64 nz_tab, QString nfile, qint16 prarx, bool prsoob, QSqlQuery query);
    quint64 ZapEdxr(QString pref, quint64 nz_tab, QString nfile, qint16 prarx, bool prsoob);


    quint64 ZapEdxr(QString pref, quint64 nz_tab, QByteArray &buf, qint16 prarx, bool prsoob, QStringList lf);

    bool ZapNfCogi(QString nfile, qulonglong &nz, bool prud, bool prsoob, bool prpovt);
    bool ZapNfCup(QString nfile, qulonglong &nz, bool prud, bool prsoob, bool pr406);

    //заполнение структуры ZapEdxr (ClassBd)
    //nfile - название исходного файла для помещения
    //nz_educh - № записи в таблице educh к которой относится помещаемый файл
    //prarx - признак архивирования(сжатия) файла: 0-не архивировать;1-архивировать
    //buf - содержимое файла, изменённое в соответствии со значением в prarx
    //кроме того определяет носитель и заполняет переменную текущего архивного носителя tek_nz_nos (ClassBd),
    //если ранее он не был определён
    bool SetEdxr(QString nfile, qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, QByteArray &buf, bool prsoob);
    bool SetEdxrFromMem(QString nfile, qulonglong nz_educh, qulonglong nz_educho, qint16 prarx, QByteArray &buf, bool prsoob);

    bool SetEdxrN(QString pref, qint64 nz_tab, QString nfile, qint16 prarx, QByteArray &buf, bool prsoob, QSqlQuery query);
    bool SetEdxrN(QString pref, qint64 nz_tab, QString nfile, qint16 prarx, QByteArray &buf, bool prsoob);


    bool SetEdxrN(QString pref, qint64 nz_tab, qint16 prarx, QByteArray &buf, QStringList lf, bool prsoob);

    //выполнение кода обработки 2 (educh)
    //переименование файла .rnx в O-файл, структурный контроль, анализ на суточный файл, запись в uducho и передача
    //для дальнейшей обработки
    void ObrSi(QString path_si, QString path_temp, qint32 kolobr, bool prsoob); //помещение сырой информации
    bool ObrKod2(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod3(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod4(qint32 kolobr, QString path_temp, bool prsoob);
//    bool ObrKod2M(bool prsoob);
    bool ObrKod7(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod8(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod6(bool prsoob);
    bool ObrKod9(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod10(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod11_15(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod11_15(QList<qlonglong> list, qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod12(qint32 kolobr, QString path_temp, bool prsoob, quint64 nz_full, QSqlQuery query);
    bool ObrKod12(qint32 kolobr, QString path_temp, bool prsoob, quint64 nz_full);
    bool ObrKod13(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod14(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod16(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod17(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod19(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod18(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod20(bool prsoob);
    bool ObrKod21(qint32 kolobr, QString path_temp, bool prsoob);
    bool ObrKod22(qint32 kolobr, QString path_temp, bool prsoob);
    void ObrNf(QString path_si, QString path_temp, bool prsoob, bool prpovt);

    //заполнение структуры ZapEducho (ClassBd) данными из структуры ZapEduch
    //значения, которых нет в ZapEduch заполняются нулями
//    void SetEducho();
    //кроме этого заполняются поля формата и интервала
    //в hdr возвращается заголовок файла
//    bool SetEducho(QString nfile, QStringList &hdr);

//    bool SetEduchoNFile(QString nfile);

    //по данным из структуры ZapEducho определяет признак файла
    //1-суточный; 2-разбить; 3-объединить
    //заполняет в структуре поля признака суток, кода и признака обработки
//    void PrSutFile();

    //копирует единицу хранения
    //nfile_arx - полный путь к исходному файлу
    //nfile_vix - полный путь к выходному файлу
    //pr - признак архивирования (0-не архив;1-архивировать;2-разархивировать)
    //ks_arx,ks_vix - КС исходного и выходного файлов
    bool CopyEdxr(QString nfile_arx, QString nfile_vix, qint8 pr, qint16 ks_arx, qint16 ks_vix);

    bool CopyEdxrInMem(QString nfile_arx,qint8 pr,qint16 ks_arx,qint16 ks_vix,QByteArray &buf);

//    bool ZapInEducho(QString nfile, qulonglong &nz, bool prud, QString tipis);

    bool RemoveEdxr(qlonglong nz, bool prsoob);

    bool RemoveFullInBasket(quint64 nz_full, bool prsoob);

    bool RemoveDdsInBasket(quint64 nz_dds, bool prsoob);

    bool RemoveDsInBasket(quint64 nz_ds, bool prsoob);

    bool RemoveRnxInBasket(QString naz_table, quint64 nz_rnxi, bool prsoob);

//    bool RemoveEducho(qlonglong nz);

    bool Stat313(QStringList list);

    bool Stat313(quint64 nz_ds, bool prsoob);

    bool StatDDSInBD(QString nfile, bool prsoob);

    bool ZapStatDDS(QStringList list, quint64 nz_dds, bool prsoob);

    bool ZapStatDDS(quint64 nz_dds, bool prsoob);

//    bool StatPPSIInBD(QString nfile_dds, QString nfile_vrv);

    //пересчёт из геоцентрической прямоугольной системы координат в геоцентрическую эллипсоидальную
    //систему (геодезическую) координат
    //Вход:
    //x, y, z - прямоугольные координаты (только для ПЗ-90)
    //Выход:
    //b, l, h - геодезические координаты
    void XYZtoBLH(qreal x, qreal y, qreal z, qreal &b, qreal &l, qreal &h);

    //пересчёт углов из радиан в градусы, минуты, секунды и доли градуса
    //Вход:
    //rad - угол в радианах
    //Выход:
    //gr, min, sec - градусы, минуты, секунды с долями секунд
    //dgrad - градусы и доли градуса
    void RadToGMS(qreal rad, qint32 &gr, qint32 &min, qreal &sec, qreal &dgrad);

    //решение пространственной обратной геодезической задачи
    //Вход:
    //xnpn, ynpn, znpn - геоцентические прямоугольные координаты (только для ПЗ-90) 1-й точки (НПН)
    //xka, yka, zka - геоцентические прямоугольные координаты (только для ПЗ-90) 2-й точки (КА)
    //Выход:
    //az, um, ras - азимут, угол места и расстояние в метрах из 1-й точки во 2-ю
    void OGZ(qreal xka, qreal yka, qreal zka, qreal xnpn, qreal ynpn, qreal znpn, qreal &az, qreal &um, qreal &ras);

    //интерполяция планируемого наведения антены ППСИ (файл *.ppsi) из исходного в секундный интервал
    //Вход:
    //nfile - полный путь с названием исходного файла плана ППСИ (*.ppsi)
    //Выход:
    //list - список с секундным интервалом наведвния антены ППСИ
    //дата(dd mm yyyy) время(hh mm ss) азимут(10 символов) угол места(10 символов) идентификатор (10 символов; = - для плана; - для интерполируемых значений)
    //nkfile - имя файла с навигационными определениями (*.*K)
    //в случае ошибки возвращает false
    bool FilePPSI(QString nfile,QStringList &list,QString &nkfile);

    //сравнение плана наведения антены ППСИ со значениями наведения полученными из навигационных определений
    //Вход:
    //fppsi - путь и название файла плана ППСИ (*.ppsi)
    //path_k - путь к директории с файлами навигационных определений (*.*K)
    //Выход:
    //prot - протокол работы
    //план.дата(01-09-2018) план.время(08:13:01) тип плана(интерполяция) азимут и угол места(350.709260  18.308884)
    //дата и время эпохи квно(01-09-2018 05:13:19) расчитанный азимут,угол места и расстояние(351.807246  18.165209 2140179.93)
    //максимальное расхождение планируемых и расчитанных нав.опред. азимута и угла места(1 Г  5 М 52.75 С   0 Г  8 М 37.23 С)
    //max_raz_az, max_raz_um - максимальное расхождение планируемых и расчитанных нав.опред. азимута и угла места
    //в случае ошибки возвращает false
    bool ComparePPSI(QString fppsi, QString path_k, QStringList &prot, qreal &max_raz_az, qreal &max_raz_um);

    //поиск эпохи в файле навигационных определений (*.*K)
    //Вход:
    //list - список с содержимым файла
    //epoch - эпоха для поиска
    //ind - индекс в списке с которого начинать поиск
    //Выход:
    //str_epoch - строка с найденой эпохой
    //ind - изменённый индекс, содержащий номер найденной строки
    //в случае успешного поиска возвращает true, иначе false
    bool FindEpochInKFile(QStringList list, QDateTime epoch, QString &str_epoch, qint32 &ind);

    //пересчёт из долей градуса в градусы, минуты и секнды
    //Вход:
    //dgrad - градусы и доли градуса
    //Выход:
    //gr, min, sec - градусы, минуты, секунды и доли секунды
    void DgradToGMS(qreal dgrad, qint32 &gr, qint32 &min, qreal &sec);

    bool CrcKadrFULL(QByteArray &buf,qint64 ind);

    void ReadGlZagKadrFULL(QByteArray &buf,qint64 ind);

    bool StatFull(QByteArray &buf, bool prprt, QString nfile_prt);
    //расчёт статистики сеанса ППСИ
    //заполняет структуру statfull
    //buf - область памяти с сеансом ППСИ
    //tn,tk - время начала/конца интервала для расчёта (обычно - план АСПСИ)
    //prprt,nfile_prt - признак печати и путь к файлу протокола
    bool StatFull(QByteArray &buf, bool prprt, QString nfile_prt,QTime tn,QTime tk);
    //получение статистики сеанса ППСИ по двум каналам
    //buf1,buf2 - области памяти с сеансами ППСИ (1 и 2)
    //tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
    //kolk - количество кадров в сеансах (соответствует масимальному количеству из двух каналов)
    //kolbr - количество отбракованных кадров (по маркеру синхр. и КС)
    bool StatFull(QByteArray &buf1,QByteArray &buf2,QTime tn,QTime tk,qint32 &kolk,qint32 &kolbr);

    QTime TimeKadrFULL(QByteArray &buf,qint64 ind);
    //получение времени из кадра ППСИ
    //buf - область памяти с сеансом ППСИ
    //ind - смещение в памяти на начало кадра
    //ms - милисекунды из времени получения кадра
    QTime TimeKadrFULL(QByteArray &buf,qint64 ind,qint32 &ms);

    //чтение файла ППСИ - FULL файла
    //buf - байтовый массив в котором находится считанный файл
    //kod - код работы: 0 - чтение массива с начала; 1 - продолжение чтения массива
    //индекс в массиве указывает на начало последнего успешно найденного кадра
    //индекс ind_full описан в классе
    //nvk - № виртуального канала
    //pr_crc - признак посчёта КС (true - правильная КС; false - неправильная)
    //при успешно найденном кадре возвращает true
    //заполняет структуру главного заголовка найденного кадра
    //если kod=0 и возврат - false, то не найдено ни одного кадра
    bool ReadFULL(QByteArray &buf,qint32 kod,qint32 &nvk,bool &pr_crc);
    bool ReadBackFULL(QByteArray &buf, qint32 kod, qint32 &nvk, bool &pr_crc); // Тоже что и ReadFULL, но читает с конца файла


    //получение информации из кадра ППСИ
    //buf - область памяти с сеансом ППСИ
    //sm - смещение в памяти на начало кадра
    //nvk - № виртуального канала (для кадра без синхросигнала: -1)
    //pr_crc - признак КС (false - плохая)
    //tm - возвращает время получения кадра hh:mm:ss.zzz
    //s - признак наличия синхросигнала (false - нет синхросигнала)
    //aru1,aru2 - АРУ станции
    //sh1,sh2 - уровень сигнал/шум
    //ms - милисекунды из времени получения кадра
    void KadrFULL(QByteArray &buf,qint64 sm,qint32 &nvk,bool &pr_crc,QTime &tm,bool &s,qint32 &aru1,qint32 &aru2,qint32 &sh1,qint32 &sh2,qint32 &ms);

    //получение телеметрической информации из кадра ППСИ
    //buf - область памяти с сеансом ППСИ
    //ind - смещение в памяти на начало кадра
    //t - возвращает время получения кадра hh:mm:ss.zzz
    //aru1,aru2 - АРУ станции
    //sh1,sh2 - уровень сигнал/шум
    //ms - милисекунды из времени получения кадра
    void TelKadrFULL(QByteArray &buf,qint64 ind,QTime &t,qint32 &aru1,qint32 &aru2,qint32 &sh1,qint32 &sh2,qint32 &ms);

    //получение содержимого двух сеансов ППСИ в виде сгруппированного по времени текстового списка
    //list - содержимое двух сеансов ППСИ в виде отсортированного текстового списка в формате:
    //время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
    //формат строки:
    //время кадра(hh:mm:ss.zzz) для двух каналов #
    //время кадра(hh:mm:ss.zzz);1 (канал);№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров) # (если нет информации по каналу: пробел #)
    //время кадра(hh:mm:ss.zzz);2 (канал);№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров) (если нет информации по каналу: ничего не выводится)
    QStringList ObKadrSeansFull(QStringList &list);

    //получение содержимого сеанса ППСИ в текстовом виде
    //buf - область памяти с сеансом ППСИ
    //tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
    //kan - канал сеанса
    //формат строки:
    //время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
    QStringList SeansFullInList(QByteArray &buf,QTime tn,QTime tk,QString kan);
    //получение содержимого двух сеансов ППСИ в виде отсортированного текстового списка
    //buf1,buf2 - области памяти с сеансами ППСИ (1 и 2)
    //tn,tk - время отбора кадров из сеанса (как правило из плана АСПСИ)
    //формат строки:
    //время кадра(hh:mm:ss.zzz);канал;№ вирт.кнала(-1 - для кадров бе маркера);С/Ш;АРУ;КС кадра(только данные.0-для плохих кадров);кол.милисекунд из времени
    QStringList SeansFullInList(QByteArray &buf1,QByteArray &buf2,QTime tn,QTime tk);

    qint32 ObrKadrKVI(QByteArray &buf, qint64 ind);

    //конвертирование кадра FULL КВИ ДДС
    //buf - байтовый массив в котором находится считанный FULL
    //ind - смещение в массиве на начало кадра
    //map - ключевой список для формирования файла измерений (ключ - эпоха, запись - измерение для спутника в формате rinex)
    //prot - протокол ошибок
    bool KonvDDSfromKadrKVI(QByteArray &buf, qint64 ind, QMap<QString, QString> &map, QStringList &prot, QDate dat_ppsi);

    qint32 KonvDDS_KVI(QByteArray buf, QMap<QString, QString> &map, QDate dat_ppsi);

    QStringList KonvFULL(QString nfile, QString path, qint32 nka);

    void ReadDDS_KVI(QByteArray buf);

    qint32 OtbrDDS_KVI(QDate dat_ppsi);

    void FormDDS_KVI(QMap<QString, QString> &map);

    QString CodSH(uchar cod);

    QString FormFileDDS_KVI(QMap<QString, QString> &map, QString path, QString naz_full, qint32 nka);

    bool KonvDDSfromKadrKVNO(QByteArray &buf, qint64 ind, QMap<QDateTime, QString> &map, QStringList &prot,QDate dat_ppsi);

    qint32 KonvDDS_KVNO(QByteArray buf, QMap<QDateTime, QString> &map, QDate dat_ppsi);

    void ReadDDS_KVNO(QByteArray buf);

    qint32 OtbrDDS_KVNO(QDate dat_ppsi);

    void FormDDS_KVNO(QMap<QDateTime, QString> &map);

    QString FormFileDDS_KVNO(QMap<QDateTime, QString> &map, QString path, qint32 nka);

    QString NazKFile(QString nka,QDate dt,QString path);

    QStringList FormKFileFromMap(QMap<QDateTime,QString> map, QDateTime &nep);

    int KolSutInK(QStringList list);

    qint32 KolSutInK(QStringList list,QDate &dn,QDate &dk);

    qint32 KolSutInK(QString nfile,QDate &dn,QDate &dk);

    QStringList RazbKFile(QString nfile);

    QStringList RazbKFile(quint64 nz, QString path, bool prsoob);

    QMap<QDateTime,QString> KFileInMap(QString nfile);

    QMap<QDateTime,QString> KFileInMap(QStringList list);

    QString ObKFile(QStringList spisf, QString nka, QString path);

    QString ObKFile(QList<qlonglong> sp_nz, QString path, bool prsoob);

    void ReadZagVRV(QByteArray &buf,qint64 ind);

    void ReadAltiInList(QByteArray &buf, QStringList &list, QString kan, QDate d_full, QStringList &prot);

    void FormZagTelVRV(QByteArray &buf, qint32 idist, QString isx_file, QString vix_file, qint64 kolpak);

    void FormTelPackVRV(QByteArray &pack, QByteArray &full1, quint64 ind_full1, QByteArray &full2, quint64 ind_full2, qint32 dlpack);

    bool CreateAltiVRV(QByteArray &buf, QByteArray &buf1, QByteArray &buf2, qint32 &ind, QStringList &list, QDate &dn, QTime &tn, qint64 &kolpak, qint32 inter, bool prdat);

    void ReadListAlti(QString str);

    QStringList CreateAllAltiVRV(QString full_file1, QString full_file2, QString path, QString nka, qint32 inter, bool prdat, QStringList &prot);

    QStringList CreateAllAltiVRV(QByteArray &buf1, QByteArray &buf2, QString path, QString nka, qint32 inter, QString nfile1, QByteArray &bufz, bool prdat, QDate d_full, QStringList &prot, qint32 nkan);

    void ReadCallInList(QByteArray &buf, QStringList &list, QString kan, qint32 idist, QDate d_full);

    bool CreateCallVRV(QByteArray &buf, QByteArray &buf1, QByteArray &buf2, qint32 &ind, QStringList &list, QDate &dn, QTime &tn, qint64 &kolpak);

    void ReadListCall(QString str);

    QStringList CreateAllCallVRV(QString full_file1, QString full_file2, QString path, QString nka, qint32 idist, QByteArray &buf, QDate d_full);

    QStringList CreateAllCallVRV(QString full_file, QString path, qint32 idist, QDate d_full);

    QStringList CreateAllCallVRV(QByteArray &buf1, QByteArray &buf2, QString path, QString nka, qint32 idist, QString file1, QByteArray &bufz, qint32 kan, QDate d_full);

    void ObrPackVRV(QByteArray &buf, QByteArray &buf1, QByteArray &buf2, qint32 dlpack, quint64 sm1, quint64 sm2, quint64 sm11, quint64 sm12, qint64 &kolpak);

    bool ReadAltiInList(QByteArray &buf, QStringList &list);

    QString NameVRVFile(QString pref, QDate dat, QTime time, qint16 ncall, qint16 kan, qint32 nka);

/*!
 * \brief ObrIzm::PrefFull
 * определение даты и времени начала и конца сеанса ППСИ
 * из заголовка FULL файла
 * \param buf[in] содержимое FULL файла
 * \param dtn[out] дата и время начала сеанса
 * \param dtk[out] дата и время конца сеанса
*/
    void PrefFull(QByteArray &buf, QDateTime &dtn, QDateTime &dtk);

    QString KonvFullDDS(QByteArray &buf, QString path, QString nka, QString nfile, QDate dat_ppsi, QStringList &prot);

    QString KonvFullKVNO(QByteArray &buf, QString path, QString nka, QDate dat_ppsi, QStringList &prot);

    quint64 SetDDSFromFileKVI(QString nfile, bool pr, qint32 kodobr, bool prsoob, QDate &dn, QDate &dk);

    quint64 SetDDSFromFileKVNO(QString nfile, bool pr, qint32 kodobr, bool prsoob, QDate &dn, QDate &dk);

    qint32 KolSutInAlti(QByteArray &buf, QDateTime &dtn, QDateTime &dtk);

    bool InfFromAlti(QByteArray &buf, QDateTime &dtn, QDateTime &dtk, qint64 &kolpak, qint32 &dlpak, qint32 &dlsh, QString &i_file, QString &v_file, qint32 &kolsut);

    bool InfFromAlti(QByteArray &buf, QDateTime &dtn, QDateTime &dtk, qint64 kolpak, qint32 &kolsut);

    quint64 SetDDSFromFileAlti(QString nfile, bool pr, QDateTime dtn, QDateTime dtk, qint32 kolsut, QString pref, qint32 kodobr, bool prsoob, QSqlQuery query);

    quint64 SetDDSFromFileAlti(QString nfile, bool pr, QDateTime dtn, QDateTime dtk, qint32 kolsut, QString pref, qint32 kodobr, bool prsoob);

    quint64 SetDDSFromFileAlti(QString nfile, bool pr, QString pref, qint32 kodobr, bool prsoob);

    bool RemoveDDS(quint64 nz, bool prsoob);

    QStringList RazbAlti(quint64 nz, QString path, bool prsoob);

    QStringList RazbAlti(QByteArray &buf, QString path, bool prsoob, quint64 nz, qint32 nkan);

    QString WriteFileAlti(QByteArray &buf, QString nka, QDate dn, QTime tn, qint64 kolpak, QString i_file, qint64 ind, qint32 dlpak, QString path, qint32 nkan);

    QString WriteFileAlti(QByteArray &buf, QString nka, QDate dn, QTime tn, qint64 kolpak, QString i_file, QString path);

    bool ObAltiInMap(QList<qlonglong> nz, QMap<QString,QString> &map, qint64 &okolpak, QString &i_file, QByteArray &obuf, qint32 &dlpak, bool prsoob);

    bool ObAltiInMapF(QList<qlonglong> nz, QMap<QString,QString> &map, qint64 &okolpak, QString &i_file, QByteArray &obuf, qint32 &dlpak, bool prsoob);

    bool ObAltiInMapF(QStringList naz_file, QMap<QString,QString> &map, qint64 &okolpak, QString &i_file, QByteArray &obuf, qint32 &dlpak, bool prsoob);

    bool AltiMapInArray(QMap<QString, QString> &map, QByteArray &buf_isx, QByteArray &buf_alti, qint32 dlpak, qint64 &kolpak);

    bool RemoveRnxv(quint64 nz, bool prsoob);

    bool ZapStatVRV(quint64 nz_dds, quint64 &nz, bool prsoob);

//    bool ZapStatVRV(QString nfile_alti, quint64 nz_dds);

//    bool ZapStatVRV(quint64 nz_dds, bool prsoob);

    bool Stat322(QStringList list);

    bool Stat322(quint64 nz_ds, bool prsoob);

    /*!
     * чтение единицы хранения в память
     * заполняется структура readedxr
     * \brief ReadEdxrInBuf
     * \param ntab[in] название таблицы (должна иметь поле nz_edxr)
     * \param nz_tab[in] № записи в таблице
     * \param buf[out] область памяти для данных
     * \param prsoob[in] признак выдачи сообщений об ошибках
     * \return
     */
    bool ReadEdxrInBuf(QString ntab, quint64 nz_tab, QByteArray &buf, bool prsoob);

    bool ReadEdxrInList(QString ntab, quint64 nz_tab, QStringList &list, bool prsoob);

    bool EdvidForDs(quint64 nz_zapr, bool prsoob);

    bool EdvidForDds(quint64 nz_zapr, bool prsoob);

    bool EdvidForPpsi(quint64 nz_zapr, bool prsoob);

    bool EdvidForRnxi(quint64 nz_zapr, bool prsoob);

    bool EdvidForRnxv(quint64 nz_zapr, bool prsoob);

    bool ZapEpvrv(QByteArray &buf);

    bool ZapEpvrvplan(QDate dat, QString nka);

    bool ZapStatVRVInList(quint64 nz_dds, QStringList &list);

    bool RasStatDs(QDate dn, QDate dk,bool prsoob);

    bool RasStatDzs(QDate dn, QDate dk,bool prsoob);

    bool RasStatDds(QDate dn, QDate dk,bool prsoob);

    bool RasStatVrv(QDate dn, QDate dk, QString nka, bool prsoob);

    bool CopyEdxr(QString ntab, quint64 nz_tab, QString path_vix, bool prsoob, QString &nfile);

    bool CreateFileForPpsi(QDate dat, qint32 kod, QString &naz_file, bool prsoob, QString nka, QList<qlonglong> &list, qint32 &kodobr);

    void OtprNf(QString path_vix, bool prsoob);

    //получение списка спутников в файле измерений ДДС
    //строка списка: название спутника;эпоха;количество частот
    //пример: R01;2018 09 29 11 15 55.0000000;1
    //список отсортирован
    QStringList SpisEpochForSatDDS(QStringList list);
    QStringList SpisEpochForSatDDS(qlonglong nz_dds, QStringList &list_mix);

    bool Compress(QString _str);     //Архивирование Хатанака
    bool UnCompress(QString _str);   //Разархивирование Хатанака

    bool RnxToRnx(QString nfile);

    bool InfDatForIzmInf(QString nfile, qint32 &year, qint32 &sut, QDate &date);

    bool DtSP3(QStringList &list, QDateTime &dtn, QDateTime &dtk, QString &nka);

    QStringList SP3InList(QString naz_file);

    QStringList SP3InList(quint64 nz_sp3,bool prsoob);

    QStringList SP3InList(QStringList &list);

    bool RemoveSp3InBasket(quint64 nz_sp3, bool prsoob);

    bool EdvidForSp3(quint64 nz_zapr,bool prsoob);

    bool ModelInList(QSqlQueryModel *model, int rcol, QStringList &list, QString delemiter, bool pr_fname);

    bool DtSol(QStringList &list, QDateTime &dtn, QDateTime &dtk, QString &nka, qint32 &kolizm, qint32 &kolk, qint32 &kolo, qreal &sko);

    bool RemoveSolInBasket(quint64 nz_sp3, bool prsoob);

    bool EdvidForSol(quint64 nz_zapr,bool prsoob);

    void KadrKviInList(QStringList &list,qint32 kod_otbr);

    void ObrNf306(quint64 nz,QStringList &list);

    bool DostIzm313(QString izmer, qint32 sh, bool &df1, bool &df2);

    bool Seans313(QStringList &list,qint32 &ind_n,qint32 &ind_k);

    quint64 PomDdsKvi(QStringList list, QString nfile, bool prsoob);

    bool PerepomEduch(QString nfile, qint32 &obr);

    bool VidEdvid(quint64 nz_edvid, QString path_vid, bool pr_dir);

    QDateTime DmtToGps(QDateTime dt_dmt);


    converterJps *convJps;

private:
    Compressor *compressor;                 //Компрессор хатанака

};

#endif // OBRIZM_H
