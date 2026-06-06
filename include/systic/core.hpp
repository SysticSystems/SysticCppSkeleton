#pragma once

namespace systic {

    class CoreEngine {
    public:
        /**
         * @Kind Cold
         * @Complexity O(n)
         * @Tradeoffs Recursive — stack depth proportional to input. Use iterative for large n.
         * @Strategy Naive recursive descent; acceptable for small compile-time-bounded inputs.
         * @Implementation Direct recursive multiplication with base-case guard.
         */
        auto computeFactorial(int inputNum) const -> int;

        /**
         * @Kind Cold
         * @Complexity O(1)
         * @Tradeoffs None — single addition, no branching.
         * @Strategy Trivial arithmetic; exists as a skeleton placeholder.
         * @Implementation Direct scalar addition of two parameters.
         */
        static auto computeSum(int firstVal, int secondVal) -> int;
    };

} // namespace systic