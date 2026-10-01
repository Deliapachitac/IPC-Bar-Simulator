# 🍺 Nemea Path Bar — Concurrent Synchronization & Shared Memory (`IPC-Bar-Simulator`)

An asynchronous, concurrent multi-process simulation built in C for Linux operating systems. The project models the "Bar on the Path to Nemea" synchronization problem, managing table allocation, customer order queues, and real-time statistics using **System V/POSIX Shared Memory** and **POSIX Semaphores** to ensure starvation-free execution.

---

## 🏛️ System Architecture & Workflow Diagram

```text
               ┌─────────────────────────────────────────────────────────┐
               │                     initializer.c                       │
               │   • Allocates Shared Memory                             │
               │   • Initializes POSIX Semaphores                        │
               │   • Spawns Receptionist & Visitor Processes             │
               └───────────┬─────────────────────────────────┬───────────┘
                           │                                 │
                           ▼                                 ▼
              ┌─────────────────────────┐       ┌─────────────────────────┐
              │     receptionist.c      │       │        visitor.c        │
              │  (Bar Manager Process)  │       │    (Customer Process)   │
              └────────────┬────────────┘       └────────────┬────────────┘
                           │                                 │
                           │   ┌─────────────────────────┐   │
                           └──►│  SHARED MEMORY SEGMENT  │◄──┘
                               │  • Waiting Queue Buffer │
                               │  • Order Queue Buffer   │
                               │  • Tables State (0/-1)  │
                               │  • Global Statistics    │
                               └────────────▲────────────┘
                                            │ Reads Metrics
                               ┌────────────┴────────────┐
                               │        monitor.c        │
                               │   (Real-time Observer)  │
                               └─────────────────────────┘
```

## 📁 Structure & Component Breakdown

* **`initializer.c` (Master Coordinator)**
  * **IPC Initialization**: Allocates shared memory segments and initializes required POSIX semaphores.
  * **Process Spawning**: Creates 1 `receptionist` process and 100 concurrent `visitor` processes (configurable via the `VISITORS` macro) using `fork()` and `execvp()`.
  * **CLI Parsing**: Reads runtime flags (`-r` order time, `-s` shared memory name, `-v` visitor rest time).
  * **Lifecycle & Cleanup**: Waits for all visitors to complete service, then unlinks/frees shared memory and destroys all semaphores upon program termination.

* **`receptionist.c` (Bar Manager)**
  * **Order Processing**: Blocks on semaphore signals (`receptionist_access`), fetches orders from the shared order buffer, and simulates order preparation over a random interval within `[0.5 * order_time, order_time]`.
  * **Progress Tracking**: Compares served visitors against total queued customers in the waiting buffer until all visitors are processed.

* **`visitor.c` (Customer Client)**
  * **Queueing & Table Allocation**: Enters the waiting circular buffer upon arrival, searches across 3 tables (4 chairs each) for empty slots, and queues in the order buffer when seated.
  * **Batch Table Reset Synchronization**: If all chairs are occupied, blocks on `table_reset` semaphore until a table fully empties (all 4 seats marked `-1`), avoiding partial seat allocation and starvation.
  * **Rest & Logging**: Simulates eating/drinking for a random duration, marks chair exit, triggers table reset signals when a table is completely vacated, and calls `log_event()` to append state updates to `bar_log.txt`.

* **`segment.c` / `segment.h` (IPC Utilities & Data Structures)**
  * **Shared Memory Layout**: Contains C structure definitions (`structs`) for shared state, circular buffers, table tracking, and statistics.
  * **Circular Buffer Operations**: Implements synchronized thread-safe push/pop operations for both the waiting queue and order queue.

* **`monitor.c` (Real-Time Observer)**
  * **Live Telemetry**: Attaches to the shared memory segment at any time to output live metrics on table occupancy, item consumption (wine, water, cheese, salad), and visit durations.
  * **Final Reporting**: Runs a final evaluation pass after program completion to print aggregate session statistics.

* **`Makefile`**
  * Provides automated compilation targets (`make`, `make run`, `make run_visitor`, `make run_monitor`, `make clean`).

---

## 🔗 Communication & Data Flow

Communication across processes relies entirely on Shared Memory and POSIX Semaphores:

* **Queue Management via Circular Buffers**
  * **Waiting Queue Buffer**: Stores arriving visitors in a FIFO circular buffer outside the bar.
  * **Order Queue Buffer**: Holds seated visitors waiting for food and drink preparation by the receptionist.

* **Synchronization & Semaphores**
  * Mutual exclusion semaphores protect shared memory access during circular buffer push/pop operations and counter updates.
  * Conditional semaphores (`receptionist_access`) wake the receptionist when a customer places an order.
  * Table coordination semaphores (`table_reset`) hold incoming visitors until an entire table has been completely vacated by all 4 previous occupants.




## 🚀 How to Run
To execute the application, first compile all project binaries using `make` and then start the main simulation process. You can monitor live statistics in a separate terminal, manually spawn extra visitors, reset historical log files before new runs, or clean up build artifacts when finished
* Compile:    `make`
* Start:      `make run`
* Monitor:    `make run_monitor`
* Add Visitors: `make run_visitor`
* Reset Logs:   `rm -f bar_log.txt`
* Clean:        `make clean`