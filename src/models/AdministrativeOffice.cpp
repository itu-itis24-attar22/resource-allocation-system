#include "AdministrativeOffice.h"

AdministrativeOffice::AdministrativeOffice(int unitId, const std::string& name)
    : OrganizationUnit(unitId, name, "AdministrativeOffice") {}

std::string AdministrativeOffice::getType() const {
    return "AdministrativeOffice";
}
