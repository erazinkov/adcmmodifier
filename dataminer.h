#ifndef DATAMINER_H
#define DATAMINER_H

#include <QObject>
#include <QFileInfo>

class DataMiner : public QObject
{
    Q_OBJECT
public:
    DataMiner();
signals:
    void finished();
public slots:
    void newData(const QFileInfo *);
private:
    void processStream(const QFileInfo *);
    void processSystem();
};

#endif // DATAMINER_H
