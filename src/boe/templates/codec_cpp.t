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
        
        if(paramGroupsElement->getParamGroupType () == 80)
        {
            BoeUnitSequencePacket* unitSequenceElement = (BoeUnitSequencePacket*) ((char*)buf + offset);
            
            cdrArray UnitSequenceArray;
            item.setInteger(NoUnspecifiedUnitReplay, unitSequenceElement->getNoUnspecifiedUnitReplay ());
            UnitSequenceArray.push_back(item);
            
            int NumUnits = unitSequenceElement->getNumOfUnits ();
            item.setInteger(NumOfUnits, unitSequenceElement->getNumOfUnits ());
            UnitSequenceArray.push_back(item);
            
            for (int i = 0; i < NumUnits; i++)
            {
                item.setInteger(UnitNumber, unitSequenceElement->getUnitNumber ());
                UnitSequenceArray.push_back(item);
                
                item.setInteger(UnitSequence, unitSequenceElement->getUnitSequence ());
                UnitSequenceArray.push_back(item);
            }
            offset += sizeof (unitSequenceElement);
        }
        else if(paramGroupsElement->getParamGroupType () == 81)
        {
            BoeReturnBitfieldPacket* returnBitfieldElement = (BoeReturnBitfieldPacket*) ((char*)buf + offset);
            
            cdrArray ReturnBitfieldArray;
            item.setInteger(MessageType, returnBitfieldElement->getMessageType ());
            ReturnBitfieldArray.push_back(item);
            
            int NumReturnBitfields = returnBitfieldElement->getNumOfReturnBitfield ();
            item.setInteger(NumOfReturnBitfield, returnBitfieldElement->getNumOfReturnBitfield ());
            ReturnBitfieldArray.push_back(item);
            
            for (int i = 0; i < NumReturnBitfields; i++)
            {
                item.setInteger(ReturnBitfield, returnBitfieldElement->getReturnBitfield ());
                ReturnBitfieldArray.push_back(item);
            }
            offset += sizeof (returnBitfieldElement);
        }
        
        offset += sizeof (BoeParamGroupsPacket);
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
                
        if(paramGroupType == 80)
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
        else if(paramGroupType == 81)
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
                uint64_t returnBitfield;
                if (!item.getInteger (ReturnBitfield, returnBitfield))
                {
                    setLastError ("ReturnBitfield missing or not integer");
                    return GW_CODEC_ERROR;
                }
                returnBitfieldElement->setReturnBitfield (returnBitfield);
                offset += sizeof (int64_t); 
            }
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
