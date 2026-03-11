#include "Space.h"

Space::Space(int id, const std::string& name, int capacity)
    : id(id), name(name), capacity(capacity) {}

int Space::getId() const {
    return id;
}

std::string Space::getName() const {
    return name;
}

int Space::getCapacity() const {
    return capacity;
}