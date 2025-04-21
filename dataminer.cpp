#include "dataminer.h"

#include <fstream>
#include <iostream>

#include "newdata.h"

DataMiner::DataMiner()
{

}

void DataMiner::newData(const QFileInfo *fileInfo)
{
    processStream(fileInfo);
    emit(finished());
}

void DataMiner::processSystem()
{
    // TODO
}
void DataMiner::processStream(const QFileInfo *fileInfo) {
    std::ifstream ifs;
    ifs.open(fileInfo->absoluteFilePath().toStdString(), std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        std::cout << "Can't open input file" << std::endl;
        return;
    }

    std::ofstream ofs;
    ofs.open("adcm.dat.mod", std::ios::out | std::ios::binary | std::ios::app);
    if (!ofs.is_open())
    {
        std::cout << "Can't open output file" << std::endl;
        return;
    }
    NewData newBlock;
    auto nd{newBlock.data()};
    nd.time = fileInfo->lastModified().toMSecsSinceEpoch();
    newBlock.setData(nd);
    ofs << newBlock;
    char buffer[1024];
    while (ifs.read(buffer, sizeof (buffer)))
    {
        ofs.write(buffer, ifs.gcount());
    }
    ofs.write(buffer, ifs.gcount());
    ifs.close();
    ofs.close();
}
