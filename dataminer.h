#ifndef DATAMINER_H
#define DATAMINER_H

#include <sys/stat.h>
#include <iostream>

class DataMiner
{
public:
    DataMiner(std::string inputPath, std::string outPath, std::string tempPath = "adcm.dat.tmp");
public:
    void process(const long long &) const;
private:
    std::string inputPath_;
    std::string outPath_;
    std::string tempPath_;

    void processStream(const long long &) const;
    void processSystem(const long long &) const;
    int exec(const std::string &) const;
};

#endif // DATAMINER_H
