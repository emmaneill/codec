/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_SIDE_GROUP_PACKET_H
#define BOE_SIDE_GROUP_PACKET_H

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

PACKED(class BoeSideGroupPacket
{
    public:

        char                    mSide[1];
        char                    mCapacity[1];
        char                    mAccount[16];
        char                    mPartyID[4];
        char                    mPartyRole[1];


        BoeSideGroupPacket ()
        {
        }

        string  getSide()
        {
            return getString(mSide, sizeof(mSide)) ;
        }
        
        string  getCapacity()
        {
            return getString(mCapacity, sizeof(mCapacity)) ;
        }
        
        string  getAccount()
        {
            return getString(mAccount, sizeof(mAccount)) ;
        }

        string  getPartyID()
        {
            return getString(mPartyID, sizeof(mPartyID)) ;
        }

        string  getPartyRole()
        {
            return getString(mPartyRole, sizeof(mPartyRole)) ;
        }

        bool setSide(char* val)
        {
            return setString(mSide, (unsigned char*)val, sizeof(mSide));
        }

        bool setCapacity(char* val)
        {
            return setString(mCapacity, (unsigned char*)val, sizeof(mCapacity));
        }

        bool setAccount(char* val)
        {
            return setString(mAccount, (unsigned char*)val, sizeof(mAccount));
        }

        bool setPartyID(char* val)
        {
            return setString(mPartyID, (unsigned char*)val, sizeof(mPartyID));
        }

        bool setPartyRole(char* val)
        {
            return setString(mPartyRole, (unsigned char*)val, sizeof(mPartyRole));
        }
});


} // namespace neueda

#endif // BOE_SIDE_GROUP_PACKET_H
