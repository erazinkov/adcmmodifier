#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QThread>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    FileWatcher(const QString &path, QObject *parent = nullptr);
signals:
    void onFileChanged(const QFileInfo *);
public slots:
    void operate();
private:
    QString m_path;
    QFileInfo *m_fileInfo;
};

#endif // FILEWATCHER_H
