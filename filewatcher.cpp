#include "filewatcher.h"

#include <QDebug>

#include <iostream>
#include <filesystem>

FileWatcher::FileWatcher(const std::string &path, QObject *parent) : QObject(parent), m_path(path)
{
    stat(m_path.c_str(), &m_stat);
    m_modTimeNs = m_stat.st_mtime;
    m_modTimeNs *= 1'000'000'000;
    m_modTimeNs += m_stat.st_mtim.tv_nsec;
}

void FileWatcher::operate()
{
   auto modTime{m_stat.st_mtime};
   auto fileSize{m_stat.st_size};
   stat(m_path.c_str(), &m_stat);
   if (modTime != m_stat.st_mtime && fileSize != m_stat.st_size) {
       m_modTimeNs = m_stat.st_mtime;
       m_modTimeNs *= 1'000'000'000;
       m_modTimeNs += m_stat.st_mtim.tv_nsec;
       std::cout << m_modTimeNs << std::endl;
       emit(onFileChanged(m_path.c_str(), m_modTimeNs));
   }
}
