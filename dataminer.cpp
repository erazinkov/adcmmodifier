#include "dataminer.h"

#include <fstream>
#include <iostream>

#include "newdata.h"

DataMiner::DataMiner(std::string outPath, std::string tempPath)
    : _outPath(outPath), _tempPath(tempPath)
{

}

void DataMiner::newData(const std::string &path, const long long &modTimeNs)
{
    std::string str;
    str.append("cp ").append(path).append(" ").append(_tempPath);
    auto r{exec(wrap(str))};
    if (r != 0 )
    {
        std::cout << "Can\'t create temporary copy " << _tempPath << " of " << path;
        return;
    }

//    processStream(_tempPath, modTimeNs);
    processSystem(_tempPath, modTimeNs);
    str.clear();
    str.append("rm ").append(_tempPath);
    r = exec(wrap(str));
    if (r != 0 )
    {
        std::cout << "Can\'t remove temporary copy " << _tempPath << " of " << path;
    }
    emit(finished());
}

void DataMiner::newDataOffline(std::ifstream &ifs, std::ofstream &ofs, const long long &size, const long long &modTimeNs)
{
    processStreamOffline(ifs, ofs, size, modTimeNs);
    emit(finished());
}

int DataMiner::exec(const std::string &command) const
{   
    const int exitCode = system(command.c_str());
    return exitCode;
}

std::string DataMiner::wrap(const std::string &command) const
{
    std::string wCommand;
    wCommand.append("$(which bash) -c \'");
    wCommand.append(command);
    wCommand.append("\'");
    return wCommand;
}

std::vector<std::byte> getByteArray(const std::string& str)
{
    std::vector<std::byte> buffer;
    for (char str_char : str)
    {
        buffer.push_back(std::byte(str_char));
    }

    return buffer;
}

void DataMiner::processSystem(const std::string &path, const long long &modTimeNs)
{   
    NewData newBlock;
    auto nd{newBlock.data()};
    nd.time = modTimeNs;
    newBlock.setData(nd);

    std::vector<uint8_t> bytes{newBlock.bytes()};

    std::stringstream ss;
    ss.str("");ss.clear();

    std::string str;
    str.append("echo -n -e ");
    for (uint8_t byte : bytes)
    {
        ss.str("");ss.clear();
        str.append("\\\\x");
        ss << std::hex << static_cast<int>(byte);
        str.append(ss.str());
    }
    str.append(" > ").append(_outPath);
    auto r{exec(wrap(str))};
    if (r != 0)
    {
        std::cout << "Can\'t modify output file " << _outPath;
        return;
    }
    str.clear();
    str.append("cat ").append(path).append(" >> ").append(_outPath);
    r = exec(wrap(str));
    if (r != 0)
    {
        std::cout << "Can\'t concatenate intput file " << path << " to output file " << _outPath;
        // TODO
        return;
    }
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
    ofs.open(_outPath, std::ios::out | std::ios::binary | std::ios::app);
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

