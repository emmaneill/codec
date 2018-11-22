/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated 13:42:16 22/11/18
 */
#ifndef BOE_CODEC_H
#define BOE_CODEC_H

#include <stdint.h>
#include "cdr.h"
#include "codec.h"
#include "codecHelpers.h"
#include "orderMsgBits.h"
#include "newOrderBits.h"
#include "cancelOrderBits.h"
#include "modifyOrderBits.h"
#include "tradeCaptureBits.h"
#include "packets/BoeHeaderPacket.h"
#include "packets/BoePackets.h"

#define MIN_MSG_SIZE 10
#define NEWORDER_BITFIELDS_SIZE 8
#define CANCELORDER_BITFIELDS_SIZE 2
#define MODIFYORDER_BITFIELDS_SIZE 2
#define TRADECAPTUREREPORT_BITFIELDS_SIZE 5
#define ORDERACKNOWLEDGEMENT_BITFIELDS_SIZE 15
#define ORDERREJECTED_BITFIELDS_SIZE 15
#define ORDERMODIFIED_BITFIELDS_SIZE 15
#define ORDERRESTATED_BITFIELDS_SIZE 15
#define USERMODIFYREJECTED_BITFIELDS_SIZE 15
#define ORDERCANCELLED_BITFIELDS_SIZE 15
#define CANCELREJECTED_BITFIELDS_SIZE 15
#define ORDEREXECUTION_BITFIELDS_SIZE 15
#define TRADECANCELCORRECT_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTACKNOWLEDGEMENT_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTREJECT_BITFIELDS_SIZE 15
#define TRADECAPTURECONFIRM_BITFIELDS_SIZE 15
#define TRADECAPTUREREPORTDECLINE_BITFIELDS_SIZE 15

namespace neueda
{

class boeCodec: public codec
{
    public:
        boeCodec()
        {
            mMsgTypes.insert(std::make_pair("0x37", "LoginRequest"));
            mMsgTypes.insert(std::make_pair("0x24", "LoginResponse"));
            mMsgTypes.insert(std::make_pair("0x08", "LogoutResponse"));
            mMsgTypes.insert(std::make_pair("0x09", "ServerHeartbeat"));
            mMsgTypes.insert(std::make_pair("0x03", "ClientHeartbeat"));
            mMsgTypes.insert(std::make_pair("0x13", "ReplayComplete"));
            mMsgTypes.insert(std::make_pair("0x38", "NewOrder"));
            mMsgTypes.insert(std::make_pair("0x39", "CancelOrder"));
            mMsgTypes.insert(std::make_pair("0x3A", "ModifyOrder"));
            mMsgTypes.insert(std::make_pair("0x3C", "TradeCaptureReport"));
            mMsgTypes.insert(std::make_pair("0x25", "OrderAcknowledgement"));
            mMsgTypes.insert(std::make_pair("0x26", "OrderRejected"));
            mMsgTypes.insert(std::make_pair("0x27", "OrderModified"));
            mMsgTypes.insert(std::make_pair("0x28", "OrderRestated"));
            mMsgTypes.insert(std::make_pair("0x29", "UserModifyRejected"));
            mMsgTypes.insert(std::make_pair("0x2A", "OrderCancelled"));
            mMsgTypes.insert(std::make_pair("0x2B", "CancelRejected"));
            mMsgTypes.insert(std::make_pair("0x2C", "OrderExecution"));
            mMsgTypes.insert(std::make_pair("0x2D", "TradeCancelCorrect"));
            mMsgTypes.insert(std::make_pair("0x30", "TradeCaptureReportAcknowledgement"));
            mMsgTypes.insert(std::make_pair("0x31", "TradeCaptureReportReject"));
            mMsgTypes.insert(std::make_pair("0x32", "TradeCaptureConfirm"));
            mMsgTypes.insert(std::make_pair("0x33", "TradeCaptureReportDecline"));
        }

        codecState decode (cdr& d,
                             const void* buf,
                             size_t len,
                             size_t& used);
        codecState encode (const cdr& d,
                             void* buf,
                             size_t len,
                             size_t& used);
        std::string getType () const { return "boe"; }

