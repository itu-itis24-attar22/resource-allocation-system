#include "Laboratory.h"

Laboratory::Laboratory(int id, const std::string& name, int capacity,
                       bool hasProjector, bool hasWhiteboard, bool hasComputers)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers) {}

std::string Laboratory::getType() const {
    return "Laboratory";
}