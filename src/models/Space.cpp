#include "Space.h"

Space::Space(int id, const std::string& name, int capacity,
             bool hasProjector, bool hasWhiteboard, bool hasComputers,
             bool isAvailable, const std::string& building)
    : id(id),
      name(name),
      capacity(capacity),
      hasProjector(hasProjector),
      hasWhiteboard(hasWhiteboard),
      hasComputers(hasComputers),
      isAvailable(isAvailable),
      building(building) {}

int Space::getId() const {
    return id;
}

std::string Space::getName() const {
    return name;
}

int Space::getCapacity() const {
    return capacity;
}

bool Space::getHasProjector() const {
    return hasProjector;
}

bool Space::getHasWhiteboard() const {
    return hasWhiteboard;
}

bool Space::getHasComputers() const {
    return hasComputers;
}

bool Space::getIsAvailable() const {
    return isAvailable;
}

void Space::setAvailability(bool availability) {
    isAvailable = availability;
}

std::string Space::getBuilding() const {
    return building;
}

bool Space::hasFeature(const std::string& feature) const {
    if (feature.empty()) return true;
    if (feature == "Projector") return hasProjector;
    if (feature == "Whiteboard") return hasWhiteboard;
    if (feature == "Computers") return hasComputers;
    return false;
}