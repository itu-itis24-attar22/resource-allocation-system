#include "Laboratory.h"

Laboratory::Laboratory(int id, const std::string& name, int capacity,
                       bool hasProjector, bool hasWhiteboard, bool hasComputers,
                       bool isAvailable, const std::string& building)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers, isAvailable, building) {}

std::string Laboratory::getType() const {
    return "Laboratory";
}