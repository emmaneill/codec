/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 25/07/2018
 */
#ifndef BOE_TRADE_CAPTURE_REPORT_ACKNOWLEDGEMENT_PACKET_H
#define BOE_TRADE_CAPTURE_REPORT_ACKNOWLEDGEMENT_PACKET_H

#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <stdint.h>
#include <stdexcept>
#include "BoePacketUtils.h"

namespace neueda
{

PACKED(class BoeTradeCaptureReportAcknowledgementPacket
{
    public:
        uint64_t                mTransactTime;
        char                    mTradeReportID[20];
        uint8_t                 mReservedInternal;
        uint8_t                 mNumberOfBitfields;


        BoeTradeCaptureReportAcknowledgementPacket ()
        {
        }

        string getTradeReportID()
        {
            return getString(mTradeReportID, sizeof(mTradeReportID));
        }

        uint64_t getTransactTime ()
        {
            return mTransactTime;
        }

        uint16_t getNumberOfBitfields ()
        {
            return mNumberOfBitfields;
        }

        bool setTradeReportID(std::string val)
        {
            return setString(mTradeReportID, val, 20);
        }

        bool setTransactTime(uint64_t val)
        {
            mTransactTime = val;
            return true;
        }

        bool setNumberOfBitfields(uint8_t val)
        {
            mNumberOfBitfields = val;
            return true;
        }

});


} // namespace neueda

#endif // BOE_TRADE_CAPTURE_REPORT_ACKNOWLEDGEMENT_PACKET_H
