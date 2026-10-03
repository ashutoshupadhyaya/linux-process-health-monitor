# Linux-Based Intelligent Process Health Monitoring and Automated Recovery System

A Linux-based process health monitoring system that detects critical process conditions and performs automated recovery actions.

## Current Project Status

The response and recovery side of the system is implemented and tested.

Current response flow:

HealthResult
    |
    v
AlertManager
    |
    v
IncidentLogger
    |
    v
RecoveryManager
    |
    v
SignalManager
    |
    v
RecoveryVerifier
    |
    v
DeviceInterfaceManager
    |
    v
/dev/phmctl
    |
    v
Linux Character Device Driver

The monitoring and health-analysis side will provide the real `HealthResult` objects.

## Requirements

- Linux / WSL2
- C++17 compiler
- CMake 3.16 or newer
- GNU Make
- Linux kernel headers/source for the character driver

## Build User-Space Application

From the repository root:

```bash
cmake -S . -B build
cmake --build build
