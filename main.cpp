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
void processTest();

void strToNs()
{
    std::string str{"2024-05-01 00:00:00"};
    std::string strNs{"123456000"};
    std::tm tm = {};
    std::stringstream ss{str};
    ss >> std::get_time(&tm, "%Y-%m-%d  %H:%M:%S");
    std::chrono::system_clock::time_point tp{std::chrono::system_clock::from_time_t(std::mktime(&tm))};
    tp += std::chrono::nanoseconds(std::atoll(strNs.c_str()));
    long long int ns{tp.time_since_epoch().count()};
    std::cout << ns << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer timer;
    auto ms{1'000};
    timer.setInterval(ms);
    const QString path = "/misc/agpf_nap/adcm.dat";
//    const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
    FileWatcher fileWatcher(path.toStdString());

    DataMiner dm;

//    auto start = std::chrono::steady_clock::now();

//    auto stop = std::chrono::steady_clock::now();
//    std::cout << "Time elapsed, ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    QObject::connect(&timer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dm, &DataMiner::newData);
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

void processTest() {

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

}

