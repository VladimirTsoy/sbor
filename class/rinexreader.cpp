#include "rinexreader.h"
#include <QString>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include "converterjps.h"

rinexReader::rinexReader(QFile *Obs, QFile *Nav, QFile *NavG, int versionInp)
{
    //ИНИЦИАЛИЗАЦИЯ ПЕРЕМЕННЫХ
    strNum = 0;             //Позиция последней прочитаной строки в потоке
    FILE = Obs;             //Адрес файла наблюдений
    VERS = versionInp;      //Признак версии конвертируемого файла
    slipVersion = false;    //Флаг ошибки версии конвертируемого файла (true если версия файла не соответствует признаку VERS)

    /* СЧИТЫВАНИЕ ШАПКИ ФАЙЛА НАБЛЮДЕНИЙ
     * В этом блоке происводится только считывание шапки файла наблюдений
     * проверка версии и вывод сообщений. Непосредственное считывание шапки
     * производится в методах RRFH210 и RRFH302(в соответствии с версией).
     * В случае отсутствия файла по указаному адресу генерируется ошибка 205.
     * В случае неправильной версии файла наблюдений генерирует ошибки 203
     * и 204(см. converterJps.h).
     */
    qDebug() << Obs->fileName();
//    if (!Obs->fileName().isEmpty())
//    {
     readRinexFileHeader(Obs);
     if(noObservationFile==false){
         qDebug() << "[1]OBS RINEX File version: " << version;
     }
     else{qDebug() << "[205]No Observation File";}
     //ПРОВЕРКА ВЕРСИИ
     qreal verDub;
     bool *ok = 0;
     verDub = version.toDouble(ok);
     if(VERS==211){
         slipVersion = (verDub==2.11|verDub==2.10);
         if(slipVersion==true){converterJps::errorList.append(203);}
     }
     if(VERS==302){
         slipVersion = (verDub==3.02);
         if(slipVersion==true){converterJps::errorList.append(204);}
     }
//    }

    //СЧИТЫВАНИЕ ШАПКИ НАВИГАЦИОННОГО ФАЙЛА GPS
    qDebug() << Nav->fileName();
    if (!Nav->fileName().isEmpty())
    {
         RRFHnav(Nav);
         if(noNavigationFile==false){
             qDebug() << "[2]NAV RINEX File version: " << versionNav;
         }
         else{
             qDebug() << "[201]No Navigation File GPS";
         }
     }

    //СЧИТЫВАНИЕ ШАПКИ НАВИГАЦИОННОГО ФАЙЛА ГЛОНАСС
    qDebug() << NavG->fileName();
    if (!NavG->fileName().isEmpty())
    {
     RRFHnavG(NavG);
     if(noNavigationFileG==false){
         qDebug() << "[3]GLO RINEX File version: " << versionNavG;
     }
     else{qDebug() << "[202]No Navigation File GLONASS";}
    }
}

void rinexReader::readRinexFileHeader(QFile *file){
    bool ok;
    if(VERS==211 || VERS==210){
        qDebug() << "[!]RINEX 2.11 read header start";
        RRFH210(file);
        qDebug() << "TEST LOG DATA:";
        qDebug() << "version:   " << version;
        qDebug() << "MrkName:   " << MrkName;
        qDebug() << "MrkNmb:    " << MrkNmb;
        qDebug() << "Agency:    " << Agency;
        qDebug() << "RecNumb:   " << RecNumb;
        qDebug() << "TypeName:  " << TypeName;
        qDebug() << "VersName:  " << VersName;
        qDebug() << "AntNumb:   " << AntNumb;
        qDebug() << "TypeName2: " << TypeName2;
        qDebug() << "Xstr:      " << Xstr;
        qDebug() << "Ystr:      " << Ystr;
        qDebug() << "Zstr:      " << Zstr;
        qDebug() << "Hstr:      " << Hstr;
        qDebug() << "Estr:      " << Estr;
        qDebug() << "Nstr:      " << Nstr;
        qDebug() << "Interval:  " << Interval;
        qDebug() << "NofTypes:  " << NofTypes;
        qDebug() << "NofTypesR: " << NofTypesR;
        qDebug() << "NofTypesG: " << NofTypesG;
        qDebug() << "TypesData: " << TypesData;
        qDebug() << "FirstObsData:     " << FirstObsData;
        qDebug() << "LastObsData:      " << LastObsData;
        qDebug() << "NmbOfGLONASS:     " << NmbOfGLONASS;
        qDebug() << "NofRsat:          " << NofRsat;
        qDebug() << "[!]RINEX 2.11 header reading is over";
    }
    if(VERS==302){
        qDebug() << "[!]RINEX 3.02 read header start";
        RRFH302(file);
        qDebug() << "TEST LOG DATA:";
        qDebug() << "version:   " << version;
        qDebug() << "MrkName:   " << MrkName;
        qDebug() << "MrkNmb:    " << MrkNmb;
        qDebug() << "Agency:    " << Agency;
        qDebug() << "RecNumb:   " << RecNumb;
        qDebug() << "TypeName:  " << TypeName;
        qDebug() << "VersName:  " << VersName;
        qDebug() << "AntNumb:   " << AntNumb;
        qDebug() << "TypeName2: " << TypeName2;
        qDebug() << "Xstr:      " << Xstr;
        qDebug() << "Ystr:      " << Ystr;
        qDebug() << "Zstr:      " << Zstr;
        qDebug() << "Hstr:      " << Hstr;
        qDebug() << "Estr:      " << Estr;
        qDebug() << "Nstr:      " << Nstr;
        qDebug() << "Interval:  " << Interval;
        qDebug() << "NofTypes:  " << NofTypes;
        qDebug() << "NofTypesR: " << NofTypesR;
        qDebug() << "NofTypesG: " << NofTypesG;
        qDebug() << "TypesData: " << TypesData;
        qDebug() << "FirstObsData:     " << FirstObsData;
        qDebug() << "LastObsData:      " << LastObsData;
        qDebug() << "NmbOfGLONASS:     " << NmbOfGLONASS;
        qDebug() << "NofRsat:          " << NofRsat;
        qDebug() << "[!]RINEX 3.02 header reading is over";
    }
}

//МЕТОД ЧТЕНИЯ ЭПОХ ИЗ ФАЙЛА RINEX 2.10
QStringList rinexReader::readInputStream(QTextStream *stream){
    QStringList data;
    QString line;
    QString NumberOfSats;
    int NsatsEpoch = 0;
    bool ok;
    int temp;

    /*int i;
    if(strNum==hedNum){
        i = 1;
    }
    else{
        i = strNum+1;
    }*/

    int i = 0;
    if(strNum==hedNum) i = 0;
    else i=strNum;

    int numSats = 2;
    int numMess = 1;
    //qDebug() << "NofTypesInt: " << NofTypesInt;
    if(NofTypesInt>5)numMess = 2;
    if(NofTypesInt>10)numMess = 3;
    while(!stream->atEnd())
    {
        line = stream->readLine();
        i++;
        if (i <= hedNum) continue;
        if (i==strNum+1){
            data.append(line);
            NumberOfSats = data.at(0);
            NumberOfSats.truncate(32); //Убираем из строки символы после 32ого символа
            NumberOfSats = NumberOfSats.right(2); //Сохраняем последние 3 символа в строке
            NsatsEpoch = NumberOfSats.toInt(&ok, 10);
            //Вычисляем число строк содержащих номера спутников ГНСС
            //В одной строке максимально может быть записано 12 идентификаторов спутников
            //Если их больше 12 то используется 2 строки
            //Если их больше 24 то используется 3 строки
            //Если их больше 36 то используется 4 строки
            numSats = 1;
            if(NsatsEpoch>12)numSats = 2;
            if(NsatsEpoch>24)numSats = 3;
            if(NsatsEpoch>36)numSats = 4;
            //numSats = (NsatsEpoch/13)+1;
            //qDebug() << "numSats" << numSats;
            temp = numSats + numMess*NsatsEpoch;
        }
        if(i==strNum+temp){
            strNum = strNum+temp;
            data.append(line);
            break;
        }
        if(i>strNum+1){
            data.append(line);
        }
    }
    return data;
} //КОНЕЦ МЕТОДА

