//----------------------------------------------------------------------------
//  Copyright 2011. All Rights Reserved.
//  Author:   Vladislav Zverev
//----------------------------------------------------------------------------
#ifndef GTTSPERECODER_H
#define GTTSPERECODER_H

#include "dmpacket2.h"
#include "dmpacket.h"

class GtTsPereCoder
{
public:

    static void Perepack(int src_pck, size_t icount, const TDatagram& src, TDatagram2& dst);
    static void Perepack2(int src_pck, size_t icount, const TDatagram2& src, TDatagram2& dst);

};

#endif // GTTSPERECODER_H
