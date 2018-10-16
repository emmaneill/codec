/*
 * Copyright 2014-2018 Neueda
 */

#ifndef ORDER_MSG_BITS_H
#define ORDER_MSG_BITS_H

        //For use with  Order Execution, Acknowledgement, Rejected, Modified, Restated, UserModify Rejected, Cancelled
        struct OrderMsgBits
        {
            //First byte
            unsigned int side : 1;
            unsigned int pegDifference : 1;
            unsigned int price : 1;
            unsigned int execInst : 1;
            unsigned int ordType : 1;
            unsigned int timeInForce : 1;
            unsigned int minQty : 1;
            unsigned int maxRemovePct : 1;
            //Second byte
            unsigned int symbol : 1;
            unsigned int symbolSfx : 1;
            unsigned int currency : 1;
            unsigned int idSource : 1;
            unsigned int securityId : 1;
            unsigned int securityExchange : 1;
            unsigned int capacity : 1;
            unsigned int reserved : 1;
            //Third byte
            unsigned int account : 1;
            unsigned int clearingFirm : 1;
            unsigned int clearingAccount : 1;
            unsigned int displayIndicator : 1;
            unsigned int maxFloor : 1;
            unsigned int discretionAmount : 1;
            unsigned int orderQty : 1;
            unsigned int preventParticipantMatch : 1;
            //Fourth byte
            unsigned int maturityDate : 1;
            unsigned int strikePrice : 1;
            unsigned int putOrCall : 1;
            unsigned int openClose : 1;
            unsigned int clOrdIdBatch : 1;
            unsigned int correctedSize : 1;
            unsigned int partyId : 1;
            unsigned int accessFee : 1;
            //Fifth byte
            unsigned int origClOrdId : 1;
            unsigned int leavesQty : 1;
            unsigned int lastShares : 1;
            unsigned int lastPrice : 1;
            unsigned int displayPrice : 1;
            unsigned int workingPrice : 1;
            unsigned int baseLiquidityIndicator : 1;
            unsigned int expireTime : 1;
            //Sixth byte
            unsigned int secondaryOrderId : 1;
            unsigned int ccp : 1;
            unsigned int contraCapacity : 1;
            unsigned int attributedQuote : 1;
            unsigned int extExecInst : 1;
            unsigned int bulkOrderIds : 1;
            unsigned int bulkRejectReasons : 1;
            unsigned int partyRole : 1;
            //Seventh byte
            unsigned int subLiquidityIndicator : 1;
            unsigned int tradeReportTypeReturn : 1;
            unsigned int tradePublishIndReturn : 1;
            unsigned int text : 1;
            unsigned int bid : 1;
            unsigned int offer : 1;
            unsigned int largeSize : 1;
            unsigned int lastMkt : 1;
           //Eighth byte
            unsigned int feeCode : 1;
            unsigned int echoText : 1;
            unsigned int stopPx : 1;
            unsigned int routingInst : 1;
            unsigned int routStategy : 1;
            unsigned int routeDeliveryMethod : 1;
            unsigned int exDestination : 1;
            unsigned int tradeReportRefId : 1;
           //Ninth byte
            unsigned int marketingFeeCode : 1;
            unsigned int targetPartyId : 1;
            unsigned int auctionId : 1;
            unsigned int orderCategory : 1;
            unsigned int liquidityProvision : 1;
            unsigned int cmtaNumber : 1;
            unsigned int crossType : 1;
            unsigned int crossPrioritization : 1;
            //Tenth byte
            unsigned int crossId : 1;
            unsigned int allocQty : 1;
            unsigned int giveUpFirmId : 1;
            unsigned int routingFirmId : 1;
            unsigned int waiverType : 1;
            unsigned int crossExclusionIndicator : 1;
            unsigned int priceFormation : 1;
            unsigned int clientQualifiedRole : 1;
            //Eleventh byte
            unsigned int clientId : 1;
            unsigned int investorId : 1;
            unsigned int executorId : 1;
            unsigned int orderOrigination : 1;
            unsigned int algorithmicIndicator : 1;
            unsigned int deferralReason : 1;
            unsigned int investorQualifiedRole : 1;
            unsigned int executorQualifiedRole : 1;
            //Twelfth byte
            unsigned int ctiCode : 1;
            unsigned int manualOrderIndicator : 1;
            unsigned int operatorId : 1;
            unsigned int tradeDate : 1;
            unsigned int variancePrice : 1;
            unsigned int varianceSize : 1;
            unsigned int origSymbolId : 1;
            unsigned int origTASPrice : 1;
            //Thirteenth byte
            unsigned int cumQty : 1;
            unsigned int dayOrderQty : 1;
            unsigned int dayCumQty : 1;
            unsigned int avgPx : 1;
            unsigned int dayAvgPx : 1;
            unsigned int pendingStatus : 1;
            unsigned int drillThruProtection : 1;
            unsigned int multilegReportingType : 1;
            //Fourteenth byte
            unsigned int legCFICode : 1;
            unsigned int legMaturityDate : 1;
            unsigned int legStrikePrice : 1;
            unsigned int quoteRoomId : 1;
            unsigned int secondaryExecId : 1;
            unsigned int userRequestId : 1;
            unsigned int username : 1;
            unsigned int userStatus : 1;
            //Fifteenth byte
            unsigned int tradeReportingIndicator : 1;
            unsigned int reserved1  : 1;
            unsigned int reserved2 : 1;
            unsigned int reserved3 : 1;
            unsigned int tradePublishInd : 1;
            unsigned int reportTime :1;
            unsigned int reserved4 :1;
            unsigned int reserved5 : 1;
        };

#endif /* ORDER_MSG_BITS_H */

