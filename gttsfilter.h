//----------------------------------------------------------------------------
//  Copyright 2011. All Rights Reserved.
//  Author:   Vladislav Zverev
//----------------------------------------------------------------------------
#ifndef GTTSFILTER_H
#define GTTSFILTER_H

#include "dmpacket2.h"
#include "dmpacket.h"


class GtTsFilter
{
public:
    GtTsFilter() = default;
    bool open(const char* fn);

    bool sift(const TDatagram& src, TDatagram2& dst);
    bool sift2(const TDatagram2& src, TDatagram2& dst);
    void clear();

private:
    void print();
};

#endif // GTTSFILTER_H
