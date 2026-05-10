# Systic C++ Skeleton

A minimalist C++23 project substrate designed for high-performance libraries and applications. It prioritizes strict toolchain enforcement, integrated verification, and total CI reproducibility orchestrated via Conan on Alpine Edge.

## Core Characteristics

* **Compiler Handshake:** Strictly enforces the Clang 21 toolchain and C++23 standard, ensuring language compliance and deterministic binary generation.
* **Infrastructure Sovereignty:** Utilizes Conan 2.0 to provision exact `cmake` and `ninja` orchestrators, eliminating host OS mismatches and ensuring environment parity.
* **Integrated Verification:** Natively wired with GoogleTest and Google Benchmark to facilitate immediate logic validation and instruction-level performance tracking.
* **Zero-Pollution CI:** Optimized for execution within an Alpine Edge container, ensuring total reproducibility across local and remote development environments.

## Project Layout

| Directory   | Purpose |
|-------------|---------|
| `include/`  | Public API and zero-cost abstraction headers. |
| `src/`      | Primary implementation logic. |
| `tests/`    | Unit and integration testing suite via GTest. |
| `bench/`    | Performance regression tracking via GBenchmark. |
| `.conan/`   | Local hardware profiles and dependency definitions. |

## Execution Handshake
Run the adaptive pipeline wrapper to execute a specific phase:
```bash
./build.sh Debug    # Compiles with symbol emission and zero optimization (-g -O0)
./build.sh Testing  # Fast compilation and execution of GTest/GBenchmark suites
./build.sh Release  # Maximum performance execution (-O3 -fomit-frame-pointer)
```
