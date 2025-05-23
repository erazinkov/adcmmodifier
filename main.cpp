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

long long int strToNs(const std::string &str, const std::string &strNs)
{
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

        QTimer timer;
        auto ms{1'000};
        timer.setInterval(ms);
//        const QString path = "/misc/agpf_nap/adcm.dat";
        const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
        FileWatcher fileWatcher(path.toStdString());

        DataMiner dm("adcm.dat.acc");

        QObject::connect(&timer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
        QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dm, &DataMiner::newData);
        timer.start();


//    QTimer::singleShot(0, [] ()
//    {
//        process();
//        QCoreApplication::exit(0);
//    });

//    QTimer::singleShot(0, [] ()
//    {
//        const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
//        DataMiner dm("adcm.dat.acc");
//        dm.newData(path.toStdString(), 1714510802425498910);
//        QCoreApplication::exit(0);
//    });

    return a.exec();
}

void process()
{
    const ChannelMap pre = ChannelMap::mapNAP();
    const std::array<std::string, 4> fileNames{
            "/home/egor/shares/tmp/tochka_1",
            "/home/egor/shares/tmp/tochka_2",
            "/home/egor/shares/tmp/tochka_3",
            "/home/egor/shares/tmp/tochka_4",
        };
    const std::array<std::string, fileNames.size()> strs{
        "2024-05-01 00:00:00",
        "2024-05-01 00:37:00",
        "2024-05-01 01:14:00",
        "2024-05-01 01:51:00",
        };
    for (size_t i{0}; i < fileNames.size(); ++i) {
        auto fileName{fileNames[i]};
        Decoder decoder(fileName, pre);
        auto pCMAP{decoder.positionsOfCMAPHeaders()};
        auto size = std::filesystem::file_size(fileName);
        pCMAP.push_back(static_cast<long>(size));

        const std::string str{strs[i]};
        const std::string strNs{"123456000"};

        long long int modTimeNs{strToNs(str, strNs)};

        DataMiner dm("");
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
            modTimeNs += 12'000'000'000;
        }
        ifs.close();
        ofs.close();
    }
}
