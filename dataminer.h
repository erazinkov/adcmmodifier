#ifndef DATAMINER_H
#define DATAMINER_H

#include <QObject>
#include <QFileInfo>

#include <sys/stat.h>

class DataMiner : public QObject
{
    Q_OBJECT
public:
    DataMiner(std::string outPath, std::string tempPath = "adcm.dat.tmp");
signals:
    void finished();
public slots:
    void newData(const std::string &, const long long &);
    void newDataOffline(std::ifstream &, std::ofstream &, const long long &, const long long &);
private:
    std::string _outPath;
    std::string _tempPath;

    void processStream(const std::string &path, const long long &);
    void processStreamOffline(
            std::ifstream &,
            std::ofstream &,
            const long long &,
            const long long &
            );
    void processSystem(const std::string &, const long long &);
    int exec(const std::string &) const;
};

#endif // DATAMINER_H
