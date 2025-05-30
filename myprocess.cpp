#include "myprocess.h"

#include <QDebug>

MyProcess::MyProcess(const QString &inputPath, const QString &outputPath, QObject *parent)
    : QObject(parent)
{
    m_fileWatcher = new FileWatcher(inputPath.toStdString());
    m_dataMiner = new DataMiner(inputPath.toStdString(), outputPath.toStdString());
    connect(&m_timer, &QTimer::timeout, this, &MyProcess::onTimeout);
    m_timer.start(1'000);
}

MyProcess::~MyProcess()
{
    delete m_dataMiner;
    m_dataMiner = nullptr;
    delete m_fileWatcher;
    m_fileWatcher = nullptr;
}

void MyProcess::onTimeout()
{
    using Status = FileWatcher::ProcessResult::Status;
    auto processResult{m_fileWatcher->process()};
    switch (processResult.statusCode)
    {
        case Status::Ok:
            m_dataMiner->process(m_fileWatcher->modTimeNs());
            qInfo() << "Data is taken";
            break;
        case Status::Wait:
            break;
        case Status::Error:
            qInfo() << "Error input file: " << processResult.errorString.value_or("Unknown").c_str();
            break;
    }
}
