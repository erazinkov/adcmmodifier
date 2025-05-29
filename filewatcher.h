#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <sys/stat.h>
#include <ctime>
#include <iomanip>

class FileWatcher
{
public:
    FileWatcher(const std::string &);
    bool process();
    long long modTimeNs() const;

private:
    const std::string m_path;
    struct stat m_stat;
    long long m_modTimeNs;
};

#endif // FILEWATCHER_H
