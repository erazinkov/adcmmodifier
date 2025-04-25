#include "dataminer.h"

#include <fstream>
#include <iostream>

#include "newdata.h"

DataMiner::DataMiner()
{

}

void DataMiner::newData(const std::string &path, const long long &modTimeNs)
{
    processStream(path, modTimeNs);
    emit(finished());
}

void DataMiner::newDataOffline(std::ifstream &ifs, std::ofstream &ofs, const long long &size, const long long &modTimeNs)
{
    processStreamOffline(ifs, ofs, size, modTimeNs);
    emit(finished());
}


void DataMiner::processSystem()
{
    // TODO
}
void DataMiner::processStream(const std::string &path, const long long &modTimeNs)
{
    std::ifstream ifs;

    ifs.open(path, std::ios::in | std::ios::binary);
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
    nd.time = modTimeNs;
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

void DataMiner::processStreamOffline(
        std::ifstream &ifs,
        std::ofstream &ofs,
        const long long &size,
        const long long &modTimeNs
        )
{
    NewData newBlock;
    auto nd{newBlock.data()};
    nd.time = modTimeNs;
    newBlock.setData(nd);
    ofs << newBlock;
    std::vector<char> buffer;
    buffer.resize(static_cast<ulong>(size));
    ifs.read(&buffer[0], static_cast<long>(buffer.size()));
    ofs.write(&buffer[0], static_cast<long>(buffer.size()));
}

