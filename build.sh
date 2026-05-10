#!/bin/bash
# ============================================================================
# Adaptive Build Script - Multi-Phase OCI/C++ Pipeline
# ============================================================================

set -e

# --- Configuration ---
# Set your project name here, or it will attempt to auto-detect from CMakeLists.txt
PROJECT_NAME="systic"

if [ -f "CMakeLists.txt" ]; then
    # Auto-extract project name if variable is empty or generic
    DETECTED_NAME=$(grep -m 1 "project(" CMakeLists.txt | cut -d'(' -f2 | cut -d' ' -f1 | tr -d ')' | tr '[:upper:]' '[:lower:]')
    PROJECT_NAME=${PROJECT_NAME:-$DETECTED_NAME}
fi

# 1. Parse Phase
PHASE=$(echo "${1:-Release}" | awk '{print toupper(substr($0,1,1))tolower(substr($0,2))}')

case "$PHASE" in
    "Debug")
        OUT_DIR=".build-debug"
        CMAKE_MODE="Debug"
        EXTRA_FLAGS="-g -O0"
        ;;
    "Testing")
        OUT_DIR=".build-test"
        CMAKE_MODE="RelWithDebInfo" # Performance + Symbols for profiling
        EXTRA_FLAGS="-O3 -g -DSYSTIC_FULL_ASSERT"
        ;;
    "Release")
        OUT_DIR=".build"
        CMAKE_MODE="Release"
        # O3 + Frame pointer omission for that sub-30ns mechanical sympathy
        EXTRA_FLAGS="-O3 -fomit-frame-pointer -DNDEBUG"
        ;;
    *)
        echo "❌ Unknown phase: $PHASE. Use Debug, Testing, or Release."
        exit 1
        ;;
esac

# Compiler Detection (Prioritizing Clang 21 for modern C++ features)
if [ -z "$CC" ] || [ -z "$CXX" ]; then
    if command -v clang-21 &> /dev/null; then
        export CC="clang-21"
        export CXX="clang++-21"
    else
        export CC="clang"
        export CXX="clang++"
    fi
fi

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

log_step() { echo -e "\n${MAGENTA}${BOLD}▶ PHASE: $PHASE | PROJECT: $PROJECT_NAME${NC}\n${MAGENTA}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"; }
log_info() { echo -e "${BLUE}[INFO]${NC} $1"; }

log_step "Step 1: Preparing directory [$OUT_DIR]"
mkdir -p "$OUT_DIR"
rm -f "$OUT_DIR/CMakeCache.txt"

log_info "Step 2: Conan install ($CMAKE_MODE)"
conan install . -of "$OUT_DIR" --build=missing \
     -pr:b=./.conan/profiles/clang21 -pr:h=./.conan/profiles/clang21 \
     -s build_type="$CMAKE_MODE" \
     -g CMakeDeps -g CMakeToolchain

# Source VirtualBuildEnv to inject Conan's cmake and ninja into our PATH
if [ -f "$OUT_DIR/conanbuild.sh" ]; then
    source "$OUT_DIR/conanbuild.sh"
fi

if command -v ninja &> /dev/null || command -v ninja-build &> /dev/null; then
    GENERATOR="Ninja"
else
    GENERATOR="Unix Makefiles"
fi

log_info "Step 3: CMake Configure ($GENERATOR)"
cmake -G "$GENERATOR" -S . -B "$OUT_DIR" \
     -DCMAKE_TOOLCHAIN_FILE="$OUT_DIR/conan_toolchain.cmake" \
     -DCMAKE_BUILD_TYPE="$CMAKE_MODE" \
     -DCMAKE_CXX_COMPILER="$CXX" \
     -DCMAKE_C_COMPILER="$CC" \
     -DCMAKE_CXX_FLAGS="$EXTRA_FLAGS"

log_info "Step 4: Compiling"
cmake --build "$OUT_DIR" -j$(nproc)

# --- Pipeline Handshake ---
if [ "$PHASE" == "Testing" ]; then
    log_step "Step 5: Execution & Artifact Handshake"

    ROOT_DIR=$(pwd)

    # Surgical binary lookup based on your specific CMake target names
    # We look for 'systic_tests' and 'systic_bench' specifically
    TEST_BIN=$(find "$OUT_DIR" -name "systic_tests" -type f -executable | head -n 1)
    BENCH_BIN=$(find "$OUT_DIR" -name "systic_bench" -type f -executable | head -n 1)

    # 1. Run Tests
    if [ -n "$TEST_BIN" ]; then
        log_info "Running Tests: $TEST_BIN"
        # We run from ROOT_DIR so the XML is dropped exactly where the CI/User expects it
        cd "$ROOT_DIR"
        "$TEST_BIN" --gtest_output="xml:$ROOT_DIR/test_results.xml" || echo -e "${RED}[!] Tests failed${NC}"
    else
        echo -e "${RED}[WARN]${NC} Target 'systic_tests' not found in $OUT_DIR"
    fi

    # 2. Run Benchmarks
    if [ -n "$BENCH_BIN" ]; then
        log_info "Running Benchmarks: $BENCH_BIN"
        cd "$ROOT_DIR"
        "$BENCH_BIN" --benchmark_out="$ROOT_DIR/bench_results.json" --benchmark_out_format=json || echo -e "${RED}[!] Benchmarks failed${NC}"
    else
        echo -e "${RED}[WARN]${NC} Target 'systic_bench' not found in $OUT_DIR"
    fi

    # Final Validation
    echo -e "\n${CYAN}--- Artifact Verification ---${NC}"
    [ -f "$ROOT_DIR/test_results.xml" ] && echo -e "  [PASS] test_results.xml" || echo -e "  ${RED}[FAIL] test_results.xml missing${NC}"
    [ -f "$ROOT_DIR/bench_results.json" ] && echo -e "  [PASS] bench_results.json" || echo -e "  ${RED}[FAIL] bench_results.json missing${NC}"
fi

echo -e "\n${GREEN}${BOLD}✅ $PROJECT_NAME [$PHASE] READY!${NC}"