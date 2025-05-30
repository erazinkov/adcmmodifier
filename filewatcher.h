#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <optional>

enum class ProcessStatus {
    MODIFY,
    ERROR,
    WAIT,
};

class FileWatcher
{
public:
    FileWatcher(const std::string &);
    struct ProcessResult {
        enum class Status {
            Ok,
            Error,
            Wait
        };
        Status statusCode{Status::Error};
        std::optional<std::string> errorString{std::nullopt};
    };
    ProcessResult process();
    long long modTimeNs() const;



private:
    const std::string m_path;
    struct stat m_stat;
    long long m_modTimeNs;
};

#endif // FILEWATCHER_H
