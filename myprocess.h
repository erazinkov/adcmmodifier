#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QObject>
#include <QTimer>

#include "dataminer.h"
#include "filewatcher.h"

class MyProcess : public QObject
{
    Q_OBJECT
public:
    MyProcess(const QString &inputPath, const QString &outputPath, QObject *parent = nullptr);
    ~MyProcess() override;
signals:
    void stop();
private slots:
    void onTimeout();
private:
    QString m_inputPath;
    QString m_outputPath;
    QTimer m_timer;
    DataMiner *m_dataMiner;
    FileWatcher *m_fileWatcher;
};

#endif // MYPROCESS_H
