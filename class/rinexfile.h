#ifndef RINEXFILE_H
#define RINEXFILE_H

#include <QtWidgets>
//#include "classbd.h"
#include "dirfile.h"

class RinexFile : public DirFile
{
public:
    RinexFile();

    qint32 kol_ep_dds[24];

    //массивы для определения количества и типа измерений спутников
    int mas_gps[32][13];
    int mas_glo[24][13];

    QStringList list_m;

    //определяет версию rinex-файла
    QString RnxVersion(QStringList list);
    QString RnxVersion(QString nfile);

    //определяет тип rinex-файла (O, N)
    QString RnxTip(QStringList list);

    //определяет спутниковую систему в rinex-файле
    QString RnxSatSys(QStringList list);

    //время первого наблюдения из заголовка rinex-файла 2.XX 3.XX
    QString RnxFirstObs(QStringList list);

    //время последнего наблюдения из заголовка rinex-файла  2.XX 3.XX
    QString RnxLastObs(QStringList list);

    //время первого наблюдения из списка дескрипторов данных rinex-файла
    QString RnxFirstObsF(QStringList list_dd);

    //время последнего наблюдения из списка дескрипторов данных rinex-файла
    QString RnxLastObsF(QStringList list_dd);

    //замена времени наблюдения в заголовке rinex-файла; pr=1 - первое набл., 2-последн.набл.
    bool UpdateRnxDTObs(QStringList &list,QDateTime dt,short pr);

    //получение списка эпох 3.XX
    QStringList SpisRnxDataDesc(QStringList list);
    //через ; записывается № строки в списке 3.XX
    QStringList SpisRnxDataDescInd(QStringList list);

    //получение символьной строки даты и времени в формате rinex-файла
    //pr: 1-HDR; 2- > (эпоха)
    QString DtToStrHdrRnx(QDateTime dt, short pr);

    //проверка версии rinex-файла на возможность обработки
    bool ProvRnxVer(QStringList list);

    //проверка правильности заполнения даты наблюдений в заголовке rinex-файла
    //возврат: -1 - ошибка обработки; 0 - заполнено правильно; 1 - ошибка в перв.; 10 - ошибка в посл; 11 - ошибка в обоих
    int ProvRnxDTObs(QStringList list,QStringList list_dd);

    //чтение заголовка rinex-файла
    QStringList ReadRnxHdr(QStringList list);
    QStringList ReadRnxHdr(QString nfile);

    //исправление даты наблюдений в соответствии с содержимым rinex-файла 3.XX
    bool IsprRnxDTObs(QString nfile);
    bool IsprRnxDTObs(QStringList &list, QStringList list_dd);

    //количество суток в файле наблюдений (выбирается из заголовка) 2.XX 3.XX
    int KolSutInRnx(QStringList list);
    int KolSutInRnx(QString nfile, QDate &dn, QDate &dk);

    //чтение измерений для эпохи  hdrdat - заголовок эпохи 3.XX
    QStringList ReadSecDatRnx(QStringList list,QString hdrdat,int &ind);

    //количество измерений(спутников) из заголовка эпохи
    //при ошибке возвращает -1
    int KolIzmInDat(QString hdrdat, QString &err);

    //проверка структуры данных в rinex-файле с записью ошибок в текстовый список
    QStringList ProvStrDatRnx(QStringList list, QString nfile); //3.XX
    QStringList ProvStrDatRnxN(QStringList list, QString nkg); //3.XX
    QStringList ProvStrDatRnx(QStringList list); //3.XX
    QStringList ProvStrDatRnxN2(QStringList list, QString nkg); //2.XX
    QStringList ProvStrDatRnx2(QStringList list, QDateTime &dn, QDateTime &dk); //2.XX

    //проверка структуры rinex-файла
    bool ProvRnxFile(QString nfile, bool pr_errfile, bool pr_progres);
    bool ProvRnxFileN(QString nfile,bool pr_errfile);
    bool ProvRnxFileN2(QString nfile,bool pr_errfile);
    bool ProvRnx2File(QString nfile,bool pr_errfile);

    //формирование имени О-файла для пункта наблюдения pn в директории path
    //имя формируется без пути к файлу
    QString NazOFile(QString pn,QStringList list,QString path);
    QString NazOFile(QString nfile);

    //создание О-файла для определённой даты из объединённого rinex-файла
    QStringList CreateOFile(QStringList list,QDate dt);

    //получение даты и времени эпохи измерений из заголовка эпохи 3.XX
    QDateTime DTForHdrDatRnx(QString hdrdat);

    //разбор О-файла на суточные
    //в sp_file - список созданных файлов (в директории исходного файла)
    //исходный файл удаляется
    bool RazbAllOFile(QString nfile, QStringList &sp_file);

    //чтение О-файла rinex в ключевой список ключ - заголовок эпохи+" "+спутник пример:G12
    //в заголовке эпохи количество спутников обнуляется
    //list - содержимое файла (с заголовком или без)
    QMap<QString,QString> ReadOFileInMap(QString nfile);
    QMap<QString,QString> ReadOFileInMap(QStringList list);

