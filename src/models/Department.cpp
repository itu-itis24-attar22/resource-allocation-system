#include "Department.h"

Department::Department(int unitId, const std::string& name)
    : OrganizationUnit(unitId, name, "Department") {}

std::string Department::getType() const {
    return "Department";
}
