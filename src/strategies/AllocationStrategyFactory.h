#ifndef ALLOCATIONSTRATEGYFACTORY_H
#define ALLOCATIONSTRATEGYFACTORY_H

#include <string>
#include "IAllocationStrategy.h"
#include "GreedyAllocationStrategy.h"
#include "PriorityAllocationStrategy.h"
#include "MultiRoomExamGreedyStrategy.h"
#include "MultiRoomExamBestFitStrategy.h"
#include "SharedRoomExamBestFitStrategy.h"

class AllocationStrategyFactory {
private:
    GreedyAllocationStrategy greedyStrategy;
    PriorityAllocationStrategy priorityStrategy;
    MultiRoomExamGreedyStrategy multiRoomExamGreedyStrategy;
    MultiRoomExamBestFitStrategy multiRoomExamBestFitStrategy;
    SharedRoomExamBestFitStrategy sharedRoomExamBestFitStrategy;

    AllocationStrategyFactory() = default;

public:
    AllocationStrategyFactory(const AllocationStrategyFactory&) = delete;
    AllocationStrategyFactory& operator=(const AllocationStrategyFactory&) = delete;

    static AllocationStrategyFactory& getInstance();

    const IAllocationStrategy* getStrategy(const std::string& strategyName) const;
};

#endif
