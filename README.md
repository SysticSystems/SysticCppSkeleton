# Systic C++ Skeleton

A starting point for C++23 projects where build reproducibility and honest benchmark numbers matter. Clone it, rename things, write code.

---

## What this is

A template repository that wires up the build toolchain, test framework, and benchmark harness so you don't have to. It gives you a working project structure, a containerised build pipeline, and a CI workflow that publishes real test and benchmark results — nothing more.

It does not impose an architecture, a logging strategy, a memory allocator, or any application-level opinions. Those are your problem.

---

## Toolchain constraints

Using this skeleton ties you to the following:

| Concern | Choice |
|---|---|
| Language standard | C++23 (enforced by Conan) |
| Compiler | Clang 21 |
| Dependency manager | Conan 2.0 |
| Build system | CMake + Ninja (system-provided via Alpine) |
| Unit / integration tests | GoogleTest 1.14.0 |
| Benchmarks | Google Benchmark 1.8.3 |
| Build container base | Alpine Edge (musl libc) |

Changing the compiler or standard means updating both the Conan profile under `.conan/profiles/` and the `configure()` block in `conanfile.py`. Everything else is replaceable.

---

## Project layout

```
include/    public headers
src/        implementation
tests/      GTest suites
bench/      Google Benchmark cases
.conan/     Conan hardware profiles
.github/    CI workflow definitions
```

---

## Building locally

Docker is required. The `run-build.sh` script manages the container lifecycle and maps build artifacts back to the host.

```sh
./run-build.sh Dev        # build + compile-db for IDE indexing
./run-build.sh Testing    # build, run tests, run benchmarks
./run-build.sh Release    # optimised build
```

If you want to build directly on the host, install Conan 2, CMake, Ninja, and Clang 21, then run `build.sh`.

---

## CI: test and benchmark results

The GitHub Actions workflow runs on a **bare-metal self-hosted runner**. This is intentional — shared cloud runners introduce scheduling jitter and variable CPU states that make benchmark numbers meaningless. Results from a real machine are surfaced directly in the Actions summary of each run: test pass/fail counts and benchmark timings are posted there so regressions are visible without digging through logs.

> If you fork this and use GitHub-hosted runners, benchmark numbers will not be comparable across runs. Use them only as smoke tests until you wire up a bare-metal runner of your own.

---

## Things not covered (you may want to add)

- **License** — none is declared. Add one before making the repo public.
- **Conan profile for your hardware** — the profile in `.conan/profiles/` targets a specific arch. Copy and adapt it before building on a different machine.
- **Sanitiser builds** — no ASan/UBSan/TSan targets exist yet. Worth adding as a fourth build mode.
- **Static analysis** — no `clang-tidy` or `clang-format` config is included.
- **How to rename the project** — after cloning, you need to update `conanfile.py` (`name`, `version`), `CMakeLists.txt`, and `.env`. A short sed one-liner or rename script here would save time.
- **Minimum host requirements** — Docker version, available disk space for the Alpine image and build cache.
