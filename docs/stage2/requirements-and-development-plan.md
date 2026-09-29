# Stage 2 - Requirements & Development Plan

## 1. Product Requirements

The system shall provide a Linux-based application for continuous monitoring of system resources and running processes. It shall analyze process behavior, detect abnormal conditions, alert the user, maintain incident records, and perform controlled recovery actions according to configured policies.

## 2. Functional Requirements

### 2.1 System Resource Monitoring

- Monitor CPU utilization.
- Monitor RAM utilization.
- Monitor disk utilization.

### 2.2 Process Monitoring

- Detect running processes.
- Track PID, process name, state, CPU usage, and memory usage.
- Track parent-process information.

### 2.3 Health and Anomaly Detection

- Identify unusually high resource consumption.
- Detect potentially problematic or unresponsive processes.
- Assign severity levels such as Normal, Warning, and Critical.

### 2.4 Alert Management

- Display detected problems to the user.
- Identify the affected process and reason for the alert.

### 2.5 Incident Logging

- Record detected problems.
- Record recovery actions and their results.

### 2.6 Recovery Management

- Apply predefined and controlled recovery actions.
- Avoid uncontrolled termination of processes.
- Allow recovery rules to be configured.

### 2.7 Recovery Verification

- Recheck the process/system after an action.
- Record whether recovery succeeded or failed.

### 2.8 Configuration

- Allow monitoring thresholds and policies to be changed without modifying the core application.

### 2.9 Graceful Shutdown

- Handle termination signals.
- Close files and resources properly before exiting.

## 3. Non-Functional Requirements

### 3.1 Performance

- The monitoring system should use minimal CPU and memory resources.
- Monitoring should run continuously without significantly affecting system performance.

### 3.2 Reliability

- The monitoring application should operate continuously during normal system operation.
- Monitoring failures should be recorded appropriately.

### 3.3 Safety

- Recovery actions must follow predefined rules.
- The system should avoid uncontrolled termination of processes.
- Critical operations should require appropriate permissions.

### 3.4 Usability

- System status and alerts should be easy to understand.
- Important process information should be clearly displayed.

### 3.5 Maintainability

- The application should use modular C++ components.
- Individual modules should be easy to modify and test.

### 3.6 Configurability

- Monitoring thresholds and recovery policies should be configurable.
- Configuration changes should not require modification of the core source code.

### 3.7 Portability

- The application should target standard Linux environments.
- The implementation should avoid unnecessary platform-specific dependencies.

### 3.8 Security

- Privileged operations should be controlled.
- The system should not perform recovery actions without appropriate authorization.

## 4. System Modules

### 4.1 System Monitor

Collects overall Linux system information such as CPU, memory, disk usage, and process count.

### 4.2 Process Monitor

Collects information about running processes including PID, process name, state, CPU usage, memory usage, and parent process.

### 4.3 Health Analyzer

Analyzes collected data and identifies abnormal system or process behavior.

### 4.4 Alert Manager

Generates alerts when abnormal conditions are detected and provides details about the affected process or resource.

### 4.5 Incident Logger

Records detected problems, alerts, recovery actions, and recovery results.

### 4.6 Recovery Manager

Applies predefined recovery policies to selected abnormal conditions.

### 4.7 Recovery Verifier

Checks the system after a recovery action and determines whether the problem has been resolved.

### 4.8 Configuration Manager

Loads and manages monitoring thresholds and recovery policies.

### 4.9 Signal and Shutdown Manager

Handles termination and other relevant signals and ensures that the application exits cleanly.

## 5. Core Features

- Continuous Linux system resource monitoring.
- Continuous process monitoring.
- Process health analysis.
- Configurable anomaly detection.
- Normal, Warning, and Critical severity levels.
- Real-time alerts.
- Incident and recovery logging.
- Controlled recovery actions.
- Recovery result verification.
- Configurable monitoring thresholds.
- Graceful application shutdown.
- Modular C++ architecture.

## 6. Inputs and Outputs

### 6.1 Inputs

The system will obtain information from the Linux environment, including:

- CPU utilization.
- Memory utilization.
- Disk utilization.
- Running process information.
- Process CPU and memory usage.
- Process state.
- PID and PPID.
- Configuration parameters.
- Monitoring thresholds.
- Recovery policies.

