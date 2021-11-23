//----------------------------------------------------------------------------
//  Copyright 2011. All Rights Reserved.
//  Author:   Vladislav Zverev
//----------------------------------------------------------------------------
#include "gttsperecoder.h"
#include "iii.h"
#include "all_std.h"

void GtTsPereCoder::Perepack(int src_pck, size_t icount, const TDatagram& src, TDatagram2& dst)
{
    if (icount == 0)
        throw std::logic_error("GtTsPereCoder::Perepack icount==0 !!!");

    dst.Clear();
    dst.Type = src.Type;
    dst.setName(src.Name);
    dst.Size = ((icount - 1) / 8 + 1) * 2;

    const uint8_t  *srcbyte = &src.Data[1];
    uint8_t  *dstbyte = &dst.Data[0];
    for (size_t i = 0; i < icount; ++i) {
        int state = III(src_pck, (const char*)srcbyte, i);

        int n2byte = i / 4;
        int n2bit = i % 4;
        //стираем
        dstbyte[n2byte] &= ~(0x03 << (n2bit * 2));
        //устанавливаем
        int s = state & 0x03;
        dstbyte[n2byte] |= (s << (n2bit * 2));
    }
}
//---------------------------------------------------------------------------
void GtTsPereCoder::Perepack2(int src_pck, size_t icount, const TDatagram2& src, TDatagram2& dst)
{
    if (icount == 0)
        throw std::logic_error("GtTsPereCoder::Perepack icount==0 !!!");

    dst.Clear();
    dst.Type = src.Type;
    dst.setName(src.Name);
    dst.Size = ((icount - 1) / 8 + 1) * 2;

    const uint8_t  *srcbyte = &src.Data[1];
    uint8_t  *dstbyte = &dst.Data[0];
    for (size_t i = 0; i < icount; ++i) {
        int state = III(src_pck, (const char*)srcbyte, i);

        int n2byte = i / 4;
        int n2bit = i % 4;
        //стираем
        dstbyte[n2byte] &= ~(0x03 << (n2bit * 2));
        //устанавливаем
        int s = state & 0x03;
        dstbyte[n2byte] |= (s << (n2bit * 2));
    }
}
//---------------------------------------------------------------------------

