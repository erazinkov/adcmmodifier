#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <sys/stat.h>
#include <ctime>
#include <iomanip>

enum class ProcessStatus {
    MODIFY,
    ERROR,
    WAIT,
};

class FileWatcher
{
public:
    FileWatcher(const std::string &);
    ProcessStatus process();
    long long modTimeNs() const;

private:
    const std::string m_path;
    struct stat m_stat;
    long long m_modTimeNs;
};

#endif // FILEWATCHER_H
