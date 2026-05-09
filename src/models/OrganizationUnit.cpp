#include "OrganizationUnit.h"

OrganizationUnit::OrganizationUnit(int unitId,
                                   const std::string& name,
                                   const std::string& unitType)
    : unitId(unitId), name(name), unitType(unitType) {}

int OrganizationUnit::getId() const {
    return unitId;
}

std::string OrganizationUnit::getName() const {
    return name;
}

std::string OrganizationUnit::getUnitType() const {
    return unitType;
}

std::string OrganizationUnit::getType() const {
    return unitType;
}
