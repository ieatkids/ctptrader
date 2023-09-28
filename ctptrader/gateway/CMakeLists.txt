set(LIB_NAME gatewayLib)

include_directories(${THIRDPARTY}/ctp/include)

add_library(${LIB_NAME} STATIC
    ctpMG.cpp
)

target_link_libraries(${LIB_NAME}
    baseLib
    utilLib
    serviceLib
    ${THIRDPARTY}/ctp/lib/libthostmduserapi_se.so
    ${THIRDPARTY}/ctp/lib/libthosttraderapi_se.so
    ${THIRDPARTY}/fmt/lib/libfmt.a
)

