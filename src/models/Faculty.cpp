#include "Faculty.h"

Faculty::Faculty(int unitId, const std::string& name)
    : OrganizationUnit(unitId, name, "Faculty") {}

std::string Faculty::getType() const {
    return "Faculty";
}
