#ifndef ALLOCATIONSTRATEGYFACTORY_H
#define ALLOCATIONSTRATEGYFACTORY_H

#include <string>
#include "IAllocationStrategy.h"
#include "GreedyAllocationStrategy.h"
#include "PriorityAllocationStrategy.h"
#include "MultiRoomExamGreedyStrategy.h"
#include "MultiRoomExamBestFitStrategy.h"

class AllocationStrategyFactory {
private:
    GreedyAllocationStrategy greedyStrategy;
    PriorityAllocationStrategy priorityStrategy;
    MultiRoomExamGreedyStrategy multiRoomExamGreedyStrategy;
    MultiRoomExamBestFitStrategy multiRoomExamBestFitStrategy;

    AllocationStrategyFactory() = default;

public:
    AllocationStrategyFactory(const AllocationStrategyFactory&) = delete;
    AllocationStrategyFactory& operator=(const AllocationStrategyFactory&) = delete;

    static AllocationStrategyFactory& getInstance();

    const IAllocationStrategy* getStrategy(const std::string& strategyName) const;
};

#endif
