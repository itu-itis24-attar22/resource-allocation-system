#include "Classroom.h"

Classroom::Classroom(int id, const std::string& name, int capacity)
    : Space(id, name, capacity) {}

std::string Classroom::getType() const {
    return "Classroom";
}