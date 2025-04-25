#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>

#include <sys/stat.h>
#include <ctime>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    FileWatcher(const std::string &path, QObject *parent = nullptr);
signals:
    void onFileChanged(const std::string &, const long long &);
public slots:
    void operate();
private:
    std::string m_path;
    struct stat m_stat;
    long long m_modTimeNs;
};

#endif // FILEWATCHER_H
