target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/error
        ${CMAKE_CURRENT_SOURCE_DIR}/logger
        ${CMAKE_CURRENT_SOURCE_DIR}/config
)


target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/config/GlobalConfig.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/error/SystemError.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/error/CStdError.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/error/BaseError.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger/SystemLogger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger/TensorLogger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/logger/QtLoggerHandler.cpp
)