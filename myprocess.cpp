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
    if(m_fileWatcher->process())
    {
        m_dataMiner->process(m_fileWatcher->modTimeNs());
        qInfo() << "Data is taken";
    }
}