### 6.2 Outputs

The system will produce:

- Current system status.
- Process status information.
- Warning and critical alerts.
- Incident logs.
- Recovery action logs.
- Recovery success or failure status.
- Monitoring reports.

## 7. Recovery Strategy

The recovery system will use controlled and predefined policies.

### 7.1 Warning Condition

For a warning condition:

- Record the event.
- Display an alert.
- Continue monitoring the affected process.

### 7.2 Critical Condition

For a critical condition:

- Record the incident.
- Generate a high-priority alert.
- Evaluate the configured recovery policy.
- Perform an approved recovery action if applicable.
- Verify the result.

### 7.3 Recovery Verification

After a recovery action:

1. Recheck the affected process or resource.
2. Compare the new condition with the previous condition.
3. Record the recovery result.
4. Continue monitoring.

The system will avoid uncontrolled termination of processes and will apply recovery actions only according to configured policies.

## 8. Technology Stack

### Programming Language

- C++

### Operating System

- Linux
- Ubuntu running through WSL2 during development

### System Interfaces

- Linux `/proc` filesystem.
- Linux process and system interfaces.
- POSIX/system programming interfaces.
- Linux signals.

### Build Tools

- GCC/G++.
- CMake.
- Make.

### Debugging

- GDB.

### Version Control

- Git.
- GitHub.

### Documentation

- Markdown.
- UML diagrams.
- Project report and presentation.

## 9. Project Scope

### Included

- Linux system resource monitoring.
- Linux process monitoring.
- Process health analysis.
- Configurable anomaly detection.
- Alert generation.
- Incident logging.
- Controlled recovery.
- Recovery verification.
- Signal-based graceful shutdown.
- Modular C++ implementation.

### Optional / Advanced

- Client-server remote monitoring.
- Network-based alerts.
- Background daemon operation.
- Additional IPC mechanisms
.
Advanced features will only be added after the core monitoring and recovery system is stable.

## 10. Deliverables

The project will produce:

- Working C++ monitoring application.
- Source code.
- Configuration files.
- Monitoring and recovery logs.
- Test cases and test results.
- UML diagrams.
- System architecture documentation.
- Stage-wise project documentation.
- Git repository with development history.
- Project report.
- Final presentation and demonstration.

## 11. Development Plan

### Phase 1 - Environment and Project Setup

- Configure Ubuntu/WSL2.
- Configure C++ development tools.
- Initialize Git repository.
- Create project structure.

### Phase 2 - Requirements and Design

- Finalize functional requirements.
- Finalize non-functional requirements.
- Design system architecture.
- Design modules and data flow.
- Prepare UML diagrams.

### Phase 3 - Basic Monitoring

- Implement system resource monitoring.
- Implement process information collection.
- Display monitoring information.

### Phase 4 - Health Analysis

- Implement monitoring thresholds.
- Implement anomaly detection.
- Add severity classification.

### Phase 5 - Alerting and Logging

- Implement alert generation.
- Implement incident logging.
- Record detected conditions and system events.

### Phase 6 - Recovery

- Implement recovery policies.
- Implement controlled recovery actions.
- Implement recovery verification.

### Phase 7 - System Programming Integration

- Integrate Linux signals.
- Implement graceful shutdown.
- Apply appropriate system programming concepts.
- Improve resource and file handling.

### Phase 8 - Testing and Improvement

- Perform unit testing.
- Perform integration testing.
- Perform system testing.
- Test abnormal conditions.
- Test recovery behavior.
- Debug and optimize the system.

### Phase 9 - Optional Advanced Features

- Evaluate client-server monitoring.
- Evaluate background daemon operation.
- Add additional features only if the core system is stable.

Advanced features will only be added after the core system is stable.

### Phase 10 - Finalization

- Complete documentation.
- Update Git repository.
- Prepare final report.
- Prepare final demonstration.
- Prepare presentation and interview explanation.

## 12. Limitations

- The initial implementation will focus on a single Linux environment.
- Monitoring accuracy depends on the information provided by the Linux system.
- Automatic recovery will be limited to predefined and controlled actions.
- Some recovery operations may require elevated privileges.
- The initial version will not attempt to automatically resolve every possible system failure.
- Remote monitoring and daemon functionality are considered advanced features and may be implemented only after the core system is complete.

