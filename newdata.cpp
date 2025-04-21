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
