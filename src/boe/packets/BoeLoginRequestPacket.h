/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_LOGIN_REQUEST_PACKET_H
#define BOE_LOGIN_REQUEST_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoeHeaderPacket.h"
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeLoginRequestPacket
{
    public:

        char                    mSessionSubID[4];
        char                    mUsername[4];
        char                    mPassword[10];
        uint8_t                 mNumberOfParamGroups;


        BoeLoginRequestPacket ()
        {
        }

        string  getSessionSubID()
        {
            return getString(mSessionSubID, sizeof(mSessionSubID));
        }

        string  getUsername()
        {
            return getString(mUsername, sizeof(mUsername)) ;
        }

        string  getPassword()
        {
            return getString(mPassword, sizeof(mPassword));
        }

        uint8_t  getNumberOfParamGroups()
        {
            return mNumberOfParamGroups;
        }

        bool setSessionSubID(char* val)
        {
            return setString(mSessionSubID, (unsigned char*)val, sizeof(mSessionSubID));
        }

        bool setUsername(char* val)
        {
            return setString(mUsername, (unsigned char*)val, sizeof(mUsername));
        }

        bool setPassword(char* val)
        {
            return setString(mPassword, (unsigned char*)val, sizeof(mPassword));
        }

        bool setNumberOfParamGroups(uint8_t val)
        {
            mNumberOfParamGroups = val;
            return true;
        }
});


} // namespace neueda

#endif // BOE_LOGIN_REQUEST_PACKET_H
