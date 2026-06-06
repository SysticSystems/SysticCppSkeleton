#include "systic/core.hpp"

namespace systic {

    auto CoreEngine::computeFactorial(int inputNum) const -> int { // NOLINT(misc-no-recursion)
        if (inputNum < 0) {
            return -1;
        }
        if (inputNum <= 1) {
            return 1;
        }
        return inputNum * computeFactorial(inputNum - 1);
    }

    auto CoreEngine::computeSum(int firstVal, int secondVal) -> int {
        return firstVal + secondVal;
    }

} // namespace systic