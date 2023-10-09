set(LIB_NAME baseLib)

add_library(${LIB_NAME} STATIC
    defs.cpp
    date.cpp
    timestamp.cpp
    msg.cpp
    stg.cpp
)