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

long long int strToNs()
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
    return ns;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    {
//        QTimer timer;
//        auto ms{1'000};
//        timer.setInterval(ms);
//        const QString path = "/misc/agpf_nap/adcm.dat";
//        //    const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
//        FileWatcher fileWatcher(path.toStdString());

//        DataMiner dm;

//        QObject::connect(&timer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
//        QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dm, &DataMiner::newData);
//        timer.start();
//    }
    QTimer::singleShot(0, [] ()
    {
        process();
        QCoreApplication::exit(0);
    });

    return a.exec();
}

void process()
{
    const ChannelMap pre = ChannelMap::mapNAP();
    const std::string fileName{"/home/egor/shares/tmp/sep18-15.13.38"};
    Decoder decoder(fileName, pre);
    auto pCMAP{decoder.positionsOfCMAPHeaders()};
    auto size = std::filesystem::file_size(fileName);
    pCMAP.push_back(static_cast<long>(size));
    long long int modTimeNs{strToNs()};

    DataMiner dm;
    std::ifstream ifs;
    ifs.open(fileName, std::ios::in | std::ios::binary);
    std::ofstream ofs;
    ofs.open(fileName + ".mod", std::ios::out | std::ios::binary);
    if (!ifs.is_open() || !ofs.is_open())
    {
        std::cout << "Can't open input || output file" << std::endl;
        ifs.close();
        ofs.close();
        return;
    }

    qInfo() << pCMAP.size();

    for (size_t i{0}; i < pCMAP.size() - 1; ++i)
    {
        dm.newDataOffline(ifs, ofs, pCMAP.at(i + 1) - pCMAP.at(i), modTimeNs);
        modTimeNs += 10'000'000'000;
    }
    ifs.close();
    ofs.close();
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

