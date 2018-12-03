/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 00:20:32 03/12/18
 */
#include "boeCodec.h"
#include "BoePackets.h"
#include "fields.h"
#include "codecHelpers.h"
#include <sstream>


namespace neueda
{

extern "C" codec*
getCodec ()
{
    return new boeCodec ();
}

codecState
boeCodec::getLoginRequest (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "LoginRequest");
    size_t offset = sizeof (BoeHeaderPacket);
    boeLoginRequestPacket* packet = (boeLoginRequestPacket*)((char*)buf + offset);
    
    d.setString (TradingSessionSubID, packet->getTradingSessionSubID ());
    offset += 4;
    
    d.setString (Username, packet->getUsername ());
    offset += 4;
    
    d.setString (Password, packet->getPassword ());
    offset += 10;
    
    d.setInteger (NumberOfParamGroups, packet->getNumberOfParamGroups ());
    offset += sizeof (uint8_t);
    int NumParamGroups = packet->getNumberOfParamGroups ();

    cdrArray ParamGroupsArray;
    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

        item.setInteger(ParamGroupLength, paramGroupsElement->getParamGroupLength ());
        offset += sizeof (uint16_t);

        item.setInteger(ParamGroupType, paramGroupsElement->getParamGroupType ());
        offset += sizeof (uint8_t);

        if(paramGroupsElement->getParamGroupType () == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            offset += sizeof (BoeUnitSequencePacket);

            cdrArray UnitSequenceArray;
            item.setInteger(NoUnspecifiedUnitReplay, unitSequenceElement->getNoUnspecifiedUnitReplay ());

            int NumUnits = unitSequenceElement->getNumOfUnits ();
            item.setInteger(NumOfUnits, unitSequenceElement->getNumOfUnits ());

            cdrArray UnitGroupArray;
            for (int i = 0; i < NumUnits; i++)
            {
                cdr unitGroupItem;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                unitGroupItem.setInteger (UnitNumber, unitGroupElement->getUnitNumber ());
                offset += sizeof (uint8_t);

                unitGroupItem.setInteger (UnitSequence, unitGroupElement->getUnitSequence ());
                offset += sizeof (uint32_t);

                UnitGroupArray.push_back (unitGroupItem);
            }
            item.setArray (UnitGroupSection, UnitGroupArray);
            d.setArray (UnitSequenceSection, UnitSequenceArray);
        }
        else if(paramGroupsElement->getParamGroupType () == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);

            cdrArray ReturnBitfieldArray;
            item.setInteger(MessageType, returnBitfieldElement->getMessageType ());
            offset += sizeof (uint8_t);

            int NumReturnBitfields = returnBitfieldElement->getNumOfReturnBitfield ();
            item.setInteger(NumOfReturnBitfield, returnBitfieldElement->getNumOfReturnBitfield ());
            offset += sizeof (uint8_t);

            //copying bitfields to new temporary buffer
            char* tmpbuf = new char[NumReturnBitfields];
            memcpy ((void*)tmpbuf, (void*)buf+offset, NumReturnBitfields);

            for (int i = 0; i < NumReturnBitfields; i++)
            {
                BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
                item.setInteger (Bitfield, bitfieldElement->getBitfield ());
                ReturnBitfieldArray.push_back(item);
                offset += sizeof (BoeBitfieldPacket);
            }
            d.setArray(BitfieldSection, ReturnBitfieldArray);

