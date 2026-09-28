## 1. Project Overview

The project is a Linux-based system monitoring and process management application developed using C++.

The system continuously monitors the health and behavior of processes running on a Linux system. It collects system and process information, analyzes the collected data, detects abnormal behavior, generates alerts, records incidents, and performs predefined recovery actions when required.

The project aims to provide a lightweight solution that can assist in identifying and responding to problematic processes before they significantly affect system performance.

## 2. Problem Statement

Linux systems may experience performance degradation because of processes that consume excessive CPU or memory resources, become unresponsive, or repeatedly fail.

Identifying such problems manually requires continuous monitoring and intervention from a system administrator.

A system is therefore required that can continuously observe process behavior, identify abnormal conditions, notify the user, record the incident, and perform predefined recovery actions.

## 3. Proposed Solution

The proposed system will continuously monitor Linux system resources and running processes.

The monitoring engine will collect relevant information, analyze process behavior, and compare it against configurable conditions.

When abnormal behavior is detected, the system will:

1. Identify the affected process.
2. Determine the severity of the condition.
3. Generate an alert.
4. Record the incident in a log.
5. Perform an appropriate predefined recovery action.
6. Verify whether the recovery was successful.
7. Continue monitoring the system.

## 4. Objectives

- Monitor Linux system resources.
- Monitor running processes and their states.
- Detect abnormal process behavior.
- Generate alerts for detected problems.
- Maintain an incident log.
- Provide controlled recovery actions.
- Verify the result of recovery actions.
- Provide a structured C++ implementation.
- Apply Linux system programming concepts in a practical application.

## 5. Scope

The project will initially focus on monitoring a Linux environment and its processes.

The system will monitor:

- CPU utilization.
- Memory utilization.
- Disk utilization.
- Number of running processes.
- Process ID.
- Process name.
- Process state.
- Process CPU usage.
- Process memory usage.
- Parent process information.

The project will also include configurable monitoring conditions, logging, alerting, and controlled recovery mechanisms.

Remote monitoring through a client-server architecture may be considered as an advanced feature if the core system is completed successfully.

## 6. Expected Outcome

The final system is expected to provide a working Linux application capable of:

- Continuously monitoring system and process health.
- Detecting abnormal resource usage or process behavior.
- Generating meaningful alerts.
- Recording incidents for later analysis.
- Performing predefined recovery actions.
- Verifying recovery results.

The system should demonstrate the practical application of Linux, system programming, process management, and C++ programming concepts.

## 7. Real-World Application

The system can be useful as a lightweight monitoring and assistance tool for Linux-based computers, servers, development systems, and other environments where process failures or excessive resource consumption need to be detected quickly.

It can assist administrators by reducing the need for continuous manual process monitoring and by providing a record of detected incidents and recovery actions.

## 8. Key Features

- Linux system resource monitoring.
- Process monitoring.
- Process health analysis.
- Abnormal behavior detection.
- Configurable monitoring conditions.
- Alert generation.
- Incident logging.
- Controlled process recovery.
- Recovery verification.
- Graceful application shutdown.
- Git-based version control.
