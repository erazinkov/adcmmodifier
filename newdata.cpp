#include "newdata.h"

#include <iostream>

NewData::NewData()
{
    _data.time = 0;
    _hdr.id = STOR_ID_ND;
    _hdr.size = sizeof (_data) + sizeof (STOR_ID_ND);
}

const stor_nd_t &NewData::data() const
{
    return _data;
}

const stor_packet_hdr_t &NewData::hdr() const
{
    return _hdr;
}

void NewData::setData(const stor_nd_t &newData)
{
    _data = newData;
}

std::ofstream &operator << (std::ofstream &stream, NewData &d)
{
    stream << d._hdr << d._data;
    return stream;
}

std::ifstream &operator >> (std::ifstream &stream, NewData &d)
{
    stream >> d._hdr >> d._data;
    return stream;
}

std::vector<uint8_t> NewData::bytes()
{
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < sizeof(_hdr.id); ++i)
    {
        bytes.push_back((_hdr.id >> (i * 8)) & 0xFF);
    }
    for (size_t i = 0; i < sizeof(_hdr.size); ++i)
    {
        bytes.push_back((_hdr.size >> (i * 8)) & 0xFF);
    }
    for (size_t i = 0; i < sizeof(_data.time); ++i)
    {
        bytes.push_back((_data.time >> (i * 8)) & 0xFF);
    }
    return bytes;
}
