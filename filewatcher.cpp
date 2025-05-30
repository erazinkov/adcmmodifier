#include "filewatcher.h"

#include <QDebug>
#include <QTimer>

#include <iostream>
#include <errno.h>

FileWatcher::FileWatcher(const std::string &path) : m_path(path)
{
    stat(m_path.c_str(), &m_stat);
    m_modTimeNs = m_stat.st_mtime;
    m_modTimeNs *= 1'000'000'000;
    m_modTimeNs += m_stat.st_mtim.tv_nsec;
}

FileWatcher::ProcessResult FileWatcher::process()
{
    using Status = ProcessResult::Status;
    auto prevModTime{m_stat.st_mtime};
    auto prevFileSize{m_stat.st_size};
    if (stat(m_path.c_str(), &m_stat) == 0)
    {
        if (prevModTime < m_stat.st_mtime && prevFileSize != m_stat.st_size) {
            m_modTimeNs = m_stat.st_mtime;
            m_modTimeNs *= 1'000'000'000;
            m_modTimeNs += m_stat.st_mtim.tv_nsec;
            return { Status::Ok };
        }
        return { Status::Wait };
    }
    return { Status::Error,  strerror(errno)};
}

long long FileWatcher::modTimeNs() const
{
    return m_modTimeNs;
}

