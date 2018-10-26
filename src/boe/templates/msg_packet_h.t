BoeTradeCaptureReportRejectPacket.@require(get_field_type, msg_data, msg_name, Venue, venue, generation_date, types, packet_get_set, get_default_value, get_field_initialise) 
/*
 * Copyright 2014-2018 Neueda Ltd.
 * 
 * Generated @{generation_date}
 */
#ifndef _@{venue.upper()}_@{msg_name.upper()}_PACKET_H_
#define _@{venue.upper()}_@{msg_name.upper()}_PACKET_H_

#include <string>
#include <sstream>
#include <stdint.h>
#include "@{venue}PacketUtils.h"
#include "@{venue}HeaderPacket.h"


namespace neueda
{

PACKED(class @{venue}@{msg_name}Packet
{
public:
@for field in msg_data[msg_name]:
    @if not field.is_group():
    @get_field_initialise(field)
    @end
@end

    @{venue}@{msg_name}Packet ()
    {
@for field in msg_data[msg_name]:
    @if not (field.is_group()):
        @get_default_value(field)
    @end
@end
    }

@for field in msg_data[msg_name]:
    @if not (field.is_group()):
    @packet_get_set(field)
    @end
@end
});

} // namespace neueda

#endif