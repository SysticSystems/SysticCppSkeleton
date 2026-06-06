// =============================================================================
// Systic Protocol — Naming, Formatting & Architectural Reference     v2.0
// Reflects the final agreed conventions enforced by toolchain pipelines.
// This file is the ground truth. When in doubt, come back here.
// =============================================================================

#include <cstddef>
#include <span>
#include <vector>

namespace systic { // ✅ NAMESPACES: Pure lower_case (No camelCase, No underscores)

// =============================================================================
// 1. CLASSES & INTERFACES — PascalCase
// =============================================================================

class RingBuffer {
public:
    /**
     * @Kind Hot
     * @Complexity O(1)
     * @Tradeoffs Sacrifices contiguous virtual space for lock-free multi-threaded throughput.
     * @Strategy Mechanical sympathy via twisted bit-scanning index masking to avoid cache misses.
     * @Implementation Direct array slot indexing via atomic memory sequence ordering.
     */
    inline void insert(int elementValue) { // ✅ FUNCTIONS: camelCase. Opening brace on same line.
        int writeIndex = currentHead % bufferCapacity; // ✅ VARIABLES: camelCase, minimum 3 chars.
        data[writeIndex] = elementValue;
        ++currentSize;
    }

    /**
     * @Kind Cold
     * @Complexity O(1)
     * @Tradeoffs Evaluates direct members; execution path scale is entirely static.
     * @Strategy Standard state retrieval API; non-allocating.
     * @Implementation Inline lookup of tracking member variables.
     */
    inline int getSize() const {
        return currentSize;
    }

private: // ✅ MEMBERS: camelCase, minimum 3 chars.
    int  currentHead    = 0;
    int  currentSize    = 0;
    int  bufferCapacity = 0;
    int* data           = nullptr;
};

// ❌ REJECTED — snake_case or missing architectural documentation block
// class ring_buffer { };

// =============================================================================
// 2. ENUMS — PascalCase + mandatory 'Enum' suffix
//            Values: pure UPPER_CASE, no K_ prefix
// =============================================================================

/**
 * @Kind Cold
 * @Complexity N/A
 * @Tradeoffs Explicit domain definition mapping; bounds layout tracking.
 * @Strategy Enum abstraction enforcing unambiguous layout definitions.
 * @Implementation Strongly typed enum backing.
 */
enum class MemoryLayoutEnum {
    LINEAR_CONTIGUOUS,
    RING_MAPPED,
    SPARSE_PAGED
};

// ❌ REJECTED — missing 'Enum' suffix or lowerCase values
// enum class MemoryLayout { linearContiguous };

// =============================================================================
// 3. CONSTANTS & CONSTEXPR — K_ prefix + UPPER_CASE
//    K_ = "baked-in compile-time value"
// =============================================================================

constexpr int K_MAX_BUFFER_SIZE  = 1024;
constexpr int K_DEFAULT_CAPACITY = 64;

// ❌ REJECTED — missing K_ prefix or lower camelCase
// constexpr int maxBufferSize = 1024;

// =============================================================================
// 4. TEMPLATE PARAMETERS — pure UPPER_CASE, minimum 4 chars.
//    - Type parameters (typename): No K_ prefix ("compile-time type hole")
//    - Value parameters (integral types): Mandatory K_ prefix ("baked-in value")
// =============================================================================

template <
    typename ELEMENT_TYPE,    // ✅ Type abstraction (No K_)
    typename ALLOCATOR_TYPE,  // ✅ Type abstraction (No K_)
    int      K_INIT_CAPACITY> // ✅ Non-type value parameter (Mandatory K_)
class TypedStack {
public:
    /**
     * @Kind Hot
     * @Complexity O(1)
     * @Tradeoffs Inlined optimization bounds checking via static array mapping.
     * @Strategy Continuous stack tracking, keeping components local to the active core L1 cache.
     * @Implementation Directly indexes pre-allocated stack storage block.
     */
    inline void push(ELEMENT_TYPE newElement) {
        storage[++topIndex] = newElement;
    }

private:
    ELEMENT_TYPE storage[K_INIT_CAPACITY];
    int          topIndex = 0;
};

// ❌ REJECTED — Single-character or short template parameters
// template <typename T, int N> class Stack { };

// =============================================================================
// 5. LOOP COUNTERS — camelCase, minimum 3 chars
// =============================================================================

inline void processSpans(int* inputBuffer, int bufferLength) {
    // ✅ CORRECT loop counter (3-character minimum satisfied)
    for (int idx = 0; idx < bufferLength; ++idx) {
        inputBuffer[idx] *= 2;
    }

    // ❌ REJECTED — Under 3 characters (Will crash the pipeline check)
    // for (int i = 0; i < bufferLength; ++i) { }
}

// =============================================================================
// 6. TYPE ALIASES — PascalCase
// =============================================================================

using ByteSpan    = std::span<std::byte>;
using ElementList = std::vector<int>;

// ❌ REJECTED — snake_case type alias
// using byte_span = std::span<std::byte>;

} // namespace systic