cmake_minimum_required(VERSION 3.28)
project(modbus_master_rtu_demo)

set(CMAKE_CXX_STANDARD 20)

add_executable(modbus_master_rtu_demo main.cpp
        connections/Connection.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MODBUS REQUIRED libmodbus)

# include 路径
target_include_directories(modbus_master_rtu_demo PRIVATE
        ${MODBUS_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/connections
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
)

# library 路径（关键）
target_link_directories(modbus_master_rtu_demo PRIVATE
        ${MODBUS_LIBRARY_DIRS}
)

# 链接库
target_link_libraries(modbus_master_rtu_demo
        ${MODBUS_LIBRARIES}
)
