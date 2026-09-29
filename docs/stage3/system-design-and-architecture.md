# Stage 3 - System Design & Architecture

## 1. System Architecture

The system will follow a modular architecture in which Linux system information is collected, analyzed, and processed before alerts or recovery actions are generated.

The overall data flow is:

Linux System
    ↓
System Monitor / Process Monitor
    ↓
Health Analyzer
    ↓
Alert Manager / Recovery Manager
    ↓
Incident Logger
    ↓
Recovery Verifier
    ↓
Continue Monitoring

## 2. Architecture Components

### 2.1 System Monitor

Responsible for collecting overall system information such as CPU utilization, memory utilization, disk utilization, and process count.

### 2.2 Process Monitor

Responsible for collecting information about individual processes including PID, PPID, process name, process state, CPU usage, and memory usage.

### 2.3 Health Analyzer

Processes collected information and determines whether the system or a process is operating normally or showing abnormal behavior.

### 2.4 Alert Manager

Generates user-readable alerts when abnormal conditions are detected.

### 2.5 Incident Logger

Stores detected incidents, alerts, recovery actions, and recovery results.

### 2.6 Recovery Manager

Evaluates configured recovery policies and performs controlled recovery actions when an applicable condition is detected.

### 2.7 Recovery Verifier

Checks the affected process or resource after recovery and determines whether the condition has improved.

### 2.8 Configuration Manager

Loads monitoring thresholds and recovery policies from configuration files.

### 2.9 Signal and Shutdown Manager

Handles relevant Linux signals and ensures that the application terminates cleanly.

## 3. System Data Flow

1. The monitoring system starts.
2. Configuration parameters are loaded.
3. System and process information is collected.
4. The collected information is passed to the Health Analyzer.
5. The analyzer evaluates the current condition.
6. Normal conditions are recorded and monitoring continues.
7. Abnormal conditions generate an alert and incident record.
8. The Recovery Manager evaluates whether a recovery action is permitted.
9. If applicable, the recovery action is performed.
10. The Recovery Verifier checks the result.
11. The result is recorded.
12. Monitoring continues.

## 4. Main Data Structures

### 4.1 ProcessInfo

The ProcessInfo structure will represent information about an individual process.

Possible fields:

- PID
- PPID
- Process name
- Process state
- CPU usage
- Memory usage
- Runtime

### 4.2 SystemInfo

The SystemInfo structure will represent overall system information.

Possible fields:

- CPU utilization
- Memory utilization
- Disk utilization
- Process count

### 4.3 Alert

The Alert structure will represent a detected abnormal condition.

Possible fields:

- PID
- Process name
- Alert type
- Severity
- Current value
- Threshold value
- Timestamp
- Description

### 4.4 RecoveryResult

The RecoveryResult structure will represent the result of a recovery operation.

Possible fields:

- PID
- Recovery action
- Start time
- End time
- Result status
- Verification status
- Description

## 5. C++ Module Design

The implementation will use separate C++ modules/classes.

Proposed classes:

- SystemMonitor
- ProcessMonitor
- HealthAnalyzer
- AlertManager
- IncidentLogger
- RecoveryManager
- RecoveryVerifier
- ConfigurationManager
- SignalManager


The main application will coordinate these components.

## 6. Class Relationships

The Main Application coordinates the major system components.

The System Monitor and Process Monitor collect information from the Linux environment.

The collected information is passed to the Health Analyzer for evaluation.

The Health Analyzer identifies normal or abnormal conditions and communicates the result to the Alert Manager and Recovery Manager.

The Alert Manager generates user-facing alerts.

The Incident Logger records important monitoring, alert, and recovery events.

The Recovery Manager evaluates configured recovery policies and performs approved actions.

The Recovery Verifier checks whether the recovery action successfully improved the monitored condition.

The Configuration Manager provides thresholds and recovery policies to the monitoring and recovery components.

The Signal and Shutdown Manager handles application termination and cleanup.

