# NIT Calicut — B.Tech CSE: Operating Systems Assignment

This repository contains my low-level programming implementations for the Operating Systems course (Winter 2026) at NIT Calicut. The projects span custom Linux kernel-space modules and user-space multithreaded performance optimization.

**Author:** Kona Bhuvaneswar Reddy  
**Roll Number:** B240214CS

---

## 📂 Repository Architecture

```text
Operating_Systems_Assignment/
├── KONA_B240214CS Assignment 1/     # Linux Character Device Driver (Kernel Space)
│   └── KONA_B240214CS/
│       ├── main.c                    # Driver initialization & cleanup routine
│       ├── device_file.c / .h        # Low-level file operations & system calls
│       ├── Makefile                  # Kernel build automation script
│       ├── README.pdf                # Compilation & deployment workflow
│       └── REPORT.pdf                # Technical architecture report
│
├── KONA_B240214CS Assignment 2/     # Parallel Multithreaded Computing (User Space)
│   └── KONA_B240214CS/
│       ├── B240214CS_A2.c            # Multithreaded engine utilizing POSIX threads
│       ├── B240214CS_Design_Document.pdf # Workload distribution & locking logic
│       ├── B240214CS_README.pdf      # Compilation flags & execution manual
│       └── B240214CS_SS.png          # Performance execution screenshot
│
├── Operating Systems Assignment 1.pdf # Official Course Problem Statement 1
└── Operating Systems Assignment 2.pdf # Official Course Problem Statement 2

```

---

## 🛠️ Project Deep Dives

### [Assignment 1: Linux Kernel Character Device Driver](https://github.com/Kona-Bhuvan/Operating_Systems_Assignment/tree/main/KONA_B240214CS%20Assignment%201/KONA_B240214CS)

* **Domain:** Kernel-Space Programming, Device Drivers, Subsystem Interfacing
* **Core Mechanics:** Developed a custom character device driver with robust memory isolation. Implemented strict synchronization requirements enforcing a read-before-write ordering logic.
* **Key Features:** Kernel version validation handling via bitwise operations, asynchronous timer-constrained task schedules, and explicit file handling (`open`, `read`, `write`, `release`).
* **Documentation:** Refer to the local [README.pdf](https://github.com/Kona-Bhuvan/Operating_Systems_Assignment/blob/main/KONA_B240214CS%20Assignment%201/KONA_B240214CS/README.pdf) for kernel loading (`insmod`) configurations and the structural [REPORT.pdf](https://github.com/Kona-Bhuvan/Operating_Systems_Assignment/blob/main/KONA_B240214CS%20Assignment%201/KONA_B240214CS/REPORT.pdf).

### [Assignment 2: Parallel Perfect Number Verifier](https://github.com/Kona-Bhuvan/Operating_Systems_Assignment/tree/main/KONA_B240214CS%20Assignment%202/KONA_B240214CS)

* **Domain:** User-Space Concurrency, Parallel Computing, Synchronization
* **Core Mechanics:** Built a high-performance mathematical verification application that leverages parallel processing across multiple worker threads.
* **Key Features:** Engineered a rigorous mathematical workload distribution mechanism using the **POSIX Threads (pthreads)** library. Utilized `pthread_mutex` primitives to secure critical sections against data races.
* **Documentation:** Full implementation specifications are detailed in the [Design Document](https://github.com/Kona-Bhuvan/Operating_Systems_Assignment/blob/main/KONA_B240214CS%20Assignment%202/KONA_B240214CS/B240214CS_Design_Document.pdf).

---

## 🧰 Environment & Tools

* **Language Standards:** C (GNU C99 / GNU C11)
* **Kernel Frameworks:** Linux Kernel Module API (`linux/init.h`, `linux/module.h`, `linux/fs.h`)
* **Threading API:** POSIX Threads (`pthread.h`)
* **Build Automation:** GNU Make
