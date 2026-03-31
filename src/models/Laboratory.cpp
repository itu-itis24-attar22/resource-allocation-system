#include "Laboratory.h"

Laboratory::Laboratory(int id, const std::string& name, int capacity,
                       bool hasProjector, bool hasWhiteboard, bool hasComputers,
                       bool isAvailable)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers, isAvailable) {}

std::string Laboratory::getType() const {
    return "Laboratory";
}