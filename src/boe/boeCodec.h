/*
 * Copyright 2014-2018 Neueda
 */
#ifndef BOE_CODEC_H
#define BOE_CODEC_H
#include <stdint.h>
#include "cdr.h"
#include "codec.h"
#include "codecHelpers.h"
#include "orderMsgBits.h"
#include "newOrderBits.h"
#include "packets/BoeHeaderPacket.h"
#include "packets/BoePackets.h"
#include "fields.h"

#define LOGIN_RESPONSE 0x24
#define LOGOUT 0x08
#define SERVER_HEARTBEAT 0x09
#define REPLAY_COMPLETE 0x13
#define ORDER_ACK 0x25
#define ORDER_REJECTED 0x26
#define ORDER_MODIFIED 0x27
#define ORDER_RESTATED 0x28
#define USER_MODIFY_REJECT 0x29
#define ORDER_CANCELLED 0x2A
#define ORDER_CANCEL_REJECTED 0x2B
#define ORDER_EXECUTION 0x2C
#define TRADE_CANCEL_CORRECTED 0x2D

#define LOGIN_REQUEST 0x37
#define CLIENT_HEARTBEAT 0x03
#define NEW_ORDER 0x38

#define MIN_MSG_SIZE 10
#define ORDER_MSG_BITFIELDS_SIZE 15
#define NEW_ORDER_BITFIELDS_SIZE 8

namespace neueda
{

class boeCodec: public codec
{
    public:
        boeCodec()
        {
            mMsgTypes.insert(std::make_pair("37", "Login Request"));
            mMsgTypes.insert(std::make_pair("24", "Login Response"));
            mMsgTypes.insert(std::make_pair("25", "Order Acknowledgment"));
            mMsgTypes.insert(std::make_pair("26", "Order Rejected"));
            mMsgTypes.insert(std::make_pair("27", "Order Modified"));
            mMsgTypes.insert(std::make_pair("28", "Order Restatement"));
            mMsgTypes.insert(std::make_pair("29", "User Modify Rejected"));
            mMsgTypes.insert(std::make_pair("03", "Client Heartbeat"));
            mMsgTypes.insert(std::make_pair("08", "Logout Response"));
            mMsgTypes.insert(std::make_pair("09", "Server Heartbeat"));
            mMsgTypes.insert(std::make_pair("2A", "Order Cancelled"));
            mMsgTypes.insert(std::make_pair("2B", "Cancel Rejected"));
            mMsgTypes.insert(std::make_pair("2D", "Cancel Correct"));
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
        codecState getLoginResponseV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);
        codecState putLoginResponseV2 (const cdr &d, BoeHeaderPacket* hdr, size_t len, size_t& used);

        codecState getServerHeartbeat (cdr& d, BoeHeaderPacket* hdr, size_t& used);

        codecState getReplayComplete (cdr& d, BoeHeaderPacket* hdr, size_t& used);

        codecState getLogoutResponseV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderExecutionV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderAcknowledgementV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderModifiedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderRestatementV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getUserModifyRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getOrderCancelledV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getCancelRejectedV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getTradeCancelCorrectV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getLoginRequestV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getNewOrderV2 (cdr &d, BoeHeaderPacket* hdr, const void* buf, size_t& used);

        codecState getClientHeartbeat (cdr& d, BoeHeaderPacket* hdr, size_t& used);

        OrderMsgBits* mOrderExecBits;
        OrderMsgBits* mOrderModifyBits;
        OrderMsgBits* mOrderRestatedBits;
        OrderMsgBits* mOrderAckBits;
        OrderMsgBits* mOrderCancelledBits;
        NewOrderBits* mNewOrderBits;
};

} // namespace neueda


#endif /* BOE_CODEC_H */
