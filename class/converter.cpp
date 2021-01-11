#include "converter.h"

ConverterBase::ConverterBase(QString _filePath)
{
    filePath = _filePath;
    if(_filePath.contains("/")) {
        fileName = _filePath.split("/").at(_filePath.split("/").size() - 1);
    }else{
        fileName = _filePath;
    }

    file.setFileName(filePath);

}

ConverterBase::~ConverterBase()
{
    delete stream;
}

void ConverterBase::setFilePath(QString _fName)
{
    filePathSet = _fName;
}

QString ConverterBase::getFilePath()
{
    return filePathSet;
}

void ConverterBase::setConverterBaseRealization(ConverterBase *_converterBase)
{
    converterBase = _converterBase;
}

void ConverterBase::writeVersion(int v)
{
    if(v == 2){
        dataForWrite[0][5] = '2';
        dataForWrite[0][6] = '.';
        dataForWrite[0][7] = '1';
        dataForWrite[0][8] = '1';
    }else if(v == 3){
        dataForWrite[0][5] = '3';
        dataForWrite[0][6] = '.';
        dataForWrite[0][7] = '0';
        dataForWrite[0][8] = '2';
    }
}


bool ConverterBase::findAllStringAndWrite(QString _str)
{
    QString str = "";
    bool res = false;
    for(int i = 0; i < dataHeader.size(); i++){
       str = dataHeader[i];
       if(str.contains(_str)){
           dataForWrite.push_back(str);
           numLineFile++;
           res = true;
       }
    }
    return res;
}

void ConverterBase::readLine(QString &line)
{
    line = stream->readLine();
}


QString ConverterBase::getSymbolsString(QString symbol, int count)
{
    QString str = "";
    for(int i = 0; i < count; i++){
        str += symbol;
    }
    return str;
}

int ConverterBase::findStringAndWrite(QString _str)
{
    QString str = "";
    QString strd;
    QString str1,str2,str3;

    if (_str == "PGM / RUN BY / DATE")
    {
     strd=QDateTime::currentDateTime().toString("yyyyMMdd hhmmss")+" UTC ";
     str1=QString("%1").arg("Converter 1.0",-20,' ');
     str2=QString("%2").arg("RKS",-20,' ');
     str3=QString("%3").arg(strd,20,' ');
     str=str1+str2+str3+_str;
     dataForWrite.push_back(str);
     numLineFile++;
     return 1;
    }

    for(int i = 0; i < dataHeader.size(); i++){
        str = dataHeader[i];
        if(str.contains(_str)){
            dataForWrite.push_back(str);
            numLineFile++;
            return i;
        }
    }
    return -1;
}


void ConverterBase::makeHeaderData()
{
    QString str = stream->readLine();
    while(!str.contains("END OF HEADER")){
        dataHeader.push_back(str);
        str = stream->readLine();
    }
}

void ConverterBase::convert()
{
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Error! File Not Open";
    }
    stream = new QTextStream(&file);

    //считывание заголовка
    makeHeaderData();

    //конвертация и запись заголовка
    converterBase->readHeader();

    //конвертация и запись эпох
    converterBase->writeEpoch();

/// Директория выдачи конвертированного файла
///*==============================================================
/// 1 - та же директория , 2 - другая, установленная директория
///*==============================================================

/*1*/   //    QFile newFile(converterBase->newFileName());
/*2*/   //    QFile newFile(converterBase->getFilePath() + fileName);

    QFile newFile(filePath);

    if(!newFile.open(QIODevice::WriteOnly)){
        qDebug()<<"Error new File not Open";
    }

    //финальный (отконвертированный) файл
    QTextStream streamF(&newFile);
    for(int i = 0; i < dataForWrite.size(); i++){
        streamF << dataForWrite[i] << endl;
    }
}

ConvertN3To2::ConvertN3To2(QString _filePath) :
    ConverterBase(_filePath)
{
    //
}

