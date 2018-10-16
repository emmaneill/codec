/*
 * Copyright 2014-2018 Neueda
 */
/* Boe Version 2.0 */
#include "boeCodec.h"
#include <bitset>
#include <string>
#include <iostream>

namespace neueda
{

extern "C" codec*
getCodec ()
{
    return new boeCodec ();
}
codecState
boeCodec::getTradeCancelCorrectV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "CancelCorrectV2");
    BoeCancelCorrectPacket* packet = (BoeCancelCorrectPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeCancelCorrectPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
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
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;
    //Get optional fields
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getCancelRejectedV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "CancelRejectedV2");
    BoeCancelRejectedPacket* packet = (BoeCancelRejectedPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeCancelRejectedPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (CancelRejectReason, packet->getCancelRejectReason ());
    d.setString (CancelRejectText, packet->getCancelRejectText ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;
    //Get optional fields
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderCancelledV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderCancelledV2");
    BoeOrderCancelledPacket* packet = (BoeOrderCancelledPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderCancelledPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (CancelReason, packet->getCancelReason ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;

    if (mOrderCancelledBits != NULL)
    {
        //Get optional fields
        char* newBuf = (char*) buf;

        if (mOrderCancelledBits->side)
        {
            d.setString (Side,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->pegDifference)
        {
            d.setInteger (PegDifference, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderCancelledBits->price)
        {
            d.setInteger (Price, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderCancelledBits->execInst)
        {
            d.setString (ExecInst,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->ordType)
        {
            d.setString (OrdType,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->timeInForce)
        {
            d.setString (TimeInForce,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->minQty)
        {
            d.setInteger (MinQty, uint32_t ( (newBuf[offset])));
            offset += 4;
        }

        //if (mOrderCancelledBits->maxRemovePct) - Cannot be requested
        //Second byte
        if (mOrderCancelledBits->symbol)
        {
            d.setString (Symbol, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //if (mOrderCancelledBits->symbolSfx) - Cannot be requested
        if (mOrderCancelledBits->currency)
        {
            d.setString (Currency, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        if (mOrderCancelledBits->idSource)
        {
            d.setString (SecurityIDSource,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->securityId)
        {
            d.setString (SecurityID, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderCancelledBits->securityExchange)
        {
            d.setString (SecurityExchange, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderCancelledBits->capacity)
        {
            d.setString (Capacity, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //    if (mOrderCancelledBits->reserved) - Cannot be requested
        //Third byte
        if (mOrderCancelledBits->account)
        {
            d.setString (Account, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderCancelledBits->clearingFirm)
        {
            d.setString (ClearingFirm, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderCancelledBits->clearingAccount)
        {
            d.setString (ClearingAccount, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderCancelledBits->displayIndicator)
        {
            d.setString (DisplayIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->maxFloor)
        {
            d.setInteger (MaxFloor, uint32_t (newBuf[offset]));
            offset += 4;
        }

        //if (mOrderCancelledBits->discretionAmount) - Cannot be requested
        if (mOrderCancelledBits->orderQty)
        {
            d.setInteger (OrderQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderCancelledBits->preventParticipantMatch)
        {
            d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        //Fourth byte - cannot be requested
        //Fifth byte
        if (mOrderModifyBits->origClOrdId)
        {
            d.setString (OrigClOrdID, getStringField (newBuf, 20, offset));
            offset += 20;
        }

        if (mOrderModifyBits->leavesQty)
        {
            d.setInteger (LeavesQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->lastShares)
        {
            d.setInteger (LastShares, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->lastPrice)
        {
            d.setInteger (LastPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->displayPrice)
        {
            d.setInteger (DisplayPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->workingPrice)
        {
            d.setInteger (WorkingPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->baseLiquidityIndicator)
        {
            d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->expireTime)
        {
            d.setString (ExpireTime, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //Sixth byte
        if (mOrderCancelledBits->secondaryOrderId)
        {
            d.setInteger (SecondaryOrderID, uint64_t (newBuf[offset]));
            offset += 8;
        }

        //Seventh byte - cannot be requested
        //Eighth byte - cannot be requested
        //Ninth byte
        if (mOrderCancelledBits->liquidityProvision)
        {
            d.setString (LiquidityProvision, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Tenth byte
        if (mOrderCancelledBits->waiverType)
        {
            d.setString (WaiverType, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->clientQualifiedRole)
        {
            d.setInteger (ClientQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Eleventh byte
        if (mOrderCancelledBits->clientId)
        {
            d.setInteger (ClientID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderCancelledBits->investorId)
        {
            d.setInteger (InvestorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderCancelledBits->executorId)
        {
            d.setInteger (ExecutorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderCancelledBits->orderOrigination)
        {
            d.setString (OrderOrigination, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->algorithmicIndicator)
        {
            d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderCancelledBits->investorQualifiedRole)
        {
            d.setInteger (InvestorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mOrderCancelledBits->executorQualifiedRole)
        {
            d.setInteger (ExecutorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifteenth byte - cannot be requested
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getUserModifyRejectedV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRejectedV2");
    BoeModifyRejectedPacket* packet = (BoeModifyRejectedPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeModifyRejectedPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (ModifyRejectReason, packet->getModifyRejectReason ());
    d.setString (ModifyRejectText, packet->getModifyRejectText ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;
    //Get optional fields
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderRestatementV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRestatedV2");
    BoeOrderRestatementPacket* packet = (BoeOrderRestatementPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderRestatementPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    d.setString (RestatementReason, packet->getRestatementReason ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;
    //Get optional fields
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderRejectedV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderRejectedV2");
    BoeOrderRejectedPacket* packet = (BoeOrderRejectedPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderRejectedPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (OrderRejectReason, packet->getOrderRejectReason ());
    d.setString (OrderRejectText, packet->getOrderRejectText ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;
    //Get optional fields
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderModifiedV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderModifiedV2");
    BoeOrderModifiedPacket* packet = (BoeOrderModifiedPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderModifiedPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;

    if (mOrderModifyBits != NULL)
    {
        //Set optional fields
        char* newBuf = (char*) buf;

        if (mOrderModifyBits->side)
        {
            d.setString (Side,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->pegDifference)
        {
            d.setInteger (PegDifference, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->price)
        {
            d.setInteger (Price, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->execInst)
        {
            d.setString (ExecInst,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->ordType)
        {
            d.setString (OrdType,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->timeInForce)
        {
            d.setString (TimeInForce,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->minQty)
        {
            d.setInteger (MinQty, uint32_t ( (newBuf[offset])));
            offset += 4;
        }

        //if (mOrderModifyBits->maxRemovePct) - Cannot be requested
        //Second byte - Cannot be requested
        //Third byte
        if (mOrderModifyBits->account)
        {
            d.setString (Account, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderModifyBits->clearingFirm)
        {
            d.setString (ClearingFirm, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderModifyBits->clearingAccount)
        {
            d.setString (ClearingAccount, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderModifyBits->displayIndicator)
        {
            d.setString (DisplayIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->maxFloor)
        {
            d.setInteger (MaxFloor, uint32_t (newBuf[offset]));
            offset += 4;
        }

        //if (mOrderModifyBits->discretionAmount) - Cannot be requested
        if (mOrderModifyBits->orderQty)
        {
            d.setInteger (OrderQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->preventParticipantMatch)
        {
            d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        //Fourth byte - cannot be requested
        //Fifth byte
        if (mOrderModifyBits->origClOrdId)
        {
            d.setString (OrigClOrdID, getStringField (newBuf, 20, offset));
            offset += 20;
        }

        if (mOrderModifyBits->leavesQty)
        {
            d.setInteger (LeavesQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->lastShares)
        {
            d.setInteger (LastShares, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->lastPrice)
        {
            d.setInteger (LastPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->displayPrice)
        {
            d.setInteger (DisplayPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->workingPrice)
        {
            d.setInteger (WorkingPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderModifyBits->baseLiquidityIndicator)
        {
            d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->expireTime)
        {
            d.setString (ExpireTime, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //Sixth byte
        if (mOrderModifyBits->secondaryOrderId)
        {
            d.setInteger (SecondaryOrderID, uint64_t (newBuf[offset]));
            offset += 8;
        }

        //Seventh byte
        //Eighth byte
        //Ninth byte
        if (mOrderModifyBits->liquidityProvision)
        {
            d.setString (LiquidityProvision, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Tenth byte
        if (mOrderModifyBits->waiverType)
        {
            d.setString (WaiverType, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Eleventh byte
        if (mOrderModifyBits->clientId)
        {
            d.setInteger (ClientID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->investorId)
        {
            d.setInteger (InvestorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->executorId)
        {
            d.setInteger (ExecutorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderModifyBits->orderOrigination)
        {
            d.setString (OrderOrigination, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->algorithmicIndicator)
        {
            d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderModifyBits->investorQualifiedRole)
        {
            d.setInteger (InvestorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mOrderModifyBits->executorQualifiedRole)
        {
            d.setInteger (ExecutorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifthteenth byte - cannot be requested
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderAcknowledgementV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "OrderAcknowledgementV2");
    BoeOrderAcknowledgementPacket* packet = (BoeOrderAcknowledgementPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderAcknowledgementPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (OrderID, packet->getOrderID ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;

    //Get optional fields
    if (mOrderAckBits != NULL)
    {
        char* newBuf = (char*) buf;

        if (mOrderAckBits->side)
        {
            d.setString (Side,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->pegDifference)
        {
            d.setInteger (PegDifference, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderAckBits->price)
        {
            d.setInteger (Price, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderAckBits->execInst)
        {
            d.setString (ExecInst,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->ordType)
        {
            d.setString (OrdType,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->timeInForce)
        {
            d.setString (TimeInForce,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->minQty)
        {
            d.setInteger (MinQty, uint32_t ( (newBuf[offset])));
            offset += 4;
        }

        //if (mOrderAckBits->maxRemovePct) - Cannot be requested
        //Second byte
        if (mOrderAckBits->symbol)
        {
            d.setString (Symbol, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //if (mOrderAckBits->symbolSfx) - Cannot be requested
        if (mOrderAckBits->currency)
        {
            d.setString (Currency, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        if (mOrderAckBits->idSource)
        {
            d.setString (SecurityIDSource,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->securityId)
        {
            d.setString (SecurityID, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderAckBits->securityExchange)
        {
            d.setString (SecurityExchange, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderAckBits->capacity)
        {
            d.setString (Capacity, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //    if (mOrderAckBits->reserved) - Cannot be requested
        //Third byte
        if (mOrderAckBits->account)
        {
            d.setString (Account, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderAckBits->clearingFirm)
        {
            d.setString (ClearingFirm, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderAckBits->clearingAccount)
        {
            d.setString (ClearingAccount, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderAckBits->displayIndicator)
        {
            d.setString (DisplayIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->maxFloor)
        {
            d.setInteger (MaxFloor, uint32_t (newBuf[offset]));
            offset += 4;
        }

        //if (mOrderAckBits->discretionAmount) - Cannot be requested
        if (mOrderAckBits->orderQty)
        {
            d.setInteger (OrderQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->preventParticipantMatch)
        {
            d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        //Fourth byte - cannot be requested
        //Fifth byte
        if (mOrderAckBits->origClOrdId)
        {
            d.setString (OrigClOrdID, getStringField (newBuf, 20, offset));
            offset += 20;
        }

        if (mOrderAckBits->leavesQty)
        {
            d.setInteger (LeavesQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->lastShares)
        {
            d.setInteger (LastShares, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->lastPrice)
        {
            d.setInteger (LastPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderAckBits->displayPrice)
        {
            d.setInteger (DisplayPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderAckBits->workingPrice)
        {
            d.setInteger (WorkingPrice, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderAckBits->baseLiquidityIndicator)
        {
            d.setString (BaseLiquidityIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->expireTime)
        {
            d.setString (ExpireTime, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //Sixth byte
        if (mOrderAckBits->secondaryOrderId)
        {
            d.setInteger (SecondaryOrderID, uint64_t (newBuf[offset]));
            offset += 8;
        }

        //Seventh byte - cannot be requested
        //Eighth byte - cannot be requested
        //Ninth byte
        if (mOrderAckBits->liquidityProvision)
        {
            d.setString (LiquidityProvision, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Tenth byte
        if (mOrderAckBits->waiverType)
        {
            d.setString (WaiverType, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->clientQualifiedRole)
        {
            d.setInteger (ClientQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Eleventh byte
        if (mOrderAckBits->clientId)
        {
            d.setInteger (ClientID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->investorId)
        {
            d.setInteger (InvestorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->executorId)
        {
            d.setInteger (ExecutorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderAckBits->orderOrigination)
        {
            d.setString (OrderOrigination, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->algorithmicIndicator)
        {
            d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderAckBits->investorQualifiedRole)
        {
            d.setInteger (InvestorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mOrderAckBits->executorQualifiedRole)
        {
            d.setInteger (ExecutorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifteenth byte - cannot be requested
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLogoutResponseV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LogoutResponseV2");
    BoeLogoutResponsePacket* packet = (BoeLogoutResponsePacket*) ( (char*) buf + offset);
    offset += sizeof (BoeLogoutResponsePacket) ;
    d.setString (LogoutReason, packet->getLogoutReason ());
    d.setString (LogoutReasonText, packet->getLogoutReasonText ());
    d.setInteger (LastReceivedSequenceNumber, packet->getLastReceivedSequenceNumber ());
    // Get optional fields
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLoginResponseV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LoginResponseV2");
    BoeLoginResponsePacket* packet = (BoeLoginResponsePacket*) ( (char*) buf + offset);
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
        BoeUnitGroupPacket* unitGroup = (BoeUnitGroupPacket*) ( (char*) buf + offset);
        offset += sizeof (BoeUnitGroupPacket);
        u.setInteger (UnitNumber, unitGroup->getUnitNumber ());
        u.setInteger (UnitSequence, unitGroup->getUnitSequence ());
        NumUnitsArray.push_back (u);
    }

    d.setArray (UnitGroupSection, NumUnitsArray);
    int nParamGroups = ( (char*) buf) [offset];
    offset += sizeof (uint8_t);
    cdrArray UnitSequenceParamArray;
    cdrArray ReturnBitfieldsParamArray;

    for (int i = 0; i < nParamGroups; ++i)
    {
        BoeParamGroupHeaderPacket* paramGroupHeader = (BoeParamGroupHeaderPacket*) ( (char*) buf + offset);
        offset += sizeof (BoeParamGroupHeaderPacket);
        uint16_t paramGroupLength = paramGroupHeader->getParamGroupLength ();
        string paramGroupType =  paramGroupHeader->getParamGroupType ();

        switch ( ( (unsigned char*) paramGroupHeader->mParamGroupType) [0])
        {
            //Unit sequences parameter group
        case 0x80:
        {
            cdr UnitSequencesGroup;
            BoeUnitSequencesParamGroupPacket* unitParamGroup = (BoeUnitSequencesParamGroupPacket*) ( (char*) buf + offset);
            offset += sizeof (BoeUnitSequencesParamGroupPacket);
            UnitSequencesGroup.setInteger (ParameterGroupLength, paramGroupLength);
            UnitSequencesGroup.setString (ParameterGroupType, paramGroupType);
            UnitSequencesGroup.setInteger (NoUnspecifiedReplay, unitParamGroup->getNoUnspecifiedReplay ());
            int numUnits = unitParamGroup->getNumberOfUnits ();
            cdrArray UnitGroupArray;

            for (int j = 0; i < numUnits; ++j)
            {
                cdr u;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ( (char*) buf + offset);
                offset += sizeof (BoeUnitGroupPacket);
                u.setInteger (UnitNumber, unitGroupElement->getUnitNumber ());
                u.setInteger (UnitSequence, unitGroupElement->getUnitSequence ());
                UnitGroupArray.push_back (u);
            }

            UnitSequencesGroup.setArray (UnitGroupSection, UnitGroupArray);
            UnitSequenceParamArray.push_back (UnitSequencesGroup);
            break;
        }
        //Return bitfields parameter group
        case 0x81:
        {
            cdr ReturnBitfieldsGroup;
            BoeBitfieldParamGroupPacket* bitfieldGroup = (BoeBitfieldParamGroupPacket*) ( (char*) buf + offset);
            offset += sizeof (BoeBitfieldParamGroupPacket);
            //Set Parameter Group Header Fields
            ReturnBitfieldsGroup.setInteger (ParameterGroupLength, paramGroupLength);
            ReturnBitfieldsGroup.setString (ParameterGroupType, paramGroupType);
            ReturnBitfieldsGroup.setInteger (MessageType,  bitfieldGroup->getMessageType ());
            int numBitfields = bitfieldGroup->getNumberOfReturnBitfields ();
            cdrArray BitfieldArray;
            //Copy section of buffer with bitfields
            char* tmpbuf = new char[numBitfields];
            memcpy ( (void*) tmpbuf, (void*) buf + offset, numBitfields);

            for (int j = 0; j < numBitfields; ++j)
            {
                cdr b;
                BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ( (char*) buf + offset);
                b.setInteger (Bitfield, bitfieldElement->getBitfield ());
                BitfieldArray.push_back (b);
                offset += sizeof (BoeBitfieldPacket);
            }

            switch (bitfieldGroup->mMessageType)
            {
                //Order Execution V2
            case ORDER_EXECUTION:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                mOrderExecBits = (OrderMsgBits*) tmpbuf;
                break;
            }
            case ORDER_ACK:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                mOrderAckBits = (OrderMsgBits*) tmpbuf;
                break;
            }
            case ORDER_MODIFIED:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                mOrderModifyBits = (OrderMsgBits*) tmpbuf;
                break;
            }
            case ORDER_RESTATED:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                mOrderRestatedBits = (OrderMsgBits*) tmpbuf;
                break;
            }
            case ORDER_CANCELLED:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (ORDER_MSG_BITFIELDS_SIZE - numBitfields));
                mOrderCancelledBits = (OrderMsgBits*) tmpbuf;
                break;
            }
            default:
                setLastError ("Unknown MessageType for Return Bitfields group");
                return GW_CODEC_ERROR;
                break;
            }

            ReturnBitfieldsGroup.setArray (BitfieldSection, BitfieldArray);
            ReturnBitfieldsParamArray.push_back (ReturnBitfieldsGroup);
            break;
        }
        default:
            setLastError ("Unknown Parameter Group");
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
boeCodec::getServerHeartbeat (cdr& d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ServerHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getClientHeartbeat (cdr& d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ClientHeartbeat");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getReplayComplete (cdr& d, BoeHeaderPacket* hdr, size_t& used)
{
    d.setString (MessageName, "ReplayComplete");
    size_t offset = sizeof (BoeHeaderPacket) ;
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getOrderExecutionV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    d.setString (MessageName, "OrderExecutionV2");
    size_t offset = sizeof (BoeHeaderPacket);
    BoeOrderExecutionPacket* packet = (BoeOrderExecutionPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeOrderExecutionPacket) ;
    d.setInteger (TransactTime, packet->getTransactionTime ());
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setInteger (ExecID, packet->getExecID ());
    d.setInteger (LastShares, packet->getLastShares ());
    d.setInteger (LastPx, packet->getLastPx ());
    d.setInteger (LeavesQty, packet->getLeavesQty ());
    d.setString (BaseLiquidityIndicator, packet->getBaseLiquidityIndicator ());
    d.setString (SubLiquidityIndicator, packet->getSubLiquidityIndicator ());
    d.setString (ContraBroker, packet->getContraBroker ());
    int numBitfields = packet->getNumberOfReturnBitfields ();
    offset += numBitfields;

    if (mOrderExecBits != NULL)
    {
        //Set optional fields
        char* newBuf = (char*) buf;

        if (mOrderExecBits->side)
        {
            d.setString (Side,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->pegDifference)
        {
            d.setInteger (PegDifference, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderExecBits->price)
        {
            d.setInteger (Price, int64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderExecBits->execInst)
        {
            d.setString (ExecInst,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->ordType)
        {
            d.setString (OrdType,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->timeInForce)
        {
            d.setString (TimeInForce,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->minQty)
        {
            d.setInteger (MinQty, uint32_t ( (newBuf[offset])));
            offset += 4;
        }

        //if (mOrderExecBits->maxRemovePct) - Cannot be requested
        //Second byte
        if (mOrderExecBits->symbol)
        {
            d.setString (Symbol, getStringField (newBuf, 8, offset));
            offset += 8;
        }

        //if (mOrderExecBits->symbolSfx) - Cannot be requested
        if (mOrderExecBits->currency)
        {
            d.setString (Currency, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        if (mOrderExecBits->idSource)
        {
            d.setString (SecurityIDSource,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->securityId)
        {
            d.setString (SecurityID, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderExecBits->securityExchange)
        {
            d.setString (SecurityExchange, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderExecBits->capacity)
        {
            d.setString (Capacity, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //    if (mOrderExecBits->reserved) - Cannot be requested
        //Third byte
        if (mOrderExecBits->account)
        {
            d.setString (Account, getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mOrderExecBits->clearingFirm)
        {
            d.setString (ClearingFirm, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderExecBits->clearingAccount)
        {
            d.setString (ClearingAccount, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mOrderExecBits->displayIndicator)
        {
            d.setString (DisplayIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->maxFloor)
        {
            d.setInteger (MaxFloor, uint32_t (newBuf[offset]));
            offset += 4;
        }

        //if (mOrderExecBits->discretionAmount) - Cannot be requested
        if (mOrderExecBits->orderQty)
        {
            d.setInteger (OrderQty, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderExecBits->preventParticipantMatch)
        {
            d.setString (PreventParticipantMatch, getStringField (newBuf, 3, offset));
            offset += 3;
        }

        //Fourth byte - cannot be requested
        //Fifth byte - cannot be requested
        //Sixth byte
        if (mOrderExecBits->secondaryOrderId)
        {
            d.setInteger (SecondaryOrderID, uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mOrderExecBits->ccp)
        {
            d.setString (Ccp, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Seventh byte
        if (mOrderExecBits->lastMkt)
        {
            d.setString (LastMkt, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        //Eighth byte
        if (mOrderExecBits->feeCode)
        {
            d.setString (FeeCode, getStringField (newBuf, 2, offset));
            offset += 2;
        }

        if (mOrderExecBits->routingInst)
        {
            d.setString (RoutingInst, getStringField (newBuf, 4, offset));
            offset += 4;
        }

        //Ninth byte
        if (mOrderExecBits->liquidityProvision)
        {
            d.setString (LiquidityProvision, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Tenth byte
        if (mOrderExecBits->waiverType)
        {
            d.setString (WaiverType, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->clientQualifiedRole)
        {
            d.setInteger (ClientQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Eleventh byte
        if (mOrderExecBits->clientId)
        {
            d.setInteger (ClientID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderExecBits->investorId)
        {
            d.setInteger (InvestorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderExecBits->executorId)
        {
            d.setInteger (ExecutorID, uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mOrderExecBits->orderOrigination)
        {
            d.setString (OrderOrigination, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->algorithmicIndicator)
        {
            d.setString (AlgorithmicIndicator, getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mOrderExecBits->investorQualifiedRole)
        {
            d.setInteger (InvestorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mOrderExecBits->executorQualifiedRole)
        {
            d.setInteger (ExecutorQualifiedRole, uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Twelfth byte - cannot be requested
        //Thirteenth byte - cannot be requested
        //Fourteenth byte - cannot be requested
        //Fifteenth byte - cannot be requested
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getLoginRequestV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "LoginRequestV2");
    BoeLoginRequestPacket* packet = (BoeLoginRequestPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeLoginRequestPacket) ;
    cdrArray UnitSequenceParamArray;
    cdrArray ReturnBitfieldsParamArray;

    for (int i = 0; i < packet->getNumberOfParamGroups (); ++i)
    {
        BoeParamGroupHeaderPacket* paramGroupHeader = (BoeParamGroupHeaderPacket*) ( (char*) buf + offset);
        offset += sizeof (BoeParamGroupHeaderPacket);
        uint16_t paramGroupLength = paramGroupHeader->getParamGroupLength ();
        string paramGroupType =  paramGroupHeader->getParamGroupType ();

        switch ( ( (unsigned char*) paramGroupHeader->mParamGroupType) [0])
        {
        case 0x80:
        {
            cdr UnitSequencesGroup;
            BoeUnitSequencesParamGroupPacket* unitParamGroup = (BoeUnitSequencesParamGroupPacket*) ( (char*) buf + offset);
            offset += sizeof (BoeUnitSequencesParamGroupPacket);
            UnitSequencesGroup.setInteger (ParameterGroupLength, paramGroupLength);
            UnitSequencesGroup.setString (ParameterGroupType, paramGroupType);
            UnitSequencesGroup.setInteger (NoUnspecifiedReplay, unitParamGroup->getNoUnspecifiedReplay ());
            int numUnits = unitParamGroup->getNumberOfUnits ();
            cdrArray UnitGroupArray;

            for (int j = 0; i < numUnits; ++j)
            {
                cdr u;
                BoeUnitGroupPacket* unitGroupElement = (BoeUnitGroupPacket*) ( (char*) buf + offset);
                offset += sizeof (BoeUnitGroupPacket);
                u.setInteger (UnitNumber, unitGroupElement->getUnitNumber ());
                u.setInteger (UnitSequence, unitGroupElement->getUnitSequence ());
                UnitGroupArray.push_back (u);
            }

            UnitSequencesGroup.setArray (UnitGroupSection, UnitGroupArray);
            UnitSequenceParamArray.push_back (UnitSequencesGroup);
            break;
        }
        case 0x81:
        {
            cdr ReturnBitfieldsGroup;
            BoeBitfieldParamGroupPacket* bitfieldGroup = (BoeBitfieldParamGroupPacket*) ( (char*) buf + offset);
            offset += sizeof (BoeBitfieldParamGroupPacket);
            //Set Parameter Group Header Fields
            ReturnBitfieldsGroup.setInteger (ParameterGroupLength, paramGroupLength);
            ReturnBitfieldsGroup.setString (ParameterGroupType, paramGroupType);
            ReturnBitfieldsGroup.setInteger (MessageType,  bitfieldGroup->getMessageType ());
            int numBitfields = bitfieldGroup->getNumberOfReturnBitfields ();
            cdrArray BitfieldArray;
            //Copy section of buffer with bitfields
            char* tmpbuf = new char[numBitfields];
            memcpy ( (void*) tmpbuf, (void*) buf + offset, numBitfields);

            for (int j = 0; j < numBitfields; ++j)
            {
                cdr b;
                BoeBitfieldPacket* bitfieldElement = (BoeBitfieldPacket*) ( (char*) buf + offset);
                b.setInteger (Bitfield, bitfieldElement->getBitfield ());
                BitfieldArray.push_back (b);
                offset += sizeof (BoeBitfieldPacket);
            }

            switch (bitfieldGroup->mMessageType)
            {
                //New Order V2
            case NEW_ORDER:
            {
                //Set the rest of the char array elements to 0
                memset (tmpbuf + numBitfields, 0, (NEW_ORDER_BITFIELDS_SIZE - numBitfields));
                mNewOrderBits = (NewOrderBits*) tmpbuf;
                break;
            }
            default:
                break;
            }

            ReturnBitfieldsGroup.setArray (BitfieldSection, BitfieldArray);
            ReturnBitfieldsParamArray.push_back (ReturnBitfieldsGroup);
            break;
        }
        default:
            setLastError ("Unknown param grouptype");
            return GW_CODEC_ERROR;
        }
    }

    d.setArray (UnitSequenceParamGroupSection, UnitSequenceParamArray);
    d.setArray (BitfieldParamGroupSection, ReturnBitfieldsParamArray);
    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::getNewOrderV2 (cdr& d, BoeHeaderPacket* hdr, const void* buf, size_t& used)
{
    size_t offset = sizeof (BoeHeaderPacket) ;
    d.setString (MessageName, "NewOrderV2");
    BoeNewOrderPacket* packet = (BoeNewOrderPacket*) ( (char*) buf + offset);
    offset += sizeof (BoeNewOrderPacket) ;
    d.setString (ClOrdID, packet->getClOrdID ());
    d.setString (Side, packet->getSide ());
    d.setInteger (OrderQty, packet->getOrderQty ());
    int numBitfields = packet->getNumberOfNewOrderBitfields ();
    char* tmpbuf = new char[numBitfields];
    memcpy ( (void*) tmpbuf, (void*) buf + offset, numBitfields);
    memset (tmpbuf + numBitfields, 0, (NEW_ORDER_BITFIELDS_SIZE - numBitfields));
    mNewOrderBits = (NewOrderBits*) tmpbuf;
    offset += numBitfields;

    //Get optional fields
    if (mNewOrderBits != NULL)
    {
        //Set optional fields
        char* newBuf = (char*) buf;

        if (mNewOrderBits->hasClearingFirm)
        {
            d.setString (ClearingFirm,  getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mNewOrderBits->hasClearingAccount)
        {
            d.setString (ClearingAccount,  getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mNewOrderBits->hasPrice)
        {
            d.setInteger (Price,  uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mNewOrderBits->hasExecInst)
        {
            d.setString (ExecInst,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasOrdType)
        {
            d.setString (OrdType,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasTimeInForce)
        {
            d.setString (TimeInForce,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasMinQty)
        {
            d.setInteger (MinQty,  uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mNewOrderBits->hasMaxFloor)
        {
            d.setInteger (MaxFloor,  uint32_t (newBuf[offset]));
            offset += 4;
        }

        //Second byte
        if (mNewOrderBits->hasSymbol)
        {
            d.setString (Symbol,  getStringField (newBuf, 8, offset));
            offset += 8;
        }

        if (mNewOrderBits->hasCurrency)
        {
            d.setString (Currency,  getStringField (newBuf, 3, offset));
            offset += 3;
        }

        if (mNewOrderBits->hasIdSource)
        {
            d.setString (IDSource,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasSecurityId)
        {
            d.setString (SecurityID,  getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mNewOrderBits->hasSecurityExchange)
        {
            d.setString (SecurityExchange,  getStringField (newBuf, 4, offset));
            offset += 4;
        }

        if (mNewOrderBits->hasCapacity)
        {
            d.setString (Capacity,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasRoutingInst)
        {
            d.setString (RoutingInst,  getStringField (newBuf, 4, offset));
            offset += 4;
        }

        //Third byte
        if (mNewOrderBits->hasAccount)
        {
            d.setString (Account,  getStringField (newBuf, 16, offset));
            offset += 16;
        }

        if (mNewOrderBits->hasDisplayIndicator)
        {
            d.setString (DisplayIndicator,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasPegDifference)
        {
            d.setInteger (PegDifference,  uint64_t (newBuf[offset]));
            offset += 8;
        }

        if (mNewOrderBits->hasPreventParticipantMatch)
        {
            d.setString (PreventParticipantMatch,  getStringField (newBuf, 3, offset));
            offset += 3;
        }

        if (mNewOrderBits->hasExpireTime)
        {
            d.setInteger (ExpireTime,  uint64_t (newBuf[offset]));
            offset += 8;
        }

        //Fourth byte
        if (mNewOrderBits->hasLiquidityProvision)
        {
            d.setString (LiquidityProvision,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Fifth byte
        if (mNewOrderBits->hasClientID)
        {
            d.setInteger (ClientID,  uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mNewOrderBits->hasInvestorID)
        {
            d.setInteger (InvestorID,  uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mNewOrderBits->hasExecutorID)
        {
            d.setInteger (ExecutorID,  uint32_t (newBuf[offset]));
            offset += 4;
        }

        if (mNewOrderBits->hasOrderOrigination)
        {
            d.setString (OrderOrigination,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        //Sixth byte - cannot be requested
        //Seventh byte
        if (mNewOrderBits->hasAlgorithmicIndicator)
        {
            d.setString (AlgorithmicIndicator,  getStringField (newBuf, 1, offset));
            offset += 1;
        }

        if (mNewOrderBits->hasClientQualifiedRole)
        {
            d.setInteger (ClientQualifiedRole,  uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mNewOrderBits->hasInvestorQualifiedRole)
        {
            d.setInteger (InvestorQualifiedRole,  uint8_t (newBuf[offset]));
            offset += 1;
        }

        if (mNewOrderBits->hasExecutorQualifiedRole)
        {
            d.setInteger (ExecutorQualifiedRole,  uint8_t (newBuf[offset]));
            offset += 1;
        }

        //Eighth byte - cannot be requested
    }

    used += offset;
    return GW_CODEC_SUCCESS;
}

codecState
boeCodec::decode (cdr& d, const void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;
    d.clear ();

    if (len < sizeof (BoeHeaderPacket))
        return GW_CODEC_SHORT;

    BoeHeaderPacket* hdr = (BoeHeaderPacket*) buf;

    if (len < sizeof hdr)
        return GW_CODEC_SHORT;

    if (len < (unsigned) hdr->mMessageLength + sizeof (hdr) - 1)
        return GW_CODEC_SHORT;

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
        return getOrderExecutionV2 (d, hdr, buf, used);
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
        setLastError ("unknown message type");
        return GW_CODEC_ERROR;
    }
}

codecState boeCodec::encode (const cdr& d, void* buf, size_t len, size_t& used)
{
    clearLastError ();
    used = 0;
    BoeHeaderPacket* hdr = (BoeHeaderPacket*) buf;

    if (len < sizeof * hdr)
        return GW_CODEC_SHORT;

    if (!d.isType (MessageType, CDR_INTEGER, 1))
    {
        setLastError ("MessageType missing or not string");
        return GW_CODEC_ERROR;
    }

    uint8_t type;
    d.getInteger (MessageType, type);
    codecState state;

    switch (type)
    {
    case LOGIN_RESPONSE:
        state = putLoginResponseV2 (d, hdr, len, used);
        break;
    case ORDER_EXECUTION:
        state = putOrderExecutionV2 (d, hdr, buf, len, used);
    default:
    {
        setLastError ("unknown message type");
        return GW_CODEC_ERROR;
    }
    }

    if (state != GW_CODEC_SUCCESS)
        return state;

    hdr->mMessageLength = used - (sizeof (*hdr) - 1);
    return GW_CODEC_SUCCESS;
}

} // namespace neueda
