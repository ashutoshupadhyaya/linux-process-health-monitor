# Stage 5 – Testing, Integration and Improvement

## 1. Stage Overview

Stage 5 focuses on testing, integration, debugging, validation, and improvement of the Linux Process Health Monitoring and Automated Recovery System.

The objective is to verify that the individual modules work correctly together and that the complete system performs monitoring, anomaly detection, alert generation, automated recovery, recovery verification, and character-device communication.

---

## 2. Testing Environment

- Operating System: Ubuntu on WSL2
- Linux Kernel: 6.18.33.2-microsoft-standard-WSL2+
- Programming Language: C++
- Kernel Driver Language: C
- Compiler: GCC/G++ 15.2.0
- Build System: CMake
- Kernel Module Build System: Kbuild
- Version Control: Git

---

## 3. Testing Scope

The following functionality was tested:

- System resource monitoring
- Process monitoring
- CPU usage measurement
- Memory usage measurement
- Disk usage measurement
- Configuration loading and validation
- Health classification
- Alert handling
- Incident logging
- Signal-based recovery
- Recovery verification
- Character-device driver compilation
- Character-device driver loading
- `/dev/phmctl` creation
- C++ to kernel-driver communication
- End-to-end recovery workflow
- Final CMake build

---

## 4. Normal Monitoring Test

The application was executed under normal system conditions.

The system successfully reported:

- CPU usage
- Memory usage
- Disk usage
- Process PID
- Parent PID
- Process name
- Process state
- Process memory
- Process CPU usage

Processes within configured thresholds were classified as:

`NORMAL`

The application generated corresponding normal health messages and completed execution successfully.

### Result

**PASS**

---

## 5. Warning Detection Test

The CPU warning threshold was temporarily lowered to force processes into the warning state.

The system successfully generated:

`WARNING`

for processes exceeding the configured warning threshold.

The AlertManager produced warning messages and IncidentLogger recorded warning incidents.

### Result

**PASS**

---

## 6. Critical Detection Test

A controlled CPU-intensive `yes` process was created for safe testing.

The critical CPU threshold was temporarily lowered so that the controlled process entered the `CRITICAL` state.

The system successfully identified:

`CRITICAL - High CPU or memory usage`

### Result

**PASS**

---

## 7. Automated Recovery Test

When the controlled process entered the critical state, the RecoveryManager initiated automated recovery.

The system sent:

`SIGTERM`

to the affected process.

The recovery operation completed successfully.

### Result

**PASS**

---

## 8. Recovery Verification Test

After sending the recovery signal, RecoveryVerifier checked whether the affected process was still present in `/proc`.

The test confirmed that the controlled process was no longer running.

The system reported:

`Recovery verified: YES`

### Result

**PASS**

---

## 9. Character Device Driver Testing

The Linux character-device driver was built against the matching WSL kernel source.

The driver successfully produced:

`phm_driver.ko`

The module was loaded successfully into the running WSL kernel.

The driver created:

`/dev/phmctl`

Kernel messages confirmed:

- Device number allocation
- Character-device registration
- Device-class creation
- Device-node creation
- Driver initialization

### Result

**PASS**

---

## 10. Device Interface Testing

The C++ DeviceInterfaceManager successfully:

- Opened `/dev/phmctl`
- Wrote a message to the device
- Read the driver response
- Closed the device

A test message was successfully transferred through the character-device interface.

### Result

**PASS**

---

## 11. End-to-End Integration Test

The complete system was tested using a controlled CPU-intensive process.

The final execution demonstrated the complete workflow:


Process Detection
        ↓
CPU Anomaly Detection
        ↓
CRITICAL Health State
        ↓
Alert Generation
        ↓
Incident Logging
        ↓
SIGTERM Recovery
        ↓
Recovery Verification
        ↓
/dev/phmctl
        ↓
Character Device Driver
        ↓
Driver Response

Observed Result
[CRITICAL] yes (PID 4784) requires immediate attention.
[RECOVERY] Critical condition detected for yes (PID 4784).
[SIGNAL] Signal 15 sent to PID 4784.
[RECOVERY] Recovery signal sent successfully.
[VERIFICATION] Process yes (PID 4784) is no longer running.
[DEVICE] /dev/phmctl opened successfully.
[DEVICE] Driver response: Critical incident handled for yes
[DEVICE] /dev/phmctl closed.
[RESULT] Recovery verified: YES

The controlled yes process was terminated successfully after recovery.
Result
PASS
12. Build and Integration Validation
The complete C++ application was built successfully using CMake.
The final build completed with:
[100%] Built target process_health_monitor
The application was then executed successfully under normal operating conditions.
Result
PASS
13. Improvements Implemented
During Stage 5, the following improvements were implemented and validated:
- Improved per-process CPU measurement using /proc/<pid>/stat
- Added configuration loading and validation
- Added warning and critical health states
- Added alert generation
- Added incident logging
- Added automated signal-based recovery
- Added recovery verification
- Added Linux character-device driver support
- Added /dev/phmctl interface
- Added C++ device-interface communication
- Integrated monitoring, analysis, response, recovery, and device modules
- Added end-to-end recovery and driver verification
- Improved error reporting for device and signal operations
14. Current Limitations
The current prototype has the following limitations:
- The main application currently evaluates a limited number of processes during a single execution.
- Continuous daemon-style monitoring is not yet implemented.
- Device-node permissions currently require elevated privileges for direct testing.
- Recovery is currently based on a predefined SIGTERM strategy.
- The current configuration file is accessed using a project-relative path.
These limitations are documented for future improvement and do not affect the demonstrated core monitoring, recovery, and device-driver functionality.
15. Stage 5 Testing Summary
Test	Result
System monitoring	PASS
Process monitoring	PASS
CPU monitoring	PASS
Memory monitoring	PASS
Disk monitoring	PASS
Configuration loading	PASS
Configuration validation	PASS
Normal health detection	PASS
Warning detection	PASS
Critical detection	PASS
Alert handling	PASS
Incident logging	PASS
Signal-based recovery	PASS
Recovery verification	PASS
Character-device driver build	PASS
Character-device driver loading	PASS
/dev/phmctl creation	PASS
Device open operation	PASS
Device write operation	PASS
Device read operation	PASS
Device close operation	PASS
C++ to kernel-driver communication	PASS
End-to-end integration	PASS
Final CMake build	PASS
Normal runtime execution	PASS


16. Stage 5 Conclusion
Stage 5 successfully validated the integration of monitoring, analysis, alerting, logging, automated recovery, recovery verification, and Linux character-device communication.
The complete system successfully demonstrated the intended:
Detect → Alert → Act → Verify
workflow.
The final integration test demonstrated that a critical process can be detected, reported, terminated through the recovery mechanism, verified as stopped, and communicated to the Linux character-device driver through /dev/phmctl.
