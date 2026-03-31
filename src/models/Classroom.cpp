#include "Classroom.h"

Classroom::Classroom(int id, const std::string& name, int capacity,
                     bool hasProjector, bool hasWhiteboard, bool hasComputers)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers) {}

std::string Classroom::getType() const {
    return "Classroom";
}