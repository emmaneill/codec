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
    hdr->setMessageType(type);

    uint8_t matchingUnit;
    if(!d.getInteger (MatchingUnit, matchingUnit))
    {
        setLastError("MatchingUnit is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setMatchingUnit(matchingUnit);

    uint32_t sequenceNumber;
    if(!d.getInteger (SequenceNumber, sequenceNumber))
    {
        setLastError("SequenceNumber is missing or not integer");
        return GW_CODEC_ERROR;
    }
    hdr->setSequenceNumber(sequenceNumber);

    used+=sizeof(@{Venue}HeaderPacket);
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
    hdr->setMessageLength(used - 2);
    return state;
}

} // namespace neueda
