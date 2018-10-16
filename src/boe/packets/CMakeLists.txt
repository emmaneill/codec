#
# Copyright 2014-2018 Neueda Ltd.
#
file (GLOB BOE_HEADERS "*.h")

include_directories(
  ${PROJECT_SOURCE_DIR}/src/boe/
  ${PROJECT_SOURCE_DIR}/src/boe/packets/
  )

install(FILES ${BOE_HEADERS}
    DESTINATION include/codec/boe/packets
    )
