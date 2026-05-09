#include "Club.h"

Club::Club(int unitId, const std::string& name)
    : OrganizationUnit(unitId, name, "Club") {}

std::string Club::getType() const {
    return "Club";
}
