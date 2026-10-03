# Stage 4 — Initial Implementation, Prototype, Testing and Results

## 1. Stage Overview

Stage 4 focuses on the initial implementation and prototype development of the Linux-Based Intelligent Process Health Monitoring and Automated Recovery System.

The objective of this stage is to convert the design prepared in Stage 3 into a working Linux/C++ prototype.

The current implementation focuses on the monitoring and analysis portion of the system:

- System resource monitoring
- Process monitoring
- Process CPU and memory monitoring
- Health analysis
- Threshold-based anomaly detection
- Configuration management
- Configuration validation
- Integration into the main application

The response, recovery, logging, and Linux character-device-driver components are integrated with the monitoring, analysis, and configuration modules as part of the current system.

---

# 2. Development Environment

The project was developed and tested using Linux through WSL2 on Windows.

## Environment

- Operating System: Ubuntu 24.04 through WSL2
- Compiler: GCC/G++
- Programming Language: C++17
- Build System: CMake
- Debugging Tool: GDB
- Version Control: Git
- Repository Hosting: GitHub

## Linux Interfaces Used

The prototype uses Linux system interfaces including:

- `/proc/stat`
- `/proc/meminfo`
- `/proc/<PID>/status`
- `/proc/<PID>/stat`
- `statvfs()`
- Linux directory operations through `dirent.h`

---

# 3. Stage 4 Project Structure

The implementation currently follows the following structure:

