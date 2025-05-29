#include <QCoreApplication>

#include <QTimer>
#include <QDeadlineTimer>
#include <QCommandLineParser>

#include "mapperparser.h"
#include "mapperquery.h"

#include "myprocess.h"

#include <csignal>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    MapperQuery query;
    MapperParser mapperParser(parser, query);
    auto parseResult = mapperParser.parseResult();

    if (!parseResult)
    {
        return 0;
    }

    QTimer timer;
    QDeadlineTimer deadLineTimer(query.time * 60 * 1'000);
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        qInfo() << "Time left:" << 0.001 * static_cast<double>(deadLineTimer.remainingTime());
        if (deadLineTimer.remainingTime() == 0)
        {
            a.quit();
        }
    });

    MyProcess myProcess(query.input, query.output);

    QObject::connect(&a, &QCoreApplication::aboutToQuit, &myProcess, &MyProcess::stop);

    signal(SIGTERM,[](int) {QCoreApplication::quit();});
    signal(SIGABRT,[](int) {QCoreApplication::quit();});
    signal(SIGINT, [](int) {QCoreApplication::quit();});
    signal(SIGKILL, [](int) {QCoreApplication::quit();});

    timer.start(1'000);
    return a.exec();
}
