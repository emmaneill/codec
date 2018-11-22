/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef _BOE_LOGINREQUEST_PACKET_H_
#define _BOE_LOGINREQUEST_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>

#include "BoePacketUtils.h"
#include "BoeHeaderPacket.h"


namespace neueda
{

PACKED(class boeLoginRequestPacket
{
public:
        char mTradingSessionSubID[4];
        char mUsername[4];
        char mPassword[10];
        int8_t mNumberOfParamGroups;

    boeLoginRequestPacket ()
    {
        memset (mTradingSessionSubID, 0, 4);
        memset (mUsername, 0, 4);
        memset (mPassword, 0, 10);
        mNumberOfParamGroups = 0;
    }

    
    string getTradingSessionSubID ()
    {
        return getString (mTradingSessionSubID, sizeof (mTradingSessionSubID));
    }

    bool setTradingSessionSubID (const string& val)
    {
        return setString (mTradingSessionSubID, val, sizeof (mTradingSessionSubID));
    }
    
    string getUsername ()
    {
        return getString (mUsername, sizeof (mUsername));
    }

    bool setUsername (const string& val)
    {
        return setString (mUsername, val, sizeof (mUsername));
    }
    
    string getPassword ()
    {
        return getString (mPassword, sizeof (mPassword));
    }

    bool setPassword (const string& val)
    {
        return setString (mPassword, val, sizeof (mPassword));
    }
    
    int8_t getNumberOfParamGroups ()
    {
        return mNumberOfParamGroups;
    }

    bool setNumberOfParamGroups (int8_t val)
    {
        mNumberOfParamGroups = val;
        return true;
    }
});

} // namespace neueda

#endif
