cmake_minimum_required(VERSION 3.16)

project(LinuxProcessHealthMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(process_health_monitor
    src/main.cpp
    src/monitoring/system_monitor.cpp
    src/monitoring/process_monitor.cpp
)
