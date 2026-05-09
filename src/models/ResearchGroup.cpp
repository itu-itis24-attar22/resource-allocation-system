#include "ResearchGroup.h"

ResearchGroup::ResearchGroup(int unitId, const std::string& name)
    : OrganizationUnit(unitId, name, "ResearchGroup") {}

std::string ResearchGroup::getType() const {
    return "ResearchGroup";
}
