@require(Venue, venue, generation_date, messages, msg_data, types, get_field, sets, set_field)
/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated @{generation_date}
 */
#include "@{venue}Codec.h"
#include "@{Venue}Packets.h"
#include "fields.h"
#include "codecHelpers.h"
#include <sstream>


namespace neueda
{

extern "C" codec*
getCodec ()
{
    return new @{venue}Codec ();
}

@for msg in messages:
codecState
@{venue}Codec::get@{msg['name']} (cdr& d, const void* buf, size_t& used)
{
    d.setString (MessageName, "@{msg['name']}");
    size_t offset = sizeof (@{Venue}HeaderPacket);
    @if len(msg_data[msg['name']]) > 0:
    @{venue}@{msg['name']}Packet* packet = (@{venue}@{msg['name']}Packet*)((char*)buf + offset);
    @end
    @for field in msg_data[msg['name']]:
    @get_field(field)
    @end
    @if 'Login' in msg['name']:
    int NumParamGroups = packet->getNumberOfParamGroups ();
   
    cdrArray ParamGroupsArray;
    for (int i = 0; i < NumParamGroups; i++)
    {
        cdr item;
        BoeParamGroupsPacket* paramGroupsElement = (BoeParamGroupsPacket*) ((char*)buf + offset);
        
        item.setInteger(ParamGroupLength, paramGroupsElement->getParamGroupLength ());
        ParamGroupsArray.push_back(item);
        
        item.setInteger(ParamGroupType, paramGroupsElement->getParamGroupType ());
        ParamGroupsArray.push_back(item);
        
        if(paramGroupsElement->getParamGroupType () == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            offset += sizeof (BoeUnitSequencePacket);
            
            cdrArray UnitSequenceArray;
            item.setInteger(NoUnspecifiedUnitReplay, unitSequenceElement->getNoUnspecifiedUnitReplay ());
            UnitSequenceArray.push_back(item);
            
            int NumUnits = unitSequenceElement->getNumOfUnits ();
            item.setInteger(NumOfUnits, unitSequenceElement->getNumOfUnits ());
            UnitSequenceArray.push_back(item);
            
            for (int i = 0; i < NumUnits; i++)
            {
                item.setInteger (UnitNumber, unitSequenceElement->getUnitNumber ());
                offset += sizeof (uint8_t);

                item.setInteger (UnitSequence, unitSequenceElement->getUnitSequence ());
                offset += sizeof (uint64_t);

                UnitSequenceArray.push_back (item);
            }
            d.setArray (UnitSequenceSection, UnitSequenceArray);
        }
        else if(paramGroupsElement->getParamGroupType () == 0x81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);
                        
            cdrArray ReturnBitfieldArray;
            item.setInteger(MessageType, returnBitfieldElement->getMessageType ());
            ReturnBitfieldArray.push_back(item);
            
            int NumReturnBitfields = returnBitfieldElement->getNumOfReturnBitfield ();
            item.setInteger(NumOfReturnBitfield, returnBitfieldElement->getNumOfReturnBitfield ());
            ReturnBitfieldArray.push_back(item);
            
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
            //Get optional fields
            char* newBuf = (char*) buf;

            if (mReturnBits->hasSide)
            {
                d.setString (Side, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasPegDifference)
            {
                d.setInteger (PegDifference, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasPrice)
            {
                d.setInteger (Price, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasExecInst)
            {
                d.setString (ExecInst, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasOrdType)
            {
                d.setString (OrdType, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasTimeInForce)
            {
                d.setString (TimeInForce, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasMinQty)
            {
                d.setInteger (MinQty, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasSymbol)
            {
                d.setString (Symbol, getStringField (newBuf, 8, offset) );
                offset += 8;
            }


            if (mReturnBits->hasSymbolSfx)
            {
                d.setInteger (SymbolSfx, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasCurrency)
            {
                d.setString (Currency, getStringField (newBuf, 3, offset) );
                offset += 3;
            }


            if (mReturnBits->hasIDSource)
            {
                d.setString (IDSource, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasSecurityID)
            {
                d.setString (SecurityID, getStringField (newBuf, 16, offset) );
                offset += 16;
            }


            if (mReturnBits->hasSecurityExchange)
            {
                d.setString (SecurityExchange, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasCapacity)
            {
                d.setString (Capacity, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasAccount)
            {
                d.setString (Account, getStringField (newBuf, 16, offset) );
                offset += 16;
            }


            if (mReturnBits->hasClearingFirm)
            {
                d.setString (ClearingFirm, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasClearingAccount)
            {
                d.setString (ClearingAccount, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasDisplayIndicator)
            {
                d.setString (DisplayIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasMaxFloor)
            {
                d.setInteger (MaxFloor, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasOrderQty)
            {
                d.setInteger (OrderQty, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasPreventParticipantMatch)
            {
                d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
                offset += 3;
            }


            if (mReturnBits->hasCorrectedSize)
            {
                d.setInteger (CorrectedSize, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasOrigClOrdID)
            {
                d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
                offset += 20;
            }


            if (mReturnBits->hasLeavesQty)
            {
                d.setInteger (LeavesQty, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasLastShares)
            {
                d.setInteger (LastShares, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasDisplayPrice)
            {
                d.setInteger (DisplayPrice, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasWorkingPrice)
            {
                d.setInteger (WorkingPrice, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasBaseLiquidityIndicator)
            {
                d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasSecondaryOrderID)
            {
                d.setInteger (SecondaryOrderID, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasCcp)
            {
                d.setString (Ccp, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasContraCapacity)
            {
                d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasAttributedQuote)
            {
                d.setInteger (AttributedQuote, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasBulkOrderIDs)
            {
                d.setInteger (BulkOrderIDs, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasBulkRejectReasons)
            {
                d.setString (BulkRejectReasons, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasPartyRole)
            {
                d.setString (PartyRole, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasSubLiquidityIndicator)
            {
                d.setString (SubLiquidityIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasTradeReportTypeReturn)
            {
                d.setInteger (TradeReportTypeReturn, uint16_t(newBuf[offset]) );
                offset += sizeof (uint16_t);
            }


            if (mReturnBits->hasTradePublishIndReturn)
            {
                d.setInteger (TradePublishIndReturn, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasText)
            {
                d.setString (Text, getStringField (newBuf, 60, offset) );
                offset += 60;
            }


            if (mReturnBits->hasBidPx)
            {
                d.setInteger (BidPx, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasLargeSize)
            {
                d.setInteger (LargeSize, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasLastMkt)
            {
                d.setString (LastMkt, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasFeeCode)
            {
                d.setString (FeeCode, getStringField (newBuf, 2, offset) );
                offset += 2;
            }


            if (mReturnBits->hasEchoText)
            {
                d.setString (EchoText, getStringField (newBuf, 60, offset) );
                offset += 60;
            }


            if (mReturnBits->hasStopPx)
            {
                d.setInteger (StopPx, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasRoutingInst)
            {
                d.setString (RoutingInst, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasRoutStrategy)
            {
                d.setString (RoutStrategy, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasExDestination)
            {
                d.setString (ExDestination, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasTradeReportRefID)
            {
                d.setString (TradeReportRefID, getStringField (newBuf, 20, offset) );
                offset += 20;
            }


            if (mReturnBits->hasMarketingFeeCode)
            {
                d.setString (MarketingFeeCode, getStringField (newBuf, 2, offset) );
                offset += 2;
            }


            if (mReturnBits->hasTargetPartyID)
            {
                d.setString (TargetPartyID, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasAuctionID)
            {
                d.setInteger (AuctionID, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasOrderCategory)
            {
                d.setInteger (OrderCategory, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasLiquidityProvision)
            {
                d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasCmtaNumber)
            {
                d.setInteger (CmtaNumber, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasCrossType)
            {
                d.setString (CrossType, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasCrossPrioritization)
            {
                d.setString (CrossPrioritization, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasAllocQty)
            {
                d.setInteger (AllocQty, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasRoutingFirmID)
            {
                d.setString (RoutingFirmID, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasWaiverType)
            {
                d.setString (WaiverType, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasCrossExclusionIndicator)
            {
                d.setString (CrossExclusionIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasPriceFormation)
            {
                d.setString (PriceFormation, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasClientQualifiedRole)
            {
                d.setInteger (ClientQualifiedRole, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasClientID)
            {
                d.setInteger (ClientID, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasInvestorID)
            {
                d.setInteger (InvestorID, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasExecutorID)
            {
                d.setInteger (ExecutorID, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasOrderOrigination)
            {
                d.setString (OrderOrigination, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasAlgorithmicIndicator)
            {
                d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasDeferralReason)
            {
                d.setString (DeferralReason, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasInvestorQualifiedRole)
            {
                d.setInteger (InvestorQualifiedRole, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasExecutorQualifiedRole)
            {
                d.setInteger (ExecutorQualifiedRole, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasCtiCode)
            {
                d.setString (CtiCode, getStringField (newBuf, 2, offset) );
                offset += 2;
            }


            if (mReturnBits->hasManualOrderIndicator)
            {
                d.setInteger (ManualOrderIndicator, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasTradeDate)
            {
                d.setInteger (TradeDate, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasVariancePrice)
            {
                d.setInteger (VariancePrice, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasVarianceSize)
            {
                d.setInteger (VarianceSize, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasOrigTASPrice)
            {
                d.setInteger (OrigTASPrice, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasCumQty)
            {
                d.setInteger (CumQty, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasDayOrderQty)
            {
                d.setInteger (DayOrderQty, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasDayCumQty)
            {
                d.setInteger (DayCumQty, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasAvgPx)
            {
                d.setInteger (AvgPx, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasDayAvgPx)
            {
                d.setInteger (DayAvgPx, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasLegCFICode)
            {
                d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
                offset += 2;
            }


            if (mReturnBits->hasLegMaturityDate)
            {
                d.setInteger (LegMaturityDate, uint64_t(newBuf[offset]) );
                offset += sizeof (uint64_t);
            }


            if (mReturnBits->hasLegStrikePrice)
            {
                d.setInteger (LegStrikePrice, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }


            if (mReturnBits->hasSecondaryExecID)
            {
                d.setInteger (SecondaryExecID, uint32_t(newBuf[offset]) );
                offset += sizeof (uint32_t);
            }


            if (mReturnBits->hasUsername)
            {
                d.setString (Username, getStringField (newBuf, 4, offset) );
                offset += 4;
            }


            if (mReturnBits->hasTradeReportingIndicator)
            {
                d.setString (TradeReportingIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasTradePublishIndicator)
            {
                d.setString (TradePublishIndicator, getStringField (newBuf, 1, offset) );
                offset += 1;
            }


            if (mReturnBits->hasReportTime)
            {
                d.setInteger (ReportTime, uint8_t(newBuf[offset]) );
                offset += sizeof (uint8_t);
            }
        }
    }
    @end
    used += offset;
    return GW_CODEC_SUCCESS;
}

@end

@for msg in messages:
codecState
@{venue}Codec::put@{msg['name']} (const cdr& d, void* buf, size_t len, size_t& used)
{
    size_t offset = sizeof (@{Venue}HeaderPacket);
    @if len(msg_data[msg['name']]) > 0:
    @{venue}@{msg['name']}Packet* packet = (@{venue}@{msg['name']}Packet*)((char*)buf + offset);
    @end

    if (len < sizeof (@{venue}@{msg['name']}Packet))
        return GW_CODEC_SHORT;

    @for field in msg_data[msg['name']]:
    @set_field(field)
    @end
    @if 'Login' in msg['name']:
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
        offset += sizeof (int16_t);
        
        uint8_t paramGroupType;
        if (!item.getInteger (ParamGroupType, paramGroupType))
        {
            setLastError ("ParamGroupType missing or not integer");
            return GW_CODEC_ERROR;
        }
        paramGroupsElement->setParamGroupType (paramGroupType);
        offset += sizeof (int8_t);
                
        if(paramGroupType == 0x80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            cdrArray UnitSequenceArray;
            
            uint8_t noUnspecifiedUnitReplay;
            if (!item.getInteger (NoUnspecifiedUnitReplay, noUnspecifiedUnitReplay))
            {
                setLastError ("NoUnspecifiedUnitReplay missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNoUnspecifiedUnitReplay (noUnspecifiedUnitReplay);
            offset += sizeof (int8_t);
            
            uint8_t numOfUnits;
            if (!item.getInteger (NumOfUnits, numOfUnits))
            {
                setLastError ("NumOfUnits missing or not integer");
                return GW_CODEC_ERROR;
            }
            unitSequenceElement->setNumOfUnits (numOfUnits);
            offset += sizeof (int8_t);
            
            for (int i = 0; i < numOfUnits; i++)
            {
                uint8_t unitNumber;
                if (!item.getInteger (UnitNumber, unitNumber))
                {
                    setLastError ("UnitNumber missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitSequenceElement->setUnitNumber (unitNumber);
                offset += sizeof (int8_t);
            
                uint64_t unitSequence;
                if (!item.getInteger (UnitSequence, unitSequence))
                {
                    setLastError ("UnitSequence missing or not integer");
                    return GW_CODEC_ERROR;
                }
                unitSequenceElement->setUnitSequence (unitSequence);
                offset += sizeof (int64_t);
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
            offset += sizeof (int8_t);
            
            uint8_t numReturnBitfields;
            if (!item.getInteger (NumOfReturnBitfield, numReturnBitfields))
            {
                setLastError ("NumReturnBitfields missing or not integer");
                return GW_CODEC_ERROR;
            }
            returnBitfieldElement->setNumOfReturnBitfield (numReturnBitfields);
            offset += sizeof (int8_t);
            
            for (int i = 0; i < numReturnBitfields; i++)
            {
                uint8_t returnBitfield;
                if (!item.getInteger (ReturnBitfield, returnBitfield))
                {
                    setLastError ("ReturnBitfield missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setReturnBitfield (returnBitfield);
                offset += sizeof (int8_t); 
            }
            
            if (mReturnBits->hasSide)
                {
                   string side;
                   if (!d.getString (Side, side))
                   {
                       setLastError ("Side missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSide (side);
                   offset += 1;
                }


                if (mReturnBits->hasPegDifference)
                {
                   uint64_t pegdifference;
                   if (!d.getInteger (PegDifference, pegdifference))
                   {
                       setLastError ("PegDifference missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setPegDifference (pegdifference);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasPrice)
                {
                   uint64_t price;
                   if (!d.getInteger (Price, price))
                   {
                       setLastError ("Price missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setPrice (price);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasExecInst)
                {
                   string execinst;
                   if (!d.getString (ExecInst, execinst))
                   {
                       setLastError ("ExecInst missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setExecInst (execinst);
                   offset += 1;
                }


                if (mReturnBits->hasOrdType)
                {
                   string ordtype;
                   if (!d.getString (OrdType, ordtype))
                   {
                       setLastError ("OrdType missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrdType (ordtype);
                   offset += 1;
                }


                if (mReturnBits->hasTimeInForce)
                {
                   string timeinforce;
                   if (!d.getString (TimeInForce, timeinforce))
                   {
                       setLastError ("TimeInForce missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTimeInForce (timeinforce);
                   offset += 1;
                }


                if (mReturnBits->hasMinQty)
                {
                   uint32_t minqty;
                   if (!d.getInteger (MinQty, minqty))
                   {
                       setLastError ("MinQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setMinQty (minqty);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasSymbol)
                {
                   string symbol;
                   if (!d.getString (Symbol, symbol))
                   {
                       setLastError ("Symbol missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSymbol (symbol);
                   offset += 8;
                }


                if (mReturnBits->hasSymbolSfx)
                {
                   uint8_t symbolsfx;
                   if (!d.getInteger (SymbolSfx, symbolsfx))
                   {
                       setLastError ("SymbolSfx missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSymbolSfx (symbolsfx);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasCurrency)
                {
                   string currency;
                   if (!d.getString (Currency, currency))
                   {
                       setLastError ("Currency missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCurrency (currency);
                   offset += 3;
                }


                if (mReturnBits->hasIDSource)
                {
                   string idsource;
                   if (!d.getString (IDSource, idsource))
                   {
                       setLastError ("IDSource missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setIDSource (idsource);
                   offset += 1;
                }


                if (mReturnBits->hasSecurityID)
                {
                   string securityid;
                   if (!d.getString (SecurityID, securityid))
                   {
                       setLastError ("SecurityID missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSecurityID (securityid);
                   offset += 16;
                }


                if (mReturnBits->hasSecurityExchange)
                {
                   string securityexchange;
                   if (!d.getString (SecurityExchange, securityexchange))
                   {
                       setLastError ("SecurityExchange missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSecurityExchange (securityexchange);
                   offset += 4;
                }


                if (mReturnBits->hasCapacity)
                {
                   string capacity;
                   if (!d.getString (Capacity, capacity))
                   {
                       setLastError ("Capacity missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCapacity (capacity);
                   offset += 1;
                }


                if (mReturnBits->hasAccount)
                {
                   string account;
                   if (!d.getString (Account, account))
                   {
                       setLastError ("Account missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAccount (account);
                   offset += 16;
                }


                if (mReturnBits->hasClearingFirm)
                {
                   string clearingfirm;
                   if (!d.getString (ClearingFirm, clearingfirm))
                   {
                       setLastError ("ClearingFirm missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setClearingFirm (clearingfirm);
                   offset += 4;
                }


                if (mReturnBits->hasClearingAccount)
                {
                   string clearingaccount;
                   if (!d.getString (ClearingAccount, clearingaccount))
                   {
                       setLastError ("ClearingAccount missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setClearingAccount (clearingaccount);
                   offset += 4;
                }


                if (mReturnBits->hasDisplayIndicator)
                {
                   string displayindicator;
                   if (!d.getString (DisplayIndicator, displayindicator))
                   {
                       setLastError ("DisplayIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDisplayIndicator (displayindicator);
                   offset += 1;
                }


                if (mReturnBits->hasMaxFloor)
                {
                   uint32_t maxfloor;
                   if (!d.getInteger (MaxFloor, maxfloor))
                   {
                       setLastError ("MaxFloor missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setMaxFloor (maxfloor);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasOrderQty)
                {
                   uint32_t orderqty;
                   if (!d.getInteger (OrderQty, orderqty))
                   {
                       setLastError ("OrderQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrderQty (orderqty);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasPreventParticipantMatch)
                {
                   string preventparticipantmatch;
                   if (!d.getString (PreventParticipantMatch, preventparticipantmatch))
                   {
                       setLastError ("PreventParticipantMatch missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setPreventParticipantMatch (preventparticipantmatch);
                   offset += 3;
                }


                if (mReturnBits->hasCorrectedSize)
                {
                   uint32_t correctedsize;
                   if (!d.getInteger (CorrectedSize, correctedsize))
                   {
                       setLastError ("CorrectedSize missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCorrectedSize (correctedsize);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasOrigClOrdID)
                {
                   string origclordid;
                   if (!d.getString (OrigClOrdID, origclordid))
                   {
                       setLastError ("OrigClOrdID missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrigClOrdID (origclordid);
                   offset += 20;
                }


                if (mReturnBits->hasLeavesQty)
                {
                   uint32_t leavesqty;
                   if (!d.getInteger (LeavesQty, leavesqty))
                   {
                       setLastError ("LeavesQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLeavesQty (leavesqty);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasLastShares)
                {
                   uint32_t lastshares;
                   if (!d.getInteger (LastShares, lastshares))
                   {
                       setLastError ("LastShares missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLastShares (lastshares);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasDisplayPrice)
                {
                   uint64_t displayprice;
                   if (!d.getInteger (DisplayPrice, displayprice))
                   {
                       setLastError ("DisplayPrice missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDisplayPrice (displayprice);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasWorkingPrice)
                {
                   uint64_t workingprice;
                   if (!d.getInteger (WorkingPrice, workingprice))
                   {
                       setLastError ("WorkingPrice missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setWorkingPrice (workingprice);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasBaseLiquidityIndicator)
                {
                   string baseliquidityindicator;
                   if (!d.getString (BaseLiquidityIndicator, baseliquidityindicator))
                   {
                       setLastError ("BaseLiquidityIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setBaseLiquidityIndicator (baseliquidityindicator);
                   offset += 1;
                }


                if (mReturnBits->hasSecondaryOrderID)
                {
                   uint64_t secondaryorderid;
                   if (!d.getInteger (SecondaryOrderID, secondaryorderid))
                   {
                       setLastError ("SecondaryOrderID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSecondaryOrderID (secondaryorderid);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasCcp)
                {
                   string ccp;
                   if (!d.getString (Ccp, ccp))
                   {
                       setLastError ("Ccp missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCcp (ccp);
                   offset += 1;
                }


                if (mReturnBits->hasContraCapacity)
                {
                   string contracapacity;
                   if (!d.getString (ContraCapacity, contracapacity))
                   {
                       setLastError ("ContraCapacity missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setContraCapacity (contracapacity);
                   offset += 1;
                }


                if (mReturnBits->hasAttributedQuote)
                {
                   uint8_t attributedquote;
                   if (!d.getInteger (AttributedQuote, attributedquote))
                   {
                       setLastError ("AttributedQuote missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAttributedQuote (attributedquote);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasBulkOrderIDs)
                {
                   uint64_t bulkorderids;
                   if (!d.getInteger (BulkOrderIDs, bulkorderids))
                   {
                       setLastError ("BulkOrderIDs missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setBulkOrderIDs (bulkorderids);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasBulkRejectReasons)
                {
                   string bulkrejectreasons;
                   if (!d.getString (BulkRejectReasons, bulkrejectreasons))
                   {
                       setLastError ("BulkRejectReasons missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setBulkRejectReasons (bulkrejectreasons);
                   offset += 1;
                }


                if (mReturnBits->hasPartyRole)
                {
                   string partyrole;
                   if (!d.getString (PartyRole, partyrole))
                   {
                       setLastError ("PartyRole missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setPartyRole (partyrole);
                   offset += 1;
                }


                if (mReturnBits->hasSubLiquidityIndicator)
                {
                   string subliquidityindicator;
                   if (!d.getString (SubLiquidityIndicator, subliquidityindicator))
                   {
                       setLastError ("SubLiquidityIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSubLiquidityIndicator (subliquidityindicator);
                   offset += 1;
                }


                if (mReturnBits->hasTradeReportTypeReturn)
                {
                   uint16_t tradereporttypereturn;
                   if (!d.getInteger (TradeReportTypeReturn, tradereporttypereturn))
                   {
                       setLastError ("TradeReportTypeReturn missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTradeReportTypeReturn (tradereporttypereturn);
                   offset += sizeof (uint16_t);
                }


                if (mReturnBits->hasTradePublishIndReturn)
                {
                   uint8_t tradepublishindreturn;
                   if (!d.getInteger (TradePublishIndReturn, tradepublishindreturn))
                   {
                       setLastError ("TradePublishIndReturn missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTradePublishIndReturn (tradepublishindreturn);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasText)
                {
                   string text;
                   if (!d.getString (Text, text))
                   {
                       setLastError ("Text missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setText (text);
                   offset += 60;
                }


                if (mReturnBits->hasBidPx)
                {
                   uint64_t bidpx;
                   if (!d.getInteger (BidPx, bidpx))
                   {
                       setLastError ("BidPx missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setBidPx (bidpx);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasLargeSize)
                {
                   uint64_t largesize;
                   if (!d.getInteger (LargeSize, largesize))
                   {
                       setLastError ("LargeSize missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLargeSize (largesize);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasLastMkt)
                {
                   string lastmkt;
                   if (!d.getString (LastMkt, lastmkt))
                   {
                       setLastError ("LastMkt missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLastMkt (lastmkt);
                   offset += 4;
                }


                if (mReturnBits->hasFeeCode)
                {
                   string feecode;
                   if (!d.getString (FeeCode, feecode))
                   {
                       setLastError ("FeeCode missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setFeeCode (feecode);
                   offset += 2;
                }


                if (mReturnBits->hasEchoText)
                {
                   string echotext;
                   if (!d.getString (EchoText, echotext))
                   {
                       setLastError ("EchoText missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setEchoText (echotext);
                   offset += 60;
                }


                if (mReturnBits->hasStopPx)
                {
                   uint64_t stoppx;
                   if (!d.getInteger (StopPx, stoppx))
                   {
                       setLastError ("StopPx missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setStopPx (stoppx);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasRoutingInst)
                {
                   string routinginst;
                   if (!d.getString (RoutingInst, routinginst))
                   {
                       setLastError ("RoutingInst missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setRoutingInst (routinginst);
                   offset += 4;
                }


                if (mReturnBits->hasRoutStrategy)
                {
                   string routstrategy;
                   if (!d.getString (RoutStrategy, routstrategy))
                   {
                       setLastError ("RoutStrategy missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setRoutStrategy (routstrategy);
                   offset += 4;
                }


                if (mReturnBits->hasExDestination)
                {
                   string exdestination;
                   if (!d.getString (ExDestination, exdestination))
                   {
                       setLastError ("ExDestination missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setExDestination (exdestination);
                   offset += 1;
                }


                if (mReturnBits->hasTradeReportRefID)
                {
                   string tradereportrefid;
                   if (!d.getString (TradeReportRefID, tradereportrefid))
                   {
                       setLastError ("TradeReportRefID missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTradeReportRefID (tradereportrefid);
                   offset += 20;
                }


                if (mReturnBits->hasMarketingFeeCode)
                {
                   string marketingfeecode;
                   if (!d.getString (MarketingFeeCode, marketingfeecode))
                   {
                       setLastError ("MarketingFeeCode missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setMarketingFeeCode (marketingfeecode);
                   offset += 2;
                }


                if (mReturnBits->hasTargetPartyID)
                {
                   string targetpartyid;
                   if (!d.getString (TargetPartyID, targetpartyid))
                   {
                       setLastError ("TargetPartyID missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTargetPartyID (targetpartyid);
                   offset += 4;
                }


                if (mReturnBits->hasAuctionID)
                {
                   uint64_t auctionid;
                   if (!d.getInteger (AuctionID, auctionid))
                   {
                       setLastError ("AuctionID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAuctionID (auctionid);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasOrderCategory)
                {
                   uint8_t ordercategory;
                   if (!d.getInteger (OrderCategory, ordercategory))
                   {
                       setLastError ("OrderCategory missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrderCategory (ordercategory);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasLiquidityProvision)
                {
                   string liquidityprovision;
                   if (!d.getString (LiquidityProvision, liquidityprovision))
                   {
                       setLastError ("LiquidityProvision missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLiquidityProvision (liquidityprovision);
                   offset += 1;
                }


                if (mReturnBits->hasCmtaNumber)
                {
                   uint32_t cmtanumber;
                   if (!d.getInteger (CmtaNumber, cmtanumber))
                   {
                       setLastError ("CmtaNumber missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCmtaNumber (cmtanumber);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasCrossType)
                {
                   string crosstype;
                   if (!d.getString (CrossType, crosstype))
                   {
                       setLastError ("CrossType missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCrossType (crosstype);
                   offset += 1;
                }


                if (mReturnBits->hasCrossPrioritization)
                {
                   string crossprioritization;
                   if (!d.getString (CrossPrioritization, crossprioritization))
                   {
                       setLastError ("CrossPrioritization missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCrossPrioritization (crossprioritization);
                   offset += 1;
                }


                if (mReturnBits->hasAllocQty)
                {
                   uint32_t allocqty;
                   if (!d.getInteger (AllocQty, allocqty))
                   {
                       setLastError ("AllocQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAllocQty (allocqty);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasRoutingFirmID)
                {
                   string routingfirmid;
                   if (!d.getString (RoutingFirmID, routingfirmid))
                   {
                       setLastError ("RoutingFirmID missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setRoutingFirmID (routingfirmid);
                   offset += 4;
                }


                if (mReturnBits->hasWaiverType)
                {
                   string waivertype;
                   if (!d.getString (WaiverType, waivertype))
                   {
                       setLastError ("WaiverType missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setWaiverType (waivertype);
                   offset += 1;
                }


                if (mReturnBits->hasCrossExclusionIndicator)
                {
                   string crossexclusionindicator;
                   if (!d.getString (CrossExclusionIndicator, crossexclusionindicator))
                   {
                       setLastError ("CrossExclusionIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCrossExclusionIndicator (crossexclusionindicator);
                   offset += 1;
                }


                if (mReturnBits->hasPriceFormation)
                {
                   string priceformation;
                   if (!d.getString (PriceFormation, priceformation))
                   {
                       setLastError ("PriceFormation missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setPriceFormation (priceformation);
                   offset += 1;
                }


                if (mReturnBits->hasClientQualifiedRole)
                {
                   uint8_t clientqualifiedrole;
                   if (!d.getInteger (ClientQualifiedRole, clientqualifiedrole))
                   {
                       setLastError ("ClientQualifiedRole missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setClientQualifiedRole (clientqualifiedrole);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasClientID)
                {
                   uint32_t clientid;
                   if (!d.getInteger (ClientID, clientid))
                   {
                       setLastError ("ClientID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setClientID (clientid);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasInvestorID)
                {
                   uint32_t investorid;
                   if (!d.getInteger (InvestorID, investorid))
                   {
                       setLastError ("InvestorID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setInvestorID (investorid);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasExecutorID)
                {
                   uint32_t executorid;
                   if (!d.getInteger (ExecutorID, executorid))
                   {
                       setLastError ("ExecutorID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setExecutorID (executorid);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasOrderOrigination)
                {
                   string orderorigination;
                   if (!d.getString (OrderOrigination, orderorigination))
                   {
                       setLastError ("OrderOrigination missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrderOrigination (orderorigination);
                   offset += 1;
                }


                if (mReturnBits->hasAlgorithmicIndicator)
                {
                   string algorithmicindicator;
                   if (!d.getString (AlgorithmicIndicator, algorithmicindicator))
                   {
                       setLastError ("AlgorithmicIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAlgorithmicIndicator (algorithmicindicator);
                   offset += 1;
                }


                if (mReturnBits->hasDeferralReason)
                {
                   string deferralreason;
                   if (!d.getString (DeferralReason, deferralreason))
                   {
                       setLastError ("DeferralReason missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDeferralReason (deferralreason);
                   offset += 1;
                }


                if (mReturnBits->hasInvestorQualifiedRole)
                {
                   uint8_t investorqualifiedrole;
                   if (!d.getInteger (InvestorQualifiedRole, investorqualifiedrole))
                   {
                       setLastError ("InvestorQualifiedRole missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setInvestorQualifiedRole (investorqualifiedrole);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasExecutorQualifiedRole)
                {
                   uint8_t executorqualifiedrole;
                   if (!d.getInteger (ExecutorQualifiedRole, executorqualifiedrole))
                   {
                       setLastError ("ExecutorQualifiedRole missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setExecutorQualifiedRole (executorqualifiedrole);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasCtiCode)
                {
                   string cticode;
                   if (!d.getString (CtiCode, cticode))
                   {
                       setLastError ("CtiCode missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCtiCode (cticode);
                   offset += 2;
                }


                if (mReturnBits->hasManualOrderIndicator)
                {
                   uint8_t manualorderindicator;
                   if (!d.getInteger (ManualOrderIndicator, manualorderindicator))
                   {
                       setLastError ("ManualOrderIndicator missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setManualOrderIndicator (manualorderindicator);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasTradeDate)
                {
                   uint64_t tradedate;
                   if (!d.getInteger (TradeDate, tradedate))
                   {
                       setLastError ("TradeDate missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTradeDate (tradedate);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasVariancePrice)
                {
                   uint8_t varianceprice;
                   if (!d.getInteger (VariancePrice, varianceprice))
                   {
                       setLastError ("VariancePrice missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setVariancePrice (varianceprice);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasVarianceSize)
                {
                   uint64_t variancesize;
                   if (!d.getInteger (VarianceSize, variancesize))
                   {
                       setLastError ("VarianceSize missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setVarianceSize (variancesize);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasOrigTASPrice)
                {
                   uint8_t origtasprice;
                   if (!d.getInteger (OrigTASPrice, origtasprice))
                   {
                       setLastError ("OrigTASPrice missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setOrigTASPrice (origtasprice);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasCumQty)
                {
                   uint8_t cumqty;
                   if (!d.getInteger (CumQty, cumqty))
                   {
                       setLastError ("CumQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setCumQty (cumqty);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasDayOrderQty)
                {
                   uint8_t dayorderqty;
                   if (!d.getInteger (DayOrderQty, dayorderqty))
                   {
                       setLastError ("DayOrderQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDayOrderQty (dayorderqty);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasDayCumQty)
                {
                   uint8_t daycumqty;
                   if (!d.getInteger (DayCumQty, daycumqty))
                   {
                       setLastError ("DayCumQty missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDayCumQty (daycumqty);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasAvgPx)
                {
                   uint64_t avgpx;
                   if (!d.getInteger (AvgPx, avgpx))
                   {
                       setLastError ("AvgPx missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setAvgPx (avgpx);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasDayAvgPx)
                {
                   uint64_t dayavgpx;
                   if (!d.getInteger (DayAvgPx, dayavgpx))
                   {
                       setLastError ("DayAvgPx missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setDayAvgPx (dayavgpx);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasLegCFICode)
                {
                   string legcficode;
                   if (!d.getString (LegCFICode, legcficode))
                   {
                       setLastError ("LegCFICode missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLegCFICode (legcficode);
                   offset += 2;
                }


                if (mReturnBits->hasLegMaturityDate)
                {
                   uint64_t legmaturitydate;
                   if (!d.getInteger (LegMaturityDate, legmaturitydate))
                   {
                       setLastError ("LegMaturityDate missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLegMaturityDate (legmaturitydate);
                   offset += sizeof (uint64_t);
                }


                if (mReturnBits->hasLegStrikePrice)
                {
                   uint8_t legstrikeprice;
                   if (!d.getInteger (LegStrikePrice, legstrikeprice))
                   {
                       setLastError ("LegStrikePrice missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setLegStrikePrice (legstrikeprice);
                   offset += sizeof (uint8_t);
                }


                if (mReturnBits->hasSecondaryExecID)
                {
                   uint32_t secondaryexecid;
                   if (!d.getInteger (SecondaryExecID, secondaryexecid))
                   {
                       setLastError ("SecondaryExecID missing or not integer");
                       return GW_CODEC_ERROR;
                   }
                   packet->setSecondaryExecID (secondaryexecid);
                   offset += sizeof (uint32_t);
                }


                if (mReturnBits->hasUsername)
                {
                   string username;
                   if (!d.getString (Username, username))
                   {
                       setLastError ("Username missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setUsername (username);
                   offset += 4;
                }


                if (mReturnBits->hasTradeReportingIndicator)
                {
                   string tradereportingindicator;
                   if (!d.getString (TradeReportingIndicator, tradereportingindicator))
                   {
                       setLastError ("TradeReportingIndicator missing or not string");
                       return GW_CODEC_ERROR;
                   }
                   packet->setTradeReportingIndicator (tradereportingindicator);
                   offset += 1;
                }

                used += offset;
            
        }
    }
    @end
    used += offset;

    return GW_CODEC_SUCCESS;
}

@end

codecState
@{venue}Codec::decode (cdr& d, const void* buf, size_t len, size_t& used)
{

    clearLastError ();
    used = 0;
    d.clear ();

    if (len < sizeof(@{Venue}HeaderPacket))
        return GW_CODEC_SHORT;

    @{Venue}HeaderPacket* hdr = (@{Venue}HeaderPacket*)((unsigned char*)buf);

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;

    d.setInteger (MessageType,  hdr->getMessageType ());
    d.setInteger (MessageLength, hdr->getMessageLength ());
    d.setInteger (MatchingUnit, hdr->getMatchingUnit ());
    d.setInteger (SequenceNumber, hdr->getSequenceNumber ());

    switch (hdr->mMessageType) {
@for message in messages:
        case @{message['id']}:
            return get@{message['name']} (d, buf, used);
@end
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    return GW_CODEC_ERROR;
}

codecState
@{venue}Codec::encode (const cdr& d, void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;

    @{Venue}HeaderPacket* hdr = (@{Venue}HeaderPacket*)((char*)buf);

    @{Venue}HeaderPacket base;
    memcpy (buf, &base, sizeof(base));

    if (len < sizeof(@{Venue}HeaderPacket))
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

    used += sizeof (@{Venue}HeaderPacket);
    codecState state = GW_CODEC_ERROR;
    switch (type) {
@for message in messages:
        case @{message['id']}:
            state = put@{message['name']} (d, buf, len, used);
            break;
@end
        default:
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
    }
    //Message Length does not include first two bytes of message
    hdr->setMessageLength (used - 2);
    return state;
}

} // namespace neueda