void ConvertN3To2::readHeader()
{
    int i = findStringAndWrite("RINEX VERSION / TYPE");
    i = findStringAndWrite("PGM / RUN BY / DATE");
    i = findAllStringAndWrite("COMMENT");
    i = findAllStringAndWrite("IONOSPHERIC CORR");
    i = findAllStringAndWrite("TIME SYSTEM CORR");
    i = findStringAndWrite("LEAP SECONDS");
}

void ConvertN3To2::writeEpoch()
{
    QString line;
    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");
    numLineFile++;
    writeVersion(2);
    while(!stream->atEnd()){
        readLine(line);
        if(line[0] == ' '){
            dataForWrite.push_back(line.right(line.size() - 3));
        }else{
            dataForWrite.push_back(line.mid(1,2) + " " + line.right(line.size() - 6));
        }
    }
}

QString ConvertN3To2::newFileName()
{
    QString res = filePath;
    res[res.size() - 5];
    return res;
}

RenexConverter::RenexConverter() : converterBase{nullptr}
{
    //filePath = QDir::currentPath();
}

void RenexConverter::convert_rinex_O_to_302(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertO2To3(_fileName);
    converterBase->setFilePath(filePath);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::convert_rinex_O_to_211(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertO3To2(_fileName);
    converterBase->setFilePath(filePath);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::convert_rinex_N_to_302(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertN2To3(_fileName);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::convert_rinex_N_to_211(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertN3To2(_fileName);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::convert_rinex_G_to_302(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertG2To3(_fileName);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::convert_rinex_G_to_211(QString _fileName)
{
    if(converterBase != nullptr){
        delete converterBase;
    }
    converterBase = new ConvertG3To2(_fileName);
    converterBase->setConverterBaseRealization(converterBase);
    converterBase->convert();
}

void RenexConverter::setFilePath(QString _filePath)
{
    filePath = _filePath;
}

ConvertN2To3::ConvertN2To3(QString _filePath) :
    ConverterBase(_filePath)
{
    //
}

void ConvertN2To3::readHeader()
{
    int i = findStringAndWrite("RINEX VERSION / TYPE");
    i = findStringAndWrite("PGM / RUN BY / DATE");
    i = findAllStringAndWrite("COMMENT");
    i = findAllStringAndWrite("IONOSPHERIC CORR");
    i = findAllStringAndWrite("TIME SYSTEM CORR");
    i = findStringAndWrite("LEAP SECONDS");
}

void ConvertN2To3::writeEpoch()
{
    QString line;
    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");
    numLineFile++;
    writeVersion(3);
    while(!stream->atEnd()){
        readLine(line);
        if(line[1] == ' '){
            dataForWrite.push_back("   " + line);
        }else{
            QString str;
            if(line[0] == ' '){ str = "G0" + line[1]; }else{ str = "G" + line[0] + line[1]; }

            qDebug()<<line;
            dataForWrite.push_back(str + " 20" + line.right(line.size() - 3));
        }
    }
}

QString ConvertN2To3::newFileName()
{
    QString res = filePath;
    res[res.size() - 5];
    return res;
}

ConvertG2To3::ConvertG2To3(QString _filePath) :
    ConverterBase(_filePath)
{
    //
}

void ConvertG2To3::readHeader()
{
    int i = findStringAndWrite("RINEX VERSION / TYPE");
    i = findStringAndWrite("PGM / RUN BY / DATE");
    i = findAllStringAndWrite("COMMENT");
    i = findAllStringAndWrite("IONOSPHERIC CORR");
    i = findAllStringAndWrite("TIME SYSTEM CORR");
    i = findStringAndWrite("LEAP SECONDS");
}

void ConvertG2To3::writeEpoch()
{
    QString line;
    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");
    numLineFile++;
    writeVersion(3);
    while(!stream->atEnd()){
        readLine(line);
        if(line[1] == ' '){
            dataForWrite.push_back("   " + line);
        }else{
            QString str;
            if(line[0] == ' '){ str = "R0" + line[1]; }else{ str = "R" + line[0] + line[1]; }

            qDebug()<<line;
            dataForWrite.push_back(str + " 20" + line.right(line.size() - 3));
        }
    }
}

QString ConvertG2To3::newFileName()
{
    QString res = filePath;
    res[res.size() - 5];
    return res;
}

ConvertG3To2::ConvertG3To2(QString _filePath) :
    ConverterBase(_filePath)
{
    //
}

void ConvertG3To2::readHeader()
{
    int i = findStringAndWrite("RINEX VERSION / TYPE");
    i = findStringAndWrite("PGM / RUN BY / DATE");
    i = findAllStringAndWrite("COMMENT");
    i = findAllStringAndWrite("IONOSPHERIC CORR");
    i = findAllStringAndWrite("TIME SYSTEM CORR");
    i = findStringAndWrite("LEAP SECONDS");
}

void ConvertG3To2::writeEpoch()
{
    QString line;
    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");
    numLineFile++;
    writeVersion(2);
    while(!stream->atEnd()){
        readLine(line);
        if(line[0] == ' '){
            dataForWrite.push_back(line.right(line.size() - 3));
        }else{
            dataForWrite.push_back(line.mid(1,2) + " " + line.right(line.size() - 6));
        }
    }
}

QString ConvertG3To2::newFileName()
{
    QString res = filePath;
    res[res.size() - 5];
    return res;
}

ConvertO2To3::ConvertO2To3(QString _filePath) :
    ConverterBase(_filePath)
{
    //
}

void ConvertO2To3::readHeader()
{
    QString str = "";

    epoch3.prnOfObs.clear();

    int i = 0;

    i = findStringAndWrite("RINEX VERSION / TYPE");

    i = findStringAndWrite("PGM / RUN BY / DATE");

    i = findAllStringAndWrite("COMMENT");

    findAllStringAndWrite("MARKER");

    i = findStringAndWrite("OBSERVER / AGENCY");

    i = findStringAndWrite("REC # / TYPE / VERS");

    i = findStringAndWrite("ANT # / TYPE");

    i = findStringAndWrite("APPROX POSITION XYZ");

    i = findStringAndWrite("ANTENNA: DELTA H/E/N");

    i = findAllStringAndWrite("WAVELENGTH FACT");

    getObsTypes();

    i = findStringAndWrite("INTERVAL");

    i = findStringAndWrite("TIME OF FIRST OBS");

    i = findStringAndWrite("TIME OF LAST OBS");

    i = findStringAndWrite("LEAP SECONDS");

    i = findStringAndWrite("# OF SATELLITES");

    prnObsTrue = findAllStringAndWrite("PRN / # OF OBS");

    prnObsTrue = false; /// тест

    findAllStringAndWrite("GLONASS SLOT / FRQ #");

    //outputStringList.push_back(getSymbolsString(" ", 60) + "END OF HEADER");

    writeVersion(3);
}

void ConvertO2To3::writeEpoch()
{
    STATE_READER = READ_EPOCH_LINE;

    int it = itNumLinesInFile;
    QString line = "";
    QString res = "";

    int linesInEpoch = 0;
    streamAtEnd = false;
    while(!streamAtEnd){

        switch (STATE_READER) {

        case READ_EPOCH_LINE:

            res = "";
            linesInEpoch = 0;
            readLine(line);
            epoch3.epochLine = line.left(32);
            epoch3.linesInEpoch = line.mid(30,2).toInt();

            res += line.mid(32, - 1);
            res = res.left(36);

            if(epoch3.linesInEpoch % 12 > 0){
                linesInEpoch++;
            }
            linesInEpoch += (epoch3.linesInEpoch / 12);

            for(int i = 1; i < linesInEpoch; i++){
                readLine(line);
                line = line.mid(32, -1);
                line = line.left(36);
                res += line;
            }

            for(int i = 0; i < res.size(); i++){
                if(res[i] == " "){
                    res[i] = '0';
                }
            }
            epoch3.sattellites = res;

            STATE_READER = READ_EPOCH_VALUES;

        case READ_EPOCH_VALUES:
            epoch3.epochValues.clear();

            for(int i = 0; i < epoch3.linesInEpoch; i++){
                res = "";
                readLine(res);
                int prevIt = 0;
                for(int j = 0; j < epoch3.epochLines - 1; j++){
                    readLine(line);

                    int itPointInCurrentString = res.lastIndexOf('.');
                    int itPointInNextString = line.indexOf('.');
                    int spaceCount = res.size() - itPointInCurrentString - 1 + itPointInNextString + 1;

                    int stringSize = res.size() - prevIt;
                    prevIt += 80;

                    res += getSymbolsString(" ", 80 - stringSize) + line; //+ "  ";
                }
                epoch3.epochValues[epoch3.sattellites.mid(0 + i*3, 3)] = res;
                getPrnObs(res, epoch3.sattellites.mid(i*3, 3));
            }
            if(!streamAtEnd){
                STATE_READER = WRITE_EPOCH;
            }else{
                STATE_READER = EXIT_FILE;
                break;
            }

        case WRITE_EPOCH:
            res = "> 20";
            res += epoch3.epochLine.mid(1, 31);
            dataEpochs.push_back(res);
            for(auto it = epoch3.epochValues.begin(); it != epoch3.epochValues.end(); it++){
                res = it.key() + it.value();
                dataEpochs.push_back(res);
            }
            if(!streamAtEnd){
                STATE_READER = READ_EPOCH_LINE;
            }else{
                STATE_READER = EXIT_FILE;
            }
        case EXIT_FILE:
            break;
        }
    }
    QString str = "   ";
    QString str2 = "   ";
    //Запись заголовка PRN / # OF OBS
    if(!prnObsTrue){
        for(auto it = epoch3.prnOfObs.begin(); it != epoch3.prnOfObs.end(); it++){
            str += it.key();
            QString space = "";
            for(int i = 0; i < it.value().size(); i++){
                if(it.value()[i] / 10 == 0) { space = "     "; }
                else if(it.value()[i] / 100 == 0) { space = "    "; }
                else if(it.value()[i] / 1000 == 0) { space = "   "; }
                else if(it.value()[i] / 10000 == 0) { space = "  "; }
                else { space = " "; }
                str += space + QString::number(it.value()[i]);
            }

            if(str.size() > 60){
                dataForWrite.push_back(str.left(60) + "PRN / # OF OBS");
                dataForWrite.push_back(str.left(6) + str.right(str.size() - 60) + getSymbolsString(" ", 114 - str.size()) + "PRN / # OF OBS");
            }else{
                str += getSymbolsString(" ", 60 - str.size()) + "PRN / # OF OBS";
                //str += '\n';
                dataForWrite.push_back(str);
            }
            str = "   ";
        }
    }

    //Запись измерительной информации
    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");
    dataForWrite = dataForWrite + dataEpochs;
}

QString ConvertO2To3::newFileName()
{
    QString res = filePath;
    res[res.size() - 5];
    return res;
}

void ConvertO2To3::readLine(QString &line)
{
    if(!stream->atEnd()){
        line = stream->readLine();
    }else{
        line = "";
        streamAtEnd = true;
    }
}


void ConvertO2To3::getPrnObs(QString _str, QString numSat)
{
    QString str = _str;
    bool mass[countTypesOfObserver];
    QVector<int> vec(countTypesOfObserver);

    for(int i = 0; i < countTypesOfObserver; i++){
        mass[i] = false;
        vec[i] = 0;
    }

    if(!epoch3.prnOfObs.contains(numSat)){
        epoch3.prnOfObs[numSat] = vec;
    }

    if(str.mid(9,3).toFloat() > 0.0){
        mass[0] = true;
    }


    for(int i = 9, j = 0; i < str.size() || j < countTypesOfObserver; i+=16, j++){
        if(str.mid(i - 1, 5) != " 0.00"  &&  str.mid(i - 1, 5) != "     "  &&  i < str.size()){

            mass[j] = true;
            epoch3.prnOfObs[numSat][j]++;
        }
    }
}

void ConvertO2To3::getObsTypes()
{
    countTypesOfObserver = 0;
    QString str = "";

    //TYPES OF OBSERV
    for(int i = 0; i < dataHeader.size(); i++) {
        if(dataHeader[i].contains("TYPES OF OBSERV")){
            str += dataHeader[i].left(dataHeader[i].size() - 19);
        }
    }
    str = str.simplified();

    epoch3.sattellites = str.mid(2,-1);
    epoch3.epochLines = str.mid(0, 2).toInt();

    int count = 1;
    while(epoch3.epochLines - 5 > 0){
        epoch3.epochLines -= 5;
        count++;
    }
    epoch3.epochLines = count;

    itNumLinesInFile = dataHeader.size() - 1;
    QStringList lst = epoch3.sattellites.split(" ");
    QStringList lstV2 = templateSat2.split("  ");
    QStringList lstV3 = templateSat3.split(" ");
    typesOfObserver = "";
    for(int i = 0; i < lst.size(); i++)
    {
     for(int j = 0; j < lstV2.size(); j++)
     {
      if(lstV2[j] == lst[i])
      {
       typesOfObserver += lstV3[j] + " ";
       countTypesOfObserver++;
       break;
       }
      }
    }
    QString strBegin = "";
    str = "";
    strBegin = "G    " + QString::number(countTypesOfObserver) + " ";
    if(countTypesOfObserver > 13){
        str = strBegin + typesOfObserver.left(typesOfObserver.size() - 4);
        str += "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
        str = strBegin + typesOfObserver.right(4);
        str += getSymbolsString(" ", 60 - str.size()) + "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
    }else{
        str = strBegin + typesOfObserver;
        str += getSymbolsString(" ", 60 - str.size()) + "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
    }

    strBegin = "R    " + QString::number(countTypesOfObserver) + " ";
    if(countTypesOfObserver > 13){
        str = strBegin + typesOfObserver.left(typesOfObserver.size() - 4);
        str += "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
        str = strBegin + typesOfObserver.right(4);
        str += getSymbolsString(" ", 60 - str.size()) + "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
    }else{
        str = strBegin + typesOfObserver;
        str += getSymbolsString(" ", 60 - str.size()) + "SYS / # / OBS TYPES";
        dataForWrite.push_back(str);
    }
}

ConvertO3To2::ConvertO3To2(QString _filePath):
    ConverterBase(_filePath)
{
    //
}

void ConvertO3To2::readHeader()
{
    int i = 0;

    i = findStringAndWrite("RINEX VERSION / TYPE");

    i = findStringAndWrite("PGM / RUN BY / DATE");

    findAllStringAndWrite("COMMENT");

    findAllStringAndWrite("MARKER");

    i = findStringAndWrite("OBSERVER / AGENCY");

    i = findStringAndWrite("REC # / TYPE / VERS");

    i = findStringAndWrite("ANT # / TYPE");

    i = findStringAndWrite("APPROX POSITION XYZ");

    i = findStringAndWrite("ANTENNA: DELTA H/E/N");

    i = findAllStringAndWrite("WAVELENGTH FACT");

    getObsTypes();

    i = findStringAndWrite("INTERVAL");

    i = findStringAndWrite("TIME OF FIRST OBS");

    i = findStringAndWrite("TIME OF LAST OBS");

    i = findStringAndWrite("LEAP SECONDS");

    i = findStringAndWrite("# OF SATELLITES");

    prnObsTrue = findAllStringAndWrite("PRN / # OF OBS");

    findAllStringAndWrite("GLONASS SLOT / FRQ #");


    dataForWrite.push_back(getSymbolsString(" ", 60) + "END OF HEADER");

    writeVersion(2);
}

void ConvertO3To2::writeEpoch()
{

    QString line = "";
    readLine(line);

    epoch2.epochLine = line.right(line.size() - 2);
    epoch2.countLines = 0;
    epoch2.sattellites.clear();

    while(!stream->atEnd()){
        readLine(line);
        if(line[0] == '>'){
            writeEpochOne();
            epoch2.epochLine = line.right(line.size() - 2);
            epoch2.countLines = 0;
            epoch2.sattellites.clear();
        }else{
            QString key = line.left(3);
            QString values = line.right(line.size() - 3);
            epoch2.sattellites[key] = values;
            epoch2.countLines++;
        }
    }
    writeEpochOne();

}

QString ConvertO3To2::newFileName()
{
    QString res = filePath;
    //res[res.size() - 5];
    return res;
}

void ConvertO3To2::writeEpochOne()
{
    QString satLine = "";
    QStringList sattellites;
    QString key;
    QVector<QString> satLines;
    int count = 0;

    ///запись верхней(главной) строки эпохи
    for(auto it = epoch2.sattellites.begin(); it != epoch2.sattellites.end(); it++){
        satLine = satLine + it.key();
        count++;
        if(count % 12 == 0){
            satLines.push_back(satLine);
            satLine = "                                ";
        }
    }
    if(satLine != "                                "){
        satLines.push_back(satLine);
    }

    satLine = "";

    QString strHelp = epoch2.epochLine.left(33);
    strHelp = " " + strHelp.right(strHelp.size() - 2);

    QString line = strHelp /* + QString::number(epoch2.countLines)*/ + satLines[0];
    dataForWrite.push_back(line);

    for(int i = 1; i < satLines.size(); i++){
        dataForWrite.push_back(satLines[i]);
    }
    satLines.clear();
    QStringList lstSattelitesGPS = epoch2.satGPS.split(" ");
    QStringList lstSattelitesGLO = epoch2.satGLO.split(" ");
    ///Печать измерений в эпохе
    for(auto it = epoch2.sattellites.begin(); it != epoch2.sattellites.end(); it++){

        QString s = it.value();
        QString sIz;
        sattellites.clear();
        for(int i = 10; i < it.value().size(); i+=16){
            sIz = s.mid(i - 10, 16);
            sattellites.push_back(sIz);
        }

        key = it.key();
        QString setSpacing  = "";
        QStringList satAllList = epoch2.satAll.split(" ");

        if(sattellites.last().size() < 16){
            QString str = getSymbolsString(" ", 16 - sattellites.last().size());
            sattellites[sattellites.size() - 1] = sattellites.last() + str;
        }

        /// Если измерения не в полном обьеме

        if(key.contains("G") && sattellites.size() < lstSattelitesGPS.size()){
            for(int i = sattellites.size(); i < lstSattelitesGPS.size(); i++){
                sattellites.push_back("         0.000  ");
            }
        }

        if(key.contains("R") && sattellites.size() < lstSattelitesGLO.size()){
            for(int i = sattellites.size(); i < lstSattelitesGLO.size(); i++){
                sattellites.push_back("         0.000  ");
            }
        }

            for(int i = 0, j = 1; i < satAllList.size(); i++, j++){

                ///Если тип измерения поддерживается файлом rinex 2.X
                if((key.contains("G") && epoch2.satGPSVecObsBool.contains(i)) || (key.contains("R") && epoch2.satGLOVecObsBool.contains(i))){

                    int numIzm = 0;

                    if(key.contains("G")){
                        numIzm = lstSattelitesGPS.indexOf(satAllList[i]);
                    }else if(key.contains("R")){
                        numIzm = lstSattelitesGLO.indexOf(satAllList[i]);
                    }

                    if(numIzm != -1){
                        satLine += sattellites[numIzm];
                    }else{
                        qDebug()<<"error: несоответствие размеров массивов sattelites и lstSattelites";
                    }

                }else{
                    if( i % 5 == 0){
                        setSpacing = "         ";
                    }else{
                        setSpacing = "         ";
                    }
                    satLine += setSpacing + "0.000  ";
                }

                setSpacing = "";

            if(j % 5 == 0){
                dataForWrite.push_back(satLine);
                satLine = "";
            }
        }
        if(satLine != ""){
            dataForWrite.push_back(satLine);
            satLine = "";
        }
    }
}

void ConvertO3To2::getObsTypes()
{
    QString satGPS = "";
    QString satGLO = "";
    epoch2.satGPS = "";
    epoch2.satGLO = "";
    epoch2.satGPSVecObsBool.clear();
    epoch2.satGLOVecObsBool.clear();
    epoch2.satGPSVecWriteIzm.clear();
    epoch2.satGLOVecWriteIzm.clear();
    epoch2.satAll = "";
    int i = 0;
    QString str = dataHeader[i];
    while(!str.contains("OBS TYPES")){
        i++;
        str = dataHeader[i];
    }

    while (str.contains("OBS TYPES")){
        if(str[0] == "G"){
            satGPS += str.mid(7, 52);
        }else if(str[0] == "R"){
            satGLO += str.mid(7, 52);
        }
    i++;
    str = dataHeader[i];
    }

    epoch2.satGPS = satGPS;
    epoch2.satGLO = satGLO;
    epoch2.satAll = satGLO;

    QStringList lst;
/*
    QStringList lst = templateSat3.split(" ");
    for(int i = 0; i < lst.size(); i++){
        if(epoch2.satGPS.contains(lst[i]) || epoch2.satGLO.contains(lst[i])){
            epoch2.satAll += lst[i] + " ";
        }
    }
*/
    epoch2.satGPS = epoch2.satGPS.simplified();
    epoch2.satGLO = epoch2.satGLO.simplified();
    epoch2.satAll = epoch2.satAll.simplified();

    qDebug()<<"Sattellites: "<<epoch2.satAll<<endl;
    ///Заполнение вектора наличия типов измерений в общем списке измерений "satAll" для GPS
    lst = epoch2.satAll.split(" ");
    for(int i = 0; i < lst.size(); i++){
        if(epoch2.satGPS.contains(lst[i])){
            epoch2.satGPSVecObsBool.push_back(i);
        }
    }
    ///Заполнение вектора для записи из строки измерений версии 3, в строку измерений версии 2 для GPS
    lst = epoch2.satGPS.split(" ");
    for(int i = 0; i < lst.size(); i++){
        if(epoch2.satAll.contains(lst[i])){
            epoch2.satGPSVecWriteIzm.push_back(i);
        }
    }

    ///Заполнение вектора наличия типов измерений в общем списке измерений "satAll" для GLO
    lst = epoch2.satAll.split(" ");
    for(int i = 0; i < lst.size(); i++){
        if(epoch2.satGLO.contains(lst[i])){
            epoch2.satGLOVecObsBool.push_back(i);
        }
    }

    ///Заполнение вектора для записи из строки измерений версии 3, в строку измерений версии 2 для GPS
    lst = epoch2.satGLO.split(" ");
    for(int i = 0; i < lst.size(); i++){
        if(epoch2.satAll.contains(lst[i])){
            epoch2.satGLOVecWriteIzm.push_back(i);
        }
    }

    writeObsTypes();
}

void ConvertO3To2::writeObsTypes()
{
    QString result = "    ";
    QStringList sattellitesVersion3 = templateSat3.split(" ");
    QStringList sattellitesVersion2 = templateSat2.split("  ");
    QStringList sattellitesAll = epoch2.satAll.split(" ");
    QStringList resultSats;
    QString str,str1;
    qDebug() << epoch2.satAll;
    qDebug() << sattellitesAll;
    qDebug() << sattellitesVersion3;
    qDebug() << sattellitesVersion2;

    for(int i = 0; i < sattellitesVersion3.size(); i++){
        if(epoch2.satAll.contains(sattellitesVersion3[i])){
            resultSats.push_back(sattellitesVersion2[i]);
        }
    }
    qDebug() << resultSats;

    resultSats.clear();
    for(int i = 0; i < sattellitesAll.count(); i++)
    {
     str=sattellitesAll[i];
     for (int j=0; j<sattellitesVersion3.count(); j++)
     {
      str1=sattellitesVersion3[j];
      if (str == str1)
      {
       resultSats << sattellitesVersion2[j];
       qDebug() << str << str1;
       break;
      }
     }
    }
    qDebug() << resultSats;

    result += QString::number(resultSats.size()) + "    ";

    for(int i = 0; i < resultSats.size(); i++){
        result += resultSats[i] + "    ";

        if(i == 8){
            result = result.left(result.size() - 4) + "# / TYPES OF OBSERV";
            dataForWrite.push_back(result);
            result = "          ";
        }
    }

    result +=  getSymbolsString(" ", 60 - result.size()) + "# / TYPES OF OBSERV";
    dataForWrite.push_back(result);
}