//МЕТОД ЧТЕНИЯ ЭПОХ ИЗ ФАЙЛА RINEX 3.02
QStringList rinexReader::readInputStream302(QTextStream *stream){
    //Переменная для хранения сиска строк текущей эпохи
    QStringList data;
    QString line;
    QString NumberOfSats;
    //Инициализируем число спутников наблюдаемых в текущей эпохе
    int NsatsEpoch = 0;
    //Переменная для работы функции ".toInt"
    bool ok;
    int temp;
    /* Описание работы метода сортировки строк по эпохам
     * strNum - переменная объекта, хранит номер
     * i - счётчик, хранящий номер текущей строки в потоке
     * NsatsEpoch - число спутников наблюдаемых в текущей эпохе
     */
    //Инициализирум счётчик
    int i = 0;
    if(strNum==hedNum){
        i = 0;
    }
    else{
        i = strNum;
    }

    //Число строк для хранения списка спутников, в 3.02 всегда = 1
    const int numSats = 1;
    //Число строк для хранения измерений по сигналам 1 спутника, в 3.02 всегда = 1
    int numMess = 1;
    //Пока поток не закончится, производим следующие операции
    while(!stream->atEnd())
        {
        //Считываем линию
        line = stream->readLine();
            //Инкрементируем счётчик
            i++;
            if (i <= hedNum) continue;
            //проверяем поочёрёдно три условия
            //если номер текущей строки в потоке (i) равен "якорь" +2 (strNum+2), то:
            if (i==strNum+1){
                //Добавляем эту строку к списку строк текущей эпохи (data)
                data.append(line);
                //Получаем из первой строки число спутников наблюдаемых в текущей эпохе (NsatsEpoch)
                NumberOfSats = data.at(0);
                NumberOfSats.truncate(35); //Убираем из строки символы после 32ого символа
                NumberOfSats = NumberOfSats.right(2); //Сохраняем последние 3 символа в строке
                NsatsEpoch = NumberOfSats.toInt(&ok, 10);
                //temp = numSats + numMess*NsatsEpoch;
            }
            //Если текущая строка последняя в эпохе, то:
            //1)задаём новое значение "якоря" как strNum+temp
            //2)добавляем эту строку в список (data)
            //3)принудительно завершаем цикл while
            if(i==strNum+NsatsEpoch+1){
                strNum = strNum+NsatsEpoch+1;
                data.append(line);
                break;
            }
            //Если текущая строка не последняя в эпохе, добавляем её в список (data)
            if(i>strNum+1){
                data.append(line);
            }
        }
    return data;
} //КОНЕЦ МЕТОДА

