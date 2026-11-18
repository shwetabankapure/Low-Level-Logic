# FreeRTOS Generic Task Scheduling Framework

A clean, lightweight multi-tasking template built on **FreeRTOS**. I put this together as a baseline architecture for embedded systems—focusing on deterministic timing, safe task communication, and clean priority separation without being tied to specific hardware.

---

## ⚙️ How It's Structured

The framework runs three tasks with distinct priorities, passing data safely using FreeRTOS queues and mutexes:

| Task | Priority | Type | What it does |
| :--- | :--- | :--- | :--- |
| **`Task1`** | High (3) | Periodic (`100ms`) | Simulates data/sensor sampling using absolute time pacing (`vTaskDelayUntil`). |
| **`Task2`** | Medium (2) | Event-Driven | Blocks on the queue until data arrives, then processes it safely. |
| **`Task3`** | Low (1) | Periodic (`1000ms`) | Handles background checks, heartbeats, or low-priority logging. |

---

## 💡 Key Design Choices

* **Drift-Free Timing:** Uses `vTaskDelayUntil` in Task 1 instead of standard delays so periodic sampling stays accurate over time.
* **Decoupled Data Flow:** Uses FreeRTOS queues (`xQueueCreate`) to pass data packets from the producer to the processor cleanly.
* **Thread Safety:** Protects shared states using mutexes (`xSemaphoreCreateMutex`) to avoid race conditions.
* **Hardware Agnostic:** Kept intentionally generic (`DataPacket_t`) so real drivers (like I2C/SPI sensors or displays) can easily drop right in later.

---

## 📂 Repository Layout

```text
Low-Level-Logic/
├── main.c          # Kernel setup, queue/mutex initialization, and task definitions
└── README.md       # Project documentation