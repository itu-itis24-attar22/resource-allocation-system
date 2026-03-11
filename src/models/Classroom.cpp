#include "Classroom.h"

Classroom::Classroom(int id, const std::string& name, int capacity)
    : id(id), name(name), capacity(capacity) {}

int Classroom::getId() const {
    return id;
}

std::string Classroom::getName() const {
    return name;
}

int Classroom::getCapacity() const {
    return capacity;
}