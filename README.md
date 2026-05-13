# Systic C++ Project Skeleton

A minimalist **C++23** project substrate designed for high-performance libraries and applications. It prioritizes strict toolchain enforcement, hardware-aligned determinism, and total CI reproducibility orchestrated via **Conan 2.0** on **Alpine Edge**.

## Core Characteristics

*   **Compiler Handshake:** Strictly enforces the **Clang 21** toolchain and C++23 standard, ensuring language compliance and deterministic binary generation.
*   **Infrastructure Sovereignty:** Utilizes Conan 2.0 to provision exact `cmake` and `ninja` orchestrators, eliminating host OS mismatches and ensuring environment parity.
*   **Integrated Verification:** Natively wired with **GoogleTest** and **Google Benchmark** to facilitate immediate logic validation and CPU-cache-level performance tracking.
*   **Zero-Pollution CI:** Optimized for execution within an OCI container, ensuring that build dependencies never "leak" into your host machine's filesystem.

## Project Layout

| Directory | Purpose |
| :--- | :--- |
| `include/` | Public API and zero-cost abstraction headers. |
| `src/` | Primary implementation logic. |
| `tests/` | Unit and integration testing suite via GTest. |
| `bench/` | Performance regression tracking via GBenchmark. |
| `.conan/` | Local hardware profiles and dependency definitions. |

## Build & Execution

### 🚀 Recommended: The Sovereign Path (Dockerized)
To maintain a "clean room" host environment and ensure 100% build reproducibility, it is **strongly recommended** to use the OCI orchestrator (`run-build.sh`). This script automatically handles the image and container lifecycle, mounting your source code and mapping artifacts back to your host.

```bash
./run-build.sh Dev      # Recommended for rapid iteration and CLion indexing
./run-build.sh Testing  # Runs GTest/GBenchmark and exports reports to host
./run-build.sh Release  # Produces final high-performance artifacts