### Component Communication Flow

Main Application
    ↓
System Monitor + Process Monitor
    ↓
Health Analyzer
    ↓
Alert Manager + Recovery Manager
    ↓
Incident Logger
    ↓
Recovery Verifier
    ↓
Continue Monitoring
## 7. Processing Workflow

### Normal Condition

Process/System
    ↓
Collect information
    ↓
Analyze
    ↓
Condition is normal
    ↓
Continue monitoring

### Abnormal Condition

Process/System
    ↓
Collect information
    ↓
Analyze
    ↓
Abnormal condition detected
    ↓
Generate Alert
    ↓
Record Incident
    ↓
Evaluate Recovery Policy
    ↓
Recovery Action
    ↓
Verify Result
    ↓
Record Result
    ↓
Continue Monitoring

## 8. Severity State Model

The system will use three primary severity levels:

NORMAL
    ↓
WARNING
    ↓
CRITICAL

A condition may return to NORMAL after the monitored value returns to an acceptable range.

A CRITICAL condition may trigger an approved recovery action depending on the configured recovery policy.

## 9. Sequence of a Recovery Event

1. Process information is collected.
2. Health Analyzer detects an abnormal condition.
3. Alert Manager generates an alert.
4. Incident Logger records the incident.
5. Recovery Manager checks the configured policy.
6. An approved recovery action is performed.
7. Recovery Verifier checks the process/system.
8. Incident Logger records the recovery result.
9. Monitoring resumes.

## 10. Linux Interfaces

The initial implementation will use Linux interfaces to obtain system and process information.

Primary interfaces include:

- `/proc`
- Linux process information
- Linux signal mechanisms
- POSIX/system programming interfaces
- Linux file operations where required

## 11. Configuration Design

Monitoring thresholds and recovery policies will be kept outside the main program logic.

Example configuration categories:

- CPU warning threshold
- CPU critical threshold
- Memory warning threshold
- Memory critical threshold
- Monitoring interval
- Recovery policy
- Logging configuration

This allows monitoring behavior to be changed without modifying the core C++ implementation.

## 12. Error Handling

The application will handle situations such as:

- Process disappearing during monitoring.
- Permission denied while accessing process information.
- Invalid configuration values.
- Failure to write logs.
- Recovery action failure.
- Interrupted monitoring.
- Invalid or unavailable system information.

Errors will be recorded where appropriate and the monitoring system will attempt to continue safely.

## 13. Development Environment

### Operating System

Ubuntu Linux running through WSL2.

### Programming

C++.

### Compiler

G++.

### Build System

CMake.

### Debugging

GDB.

### Version Control

Git and GitHub.

### Documentation

Markdown and UML diagrams.

## 14. Git Development Strategy

The project will use the `main` branch for stable development.

Development will be divided into logical commits such as:

- Initial project structure
- Requirements documentation
- Architecture documentation
- Basic monitoring implementation
- Process monitoring implementation
- Health analysis implementation
- Alert and logging implementation
- Recovery implementation
- Testing and improvements
- Final documentation

## 15. Implementation Plan

The implementation will follow this order:

1. Create the C++ project and CMake configuration.
2. Implement basic system monitoring.
3. Implement process information collection.
4. Implement health analysis.
5. Implement alert generation.
6. Implement incident logging.
7. Implement configuration handling.
8. Implement controlled recovery.
9. Implement recovery verification.
10. Implement signal handling and graceful shutdown.
11. Integrate and test all modules.
12. Optimize and document the final system.

## 16. UML Diagrams

The following UML diagrams will be prepared:

- System architecture diagram.
- Class diagram.
- Sequence diagram.
- State machine diagram.
- System workflow/flowchart.

## 17. Expected Design Outcome

The final design will provide a modular Linux monitoring architecture where monitoring, analysis, alerting, logging, recovery, and verification are separated into independent components.

This design will allow individual modules to be developed and tested independently before being integrated into the complete system.