            memset (tmpbuf+NumReturnBitfields, 0, (ORDERMSG_BITFIELDS_SIZE - NumReturnBitfields));
            OrderMsgBits*  mReturnBits = (OrderMsgBits*) tmpbuf;
            if (mReturnBits == NULL)
            {
               if (NumReturnBitfields !=0)
               {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
               }
               else
               {
                   used += offset;
                   return GW_CODEC_SUCCESS;
               }
            }
            // TODO - Get optional fields
        }
    }
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLoginResponse (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "LoginResponse");
    size_t offset = sizeof (BoeHeaderPacket);
    boeLoginResponsePacket* packet = (boeLoginResponsePacket*)((char*)buf + offset);
    
    d.setInteger (LoginResponseStatus, packet->getLoginResponseStatus ());
    offset += sizeof (uint8_t);
    
    d.setString (LoginResponseText, packet->getLoginResponseText ());
    offset += 60;
    
    d.setInteger (NoUnspecifiedUnitReplay, packet->getNoUnspecifiedUnitReplay ());
    offset += sizeof (uint8_t);
    
    d.setInteger (LastReceivedSequenceNumber, packet->getLastReceivedSequenceNumber ());
    offset += sizeof (uint32_t);
    
    d.setInteger (NumberOfUnits, packet->getNumberOfUnits ());
    offset += sizeof (uint8_t);
    
    int numInGroup = 0;
    d.getInteger (NumberOfUnits, numInGroup) ;
    cdrArray UnitGroupArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeUnitGroupPacket* packet6 = (BoeUnitGroupPacket*)((char*)buf + offset);

        item.setInteger (UnitNumber, packet6->getUnitNumber ());
        offset += sizeof (uint8_t);

        item.setInteger (UnitSequence, packet6->getUnitSequence ());
        offset += sizeof (uint32_t);

        UnitGroupArray.push_back (item);
    }
    d.setArray (UnitGroupSection, UnitGroupArray);

    // currently hardcoded as packets are assinging inccorectly
    int NumParamGroups = *(uint8_t*)(buf + offset);
    d.setInteger (NumberOfParamGroups, *(uint8_t*)(buf + offset));
    offset += sizeof (uint8_t);

    cdrArray ParamGroupsArray;
    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

        item.setInteger(ParamGroupLength, paramGroupsElement->getParamGroupLength ());
        offset += sizeof (uint16_t);

        item.setInteger(ParamGroupType, paramGroupsElement->getParamGroupType ());
        offset += sizeof (uint8_t);

        if(paramGroupsElement->getParamGroupType () == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            offset += sizeof (BoeUnitSequencePacket);

            cdrArray UnitSequenceArray;
            item.setInteger(NoUnspecifiedUnitReplay, unitSequenceElement->getNoUnspecifiedUnitReplay ());

            int NumUnits = unitSequenceElement->getNumOfUnits ();
            item.setInteger(NumOfUnits, unitSequenceElement->getNumOfUnits ());

            cdrArray UnitGroupArray;
            for (int i = 0; i < NumUnits; i++)
            {
                cdr unitGroupItem;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                unitGroupItem.setInteger (UnitNumber, unitGroupElement->getUnitNumber ());
                offset += sizeof (uint8_t);

                unitGroupItem.setInteger (UnitSequence, unitGroupElement->getUnitSequence ());
                offset += sizeof (uint32_t);

                UnitGroupArray.push_back (unitGroupItem);
            }
            item.setArray (UnitGroupSection, UnitGroupArray);
            d.setArray (UnitSequenceSection, UnitSequenceArray);
        }
        else if(paramGroupsElement->getParamGroupType () == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);

            cdrArray ReturnBitfieldArray;
            item.setInteger(MessageType, returnBitfieldElement->getMessageType ());
            offset += sizeof (uint8_t);

            int NumReturnBitfields = returnBitfieldElement->getNumOfReturnBitfield ();
            item.setInteger(NumOfReturnBitfield, returnBitfieldElement->getNumOfReturnBitfield ());
            offset += sizeof (uint8_t);

            //copying bitfields to new temporary buffer
            char* tmpbuf = new char[NumReturnBitfields];
            memcpy ((void*)tmpbuf, (void*)buf+offset, NumReturnBitfields);

            for (int i = 0; i < NumReturnBitfields; i++)
            {
                BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
                item.setInteger (Bitfield, bitfieldElement->getBitfield ());
                ReturnBitfieldArray.push_back(item);
                offset += sizeof (BoeBitfieldPacket);
            }
            d.setArray(BitfieldSection, ReturnBitfieldArray);

            memset (tmpbuf+NumReturnBitfields, 0, (ORDERMSG_BITFIELDS_SIZE - NumReturnBitfields));
            OrderMsgBits*  mReturnBits = (OrderMsgBits*) tmpbuf;
            if (mReturnBits == NULL)
            {
               if (NumReturnBitfields !=0)
               {
                   setLastError("Bitfields not correctly set");
                   return GW_CODEC_ERROR;
               }
               else
               {
                   used += offset;
                   return GW_CODEC_SUCCESS;
               }
            }
            // TODO - Get optional fields
        }
    }
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLogoutResponse (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "LogoutResponse");
    size_t offset = sizeof (BoeHeaderPacket);
    boeLogoutResponsePacket* packet = (boeLogoutResponsePacket*)((char*)buf + offset);
    
    d.setInteger (LogoutReason, packet->getLogoutReason ());
    offset += sizeof (uint8_t);
    
    d.setString (LogoutReasonText, packet->getLogoutReasonText ());
    offset += 60;
    
    d.setInteger (LastReceivedSequenceNumber, packet->getLastReceivedSequenceNumber ());
    offset += sizeof (uint32_t);
    
    d.setInteger (NumberOfUnits, packet->getNumberOfUnits ());
    offset += sizeof (uint8_t);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getServerHeartbeat (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "ServerHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getClientHeartbeat (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "ClientHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getReplayComplete (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "ReplayComplete");
    size_t offset = sizeof (BoeHeaderPacket);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getNewOrder (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "NewOrder");
    size_t offset = sizeof (BoeHeaderPacket);
    boeNewOrderPacket* packet = (boeNewOrderPacket*)((char*)buf + offset);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (Side, packet->getSide ());
    offset += sizeof (uint8_t);
    
    d.setInteger (OrderQty, packet->getOrderQty ());
    offset += sizeof (uint32_t);
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (NEWORDER_BITFIELDS_SIZE - numBitfields));
    mNewOrderBits = (NewOrderBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mNewOrderBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mNewOrderBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mNewOrderBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mNewOrderBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mNewOrderBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mNewOrderBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mNewOrderBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mNewOrderBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mNewOrderBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mNewOrderBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mNewOrderBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mNewOrderBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mNewOrderBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mNewOrderBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mNewOrderBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mNewOrderBits->hasExpireTime)
    {
        d.setInteger (ExpireTime, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mNewOrderBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mNewOrderBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mNewOrderBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mNewOrderBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mNewOrderBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mNewOrderBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mNewOrderBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mNewOrderBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getCancelOrder (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "CancelOrder");
    size_t offset = sizeof (BoeHeaderPacket);
    boeCancelOrderPacket* packet = (boeCancelOrderPacket*)((char*)buf + offset);
    
    d.setString (OrigClOrdID, packet->getOrigClOrdID ());
    offset += 20;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (CANCELORDER_BITFIELDS_SIZE - numBitfields));
    mCancelOrderBits = (CancelOrderBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mCancelOrderBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mCancelOrderBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getModifyOrder (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "ModifyOrder");
    size_t offset = sizeof (BoeHeaderPacket);
    boeModifyOrderPacket* packet = (boeModifyOrderPacket*)((char*)buf + offset);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setString (OrigClOrdID, packet->getOrigClOrdID ());
    offset += 20;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (MODIFYORDER_BITFIELDS_SIZE - numBitfields));
    mModifyOrderBits = (ModifyOrderBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mModifyOrderBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mModifyOrderBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mModifyOrderBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mModifyOrderBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mModifyOrderBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mModifyOrderBits->hasCancelOrigOnReject)
    {
        d.setString (CancelOrigOnReject, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mModifyOrderBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCaptureReport (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCaptureReport");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportPacket* packet = (boeTradeCaptureReportPacket*)((char*)buf + offset);
    
    d.setString (TradeReportID, packet->getTradeReportID ());
    offset += 20;
    
    d.setInteger (LastShares, packet->getLastShares ());
    offset += sizeof (uint32_t);
    
    d.setInteger (LastPx, packet->getLastPx ());
    offset += sizeof (uint64_t);
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECAPTUREREPORT_BITFIELDS_SIZE - numBitfields));
    mTradeCaptureReportBits = (TradeCaptureReportBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCaptureReportBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    d.setInteger (NoSides, packet->getNoSides ());
    offset += sizeof (uint8_t);
    
    if (mTradeCaptureReportBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCaptureReportBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportBits->hasTransactionCategory)
    {
        d.setString (TransactionCategory, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasTradeTime)
    {
        d.setInteger (TradeTime, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasTradeReportTransType)
    {
        d.setInteger (TradeReportTransType, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasTradeID)
    {
        d.setInteger (TradeID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasVenueType)
    {
        d.setString (VenueType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasTradingSessionSubID)
    {
        d.setInteger (TradingSessionSubID, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasMatchType)
    {
        d.setInteger (MatchType, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasTrdSubType)
    {
        d.setInteger (TrdSubType, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasSecondaryTrdType)
    {
        d.setInteger (SecondaryTrdType, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasTradePriceCondition)
    {
        d.setInteger (TradePriceCondition, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasExecutionMethod)
    {
        d.setString (ExecutionMethod, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasTradeReportType)
    {
        d.setInteger (TradeReportType, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasTradeHandlingInstruction)
    {
        d.setInteger (TradeHandlingInstruction, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasTradeLinkID)
    {
        d.setString (TradeLinkID, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportBits->hasGrossTradeAmt)
    {
        d.setInteger (GrossTradeAmt, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasTolerance)
    {
        d.setInteger (Tolerance, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCaptureReportBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportBits->hasSettlementPrice)
    {
        d.setInteger (SettlementPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasSettlementDate)
    {
        d.setInteger (SettlementDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportBits->hasSettlementCurrency)
    {
        d.setString (SettlementCurrency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportBits->hasSettlementLocation)
    {
        d.setString (SettlementLocation, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderAcknowledgement (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderAcknowledgement");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderAcknowledgementPacket* packet = (boeOrderAcknowledgementPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (OrderID, packet->getOrderID ());
    offset += sizeof (uint64_t);
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDERACKNOWLEDGEMENT_BITFIELDS_SIZE - numBitfields));
    mOrderAcknowledgementBits = (OrderAcknowledgementBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderAcknowledgementBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderAcknowledgementBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderAcknowledgementBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderAcknowledgementBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderAcknowledgementBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderAcknowledgementBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderAcknowledgementBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderAcknowledgementBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderAcknowledgementBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderAcknowledgementBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderAcknowledgementBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderAcknowledgementBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderAcknowledgementBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderAcknowledgementBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderAcknowledgementBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderAcknowledgementBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderAcknowledgementBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderAcknowledgementBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderAcknowledgementBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderAcknowledgementBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderAcknowledgementBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderRejected (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderRejected");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderRejectedPacket* packet = (boeOrderRejectedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setString (OrderRejectReason, packet->getOrderRejectReason ());
    offset += 1;
    
    d.setString (OrderRejectText, packet->getOrderRejectText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDERREJECTED_BITFIELDS_SIZE - numBitfields));
    mOrderRejectedBits = (OrderRejectedBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderRejectedBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderRejectedBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderRejectedBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderRejectedBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderRejectedBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderRejectedBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderRejectedBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderRejectedBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderRejectedBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderRejectedBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRejectedBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderRejectedBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderRejectedBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRejectedBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRejectedBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRejectedBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRejectedBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRejectedBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRejectedBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRejectedBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRejectedBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderModified (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderModified");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderModifiedPacket* packet = (boeOrderModifiedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (OrderID, packet->getOrderID ());
    offset += sizeof (uint64_t);
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDERMODIFIED_BITFIELDS_SIZE - numBitfields));
    mOrderModifiedBits = (OrderModifiedBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderModifiedBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderModifiedBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderModifiedBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderModifiedBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderModifiedBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderModifiedBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderModifiedBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderModifiedBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderModifiedBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderModifiedBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderModifiedBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderModifiedBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderModifiedBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderModifiedBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderModifiedBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderModifiedBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderModifiedBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderModifiedBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderModifiedBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderModifiedBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderModifiedBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderRestated (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderRestated");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderRestatedPacket* packet = (boeOrderRestatedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (OrderID, packet->getOrderID ());
    offset += sizeof (uint64_t);
    
    d.setInteger (RestatementReason, packet->getRestatementReason ());
    offset += sizeof (uint8_t);
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDERRESTATED_BITFIELDS_SIZE - numBitfields));
    mOrderRestatedBits = (OrderRestatedBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderRestatedBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderRestatedBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderRestatedBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderRestatedBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderRestatedBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderRestatedBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderRestatedBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderRestatedBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderRestatedBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderRestatedBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRestatedBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderRestatedBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderRestatedBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRestatedBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRestatedBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderRestatedBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderRestatedBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderRestatedBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderRestatedBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderRestatedBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderRestatedBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getUserModifyRejected (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "UserModifyRejected");
    size_t offset = sizeof (BoeHeaderPacket);
    boeUserModifyRejectedPacket* packet = (boeUserModifyRejectedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (ModifyRejectReason, packet->getModifyRejectReason ());
    offset += sizeof (uint8_t);
    
    d.setString (ModifyRejectText, packet->getModifyRejectText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (USERMODIFYREJECTED_BITFIELDS_SIZE - numBitfields));
    mUserModifyRejectedBits = (UserModifyRejectedBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mUserModifyRejectedBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mUserModifyRejectedBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mUserModifyRejectedBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mUserModifyRejectedBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mUserModifyRejectedBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mUserModifyRejectedBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mUserModifyRejectedBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mUserModifyRejectedBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mUserModifyRejectedBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mUserModifyRejectedBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mUserModifyRejectedBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mUserModifyRejectedBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mUserModifyRejectedBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mUserModifyRejectedBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mUserModifyRejectedBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mUserModifyRejectedBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mUserModifyRejectedBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mUserModifyRejectedBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mUserModifyRejectedBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mUserModifyRejectedBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mUserModifyRejectedBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderCancelled (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderCancelled");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderCancelledPacket* packet = (boeOrderCancelledPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (CancelReason, packet->getCancelReason ());
    offset += sizeof (uint8_t);
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDERCANCELLED_BITFIELDS_SIZE - numBitfields));
    mOrderCancelledBits = (OrderCancelledBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderCancelledBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderCancelledBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderCancelledBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderCancelledBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderCancelledBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderCancelledBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderCancelledBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderCancelledBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderCancelledBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderCancelledBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderCancelledBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderCancelledBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderCancelledBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderCancelledBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderCancelledBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderCancelledBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderCancelledBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderCancelledBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderCancelledBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderCancelledBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderCancelledBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getCancelRejected (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "CancelRejected");
    size_t offset = sizeof (BoeHeaderPacket);
    boeCancelRejectedPacket* packet = (boeCancelRejectedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (CancelRejectReason, packet->getCancelRejectReason ());
    offset += sizeof (uint8_t);
    
    d.setString (CancelRejectText, packet->getCancelRejectText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (CANCELREJECTED_BITFIELDS_SIZE - numBitfields));
    mCancelRejectedBits = (CancelRejectedBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mCancelRejectedBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mCancelRejectedBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mCancelRejectedBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mCancelRejectedBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mCancelRejectedBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mCancelRejectedBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mCancelRejectedBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mCancelRejectedBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mCancelRejectedBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mCancelRejectedBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mCancelRejectedBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mCancelRejectedBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mCancelRejectedBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mCancelRejectedBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mCancelRejectedBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mCancelRejectedBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mCancelRejectedBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mCancelRejectedBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mCancelRejectedBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mCancelRejectedBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mCancelRejectedBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderExecution (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderExecution");
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderExecutionPacket* packet = (boeOrderExecutionPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (ExecID, packet->getExecID ());
    offset += sizeof (uint64_t);
    
    d.setInteger (OrderExecLastShares, packet->getOrderExecLastShares ());
    offset += sizeof (uint32_t);
    
    d.setInteger (LastPx, packet->getLastPx ());
    offset += sizeof (uint64_t);
    
    d.setInteger (OrderExecLeavesQty, packet->getOrderExecLeavesQty ());
    offset += sizeof (uint32_t);
    
    d.setString (OrderExecBaseLiquidityIndicator, packet->getOrderExecBaseLiquidityIndicator ());
    offset += 1;
    
    d.setString (OrderExecSubLiquidityIndicator, packet->getOrderExecSubLiquidityIndicator ());
    offset += 1;
    
    d.setString (ContraBroker, packet->getContraBroker ());
    offset += 4;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (ORDEREXECUTION_BITFIELDS_SIZE - numBitfields));
    mOrderExecutionBits = (OrderExecutionBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mOrderExecutionBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mOrderExecutionBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mOrderExecutionBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderExecutionBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderExecutionBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mOrderExecutionBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mOrderExecutionBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderExecutionBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mOrderExecutionBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderExecutionBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderExecutionBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mOrderExecutionBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mOrderExecutionBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderExecutionBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mOrderExecutionBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderExecutionBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mOrderExecutionBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mOrderExecutionBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mOrderExecutionBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mOrderExecutionBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mOrderExecutionBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCancelCorrect (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCancelCorrect");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCancelCorrectPacket* packet = (boeTradeCancelCorrectPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (ClOrdID, packet->getClOrdID ());
    offset += 20;
    
    d.setInteger (OrderID, packet->getOrderID ());
    offset += sizeof (uint64_t);
    
    d.setInteger (ExecRefID, packet->getExecRefID ());
    offset += sizeof (uint64_t);
    
    d.setInteger (TradeCancelCorrectSide, packet->getTradeCancelCorrectSide ());
    offset += sizeof (uint8_t);
    
    d.setString (TradeCancelCorrectBaseLiquidityIndicator, packet->getTradeCancelCorrectBaseLiquidityIndicator ());
    offset += 1;
    
    d.setString (TradeCancelCorrectClearingFirm, packet->getTradeCancelCorrectClearingFirm ());
    offset += 4;
    
    d.setString (TradeCancelCorrectClearingAccount, packet->getTradeCancelCorrectClearingAccount ());
    offset += 4;
    
    d.setInteger (TradeCancelCorrectLastShares, packet->getTradeCancelCorrectLastShares ());
    offset += sizeof (uint32_t);
    
    d.setInteger (LastPx, packet->getLastPx ());
    offset += sizeof (uint64_t);
    
    d.setInteger (CorrectedPrice, packet->getCorrectedPrice ());
    offset += sizeof (uint64_t);
    
    d.setString (OrigTime, packet->getOrigTime ());
    offset += 8;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECANCELCORRECT_BITFIELDS_SIZE - numBitfields));
    mTradeCancelCorrectBits = (TradeCancelCorrectBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCancelCorrectBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mTradeCancelCorrectBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCancelCorrectBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCancelCorrectBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCancelCorrectBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCancelCorrectBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCancelCorrectBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCancelCorrectBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCancelCorrectBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCancelCorrectBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCancelCorrectBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCancelCorrectBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCancelCorrectBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCancelCorrectBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCancelCorrectBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCancelCorrectBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCancelCorrectBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCancelCorrectBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCancelCorrectBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCancelCorrectBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCancelCorrectBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCaptureReportAcknowledgement (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCaptureReportAcknowledgement");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportAcknowledgementPacket* packet = (boeTradeCaptureReportAcknowledgementPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (TradeReportID, packet->getTradeReportID ());
    offset += 20;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECAPTUREREPORTACKNOWLEDGEMENT_BITFIELDS_SIZE - numBitfields));
    mTradeCaptureReportAcknowledgementBits = (TradeCaptureReportAcknowledgementBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCaptureReportAcknowledgementBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mTradeCaptureReportAcknowledgementBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportAcknowledgementBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    d.setInteger (NoSides, *(uint8_t*)(newBuf + offset) );
    offset += sizeof (uint8_t);
    
    
    int numInGroup = 0;
    d.getInteger (NoSides, numInGroup) ;
    cdrArray SideGroupArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeSideGroupPacket* packet128 = (BoeSideGroupPacket*)((char*)buf + offset);

        item.setInteger (Side, packet128->getSide ());
        offset += sizeof (uint8_t);

        item.setInteger (Capacity, packet128->getCapacity ());
        offset += sizeof (uint8_t);

        item.setString (PartyID, packet128->getPartyID ());
        offset += 4;

        item.setString (Account, packet128->getAccount ());
        offset += 16;

        item.setInteger (PartyRole, packet128->getPartyRole ());
        offset += sizeof (uint8_t);

        SideGroupArray.push_back (item);
    }
    d.setArray (SideGroupSection, SideGroupArray);

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCaptureReportReject (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCaptureReportReject");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportRejectPacket* packet = (boeTradeCaptureReportRejectPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (TradeReportID, packet->getTradeReportID ());
    offset += 20;
    
    d.setInteger (TradeCaptureRejectReason, packet->getTradeCaptureRejectReason ());
    offset += sizeof (uint8_t);
    
    d.setString (TradeCaptureRejectText, packet->getTradeCaptureRejectText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECAPTUREREPORTREJECT_BITFIELDS_SIZE - numBitfields));
    mTradeCaptureReportRejectBits = (TradeCaptureReportRejectBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCaptureReportRejectBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mTradeCaptureReportRejectBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCaptureReportRejectBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportRejectBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportRejectBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportRejectBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportRejectBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportRejectBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportRejectBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportRejectBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportRejectBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportRejectBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportRejectBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportRejectBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportRejectBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportRejectBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportRejectBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportRejectBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    d.setInteger (NoSides, *(uint8_t*)(newBuf + offset) );
    offset += sizeof (uint8_t);
    
    
    int numInGroup = 0;
    d.getInteger (NoSides, numInGroup) ;
    cdrArray SideGroupArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeSideGroupPacket* packet129 = (BoeSideGroupPacket*)((char*)buf + offset);

        item.setInteger (Side, packet129->getSide ());
        offset += sizeof (uint8_t);

        item.setInteger (Capacity, packet129->getCapacity ());
        offset += sizeof (uint8_t);

        item.setString (PartyID, packet129->getPartyID ());
        offset += 4;

        item.setString (Account, packet129->getAccount ());
        offset += 16;

        item.setInteger (PartyRole, packet129->getPartyRole ());
        offset += sizeof (uint8_t);

        SideGroupArray.push_back (item);
    }
    d.setArray (SideGroupSection, SideGroupArray);

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCaptureConfirm (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCaptureConfirm");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureConfirmPacket* packet = (boeTradeCaptureConfirmPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (TradeReportID, packet->getTradeReportID ());
    offset += 20;
    
    d.setString (TradeCaptureTradeReportRefID, packet->getTradeCaptureTradeReportRefID ());
    offset += 20;
    
    d.setString (TradeID, packet->getTradeID ());
    offset += 8;
    
    d.setInteger (TradeCaptureLastShares, packet->getTradeCaptureLastShares ());
    offset += sizeof (uint32_t);
    
    d.setInteger (LastPx, packet->getLastPx ());
    offset += sizeof (uint64_t);
    
    d.setString (ContraBroker, packet->getContraBroker ());
    offset += 4;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECAPTURECONFIRM_BITFIELDS_SIZE - numBitfields));
    mTradeCaptureConfirmBits = (TradeCaptureConfirmBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCaptureConfirmBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mTradeCaptureConfirmBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCaptureConfirmBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureConfirmBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureConfirmBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureConfirmBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureConfirmBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureConfirmBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCaptureConfirmBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureConfirmBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureConfirmBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureConfirmBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureConfirmBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureConfirmBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureConfirmBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureConfirmBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureConfirmBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureConfirmBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureConfirmBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureConfirmBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureConfirmBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    d.setInteger (NoSides, *(uint8_t*)(newBuf + offset) );
    offset += sizeof (uint8_t);
    
    
    int numInGroup = 0;
    d.getInteger (NoSides, numInGroup) ;
    cdrArray SideGroupArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeSideGroupPacket* packet132 = (BoeSideGroupPacket*)((char*)buf + offset);

        item.setInteger (Side, packet132->getSide ());
        offset += sizeof (uint8_t);

        item.setInteger (Capacity, packet132->getCapacity ());
        offset += sizeof (uint8_t);

        item.setString (PartyID, packet132->getPartyID ());
        offset += 4;

        item.setString (Account, packet132->getAccount ());
        offset += 16;

        item.setInteger (PartyRole, packet132->getPartyRole ());
        offset += sizeof (uint8_t);

        SideGroupArray.push_back (item);
    }
    d.setArray (SideGroupSection, SideGroupArray);

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getTradeCaptureReportDecline (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "TradeCaptureReportDecline");
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportDeclinePacket* packet = (boeTradeCaptureReportDeclinePacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setString (TradeReportID, packet->getTradeReportID ());
    offset += 20;
    
    d.setString (TradeCaptureDeclineTradeReportRefID, packet->getTradeCaptureDeclineTradeReportRefID ());
    offset += 20;
    
    d.setString (TradeID, packet->getTradeID ());
    offset += 8;
    
    d.setInteger (TradeCaptureDeclineLastShares, packet->getTradeCaptureDeclineLastShares ());
    offset += sizeof (uint32_t);
    
    d.setInteger (LastPx, packet->getLastPx ());
    offset += sizeof (uint64_t);
    
    d.setString (ContraBroker, packet->getContraBroker ());
    offset += 4;
    
    d.setInteger (TradeCaptureDeclineReason, packet->getTradeCaptureDeclineReason ());
    offset += sizeof (uint8_t);
    
    d.setString (TradeCaptureDeclineText, packet->getTradeCaptureDeclineText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (TRADECAPTUREREPORTDECLINE_BITFIELDS_SIZE - numBitfields));
    mTradeCaptureReportDeclineBits = (TradeCaptureReportDeclineBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mTradeCaptureReportDeclineBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mTradeCaptureReportDeclineBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasPegDifference)
    {
        d.setInteger (PegDifference, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasPrice)
    {
        d.setInteger (Price, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasExecInst)
    {
        d.setString (ExecInst, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrdType)
    {
        d.setString (OrdType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTimeInForce)
    {
        d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasMinQty)
    {
        d.setInteger (MinQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }

    
    if (mTradeCaptureReportDeclineBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasCurrency)
    {
        d.setString (Currency, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportDeclineBits->hasIDSource)
    {
        d.setString (IDSource, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasSecurityID)
    {
        d.setString (SecurityID, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportDeclineBits->hasSecurityExchange)
    {
        d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasCapacity)
    {
        d.setString (Capacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasAccount)
    {
        d.setString (Account, getStringField (newBuf, 16, offset) );
        offset += 16;
    }

    
    if (mTradeCaptureReportDeclineBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasClearingAccount)
    {
        d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasDisplayIndicator)
    {
        d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasMaxFloor)
    {
        d.setInteger (MaxFloor, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrderQty)
    {
        d.setInteger (OrderQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }

    
    if (mTradeCaptureReportDeclineBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportDeclineBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLastShares)
    {
        d.setInteger (LastShares, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasCcp)
    {
        d.setString (Ccp, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasBulkRejectReasons)
    {
        d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasPartyRole)
    {
        d.setString (PartyRole, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasSubLiquidityIndicator)
    {
        d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradeReportTypeReturn)
    {
        d.setInteger (TradeReportTypeReturn, *(uint16_t*)(newBuf + offset) );
        offset += sizeof (uint16_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportDeclineBits->hasBidPx)
    {
        d.setInteger (BidPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLargeSize)
    {
        d.setInteger (LargeSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLastMkt)
    {
        d.setString (LastMkt, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasFeeCode)
    {
        d.setString (FeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportDeclineBits->hasEchoText)
    {
        d.setString (EchoText, getStringField (newBuf, 60, offset) );
        offset += 60;
    }

    
    if (mTradeCaptureReportDeclineBits->hasStopPx)
    {
        d.setInteger (StopPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasRoutingInst)
    {
        d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasRoutStrategy)
    {
        d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasExDestination)
    {
        d.setString (ExDestination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradeReportRefID)
    {
        d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }

    
    if (mTradeCaptureReportDeclineBits->hasMarketingFeeCode)
    {
        d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTargetPartyID)
    {
        d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasAuctionID)
    {
        d.setInteger (AuctionID, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasCrossType)
    {
        d.setString (CrossType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasCrossPrioritization)
    {
        d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasAllocQty)
    {
        d.setInteger (AllocQty, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasRoutingFirmID)
    {
        d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasWaiverType)
    {
        d.setString (WaiverType, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasCrossExclusionIndicator)
    {
        d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasPriceFormation)
    {
        d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasClientQualifiedRole)
    {
        d.setInteger (ClientQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasClientID)
    {
        d.setInteger (ClientID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasInvestorID)
    {
        d.setInteger (InvestorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrderOrigination)
    {
        d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasAlgorithmicIndicator)
    {
        d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasDeferralReason)
    {
        d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasInvestorQualifiedRole)
    {
        d.setInteger (InvestorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportDeclineBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradeDate)
    {
        d.setInteger (TradeDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasCumQty)
    {
        d.setInteger (CumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasAvgPx)
    {
        d.setInteger (AvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }

    
    if (mTradeCaptureReportDeclineBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, *(uint64_t*)(newBuf + offset) );
        offset += sizeof (uint64_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, *(uint32_t*)(newBuf + offset) );
        offset += sizeof (uint32_t);
    }

    
    if (mTradeCaptureReportDeclineBits->hasUsername)
    {
        d.setString (Username, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradeReportingIndicator)
    {
        d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasTradePublishIndicator)
    {
        d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }

    
    if (mTradeCaptureReportDeclineBits->hasReportTime)
    {
        d.setInteger (ReportTime, *(uint8_t*)(newBuf + offset) );
        offset += sizeof (uint8_t);
    }

    
    d.setInteger (NoSides, *(uint8_t*)(newBuf + offset) );
    offset += sizeof (uint8_t);
    
    
    int numInGroup = 0;
    d.getInteger (NoSides, numInGroup) ;
    cdrArray SideGroupArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeSideGroupPacket* packet134 = (BoeSideGroupPacket*)((char*)buf + offset);

        item.setInteger (Side, packet134->getSide ());
        offset += sizeof (uint8_t);

        item.setInteger (Capacity, packet134->getCapacity ());
        offset += sizeof (uint8_t);

        item.setString (PartyID, packet134->getPartyID ());
        offset += 4;

        item.setString (Account, packet134->getAccount ());
        offset += 16;

        item.setInteger (PartyRole, packet134->getPartyRole ());
        offset += sizeof (uint8_t);

        SideGroupArray.push_back (item);
    }
    d.setArray (SideGroupSection, SideGroupArray);

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getPurgeOrder (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "PurgeOrder");
    size_t offset = sizeof (BoeHeaderPacket);
    boePurgeOrderPacket* packet = (boePurgeOrderPacket*)((char*)buf + offset);
    
    d.setString (MassCancel, packet->getMassCancel ());
    offset += 1;
    
    int numBitfields = packet->getNumberOfBitfields ();
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    offset += sizeof (uint8_t);
    
    //copying bitfields to new temporary buffer
    char* tmpbuf = new char[numBitfields];
    memcpy ((void*)tmpbuf, (void*)buf+offset, numBitfields);

    cdrArray BitfieldArray;
    for (int i = 0; i < numBitfields; i++)
    {
        cdr item;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);
        item.setInteger (Bitfield, bitfieldElement->getBitfield ());
        BitfieldArray.push_back(item);
        offset += sizeof (BoeBitfieldPacket);
    }
    //setting the rest of the remaining bitfields to zero
    memset (tmpbuf+numBitfields, 0, (PURGEORDER_BITFIELDS_SIZE - numBitfields));
    mPurgeOrderBits = (PurgeOrderBits*) tmpbuf;

    d.setArray(BitfieldSection, BitfieldArray);
    if (mPurgeOrderBits == NULL)
    {
       if (numBitfields !=0)
       {
           setLastError("Bitfields not correctly set");
           return GW_CODEC_ERROR;
       }
       else
       {
           used += offset;
           return GW_CODEC_SUCCESS;
       }
    }
    //Get optional fields
    char* newBuf = (char*) buf;

    
    if (mPurgeOrderBits->hasClearingFirm)
    {
        d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
        offset += 4;
    }

    
    d.setInteger (NumberOfCustomGroupIDs, packet->getNumberOfCustomGroupIDs ());
    offset += sizeof (uint8_t);
    
    int numInGroup = 0;
    d.getInteger (NumberOfCustomGroupIDs, numInGroup) ;
    cdrArray CustomGroupIDArray;
    for (int i = 0; i < numInGroup; i++)
    {
        cdr item;
        BoeCustomGroupIDPacket* packet13 = (BoeCustomGroupIDPacket*)((char*)buf + offset);

        item.setInteger (CustomGroupID, packet13->getCustomGroupID ());
        offset += sizeof (uint16_t);

        CustomGroupIDArray.push_back (item);
    }
    d.setArray (CustomGroupIDSection, CustomGroupIDArray);

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getPurgeRejected (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "PurgeRejected");
    size_t offset = sizeof (BoeHeaderPacket);
    boePurgeRejectedPacket* packet = (boePurgeRejectedPacket*)((char*)buf + offset);
    
    d.setInteger (TransactTime, packet->getTransactTime ());
    offset += sizeof (uint64_t);
    
    d.setInteger (PurgeRejectReason, packet->getPurgeRejectReason ());
    offset += sizeof (uint8_t);
    
    d.setString (PurgeRejectText, packet->getPurgeRejectText ());
    offset += 60;
    
    d.setString (ReservedInternal, packet->getReservedInternal ());
    offset += 1;
    used += offset;
    return GW_CODEC_SUCCESS;
}


codecState
boeCodec::putLoginRequest (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeLoginRequestPacket* packet = (boeLoginRequestPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeLoginRequestPacket))
        return GW_CODEC_SHORT;

    
    string tradingsessionsubid;
    if (!d.getString (TradingSessionSubID, tradingsessionsubid))
    {
        setLastError ("TradingSessionSubID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradingSessionSubID (tradingsessionsubid);
    offset += 4;
    
    string username;
    if (!d.getString (Username, username))
    {
        setLastError ("Username missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setUsername (username);
    offset += 4;
    
    string password;
    if (!d.getString (Password, password))
    {
        setLastError ("Password missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setPassword (password);
    offset += 10;
    
    uint8_t numberofparamgroups;
    if (!d.getInteger (NumberOfParamGroups, numberofparamgroups))
    {
        setLastError ("NumberOfParamGroups missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfParamGroups (numberofparamgroups);
    offset += sizeof (uint8_t);
    int NumParamGroups = numberofparamgroups;

    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

        uint16_t paramGroupLength;
        if (!item.getInteger (ParamGroupLength, paramGroupLength))
        {
            setLastError ("ParamGroupLength missing or not integer");
            return GW_CODEC_ERROR;
        }
        paramGroupsElement->setParamGroupLength (paramGroupLength);
        offset += sizeof (uint16_t);
        
        uint8_t paramGroupType;
        if (!item.getInteger (ParamGroupType, paramGroupType))
        {
            setLastError ("ParamGroupType missing or not integer");
            return GW_CODEC_ERROR;
        }
        paramGroupsElement->setParamGroupType (paramGroupType);
        offset += sizeof (uint8_t);
                
        if(paramGroupType == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            cdrArray UnitSequenceParamGroupArray;
            
            uint8_t noUnspecifiedUnitReplay;
            if (!item.getInteger (NoUnspecifiedUnitReplay, noUnspecifiedUnitReplay))
            {
                setLastError ("NoUnspecifiedUnitReplay missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNoUnspecifiedUnitReplay (noUnspecifiedUnitReplay);
            offset += sizeof (uint8_t);
            
            uint8_t numOfUnits;
            if (!item.getInteger (NumOfUnits, numOfUnits))
            {
                setLastError ("NumOfUnits missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNumOfUnits (numOfUnits);
            offset += sizeof (uint8_t);
            
            cdrArray UnitGroupArray;
            for (int i = 0; i < numOfUnits; i++)
            {
                cdr unitGroupItem;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                uint8_t unitNumber;
                if (!unitGroupItem.getInteger (UnitNumber, unitNumber))
                {
                    setLastError ("UnitNumber missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitGroupElement->setUnitNumber (unitNumber);
                offset += sizeof (uint8_t);
            
                uint64_t unitSequence;
                if (!unitGroupItem.getInteger (UnitSequence, unitSequence))
                {
                    setLastError ("UnitSequence missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitGroupElement->setUnitSequence (unitSequence);
                offset += sizeof (uint64_t);
            }
        }
        else if(paramGroupType == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);
            
            cdrArray ReturnBitfieldArray;
            
            uint8_t messageType;
            if (!item.getInteger (MessageType, messageType))
            {
                setLastError ("MessageType missing or not integer");
                return GW_CODEC_ERROR;
            }
            returnBitfieldElement->setMessageType (messageType);
            offset += sizeof (uint8_t);
            
            uint8_t numReturnBitfields;
            if (!item.getInteger (NumOfReturnBitfield, numReturnBitfields))
            {
                setLastError ("NumReturnBitfields missing or not integer");
                return GW_CODEC_ERROR;
            }
            returnBitfieldElement->setNumOfReturnBitfield (numReturnBitfields);
            offset += sizeof (uint8_t);
            
            for (int i = 0; i < numReturnBitfields; i++)
            {
                uint8_t returnBitfield;
                if (!item.getInteger (ReturnBitfield, returnBitfield))
                {
                    setLastError ("ReturnBitfield missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setReturnBitfield (returnBitfield);
                offset += sizeof (uint8_t);
            }
            //TODO - set Optional fields
        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putLoginResponse (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeLoginResponsePacket* packet = (boeLoginResponsePacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeLoginResponsePacket))
        return GW_CODEC_SHORT;

    
    uint8_t loginresponsestatus;
    if (!d.getInteger (LoginResponseStatus, loginresponsestatus))
    {
        setLastError ("LoginResponseStatus missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLoginResponseStatus (loginresponsestatus);
    offset += sizeof (uint8_t);
    
    string loginresponsetext;
    if (!d.getString (LoginResponseText, loginresponsetext))
    {
        setLastError ("LoginResponseText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setLoginResponseText (loginresponsetext);
    offset += 60;
    
    uint8_t nounspecifiedunitreplay;
    if (!d.getInteger (NoUnspecifiedUnitReplay, nounspecifiedunitreplay))
    {
        setLastError ("NoUnspecifiedUnitReplay missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoUnspecifiedUnitReplay (nounspecifiedunitreplay);
    offset += sizeof (uint8_t);
    
    uint32_t lastreceivedsequencenumber;
    if (!d.getInteger (LastReceivedSequenceNumber, lastreceivedsequencenumber))
    {
        setLastError ("LastReceivedSequenceNumber missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastReceivedSequenceNumber (lastreceivedsequencenumber);
    offset += sizeof (uint32_t);
    
    uint8_t numberofunits;
    if (!d.getInteger (NumberOfUnits, numberofunits))
    {
        setLastError ("NumberOfUnits missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfUnits (numberofunits);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (UnitGroupSection) > 0)
    {
        cdrArray* UnitGroupArray = NULL;
        d.getArray (UnitGroupSection, (const cdrArray**)(&UnitGroupArray));
        for (cdrArray::iterator it = UnitGroupArray->begin(); it != UnitGroupArray->end(); ++it)
        {
            cdr& item = *it;
            BoeUnitGroupPacket* unitgroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

            uint8_t unitnumber;
            if (!item.getInteger (UnitNumber, unitnumber))
            {
                setLastError ("UnitNumber missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitgroupElement->setUnitNumber (unitnumber);
            offset += sizeof (uint8_t);

            uint32_t unitsequence;
            if (!item.getInteger (UnitSequence, unitsequence))
            {
                setLastError ("UnitSequence missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitgroupElement->setUnitSequence (unitsequence);
            offset += sizeof (uint32_t);

        }
    }
    
    uint8_t numberofparamgroups;
    if (!d.getInteger (NumberOfParamGroups, numberofparamgroups))
    {
        setLastError ("NumberOfParamGroups missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfParamGroups (numberofparamgroups);
    offset += sizeof (uint8_t);
    
    int NumParamGroups = numberofparamgroups;

    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);

        uint16_t paramGroupLength;
        if (!item.getInteger (ParamGroupLength, paramGroupLength))
        {
            setLastError ("ParamGroupLength missing or not integer");
            return GW_CODEC_ERROR;
        }
        paramGroupsElement->setParamGroupLength (paramGroupLength);
        offset += sizeof (uint16_t);
        
        uint8_t paramGroupType;
        if (!item.getInteger (ParamGroupType, paramGroupType))
        {
            setLastError ("ParamGroupType missing or not integer");
            return GW_CODEC_ERROR;
        }
        paramGroupsElement->setParamGroupType (paramGroupType);
        offset += sizeof (uint8_t);
                
        if(paramGroupType == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            cdrArray UnitSequenceParamGroupArray;
            
            uint8_t noUnspecifiedUnitReplay;
            if (!item.getInteger (NoUnspecifiedUnitReplay, noUnspecifiedUnitReplay))
            {
                setLastError ("NoUnspecifiedUnitReplay missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNoUnspecifiedUnitReplay (noUnspecifiedUnitReplay);
            offset += sizeof (uint8_t);
            
            uint8_t numOfUnits;
            if (!item.getInteger (NumOfUnits, numOfUnits))
            {
                setLastError ("NumOfUnits missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNumOfUnits (numOfUnits);
            offset += sizeof (uint8_t);
            
            cdrArray UnitGroupArray;
            for (int i = 0; i < numOfUnits; i++)
            {
                cdr unitGroupItem;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ((char*)buf + offset);

                uint8_t unitNumber;
                if (!unitGroupItem.getInteger (UnitNumber, unitNumber))
                {
                    setLastError ("UnitNumber missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitGroupElement->setUnitNumber (unitNumber);
                offset += sizeof (uint8_t);
            
                uint64_t unitSequence;
                if (!unitGroupItem.getInteger (UnitSequence, unitSequence))
                {
                    setLastError ("UnitSequence missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitGroupElement->setUnitSequence (unitSequence);
                offset += sizeof (uint64_t);
            }
        }
        else if(paramGroupType == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);
            
            cdrArray ReturnBitfieldArray;
            
            uint8_t messageType;
            if (!item.getInteger (MessageType, messageType))
            {
                setLastError ("MessageType missing or not integer");
                return GW_CODEC_ERROR;
            }
            returnBitfieldElement->setMessageType (messageType);
            offset += sizeof (uint8_t);
            
            uint8_t numReturnBitfields;
            if (!item.getInteger (NumOfReturnBitfield, numReturnBitfields))
            {
                setLastError ("NumReturnBitfields missing or not integer");
                return GW_CODEC_ERROR;
            }
            returnBitfieldElement->setNumOfReturnBitfield (numReturnBitfields);
            offset += sizeof (uint8_t);
            
            for (int i = 0; i < numReturnBitfields; i++)
            {
                uint8_t returnBitfield;
                if (!item.getInteger (ReturnBitfield, returnBitfield))
                {
                    setLastError ("ReturnBitfield missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setReturnBitfield (returnBitfield);
                offset += sizeof (uint8_t);
            }
            //TODO - set Optional fields
        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putLogoutResponse (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeLogoutResponsePacket* packet = (boeLogoutResponsePacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeLogoutResponsePacket))
        return GW_CODEC_SHORT;

    
    uint8_t logoutreason;
    if (!d.getInteger (LogoutReason, logoutreason))
    {
        setLastError ("LogoutReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLogoutReason (logoutreason);
    offset += sizeof (uint8_t);
    
    string logoutreasontext;
    if (!d.getString (LogoutReasonText, logoutreasontext))
    {
        setLastError ("LogoutReasonText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setLogoutReasonText (logoutreasontext);
    offset += 60;
    
    uint32_t lastreceivedsequencenumber;
    if (!d.getInteger (LastReceivedSequenceNumber, lastreceivedsequencenumber))
    {
        setLastError ("LastReceivedSequenceNumber missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastReceivedSequenceNumber (lastreceivedsequencenumber);
    offset += sizeof (uint32_t);
    
    uint8_t numberofunits;
    if (!d.getInteger (NumberOfUnits, numberofunits))
    {
        setLastError ("NumberOfUnits missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfUnits (numberofunits);
    offset += sizeof (uint8_t);
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putServerHeartbeat (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeServerHeartbeatPacket))
        return GW_CODEC_SHORT;

    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putClientHeartbeat (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeClientHeartbeatPacket))
        return GW_CODEC_SHORT;

    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putReplayComplete (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeReplayCompletePacket))
        return GW_CODEC_SHORT;

    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putNewOrder (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeNewOrderPacket* packet = (boeNewOrderPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeNewOrderPacket))
        return GW_CODEC_SHORT;

    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint8_t side;
    if (!d.getInteger (Side, side))
    {
        setLastError ("Side missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setSide (side);
    offset += sizeof (uint8_t);
    
    uint32_t orderqty;
    if (!d.getInteger (OrderQty, orderqty))
    {
        setLastError ("OrderQty missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderQty (orderqty);
    offset += sizeof (uint32_t);
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
            {
                setLastError ("Bitfield missing or not integer");
                return GW_CODEC_ERROR;
            }
            bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);

        }
    }
    
    if (mNewOrderBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mNewOrderBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mNewOrderBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mNewOrderBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mNewOrderBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mNewOrderBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mNewOrderBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mNewOrderBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mNewOrderBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mNewOrderBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mNewOrderBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mNewOrderBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mNewOrderBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mNewOrderBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mNewOrderBits->hasExpireTime)
    {
       uint64_t expiretime;
       if (!d.getInteger (ExpireTime, expiretime))
       {
           setLastError ("ExpireTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &expiretime, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mNewOrderBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mNewOrderBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mNewOrderBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mNewOrderBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mNewOrderBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mNewOrderBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mNewOrderBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mNewOrderBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putCancelOrder (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeCancelOrderPacket* packet = (boeCancelOrderPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeCancelOrderPacket))
        return GW_CODEC_SHORT;

    
    string origclordid;
    if (!d.getString (OrigClOrdID, origclordid))
    {
        setLastError ("OrigClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrigClOrdID (origclordid);
    offset += 20;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mCancelOrderBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putModifyOrder (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeModifyOrderPacket* packet = (boeModifyOrderPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeModifyOrderPacket))
        return GW_CODEC_SHORT;

    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    string origclordid;
    if (!d.getString (OrigClOrdID, origclordid))
    {
        setLastError ("OrigClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrigClOrdID (origclordid);
    offset += 20;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mModifyOrderBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mModifyOrderBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mModifyOrderBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mModifyOrderBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mModifyOrderBits->hasCancelOrigOnReject)
    {
       string cancelorigonreject;
       if (!d.getString (CancelOrigOnReject, cancelorigonreject))
       {
           setLastError ("CancelOrigOnReject missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cancelorigonreject.c_str(), 1);
       offset += 1;
    }
    
    
    if (mModifyOrderBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCaptureReport (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportPacket* packet = (boeTradeCaptureReportPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCaptureReportPacket))
        return GW_CODEC_SHORT;

    
    string tradereportid;
    if (!d.getString (TradeReportID, tradereportid))
    {
        setLastError ("TradeReportID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeReportID (tradereportid);
    offset += 20;
    
    uint32_t lastshares;
    if (!d.getInteger (LastShares, lastshares))
    {
        setLastError ("LastShares missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastShares (lastshares);
    offset += sizeof (uint32_t);
    
    uint64_t lastpx;
    if (!d.getInteger (LastPx, lastpx))
    {
        setLastError ("LastPx missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx (lastpx);
    offset += sizeof (uint64_t);
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    uint8_t nosides;
    if (!d.getInteger (NoSides, nosides))
    {
        setLastError ("NoSides missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoSides (nosides);
    offset += sizeof (uint8_t);
    
    if (mTradeCaptureReportBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCaptureReportBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportBits->hasTransactionCategory)
    {
       string transactioncategory;
       if (!d.getString (TransactionCategory, transactioncategory))
       {
           setLastError ("TransactionCategory missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, transactioncategory.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasTradeTime)
    {
       uint64_t tradetime;
       if (!d.getInteger (TradeTime, tradetime))
       {
           setLastError ("TradeTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradetime, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasTradeReportTransType)
    {
       uint8_t tradereporttranstype;
       if (!d.getInteger (TradeReportTransType, tradereporttranstype))
       {
           setLastError ("TradeReportTransType missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttranstype, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTradeID)
    {
       uint64_t tradeid;
       if (!d.getInteger (TradeID, tradeid))
       {
           setLastError ("TradeID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradeid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasVenueType)
    {
       string venuetype;
       if (!d.getString (VenueType, venuetype))
       {
           setLastError ("VenueType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, venuetype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasTradingSessionSubID)
    {
       uint8_t tradingsessionsubid;
       if (!d.getInteger (TradingSessionSubID, tradingsessionsubid))
       {
           setLastError ("TradingSessionSubID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradingsessionsubid, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasMatchType)
    {
       uint8_t matchtype;
       if (!d.getInteger (MatchType, matchtype))
       {
           setLastError ("MatchType missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &matchtype, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTrdSubType)
    {
       uint8_t trdsubtype;
       if (!d.getInteger (TrdSubType, trdsubtype))
       {
           setLastError ("TrdSubType missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &trdsubtype, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasSecondaryTrdType)
    {
       uint8_t secondarytrdtype;
       if (!d.getInteger (SecondaryTrdType, secondarytrdtype))
       {
           setLastError ("SecondaryTrdType missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondarytrdtype, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTradePriceCondition)
    {
       uint8_t tradepricecondition;
       if (!d.getInteger (TradePriceCondition, tradepricecondition))
       {
           setLastError ("TradePriceCondition missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepricecondition, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasExecutionMethod)
    {
       string executionmethod;
       if (!d.getString (ExecutionMethod, executionmethod))
       {
           setLastError ("ExecutionMethod missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, executionmethod.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasTradeReportType)
    {
       uint8_t tradereporttype;
       if (!d.getInteger (TradeReportType, tradereporttype))
       {
           setLastError ("TradeReportType missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttype, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTradeHandlingInstruction)
    {
       uint8_t tradehandlinginstruction;
       if (!d.getInteger (TradeHandlingInstruction, tradehandlinginstruction))
       {
           setLastError ("TradeHandlingInstruction missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradehandlinginstruction, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTradeLinkID)
    {
       string tradelinkid;
       if (!d.getString (TradeLinkID, tradelinkid))
       {
           setLastError ("TradeLinkID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradelinkid.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportBits->hasGrossTradeAmt)
    {
       uint64_t grosstradeamt;
       if (!d.getInteger (GrossTradeAmt, grosstradeamt))
       {
           setLastError ("GrossTradeAmt missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &grosstradeamt, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasTolerance)
    {
       uint16_t tolerance;
       if (!d.getInteger (Tolerance, tolerance))
       {
           setLastError ("Tolerance missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tolerance, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCaptureReportBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportBits->hasSettlementPrice)
    {
       uint64_t settlementprice;
       if (!d.getInteger (SettlementPrice, settlementprice))
       {
           setLastError ("SettlementPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &settlementprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasSettlementDate)
    {
       uint64_t settlementdate;
       if (!d.getInteger (SettlementDate, settlementdate))
       {
           setLastError ("SettlementDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &settlementdate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportBits->hasSettlementCurrency)
    {
       string settlementcurrency;
       if (!d.getString (SettlementCurrency, settlementcurrency))
       {
           setLastError ("SettlementCurrency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, settlementcurrency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportBits->hasSettlementLocation)
    {
       string settlementlocation;
       if (!d.getString (SettlementLocation, settlementlocation))
       {
           setLastError ("SettlementLocation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, settlementlocation.c_str(), 2);
       offset += 2;
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderAcknowledgement (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderAcknowledgementPacket* packet = (boeOrderAcknowledgementPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderAcknowledgementPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint64_t orderid;
    if (!d.getInteger (OrderID, orderid))
    {
        setLastError ("OrderID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderID (orderid);
    offset += sizeof (uint64_t);
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderAcknowledgementBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderAcknowledgementBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderAcknowledgementBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderAcknowledgementBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderAcknowledgementBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderAcknowledgementBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderAcknowledgementBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderAcknowledgementBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderAcknowledgementBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderAcknowledgementBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderAcknowledgementBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderAcknowledgementBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderAcknowledgementBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderAcknowledgementBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderAcknowledgementBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderAcknowledgementBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderAcknowledgementBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderRejected (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderRejectedPacket* packet = (boeOrderRejectedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderRejectedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    string orderrejectreason;
    if (!d.getString (OrderRejectReason, orderrejectreason))
    {
        setLastError ("OrderRejectReason missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrderRejectReason (orderrejectreason);
    offset += 1;
    
    string orderrejecttext;
    if (!d.getString (OrderRejectText, orderrejecttext))
    {
        setLastError ("OrderRejectText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrderRejectText (orderrejecttext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderRejectedBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderRejectedBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderRejectedBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderRejectedBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderRejectedBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderRejectedBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderRejectedBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderRejectedBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderRejectedBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRejectedBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderRejectedBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderRejectedBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRejectedBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRejectedBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRejectedBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRejectedBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRejectedBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRejectedBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRejectedBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRejectedBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderModified (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderModifiedPacket* packet = (boeOrderModifiedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderModifiedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint64_t orderid;
    if (!d.getInteger (OrderID, orderid))
    {
        setLastError ("OrderID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderID (orderid);
    offset += sizeof (uint64_t);
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderModifiedBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderModifiedBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderModifiedBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderModifiedBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderModifiedBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderModifiedBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderModifiedBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderModifiedBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderModifiedBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderModifiedBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderModifiedBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderModifiedBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderModifiedBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderModifiedBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderModifiedBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderModifiedBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderModifiedBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderModifiedBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderModifiedBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderModifiedBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderRestated (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderRestatedPacket* packet = (boeOrderRestatedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderRestatedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint64_t orderid;
    if (!d.getInteger (OrderID, orderid))
    {
        setLastError ("OrderID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderID (orderid);
    offset += sizeof (uint64_t);
    
    uint8_t restatementreason;
    if (!d.getInteger (RestatementReason, restatementreason))
    {
        setLastError ("RestatementReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setRestatementReason (restatementreason);
    offset += sizeof (uint8_t);
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderRestatedBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderRestatedBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderRestatedBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderRestatedBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderRestatedBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderRestatedBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderRestatedBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderRestatedBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderRestatedBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRestatedBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderRestatedBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderRestatedBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRestatedBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRestatedBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderRestatedBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderRestatedBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderRestatedBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderRestatedBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderRestatedBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderRestatedBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putUserModifyRejected (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeUserModifyRejectedPacket* packet = (boeUserModifyRejectedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeUserModifyRejectedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint8_t modifyrejectreason;
    if (!d.getInteger (ModifyRejectReason, modifyrejectreason))
    {
        setLastError ("ModifyRejectReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setModifyRejectReason (modifyrejectreason);
    offset += sizeof (uint8_t);
    
    string modifyrejecttext;
    if (!d.getString (ModifyRejectText, modifyrejecttext))
    {
        setLastError ("ModifyRejectText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setModifyRejectText (modifyrejecttext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mUserModifyRejectedBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mUserModifyRejectedBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mUserModifyRejectedBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mUserModifyRejectedBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mUserModifyRejectedBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mUserModifyRejectedBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mUserModifyRejectedBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mUserModifyRejectedBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mUserModifyRejectedBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mUserModifyRejectedBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mUserModifyRejectedBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mUserModifyRejectedBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mUserModifyRejectedBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mUserModifyRejectedBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mUserModifyRejectedBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mUserModifyRejectedBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mUserModifyRejectedBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mUserModifyRejectedBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mUserModifyRejectedBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mUserModifyRejectedBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderCancelled (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderCancelledPacket* packet = (boeOrderCancelledPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderCancelledPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint8_t cancelreason;
    if (!d.getInteger (CancelReason, cancelreason))
    {
        setLastError ("CancelReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setCancelReason (cancelreason);
    offset += sizeof (uint8_t);
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderCancelledBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderCancelledBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderCancelledBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderCancelledBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderCancelledBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderCancelledBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderCancelledBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderCancelledBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderCancelledBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderCancelledBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderCancelledBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderCancelledBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderCancelledBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderCancelledBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderCancelledBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderCancelledBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderCancelledBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderCancelledBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderCancelledBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderCancelledBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putCancelRejected (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeCancelRejectedPacket* packet = (boeCancelRejectedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeCancelRejectedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint8_t cancelrejectreason;
    if (!d.getInteger (CancelRejectReason, cancelrejectreason))
    {
        setLastError ("CancelRejectReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setCancelRejectReason (cancelrejectreason);
    offset += sizeof (uint8_t);
    
    string cancelrejecttext;
    if (!d.getString (CancelRejectText, cancelrejecttext))
    {
        setLastError ("CancelRejectText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setCancelRejectText (cancelrejecttext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mCancelRejectedBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mCancelRejectedBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mCancelRejectedBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mCancelRejectedBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mCancelRejectedBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mCancelRejectedBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mCancelRejectedBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mCancelRejectedBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mCancelRejectedBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mCancelRejectedBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mCancelRejectedBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mCancelRejectedBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mCancelRejectedBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mCancelRejectedBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mCancelRejectedBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mCancelRejectedBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mCancelRejectedBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mCancelRejectedBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mCancelRejectedBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mCancelRejectedBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderExecution (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeOrderExecutionPacket* packet = (boeOrderExecutionPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeOrderExecutionPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint64_t execid;
    if (!d.getInteger (ExecID, execid))
    {
        setLastError ("ExecID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setExecID (execid);
    offset += sizeof (uint64_t);
    
    uint32_t orderexeclastshares;
    if (!d.getInteger (OrderExecLastShares, orderexeclastshares))
    {
        setLastError ("OrderExecLastShares missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderExecLastShares (orderexeclastshares);
    offset += sizeof (uint32_t);
    
    uint64_t lastpx;
    if (!d.getInteger (LastPx, lastpx))
    {
        setLastError ("LastPx missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx (lastpx);
    offset += sizeof (uint64_t);
    
    uint32_t orderexecleavesqty;
    if (!d.getInteger (OrderExecLeavesQty, orderexecleavesqty))
    {
        setLastError ("OrderExecLeavesQty missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderExecLeavesQty (orderexecleavesqty);
    offset += sizeof (uint32_t);
    
    string orderexecbaseliquidityindicator;
    if (!d.getString (OrderExecBaseLiquidityIndicator, orderexecbaseliquidityindicator))
    {
        setLastError ("OrderExecBaseLiquidityIndicator missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrderExecBaseLiquidityIndicator (orderexecbaseliquidityindicator);
    offset += 1;
    
    string orderexecsubliquidityindicator;
    if (!d.getString (OrderExecSubLiquidityIndicator, orderexecsubliquidityindicator))
    {
        setLastError ("OrderExecSubLiquidityIndicator missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrderExecSubLiquidityIndicator (orderexecsubliquidityindicator);
    offset += 1;
    
    string contrabroker;
    if (!d.getString (ContraBroker, contrabroker))
    {
        setLastError ("ContraBroker missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setContraBroker (contrabroker);
    offset += 4;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mOrderExecutionBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mOrderExecutionBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderExecutionBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderExecutionBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mOrderExecutionBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mOrderExecutionBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderExecutionBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mOrderExecutionBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderExecutionBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderExecutionBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mOrderExecutionBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mOrderExecutionBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderExecutionBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mOrderExecutionBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderExecutionBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mOrderExecutionBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mOrderExecutionBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mOrderExecutionBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mOrderExecutionBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mOrderExecutionBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCancelCorrect (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCancelCorrectPacket* packet = (boeTradeCancelCorrectPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCancelCorrectPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string clordid;
    if (!d.getString (ClOrdID, clordid))
    {
        setLastError ("ClOrdID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID (clordid);
    offset += 20;
    
    uint64_t orderid;
    if (!d.getInteger (OrderID, orderid))
    {
        setLastError ("OrderID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setOrderID (orderid);
    offset += sizeof (uint64_t);
    
    uint64_t execrefid;
    if (!d.getInteger (ExecRefID, execrefid))
    {
        setLastError ("ExecRefID missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setExecRefID (execrefid);
    offset += sizeof (uint64_t);
    
    uint8_t tradecancelcorrectside;
    if (!d.getInteger (TradeCancelCorrectSide, tradecancelcorrectside))
    {
        setLastError ("TradeCancelCorrectSide missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCancelCorrectSide (tradecancelcorrectside);
    offset += sizeof (uint8_t);
    
    string tradecancelcorrectbaseliquidityindicator;
    if (!d.getString (TradeCancelCorrectBaseLiquidityIndicator, tradecancelcorrectbaseliquidityindicator))
    {
        setLastError ("TradeCancelCorrectBaseLiquidityIndicator missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCancelCorrectBaseLiquidityIndicator (tradecancelcorrectbaseliquidityindicator);
    offset += 1;
    
    string tradecancelcorrectclearingfirm;
    if (!d.getString (TradeCancelCorrectClearingFirm, tradecancelcorrectclearingfirm))
    {
        setLastError ("TradeCancelCorrectClearingFirm missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCancelCorrectClearingFirm (tradecancelcorrectclearingfirm);
    offset += 4;
    
    string tradecancelcorrectclearingaccount;
    if (!d.getString (TradeCancelCorrectClearingAccount, tradecancelcorrectclearingaccount))
    {
        setLastError ("TradeCancelCorrectClearingAccount missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCancelCorrectClearingAccount (tradecancelcorrectclearingaccount);
    offset += 4;
    
    uint32_t tradecancelcorrectlastshares;
    if (!d.getInteger (TradeCancelCorrectLastShares, tradecancelcorrectlastshares))
    {
        setLastError ("TradeCancelCorrectLastShares missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCancelCorrectLastShares (tradecancelcorrectlastshares);
    offset += sizeof (uint32_t);
    
    uint64_t lastpx;
    if (!d.getInteger (LastPx, lastpx))
    {
        setLastError ("LastPx missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx (lastpx);
    offset += sizeof (uint64_t);
    
    uint64_t correctedprice;
    if (!d.getInteger (CorrectedPrice, correctedprice))
    {
        setLastError ("CorrectedPrice missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setCorrectedPrice (correctedprice);
    offset += sizeof (uint64_t);
    
    string origtime;
    if (!d.getString (OrigTime, origtime))
    {
        setLastError ("OrigTime missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setOrigTime (origtime);
    offset += 8;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mTradeCancelCorrectBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCancelCorrectBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCancelCorrectBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCancelCorrectBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCancelCorrectBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCancelCorrectBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCancelCorrectBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCancelCorrectBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCancelCorrectBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCancelCorrectBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCancelCorrectBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCancelCorrectBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCancelCorrectBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCancelCorrectBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCancelCorrectBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCancelCorrectBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCancelCorrectBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCaptureReportAcknowledgement (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportAcknowledgementPacket* packet = (boeTradeCaptureReportAcknowledgementPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCaptureReportAcknowledgementPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string tradereportid;
    if (!d.getString (TradeReportID, tradereportid))
    {
        setLastError ("TradeReportID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeReportID (tradereportid);
    offset += 20;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mTradeCaptureReportAcknowledgementBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportAcknowledgementBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    uint8_t nosides;
    if (!d.getInteger (NoSides, nosides))
    {
        setLastError ("NoSides missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoSides (nosides);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (SideGroupSection) > 0)
    {
        cdrArray* SideGroupArray = NULL;
        d.getArray (SideGroupSection, (const cdrArray**)(&SideGroupArray));
        for (cdrArray::iterator it = SideGroupArray->begin(); it != SideGroupArray->end(); ++it)
        {
            cdr& item = *it;
            BoeSideGroupPacket* sidegroupElement = (BoeSideGroupPacket*) ((char*)buf + offset);

            uint8_t side;
            if (!item.getInteger (Side, side))
            {
                setLastError ("Side missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setSide (side);
            offset += sizeof (uint8_t);

            uint8_t capacity;
            if (!item.getInteger (Capacity, capacity))
            {
                setLastError ("Capacity missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setCapacity (capacity);
            offset += sizeof (uint8_t);

            string partyid;
            if (!item.getString (PartyID, partyid))
            {
                setLastError ("PartyID missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyID (partyid);
            offset += 4;

            string account;
            if (!item.getString (Account, account))
            {
                setLastError ("Account missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setAccount (account);
            offset += 16;

            uint8_t partyrole;
            if (!item.getInteger (PartyRole, partyrole))
            {
                setLastError ("PartyRole missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyRole (partyrole);
            offset += sizeof (uint8_t);

        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCaptureReportReject (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportRejectPacket* packet = (boeTradeCaptureReportRejectPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCaptureReportRejectPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string tradereportid;
    if (!d.getString (TradeReportID, tradereportid))
    {
        setLastError ("TradeReportID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeReportID (tradereportid);
    offset += 20;
    
    uint8_t tradecapturerejectreason;
    if (!d.getInteger (TradeCaptureRejectReason, tradecapturerejectreason))
    {
        setLastError ("TradeCaptureRejectReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureRejectReason (tradecapturerejectreason);
    offset += sizeof (uint8_t);
    
    string tradecapturerejecttext;
    if (!d.getString (TradeCaptureRejectText, tradecapturerejecttext))
    {
        setLastError ("TradeCaptureRejectText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureRejectText (tradecapturerejecttext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mTradeCaptureReportRejectBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportRejectBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportRejectBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    uint8_t nosides;
    if (!d.getInteger (NoSides, nosides))
    {
        setLastError ("NoSides missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoSides (nosides);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (SideGroupSection) > 0)
    {
        cdrArray* SideGroupArray = NULL;
        d.getArray (SideGroupSection, (const cdrArray**)(&SideGroupArray));
        for (cdrArray::iterator it = SideGroupArray->begin(); it != SideGroupArray->end(); ++it)
        {
            cdr& item = *it;
            BoeSideGroupPacket* sidegroupElement = (BoeSideGroupPacket*) ((char*)buf + offset);

            uint8_t side;
            if (!item.getInteger (Side, side))
            {
                setLastError ("Side missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setSide (side);
            offset += sizeof (uint8_t);

            uint8_t capacity;
            if (!item.getInteger (Capacity, capacity))
            {
                setLastError ("Capacity missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setCapacity (capacity);
            offset += sizeof (uint8_t);

            string partyid;
            if (!item.getString (PartyID, partyid))
            {
                setLastError ("PartyID missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyID (partyid);
            offset += 4;

            string account;
            if (!item.getString (Account, account))
            {
                setLastError ("Account missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setAccount (account);
            offset += 16;

            uint8_t partyrole;
            if (!item.getInteger (PartyRole, partyrole))
            {
                setLastError ("PartyRole missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyRole (partyrole);
            offset += sizeof (uint8_t);

        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCaptureConfirm (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureConfirmPacket* packet = (boeTradeCaptureConfirmPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCaptureConfirmPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string tradereportid;
    if (!d.getString (TradeReportID, tradereportid))
    {
        setLastError ("TradeReportID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeReportID (tradereportid);
    offset += 20;
    
    string tradecapturetradereportrefid;
    if (!d.getString (TradeCaptureTradeReportRefID, tradecapturetradereportrefid))
    {
        setLastError ("TradeCaptureTradeReportRefID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureTradeReportRefID (tradecapturetradereportrefid);
    offset += 20;
    
    string tradeid;
    if (!d.getString (TradeID, tradeid))
    {
        setLastError ("TradeID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeID (tradeid);
    offset += 8;
    
    uint32_t tradecapturelastshares;
    if (!d.getInteger (TradeCaptureLastShares, tradecapturelastshares))
    {
        setLastError ("TradeCaptureLastShares missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureLastShares (tradecapturelastshares);
    offset += sizeof (uint32_t);
    
    uint64_t lastpx;
    if (!d.getInteger (LastPx, lastpx))
    {
        setLastError ("LastPx missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx (lastpx);
    offset += sizeof (uint64_t);
    
    string contrabroker;
    if (!d.getString (ContraBroker, contrabroker))
    {
        setLastError ("ContraBroker missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setContraBroker (contrabroker);
    offset += 4;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mTradeCaptureConfirmBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCaptureConfirmBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureConfirmBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureConfirmBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureConfirmBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureConfirmBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureConfirmBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureConfirmBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureConfirmBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureConfirmBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureConfirmBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureConfirmBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureConfirmBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureConfirmBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureConfirmBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    uint8_t nosides;
    if (!d.getInteger (NoSides, nosides))
    {
        setLastError ("NoSides missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoSides (nosides);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (SideGroupSection) > 0)
    {
        cdrArray* SideGroupArray = NULL;
        d.getArray (SideGroupSection, (const cdrArray**)(&SideGroupArray));
        for (cdrArray::iterator it = SideGroupArray->begin(); it != SideGroupArray->end(); ++it)
        {
            cdr& item = *it;
            BoeSideGroupPacket* sidegroupElement = (BoeSideGroupPacket*) ((char*)buf + offset);

            uint8_t side;
            if (!item.getInteger (Side, side))
            {
                setLastError ("Side missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setSide (side);
            offset += sizeof (uint8_t);

            uint8_t capacity;
            if (!item.getInteger (Capacity, capacity))
            {
                setLastError ("Capacity missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setCapacity (capacity);
            offset += sizeof (uint8_t);

            string partyid;
            if (!item.getString (PartyID, partyid))
            {
                setLastError ("PartyID missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyID (partyid);
            offset += 4;

            string account;
            if (!item.getString (Account, account))
            {
                setLastError ("Account missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setAccount (account);
            offset += 16;

            uint8_t partyrole;
            if (!item.getInteger (PartyRole, partyrole))
            {
                setLastError ("PartyRole missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyRole (partyrole);
            offset += sizeof (uint8_t);

        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putTradeCaptureReportDecline (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boeTradeCaptureReportDeclinePacket* packet = (boeTradeCaptureReportDeclinePacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boeTradeCaptureReportDeclinePacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    string tradereportid;
    if (!d.getString (TradeReportID, tradereportid))
    {
        setLastError ("TradeReportID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeReportID (tradereportid);
    offset += 20;
    
    string tradecapturedeclinetradereportrefid;
    if (!d.getString (TradeCaptureDeclineTradeReportRefID, tradecapturedeclinetradereportrefid))
    {
        setLastError ("TradeCaptureDeclineTradeReportRefID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureDeclineTradeReportRefID (tradecapturedeclinetradereportrefid);
    offset += 20;
    
    string tradeid;
    if (!d.getString (TradeID, tradeid))
    {
        setLastError ("TradeID missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeID (tradeid);
    offset += 8;
    
    uint32_t tradecapturedeclinelastshares;
    if (!d.getInteger (TradeCaptureDeclineLastShares, tradecapturedeclinelastshares))
    {
        setLastError ("TradeCaptureDeclineLastShares missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureDeclineLastShares (tradecapturedeclinelastshares);
    offset += sizeof (uint32_t);
    
    uint64_t lastpx;
    if (!d.getInteger (LastPx, lastpx))
    {
        setLastError ("LastPx missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx (lastpx);
    offset += sizeof (uint64_t);
    
    string contrabroker;
    if (!d.getString (ContraBroker, contrabroker))
    {
        setLastError ("ContraBroker missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setContraBroker (contrabroker);
    offset += 4;
    
    uint8_t tradecapturedeclinereason;
    if (!d.getInteger (TradeCaptureDeclineReason, tradecapturedeclinereason))
    {
        setLastError ("TradeCaptureDeclineReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureDeclineReason (tradecapturedeclinereason);
    offset += sizeof (uint8_t);
    
    string tradecapturedeclinetext;
    if (!d.getString (TradeCaptureDeclineText, tradecapturedeclinetext))
    {
        setLastError ("TradeCaptureDeclineText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setTradeCaptureDeclineText (tradecapturedeclinetext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mTradeCaptureReportDeclineBits->hasSide)
    {
       string side;
       if (!d.getString (Side, side))
       {
           setLastError ("Side missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, side.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasPegDifference)
    {
       uint64_t pegdifference;
       if (!d.getInteger (PegDifference, pegdifference))
       {
           setLastError ("PegDifference missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &pegdifference, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasPrice)
    {
       uint64_t price;
       if (!d.getInteger (Price, price))
       {
           setLastError ("Price missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &price, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasExecInst)
    {
       string execinst;
       if (!d.getString (ExecInst, execinst))
       {
           setLastError ("ExecInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, execinst.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrdType)
    {
       string ordtype;
       if (!d.getString (OrdType, ordtype))
       {
           setLastError ("OrdType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ordtype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTimeInForce)
    {
       string timeinforce;
       if (!d.getString (TimeInForce, timeinforce))
       {
           setLastError ("TimeInForce missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, timeinforce.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasMinQty)
    {
       uint32_t minqty;
       if (!d.getInteger (MinQty, minqty))
       {
           setLastError ("MinQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &minqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSymbol)
    {
       string symbol;
       if (!d.getString (Symbol, symbol))
       {
           setLastError ("Symbol missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, symbol.c_str(), 8);
       offset += 8;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSymbolSfx)
    {
       uint8_t symbolsfx;
       if (!d.getInteger (SymbolSfx, symbolsfx))
       {
           setLastError ("SymbolSfx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &symbolsfx, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCurrency)
    {
       string currency;
       if (!d.getString (Currency, currency))
       {
           setLastError ("Currency missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, currency.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasIDSource)
    {
       string idsource;
       if (!d.getString (IDSource, idsource))
       {
           setLastError ("IDSource missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, idsource.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSecurityID)
    {
       string securityid;
       if (!d.getString (SecurityID, securityid))
       {
           setLastError ("SecurityID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityid.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSecurityExchange)
    {
       string securityexchange;
       if (!d.getString (SecurityExchange, securityexchange))
       {
           setLastError ("SecurityExchange missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, securityexchange.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCapacity)
    {
       string capacity;
       if (!d.getString (Capacity, capacity))
       {
           setLastError ("Capacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, capacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAccount)
    {
       string account;
       if (!d.getString (Account, account))
       {
           setLastError ("Account missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, account.c_str(), 16);
       offset += 16;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasClearingAccount)
    {
       string clearingaccount;
       if (!d.getString (ClearingAccount, clearingaccount))
       {
           setLastError ("ClearingAccount missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingaccount.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDisplayIndicator)
    {
       string displayindicator;
       if (!d.getString (DisplayIndicator, displayindicator))
       {
           setLastError ("DisplayIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, displayindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasMaxFloor)
    {
       uint32_t maxfloor;
       if (!d.getInteger (MaxFloor, maxfloor))
       {
           setLastError ("MaxFloor missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &maxfloor, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrderQty)
    {
       uint32_t orderqty;
       if (!d.getInteger (OrderQty, orderqty))
       {
           setLastError ("OrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &orderqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasPreventParticipantMatch)
    {
       string preventparticipantmatch;
       if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
       {
           setLastError ("PreventParticipantMatch missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, preventparticipantmatch.c_str(), 3);
       offset += 3;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCorrectedSize)
    {
       uint32_t correctedsize;
       if (!d.getInteger (CorrectedSize, correctedsize))
       {
           setLastError ("CorrectedSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &correctedsize, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrigClOrdID)
    {
       string origclordid;
       if (!d.getString (OrigClOrdID, origclordid))
       {
           setLastError ("OrigClOrdID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, origclordid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLeavesQty)
    {
       uint32_t leavesqty;
       if (!d.getInteger (LeavesQty, leavesqty))
       {
           setLastError ("LeavesQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &leavesqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLastShares)
    {
       uint32_t lastshares;
       if (!d.getInteger (LastShares, lastshares))
       {
           setLastError ("LastShares missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &lastshares, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDisplayPrice)
    {
       uint64_t displayprice;
       if (!d.getInteger (DisplayPrice, displayprice))
       {
           setLastError ("DisplayPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &displayprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasWorkingPrice)
    {
       uint64_t workingprice;
       if (!d.getInteger (WorkingPrice, workingprice))
       {
           setLastError ("WorkingPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &workingprice, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasBaseLiquidityIndicator)
    {
       string baseliquidityindicator;
       if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
       {
           setLastError ("BaseLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, baseliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSecondaryOrderID)
    {
       uint64_t secondaryorderid;
       if (!d.getInteger (SecondaryOrderID, secondaryorderid))
       {
           setLastError ("SecondaryOrderID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryorderid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCcp)
    {
       string ccp;
       if (!d.getString (Ccp, ccp))
       {
           setLastError ("Ccp missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, ccp.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasContraCapacity)
    {
       string contracapacity;
       if (!d.getString (ContraCapacity, contracapacity))
       {
           setLastError ("ContraCapacity missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, contracapacity.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAttributedQuote)
    {
       uint8_t attributedquote;
       if (!d.getInteger (AttributedQuote, attributedquote))
       {
           setLastError ("AttributedQuote missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &attributedquote, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasBulkOrderIDs)
    {
       uint64_t bulkorderids;
       if (!d.getInteger (BulkOrderIDs, bulkorderids))
       {
           setLastError ("BulkOrderIDs missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bulkorderids, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasBulkRejectReasons)
    {
       string bulkrejectreasons;
       if (!d.getString (BulkRejectReasons, bulkrejectreasons))
       {
           setLastError ("BulkRejectReasons missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, bulkrejectreasons.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasPartyRole)
    {
       string partyrole;
       if (!d.getString (PartyRole, partyrole))
       {
           setLastError ("PartyRole missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, partyrole.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSubLiquidityIndicator)
    {
       string subliquidityindicator;
       if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
       {
           setLastError ("SubLiquidityIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, subliquidityindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradeReportTypeReturn)
    {
       uint16_t tradereporttypereturn;
       if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
       {
           setLastError ("TradeReportTypeReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradereporttypereturn, sizeof(uint16_t));
       offset += sizeof (uint16_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradePublishIndReturn)
    {
       uint8_t tradepublishindreturn;
       if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
       {
           setLastError ("TradePublishIndReturn missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradepublishindreturn, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasText)
    {
       string text;
       if (!d.getString (Text, text))
       {
           setLastError ("Text missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, text.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasBidPx)
    {
       uint64_t bidpx;
       if (!d.getInteger (BidPx, bidpx))
       {
           setLastError ("BidPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &bidpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLargeSize)
    {
       uint64_t largesize;
       if (!d.getInteger (LargeSize, largesize))
       {
           setLastError ("LargeSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &largesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLastMkt)
    {
       string lastmkt;
       if (!d.getString (LastMkt, lastmkt))
       {
           setLastError ("LastMkt missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, lastmkt.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasFeeCode)
    {
       string feecode;
       if (!d.getString (FeeCode, feecode))
       {
           setLastError ("FeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, feecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasEchoText)
    {
       string echotext;
       if (!d.getString (EchoText, echotext))
       {
           setLastError ("EchoText missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, echotext.c_str(), 60);
       offset += 60;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasStopPx)
    {
       uint64_t stoppx;
       if (!d.getInteger (StopPx, stoppx))
       {
           setLastError ("StopPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &stoppx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasRoutingInst)
    {
       string routinginst;
       if (!d.getString (RoutingInst, routinginst))
       {
           setLastError ("RoutingInst missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routinginst.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasRoutStrategy)
    {
       string routstrategy;
       if (!d.getString (RoutStrategy, routstrategy))
       {
           setLastError ("RoutStrategy missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routstrategy.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasExDestination)
    {
       string exdestination;
       if (!d.getString (ExDestination, exdestination))
       {
           setLastError ("ExDestination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, exdestination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradeReportRefID)
    {
       string tradereportrefid;
       if (!d.getString (TradeReportRefID, tradereportrefid))
       {
           setLastError ("TradeReportRefID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportrefid.c_str(), 20);
       offset += 20;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasMarketingFeeCode)
    {
       string marketingfeecode;
       if (!d.getString (MarketingFeeCode, marketingfeecode))
       {
           setLastError ("MarketingFeeCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, marketingfeecode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTargetPartyID)
    {
       string targetpartyid;
       if (!d.getString (TargetPartyID, targetpartyid))
       {
           setLastError ("TargetPartyID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, targetpartyid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAuctionID)
    {
       uint64_t auctionid;
       if (!d.getInteger (AuctionID, auctionid))
       {
           setLastError ("AuctionID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &auctionid, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrderCategory)
    {
       uint8_t ordercategory;
       if (!d.getInteger (OrderCategory, ordercategory))
       {
           setLastError ("OrderCategory missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &ordercategory, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLiquidityProvision)
    {
       string liquidityprovision;
       if (!d.getString (LiquidityProvision, liquidityprovision))
       {
           setLastError ("LiquidityProvision missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, liquidityprovision.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCmtaNumber)
    {
       uint32_t cmtanumber;
       if (!d.getInteger (CmtaNumber, cmtanumber))
       {
           setLastError ("CmtaNumber missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cmtanumber, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCrossType)
    {
       string crosstype;
       if (!d.getString (CrossType, crosstype))
       {
           setLastError ("CrossType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crosstype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCrossPrioritization)
    {
       string crossprioritization;
       if (!d.getString (CrossPrioritization, crossprioritization))
       {
           setLastError ("CrossPrioritization missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossprioritization.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAllocQty)
    {
       uint32_t allocqty;
       if (!d.getInteger (AllocQty, allocqty))
       {
           setLastError ("AllocQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &allocqty, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasRoutingFirmID)
    {
       string routingfirmid;
       if (!d.getString (RoutingFirmID, routingfirmid))
       {
           setLastError ("RoutingFirmID missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, routingfirmid.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasWaiverType)
    {
       string waivertype;
       if (!d.getString (WaiverType, waivertype))
       {
           setLastError ("WaiverType missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, waivertype.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCrossExclusionIndicator)
    {
       string crossexclusionindicator;
       if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
       {
           setLastError ("CrossExclusionIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, crossexclusionindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasPriceFormation)
    {
       string priceformation;
       if (!d.getString (PriceFormation, priceformation))
       {
           setLastError ("PriceFormation missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, priceformation.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasClientQualifiedRole)
    {
       uint8_t clientqualifiedrole;
       if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
       {
           setLastError ("ClientQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasClientID)
    {
       uint32_t clientid;
       if (!d.getInteger (ClientID, clientid))
       {
           setLastError ("ClientID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &clientid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasInvestorID)
    {
       uint32_t investorid;
       if (!d.getInteger (InvestorID, investorid))
       {
           setLastError ("InvestorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasExecutorID)
    {
       uint32_t executorid;
       if (!d.getInteger (ExecutorID, executorid))
       {
           setLastError ("ExecutorID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrderOrigination)
    {
       string orderorigination;
       if (!d.getString (OrderOrigination, orderorigination))
       {
           setLastError ("OrderOrigination missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, orderorigination.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAlgorithmicIndicator)
    {
       string algorithmicindicator;
       if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
       {
           setLastError ("AlgorithmicIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, algorithmicindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDeferralReason)
    {
       string deferralreason;
       if (!d.getString (DeferralReason, deferralreason))
       {
           setLastError ("DeferralReason missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, deferralreason.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasInvestorQualifiedRole)
    {
       uint8_t investorqualifiedrole;
       if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
       {
           setLastError ("InvestorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &investorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasExecutorQualifiedRole)
    {
       uint8_t executorqualifiedrole;
       if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
       {
           setLastError ("ExecutorQualifiedRole missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &executorqualifiedrole, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCtiCode)
    {
       string cticode;
       if (!d.getString (CtiCode, cticode))
       {
           setLastError ("CtiCode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, cticode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasManualOrderIndicator)
    {
       uint8_t manualorderindicator;
       if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
       {
           setLastError ("ManualOrderIndicator missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &manualorderindicator, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradeDate)
    {
       uint64_t tradedate;
       if (!d.getInteger (TradeDate, tradedate))
       {
           setLastError ("TradeDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &tradedate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasVariancePrice)
    {
       uint8_t varianceprice;
       if (!d.getInteger (VariancePrice, varianceprice))
       {
           setLastError ("VariancePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &varianceprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasVarianceSize)
    {
       uint64_t variancesize;
       if (!d.getInteger (VarianceSize, variancesize))
       {
           setLastError ("VarianceSize missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &variancesize, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasOrigTASPrice)
    {
       uint8_t origtasprice;
       if (!d.getInteger (OrigTASPrice, origtasprice))
       {
           setLastError ("OrigTASPrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &origtasprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasCumQty)
    {
       uint8_t cumqty;
       if (!d.getInteger (CumQty, cumqty))
       {
           setLastError ("CumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &cumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDayOrderQty)
    {
       uint8_t dayorderqty;
       if (!d.getInteger (DayOrderQty, dayorderqty))
       {
           setLastError ("DayOrderQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayorderqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDayCumQty)
    {
       uint8_t daycumqty;
       if (!d.getInteger (DayCumQty, daycumqty))
       {
           setLastError ("DayCumQty missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &daycumqty, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasAvgPx)
    {
       uint64_t avgpx;
       if (!d.getInteger (AvgPx, avgpx))
       {
           setLastError ("AvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &avgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasDayAvgPx)
    {
       uint64_t dayavgpx;
       if (!d.getInteger (DayAvgPx, dayavgpx))
       {
           setLastError ("DayAvgPx missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &dayavgpx, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLegCFICode)
    {
       string legcficode;
       if (!d.getString (LegCFICode, legcficode))
       {
           setLastError ("LegCFICode missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, legcficode.c_str(), 2);
       offset += 2;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLegMaturityDate)
    {
       uint64_t legmaturitydate;
       if (!d.getInteger (LegMaturityDate, legmaturitydate))
       {
           setLastError ("LegMaturityDate missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legmaturitydate, sizeof(uint64_t));
       offset += sizeof (uint64_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasLegStrikePrice)
    {
       uint8_t legstrikeprice;
       if (!d.getInteger (LegStrikePrice, legstrikeprice))
       {
           setLastError ("LegStrikePrice missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &legstrikeprice, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasSecondaryExecID)
    {
       uint32_t secondaryexecid;
       if (!d.getInteger (SecondaryExecID, secondaryexecid))
       {
           setLastError ("SecondaryExecID missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &secondaryexecid, sizeof(uint32_t));
       offset += sizeof (uint32_t);
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasUsername)
    {
       string username;
       if (!d.getString (Username, username))
       {
           setLastError ("Username missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, username.c_str(), 4);
       offset += 4;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradeReportingIndicator)
    {
       string tradereportingindicator;
       if (!d.getString (TradeReportingIndicator, tradereportingindicator))
       {
           setLastError ("TradeReportingIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradereportingindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasTradePublishIndicator)
    {
       string tradepublishindicator;
       if (!d.getString (TradePublishIndicator, tradepublishindicator))
       {
           setLastError ("TradePublishIndicator missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, tradepublishindicator.c_str(), 1);
       offset += 1;
    }
    
    
    if (mTradeCaptureReportDeclineBits->hasReportTime)
    {
       uint8_t reporttime;
       if (!d.getInteger (ReportTime, reporttime))
       {
           setLastError ("ReportTime missing or not integer");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, &reporttime, sizeof(uint8_t));
       offset += sizeof (uint8_t);
    }
    
    
    uint8_t nosides;
    if (!d.getInteger (NoSides, nosides))
    {
        setLastError ("NoSides missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNoSides (nosides);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (SideGroupSection) > 0)
    {
        cdrArray* SideGroupArray = NULL;
        d.getArray (SideGroupSection, (const cdrArray**)(&SideGroupArray));
        for (cdrArray::iterator it = SideGroupArray->begin(); it != SideGroupArray->end(); ++it)
        {
            cdr& item = *it;
            BoeSideGroupPacket* sidegroupElement = (BoeSideGroupPacket*) ((char*)buf + offset);

            uint8_t side;
            if (!item.getInteger (Side, side))
            {
                setLastError ("Side missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setSide (side);
            offset += sizeof (uint8_t);

            uint8_t capacity;
            if (!item.getInteger (Capacity, capacity))
            {
                setLastError ("Capacity missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setCapacity (capacity);
            offset += sizeof (uint8_t);

            string partyid;
            if (!item.getString (PartyID, partyid))
            {
                setLastError ("PartyID missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyID (partyid);
            offset += 4;

            string account;
            if (!item.getString (Account, account))
            {
                setLastError ("Account missing or not string");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setAccount (account);
            offset += 16;

            uint8_t partyrole;
            if (!item.getInteger (PartyRole, partyrole))
            {
                setLastError ("PartyRole missing or not integer");
                return GW_CODEC_ERROR;
            }
            sidegroupElement->setPartyRole (partyrole);
            offset += sizeof (uint8_t);

        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putPurgeOrder (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boePurgeOrderPacket* packet = (boePurgeOrderPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boePurgeOrderPacket))
        return GW_CODEC_SHORT;

    
    string masscancel;
    if (!d.getString (MassCancel, masscancel))
    {
        setLastError ("MassCancel missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setMassCancel (masscancel);
    offset += 1;
    
    uint8_t numberofbitfields;
    if (!d.getInteger (NumberOfBitfields, numberofbitfields))
    {
        setLastError ("NumberOfBitfields missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields (numberofbitfields);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (BitfieldSection) > 0)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator it = BitfieldArray->begin(); it != BitfieldArray->end(); ++it)
        {
            cdr& item = *it;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ((char*)buf + offset);

            uint8_t bitfield;
            if (!item.getInteger (Bitfield, bitfield))
                bitfieldElement->setBitfield (UINT8_MAX);
            else
                bitfieldElement->setBitfield (bitfield);
            offset += sizeof (uint8_t);
            

        }
    }
    
    if (mPurgeOrderBits->hasClearingFirm)
    {
       string clearingfirm;
       if (!d.getString (ClearingFirm, clearingfirm))
       {
           setLastError ("ClearingFirm missing or not string");
           return GW_CODEC_ERROR;
       }
       memcpy (tmpBuf + offset, clearingfirm.c_str(), 4);
       offset += 4;
    }
    
    
    uint8_t numberofcustomgroupids;
    if (!d.getInteger (NumberOfCustomGroupIDs, numberofcustomgroupids))
    {
        setLastError ("NumberOfCustomGroupIDs missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfCustomGroupIDs (numberofcustomgroupids);
    offset += sizeof (uint8_t);
    
    if (d.getArraySize (CustomGroupIDSection) > 0)
    {
        cdrArray* CustomGroupIDArray = NULL;
        d.getArray (CustomGroupIDSection, (const cdrArray**)(&CustomGroupIDArray));
        for (cdrArray::iterator it = CustomGroupIDArray->begin(); it != CustomGroupIDArray->end(); ++it)
        {
            cdr& item = *it;
            BoeCustomGroupIDPacket* customgroupidElement = (BoeCustomGroupIDPacket*) ((char*)buf + offset);

            uint16_t customgroupid;
            if (!item.getInteger (CustomGroupID, customgroupid))
            {
                setLastError ("CustomGroupID missing or not integer");
                return GW_CODEC_ERROR;
            }
            customgroupidElement->setCustomGroupID (customgroupid);
            offset += sizeof (uint16_t);

        }
    }
    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putPurgeRejected (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket);
    boePurgeRejectedPacket* packet = (boePurgeRejectedPacket*)((char*)buf + offset);

    char *tmpBuf = (char *)buf;

    if (len < sizeof (boePurgeRejectedPacket))
        return GW_CODEC_SHORT;

    
    uint64_t transacttime;
    if (!d.getInteger (TransactTime, transacttime))
    {
        setLastError ("TransactTime missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime (transacttime);
    offset += sizeof (uint64_t);
    
    uint8_t purgerejectreason;
    if (!d.getInteger (PurgeRejectReason, purgerejectreason))
    {
        setLastError ("PurgeRejectReason missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setPurgeRejectReason (purgerejectreason);
    offset += sizeof (uint8_t);
    
    string purgerejecttext;
    if (!d.getString (PurgeRejectText, purgerejecttext))
    {
        setLastError ("PurgeRejectText missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setPurgeRejectText (purgerejecttext);
    offset += 60;
    
    string reservedinternal;
    if (!d.getString (ReservedInternal, reservedinternal))
    {
        setLastError ("ReservedInternal missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setReservedInternal (reservedinternal);
    offset += 1;
    used += offset;

    return GW_CODEC_SUCCESS;
}


codecState
boeCodec::decode (cdr& d, const void* buf, size_t len, size_t& used)
{

    clearLastError ();
    used = 0;
    d.clear ();

    if (len < sizeof(BoeHeaderPacket))
        return GW_CODEC_SHORT;

    BoeHeaderPacket* hdr = (BoeHeaderPacket*)((unsigned char*)buf);

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;

    d.setInteger (MessageType,  hdr->getMessageType ());
    d.setInteger (MessageLength, hdr->getMessageLength ());
    d.setInteger (MatchingUnit, hdr->getMatchingUnit ());
    d.setInteger (SequenceNumber, hdr->getSequenceNumber ());

    switch (hdr->mMessageType) {
        case 0x37:
            return getLoginRequest (d, buf, used);
        case 0x24:
            return getLoginResponse (d, buf, used);
        case 0x08:
            return getLogoutResponse (d, buf, used);
        case 0x09:
            return getServerHeartbeat (d, buf, used);
        case 0x03:
            return getClientHeartbeat (d, buf, used);
        case 0x13:
            return getReplayComplete (d, buf, used);
        case 0x38:
            return getNewOrder (d, buf, used);
        case 0x39:
            return getCancelOrder (d, buf, used);
        case 0x3A:
            return getModifyOrder (d, buf, used);
        case 0x3C:
            return getTradeCaptureReport (d, buf, used);
        case 0x25:
            return getOrderAcknowledgement (d, buf, used);
        case 0x26:
            return getOrderRejected (d, buf, used);
        case 0x27:
            return getOrderModified (d, buf, used);
        case 0x28:
            return getOrderRestated (d, buf, used);
        case 0x29:
            return getUserModifyRejected (d, buf, used);
        case 0x2A:
            return getOrderCancelled (d, buf, used);
        case 0x2B:
            return getCancelRejected (d, buf, used);
        case 0x2C:
            return getOrderExecution (d, buf, used);
        case 0x2D:
            return getTradeCancelCorrect (d, buf, used);
        case 0x30:
            return getTradeCaptureReportAcknowledgement (d, buf, used);
        case 0x31:
            return getTradeCaptureReportReject (d, buf, used);
        case 0x32:
            return getTradeCaptureConfirm (d, buf, used);
        case 0x33:
            return getTradeCaptureReportDecline (d, buf, used);
        case 0x47:
            return getPurgeOrder (d, buf, used);
        case 0x48:
            return getPurgeRejected (d, buf, used);
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    return GW_CODEC_ERROR;
}

codecState
boeCodec::encode (const cdr& d, void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;

    BoeHeaderPacket* hdr = (BoeHeaderPacket*)((char*)buf);

    BoeHeaderPacket base;
    memcpy (buf, &base, sizeof(base));

    if (len < sizeof(BoeHeaderPacket))
        return GW_CODEC_SHORT;

    if (!d.isType (MessageType, CDR_INTEGER, 1))
    {
        setLastError ("MessageType missing or not string");
        return GW_CODEC_ERROR;
    }
    uint8_t type;
    d.getInteger (MessageType, type);
    hdr->setMessageType (type);

    uint8_t matchingUnit;
    if(!d.getInteger (MatchingUnit, matchingUnit))
    {
        setLastError("MatchingUnit is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMatchingUnit (matchingUnit);

    uint32_t sequenceNumber;
    if(!d.getInteger (SequenceNumber, sequenceNumber))
    {
        setLastError("SequenceNumber is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setSequenceNumber (sequenceNumber);

    codecState state = GW_CODEC_ERROR;
    switch (type) {
        case 0x37:
            state = putLoginRequest (d, buf, len, used);
            break;
        case 0x24:
            state = putLoginResponse (d, buf, len, used);
            break;
        case 0x08:
            state = putLogoutResponse (d, buf, len, used);
            break;
        case 0x09:
            state = putServerHeartbeat (d, buf, len, used);
            break;
        case 0x03:
            state = putClientHeartbeat (d, buf, len, used);
            break;
        case 0x13:
            state = putReplayComplete (d, buf, len, used);
            break;
        case 0x38:
            state = putNewOrder (d, buf, len, used);
            break;
        case 0x39:
            state = putCancelOrder (d, buf, len, used);
            break;
        case 0x3A:
            state = putModifyOrder (d, buf, len, used);
            break;
        case 0x3C:
            state = putTradeCaptureReport (d, buf, len, used);
            break;
        case 0x25:
            state = putOrderAcknowledgement (d, buf, len, used);
            break;
        case 0x26:
            state = putOrderRejected (d, buf, len, used);
            break;
        case 0x27:
            state = putOrderModified (d, buf, len, used);
            break;
        case 0x28:
            state = putOrderRestated (d, buf, len, used);
            break;
        case 0x29:
            state = putUserModifyRejected (d, buf, len, used);
            break;
        case 0x2A:
            state = putOrderCancelled (d, buf, len, used);
            break;
        case 0x2B:
            state = putCancelRejected (d, buf, len, used);
            break;
        case 0x2C:
            state = putOrderExecution (d, buf, len, used);
            break;
        case 0x2D:
            state = putTradeCancelCorrect (d, buf, len, used);
            break;
        case 0x30:
            state = putTradeCaptureReportAcknowledgement (d, buf, len, used);
            break;
        case 0x31:
            state = putTradeCaptureReportReject (d, buf, len, used);
            break;
        case 0x32:
            state = putTradeCaptureConfirm (d, buf, len, used);
            break;
        case 0x33:
            state = putTradeCaptureReportDecline (d, buf, len, used);
            break;
        case 0x47:
            state = putPurgeOrder (d, buf, len, used);
            break;
        case 0x48:
            state = putPurgeRejected (d, buf, len, used);
            break;
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    //Message Length does not include first two bytes of message
    hdr->setMessageLength (used - 2);
    return state;
}

} // namespace neueda
