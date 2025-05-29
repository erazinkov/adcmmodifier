#include "filewatcher.h"

#include <QDebug>
#include <QTimer>

#include <iostream>
#include <filesystem>

FileWatcher::FileWatcher(const std::string &path) : m_path(path)
{
    stat(m_path.c_str(), &m_stat);
    m_modTimeNs = m_stat.st_mtime;
    m_modTimeNs *= 1'000'000'000;
    m_modTimeNs += m_stat.st_mtim.tv_nsec;
}

bool FileWatcher::process()
{
   auto prevModTime{m_stat.st_mtime};
   auto prevFileSize{m_stat.st_size};
   if (stat(m_path.c_str(), &m_stat) == 0)
   {
       if (prevModTime < m_stat.st_mtime && prevFileSize != m_stat.st_size) {
           m_modTimeNs = m_stat.st_mtime;
           m_modTimeNs *= 1'000'000'000;
           m_modTimeNs += m_stat.st_mtim.tv_nsec;
           return true;
       }
   }
   return false;
}

long long FileWatcher::modTimeNs() const
{
    return m_modTimeNs;
}

bool FileWatcher::checkPath(const std::string &path)
{
    struct stat checkStat;
    if (stat(path.c_str(), &checkStat) != 0)
    {
        return false;
    }
    return true;
}
