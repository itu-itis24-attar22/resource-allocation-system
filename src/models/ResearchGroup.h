#ifndef RESEARCHGROUP_H
#define RESEARCHGROUP_H

#include "OrganizationUnit.h"

class ResearchGroup : public OrganizationUnit {
public:
    ResearchGroup(int unitId, const std::string& name);
    std::string getType() const override;
};

#endif
