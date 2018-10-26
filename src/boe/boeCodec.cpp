/*
 * Copyright 2014-2018 Neueda
 */
/* Boe Version 2.0 */
#include "boeCodec.h"
#include <bitset>
#include <string>
#include <iostream>
#include <fstream>

namespace neueda
{

extern "C" codec*
getCodec ()
{
    return new boeCodec ();
}
codecState
boeCodec::getTradeCancelCorrectV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "CancelCorrectV2");
    BoeTradeCancelCorrectPacket* packet = (BoeTradeCancelCorrectPacket*)((char*)buf + offset);
    offset += sizeof (BoeTradeCancelCorrectPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    d.setInteger (ExecRefID, packet->getExecRefID ());
    d.setString (Side, packet->getSide ());
    d.setString (BaseLiquidityIndicator, packet->getBaseLiquidityIndicator ());
    d.setString (ClearingFirm, packet->getClearingFirm ());
    d.setString (ClearingAccount, packet->getClearingAccount ());
    d.setInteger (LastShares, packet->getLastShares ());
    d.setInteger (LastPx, packet->getLastPx ());
    d.setInteger (CorrectedPrice, packet->getCorrectedPrice ());
    d.setString (OrigTime, packet->getOrigTime ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    //Get optional fields

    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getCancelRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "CancelRejectedV2");
    BoeCancelRejectedPacket* packet = (BoeCancelRejectedPacket*)((char*)buf + offset);
    offset += sizeof (BoeCancelRejectedPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (CancelRejectReason, packet->getCancelRejectReason ());
    d.setString (CancelRejectText, packet->getCancelRejectText ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    //Get optional fields
    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderCancelledV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderCancelledV2");

    BoeOrderCancelledPacket* packet = (BoeOrderCancelledPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderCancelledPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (CancelReason, packet->getCancelReason ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    if (mOrderCancelledBits != NULL)
    {
        //Get optional fields
        char* newBuf = (char*) buf;
    if (mOrderCancelledBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderCancelledBits->hasPegDifference)
    {
        d.setInteger (PegDifference, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasPrice)
    {
        d.setInteger (Price, uint64_t(newBuf[offset]) );
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
        d.setInteger (MinQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }


    if (mOrderCancelledBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, uint8_t(newBuf[offset]) );
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
        d.setInteger (MaxFloor, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasOrderQty)
    {
        d.setInteger (OrderQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }
    if (mOrderCancelledBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }


    if (mOrderCancelledBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }


    if (mOrderCancelledBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasLastShares)
    {
        d.setInteger (LastShares, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderCancelledBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasContraCapacity)
    {
        d.setString (ContraCapacity, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderCancelledBits->hasAttributedQuote)
    {
        d.setInteger (AttributedQuote, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, uint64_t(newBuf[offset]) );
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
        d.setInteger (TradeReportTypeReturn, uint16_t(newBuf[offset]) );
        offset += sizeof (uint16_t);
    }


    if (mOrderCancelledBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }


    if (mOrderCancelledBits->hasBidPx)
    {
        d.setInteger (BidPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasLargeSize)
    {
        d.setInteger (LargeSize, uint64_t(newBuf[offset]) );
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
        d.setInteger (StopPx, uint64_t(newBuf[offset]) );
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
        d.setInteger (AuctionID, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderCancelledBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, uint32_t(newBuf[offset]) );
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
        d.setInteger (AllocQty, uint32_t(newBuf[offset]) );
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
        d.setInteger (ClientQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasClientID)
    {
        d.setInteger (ClientID, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasInvestorID)
    {
        d.setInteger (InvestorID, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderCancelledBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, uint32_t(newBuf[offset]) );
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
        d.setInteger (InvestorQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }


    if (mOrderCancelledBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasTradeDate)
    {
        d.setInteger (TradeDate, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasCumQty)
    {
        d.setInteger (CumQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }

    if (mOrderCancelledBits->hasAvgPx)
    {
        d.setInteger (AvgPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }


    if (mOrderCancelledBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderCancelledBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderCancelledBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, uint32_t(newBuf[offset]) );
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
        d.setInteger (ReportTime, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }

    }

    used += offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getUserModifyRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRejectedV2");
    BoeUserModifyRejectedPacket* packet = (BoeUserModifyRejectedPacket*)((char*)buf + offset);
    offset += sizeof (BoeUserModifyRejectedPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (ModifyRejectReason, packet->getModifyRejectReason ());
    d.setString (ModifyRejectText, packet->getModifyRejectText ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    //Get optional fields

    used +=offset;
    return GW_CODEC_SUCCESS;
}
 
codecState
boeCodec::getOrderRestatementV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRestatedV2");

    BoeOrderRestatedPacket* packet = (BoeOrderRestatedPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderRestatedPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    d.setString (RestatementReason, packet->getRestatementReason ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    //Get optional fields

    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRejectedV2");

    BoeOrderRejectedPacket* packet = (BoeOrderRejectedPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderRejectedPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (OrderRejectReason, packet->getOrderRejectReason ());
    d.setString (OrderRejectText, packet->getOrderRejectText ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    //Get optional fields

    used +=offset;
    return GW_CODEC_SUCCESS;
}

/*codecState
boeCodec::getOrderModifiedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderModifiedV2");

    BoeOrderModifiedPacket* packet = (BoeOrderModifiedPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderModifiedPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;

    if (mOrderModifyBits != NULL)
    {
        //Set optional fields
        char* newBuf = (char*) buf;

        if (mOrderModifyBits->side)
        {
            d.setString(Side,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->pegDifference)
        {
            d.setInteger(PegDifference, int64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderModifyBits->price)
        {
            d.setInteger(Price, int64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderModifyBits->execInst)
        {
            d.setString(ExecInst,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->ordType)
        {
            d.setString(OrdType,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->timeInForce)
        {
            d.setString(TimeInForce,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->minQty)
        {
            d.setInteger(MinQty, uint32_t((newBuf[offset])));
            offset +=4;
        }
        //if (mOrderModifyBits->maxRemovePct) - Cannot be requested
        //Second byte - Cannot be requested
        //Third byte
        if (mOrderModifyBits->account)
        {
            d.setString(Account, getStringField(newBuf, 16, offset));
            offset +=16;
        }
        if (mOrderModifyBits->clearingFirm)
        {
            d.setString(ClearingFirm, getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mOrderModifyBits->clearingAccount)
        {
            d.setString(ClearingAccount, getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mOrderModifyBits->displayIndicator)
        {
            d.setString(DisplayIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->maxFloor)
        {
            d.setInteger(MaxFloor, uint32_t(newBuf[offset]));
            offset +=4;
        }
        //if (mOrderModifyBits->discretionAmount) - Cannot be requested
        if (mOrderModifyBits->orderQty)
        {
            d.setInteger(OrderQty, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->preventParticipantMatch)
        {
            d.setString(PreventParticipantMatch, getStringField(newBuf, 3, offset));
            offset +=3;
        }
        //Fourth byte - cannot be requested
        //Fifth byte
        if (mOrderModifyBits->origClOrdID)
        {
            d.setString(OrigClOrdID, getStringField(newBuf, 20, offset));
            offset +=20;
        }
        if (mOrderModifyBits->leavesQty)
        {
            d.setInteger(LeavesQty, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->lastShares)
        {
            d.setInteger(LastShares, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->lastPrice)
        {
            d.setInteger(LastPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderModifyBits->displayPrice)
        {
            d.setInteger(DisplayPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderModifyBits->workingPrice)
        {
            d.setInteger(WorkingPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderModifyBits->baseLiquidityIndicator)
        {
            d.setString(BaseLiquidityIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->expireTime)
        {
            d.setString(ExpireTime, getStringField(newBuf, 8, offset));
            offset +=8;
        }
        //Sixth byte
        if (mOrderModifyBits->secondaryOrderID)
        {
            d.setInteger(SecondaryOrderID, uint64_t(newBuf[offset]));
            offset +=8;
        }
        //Seventh byte
       //Eighth byte
       //Ninth byte
        if (mOrderModifyBits->liquidityProvision)
        {
            d.setString(LiquidityProvision, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //Tenth byte
        if (mOrderModifyBits->waiverType)
        {
            d.setString(WaiverType, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //Eleventh byte
        if (mOrderModifyBits->clientID)
        {
            d.setInteger(ClientID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->investorID)
        {
            d.setInteger(InvestorID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->executorID)
        {
            d.setInteger(ExecutorID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderModifyBits->orderOrigination)
        {
            d.setString(OrderOrigination, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->algorithmicIndicator)
        {
            d.setString(AlgorithmicIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderModifyBits->investorQualifiedRole)
        {
            d.setInteger(InvestorQualifiedRole, uint8_t(newBuf[offset]));
            offset +=1;
        }
        if (mOrderModifyBits->executorQualifiedRole)
        {
            d.setInteger(ExecutorQualifiedRole, uint8_t(newBuf[offset]));
            offset +=1;
        }
        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifthteenth byte - cannot be requested
    }
    used +=offset;
    return GW_CODEC_SUCCESS;    
}*/

codecState
boeCodec::getOrderAcknowledgementV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderAcknowledgementV2");

    BoeOrderAcknowledgementPacket* packet = (BoeOrderAcknowledgementPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderAcknowledgementPacket) ;

    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger(OrderID, packet->getOrderID ());
    int numBitfields = packet->getNumberOfBitfields ();
    offset += numBitfields;
    //Get optional fields
    if (mOrderAcknowledgementBits != NULL)
    {
        char* newBuf = (char*) buf;

        if (mOrderAcknowledgementBits->hasSide)
        {
            d.setString(Side,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasPegDifference)
        {
            d.setInteger(PegDifference, int64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderAcknowledgementBits->hasPrice)
        {
            d.setInteger(Price, int64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderAcknowledgementBits->hasExecInst)
        {
            d.setString(ExecInst,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasOrdType)
        {
            d.setString(OrdType,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasTimeInForce)
        {
            d.setString(TimeInForce,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasMinQty)
        {
            d.setInteger(MinQty, uint32_t((newBuf[offset])));
            offset +=4;
        }
        //if (mOrderAcknowledgementBits->hasmaxRemovePct) - Cannot be requested
        //Second byte
        if (mOrderAcknowledgementBits->hasSymbol)
        {
            d.setString(Symbol, getStringField(newBuf, 8, offset));
            offset +=8;
        }
        //if (mOrderAcknowledgementBits->hassymbolSfx) - Cannot be requested
        if (mOrderAcknowledgementBits->hasCurrency)
        {
            d.setString(Currency, getStringField(newBuf, 3, offset));
            offset +=3;
        }
        if (mOrderAcknowledgementBits->hasIDSource)
        {
            d.setString(SecurityIDSource,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasSecurityID)
        {
            d.setString(SecurityID, getStringField(newBuf, 16, offset));
            offset +=16;
        }
        if (mOrderAcknowledgementBits->hasSecurityExchange)
        {
            d.setString(SecurityExchange, getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasCapacity)
        {
            d.setString(Capacity, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //    if (mOrderAcknowledgementBits->hasreserved) - Cannot be requested
        //Third byte
        if (mOrderAcknowledgementBits->hasAccount)
        {
            d.setString(Account, getStringField(newBuf, 16, offset));
            offset +=16;
        }
        if (mOrderAcknowledgementBits->hasClearingFirm)
        {
            d.setString(ClearingFirm, getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasClearingAccount)
        {
            d.setString(ClearingAccount, getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasDisplayIndicator)
        {
            d.setString(DisplayIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasMaxFloor)
        {
            d.setInteger(MaxFloor, uint32_t(newBuf[offset]));
            offset +=4;
        }
        //if (mOrderAcknowledgementBits->hasdiscretionAmount) - Cannot be requested
        if (mOrderAcknowledgementBits->hasOrderQty)
        {
            d.setInteger(OrderQty, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasPreventParticipantMatch)
        {
            d.setString(PreventParticipantMatch, getStringField(newBuf, 3, offset));
            offset +=3;
        }
        //Fourth byte - cannot be requested
        //Fifth byte 
        if (mOrderAcknowledgementBits->hasOrigClOrdID)
        {
            d.setString(OrigClOrdID, getStringField(newBuf, 20, offset));
            offset +=20;
        }
        if (mOrderAcknowledgementBits->hasLeavesQty)
        {
            d.setInteger(LeavesQty, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasLastShares)
        {
            d.setInteger(LastShares, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasLastPrice)
        {
            d.setInteger(LastPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderAcknowledgementBits->hasDisplayPrice)
        {
            d.setInteger(DisplayPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderAcknowledgementBits->hasWorkingPrice)
        {
            d.setInteger(WorkingPrice, uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mOrderAcknowledgementBits->hasBaseLiquidityIndicator)
        {
            d.setString(BaseLiquidityIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasExpireTime)
        {
            d.setString(ExpireTime, getStringField(newBuf, 8, offset));
            offset +=8;
        }
        //Sixth byte
        if (mOrderAcknowledgementBits->hasSecondaryOrderID)
        {
            d.setInteger(SecondaryOrderID, uint64_t(newBuf[offset]));
            offset +=8;
        }
        //Seventh byte - cannot be requested
        //Eighth byte - cannot be requested
        //Ninth byte
        if (mOrderAcknowledgementBits->hasLiquidityProvision)
        {
            d.setString(LiquidityProvision, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //Tenth byte
        if (mOrderAcknowledgementBits->hasWaiverType)
        {
            d.setString(WaiverType, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasClientQualifiedRole)
        {
            d.setInteger(ClientQualifiedRole, uint8_t(newBuf[offset]));
            offset +=1;
        }
        //Eleventh byte
        if (mOrderAcknowledgementBits->hasClientID)
        {
            d.setInteger(ClientID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasInvestorID)
        {
            d.setInteger(InvestorID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasExecutorID)
        {
            d.setInteger(ExecutorID, uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mOrderAcknowledgementBits->hasOrderOrigination)
        {
            d.setString(OrderOrigination, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasAlgorithmicIndicator)
        {
            d.setString(AlgorithmicIndicator, getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasInvestorQualifiedRole)
        {
            d.setInteger(InvestorQualifiedRole, uint8_t(newBuf[offset]));
            offset +=1;
        }
        if (mOrderAcknowledgementBits->hasExecutorQualifiedRole)
        {
            d.setInteger(ExecutorQualifiedRole, uint8_t(newBuf[offset]));
            offset +=1;
        }
        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifteenth byte - cannot be requested
    }
    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLogoutResponseV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LogoutResponseV2");

    BoeLogoutResponsePacket* packet = (BoeLogoutResponsePacket*)((char*)buf + offset);
    offset += sizeof (BoeLogoutResponsePacket) ;

    d.setString (LogoutReason, packet->getLogoutReason ());
    d.setString (LogoutReasonText, packet->getLogoutReasonText ());
    d.setInteger (LastReceivedSequenceNumber, packet->getLastReceivedSequenceNumber ());

    // Get optional fields

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLoginResponseV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LoginResponseV2");

    BoeLoginResponsePacket* packet = (BoeLoginResponsePacket*)((char*)buf + offset);
    offset += sizeof (BoeLoginResponsePacket) ;

    d.setString (LoginResponseStatus, packet->getLoginResponseStatus ());
    d.setString (LoginResponseText, packet->getLoginResponseText ());
    d.setInteger (NoUnspecifiedUnitReplay, packet->getNoUnspecifiedUnitReplay ());
    d.setInteger (LastReceivedSequenceNumber, packet->getLastReceivedSequenceNumber ());
    int numUnits = packet->getNumberOfUnits ();

    cdrArray NumUnitsArray;

    for (int i = 0; i < numUnits; ++i)
    {
        cdr u;
        BoeUnitGroupPacket* unitGroup = (BoeUnitGroupPacket*)((char*)buf + offset);
        offset += sizeof (BoeUnitGroupPacket);

        u.setInteger(UnitNumber, unitGroup->getUnitNumber ());
        u.setInteger(UnitSequence, unitGroup->getUnitSequence ());

        NumUnitsArray.push_back (u);
    }
    d.setArray (UnitGroupSection, NumUnitsArray);

    int nParamGroups = ((char*)buf)[offset];
    offset += sizeof(uint8_t);

    cdrArray UnitSequenceParamArray;
    cdrArray ReturnBitfieldsParamArray;

    for(int i = 0; i < nParamGroups; ++i)
    {
        BoeParamGroupHeaderPacket* paramGroupHeader = (BoeParamGroupHeaderPacket*)((char*)buf + offset);
        offset += sizeof(BoeParamGroupHeaderPacket);

        uint16_t paramGroupLength = paramGroupHeader->getParamGroupLength ();
        string paramGroupType =  paramGroupHeader->getParamGroupType ();

        switch (((unsigned char*)paramGroupHeader->mParamGroupType)[0]) 
        {
            //Unit sequences parameter group
            case 0x80:
            {
                cdr UnitSequencesGroup;
                BoeUnitSequencesParamGroupPacket* unitParamGroup = (BoeUnitSequencesParamGroupPacket*)((char*)buf + offset);
                offset += sizeof( BoeUnitSequencesParamGroupPacket);

                UnitSequencesGroup.setInteger(ParameterGroupLength, paramGroupLength);
                UnitSequencesGroup.setString(ParameterGroupType, paramGroupType);
                UnitSequencesGroup.setInteger(NoUnspecifiedReplay, unitParamGroup->getNoUnspecifiedReplay ());
                int numUnits = unitParamGroup->getNumberOfUnits ();

                cdrArray UnitGroupArray;

                for (int j =0; i<numUnits; ++j)
                {
                    cdr u;
                    BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*)((char*)buf + offset);
                    offset += sizeof( BoeUnitGroupPacket);

                    u.setInteger(UnitNumber, unitGroupElement->getUnitNumber ());
                    u.setInteger(UnitSequence, unitGroupElement->getUnitSequence ());

                    UnitGroupArray.push_back (u);
                 }

                 UnitSequencesGroup.setArray(UnitGroupSection, UnitGroupArray);
                 UnitSequenceParamArray.push_back(UnitSequencesGroup);

                break;
            }
            //Return bitfields parameter group
            case 0x81:
            {
                cdr ReturnBitfieldsGroup;
                BoeBitfieldParamGroupPacket* bitfieldGroup = (BoeBitfieldParamGroupPacket*)((char*)buf + offset);
                offset += sizeof(BoeBitfieldParamGroupPacket);

                //Set Parameter Group Header Fields
                ReturnBitfieldsGroup.setInteger(ParameterGroupLength, paramGroupLength);
                ReturnBitfieldsGroup.setString(ParameterGroupType, paramGroupType);

                ReturnBitfieldsGroup.setInteger(MessageType,  bitfieldGroup->getMessageType ());

//                unsigned char msgType = (((unsigned char*)bitfieldGroup->mMessageType)[0]) ;
                int numBitfields = bitfieldGroup->getNumberOfBitfields ();

                cdrArray BitfieldArray;

                //Copy section of buffer with bitfields
                char* tmpbuf = new char[numBitfields];
                memcpy((void*)tmpbuf, (void*)buf+offset, numBitfields);

                for(int j = 0; j < numBitfields; ++j)
                {
                    cdr b;
                    BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*)((char*)buf +offset);

                    b.setString(Bitfield, bitfieldElement->getBitfield ());
                    BitfieldArray.push_back(b);

                    offset += sizeof(BoeBitfieldPacket);
                }

                switch(bitfieldGroup->mMessageType)
                {
                    //Order Execution V2
                    case ORDER_EXECUTION:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                        mOrderExecutionBits = (OrderMsgBits*) tmpbuf;
                        break;
                    }
                    case ORDER_ACK:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                        mOrderAcknowledgementBits = (OrderMsgBits*) tmpbuf;
                        break;
                    }
                    case ORDER_MODIFIED:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                        mOrderModifiedBits = (OrderMsgBits*) tmpbuf;
                        break;
                    }
                    case ORDER_RESTATED:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                        mOrderRestatedBits = (OrderMsgBits*) tmpbuf;
                        break;
                    }
                    case ORDER_CANCELLED:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                        mOrderCancelledBits = (OrderMsgBits*) tmpbuf;
                        break;
                    }
                    default:
                        setLastError("Unknown MessageType for Return Bitfields group");
                        return GW_CODEC_ERROR;
                    break;
                }

                ReturnBitfieldsGroup.setArray(BitfieldSection, BitfieldArray);
                ReturnBitfieldsParamArray.push_back(ReturnBitfieldsGroup);
                break;
            }
            default:
                    setLastError("Unknown Parameter Group");
                    return GW_CODEC_ERROR;
                break;
        }
    }

    d.setArray (UnitSequenceParamGroupSection, UnitSequenceParamArray);
    d.setArray (BitfieldParamGroupSection, ReturnBitfieldsParamArray);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getServerHeartbeat (cdr &d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ServerHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getClientHeartbeat (cdr &d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ClientHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getReplayComplete (cdr &d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ReplayComplete");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderExecutionV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderExecutionV2");
    size_t offset = sizeof (BoeHeaderPacket);

    BoeOrderExecutionPacket* packet = (BoeOrderExecutionPacket*)((char*)buf + offset);
    offset += sizeof (BoeOrderExecutionPacket);
    d.setInteger (TransactTime, packet->getTransactTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (ExecID, packet->getExecID ());
    d.setInteger (LastShares, packet->getLastShares ());
    d.setInteger (LastPx, packet->getLastPx ());
    d.setInteger (LeavesQty, packet->getLeavesQty ());
    d.setString (BaseLiquidityIndicator, packet->getBaseLiquidityIndicator ());
    d.setString (SubLiquidityIndicator, packet->getSubLiquidityIndicator ());
    d.setString (ContraBroker, packet->getContraBroker ());
    d.setInteger (NumberOfBitfields, packet->getNumberOfBitfields ());
    int numBitfields = packet->getNumberOfBitfields ();

    cdrArray BitfieldArray;
    //Taken from LoginRequest
    //Copy section of buffer with bitfields
    char* tmpbuf = new char[numBitfields];
    memcpy((void*)tmpbuf, (void*)buf+offset, numBitfields);

    for(int j = 0; j < numBitfields; ++j)
    {
        cdr b;
        BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*)((char*)buf +offset);

        b.setString(Bitfield, bitfieldElement->getBitfield ());
        std::cout << "Bitfield get: " << string(bitfieldElement->getBitfield ()) << std::endl;
        BitfieldArray.push_back(b);

        offset += sizeof(BoeBitfieldPacket);
    }
    //Set the rest of the char array elements to 0
    memset(tmpbuf+numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
    mOrderExecutionBits = (OrderMsgBits*) tmpbuf;
    d.setArray(BitfieldSection, BitfieldArray);

    if (mOrderExecutionBits != NULL)
    {
        //Set optional fields
        char* newBuf = (char*) buf;
    if (mOrderExecutionBits->hasSide)
    {
        d.setString (Side, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderExecutionBits->hasPegDifference)
    {
        d.setInteger (PegDifference, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasPrice)
    {
        d.setInteger (Price, uint64_t(newBuf[offset]) );
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
        d.setInteger (MinQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasSymbol)
    {
        d.setString (Symbol, getStringField (newBuf, 8, offset) );
        offset += 8;
    }


    if (mOrderExecutionBits->hasSymbolSfx)
    {
        d.setInteger (SymbolSfx, uint8_t(newBuf[offset]) );
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
        d.setInteger (MaxFloor, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasOrderQty)
    {
        d.setInteger (OrderQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasPreventParticipantMatch)
    {
        d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset) );
        offset += 3;
    }


    if (mOrderExecutionBits->hasCorrectedSize)
    {
        d.setInteger (CorrectedSize, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasOrigClOrdID)
    {
        d.setString (OrigClOrdID, getStringField (newBuf, 20, offset) );
        offset += 20;
    }


    if (mOrderExecutionBits->hasLeavesQty)
    {
        d.setInteger (LeavesQty, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasLastShares)
    {
        d.setInteger (LastShares, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasDisplayPrice)
    {
        d.setInteger (DisplayPrice, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }
    if (mOrderExecutionBits->hasWorkingPrice)
    {
        d.setInteger (WorkingPrice, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasBaseLiquidityIndicator)
    {
        d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderExecutionBits->hasSecondaryOrderID)
    {
        d.setInteger (SecondaryOrderID, uint64_t(newBuf[offset]) );
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
        d.setInteger (AttributedQuote, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasBulkOrderIDs)
    {
        d.setInteger (BulkOrderIDs, uint64_t(newBuf[offset]) );
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
        d.setInteger (TradeReportTypeReturn, uint16_t(newBuf[offset]) );
        offset += sizeof (uint16_t);
    }


    if (mOrderExecutionBits->hasTradePublishIndReturn)
    {
        d.setInteger (TradePublishIndReturn, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasText)
    {
        d.setString (Text, getStringField (newBuf, 60, offset) );
        offset += 60;
    }


    if (mOrderExecutionBits->hasBidPx)
    {
        d.setInteger (BidPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasLargeSize)
        {
        d.setInteger (LargeSize, uint64_t(newBuf[offset]) );
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
        d.setInteger (StopPx, uint64_t(newBuf[offset]) );
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
        d.setInteger (AuctionID, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasOrderCategory)
    {
        d.setInteger (OrderCategory, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasLiquidityProvision)
    {
        d.setString (LiquidityProvision, getStringField (newBuf, 1, offset) );
        offset += 1;
    }


    if (mOrderExecutionBits->hasCmtaNumber)
    {
        d.setInteger (CmtaNumber, uint32_t(newBuf[offset]) );
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
        d.setInteger (AllocQty, uint32_t(newBuf[offset]) );
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
        d.setInteger (ClientQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasClientID)
    {
        d.setInteger (ClientID, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasInvestorID)
    {
        d.setInteger (InvestorID, uint32_t(newBuf[offset]) );
        offset += sizeof (uint32_t);
    }


    if (mOrderExecutionBits->hasExecutorID)
    {
        d.setInteger (ExecutorID, uint32_t(newBuf[offset]) );
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
        d.setInteger (InvestorQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasExecutorQualifiedRole)
    {
        d.setInteger (ExecutorQualifiedRole, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasCtiCode)
    {
        d.setString (CtiCode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }


    if (mOrderExecutionBits->hasManualOrderIndicator)
    {
        d.setInteger (ManualOrderIndicator, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }
    if (mOrderExecutionBits->hasTradeDate)
    {
        d.setInteger (TradeDate, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasVariancePrice)
    {
        d.setInteger (VariancePrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasVarianceSize)
    {
        d.setInteger (VarianceSize, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasOrigTASPrice)
    {
        d.setInteger (OrigTASPrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasCumQty)
    {
        d.setInteger (CumQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasDayOrderQty)
    {
        d.setInteger (DayOrderQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasDayCumQty)
    {
        d.setInteger (DayCumQty, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasAvgPx)
    {
        d.setInteger (AvgPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasDayAvgPx)
    {
        d.setInteger (DayAvgPx, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasLegCFICode)
    {
        d.setString (LegCFICode, getStringField (newBuf, 2, offset) );
        offset += 2;
    }


    if (mOrderExecutionBits->hasLegMaturityDate)
    {
        d.setInteger (LegMaturityDate, uint64_t(newBuf[offset]) );
        offset += sizeof (uint64_t);
    }


    if (mOrderExecutionBits->hasLegStrikePrice)
    {
        d.setInteger (LegStrikePrice, uint8_t(newBuf[offset]) );
        offset += sizeof (uint8_t);
    }


    if (mOrderExecutionBits->hasSecondaryExecID)
    {
        d.setInteger (SecondaryExecID, uint32_t(newBuf[offset]) );
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
    }

    used +=offset;

    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::putOrderExecutionV2 (const cdr &d, void* buf,  size_t len, size_t& used)
{
    //BoeOrderExecutionPacket base;
    //memcpy(buf, &base, sizeof(base));

    size_t offset = sizeof (BoeHeaderPacket);
    BoeOrderExecutionPacket* packet = (BoeOrderExecutionPacket*)((char*)buf + offset);

    if (len < sizeof (BoeOrderExecutionPacket))
        return GW_CODEC_SHORT;
    uint64_t transactionTime;
    if (!d.getInteger (TransactTime, transactionTime))
    {
        setLastError("TransactTime is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setTransactTime(transactionTime);
    offset += sizeof(uint64_t);

    string clOrdID;
    if (!d.getString (ClOrdID, clOrdID))
    {
        setLastError("ClOrdID is missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setClOrdID(clOrdID);
    offset += 20;

    uint64_t execID;
    if (!d.getInteger (ExecID, execID))
    {
        setLastError("ExecID is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setExecID(execID);
    offset += sizeof(uint64_t);

    uint32_t lastShares;
    if (!d.getInteger (LastShares, lastShares))
    {
        setLastError("LastShares is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastShares(lastShares);
    offset += sizeof(uint32_t);

    uint64_t lastPx;
    if (!d.getInteger (LastPx, lastPx))
    {
        setLastError("LastPx is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLastPx(lastPx);
    offset += sizeof(uint64_t);

    uint32_t leavesQty;
    if (!d.getInteger (LeavesQty, leavesQty))
    {
        setLastError("LeavesQty is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setLeavesQty(leavesQty);
    offset +=sizeof(uint32_t);

    string baseLiquidityIndicator;
    if (!d.getString (BaseLiquidityIndicator, baseLiquidityIndicator))
    {
        setLastError("BaseLiquidityIndicator is missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setBaseLiquidityIndicator(baseLiquidityIndicator);
    offset += 1;

    string subLiquidityIndicator;
    if (!d.getString (SubLiquidityIndicator, subLiquidityIndicator))
    {
        setLastError("SubLiquidityIndicator is missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setSubLiquidityIndicator(subLiquidityIndicator);
    offset += 1;

    string contraBroker;
    if (!d.getString (ContraBroker, contraBroker))
    {
        setLastError("ContraBroker is missing or not string");
        return GW_CODEC_ERROR;
    }
    packet->setContraBroker(contraBroker);
    offset += 4;

    //padding for ReservedInternal
    offset += sizeof(uint8_t);
    uint8_t numBitfields;
    if (!d.getInteger (NumberOfBitfields, numBitfields))
    {
        setLastError("NumberOfBitfields is missing or not integer");
        return GW_CODEC_ERROR;
    }
    packet->setNumberOfBitfields(numBitfields);
    offset += sizeof(uint8_t);

    std::cout << "Array Size: "  << d.getArraySize (BitfieldSection) << std::endl;
    if(d.getArraySize (BitfieldSection) > 0 && d.getArraySize (BitfieldSection) == numBitfields)
    {
        cdrArray* BitfieldArray = NULL;
        d.getArray (BitfieldSection, (const cdrArray**)(&BitfieldArray));
        for (cdrArray::iterator firstIt = BitfieldArray->begin(); firstIt != BitfieldArray->end(); ++firstIt)
        {
            cdr& item = *firstIt;
            BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*)((char*)buf +offset);
            string bitfield;
            if (!item.getString (Bitfield, bitfield))
            {
                setLastError("Bitfield is missing or not integer");
                return GW_CODEC_ERROR;
            }
            std::cout << "BitfieldElement: " << bitfield << std::endl;
            bitfieldElement->setBitfield (bitfield);
            offset+=sizeof(BoeBitfieldPacket);
        }
    }
    //Taken from LoginRequest

    //Set optional fields
    if (mOrderExecutionBits->hasSide)
    {
        std::string side;
        if(!d.getString(Side,  side))
        {
            setLastError("Side is missing or not string");
            return GW_CODEC_ERROR;
        }
        //xecBitfields.push_back(side)
        offset +=1;
    }
    if (mOrderExecutionBits->hasPegDifference)
    {
        uint64_t pegDifference;
        if(!d.getInteger(PegDifference,  pegDifference))
        {
            setLastError("PegDifference is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=8;
    }
    if (mOrderExecutionBits->hasPrice)
    {
        uint64_t price;
        if(!d.getInteger(Price,  price))
        {
            setLastError("Price is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=8;
    }
    if (mOrderExecutionBits->hasExecInst)
    {
        std::string execInst;
        if(!d.getString(ExecInst,  execInst))
        {
            setLastError("ExecInst is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasOrdType)
    {
        std::string ordType;
        if(!d.getString(OrdType,  ordType))
        {
            setLastError("OrdType is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasTimeInForce)
    {
        std::string timeInForce;
        if(!d.getString(TimeInForce,  timeInForce))
        {
            setLastError("TimeInForce is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasMinQty)
    {
        uint32_t minQty;
        if(!d.getInteger(MinQty,  minQty))
        {
            setLastError("MinQty is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    //if (mOrderExecutionBits->maxRemovePct) - Cannot be requested
    //Second byte
    if (mOrderExecutionBits->hasSymbol)
    {
        std::string symbol;
        if(!d.getString(Symbol,  symbol))
        {
            setLastError("Symbol is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=8;
    }
    //if (mOrderExecutionBits->symbolSfx) - Cannot be requested
    if (mOrderExecutionBits->hasCurrency)
    {
        std::string currency;
        currency = (char*)buf + offset;
        if(!d.getString(Currency,  currency))
        {
            setLastError("Currency is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=3;
    }
    if (mOrderExecutionBits->hasIDSource)
    {
        std::string securityIDSource;
        if(!d.getString(SecurityIDSource,  securityIDSource))
        {
            setLastError("SecurityIDSource is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasSecurityID)
    {
        std::string securityID;
        if(!d.getString(SecurityID,  securityID))
        {
            setLastError("SecurityID is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=16;
    }
    if (mOrderExecutionBits->hasSecurityExchange)
    {
        std::string securityExchange;
        if(!d.getString(SecurityExchange,  securityExchange))
        {
            setLastError("SecurityExchange is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasCapacity)
    {
        std::string capacity;
        if(!d.getString(Capacity,  capacity))
        {
            setLastError("Capacity is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    //    if (mOrderExecutionBits->reserved) - Cannot be requested
    //Third byte
    if (mOrderExecutionBits->hasAccount)
    {
        std::string account;
        if(!d.getString(Account,  account))
        {
            setLastError("Account is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=16;
    }
    if (mOrderExecutionBits->hasClearingFirm)
    {
        std::string clearingFirm;
        if(!d.getString(ClearingFirm,  clearingFirm))
        {
            setLastError("ClearingFirm is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasClearingAccount)
    {
        std::string clearingAccount;
        if(!d.getString(ClearingAccount,  clearingAccount))
        {
            setLastError("ClearingAccount is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasDisplayIndicator)
    {
        std::string displayIndicator;
        if(!d.getString(DisplayIndicator,  displayIndicator))
        {
            setLastError("DisplayIndicator is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasMaxFloor)
    {
        uint32_t maxFloor;
        if(!d.getInteger(MaxFloor,  maxFloor))
        {
            setLastError("MaxFloor is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    //if (mOrderExecutionBits->discretionAmount) - Cannot be requested
    if (mOrderExecutionBits->hasOrderQty)
    {
        uint32_t orderQty;
        if(!d.getInteger(OrderQty,  orderQty))
        {
            setLastError("OrderQty is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasPreventParticipantMatch)
    {
        std::string preventParticipantMatch;
        if(!d.getString(PreventParticipantMatch,  preventParticipantMatch))
        {
            setLastError("PreventParticipantMatch is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=3;
    }
    //Fourth byte - cannot be requested
    //Fifth byte - cannot be requested
    //Sixth byte
    if (mOrderExecutionBits->hasSecondaryOrderID)
    {
        uint64_t secondaryOrderID;
        if(!d.getInteger(SecondaryOrderID,  secondaryOrderID))
        {
            setLastError("SecondaryOrderID is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=8;
    }
    if (mOrderExecutionBits->hasCcp)
    {
        std::string ccp;
        if(!d.getString(Ccp,  ccp))
        {
            setLastError("Ccp is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    //Seventh byte
    if (mOrderExecutionBits->hasLastMkt)
    {
        std::string lastMkt;
        if(!d.getString(LastMkt,  lastMkt))
        {
            setLastError("LastMkt is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
   //Eighth byte
    if (mOrderExecutionBits->hasFeeCode)
    {
        std::string feeCode;
        if(!d.getString(FeeCode,  feeCode))
        {
            setLastError("FeeCode is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=2;
    }
    if (mOrderExecutionBits->hasRoutingInst)
    {
        std::string routingInst;
        if(!d.getString(RoutingInst,  routingInst))
        {
            setLastError("RoutingInst is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
   //Ninth byte
    if (mOrderExecutionBits->hasLiquidityProvision)
    {
        std::string liquidityProvision;
        if(!d.getString(LiquidityProvision, liquidityProvision))
        {
            setLastError("LiquidityProvision is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    //Tenth byte
    if (mOrderExecutionBits->hasWaiverType)
    {
        std::string waiverType;
        if(!d.getString(WaiverType, waiverType))
        {
            setLastError("WaiverType is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasClientQualifiedRole)
    {
        uint8_t clientQualifiedRole;
        if(!d.getInteger(ClientQualifiedRole,  clientQualifiedRole))
        {
            setLastError("ClientQualifiedRole is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    //Eleventh byte
    if (mOrderExecutionBits->hasClientID)
    {
        uint32_t clientID;
        if(!d.getInteger(ClientID,  clientID))
        {
            setLastError("ClientID is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasInvestorID)
    {
        uint32_t investorID;
        if(!d.getInteger(InvestorID,  investorID))
        {
            setLastError("InvestorID is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasExecutorID)
    {
        uint32_t executorID;
        if(!d.getInteger(ExecutorID,  executorID))
        {
            setLastError("ExecutorID is missing or not integer");
            return GW_CODEC_ERROR;
        }
        offset +=4;
    }
    if (mOrderExecutionBits->hasOrderOrigination)
    {
        std::string orderOrigination;
        if(!d.getString(OrderOrigination, orderOrigination))
        {
            setLastError("OrderOrigination is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasAlgorithmicIndicator)
    {
        std::string algorithmicIndicator;
        if(!d.getString(AlgorithmicIndicator, algorithmicIndicator))
        {
            setLastError("AlgorithmicIndicator is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasInvestorQualifiedRole)
    {
        uint8_t investorQualifiedRole;
        if(!d.getInteger(InvestorQualifiedRole, investorQualifiedRole))
        {
            setLastError("InvestorQualifiedRole is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    if (mOrderExecutionBits->hasExecutorQualifiedRole)
    {
        uint8_t executorQualifiedRole;
        if(!d.getInteger(ExecutorQualifiedRole, executorQualifiedRole))
        {
            setLastError("ExecutorQualifiedRole is missing or not string");
            return GW_CODEC_ERROR;
        }
        offset +=1;
    }
    used +=offset;
    return GW_CODEC_SUCCESS;
}
codecState
boeCodec::getLoginRequestV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    //Temporary implementation -  simply moves the buffer pointer
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LoginRequestV2");

    BoeLoginRequestPacket* packet = (BoeLoginRequestPacket*)((char*)buf + offset);
    offset += sizeof (BoeLoginRequestPacket) ;

    cdrArray UnitSequenceParamArray;
    cdrArray ReturnBitfieldsParamArray;

    for(int i = 0; i < packet->getNumberOfParamGroups (); ++i)
    {
        BoeParamGroupHeaderPacket* paramGroupHeader = (BoeParamGroupHeaderPacket*)((char*)buf + offset);
        offset += sizeof(BoeParamGroupHeaderPacket);

        uint16_t paramGroupLength = paramGroupHeader->getParamGroupLength ();
        string paramGroupType =  paramGroupHeader->getParamGroupType ();

        switch (((unsigned char*)paramGroupHeader->mParamGroupType)[0])
        {
            case 0x80:
            {
                cdr UnitSequencesGroup;
                BoeUnitSequencesParamGroupPacket* unitParamGroup = (BoeUnitSequencesParamGroupPacket*)((char*)buf + offset);
                offset += sizeof( BoeUnitSequencesParamGroupPacket);

                UnitSequencesGroup.setInteger(ParameterGroupLength, paramGroupLength);
                UnitSequencesGroup.setString(ParameterGroupType, paramGroupType);
                UnitSequencesGroup.setInteger(NoUnspecifiedReplay, unitParamGroup->getNoUnspecifiedReplay ());
                int numUnits = unitParamGroup->getNumberOfUnits ();

                cdrArray UnitGroupArray;
                
                for (int j =0; i<numUnits; ++j)
                {
                    cdr u;
                    BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*)((char*)buf + offset);
                    offset += sizeof( BoeUnitGroupPacket);

                    u.setInteger(UnitNumber, unitGroupElement->getUnitNumber ());
                    u.setInteger(UnitSequence, unitGroupElement->getUnitSequence ());

                    UnitGroupArray.push_back (u);
                 }

                 UnitSequencesGroup.setArray(UnitGroupSection, UnitGroupArray);
                 UnitSequenceParamArray.push_back(UnitSequencesGroup);

                break;
            }
            case 0x81:
            {
                cdr ReturnBitfieldsGroup;
                BoeBitfieldParamGroupPacket* bitfieldGroup = (BoeBitfieldParamGroupPacket*)((char*)buf + offset);
                offset += sizeof(BoeBitfieldParamGroupPacket);

                //Set Parameter Group Header Fields
                ReturnBitfieldsGroup.setInteger(ParameterGroupLength, paramGroupLength);
                ReturnBitfieldsGroup.setString(ParameterGroupType, paramGroupType);

                ReturnBitfieldsGroup.setInteger(MessageType,  bitfieldGroup->getMessageType ());

///                unsigned char msgType = (((unsigned char*)bitfieldGroup->mMessageType)[0]) ;
                int numBitfields = bitfieldGroup->getNumberOfBitfields ();

                cdrArray BitfieldArray;

                //Copy section of buffer with bitfields
                char* tmpbuf = new char[numBitfields];
                memcpy((void*)tmpbuf, (void*)buf+offset, numBitfields);

                for(int j = 0; j < numBitfields; ++j)
                {
                    cdr b;
                    BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*)((char*)buf +offset);

                    b.setString(Bitfield, bitfieldElement->getBitfield ());
                    BitfieldArray.push_back(b);

                    offset += sizeof(BoeBitfieldPacket);
                }
                switch(bitfieldGroup->mMessageType)
                {
                    //New Order V2
                    case NEW_ORDER:
                    {
                        //Set the rest of the char array elements to 0
                        memset(tmpbuf+numBitfields, 0, (NEW_ORDER_BITFIELDS_SIZE - numBitfields));
                        mNewOrderBits = (NewOrderBits*) tmpbuf;
                        break;
                    }
                    default:
                        break;
                }

                ReturnBitfieldsGroup.setArray(BitfieldSection, BitfieldArray);
                ReturnBitfieldsParamArray.push_back(ReturnBitfieldsGroup);
                break;
            }
            default: 
                setLastError ("Unknown param grouptype");
                return GW_CODEC_ERROR;
        }

    }

    d.setArray (UnitSequenceParamGroupSection, UnitSequenceParamArray);
    d.setArray (BitfieldParamGroupSection, ReturnBitfieldsParamArray);

    used +=offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getNewOrderV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "NewOrderV2");

    BoeNewOrderPacket* packet = (BoeNewOrderPacket*)((char*)buf + offset);
    offset += sizeof (BoeNewOrderPacket) ;

    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (Side, packet->getSide ());
    d.setInteger (OrderQty, packet->getOrderQty ());
    int numBitfields = packet->getNumberOfBitfields ();

    char* tmpbuf = new char[numBitfields];
    memcpy((void*)tmpbuf, (void*)buf+offset, numBitfields);
    memset(tmpbuf+numBitfields, 0, (NEW_ORDER_BITFIELDS_SIZE - numBitfields));
    mNewOrderBits = (NewOrderBits*) tmpbuf;
        
    offset += numBitfields;
    //Get optional fields
    if (mNewOrderBits != NULL)
    {
        //Set optional fields 
        char* newBuf = (char*) buf;
        if (mNewOrderBits->hasClearingFirm)
        {
            d.setString(ClearingFirm,  getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mNewOrderBits->hasClearingAccount)
        {
            d.setString(ClearingAccount,  getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mNewOrderBits->hasPrice)
        {
            d.setInteger(Price,  uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mNewOrderBits->hasExecInst)
        {
            d.setString(ExecInst,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasOrdType)
        {
            d.setString(OrdType,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasTimeInForce)
        {
            d.setString(TimeInForce,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasMinQty)
        {
            d.setInteger(MinQty,  uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mNewOrderBits->hasMaxFloor)
        {
            d.setInteger(MaxFloor,  uint32_t(newBuf[offset]));
            offset +=4;
        }
        //Second byte
        if (mNewOrderBits->hasSymbol)
        {
            d.setString(Symbol,  getStringField(newBuf, 8, offset));
            offset +=8;
        }
        if (mNewOrderBits->hasCurrency)
        {
            d.setString(Currency,  getStringField(newBuf, 3, offset));
            offset +=3;
        }
        if (mNewOrderBits->hasIDSource)
        {
            d.setString(IDSource,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasSecurityID)
        {
            d.setString(SecurityID,  getStringField(newBuf, 16, offset));
            offset +=16;
        }
        if (mNewOrderBits->hasSecurityExchange)
        {
            d.setString(SecurityExchange,  getStringField(newBuf, 4, offset));
            offset +=4;
        }
        if (mNewOrderBits->hasCapacity)
        {
            d.setString(Capacity,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasRoutingInst)
        {
            d.setString(RoutingInst,  getStringField(newBuf, 4, offset));
            offset +=4;
        }
        //Third byte
        if (mNewOrderBits->hasAccount)
        {
            d.setString(Account,  getStringField(newBuf, 16, offset));
            offset +=16;
        }
        if (mNewOrderBits->hasDisplayIndicator)
        {
            d.setString(DisplayIndicator,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasPegDifference)
        {
            d.setInteger(PegDifference,  uint64_t(newBuf[offset]));
            offset +=8;
        }
        if (mNewOrderBits->hasPreventParticipantMatch)
        {
            d.setString(PreventParticipantMatch,  getStringField(newBuf, 3, offset));
            offset +=3;
        }
        if (mNewOrderBits->hasExpireTime)
        {
            d.setInteger(ExpireTime,  uint64_t(newBuf[offset]));
            offset +=8;
        }
        //Fourth byte
        if (mNewOrderBits->hasLiquidityProvision)
        {
            d.setString(LiquidityProvision,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //Fifth byte
        if (mNewOrderBits->hasClientID)
        {
            d.setInteger(ClientID,  uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mNewOrderBits->hasInvestorID)
        {
            d.setInteger(InvestorID,  uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mNewOrderBits->hasExecutorID)
        {
            d.setInteger(ExecutorID,  uint32_t(newBuf[offset]));
            offset +=4;
        }
        if (mNewOrderBits->hasOrderOrigination)
        {
            d.setString(OrderOrigination,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        //Sixth byte - cannot be requested
        //Seventh byte
        if (mNewOrderBits->hasAlgorithmicIndicator)
        {
            d.setString(AlgorithmicIndicator,  getStringField(newBuf, 1, offset));
            offset +=1;
        }
        if (mNewOrderBits->hasClientQualifiedRole)
        {
            d.setInteger(ClientQualifiedRole,  uint8_t(newBuf[offset]));
            offset +=1;
        }
        if (mNewOrderBits->hasInvestorQualifiedRole)
        {
            d.setInteger(InvestorQualifiedRole,  uint8_t(newBuf[offset]));
            offset +=1;
        }
        if (mNewOrderBits->hasExecutorQualifiedRole)
        {
            d.setInteger(ExecutorQualifiedRole,  uint8_t(newBuf[offset]));
            offset +=1;
        }
        //Eighth byte - cannot be requested
    }

    used +=offset;
    return GW_CODEC_SUCCESS;
}


/*codecState
boeCodec::putLoginResponseV2 (const cdr &d, BoeHeaderPacket* hdr, size_t len, size_t& used)
{

    return GW_CODEC_SUCCESS;
} */

/*codecState
boeCodec::putHeartbeat (const cdr &d, BoeHeader* hdr, size_t len, size_t& used)
{
    BoeHeartbeat* p = (BoeHeartbeat*)hdr;
    if (len < sizeof *p)
        return GW_CODEC_SHORT;
    used = sizeof *p;


    return GW_CODEC_SUCCESS;
} */


codecState
boeCodec::decode (cdr& d, const void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;
    d.clear ();

    if (len < sizeof(BoeHeaderPacket))
        return GW_CODEC_SHORT;

    BoeHeaderPacket* hdr = (BoeHeaderPacket*)buf;

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;
    /*if (len < (unsigned)hdr->mMessageLength + sizeof(hdr) - 1)
        return GW_CODEC_SHORT;*/

    d.setInteger (MessageType,  hdr->getMessageType ());
    d.setInteger (MessageLength, hdr->getMessageLength ());
    d.setInteger (MatchingUnit, hdr->getMatchingUnit ());
    d.setInteger (SequenceNumber, hdr->getSequenceNumber ());

    //switch (((unsigned char*)hdr->mMessageType)[0])
    switch (hdr->mMessageType)
    {
        case LOGIN_RESPONSE:
            return getLoginResponseV2 (d, hdr, buf, used);
        case SERVER_HEARTBEAT:
            return getServerHeartbeat (d, hdr, used);
        case REPLAY_COMPLETE:
            return getReplayComplete (d, hdr, used);
        case ORDER_EXECUTION:
        {
            //Temp hack - decode and then 
            void* tmpBuffer;
            size_t preserved = 0;
            vector<uint8_t> output(500);
            uint8_t *outputPtr = output.data ();
            codecState state =  getOrderExecutionV2 (d, hdr, buf, used);
            if(state == GW_CODEC_SUCCESS)
            {
                codecState encodeState = encode(d, outputPtr, len, preserved);
                if (encodeState == GW_CODEC_SUCCESS)
                {
                    size_t newUsed = 0;
                    codecState  decodeState = decode(d, outputPtr, len, newUsed);
                    //return decodeState;
                    exit(0);
                }
                else
                {
                    //return state;
                    exit(0);
                }
            }
            else
                //return state;
                exit(0);
            
        }
        case LOGOUT:
            return getLogoutResponseV2 (d, hdr, buf, used);
        case ORDER_ACK:
            return getOrderAcknowledgementV2 (d, hdr, buf, used);
        case ORDER_REJECTED:
            return getOrderRejectedV2 (d, hdr, buf, used);
        case ORDER_MODIFIED:
            return getOrderModifiedV2 (d, hdr, buf, used);
        case ORDER_RESTATED:
            return getOrderRestatementV2 (d, hdr, buf, used);
        case USER_MODIFY_REJECT:
            return getUserModifyRejectedV2 (d, hdr, buf, used);
        case ORDER_CANCELLED:
            return getOrderCancelledV2 (d, hdr, buf, used);
        case ORDER_CANCEL_REJECTED:
            return getCancelRejectedV2 (d, hdr, buf, used);
        case TRADE_CANCEL_CORRECTED:
            return getTradeCancelCorrectV2 (d, hdr, buf, used);
        case LOGIN_REQUEST:
            return getLoginRequestV2 (d, hdr, buf, used);
        case CLIENT_HEARTBEAT:
            return getClientHeartbeat (d, hdr, used);
        case NEW_ORDER:
            return getNewOrderV2 (d, hdr, buf, used);
        default:
        {
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
        }
    }
}

codecState boeCodec::encode (const cdr& d, void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;

    BoeHeaderPacket* hdr = (BoeHeaderPacket*)((char*)buf);

    BoeHeaderPacket base;
    memcpy(buf, &base, sizeof(base));

    if (len < sizeof *hdr)
        return GW_CODEC_SHORT;

    if (!d.isType (MessageType, CDR_INTEGER, 1))
    {
        setLastError ("MessageType missing or not string");
        return GW_CODEC_ERROR;
    }

    uint16_t messageLength;
    if(!d.getInteger (MessageLength, messageLength))
    {
        setLastError("MessageLength is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMessageLength(messageLength);

    uint8_t type;
    if(!d.getInteger (MessageType, type))
    {
        setLastError("MessageType is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMessageType(type);

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;
    /*if (len < (unsigned)hdr->mMessageLength + sizeof(hdr) - 1)
        return GW_CODEC_SHORT;*/

//    offset+=sizeof(uint16_t);
    uint8_t matchingUnit;
    if(!d.getInteger (MatchingUnit, matchingUnit))
    {
        setLastError("MatchingUnit is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMatchingUnit(matchingUnit);
//    offset+=(uint8_t);
    uint32_t sequenceNumber;
    if(!d.getInteger (SequenceNumber, sequenceNumber))
    {
        setLastError("SequenceNumber is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setSequenceNumber(sequenceNumber);
//    offset+=(uint32_t);*/
    used+=(sizeof(BoeHeaderPacket));
    codecState state = GW_CODEC_ERROR;
    switch (type)
    {
        case LOGIN_RESPONSE:
            state = putLoginResponseV2 (d, hdr, len, used);
            break;
        case ORDER_EXECUTION:
            state = putOrderExecutionV2 (d, buf, len, used);
            break;
        default:
        {
            setLastError ("unknown message type");
            return GW_CODEC_ERROR;
        }
    }
    if (state != GW_CODEC_SUCCESS)
        return state;

    hdr->setMessageLength(used - 2);
    return GW_CODEC_SUCCESS;
    }
} // namespace neueda
