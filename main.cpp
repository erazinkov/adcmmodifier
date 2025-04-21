#include <QCoreApplication>

#include <QTimer>
#include <QDebug>
#include <QProcess>

#include <fstream>
#include <iostream>
#include <chrono>

#include "filewatcher.h"

#include "newdata.h"
#include "channelmap.h"
#include "decoder.h"
#include "dataminer.h"



void process();
void process1();
void takeData1(const QFileInfo *fileInfo);
void takeData2(const QFileInfo *fileInfo);



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer timer;
    auto ms{1'000};
    timer.setInterval(ms);
    const QString path = "/misc/agpf_nap/adcm.dat";
//    const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
    FileWatcher fileWatcher(path);

    DataMiner dm;

//    auto start = std::chrono::steady_clock::now();

//    auto stop = std::chrono::steady_clock::now();
//    std::cout << "Time elapsed, ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    QObject::connect(&timer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
//    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dm, &DataMiner::newData);
    timer.start();
//    QTimer::singleShot(0, [] ()
//    {
////        process1();
//        process();
//        QCoreApplication::exit(0);
//    });

    return a.exec();
}

void process()
{
    const ChannelMap pre = ChannelMap::mapNAP();
    Decoder decoder("/home/egor/build-adcmmodifier-Desktop-Debug/adcm.dat.mod_1", pre);
    auto p{decoder.positionsOfNDHeaders()};
    qInfo() << p.size();
}

void takeData1(const QFileInfo *fileInfo) {

    std::ifstream ifs;
    ifs.open(fileInfo->absoluteFilePath().toStdString(), std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        std::cout << "Can't open input file" << std::endl;
        return;
    }

    std::ofstream ofs;
    ofs.open("adcm.dat.mod", std::ios::out | std::ios::binary | std::ios::app);
    if (!ofs.is_open())
    {
        std::cout << "Can't open output file" << std::endl;
        return;
    }
    NewData newBlock;
    auto nd{newBlock.data()};
    auto d{fileInfo->lastModified().currentMSecsSinceEpoch()};
    nd.time = d;
    newBlock.setData(nd);
    ofs << newBlock;
    char buffer[1024];
    while (ifs.read(buffer, sizeof (buffer)))
    {
        ofs.write(buffer, ifs.gcount());
    }
    ofs.write(buffer, ifs.gcount());
    ifs.close();
    ofs.close();
}

void takeData2(const QFileInfo *fileInfo) {

//    QString program = "ping";
//    QStringList arguments;
//    arguments << "10.90.90.4" << "-c" << "4";

//    QProcess *myProcess = new QProcess;

//    QObject::connect(myProcess, &QProcess::readyReadStandardOutput, [myProcess](){
//        auto ba = myProcess->readAllStandardOutput();
//        qInfo() << QString(ba);
//    });
//    myProcess->start(program, arguments);
//    myProcess->waitForFinished();
//    myProcess->close();
}

void process1() {
//    QObject *parent;

    QString program = "ping";
    QStringList arguments;
    arguments << "10.90.90.4" << "-c" << "4";

    QProcess *myProcess = new QProcess;

    QObject::connect(myProcess, &QProcess::readyReadStandardOutput, [myProcess](){
        auto ba = myProcess->readAllStandardOutput();
        qInfo() << QString(ba);
    });
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    myProcess->close();

//    qInfo() << myProcess;
}