        std::string getStringField(char* buf, int len, size_t& pos)
        {
            std::string c = "";
            c.assign((const char*)buf+pos, len);
            return c;
        }

    private:
        codecState getLoginRequest (cdr& d, const void* buf, size_t& used);
        codecState getLoginResponse (cdr& d, const void* buf, size_t& used);
        codecState getLogoutResponse (cdr& d, const void* buf, size_t& used);
        codecState getServerHeartbeat (cdr& d, const void* buf, size_t& used);
        codecState getClientHeartbeat (cdr& d, const void* buf, size_t& used);
        codecState getReplayComplete (cdr& d, const void* buf, size_t& used);
        codecState getNewOrder (cdr& d, const void* buf, size_t& used);
        codecState getCancelOrder (cdr& d, const void* buf, size_t& used);
        codecState getModifyOrder (cdr& d, const void* buf, size_t& used);
        codecState getTradeCaptureReport (cdr& d, const void* buf, size_t& used);
        codecState getOrderAcknowledgement (cdr& d, const void* buf, size_t& used);
        codecState getOrderRejected (cdr& d, const void* buf, size_t& used);
        codecState getOrderModified (cdr& d, const void* buf, size_t& used);
        codecState getOrderRestated (cdr& d, const void* buf, size_t& used);
        codecState getUserModifyRejected (cdr& d, const void* buf, size_t& used);
        codecState getOrderCancelled (cdr& d, const void* buf, size_t& used);
        codecState getCancelRejected (cdr& d, const void* buf, size_t& used);
        codecState getOrderExecution (cdr& d, const void* buf, size_t& used);
        codecState getTradeCancelCorrect (cdr& d, const void* buf, size_t& used);
        codecState getTradeCaptureReportAcknowledgement (cdr& d, const void* buf, size_t& used);
        codecState getTradeCaptureReportReject (cdr& d, const void* buf, size_t& used);
        codecState getTradeCaptureConfirm (cdr& d, const void* buf, size_t& used);
        codecState getTradeCaptureReportDecline (cdr& d, const void* buf, size_t& used);

        codecState putLoginRequest (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putLoginResponse (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putLogoutResponse (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putServerHeartbeat (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putClientHeartbeat (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putReplayComplete (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putNewOrder (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putCancelOrder (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putModifyOrder (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCaptureReport (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderAcknowledgement (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderRejected (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderModified (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderRestated (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putUserModifyRejected (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderCancelled (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putCancelRejected (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putOrderExecution (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCancelCorrect (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCaptureReportAcknowledgement (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCaptureReportReject (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCaptureConfirm (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);
        codecState putTradeCaptureReportDecline (const cdr& d,
                                    void* hdr,
                                    size_t len,
                                    size_t& used);


        OrderExecutionBits* mOrderExecutionBits;
        OrderModifiedBits* mOrderModifiedBits;
        OrderRejectedBits* mOrderRejectedBits;
        OrderRestatedBits* mOrderRestatedBits;
        OrderAcknowledgementBits* mOrderAcknowledgementBits;
        OrderCancelledBits* mOrderCancelledBits;
        UserModifyRejectedBits* mUserModifyRejectedBits;
        CancelRejectedBits* mCancelRejectedBits;
        TradeCancelCorrectBits* mTradeCancelCorrectBits;
        TradeCaptureReportRejectBits* mTradeCaptureReportRejectBits;
        TradeCaptureConfirmBits* mTradeCaptureConfirmBits;
        TradeCaptureReportDeclineBits* mTradeCaptureReportDeclineBits;
        TradeCaptureReportAcknowledgementBits* mTradeCaptureReportAcknowledgementBits;
        NewOrderBits* mNewOrderBits;
        CancelOrderBits* mCancelOrderBits;
        ModifyOrderBits* mModifyOrderBits;
        TradeCaptureReportBits* mTradeCaptureReportBits;


};

} // namespace neueda

#endif /* BOE_CODEC_H */
