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
    @{Venue}@{msg['name']}Packet* packet = (@{Venue}@{msg['name']}Packet*)((char*)buf + offset);
    offset += sizeof (@{Venue}@{msg['name']}Packet);
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
    @{venue}@{msg['name']}Packet base;
    memcpy (buf, &base, sizeof(base));

    @if len(msg_data[msg['name']]) > 0:
    @{venue}@{msg['name']}Packet* packet = (@{venue}@{msg['name']}Packet*)((char*)buf);
    @end
    size_t offset = sizeof (@{venue}HeaderPacket) + 2;

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

    if (len < sizeof(@{venue}HeaderPacket))
        return GW_CODEC_SHORT;

    uint16_t frameLength = *((uint16_t*)buf);
    if (len < frameLength)
        return GW_CODEC_SHORT;

    @{venue}HeaderPacket* hdr =
        (@{venue}HeaderPacket*)
            (((unsigned char*)buf) + sizeof(uint16_t));

    used = frameLength;

    std::ostringstream oss;
    oss << hdr->mMessageType;
    
    d.setInteger (FrameLength, frameLength);
    d.setString (MessageType, "%s", oss.str ().c_str ());
    d.setInteger (MessageType, hdr->mMessageType);
    d.setInteger (SchemaId, hdr->mSchemaId);
    d.setInteger (Version, hdr->mVersion);

    switch (hdr->mMessageType) {
@for message in messages:
        case @{message['id']}:
            return get@{message['name']} (d, buf);
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

    if (len < sizeof(@{Venue}HeaderPacket))
        return GW_CODEC_SHORT;

    if (!d.isType (MessageType, CDR_INTEGER, 1))
    {
        setLastError ("MessageType missing or not string");
        return GW_CODEC_ERROR;
    }
    uint8_t type;
    d.getInteger (MessageType, type);

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

    uint16_t* frameLength = (uint16_t*)buf;
    *frameLength = used;

    return state;
}

} // namespace neueda
