#ifndef DATAMINER_H
#define DATAMINER_H

#include <QObject>
#include <QFileInfo>

#include <sys/stat.h>

class DataMiner : public QObject
{
    Q_OBJECT
public:
    DataMiner();
signals:
    void finished();
public slots:
    void newData(const std::string &, const long long &modTimeNs);
private:
    void processStream(const std::string &path, const long long &modTimeNs);
    void processSystem();
};

#endif // DATAMINER_H