    //формирование эпох О-файла без заголовка rinex-файла из ключевого списка
    //количество спутников в заголовках эпохи восстанавливается
    QStringList FormOFileFromMap(QMap<QString,QString> map);

    //корректировка количества спутников на эпоху и заполнение mas_gps и mas_glo
    void KorrKolSputn(QStringList &list);

    //формирование заголовка rinex-файла
    //spis_f - список файлов ППСИ (с полными путями) для записи в заголовок
    //list - список содержащий данные с наблюдениями по эпохам
    //nka - номер КА пример:126
    QStringList FormHdrDDS(QStringList spis_f, QStringList list, QString nka);

    //формирование строки типов измерений в формате rinex-файла для ДДС
    //kod: 1-для GPS, 2-для ГЛОНАСС
    QString CreateObsTypesDDS(int kod);

    //формирование строки количества КА в формате rinex-файла
    //выполняется по заполненному массиву измерений
    QString CreateOfSatellites();

    //формирование строк по количеству наблюдений спутников
    QStringList CreatePrnOfObsDDS();

    //разбивка ДДС файла на суточные
    //nfile - путь и название исходного файла
    //spis_file - список исходных файлов для включения в заголовок
    //nka - номер КА пример: 126
    //выдаёт список (путь и название) созданных файлов
    //если файл не разделялся, то в список заносится путь и название исходного файла
    //если обнаружены ошибки в исходном файле выдаётся пустой список
    //при успешной разбивки исходный файл удаляется
    QStringList RazbDDSOFile(QString nfile, QStringList spis_file, QString nka);
    QStringList RazbDDSOFile(QStringList list, QString path);

    //создаёт из исходного списка эпох list новый список
    //с отбором эпох на дату dt
    //при неудачном отборе данных выдаёт пустой список
    //выходной список не содержит заголовка файла
    QStringList CreateListForDate(QStringList list,QDate dt);

    //копирует О-файл в директорию БИИ
    //nfile - исходный файл (полный путь), должен иметь стандартное название
    //path_bii - путь к папке 1-го уровня соответствующих измерений в БИИ
    //pr_zam - признак замены файлов с одинаковым названием в БИИ;
    //если признак - false, то создаётся уникальное имя файла в папке БИИ
    //pr_ud - признак удаления исходного файла
    //при копировании, в соответствии с именем файла и путём к папке 1-го уровня
    //создаются соответствующие директории
    //в перегруженной функции копируются файлы из списка lfile
    bool CopyOFileInBII(QString nfile,QString path_bii,bool pr_zam,bool pr_ud);
    bool CopyOFileInBII(QStringList lfile,QString path_bii,bool pr_zam,bool pr_ud);

    //формирует путь для О-файла nfile в дирректории path_bii
    //возвращает путь для размещения файла (path_bii/год/сутки)
    QString PathInBIIOFile(QString nfile,QString path_bii);

    //объединяет О-файлы из списка sp_file (с полными путями)
    //результат сохраняется в списке (отсортированный перечень эпох без заголовка файла)
    QStringList ObOFile(QStringList sp_file);

    //анализ заголовка О-файла на соответствие ДДС
    //list - содержимое О-файла
    //возвращает: nka - номер КА из заголовка; spis_f - список отконвертированных файлов из заголовка (список дополняется каждый вызов)
    //zn_sec - содержит значение последней секунды во времени первого измерения (1 для 21:32:21; 4 для 21:32:24 и т.д.)
    //при возврате false - файл не соответствует формату ДДС
    bool ReadHdrDDS(QStringList list, QString &nka, QStringList &spis_f, int &zn_sec);
    bool ReadHdrDDS(QStringList list);

    //объединение файлов ДДС из списка
    //spis_file - список файлов ДДС с полными путями
    //path_bii - путь к папке 1-го уровня соответствующих измерений в БИИ
    //path_temp - путь к директории в которой размещаются временные файлы во время обработки
    bool ObDDS(QStringList spis_file, QString path_bii, QString path_temp, bool prd_show, bool pr_soob);
    bool ObDDS(QMap<QString,QString> &map, QStringList dob_file, QStringList &ppsi_file, QString nka);

    //расчитывает статистику наблюдений по суточному файлу ДДС
    QStringList StatDDS(QStringList list);

    //очистка массива по измерениям спутников
    void ClearMasSputn();

    //заполнение массива измерений спутников
    //str - строка наблюдений
    void ZapSputnInMas(QString str);
    void ZapSputnInMas(QStringList list);

    //читает из заголовка количество типов измерений
    //list - содержимое файла или заголовка
    //kod - для каких НКА : "G", "R"
    //в случае неудачи, возвращает -1
    int ReadKolObsTypes(QStringList list, QString kod);

    //создание списка количества наблюдений по спутникам
    QStringList CreatePrnOfObs(QStringList list);

