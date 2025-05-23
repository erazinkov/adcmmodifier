#ifndef NEWDATA_H
#define NEWDATA_H

#include <sys/types.h>
#include <fstream>

#include "adcm_df.h"

class NewData
{

public:
    NewData();
    friend std::ofstream &operator << (std::ofstream &, NewData &);
    friend std::ifstream &operator >> (std::ifstream &, NewData &);

    const stor_nd_t &data() const;

    const stor_packet_hdr_t &hdr() const;

    void setData(const stor_nd_t &newData);

    std::vector<uint8_t> bytes();
private:
    stor_packet_hdr_t _hdr;
    stor_nd_t _data;
};

#endif // NEWDATA_H
