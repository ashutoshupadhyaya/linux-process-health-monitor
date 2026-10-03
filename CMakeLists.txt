cmake_minimum_required(VERSION 3.16)

project(process_health_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(process_health_monitor
    src/main.cpp

    src/monitoring/system_monitor.cpp
    src/monitoring/process_monitor.cpp

    src/configuration/configuration_manager.cpp

    src/analysis/health_analyzer.cpp

    src/response/alert_manager.cpp
    src/response/incident_logger.cpp

    src/recovery/signal_manager.cpp
    src/recovery/recovery_manager.cpp
    src/recovery/recovery_verifier.cpp

    src/device/device_interface_manager.cpp
)