    //изменение количества наблюдений по спутникам и количества спутников
    void UpdatePrnOfObs(QStringList &list);

    //чтение эпохи из строки N-файла
    //в случае ошибки возвращает ""
    QString ReadDTFromN(QString str);

    //формирование списка по содержимому N-файла
    //список содержит строки по эпохам, блоки наблюдений отделены знаком ;
    QStringList SetListNFile(QStringList list);

    //формирование содержимого N-файла
    //из списка по эпохам
    QStringList GetListNFile(QStringList list);

    //получение начальной и конечной даты из N-файла и расчёт количества суток
    //исходным является список эпох
    bool KolSutNFile(QStringList list,QDate &dn,QDate &dk,int &kolsut);
    bool KolSutNFile(QStringList list,QDateTime &dtn,QDateTime &dtk,int &kolsut);

    //разбор N-файла по датам
    //формируются новые файлы разбитые по датам в директории исходного файла
    //исходный файл уничтожается
    QStringList RazbNFile(QString nfile);

    //формирование списка эпох N-файла с отбором по дате
    //list - исходный список эпох
    //depoh - дата для отбора эпох
    QStringList OtbNFileForDat(QStringList list, QDate depoh);

    //получение уникального названия N-файла
    //npn - № пункта (4 символа)
    //d - дата для формирования названия
    //path - путь для размещения файла
    //tipf - тип файла (в расширении N или G)
    QString NazNFile(QString npn, QDate d, QString path, QString tipf);

    //объединение N файлов из списка spis_f
    bool ObNFile(QStringList spis_f, QString &nfile);

    //копирование содержимого списка эпох в map
    //ключ выбирается из строки спутник,дата,время
    QMap<QString,QString> CopyListToMapNFile(QStringList list);

    //копирование содержимого map в список эпох
    //отбор и сортировка по ключу - спутник,дата,время
    QStringList CopyMapToListNFile(QMap<QString,QString> map);

    //перезаписать дату в заголовке N-файла
    bool UpdateDateNFile(QDate dat, QStringList &list);

    //получение всех суточных N-файлов в директории path
    bool ObAllNFile(QString path, QStringList &sp_g, QStringList &sp_n);

    //формирование списка по содержимому G,N-файла версии 2.10
    //список содержит строки по эпохам + блоки наблюдений - отделены знаком ;
    //заголовок файла отсутствует
    QStringList SetListNFile2(QStringList list);

    //чтение эпохи из строки N-файла версии 2
    //в случае ошибки возвращает ""
    QString ReadDTFromN2(QString str);

    //получение начальной и конечной даты из N-файла версии 2 и расчёт количества суток
    //исходным является список эпох
    bool KolSutNFile2(QStringList list, QDateTime &dtn, QDateTime &dtk, int &kolsut);

    //возвращает интервал наблюдений из заголовка
    //если интервал в заголовке не найден, возвращает -1
    qint32 RnxInterval(QStringList list);

    //перезапись количества спутников в заголовке файла
    void UpdateOfSat(QStringList &list);

    //удаление эпох с количеством спутников меньше kol_sat
    bool DelEpoch(qint32 kol_sat, QStringList &list, QString &err);
    bool DelEpoch(QString str_ep, QStringList &list, QString &err);

    //разделение N-файла по спутниковым группировкам
    QStringList RazdNFileToNG(QStringList list,QString kod);
    bool RazdNFileToNG(QString nfile, QString npn, QStringList &sp_file);

    //получение статистики спутников ГЛОНАСС для файла ДДС
    //list - содержимое файла
    //kol1,kol2,kol3 - количество эпох с количеством спутников ГЛОНАСС <4,=4,>4
    //kolep - общее количество эпох
    bool StatEpochForDDS(QStringList list, qint32 &kol1, qint32 &kol2, qint32 &kol3, qint32 &kolep);

    void SetToGPS(QDateTime dt,qint32 &weekGPS, qint32 &secGPS);

    void SetToGPS(QDateTime dt,qint32 &weekGPS, qint32 &secGPS,qint32 &weekGPS1);

    void SetToGLO(QDateTime dt,qint32 &sutGLO, qint32 &secGLO);

    bool ProvRnxInterval(QStringList rnx,QStringList &err);
    bool ProvRnxInterval(QStringList rnx,QStringList &err,QStringList &sp_err_epoch);

    bool ProvRnxNSat(QStringList rnx, qint32 ind_str, qint32 kolizm, QList<qint32> &ind_err);

    //bool test_rinex_V2(QFile *file, QStringList &_errorList);

    qint32 StatGlonassForEpoch(QStringList &list,qint32 ind,qint32 &kolizm);

    QStringList StatMixForGlonass(QStringList list);

    QMap<QString, QString> DelDublInMap(QMap<QString, QString> map);

    qint32 ZnSekDds(QString ep, QTime &t_ep);

    QDate DatRnxN2(QStringList &list);

    bool ZagEpoch2(QString zag_epoch);


};

#endif // RINEXFILE_H
