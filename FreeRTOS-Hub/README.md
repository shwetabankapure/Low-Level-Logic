# FreeRTOS Generic Multi-Tasking & Scheduling Framework

### ⚡ Low-Level Firmware Architecture Portfolio Project

A robust, modular, and deterministic multi-tasking framework built on **FreeRTOS**. Designed to demonstrate real-time embedded software principles, thread safety, inter-task communication, and structured task prioritization without hardware lock-in.

---

## 🏗️ System Architecture & Task Breakdown

The system implements a cooperative yet prioritized concurrency model using three distinct FreeRTOS tasks communicating via thread-safe primitives.

| Task Name | Priority | Periodicity / Type | Core Responsibility |
| :--- | :--- | :--- | :--- |
| **`Task1_Producer`** | High (Priority 3) | Periodic (`100ms` via `vTaskDelayUntil`) | Simulates high-frequency data/sensor acquisition and pushes payloads to the shared queue. |
| **`Task2_Processor`** | Medium (Priority 2) | Event-Driven (Blocked on Queue) | Receives data packets from the queue, processes state, and protects critical sections using Mutexes. |
| **`Task3_Background`** | Low (Priority 1) | Periodic (`1000ms` via `vTaskDelay`) | Handles background system health monitoring, telemetry formatting, or diagnostic logging. |

---

## 🔑 Key Engineering Highlights

* **Deterministic Timing (`vTaskDelayUntil`):** Task 1 utilizes absolute time pacing instead of relative delays, preventing cumulative timing drift in periodic data acquisition.
* **Inter-Task Communication (Queues):** Decouples the data producer (`Task1`) from the consumer (`Task2`) safely using FreeRTOS message queues (`xQueueCreate`) with timeout management.
* **Resource Protection (Mutexes):** Demonstrates thread safety by protecting shared global states and resources with a Mutex semaphore (`xSemaphoreCreateMutex`).
* **Scalable Framework Design:** Kept intentionally generic (`DataPacket_t`) so hardware drivers (such as I2C sensors or SPI displays) can be cleanly plugged into the architecture later.

---

## 📂 Repository Structure

```text
Low-Level-Logic/
├── main.c          # Core scheduler initialization, queue/mutex setup, and task definitions
└── README.md       # Project documentation and architecture overview