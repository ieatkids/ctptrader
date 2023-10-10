set(BIN_NAME trader)

include_directories(${THIRDPARTY}/ctp/include)

add_executable(${BIN_NAME} main.cpp)

target_link_libraries(${BIN_NAME} PRIVATE 
    baseLib
    utilLib
    serviceLib
    appLib
    ${THIRDPARTY}/ctp/lib/libthostmduserapi_se.so
    ${THIRDPARTY}/ctp/lib/libthosttraderapi_se.so
)

install(TARGETS ${BIN_NAME} DESTINATION ${APP_BIN_DIR})