//МЕТОД ЧТЕНИЯ ШАПКИ ФАЙЛА НАБЛЮДЕНИЙ RINEX 2.10
void rinexReader::RRFH210(QFile *file){
    QList<QString> data;
    quint16 k = 0;
    quint16 GLOLitStart = 0;
    quint8 testIter = 0;
    double Interv;
    int nGLONASS;
    int nmbR;
    bool litDataCheck = 0;
    QString BuffTypes;
    QString BuffTypes2;
    QString BuffTypes3;
    qint8 NT;

    /* ПРАВИЛА СОРТИРОВКИ ИЗМЕРЕНИЙ
     * Массив "TypesOfObsCheck" используется для хранения ИМЕЮЩИХСЯ в файле типов измерений
     * Каждая ячейка массива соответствует одному типу измерений. Соответствие следующее:
     * ==========================================================================
     * | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14
     * | N  | C1 | P1 | L1 | D1 | S1 | C2 | P2 | L2 | D2 | S2 | C5 | L5 | D5 | S5
     * ==========================================================================
     * Массив "NumbInOrderTypepsObs" используется для хранения ПОРЯДКА типов измерений в файле.
     * Значение ячейки соответствует порядковому номеру типа измерений в файле. Соответствие следующее:
     * ==========================================================================
     * | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14
     * | N  | C1 | P1 | L1 | D1 | S1 | C2 | P2 | L2 | D2 | S2 | C5 | L5 | D5 | S5
     * ==========================================================================
     * Порядковый номер ячейки соответствует порядку типов измерений в файле.
     * Лишним ячейкам присваивается значение 1000
     */

    NumbInOrderTypepsObs[0]=0;
    for(int i=1; i<15; i++){
        NumbInOrderTypepsObs[i]=1000;
        TypesOfObsCheck[i]=false;
    }

    qint16 GLONASSlit[26];

    // Проверяем, возможно ли открыть наш файл для чтения

    if (file->open(QIODevice::ReadOnly))
    {
        QTextStream stream(file);
        QString line;
        QString temp ("END OF HEADER");
        QString temp2 ("END OF HEADER       ");
        while(!stream.atEnd())
            {
                line = stream.readLine();
                data.append(line);
                //qDebug() << line;
                strNum++;
                if(line.lastIndexOf(temp)>0 || line.lastIndexOf(temp2)>0)
                {
                    file->close();
                    break;
                }
            }
        //Сохранение номера последней строки заголовка
        hedNum = strNum;
        //Цикл интерпретации данных заголовка
        for(qint32 i = 0; i<data.size(); i++)
        {
            if (data.at(i).endsWith("RINEX VERSION / TYPE"))
            {
                version = data.at(i);
                version.truncate(9); //Убираем из строки символы после 9ого символа
                version = version.right(4); //Сохраняем последние 4 символа в строке
            }
            if (data.at(i).endsWith("MARKER NAME")          || data.at(i).endsWith("MARKER NAME         "))
            {
                MrkName = data.at (i);
                MrkName.truncate(20); //Убираем из строки символы после 3ого символа
            }
            if (data.at(i).endsWith("MARKER NUMBER")        || data.at(i).endsWith("MARKER NUMBER       "))
            {
                MrkNmb = data.at (i);
                MrkNmb.truncate(20); //Убираем из строки символы после 3ого символа
            }
            if (data.at(i).endsWith("OBSERVER / AGENCY")    || data.at(i).endsWith("OBSERVER / AGENCY   "))
            {
                Agency = data.at (i);
                Agency.truncate(40); //Убираем из строки символы после 23ого символа
                Agency = Agency.right(20); //Сохраняем последние 20 символа в строке
            }
            if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
            {
                RecNumb = data.at (i);
                RecNumb.truncate(20); //Убираем из строки символы после 3ого символа
            }
            if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
            {
                TypeName = data.at (i);
                TypeName.truncate(40); //Убираем из строки символы после 3ого символа
                TypeName = TypeName.right(20); //Сохраняем последние 20 символа в строке
            }
            if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
            {
                VersName = data.at (i);
                VersName.truncate(60); //Убираем из строки символы после 3ого символа
                VersName = VersName.right(20); //Сохраняем последние 20 символа в строке
            }
            if (data.at(i).endsWith("ANT # / TYPE")         || data.at(i).endsWith("ANT # / TYPE        "))
            {
                AntNumb = data.at (i);
                AntNumb.truncate(20); //Убираем из строки символы после 3ого символа
            }
            if (data.at(i).endsWith("ANT # / TYPE")         || data.at(i).endsWith("ANT # / TYPE        "))
            {
                TypeName2 = data.at (i);
                TypeName2.truncate(40); //Убираем из строки символы после 3ого символа
                TypeName2 = TypeName2.right(20); //Сохраняем последние 20 символа в строк
            }
            if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
                {
                    Xstr = data.at (i);
                    Xstr.truncate(14); //Убираем из строки символы после 4ого символа
                    Xstr = Xstr.right(12); //Сохраняем последние 20 символа в строк
                    X = Xstr.toDouble();
                }
            if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
                {
                    Ystr = data.at (i);
                    Ystr.truncate(28); //Убираем из строки символы после 3ого символа
                    Ystr = Ystr.right(12); //Сохраняем последние 20 символа в строк
                    Y = Ystr.toDouble();
                }
            if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
                {
                    Zstr = data.at (i);
                    Zstr.truncate(42); //Убираем из строки символы после 3ого символа
                    Zstr = Zstr.right(12); //Сохраняем последние 20 символа в строк
                    Z = Zstr.toDouble();
                }
            if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
                {
                    Hstr = data.at (i);
                    Hstr.truncate(14); //Убираем из строки символы после 4ого символа
                    Hstr = Hstr.right(12); //Сохраняем последние 20 символа в строк
                    H = Hstr.toDouble();
                }
            if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
                {
                    Estr = data.at (i);
                    Estr.truncate(28); //Убираем из строки символы после 3ого символа
                    Estr = Estr.right(12); //Сохраняем последние 20 символа в строк
                    E = Estr.toDouble();
                }
            if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
                {
                    Nstr = data.at (i);
                    Nstr.truncate(42); //Убираем из строки символы после 3ого символа
                    Nstr = Nstr.right(12); //Сохраняем последние 20 символа в строк
                    N = Nstr.toDouble();
                }
            if (data.at(i).endsWith("INTERVAL")             || data.at(i).endsWith("INTERVAL            "))
                {
                    Interval = data.at (i);
                    Interval.truncate(10); //Убираем из строки символы после 3ого символа
                    Interval = Interval.right(6); //Сохраняем последние 20 символа в строк
                    Interv = Interval.toDouble();
                }
            if (data.at(i).endsWith("# / TYPES OF OBSERV")  || data.at(i).endsWith("# / TYPES OF OBSERV "))
                {
                    k++;
                    if(k==1)
                    {NT = i;}
                }
            if (data.at(i).endsWith("# / TYPES OF OBSERV")  || data.at(i).endsWith("# / TYPES OF OBSERV "))
            {
                if(k==1)
                {
                    NofTypes = data.at(i);
                    NofTypes.truncate(6); //Убираем из строки символы после 3ого символа
                    NofTypes = NofTypes.right(2); //Сохраняем последние 20 символа в строк
                    NofTypesInt = NofTypes.toInt();
                }
            }
            if (data.at(i).endsWith("# / TYPES OF OBSERV")  || data.at(i).endsWith("# / TYPES OF OBSERV "))
                {
                if(k==1)
                {
                   BuffTypes = data.at (NT );
                   BuffTypes.truncate(60);
                   BuffTypes = BuffTypes.right(54);
                }
                if(k==2)
                {
                    BuffTypes = data.at (NT);
                    BuffTypes.truncate(60);
                    BuffTypes = BuffTypes.right(54);
                    BuffTypes2 = data.at (NT +1);
                    BuffTypes2.truncate(60);
                    BuffTypes2 = BuffTypes2.right(54);
                    BuffTypes =BuffTypes.append(BuffTypes2);
                }
                if(k==3)
                {
                    BuffTypes = data.at (NT );
                    BuffTypes.truncate(60);
                    BuffTypes = BuffTypes.right(54);
                    BuffTypes2 = data.at (NT +1);
                    BuffTypes2.truncate(60);
                    BuffTypes2 = BuffTypes2.right(54);
                    BuffTypes3 = data.at (NT +2);
                    BuffTypes3.truncate(60);
                    BuffTypes3 = BuffTypes3.right(54);
                    BuffTypes =BuffTypes.append(BuffTypes2).append(BuffTypes3);
                }

                TypesData = BuffTypes;
                QStringList BufTypesData = TypesData.split("    ");
                qDebug()<<BuffTypes;
                /* Массив "TypesOfObsCheck" используется для хранения ИМЕЮЩИХСЯ в файле типов измерений
                * Каждая ячейка массива соответствует одному типу измерений. Соответствие следующее:
                * ==========================================================================
                * | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14
                * | N  | C1 | P1 | L1 | D1 | S1 | C2 | P2 | L2 | D2 | S2 | C5 | L5 | D5 | S5
                * ==========================================================================
                * Массив "NumbInOrderTypepsObs" используется для хранения ПОРЯДКА типов измерений в файле.
                * Значение ячейки соответствует порядковому номеру типа измерений в файле. Соответствие следующее:
                * ==========================================================================
                * | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14
                * | N  | C1 | P1 | L1 | D1 | S1 | C2 | P2 | L2 | D2 | S2 | C5 | L5 | D5 | S5
                * ==========================================================================
                * Порядковый номер ячейки соответствует порядку типов измерений в файле.
                * Лишним ячейкам присваивается значение 1000
                */

                for (int it = 0; it < BufTypesData.size(); it++)
                {
                    if(BufTypesData.at(it).contains("C1", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[1] = true;
                        NumbInOrderTypepsObs[it] = 1;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("P1", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[2] = true;
                        NumbInOrderTypepsObs[it] = 2;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("L1", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[3] = true;
                        NumbInOrderTypepsObs[it] = 3;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("D1", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[4] = true;
                        NumbInOrderTypepsObs[it] = 4;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("S1", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[5] = true;
                        NumbInOrderTypepsObs[it] = 5;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("C2", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[6] = true;
                        NumbInOrderTypepsObs[it] = 6;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("P2", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[7] = true;
                        NumbInOrderTypepsObs[it] = 7;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("L2", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[8] = true;
                        NumbInOrderTypepsObs[it] = 8;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("D2", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[9] = true;
                        NumbInOrderTypepsObs[it] = 9;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("S2", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[10] = true;
                        NumbInOrderTypepsObs[it] = 10;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("C5", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[11] = true;
                        NumbInOrderTypepsObs[it] = 11;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("L5", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[12] = true;
                        NumbInOrderTypepsObs[it] = 12;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("D5", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[13] = true;
                        NumbInOrderTypepsObs[it] = 13;
                        NumbInOrderTypepsObs[0]++;
                    }
                    if(BufTypesData.at(it).contains("S5", Qt::CaseInsensitive))
                    {
                        TypesOfObsCheck[14] = true;
                        NumbInOrderTypepsObs[it] = 14;
                        NumbInOrderTypepsObs[0]++;
                    }
                }
                if(k>1)
                    NumbInOrderTypepsObs[0] = NumbInOrderTypepsObs[0]-(k-1)*9;
            }
            if (data.at(i).endsWith("TIME OF FIRST OBS")  || data.at(i).endsWith("TIME OF FIRST OBS   "))
            {
                //Получение даты начала измерений (статическая переменная timeOfFirst)

                YearF = data.at(i);
                YearF.truncate(6); //Убираем из строки символы после 6ого символа
                YearF = YearF.right(4); //Сохраняем последние 4 символа в строк
                int Year = YearF.toInt();
                MonthF = data.at(i);
                MonthF.truncate(12); //Убираем из строки символы после 12ого символа
                MonthF = MonthF.right(2); //Сохраняем последние 2 символа в строк
                int Month = MonthF.toInt();
                DayF = data.at(i);
                DayF.truncate(18); //Убираем из строки символы после 18ого символа
                DayF = DayF.right(2); //Сохраняем последние 2 символа в строк
                int Day = DayF.toInt();
                HourF = data.at(i);
                HourF.truncate(24); //Убираем из строки символы после 24ого символа
                HourF = HourF.right(2); //Сохраняем последние 2 символа в строк
                int Hour = HourF.toInt();
                MinuteF = data.at(i);
                MinuteF.truncate(30); //Убираем из строки символы после 30ого символа
                MinuteF = MinuteF.right(2); //Сохраняем последние 2 символа в строк
                int Minute = MinuteF.toInt();
                SecondF = data.at(i);
                SecondF.truncate(43); //Убираем из строки символы после 43ого символа
                SecondF = SecondF.right(10); //Сохраняем последние 10 символа в строк
                qreal Second = SecondF.toDouble();
                QDate temp;
                temp.setDate(Year,Month,Day);
                QTime time;
                int tempSec = Second;
                int tempMSec = Second*1000;
                tempMSec = tempMSec-tempSec*1000;
                time.setHMS(Hour,Minute,tempSec,tempMSec);
                QDateTime *ddd = new QDateTime(temp,time,Qt::OffsetFromUTC);
                if(ddd->isValid())
                    timeOfFirst = ddd;

            }
            if (data.at(i).endsWith("TIME OF LAST OBS")  || data.at(i).endsWith("TIME OF LAST OBS    "))
            {
                YearL = data.at(i);
                YearL.truncate(6); //Убираем из строки символы после 6ого символа
                YearL = YearL.right(4); //Сохраняем последние 4 символа в строк
                int Year = YearL.toInt();
                MonthL = data.at(i);
                MonthL.truncate(12); //Убираем из строки символы после 12ого символа
                MonthL = MonthL.right(2); //Сохраняем последние 2 символа в строк
                int Month = MonthL.toInt();
                DayL = data.at(i);
                DayL.truncate(18); //Убираем из строки символы после 18ого символа
                DayL = DayL.right(2); //Сохраняем последние 2 символа в строк
                int Day = DayL.toInt();
                HourL = data.at(i);
                HourL.truncate(24); //Убираем из строки символы после 24ого символа
                HourL = HourL.right(2); //Сохраняем последние 2 символа в строк
                int Hour = HourL.toInt();
                MinuteL = data.at(i);
                MinuteL.truncate(30); //Убираем из строки символы после 30ого символа
                MinuteL = MinuteL.right(2); //Сохраняем последние 2 символа в строк
                int Minute = MinuteL.toInt();
                SecondL = data.at(i);
                SecondL.truncate(43); //Убираем из строки символы после 43ого символа
                SecondL = SecondL.right(10); //Сохраняем последние 10 символа в строк
                qreal Second = SecondL.toDouble();
                QDate temp;
                temp.setDate(Year,Month,Day);
                QTime time;
                int tempSec = Second;
                int tempMSec = Second*1000;
                tempMSec = tempMSec-tempSec*1000;
                time.setHMS(Hour,Minute,tempSec,tempMSec);
                QDateTime *ddd = new QDateTime(temp,time,Qt::OffsetFromUTC);
                if(ddd->isValid())
                    timeOfLast = ddd;
            }
            if (data.at(i).endsWith("GLONASS SLOT / FRQ #"))
                {
                litDataCheck = 1;
                    testIter++;
                    if(testIter == 1)
                    {
                    GLOLitStart = i;
                    NmbOfGLONASS = data.at (i);
                    NmbOfGLONASS.truncate(3); //Убираем из строки символы после 3ого символа
                    NmbOfGLONASS = NmbOfGLONASS.right(2); //Сохраняем последние 2 символа в строке
                    nGLONASS = NmbOfGLONASS.toInt();
                    LitOfGLONASSBuff = data.at (i);
                    LitOfGLONASSBuff.truncate(59); //Убираем из строки символы после 4ого символа
                    LitOfGLONASSBuff = LitOfGLONASSBuff.right(55); //Сохраняем последние 20 символа в строк
                    }
                    if(testIter > 1)
                    {

                    BuffStr = data.at (i);
                    BuffStr.truncate(59); //Убираем из строки символы после 4ого символа
                    BuffStr = BuffStr.right(55); //Сохраняем последние 20 символа в строк
                    LitOfGLONASSBuff.append(" ").append(BuffStr);
                    }
                    //LitOfGLONASSBuff;
                    QStringList BufferListData = LitOfGLONASSBuff.split(" ", QString::SkipEmptyParts);
                    //qDebug() <<  "BufferListData = " << BufferListData;
                    for (int it = 0; it < BufferListData.size(); ++it)
                    {
                        i = it;
                        NofRsat = BufferListData.at (i);
                        if( BufferListData.at (i).contains("R", Qt::CaseInsensitive))
                        {
                            NofRsat = NofRsat.right(2); //Сохраняем последние 20 символа в строк
                            nmbR = NofRsat.toInt();
                            GLONASSlit [nmbR-1] = BufferListData.at(i+1).toInt();
                        }
                    }
                    //Запись литер
                    for(int t=0; t< 40; t++){
                        lit[t] = 255;
                    }

                    LitOfGLONASSBuff = data.at(GLOLitStart);
                    LitOfGLONASSBuff.truncate(59); //Убираем из строки символы после 4ого символа
                    LitOfGLONASSBuff = LitOfGLONASSBuff.right(55); //Сохраняем последние 20 символа в строк

                }
            if ( litDataCheck!=1)
            {
                for(int t=0; t< 40; t++)
                {
                    lit[t] = 255;
                }

            }
            for (int it = 1; it < testIter; it++)
            {
            BuffStr = data.at(GLOLitStart+it);
            BuffStr.truncate(59); //Убираем из строки символы после 4ого символа
            BuffStr = BuffStr.right(55); //Сохраняем последние 20 символа в строк
            LitOfGLONASSBuff.append(" ").append(BuffStr);
            }
            QStringList BufferListData = LitOfGLONASSBuff.split(" ", QString::SkipEmptyParts);
            for (int it = 0; it < BufferListData.size(); ++it)
            {
                NofRsat = BufferListData.at(it);
                if( BufferListData.at(it).contains("R", Qt::CaseInsensitive))
                {
                    NofRsat = NofRsat.right(2); //Сохраняем последние 20 символа в строк
                    nmbR = NofRsat.toInt();
                    lit [nmbR] = BufferListData.at(it+1).toInt();
                }
            }
        }
        //Закрытие файла
//        file->close();
        noObservationFile = false;
    }
    else
    {
        qDebug() << "open error : noObservationFile = true";
        noObservationFile = true;
        converterJps::errorList.append(205);
    }
} //КОНЕЦ МЕТОДА

//МЕТОД ЧТЕНИЯ ШАПКИ ФАЙЛА НАБЛЮДЕНИЙ RINEX 3.02
void rinexReader::RRFH302(QFile *file){
     qDebug()<<"RRFH302";
     QList<QString> data;

     QDateTime FirstObs;
     QDateTime LastObs;
     QString formatOfTime1 = "yyyy    M    d    h    m    mm    s.zzz";
     QString formatOfTime2 = "yyyy    MM    dd    yy    hh    mm    ss.zzz";

     quint16 k = 0;
     quint16 GLOLitStart = 0;
     quint8 testIter = 0;
     quint8 isGPS = 0;
     quint8 isGLO = 0;

     int nGLONASS;
     int nmbR;
     bool ok;

     /* ПРАВИЛА СОРТИРОВКИ ИЗМЕРЕНИЙ
      * ДЛЯ СИСТЕМЫ GPS
      * Массив "NumbInOrderTypepsObsGPS" используется для хранения ПОРЯДКА типов измерений в файле.
      * Значение ячейки соответствует порядковому номеру типа измерений в файле.
      * Лишним ячейкам присваивается значение 1000.
      * Ячейка NumbInOrderTypepsObsGPS[0] хранит число типов измерений по сигналам GPS имеющихся в файле.
      * Массив "TypesOfObsCheckGPS" используется для хранения ИМЕЮЩИХСЯ в файле типов измерений
      * Каждая ячейка массива соответствует одному типу измерений.
      * Соответствие следующее:
      * ==================================================================================================
      * | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  |
      * | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
      * ---------------------------------------------------------------------------
      * | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  | 27  | 28  | 29  | 30  | 31  | 32  |
      * | C5C | L5C | D5C | S5C | C5P | L5P | D5P | S5P | --- | --- | --- | --- | --- | --- | --- | --- |
      * ==================================================================================================
      * ДЛЯ СИСТЕМЫ ГЛОНАСС
      * Массив "NumbInOrderTypepsObsGLO" используется для хранения ПОРЯДКА типов измерений в файле.
      * Значение ячейки соответствует порядковому номеру типа измерений в файле.
      * Лишним ячейкам присваивается значение 1000.
      * Ячейка NumbInOrderTypepsObsGLO[0] хранит число типов измерений по сигналам ГЛОНАСС имеющихся в файле.
      * Массив "TypesOfObsCheckGLO" используется для хранения ИМЕЮЩИХСЯ в файле типов измерений
      * Каждая ячейка массива соответствует одному типу измерений.
      * Соответствие следующее:
      * ==================================================================================================
      * | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  |
      * | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
      * ==================================================================================================
      */

     NumbInOrderTypepsObsGPS[0]=0;
     for(int i=1; i<36; i++){
         NumbInOrderTypepsObsGPS[i]=1000;
         TypesOfObsCheckGPS[i]=false;
     }
     NumbInOrderTypepsObsGLO[0]=0;
     for(int i=1; i<17; i++){
         NumbInOrderTypepsObsGLO[i]=1000;
         TypesOfObsCheckGLO[i]=false;
     }
     // Проверяем, возможно ли открыть наш файл для чтения
     if (file->open(QIODevice::ReadOnly))
         {
         QTextStream stream(file);
         QString line;
         QString temp ("END OF HEADER");
         while(!stream.atEnd())
             {
                 line = stream.readLine();
                 data.append(line);
                 strNum++;
                 if(line.lastIndexOf(temp)>0)
                 {
                     file->close();
                     break;
                 }
             }
         file->close();
         }
         else
         {
             qDebug() << "3003";
         }
     hedNum = strNum;

     for(qint32 i = 0; i<data.size(); i++)
     {
         if (data.at(i).endsWith("RINEX VERSION / TYPE"))
         {
             version = data.at(i);
             version.truncate(9); //Убираем из строки символы после 9ого символа
             version = version.right(4); //Сохраняем последние 4 символа в строке
         }
         if (data.at(i).endsWith("MARKER NAME")          || data.at(i).endsWith("MARKER NAME         "))
         {
             MrkName = data.at(i);
             MrkName.truncate(20); //Убираем из строки символы после 3ого символа
         }
         if (data.at(i).endsWith("MARKER NUMBER")        || data.at(i).endsWith("MARKER NUMBER       "))
         {
             MrkNmb = data.at(i);
             MrkNmb.truncate(20); //Убираем из строки символы после 3ого символа
         }
         if (data.at(i).endsWith("OBSERVER / AGENCY")    || data.at(i).endsWith("OBSERVER / AGENCY   "))
         {
             Agency = data.at(i);
             Agency.truncate(40); //Убираем из строки символы после 23ого символа
             Agency = Agency.right(20); //Сохраняем последние 20 символа в строке
         }
         if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
         {
             RecNumb = data.at(i);
             RecNumb.truncate(20); //Убираем из строки символы после 3ого символа
         }
         if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
         {
             TypeName = data.at(i);
             TypeName.truncate(40); //Убираем из строки символы после 3ого символа
             TypeName = TypeName.right(20); //Сохраняем последние 20 символа в строке
         }
         if (data.at(i).endsWith("REC # / TYPE / VERS")  || data.at(i).endsWith("REC # / TYPE / VERS "))
         {
             VersName = data.at(i);
             VersName.truncate(60); //Убираем из строки символы после 3ого символа
             VersName = VersName.right(20); //Сохраняем последние 20 символа в строке
         }
         if (data.at(i).endsWith("ANT # / TYPE")         || data.at(i).endsWith("ANT # / TYPE        "))
         {

             AntNumb = data.at(i);
             AntNumb.truncate(20); //Убираем из строки символы после 3ого символа
         }
         if (data.at(i).endsWith("ANT # / TYPE")         || data.at(i).endsWith("ANT # / TYPE        "))
         {
             TypeName2 = data.at(i);
             TypeName2.truncate(40); //Убираем из строки символы после 3ого символа
             TypeName2 = TypeName2.right(20); //Сохраняем последние 20 символа в строк
         }
         if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
             {
                 Xstr = data.at(i);
                 Xstr.truncate(14); //Убираем из строки символы после 4ого символа
                 Xstr = Xstr.right(12); //Сохраняем последние 20 символа в строк
                 X = Xstr.toDouble();
             }
         if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
             {
                 Ystr = data.at(i);
                 Ystr.truncate(28); //Убираем из строки символы после 3ого символа
                 Ystr = Ystr.right(12); //Сохраняем последние 20 символа в строк
                 Y = Ystr.toDouble();
             }
         if (data.at(i).endsWith("APPROX POSITION XYZ")  || data.at(i).endsWith("APPROX POSITION XYZ "))
             {
                 Zstr = data.at(i);
                 Zstr.truncate(42); //Убираем из строки символы после 3ого символа
                 Zstr = Zstr.right(12); //Сохраняем последние 20 символа в строк
                 Z = Zstr.toDouble();
             }
         if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
             {
                 Hstr = data.at(i);
                 Hstr.truncate(14); //Убираем из строки символы после 4ого символа
                 Hstr = Hstr.right(12); //Сохраняем последние 20 символа в строк
                 H = Hstr.toDouble();
             }
         if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
             {
                 Estr = data.at(i);
                 Estr.truncate(28); //Убираем из строки символы после 3ого символа
                 Estr = Estr.right(12); //Сохраняем последние 20 символа в строк
                 E = Estr.toDouble();
             }
         if (data.at(i).endsWith("ANTENNA: DELTA H/E/N"))
             {
                 Nstr = data.at(i);
                 Nstr.truncate(42); //Убираем из строки символы после 3ого символа
                 Nstr = Nstr.right(12); //Сохраняем последние 20 символа в строк
                 N = Nstr.toDouble();
             }
         if (data.at(i).endsWith("INTERVAL")             || data.at(i).endsWith("INTERVAL            "))
             {
                 Interval = data.at(i);
                 Interval.truncate(10); //Убираем из строки символы после 3ого символа
                 Interval = Interval.right(6); //Сохраняем последние 20 символа в строк
             }

         /*  Считывание времени начала и времени окончания сеанса наблюдений
          *  Данные сохраняются в переменных типа QDateTime
          *  Время начала наблюдений - QDateTime *timeOfFirst
          *  Момент окончания наблюдений - QDateTime *timeOfLast
          */
         if (data.at(i).endsWith("TIME OF FIRST OBS")  || data.at(i).endsWith("TIME OF FIRST OBS   "))
         {
             //Получение даты начала измерений (статическая переменная timeOfFirst)

             YearF = data.at(i);
             YearF.truncate(6); //Убираем из строки символы после 6ого символа
             YearF = YearF.right(4); //Сохраняем последние 4 символа в строк
             int Year = YearF.toInt();
             MonthF = data.at(i);
             MonthF.truncate(12); //Убираем из строки символы после 12ого символа
             MonthF = MonthF.right(2); //Сохраняем последние 2 символа в строк
             int Month = MonthF.toInt();
             DayF = data.at(i);
             DayF.truncate(18); //Убираем из строки символы после 18ого символа
             DayF = DayF.right(2); //Сохраняем последние 2 символа в строк
             int Day = DayF.toInt();
             HourF = data.at(i);
             HourF.truncate(24); //Убираем из строки символы после 24ого символа
             HourF = HourF.right(2); //Сохраняем последние 2 символа в строк
             int Hour = HourF.toInt();
             MinuteF = data.at(i);
             MinuteF.truncate(30); //Убираем из строки символы после 30ого символа
             MinuteF = MinuteF.right(2); //Сохраняем последние 2 символа в строк
             int Minute = MinuteF.toInt();
             SecondF = data.at(i);
             SecondF.truncate(43); //Убираем из строки символы после 43ого символа
             SecondF = SecondF.right(10); //Сохраняем последние 10 символа в строк
             qreal Second = SecondF.toDouble();
             QDate temp;
             temp.setDate(Year,Month,Day);
             QTime time;
             int tempSec = Second;
             int tempMSec = Second*1000;
             tempMSec = tempMSec-tempSec*1000;
             time.setHMS(Hour,Minute,tempSec,tempMSec);

             QDateTime *ddd = new QDateTime(temp,time,Qt::OffsetFromUTC);

             if(ddd->isValid())
                 timeOfFirst = ddd;

         }         
         if (data.at(i).endsWith("TIME OF LAST OBS")  || data.at(i).endsWith("TIME OF LAST OBS    "))
         {
             YearL = data.at(i);
             YearL.truncate(6); //Убираем из строки символы после 6ого символа
             YearL = YearL.right(4); //Сохраняем последние 4 символа в строк
             int Year = YearL.toInt();
             MonthL = data.at(i);
             MonthL.truncate(12); //Убираем из строки символы после 12ого символа
             MonthL = MonthL.right(2); //Сохраняем последние 2 символа в строк
             int Month = MonthL.toInt();
             DayL = data.at(i);
             DayL.truncate(18); //Убираем из строки символы после 18ого символа
             DayL = DayL.right(2); //Сохраняем последние 2 символа в строк
             int Day = DayL.toInt();
             HourL = data.at(i);
             HourL.truncate(24); //Убираем из строки символы после 24ого символа
             HourL = HourL.right(2); //Сохраняем последние 2 символа в строк
             int Hour = HourL.toInt();
             MinuteL = data.at(i);
             MinuteL.truncate(30); //Убираем из строки символы после 30ого символа
             MinuteL = MinuteL.right(2); //Сохраняем последние 2 символа в строк
             int Minute = MinuteL.toInt();
             SecondL = data.at(i);
             SecondL.truncate(43); //Убираем из строки символы после 43ого символа
             SecondL = SecondL.right(10); //Сохраняем последние 10 символа в строк
             qreal Second = SecondL.toDouble();
             QDate temp;
             temp.setDate(Year,Month,Day);
             QTime time;
             int tempSec = Second;
             int tempMSec = Second*1000;
             tempMSec = tempMSec-tempSec*1000;
             time.setHMS(Hour,Minute,tempSec,tempMSec);
             QDateTime *ddd = new QDateTime(temp,time,Qt::OffsetFromUTC);
             if(ddd->isValid())
                 timeOfLast = ddd;
         }
         if (data.at(i).endsWith("SYS / # / OBS TYPES")  || data.at(i).endsWith("SYS / # / OBS TYPES "))
             {
                 k++;
             }
         if (data.at(i).endsWith("SYS / # / OBS TYPES")  || data.at(i).endsWith("SYS / # / OBS TYPES "))
             {
                 NofTypes = data.at(i);
                 //Проверяем тип системы
                 if(data.at(i).contains("G", Qt::CaseInsensitive))
                 {isGPS=1;isGLO=0;}
                 if(data.at(i).contains("R", Qt::CaseInsensitive))
                 {isGPS=0;isGLO=1;}
                 NofTypes.truncate(6); //Убираем из строки символы после 3ого символа
                 NofTypes = NofTypes.right(2); //Сохраняем последние 20 символа в строк
                 NofTypesInt = NofTypes.toInt(&ok, 10);
                 if(isGLO==1){NofTypesR = NofTypes;}
                 if(isGPS==1){NofTypesG = NofTypes;}
             }
         if (data.at(i).endsWith("SYS / # / OBS TYPES")  || data.at(i).endsWith("SYS / # / OBS TYPES "))
             {
                 TypesData = data.at(i);
                 TypesData.truncate(60); //Убираем из строки символы после 60ого символа
                 TypesData = TypesData.right(53); //Сохраняем последние 50 символов в строке
                 // | 0   |
                 // | N   |
                 // --------------------------------------------------------------------------------------------------
                 // | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10   | 11  | 12  | 13  | 14  | 15  | 16  |
                 // | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C  | D2C | S2C | C2P | L2P | D2P | S2P |
                 // --------------------------------------------------------------------------------------------------
                 // | 17  | 18  | 19  | 20  | 21  | 22  | 23  | 24  | 25  | 26  | 27  | 28  | 29  | 30  | 31  | 32  |
                 // | C5C | L5C | D5C | S5C | C5P | L5P | D5P | S5P | --- | --- | --- | --- | --- | --- | --- | --- |
                 if (isGPS == 1)
                 {
                     QStringList BufTypesData = TypesData.split(" ");
                     for (int it = 0; it < BufTypesData.size(); ++it)
                     {
                         if(BufTypesData.at(it).contains("C1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[1] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 1;
                             NumbInOrderTypepsObsGPS[0]++;
                             qDebug()<<"NumbInOrderTypepsObs[it]"<<NumbInOrderTypepsObs[it];
                             qDebug()<<"it"<<it;
                         }
                         if(BufTypesData.at(it).contains("L1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[2] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 2;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("D1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[3] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 3;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("S1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[4] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 4;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("C1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[5] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 5;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("L1P", Qt::CaseInsensitive))
                         {
                            TypesOfObsCheckGPS[6] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 6;
                         }
                         if(BufTypesData.at(it).contains("D1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[7] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 7;
                         }
                         if(BufTypesData.at(it).contains("S1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[8] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 8;
                         }


                         if(BufTypesData.at(it).contains("C2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[9] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 9;
                         }
                         if(BufTypesData.at(it).contains("L2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[10] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 10;
                         }
                         if(BufTypesData.at(it).contains("D2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[11] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 11;
                         }
                         if(BufTypesData.at(it).contains("S2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[12] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 12;
                         }
                         if(BufTypesData.at(it).contains("C2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[13] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 13;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("L2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[14] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 14;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("D2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[15] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 15;
                             NumbInOrderTypepsObsGPS[0]++;
                         }
                         if(BufTypesData.at(it).contains("S2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[16] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 16;
                             NumbInOrderTypepsObsGPS[0]++;
                         }


                         if(BufTypesData.at(it).contains("C5C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[17] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 17;
                         }
                         if(BufTypesData.at(it).contains("L5C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[18] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 18;
                         }
                         if(BufTypesData.at(it).contains("D5C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[19] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 19;
                         }
                         if(BufTypesData.at(it).contains("S5C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[20] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 20;
                         }
                         if(BufTypesData.at(it).contains("C5P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[21] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 21;
                         }
                         if(BufTypesData.at(it).contains("L5P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[22] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 22;
                         }
                         if(BufTypesData.at(it).contains("D5P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[23] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 23;
                         }
                         if(BufTypesData.at(it).contains("S5P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGPS[24] = true;
                             NumbInOrderTypepsObsGPS[it+1] = 24;
                         }


                    }
                 }
                 // =======================================================================================================
                 // | 0   | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 16  |
                 // | N   | C1C | L1C | D1C | S1C | C1P | L1P | D1P | S1P | C2C | L2C | D2C | S2C | C2P | L2P | D2P | S2P |
                 // =======================================================================================================
                 if (isGLO == 1)
                 {
                     QStringList BufTypesData = TypesData.split(" ");
                     for (int it = 0; it < BufTypesData.size(); ++it)
                     {
                         if(BufTypesData.at(it).contains("C1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[1] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 1;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("L1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[2] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 2;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("D1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[3] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 3;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("S1C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[4] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 4;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("C1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[5] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 5;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("L1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[6] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 6;
                         }
                         if(BufTypesData.at(it).contains("D1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[7] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 7;
                         }
                         if(BufTypesData.at(it).contains("S1P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[8] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 8;
                         }
                         if(BufTypesData.at(it).contains("C2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[9] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 9;
                         }
                         if(BufTypesData.at(it).contains("L2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[10] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 10;
                         }
                         if(BufTypesData.at(it).contains("D2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[11] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 11;
                         }
                         if(BufTypesData.at(it).contains("S2C", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[12] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 12;
                         }
                         if(BufTypesData.at(it).contains("C2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[13] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 13;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("L2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[14] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 14;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("D2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[15] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 15;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                         if(BufTypesData.at(it).contains("S2P", Qt::CaseInsensitive))
                         {
                             TypesOfObsCheckGLO[16] = true;
                             NumbInOrderTypepsObsGLO[it+1] = 16;
                             NumbInOrderTypepsObsGLO[0]++;
                         }
                     }
                 }
             }
         if (data.at(i).endsWith("TIME OF FIRST OBS")    || data.at(i).endsWith("TIME OF FIRST OBS   "))
             {
                 i = i;
                 FirstObsData = data.at(i);
                 FirstObsData.truncate(39); //Убираем из строки символы после 60ого символа
                 FirstObsData = FirstObsData.right(37); //Сохраняем последние 50 символов в строке
                 FirstObs = QDateTime::fromString(FirstObsData, formatOfTime1);
             }
         if (data.at(i).endsWith("TIME OF LAST OBS")     || data.at(i).endsWith("TIME OF LAST OBS    "))
             {
                 i = i;
                 LastObsData = data.at(i);
                 LastObsData.truncate(39); //Убираем из строки символы после 60ого символа
                 LastObsData = LastObsData.right(37); //Сохраняем последние 50 символов в строке
                 LastObs = QDateTime::fromString(LastObsData, formatOfTime1);
             }
         if (data.at(i).endsWith("GLONASS SLOT / FRQ #"))
             {
                 testIter=testIter+1;
         }
         if (data.at(i).endsWith("GLONASS SLOT / FRQ #"))
             {
                 if(testIter == 1)
                 {
                 GLOLitStart = i;
                 NmbOfGLONASS = data.at(i);
                 NmbOfGLONASS.truncate(3); //Убираем из строки символы после 4ого символа
                 NmbOfGLONASS = NmbOfGLONASS.right(2); //Сохраняем последние 20 символа в строк
                 nGLONASS = NmbOfGLONASS.toInt();
                 }
             }
         //Запись литер
         for(int t=0; t< 40; t++){
             lit[t] = 255;
         }
         LitOfGLONASSBuff = data.at(GLOLitStart);
         LitOfGLONASSBuff.truncate(59); //Убираем из строки символы после 4ого символа
         LitOfGLONASSBuff = LitOfGLONASSBuff.right(55); //Сохраняем последние 20 символа в строк
         }
         for (int it = 1; it < testIter; it++)
         {
         BuffStr = data.at(GLOLitStart+it);
         BuffStr.truncate(59);
         BuffStr = BuffStr.right(55);
         LitOfGLONASSBuff.append(" ").append(BuffStr);
         }
         QStringList BufferListData = LitOfGLONASSBuff.split(" ", QString::SkipEmptyParts);
         for (int it = 0; it < BufferListData.size(); ++it)
         {
             NofRsat = BufferListData.at(it);
             if( BufferListData.at(it).contains("R", Qt::CaseInsensitive))
             {
                 NofRsat = NofRsat.right(2);
                 nmbR = NofRsat.toInt();
                 lit [nmbR] = BufferListData.at(it+1).toInt();
             }
         }
} //КОНЕЦ МЕТОДА

//МЕТОД ДЛЯ ЧТЕНИЯ ШАПКИ НАВИГАЦИОННОГО ФАЙЛА GPS RINEX
void rinexReader::RRFHnav(QFile *file){
    QList<QString> data;
    strNumNav=0;
    // Проверяем, возможно ли открыть наш файл для чтения

//    QFile *f = new QFile("home/user3/temp/621g057a.18N");
//    if (f->open(QIODevice::ReadOnly)){ qDebug()<<"Work!"; }else{ qDebug()<<"NO"; }

    if (file->open(QIODevice::ReadOnly))
    {
        QTextStream stream(file);
        QString line;
        QString temp  ( "END OF HEADER" );
        QString temp2 ( "END OF HEADER       " );

        while(!stream.atEnd())
        {
            line = stream.readLine();
            data.append(line);
            strNumNav++;
            if(line.lastIndexOf(temp)>0 || line.lastIndexOf(temp2)>0)
            {
                file->close();
                break;
            }
        }

        file->close();
        noNavigationFile = false;

        hedNumNav = strNumNav;
        for(qint32 i = 0; i<data.size(); i++)
        {
            if (data.at(i).endsWith("RINEX VERSION / TYPE"))
            {
                versionNav = data.at(i);
                versionNav.truncate(9); //Убираем из строки символы после 9ого символа
                versionNav = versionNav.right(4); //Сохраняем последние 4 символа в строке
            }
            if ((data.at(i).endsWith("ION ALPHA"))||(data.at(i).endsWith("ION ALPHA           ")))
            {
                ionAOn=true;
                ion_a0 = data.at(i);
                ion_a0.truncate(14); //Убираем из строки символы после 9ого символа
                ion_a0 = ion_a0.right(12); //Сохраняем последние 4 символа в строке
                ion_a0R = ion_a0.toDouble();
                ion_a1 = data.at(i);
                ion_a1.truncate(26); //Убираем из строки символы после 9ого символа
                ion_a1 = ion_a1.right(12); //Сохраняем последние 4 символа в строке
                ion_a1R = ion_a1.toDouble();
                ion_a2 = data.at(i);
                ion_a2.truncate(38); //Убираем из строки символы после 9ого символа
                ion_a2 = ion_a2.right(12); //Сохраняем последние 4 символа в строке
                ion_a2R = ion_a2.toDouble();
                ion_a3 = data.at(i);
                ion_a3.truncate(50); //Убираем из строки символы после 9ого символа
                ion_a3 = ion_a3.right(12); //Сохраняем последние 4 символа в строке
                ion_a3R = ion_a3.toDouble();
            }
            if ((data.at(i).endsWith("ION BETA"))||(data.at(i).endsWith("ION BETA            ")))
            {
                ionBOn=true;
                ion_b0 = data.at(i);
                ion_b0.truncate(14); //Убираем из строки символы после 9ого символа
                ion_b0 = ion_b0.right(12); //Сохраняем последние 4 символа в строке
                ion_b0R = ion_b0.toDouble();
                ion_b1 = data.at(i);
                ion_b1.truncate(26); //Убираем из строки символы после 9ого символа
                ion_b1 = ion_b1.right(12); //Сохраняем последние 4 символа в строке
                ion_b1R = ion_b1.toDouble();
                ion_b2 = data.at(i);
                ion_b2.truncate(38); //Убираем из строки символы после 9ого символа
                ion_b2 = ion_b2.right(12); //Сохраняем последние 4 символа в строке
                ion_b2R = ion_b2.toDouble();
                ion_b3 = data.at(i);
                ion_b3.truncate(50); //Убираем из строки символы после 9ого символа
                ion_b3 = ion_b3.right(12); //Сохраняем последние 4 символа в строке
                ion_b3R = ion_b3.toDouble();
            }
            if ((data.at(i).endsWith("DELTA-UTC: A0,A1,T,W")))
            {
                TgpsOn=true;
                    a0 = data.at(i);
                    a0.truncate(22); //Убираем из строки символы после 9ого символа
                    a0 = a0.right(19); //Сохраняем последние 4 символа в строке
                    a0R = a0.toDouble();
                    a1 = data.at(i);
                    a1.truncate(41); //Убираем из строки символы после 9ого символа
                    a1 = a1.right(19); //Сохраняем последние 4 символа в строке
                    a1R = a1.toDouble();
                    W = data.at(i);
                    W.truncate(50); //Убираем из строки символы после 9ого символа
                    W = W.right(9); //Сохраняем последние 4 символа в строке
                    Wr = W.toDouble();
                    T = data.at(i);
                    T.truncate(59); //Убираем из строки символы после 9ого символа
                    T = T.right(9); //Сохраняем последние 4 символа в строке
                    Tr = T.toDouble();
            }
            if ((data.at(i).endsWith("TIME SYSTEM CORR"))||(data.at(i).endsWith("TIME SYSTEM CORR    ")))
            {
                TimeSysCorr = data.at(i);
                TimeSysCorr.truncate(4); //Убираем из строки символы после 9ого символа
                if(TimeSysCorr =="GPUT")
                {
                    TgpsOn=true;
                    a0 = data.at(i);
                    a0.truncate(22); //Убираем из строки символы после 9ого символа
                    a0 = a0.right(17); //Сохраняем последние 4 символа в строке
                    a0R = a0.toDouble();
                    a1 = data.at(i);
                    a1.truncate(38); //Убираем из строки символы после 9ого символа
                    a1 = a1.right(16); //Сохраняем последние 4 символа в строке
                    a1R = a1.toDouble();
                    W = data.at(i);
                    W.truncate(45); //Убираем из строки символы после 9ого символа
                    W = W.right(7); //Сохраняем последние 4 символа в строке
                    Wr = W.toDouble();
                    T = data.at(i);
                    T.truncate(50); //Убираем из строки символы после 9ого символа
                    T = T.right(5); //Сохраняем последние 4 символа в строке
                    Tr = T.toDouble();
                }
                if(TimeSysCorr =="GLUT")
                {
                    TgloOn=true;
                    a0_TauC = data.at(i);
                    a0_TauC.truncate(22); //Убираем из строки символы после 9ого символа
                    a0_TauC = a0_TauC.right(17); //Сохраняем последние 4 символа в строке
                    a0_TauCr = a0_TauC.toDouble();
                }
            }
                //IONOSPHERIC CORR
            if ((data.at(i).endsWith("IONOSPHERIC CORR"))||(data.at(i).endsWith("IONOSPHERIC CORR    ")))
            {
                TypeIonCorr = data.at(i);
                TypeIonCorr.truncate(4); //Убираем из строки символы после 9ого символа
                if(TypeIonCorr =="GPSA")
                {
                    ionAOn=true;
                ion_a0 = data.at(i);
                ion_a0.truncate(17); //Убираем из строки символы после 9ого символа
                ion_a0 = ion_a0.right(12); //Сохраняем последние 4 символа в строке
                ion_a0R = ion_a0.toDouble();
                ion_a1 = data.at(i);
                ion_a1.truncate(29); //Убираем из строки символы после 9ого символа
                ion_a1 = ion_a1.right(12); //Сохраняем последние 4 символа в строке
                ion_a1R = ion_a1.toDouble();
                ion_a2 = data.at(i);
                ion_a2.truncate(41); //Убираем из строки символы после 9ого символа
                ion_a2 = ion_a2.right(12); //Сохраняем последние 4 символа в строке
                ion_a2R = ion_a2.toDouble();
                ion_a3 = data.at(i);
                ion_a3.truncate(53); //Убираем из строки символы после 9ого символа
                ion_a3 = ion_a3.right(12); //Сохраняем последние 4 символа в строке
                ion_a3R = ion_a3.toDouble();
                }
                if(TypeIonCorr =="GPSB")
                {
                    ionBOn=true;
                ion_b0 = data.at(i);
                ion_b0.truncate(17); //Убираем из строки символы после 9ого символа
                ion_b0 = ion_b0.right(12); //Сохраняем последние 4 символа в строке
                ion_b0R = ion_b0.toDouble();
                ion_b1 = data.at(i);
                ion_b1.truncate(29); //Убираем из строки символы после 9ого символа
                ion_b1 = ion_b1.right(12); //Сохраняем последние 4 символа в строке
                ion_b1R = ion_b1.toDouble();
                ion_b2 = data.at(i);
                ion_b2.truncate(41); //Убираем из строки символы после 9ого символа
                ion_b2 = ion_b2.right(12); //Сохраняем последние 4 символа в строке
                ion_b2R = ion_b2.toDouble();
                ion_b3 = data.at(i);
                ion_b3.truncate(53); //Убираем из строки символы после 9ого символа
                ion_b3 = ion_b3.right(12); //Сохраняем последние 4 символа в строке
                ion_b3R = ion_b3.toDouble();
                }
        }

            if ((data.at(i).endsWith("LEAP SECONDS"))||(data.at(i).endsWith("LEAP SECONDS        ")))
            {
                leapSecOn=true;
                LeapSec = data.at(i);
                LeapSec.truncate(6); //Убираем из строки символы после 9ого символа
                LeapSecR = ion_b3.toDouble();
            }
        }
    }
    else
    {
        noNavigationFile = true;
        converterJps::errorList.append(201);
    }

} //КОНЕЦ МЕТОДА

//МЕТОД ДЛЯ ЧТЕНИЯ ШАПКИ НАВИГАЦИОННОГО ФАЙЛА ГЛОНАСС RINEX
void rinexReader::RRFHnavG(QFile *file){
    QList<QString> data;
    strNumNavG=0;
    // Проверяем, возможно ли открыть наш файл для чтения
    if (file->open(QIODevice::ReadOnly))
    {
        QTextStream stream(file);
        QString line;
        QString temp  ("END OF HEADER");
        QString temp2 ("END OF HEADER       ");
        while(!stream.atEnd())
        {
            line = stream.readLine();
            data.append(line);
            strNumNavG++;
            if(line.lastIndexOf(temp)>0 || line.lastIndexOf(temp2)>0)
            {
                file->close();
                break;
            }
        }
        file->close();
        noNavigationFileG = false;

        hedNumNavG = strNumNavG;
        for(qint32 i = 0; i<data.size(); i++)
        {
            if (data.at(i).endsWith("RINEX VERSION / TYPE"))
            {
                versionNavG = data.at(i);
                versionNavG.truncate(9); //Убираем из строки символы после 9ого символа
                versionNavG = versionNavG.right(4); //Сохраняем последние 4 символа в строке
            }
            if ((data.at(i).endsWith("CORR TO SYSTEM TIME"))||(data.at(i).endsWith("CORR TO SYSTEM TIME ")))
            {
                TgloOn=true;
                a0_TauC = data.at(i);
                a0_TauC.truncate(40); //Убираем из строки символы после 9ого символа
                a0_TauC = a0_TauC.right(18); //Сохраняем последние 4 символа в строке
            }
            if ((data.at(i).endsWith("TIME SYSTEM CORR"))||(data.at(i).endsWith("TIME SYSTEM CORR    ")))
            {
                TimeSysCorr = data.at(i);
                TimeSysCorr.truncate(4); //Убираем из строки символы после 9ого символа
                if(TimeSysCorr =="GPUT")
                {
                    TgpsOn=true;
                    a0 = data.at(i);
                    a0.truncate(22); //Убираем из строки символы после 9ого символа
                    a0 = a0.right(17); //Сохраняем последние 4 символа в строке
                    a1 = data.at(i);
                    a1.truncate(38); //Убираем из строки символы после 9ого символа
                    a1 = a1.right(16); //Сохраняем последние 4 символа в строке
                    W = data.at(i);
                    W.truncate(45); //Убираем из строки символы после 9ого символа
                    W = W.right(7); //Сохраняем последние 4 символа в строке
                    T = data.at(i);
                    T.truncate(45); //Убираем из строки символы после 9ого символа
                    T = T.right(7); //Сохраняем последние 4 символа в строке
                }
                if(TimeSysCorr =="GLUT")
                {
                    TgloOn=true;
                    a0_TauC = data.at(i);
                    a0_TauC.truncate(22); //Убираем из строки символы после 9ого символа
                    a0_TauC = a0_TauC.right(17); //Сохраняем последние 4 символа в строке
                }
            }
            if ((data.at(i).endsWith("LEAP SECONDS"))||(data.at(i).endsWith("LEAP SECONDS        ")))
            {
                leapSecOn=true;
                LeapSec = data.at(i);
                LeapSec.truncate(6); //Убираем из строки символы после 9ого символа
            }
        }
    }
    else
    {
        noNavigationFileG = true;
        converterJps::errorList.append(202);
    }
} //КОНЕЦ МЕТОДА

//МЕТОД ЧТЕНИЯ БЛОКОВ ЭФЕМЕРИД ИЗ ФАЙЛА RINEX 2.10
QStringList rinexReader::readInputStreamNav210(QTextStream *stream){
    QStringList data;
    QString line;
    int i = 0;
    int EphStrN = 8;

    //При первом применении начинает считывание файла с 0 строки
    //Строки заголовка отбрасываются заданными ниже условиями
    if(strNumNav==hedNumNav){
        i = 0;
    }
    //При повторном применении начинает с последней строки предыдущего блока эфемерид
    else{
        i = strNumNav;
    }
    while(!stream->atEnd())
    {
        line = stream->readLine();
        //Счётчик строк, при первом прохождении цикла получает значение 1, при последнем 8
        i++;

        if(line.endsWith("END OF HEADER")        || line.endsWith("END OF HEADER       "))
        {hedNumNav=i;
        strNumNav =i;
        }
        if((i>strNumNav)&(i<strNumNav+EphStrN)){
            data.append(line);
        }
        if(i==(strNumNav+EphStrN)){
            data.append(line);
            strNumNav = (strNumNav+EphStrN);
            break;
        }
    } //КОНЕЦ ЦИКЛА
    return data;
} //КОНЕЦ МЕТОДА

//МЕТОД ЧТЕНИЯ БЛОКОВ ЭФЕМЕРИД ГЛОНАСС ИЗ ФАЙЛА RINEX 2.10
QStringList rinexReader::readInputStreamNavG210(QTextStream *stream){
    QStringList data;
    QString line;
    int i = 0;
    int EphStrN = 4;

    //При первом применении начинает считывание файла с 0 строки
    //Строки заголовка отбрасываются заданными ниже условиями
    if(strNumNavG==hedNumNavG){
        i = 0;
    }
    //При повторном применении начинает с последней строки предъыдущего блока эфемерид
    else{
        i = strNumNavG;
    }
    while(!stream->atEnd())
    {
        line = stream->readLine();
        //Счётчик строк, при первом прохождении цикла получает значение 1, при последнем 8
        i++;
        if(line.endsWith("END OF HEADER")        || line.endsWith("END OF HEADER       "))
        {hedNumNavG=i;
        strNumNavG =i;
        }
        if((i>strNumNavG)&(i<strNumNavG+EphStrN)){
            data.append(line);
        }
        if(i==(strNumNavG+EphStrN)){
            data.append(line);
            strNumNavG = (strNumNavG+EphStrN);
            break;
        }
    } //КОНЕЦ ЦИКЛА
    return data;
} //КОНЕЦ МЕТОДА

quint32 rinexReader::ConstLitGlo(int A)
{
    for(int i=0;i<40;i++){
        lit[i]=255;
    }
    int tempNum;
    int tempLit;
    for(int i=0; i<40; i++)
    {

        tempNum = A;

        lit[0]=1;lit[1]=-4;lit[2]=5;lit[3]=6;lit[4]=1;
        lit[5]=-4;lit[6]=5;lit[7]=6;lit[8]=-2;lit[9]=-7;
        lit[10]=0;lit[11]=-1;lit[12]=4;lit[13]=-3;lit[14]=3;
        lit[15]=2;lit[16]=4;lit[17]=-3;lit[18]=3;lit[19]=2;

        if(tempNum<40){
            if(tempLit>-8&tempLit<7){
                lit[tempNum] = tempLit;

            }
        }
    }
    return tempLit;
}
