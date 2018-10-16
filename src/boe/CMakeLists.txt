include_directories(
  ${PROJECT_SOURCE_DIR}/src/boe
  ${PROJECT_SOURCE_DIR}/src/boe/packets
  )

set(BOE_CODEC_HEADERS
  boeCodec.h
  orderMsgBits.h
  newOrderBits.h
  )

add_library(boecodec SHARED
  ${BOE_CODEC_HEADERS}
  boeCodec.cpp
    )
target_link_libraries (boecodec cdr codechelpers fields)

install(FILES ${BOE_CODEC_HEADERS}
  DESTINATION include/codec/boe
    )

install(TARGETS boecodec EXPORT boecodec
  LIBRARY DESTINATION lib)

add_subdirectory(packets)
