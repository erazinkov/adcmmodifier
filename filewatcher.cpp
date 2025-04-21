#include "filewatcher.h"

#include <QDebug>

FileWatcher::FileWatcher(const QString &path, QObject *parent) : QObject(parent), m_path(path)
{
    m_fileInfo = new QFileInfo(m_path);
}

void FileWatcher::operate()
{
    auto lastModified{m_fileInfo->lastModified()};
    auto size{m_fileInfo->size()};
    m_fileInfo->refresh();
    if (lastModified != m_fileInfo->lastModified() && size != m_fileInfo->size())
    {

        emit(onFileChanged(m_fileInfo));
    }
}
