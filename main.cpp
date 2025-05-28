#include <QCoreApplication>

#include <QTimer>
#include <QProcess>
#include <QElapsedTimer>
#include <QCommandLineParser>

#include <fstream>
#include <iostream>
#include <chrono>

#include "filewatcher.h"

#include "newdata.h"
#include "channelmap.h"
#include "decoder.h"
#include "dataminer.h"

#include "mapperparser.h"
#include "mapperquery.h"

void process();

void process(const MapperQuery &query);

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


    QCommandLineParser parser;
    MapperQuery query;
    MapperParser mapperParser(parser, query);
    auto parseResult = mapperParser.parseResult();

    if (parseResult)
    {
        a.quit();
    }


    FileWatcher fileWatcher(query.input.toStdString());
    DataMiner dataMiner(query.output.toStdString());

    QTimer watcherTimer;
    const auto watcherIntervalMs{1'000};
    watcherTimer.setInterval(watcherIntervalMs);
    QElapsedTimer elapsedTimer;
    const auto elapsedIntervalMs{query.time * 60 * 1'000};

    QTextStream ss;

    QObject::connect(&watcherTimer, &QTimer::timeout, [&](){
        if (!elapsedTimer.hasExpired(elapsedIntervalMs))
        {
            qInfo() << "Elapsed time" << 0.001 * static_cast<double>(elapsedTimer.elapsed()) << "s";
            return;
        }
        watcherTimer.stop();
        a.quit();
    });
    QObject::connect(&watcherTimer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &watcherTimer, &QTimer::stop);
    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dataMiner, &DataMiner::newData);
    QObject::connect(&dataMiner, &DataMiner::finished, &watcherTimer, [&](){
        QFileInfo inputInfo{query.input};
        qInfo() << "Input size: " << inputInfo.size() << "bytes";
        watcherTimer.start();
    });

    watcherTimer.start();
    elapsedTimer.start();

    return a.exec();
}

void process(const MapperQuery &query)
{

//    const QString path = "/misc/agpf_nap/adcm.dat";
//    const QString path = "/home/egor/build-adcmemulate-Desktop-Debug/adcm.dat";
    FileWatcher fileWatcher(query.input.toStdString());
    DataMiner dataMiner(query.output.toStdString());

    QTimer watcherTimer;
    const auto watcherIntervalMs{1'000};
    watcherTimer.setInterval(watcherIntervalMs);
    QElapsedTimer elapsedTimer;
    const auto elapsedIntervalMs{query.time * 1'000};

    QObject::connect(&watcherTimer, &QTimer::timeout, [&](){
        if (!elapsedTimer.hasExpired(elapsedIntervalMs)) {
            qInfo() << "Elapsed time" << elapsedTimer.elapsed() << "ms";
            return;
        }
        watcherTimer.stop();
        QCoreApplication::quit();
    });
    QObject::connect(&watcherTimer, &QTimer::timeout, &fileWatcher, &FileWatcher::operate);
    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &watcherTimer, &QTimer::stop);
    QObject::connect(&fileWatcher, &FileWatcher::onFileChanged, &dataMiner, &DataMiner::newData);
    QObject::connect(&dataMiner, &DataMiner::finished, &watcherTimer, [&](){watcherTimer.start();});

    watcherTimer.start();
    elapsedTimer.start();
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
