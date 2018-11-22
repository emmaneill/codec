/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_MODIFYORDER_PACKET_H_
#define _BOE_MODIFYORDER_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeModifyOrderPacket
{
public:
        char mClOrdID[20];
        char mOrigClOrdID[20];
        uint8_t mNumberOfBitfields;
        char mClearingFirm[4];
        uint32_t mOrderQty;
        uint64_t mPrice;
        char mOrdType[1];
        char mCancelOrigOnReject[1];
        char mExecInst[1];

    boeModifyOrderPacket ()
    {
        memset (mClOrdID, 0, 20);
        memset (mOrigClOrdID, 0, 20);
        mNumberOfBitfields = 0;
        memset (mClearingFirm, 0, 4);
        mOrderQty = UINT32_MAX;
        mPrice = UINT64_MAX;
        memset (mOrdType, 0, 1);
        memset (mCancelOrigOnReject, 0, 1);
        memset (mExecInst, 0, 1);
    }

    
    string getClOrdID ()
    {
        return getString (mClOrdID, sizeof (mClOrdID));
    }

    bool setClOrdID (const string& val)
    {
        return setString (mClOrdID, val, sizeof (mClOrdID));
    }
    
    string getOrigClOrdID ()
    {
        return getString (mOrigClOrdID, sizeof (mOrigClOrdID));
    }

    bool setOrigClOrdID (const string& val)
    {
        return setString (mOrigClOrdID, val, sizeof (mOrigClOrdID));
    }
    
    uint8_t getNumberOfBitfields ()
    {
        return mNumberOfBitfields;
    }

    bool setNumberOfBitfields (uint8_t val)
    {
        mNumberOfBitfields = val;
        return true;
    }
    
    string getClearingFirm ()
    {
        return getString (mClearingFirm, sizeof (mClearingFirm));
    }

    bool setClearingFirm (const string& val)
    {
        return setString (mClearingFirm, val, sizeof (mClearingFirm));
    }
    
    uint32_t getOrderQty ()
    {
        return mOrderQty;
    }

    bool setOrderQty (uint32_t val)
    {
        mOrderQty = val;
        return true;
    }
    
    uint64_t getPrice ()
    {
        return mPrice;
    }

    bool setPrice (uint64_t val)
    {
        mPrice = val;
        return true;
    }
    
    string getOrdType ()
    {
        return getString (mOrdType, sizeof (mOrdType));
    }

    bool setOrdType (const string& val)
    {
        return setString (mOrdType, val, sizeof (mOrdType));
    }
    
    string getCancelOrigOnReject ()
    {
        return getString (mCancelOrigOnReject, sizeof (mCancelOrigOnReject));
    }

    bool setCancelOrigOnReject (const string& val)
    {
        return setString (mCancelOrigOnReject, val, sizeof (mCancelOrigOnReject));
    }
    
    string getExecInst ()
    {
        return getString (mExecInst, sizeof (mExecInst));
    }

    bool setExecInst (const string& val)
    {
        return setString (mExecInst, val, sizeof (mExecInst));
    }
});

} // namespace neueda

#endif
