#include "dataminer.h"

#include <fstream>
#include <sstream>

#include "newdata.h"

DataMiner::DataMiner(std::string inputPath, std::string outPath, std::string tempPath)
    : inputPath_(inputPath), outPath_(outPath), tempPath_(tempPath)
{
}

void DataMiner::process(const long long &modTimeNs) const
{
    std::string str;
    str.append("cp ").append(inputPath_).append(" ").append(tempPath_);
    auto r{exec(str)};
    if (r != 0 )
    {
        std::cout << "Can\'t create temporary copy " << tempPath_ << " of " << inputPath_;
        return;
    }

//    processStream(modTimeNs);
    processSystem(modTimeNs);
//    str.clear();
//    str.append("rm ").append(tempPath_);
//    r = exec(str);
//    if (r != 0 )
//    {
//        std::cout << "Can\'t remove temporary copy " << tempPath_ << " of " << inputPath_;
//    }
}

int DataMiner::exec(const std::string &command) const
{   
    std::string wC{"$(which bash) -c \'"};
    wC.append(command);
    wC.append("\'");
    const int exitCode = system(wC.c_str());
    return exitCode;
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

void DataMiner::processSystem(const long long &modTimeNs) const
{   
    NewData newBlock;
    auto nd{newBlock.data()};
    nd.time = modTimeNs;
    newBlock.setData(nd);

    std::vector<uint8_t> bytes{newBlock.bytes()};

    auto r{-2};
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
    str.append(" >> ").append(outPath_);
    r = exec(str);
    if (r != 0)
    {
        std::cout << "Can\'t modify output file " << outPath_;
        return;
    }
    str.clear();
    str.append("cat ").append(tempPath_).append(" >> ").append(outPath_);
    r = exec(str);
    if (r != 0)
    {
        std::cout << "Can\'t concatenate input file " << tempPath_ << " to output file " << outPath_;
        // TODO
        return;
    }
}
void DataMiner::processStream(const long long &modTimeNs) const
{
    std::ifstream ifs;

    ifs.open(tempPath_, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
    {
        std::cout << "Can't open input file" << std::endl;
        return;
    }

    std::ofstream ofs;
    ofs.open(outPath_, std::ios::out | std::ios::binary | std::ios::app);
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