```text
linux-process-health-monitor/
│
├── config/
│   └── monitor.conf
│
├── docs/
│   ├── stage1/
│   ├── stage2/
│   ├── stage3/
│   └── stage4/
│       └── stage4-implementation-testing-and-results.md
│
├── include/
│   ├── system_monitor.hpp
│   ├── process_monitor.hpp
│   ├── health_analyzer.hpp
│   └── configuration_manager.hpp
│
├── src/
│   ├── main.cpp
│   │
│   ├── monitoring/
│   │   ├── system_monitor.cpp
│   │   └── process_monitor.cpp
│   │
│   ├── analysis/
│   │   └── health_analyzer.cpp
│   │
│   └── configuration/
│       └── configuration_manager.cpp
│
├── tests/
│
├── CMakeLists.txt
└── README.md
4. CMake Build Configuration

CMake is used to compile the C++ project.

The executable currently includes:

main.cpp
system_monitor.cpp
process_monitor.cpp
configuration_manager.cpp
health_analyzer.cpp

The project uses the C++17 standard.

The application is built using:

cd ~/linux-process-health-monitor/build
cmake ..
make

A clean build was also tested using:

make clean
make

The clean build completed successfully:

[100%] Built target process_health_monitor
5. SystemMonitor Implementation
5.1 Purpose

The SystemMonitor component collects overall Linux system resource information.

The current implementation monitors:

CPU usage
Memory usage
Disk usage
5.2 CPU Monitoring

CPU information is obtained from:

/proc/stat

The implementation reads the CPU time counters and calculates CPU utilization using two measurements separated by a short interval.

The CPU usage is calculated from the difference between the two measurements.

The result is returned as a percentage.

Example output:

CPU Usage: 0%

The CPU monitoring component was successfully compiled and executed during testing.

Status:

PASS

6. Memory Monitoring

Memory information is obtained from:

/proc/meminfo

The implementation reads:

MemTotal
MemAvailable

Memory utilization is calculated using the difference between total and available memory.

Example output:

Memory Usage: 11.86%

Status:

PASS

7. Disk Monitoring

Disk utilization is obtained using the Linux statvfs() interface.

The prototype checks the root filesystem:

/

Example output:

Disk Usage: 5.28%

The disk monitoring component successfully compiled and executed.

Status:

PASS

8. ProcessMonitor Implementation
8.1 Purpose

The ProcessMonitor component discovers running Linux processes and collects information about individual processes.

Linux process information is obtained from the /proc filesystem.

The implementation scans numeric directories under:

/proc

Each numeric directory represents a process ID.

8.2 Process Information

The current ProcessInfo structure contains:

PID
PPID
Process name
Process state
Memory usage
CPU usage

The information is primarily collected from:

/proc/<PID>/status

and:

/proc/<PID>/stat
9. Process PID, PPID, Name and State

The ProcessMonitor extracts:

PID
Parent PID
Process name
Process state

Example:

PID: 1 | PPID: 0 | Name: systemd | State: S (sleeping)

The implementation successfully discovered multiple running Linux processes.

Status:

PASS

10. Process Memory Monitoring

Process memory usage is obtained from the:

VmRSS

field in:

/proc/<PID>/status

The value is stored in kilobytes.

Example:

Memory: 15344 kB

Status:

PASS

11. Process CPU Monitoring

Process CPU usage is calculated using process CPU time from:

/proc/<PID>/stat

The implementation compares process CPU ticks before and after a short sampling interval.

The process CPU difference is compared against the total CPU tick difference.

Example:

PID: 1853 | PPID: 1807 | Name: yes
State: R (running)
Memory: 7488 kB
CPU: 8.87097%

A CPU-intensive yes process was used during testing.

The ProcessMonitor successfully detected the process and reported its CPU activity.

Status:

PASS

12. HealthAnalyzer Implementation
12.1 Purpose

The HealthAnalyzer evaluates the resource usage of each monitored process.

The analyzer converts resource measurements into health states.

Three health states are supported:

NORMAL
WARNING
CRITICAL
13. Health Classification

The HealthAnalyzer currently considers process CPU and memory usage against configured thresholds.

The resulting structure contains:

PID
Process name
Health status
Reason

For normal resource usage, the application reports:

Health: NORMAL - Resource usage within limits

Example output:

PID: 1 | PPID: 0 | Name: systemd | State: S (sleeping) |
Memory: 15344 kB | CPU: 0%

Health: NORMAL - Resource usage within limits

Status:

PASS

14. Threshold-Based Anomaly Detection

The system uses configurable thresholds to classify abnormal resource usage.

The configuration includes CPU and memory warning/critical thresholds.

The basic classification model is:

Resource usage within normal range
        ↓
      NORMAL

Elevated resource usage
        ↓
      WARNING

Severely elevated resource usage
        ↓
      CRITICAL

The thresholds can be modified without changing the source code.

A threshold modification was tested during development and successfully affected the resulting health classification.

Status:

PASS

15. ConfigurationManager
15.1 Purpose

The ConfigurationManager provides configurable parameters to the monitoring and analysis components.

Configuration values are stored in:

config/monitor.conf

The current configuration contains:

CPU_WARNING=70
CPU_CRITICAL=90
MEMORY_WARNING=250000
MEMORY_CRITICAL=500000
MONITOR_INTERVAL_MS=100
16. Configuration Parameters
CPU_WARNING

Defines the CPU usage level at which warning-level behavior can begin.

Current value:

70
CPU_CRITICAL

Defines the CPU usage level for critical classification.

Current value:

90
MEMORY_WARNING

Defines the process memory warning threshold.

Current value:

250000
MEMORY_CRITICAL

Defines the process memory critical threshold.

Current value:

500000
MONITOR_INTERVAL_MS

Defines the monitoring sampling interval.

Current value:

100
17. Configuration Validation

Configuration validation was added to prevent invalid values from producing unsafe or unusable monitoring behavior.

The implementation validates:

Negative CPU warning thresholds
Invalid CPU critical thresholds
Negative memory thresholds
Invalid memory critical thresholds
Invalid monitoring intervals

Fallback values are used when necessary.

For example, the critical CPU threshold must remain greater than the warning threshold.

18. Invalid Configuration Test

An invalid configuration was intentionally introduced:

CPU_WARNING=abc

Initially, the conversion operation produced:

std::invalid_argument

and the application terminated.

The ConfigurationManager was then improved by adding exception handling around numeric conversions.

After the improvement, the same invalid configuration was tested again.

The application continued running normally instead of terminating.

Example result:

Linux Process Health Monitor started.

CPU Usage: 0%
Memory Usage: ...
Disk Usage: ...

Running Processes:
...
Health: NORMAL - Resource usage within limits

No std::invalid_argument or Aborted message was produced.

Status:

PASS

19. Main Application Integration

The current main application integrates:

SystemMonitor
ProcessMonitor
HealthAnalyzer
ConfigurationManager

The current application flow is:

Linux System
     |
     v
SystemMonitor
     |
     +---- CPU
     +---- Memory
     +---- Disk
     |
     v
ProcessMonitor
     |
     +---- PID
     +---- PPID
     +---- Name
     +---- State
     +---- CPU
     +---- Memory
     |
     v
HealthAnalyzer
     |
     +---- NORMAL
     +---- WARNING
     +---- CRITICAL
     |
     v
Console Output

The application successfully builds and executes as a single C++ executable.

20. Testing Summary
Test	Result
CMake configuration	PASS
Clean compilation	PASS
CPU monitoring	PASS
System memory monitoring	PASS
Disk monitoring	PASS
Process discovery	PASS
PID/PPID detection	PASS
Process name detection	PASS
Process state detection	PASS
Process memory monitoring	PASS
Process CPU monitoring	PASS
CPU-intensive process test	PASS
HealthAnalyzer	PASS
NORMAL state	PASS
WARNING state	PASS
CRITICAL state	PASS
Configuration loading	PASS
Threshold modification	PASS
Invalid configuration handling	PASS
Configuration validation	PASS
Main application integration	PASS
21. Prototype Limitations

The current Stage 4 implementation is an initial prototype.

Current limitations include:

Monitoring output is currently console-based.
The application currently performs a limited monitoring/display cycle rather than providing the final long-running monitoring service.
Alerting and automated recovery are not yet integrated.
Incident logging is not yet integrated.
Recovery verification is not yet integrated.
The Linux character-device driver is not yet integrated.
Remote/client-server monitoring is not yet implemented.
Advanced daemon functionality is planned for later stages.
More extensive automated testing is still required.

These limitations are consistent with the prototype stage and will be addressed during later implementation and integration.

22. Current Architecture

The current implemented portion of the system can be represented as:

                LINUX SYSTEM
                     |
          +----------+----------+
          |                     |
          v                     v
      /proc/stat           /proc/meminfo
          |                     |
          +----------+----------+
                     |
                     v
              SystemMonitor
                     |
                     v
              ProcessMonitor
                     |
          +----------+----------+
          |                     |
          v                     v
    Process Information    Resource Usage
          |                     |
          +----------+----------+
                     |
                     v
              HealthAnalyzer
                     |
                     v
          NORMAL / WARNING /
             CRITICAL
                     |
                     v
               Main Output

Configuration is provided through:

config/monitor.conf
        |
        v
ConfigurationManager
        |
        v
Monitoring + Health Analysis
23. Git and Version Control

Git was used throughout development to maintain version history.

The project is maintained on the following development branch:

feature/monitoring-analysis

The development process follows:

Implement
    ↓
Build
    ↓
Test
    ↓
Review
    ↓
Commit
    ↓
Push to GitHub

Meaningful development milestones were committed separately during the implementation.

The current Stage 4 work will be consolidated into a final Stage 4 checkpoint after documentation is completed.

24. Stage 4 Achievements

The following achievements were completed during Stage 4:

Developed a working C++ Linux monitoring prototype.
Implemented system CPU monitoring.
Implemented system memory monitoring.
Implemented disk usage monitoring.
Implemented Linux process discovery.
Implemented PID and PPID monitoring.
Implemented process name and state monitoring.
Implemented process memory monitoring.
Implemented process CPU monitoring.
Implemented configurable health thresholds.
Implemented NORMAL/WARNING/CRITICAL classification.
Implemented configuration file support.
Implemented configuration validation.
Added handling for invalid numeric configuration values.
Integrated monitoring and analysis components into the main application.
Successfully built the application using CMake.
Tested the application under Linux/WSL2.
Tested the system using a CPU-intensive process.
25. Work Remaining After Stage 4

The following components will be developed and integrated in later work:

AlertManager
IncidentLogger
RecoveryManager
RecoveryVerifier
SignalManager
DeviceInterfaceManager
Linux Character Device Driver

These components form the integrated response, recovery, and device-driver layer of the Linux Process Health Monitoring and Automated Recovery System.

After these components are developed, they will be integrated with the monitoring and analysis components implemented in the current Stage 4 prototype.

26. Next Stage Direction

The next development phase will focus on completing the remaining system components and integrating the response and recovery workflow.

The intended final workflow is:

MONITOR
   ↓
ANALYZE
   ↓
DETECT
   ↓
ALERT
   ↓
LOG
   ↓
RECOVER
   ↓
VERIFY
   ↓
CONTINUE MONITORING

The monitoring and analysis components developed in Stage 4 provide the foundation for this complete workflow.

27. Stage 4 Conclusion

Stage 4 successfully produced a functional Linux/C++ monitoring and analysis prototype.

The prototype can collect system and process information from Linux interfaces, calculate resource usage, evaluate process health, classify resource conditions, load configurable thresholds, and handle invalid configuration values without terminating the application.

The implementation has been compiled and tested successfully using CMake and GNU Make on Ubuntu through WSL2.

The monitoring and analysis portion is now ready to be integrated with the alert, logging, recovery, verification, signal-handling, and Linux character-device-driver components being developed for the remaining project functionality.

Stage 4 Monitoring & Analysis Prototype: COMPLETED
