#include "Laboratory.h"

Laboratory::Laboratory(int id, const std::string& name, int capacity)
    : Space(id, name, capacity) {}

std::string Laboratory::getType() const {
    return "Laboratory";
}