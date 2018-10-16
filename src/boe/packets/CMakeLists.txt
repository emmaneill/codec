#
# Copyright 2014-2018 Neueda Ltd.
#
file (GLOB BOE_HEADERS *.h)

install(FILES ${BOE_HEADERS}
    DESTINATION include/codec/boe/packets
    )
