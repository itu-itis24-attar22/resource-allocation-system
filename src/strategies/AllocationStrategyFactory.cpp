#include "AllocationStrategyFactory.h"
#include <cctype>

namespace {
    std::string normalizeStrategyName(const std::string& strategyName) {
        std::string normalized;

        for (char ch : strategyName) {
            normalized += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        return normalized;
    }
}

AllocationStrategyFactory& AllocationStrategyFactory::getInstance() {
    static AllocationStrategyFactory instance;
    return instance;
}

const IAllocationStrategy* AllocationStrategyFactory::getStrategy(const std::string& strategyName) const {
    std::string normalizedName = normalizeStrategyName(strategyName);

    if (normalizedName.empty() || normalizedName == "greedy") {
        return &greedyStrategy;
    }

    return &greedyStrategy;
}
